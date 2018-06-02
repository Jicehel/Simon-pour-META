#ifndef _SOUND_H_
#define _SOUND_H_

#include <Gamebuino-Meta.h>
#include <D:\Jean-Charles\META\Simon\Simon.h>

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

void JoueNote();
int intro();

#endif _SOUND_H_


