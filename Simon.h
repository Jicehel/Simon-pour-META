#ifndef _SIMON_H_
#define _SIMON_H_

#define TOUCHE_BLEUE 1  // Touche droite    
#define TOUCHE_JAUNE 2  // Touche bas     
#define TOUCHE_VERTE 3  // Touche gauche           
#define TOUCHE_ROUGE 4  // Touche haut 

#define MAX_MULTI 4
#define timer_max 16 // Temps pour jouer une note
#define NOTESMAX 42  // Nombre maximum de notes de la mélodie + 2 à ajouter au nombre désiré
#define NBNOTES 4    // Nombre de notes possible (sur le Simon classique c'est 4 mais on pourrait faire des variantes)

// Définition des valeurs pour le tour à jouer
#define Joueur false
#define Ordinateur true

extern void MasqueLeBouton();
extern void DessineBoutons();
extern void Appuie_bouton_Menu();
extern void EtatMenuInitial();
extern void TourOrdinateur();
extern void Partie1joueur();
extern void initGame();
extern void TourJoueur();
extern void RedessineSimon();
extern int  AttenteProchaineNote; 
extern byte NoteCourante; 
extern byte EtapeMelodieValidee;
extern bool AQuiDeJouer;
extern int  JoueursOK[];
extern char ChaineJoueur[];

#endif _SIMON_H_

