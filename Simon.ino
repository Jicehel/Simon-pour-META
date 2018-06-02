// Jeu du Simon sur la console française META
// site: www.gamebuino.com
// Version 1.1

#include <Gamebuino-Meta.h>
#include <D:\Jean-Charles\META\Simon\Graphiques.h>  //import des graphiques
#include <D:\Jean-Charles\META\Simon\Sons.h>  //import des graphiques

#define TOUCHE_BLEUE 1  // touche droite    
#define TOUCHE_JAUNE 2  // touche bas     
#define TOUCHE_VERTE 3  // touche gauche           
#define TOUCHE_ROUGE 4  // touche haut 

#define MAX_MULTI 8
#define timer_max 16 // Temps pour jouer une note
#define NOTESMAX 42  // Nombre maximum de notes de la mélodie + 2 à ajouter au nombre désiré
#define NBNOTES 4    // Nombre de notes possible (sur le Simon classique c'est 4 mais on pourrait faire des variantes)

// Definition des valeurs pour le tour à jouer
#define Joueur false
#define Ordinateur true

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
bool AQuiDeJouer;    // Actif quand l'ordi joue la mélodie
bool JoueMelodieComplete;      // Actif quand le joueur a joué toute la mélodie
bool ErreurJoueur;    // Actif en cas d'erreur du joueur

byte Echec_timer;             // Temps écoulé
byte Attente_TimerDebutJeu;
byte Attente_TimerTour;
byte SonAJouer;               // Mémorise le son à jouer et la touche à allumer puis à éteindre
byte Melodie[NOTESMAX];       // Dimensionne le tableau contenant les notes à trouver
byte NoteCourante;            // Indicateur de position dans la mélodie de la note à jouer 
byte EtapeMelodieValidee;     // Augmente à chaque fois que le joueur trouve la bonne note
byte AttenteProchaineNoteOrdinateur;
byte AttenteNote;

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
  AQuiDeJouer = Ordinateur;       // L'ordinateur commence
  JoueMelodieComplete = false;    // Jouer la mélodie complète
  ErreurJoueur = false;           // Réinitialise l'indicateur d'erreur
  Echec_timer = 0;                // Réinitalise de marqueur de dépassement de temps
  Attente_TimerDebutJeu = 0;      // Remet le décompte du temps avant le démarrage du jeu à 0
  Attente_TimerTour = 0;
  SonAJouer = 0;                  // Aucun son à jouer
  NoteCourante = 0;//
  EtapeMelodieValidee = 1;        
  AttenteProchaineNoteOrdinateur = 0;
  AttenteNote = 0;
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
         // NoteCourante = 0; // Réinitialise la note courrante de la mélodie en début de jeu
        JeuCommence = true;
      }
    }

    // Test si le joueur a gagné (on a atteind le nombre de notes maximum)
    if ( EtapeMelodieValidee >= (NOTESMAX - 1) ) {
      //endRecord();
      gb.gui.popup(HighPopup, 50);
      if (JoueMelodieComplete == false) JoueMelodieComplete == true; // Demande de jouer toute la mélodie
    }
    else
    {
      // Affiche le fond d'écran
      gb.display.clear(); 
      gb.display.drawImage(1, 0, Simon);
      
      // Gestion des tours
      if ((AttenteNote == 0) && (JeuCommence == true)) {
        if (AQuiDeJouer == Ordinateur) {
          TourOrdinateur();    // C'est l'ordinateur qui joue
        } else TourJoueur();   // C'est au tour du joueur
      }
      
      // Attente après le tour du joueur
      if (AttenteNote > 0) {
        AttenteNote ++;
        if (AttenteNote > timer_max >> 1) AttenteNote = 0;
      }
      // Dessine les buttons actifs
      DessineBoutons();
    } // Fin de la gestion des tours
  }// Fin du test gb.update
}// Fin de la boucle principale


void TourOrdinateur() {
  if (AttenteProchaineNoteOrdinateur == 0) { // Si le délai 
    SonAJouer = Melodie[NoteCourante]; // Récupère la note suivante de la Mélodie à jouer
    JoueNote(); // Joue le bon son
    DessineBoutons();
    delay(70);
    MasqueLeBouton(); // Cache le bouton allumé
    NoteCourante ++;  // Passe à la note suivante
    AttenteProchaineNoteOrdinateur ++; // Augmente le timer de 1
  }

  if (AttenteProchaineNoteOrdinateur > 0) { // Incrémente le timer entre les notes
    AttenteProchaineNoteOrdinateur ++;
  }
  if (AttenteProchaineNoteOrdinateur > timer_max) { // Fin du délai entre les notes, vérifie s'il rese des notes à jouer pour cette étape

    if (NoteCourante >= EtapeMelodieValidee) { // Si on a joué toutes les notes, passer au tour du joueur 
      NoteCourante = 0;  // réinitialise le compteur de la note courante pour le tour du joueur
      AttenteProchaineNoteOrdinateur = 0; // réinitialise le timer pour la mélodie pour le prochain tour de l'ordinateur
      AQuiDeJouer = Joueur; // Passe au tour du joueur
    }
    else {// Attente des notes restant à jouer
      AttenteProchaineNoteOrdinateur = 0;
    }
  } // Fin de la gestion de l'attente du délai après que l'ordinateur ait joué une note
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
  gb.display.cursorX = 68;  gb.display.cursorY = 9;
  gb.display.setColor(BLACK);
  gb.display.print(EtapeMelodieValidee - 1);
}


// Gestion du tour du joueur
void TourJoueur() {
  
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
        AttenteProchaineNoteOrdinateur = 1;
        Attente_TimerTour = 0;
        if (NoteCourante >= EtapeMelodieValidee) { // Si le joueur a joué toutes les notes jusqu'à la position actuelle, on continue
          NoteCourante = 0;                        // Réinitialise la note à jouer par l'ordinateur au dbut de la mélodie.
          AttenteProchaineNoteOrdinateur = 0;      // Réinitialise le timer de la mélodie.
          AQuiDeJouer = Ordinateur;                // Passe au tour de l'ordinateur
          EtapeMelodieValidee ++;                  // Valide la note actuelle comme étape franchie
          AttenteNote = 1;                         
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
  SonAJouer = 5;    // Joue son 'erreur'
  if (Echec_timer <= 1) JoueNote(); 
  delay(25);
  MasqueLeBouton();
  if (Echec_timer > timer_max) { // Temps écoulé
    initGame();
  }
}




