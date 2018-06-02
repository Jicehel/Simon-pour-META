#include "Sons.h"

// introductions
int intro() {
    gb.gui.popup(NewGamePopup, 50,2);
    SonAJouer = TOUCHE_ROUGE; JoueNote();MasqueLeBouton();
    SonAJouer = TOUCHE_BLEUE; JoueNote();MasqueLeBouton();
    SonAJouer = TOUCHE_VERTE; JoueNote();MasqueLeBouton();
    SonAJouer = TOUCHE_JAUNE; JoueNote();MasqueLeBouton();
    delay(1000);
    SonAJouer = TOUCHE_JAUNE; JoueNote();MasqueLeBouton();
    delay(250);
    SonAJouer = TOUCHE_JAUNE; JoueNote();MasqueLeBouton();
    delay(250);
    SonAJouer = TOUCHE_ROUGE; JoueNote();MasqueLeBouton();
    JoueIntro = false;
}


// Joue le son corespondant à la touche
void JoueNote() {
  switch (SonAJouer) {
    case 1:
      gb.sound.tone(1318,250);
      AfficheBoutonBleu = true;
      break;
    case 2:
      gb.sound.tone(1046,250);
      AfficheBoutonJaune = true;
      break;
    case 3:
      gb.sound.tone(1760,250);
      AfficheBoutonVert = true;
      break;
    case 4:
      gb.sound.tone(659,250);
      AfficheBoutonRouge = true;
      break;
    case 5://=fail
      gb.sound.playCancel();
      break;
  }
}
