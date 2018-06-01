// Jeu du Simon sur la console fran�aise META
// site: www.gamebuino.com
// Version 1.0

#include <Gamebuino-Meta.h>
#include <D:\Jean-Charles\META\Simon\Graphiques.h>  //import des graphiques
#include <D:\Jean-Charles\META\Simon\sons.h>  //import des graphiques

#define TOUCHE_BLEUE 1  // touche droite    
#define TOUCHE_JAUNE 2  // touche bas     
#define TOUCHE_VERTE 3  // touche gauche           
#define TOUCHE_ROUGE 4  // touche haut 

#define MAX_MULTI 8
#define timer_max 16 // Temps pour jouer une note
#define NOTESMAX 42  // Nombre maximum de notes de la mélodie + 2 à ajouter au nombre désiré
#define NBNOTES 4    // Nombre de notes possible (sur le Simon classique c'est 4 mais on pourrait faire des variantes)

// Variables du jeu
byte niveau        = 0; // nombre de notes à jouer 
byte longueur      = 0; // longueur que le joueur à joué (nombre de notes)
byte sequenceSimon[50]; 
byte sequenceJoueur[50];
byte compteur;
byte hiScore       = 0;  // le meilleur score

int  joueur        = 1; // a qui de jouer (1,2,3,4...nbJoueurs?)
int  nbJoueurs     = 2; // nombre de joueurs multijoueur (2 à 8?);
int  joueursOK[MAX_MULTI] = {0,0,0,0,0,0,0,0}; // Les joueurs encore en jeu

// Variables permettant d'afficher ou on les boutons allumés
bool AfficheBoutonRouge;
bool AfficheBoutonBleu;
bool AfficheBoutonJaune;
bool AfficheBoutonVert;

bool JeuCommence;     // Délai avant le début du jeu
bool JoueIntro;       // On joue l'intro une fois
bool Joue_Melodie;    // Actif quand l'ordi joue la mélodie
bool JoueMelodieComplete;      // Actif quand le joueur a joué toute la mélodie
bool ErreurJoueur;    // Actif en cas d'erreur du joueur

byte Echec_timer;             // Temps écoulé
byte Attente_TimerDebutJeu;
byte Attente_TimerTour;
byte SonAJouer;               // Mémorise le son à jouer et la touche à allumer puis à éteindre
byte Melodie[NOTESMAX];        // Dimensionne le tableau contenant les notes à trouver
byte NoteCourante;  //
byte EtapeMelodieValidee;     // Augmente à chaque fois que le joueur trouve la bonne note
byte Timer_Melodie;
byte Timer_attente;

// Définition des textes des popups
const MultiLang NewGamePopup[] = {
    { LANG_EN, "New game" },
    { LANG_FR, "Nouvelle partie" },
    };

const MultiLang HighPopup[] = {
    { LANG_EN, "Well done !" },
    { LANG_FR, "Bien joué !" },
    };

// Initialisation
void setup() {
  gb.begin();
  initGame();
}

// Paramètrage des valeurs en début de partie
void initGame() {
  gb.pickRandomSeed();          //Normalement, ce n'est plus utile, mais dans le doute ...
  // Créé la mélodie
  for (byte i = 0; i < (NOTESMAX - 1); i++) {
    Melodie[i - 1] = random(NBNOTES) + 1;
  }
  JeuCommence = false;
  AfficheBoutonRouge = false;
  AfficheBoutonBleu = false;
  AfficheBoutonJaune = false;
  AfficheBoutonVert = false;
  Joue_Melodie = true;            // L'ordinateur commence
  JoueMelodieComplete = false;    // Jouer la mélodie complète
  ErreurJoueur = false;           // Réinitialise l'indicateur d'erreur
  Echec_timer = 0;                // Réinitalise de marqueur de dépassement de temps
  Attente_TimerDebutJeu = 0;      // Remet le décompte du temps avant le démarrage du jeu à 0
  Attente_TimerTour = 0;
  SonAJouer = 0;                  // Aucun son à jouer
  NoteCourante = 0;//
  EtapeMelodieValidee = 1;        
  Timer_Melodie = 0;
  Timer_attente = 0;
  JoueIntro = true;
}

void loop() {
  if (gb.update()) {
    Appuie_bouton_Menu();  // L'appuie sur Menu relance le jeu pour le moment (à terme, sera remplacé par le menu d'options)

    // Attente du délai de démarrage
    if (JeuCommence == false) {
      if (JoueIntro == true) { intro(); }
      Attente_TimerDebutJeu ++;
      if (Attente_TimerDebutJeu > timer_max * 1) {
         // NoteCourante = 0; //réinitialise la note courrante de la mélodie en début de jeu
        JeuCommence = true;
      }
    }

    // Test si gagné (on a atteind le nombre de notes maximum)
    if ( EtapeMelodieValidee >= (NOTESMAX - 1) ) {
      //endRecord();
      gb.gui.popup(HighPopup, 50);
      if (JoueMelodieComplete == false) JoueMelodieComplete == true; // Delande de joueur toute la mélodie
    }
    else
    {
      // Affiche le fond d'écran
      gb.display.clear(); 
      gb.display.drawImage(1, 0, Simon);
      
      // Gestion des tours
      if ((Timer_attente == 0) && (JeuCommence == true)) {
        if (Joue_Melodie == true) {
          melodyPlaying();        // C'est l'ordinateur qui joue
        } else playerPlaying();   // C'est u tour du joueur
      }
      
      // Attente après le tour du joueur
      if (Timer_attente > 0) {
        Timer_attente ++;
        if (Timer_attente > timer_max >> 1) Timer_attente = 0;
      }
      
      // Dessine les buttons actifs
      DessineBoutons();
    }//end record
  }//end update.
}//end loop.

void melodyPlaying() {
  //MELODY DELAY
  if (Timer_Melodie == 0) {
    SonAJouer = Melodie[NoteCourante];//update the value storing the value to play. //4;
    JoueNote();//play the right sound.
    DessineBoutons();
    delay(70);
    MasqueLeBouton();//hide the right button
    NoteCourante ++; //update for next playable note.
    Timer_Melodie ++; //update delay
  }

  if (Timer_Melodie > 0) { //start delay in-between notes
    Timer_Melodie ++;
  }
  if (Timer_Melodie > timer_max) { //end delay in-between notes = check if there are other notes to play...

    if (NoteCourante >= EtapeMelodieValidee) {// = no more notes to play = player turn.
      NoteCourante = 0; //reinitialize current playing note for the player.
      Timer_Melodie = 0; // reinitialize the timer for the next time the CPU will play the extended melody.
      Joue_Melodie = false; // = player turn
    }
    else {// = there are MORE notes to play until the currentnote hit the step limit.
      Timer_Melodie = 0;// loop.
    }
  } //delay after notes played by CPU.
}


//Masque le bouton
void MasqueLeBouton() {
  switch (SonAJouer) {
    case 1:
      AfficheBoutonBleu = false;
      break;
    case 2:
      AfficheBoutonJaune = false;
      break;
    case 3:
      AfficheBoutonVert = false;
      break;
    case 4:
      AfficheBoutonRouge = false;
      break;
  }
}


//Affiche les boutons
void DessineBoutons() {
  if (AfficheBoutonRouge == true) {
    gb.display.drawImage( 17, 3 , rouge_allume);
  }
  if (AfficheBoutonBleu == true) {
   gb.display.drawImage( 41,12 , bleu_allume);
  }
  if (AfficheBoutonJaune == true) {
   gb.display.drawImage( 14,38 , jaune_allume);
  }
  if (AfficheBoutonVert == true) {
    gb.display.drawImage(  6,13 , vert_allume);
  }
 
  //Score actuel
  gb.display.cursorX = 70;  gb.display.cursorY = 10;
  gb.display.setColor(BLACK);
  gb.display.print(EtapeMelodieValidee - 1);
}


// Gestion du tour du joueur
void playerPlaying() {
  
  // Gestion des touches appuyées
  if (Attente_TimerTour == 0) {
    
    if (gb.buttons.pressed(BUTTON_UP)) {
      Attente_TimerTour = 1;
      SonAJouer = TOUCHE_ROUGE;
      JoueNote(); 
    }
    if (gb.buttons.pressed(BUTTON_RIGHT)) {
      Attente_TimerTour = 1;
      SonAJouer = TOUCHE_BLEUE;
      JoueNote();
    }
    if (gb.buttons.pressed(BUTTON_DOWN)) {
      Attente_TimerTour = 1;
      SonAJouer = TOUCHE_JAUNE;
      JoueNote(); 
    }
    if (gb.buttons.pressed(BUTTON_LEFT)) {
      Attente_TimerTour = 1;
      SonAJouer = TOUCHE_VERTE;
      JoueNote();
    }
    DessineBoutons();
    delay(25);
  }

  // Attente après que le joueur ait sélectionné une touche
  if (Attente_TimerTour > 0) {
    Attente_TimerTour ++;
    MasqueLeBouton();
    if (Attente_TimerTour > (timer_max >> 1)) { //Délai dépassé...

      // Traitement d'une erreur de touche
      if (SonAJouer != Melodie[NoteCourante] ) {
        ErreurJoueur = true;
        Erreur_Joueur();
      }
      else  
      {
        NoteCourante ++;
        Timer_Melodie = 1;
        Attente_TimerTour = 0;
        if (NoteCourante >= EtapeMelodieValidee) { // Si le joueur a joué toutes les notes jusqu'à la position actuelle, on continue
          NoteCourante = 0;                        // Réinitialize la note à jouer par l'ordinateur au dbut de la mélodie.
          Timer_Melodie = 0;                       // Rinitialize le timer de la mélodie.
          Joue_Melodie = true;                     // Passe au tour de l'ordinateur
          EtapeMelodieValidee ++;                  // Valide la note actuelle comme étape franchie
          Timer_attente = 1;                         
        }//fin du test de validation de la note
      }//fin du traitement du test d'ereur de la note jouée.
    }//Fin du traitement du test de dépassement du délai
  }//Fin du test du délai entre 2 notes
}//Fin de la gestion du tour du joueur


// Gestion de la touche Menu => Reset: relance le jeu
void Appuie_bouton_Menu() {
  if ((gb.buttons.pressed(BUTTON_MENU))) { //|| (ErreurJoueur == true)) {
    initGame();
    ErreurJoueur = false;
  }
}


// Traitement d'une erreur du joueur
void Erreur_Joueur() {
  Echec_timer ++;
  SonAJouer = 5;//gb.sound.playCancel(); //sfx
  if (Echec_timer <= 1) JoueNote(); //play sfx soundFail();
  MasqueLeBouton();
  if (Echec_timer > timer_max) { //delay ended...
    initGame();
  }
}




