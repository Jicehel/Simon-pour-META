// Gestion des sons

#include <Gamebuino-Meta.h>
#include "Sons.h"
#include "Graphiques.h"
#include "Language.h"

// introductions
int intro() {
    gb.display.clear();
    gb.gui.popup(NewGamePopup, 50,LANGUAGE_DEFAULT_SIZE);
    delay(2500);
    SonAJouer = TOUCHE_ROUGE; JoueNote();
    SonAJouer = TOUCHE_BLEUE; JoueNote();
    SonAJouer = TOUCHE_VERTE; JoueNote();
    SonAJouer = TOUCHE_JAUNE; JoueNote();
    delay(1000);
    SonAJouer = TOUCHE_JAUNE; JoueNote();
    delay(250);
    SonAJouer = TOUCHE_JAUNE; JoueNote();
    delay(250);
    SonAJouer = TOUCHE_ROUGE; JoueNote();
    delay(1500);
    JoueIntro = false;
}

int introMulti() {
    gb.display.clear();
    gb.gui.popup(NewGameMultiPopup, 50,LANGUAGE_DEFAULT_SIZE);
    delay(2500);
    SonAJouer = TOUCHE_JAUNE; JoueNote();
    SonAJouer = TOUCHE_VERTE; JoueNote();
    SonAJouer = TOUCHE_BLEUE; JoueNote();
    SonAJouer = TOUCHE_ROUGE; JoueNote();
    delay(1000);
    SonAJouer = TOUCHE_ROUGE; JoueNote();
    delay(250);
    SonAJouer = TOUCHE_ROUGE; JoueNote();
    delay(250);
    SonAJouer = TOUCHE_JAUNE; JoueNote();
    delay(1500);
    JoueIntro = false;
}

// Joue le son corespondant à la touche
void JoueNote() {
  switch (SonAJouer) {
    case 1:  // Bouton bleu
      gb.sound.tone(1318,250);
      AfficheBoutonBleu = true;
      break;
    case 2:  // Bouton jaune
      gb.sound.tone(1046,250);
      AfficheBoutonJaune = true;
      break;
    case 3:  // Bouton vert
      gb.sound.tone(1760,250);
      AfficheBoutonVert = true;
      break;
    case 4:  // Bouton rouge
      gb.sound.tone(659,250);
      AfficheBoutonRouge = true;
      break;
    case 5:   //Son échec allume tous les boutons
      gb.sound.playCancel();
      AfficheBoutonBleu = true;
      AfficheBoutonJaune = true;
      AfficheBoutonVert = true;
      AfficheBoutonRouge = true;
      break;
  }
  DessineBoutons();
  if ((AQuiDeJouer ==  Ordinateur) && (NoteCourante < EtapeMelodieValidee )) { delay(AttenteProchaineNote/6 - (NoteCourante*2)); } 
  else if (AQuiDeJouer ==  Joueur) { delay(100);   }
  MasqueLeBouton();
}
