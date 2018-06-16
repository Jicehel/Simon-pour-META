// Jeu du Simon sur la console française META
// site: www.gamebuino.com
// Version 1.1

#include <Gamebuino-Meta.h>
#include "Graphiques.h"  // Import des graphiques
#include "Sons.h"        // Gestion des sons
#include "Language.h"    // Gestion des messages en Anglais et en français
#include "Simon.h"
#include "Highscore.h"

// Variables du jeu
byte niveau        = 0;        // Nombre de notes à jouer 
byte longueur      = 0;        // Longueur que le joueur à joué (nombre de notes)
byte sequenceSimon[50]; 
byte sequenceJoueur[50];
byte compteur;
byte hiScore       = 0;        // Meilleur score

int  JoueurActif;              // A qui de jouer (1,2,3,4)
int  NbJoueurs;                // Nombre de joueurs multijoueur (1 à 4);
int  JoueursOK[(MAX_MULTI+1)] = {0,0,0,0,0}; // Les joueurs encore en jeu (le 0 n'est pas utiisé)

// Variables permettant d'afficher ou non les boutons allumés
bool AfficheBoutonRouge;
bool AfficheBoutonBleu;
bool AfficheBoutonJaune;
bool AfficheBoutonVert;

bool JeuCommence;             // Délai avant le début du jeu
bool JoueIntro;               // On joue l'intro une fois
bool AQuiDeJouer;             // Actif quand l'ordi joue la mélodie
bool JoueMelodieComplete;     // Actif quand le joueur a joué toute la mélodie
bool ErreurJoueur;            // Actif en cas d'erreur du joueur

byte Echec_timer;             // Temps écoulé
byte Attente_TimerDebutJeu;
byte Attente_TimerTour;
byte SonAJouer;               // Mémorise le son à jouer et la touche à allumer puis à éteindre
byte Melodie[NOTESMAX];       // Dimensionne le tableau contenant les notes à trouver
byte NoteCourante;            // Indicateur de position dans la mélodie de la note à jouer 
byte EtapeMelodieValidee;     // Augmente à chaque fois que le joueur trouve la bonne note
int  AttenteProchaineNote;    // Temps entre 2 notes
byte AttenteNote;
char ChaineJoueur[40];


// Initialisation
void setup() {
  gb.begin();
  initHighscore();
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
  JoueurActif = 0;
  NbJoueurs = 0;
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
  AttenteProchaineNote = 800;     
  AttenteNote = 0;
  hiScore = highscore[0];
  JoueIntro = true;
}

void loop() {
  if (gb.update()) {
    Appuie_bouton_Menu();          // L'appuie sur Menu relance le jeu pour le moment (à terme, sera remplacé par le menu d'options)
    if ((JeuCommence == false) && (NbJoueurs == 0)) EtatMenuInitial();
    if (NbJoueurs > 0) GestionDePartie();
  }// Fin du test gb.update
}// Fin de la boucle principale


void GestionDePartie() {
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
      gb.gui.popup(HighPopup, 50,LANGUAGE_DEFAULT_SIZE);
      if (JoueMelodieComplete == false) JoueMelodieComplete == true; // Demande de jouer toute la mélodie
    }
    else
    {
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
    } // Fin de la gestion des tours
}

void TourOrdinateur() {

    SonAJouer = Melodie[NoteCourante]; // Récupère la note suivante de la Mélodie à jouer
    JoueNote(); // Joue le bon son
    NoteCourante ++;  // Passe à la note suivante
  
    if (NoteCourante >= EtapeMelodieValidee) { // Si on a joué toutes les notes, passer au tour du joueur 
       if (NbJoueurs > 1) {
          while (AQuiDeJouer  == Ordinateur) {
              if (JoueurActif < NbJoueurs) {
                    JoueurActif = (JoueurActif+1);
              } else  JoueurActif = 1;     
              if (JoueursOK[(JoueurActif)]== 1) {
                    AQuiDeJouer = Joueur;
              }
          }
       sprintf( ChaineJoueur, gb.language.get(lang_joueur,LANGUAGE_DEFAULT_SIZE), JoueurActif);
       gb.gui.popup(ChaineJoueur, 30);      
       delay(2500);
       }  
       AQuiDeJouer = Joueur;
       NoteCourante = 0;  // réinitialise le compteur de la note courante pour le tour du joueur  
       RedessineSimon();
    } else {
       // Ligne ci-dessous a activer en mode debug pour voir et ajuster la valeur d'attente entre 2 touches si besoin
       // gb.display.cursorX = 58;  gb.display.cursorY = 49; gb.display.print(AttenteProchaineNote - (12*NoteCourante));
       delay(AttenteProchaineNote - (12*NoteCourante));
    }
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
  RedessineSimon();
}


void RedessineSimon() {
  gb.display.drawImage(1, 0, Simon);
  //Score actuel
  gb.display.cursorX = 68;  gb.display.cursorY = 9;
  gb.display.setColor(BLACK);
  gb.display.print(EtapeMelodieValidee - 1);
}


//Affiche les boutons
void DessineBoutons() {
  RedessineSimon();
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
  }

  // Attente après que le joueur ait sélectionné une touche
  if (Attente_TimerTour > 0) {
    Attente_TimerTour ++;

    if (Attente_TimerTour > (timer_max >> 1)) { //Délai dépassé...

      // Traitement d'une erreur de touche
      if (SonAJouer != Melodie[NoteCourante] ) {
        ErreurJoueur = true;
        Erreur_Joueur();
      }
      else  
      {
        NoteCourante ++;
        Attente_TimerTour = 0;
        if (NoteCourante >= EtapeMelodieValidee) { // Si le joueur a joué toutes les notes jusqu'à la position actuelle, on continue
          NoteCourante = 0;                        // Réinitialise la note à jouer par l'ordinateur au dbut de la mélodie.
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
  if ((gb.buttons.pressed(BUTTON_MENU))) { // Appuie sur c pour le moment, réinialise le jeu mais sera remplacé à terme par un menu d'options 
    initGame();
    ErreurJoueur = false;
  }
}


// Traitement d'une erreur du joueur
void Erreur_Joueur() {
  Echec_timer ++;
  SonAJouer = 5;    // Joue son 'erreur'
  if (Echec_timer <= 1) JoueNote(); 
  if (Echec_timer > timer_max) { // Temps écoulé
    delay(1000);
    JoueursOK[(JoueurActif)] = 0;
    NbJoueurs--;
    if (NbJoueurs == 0) {
        saveHighscore(EtapeMelodieValidee-1);
    }
  }
}


// Le menu initial
void EtatMenuInitial() {
    delay(200);
 
    // On peut appeler le menu
    uint8_t entry = gb.gui.menu(title, entries, 5, LANGUAGE_DEFAULT_SIZE);

    // On fixe l'état selon le mode choisit
    if (entry < 4) { 
      NbJoueurs = entry+ 1;
      if (NbJoueurs == 1) JoueurActif = 1;
      // Met à 1 le marqueur de joueur actif de 1 au nombre de joueurs sélectionnés
      for(/*initialisation*/ int i=1; /*condition*/ i<=NbJoueurs; /*incrémentation*/ ++i){ JoueursOK[i]=1;   }   
   } else { AQuiDeJouer = Joueur; drawHighScores(1); }
}


