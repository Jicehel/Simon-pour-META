#ifndef _SOUND_H_
#define _SOUND_H_

#include <Gamebuino-Meta.h>
#include "Simon.h"
#include "Language.h"

extern byte SonAJouer;
extern bool JoueIntro;
extern bool AfficheBoutonBleu;
extern bool AfficheBoutonRouge;
extern bool AfficheBoutonJaune;
extern bool AfficheBoutonVert;

#define TOUCHE_BLEUE 1  // touche droite    
#define TOUCHE_JAUNE 2  // touche bas     
#define TOUCHE_VERTE 3  // touche gauche           
#define TOUCHE_ROUGE 4  // touche haut 

extern void JoueNote();
extern int intro();
extern int introMulti();

#endif _SOUND_H_


