// Définition des textes des menus et messages en français et en anglais (pour le moment)

#include <Gamebuino-Meta.h>
#include "Language.h"

// Définition des textes des popups

// Messge: Nouvelle partie
const MultiLang NewGamePopup[] = {
    { LANG_EN, "New game" }, { LANG_FR, "Nouvelle partie" },
    { LANG_DE, "Neues Spiel" }, { LANG_ES, "Nueva partida" },
};

// Message: Nouvelle partie multi joueurs
const MultiLang NewGameMultiPopup[] = {
    { LANG_EN, "New game multiplayers" },  { LANG_FR, "Nouvelle partie multijoueurs" }, 
    { LANG_DE, "Neue Mehrspieler-Modus" }, { LANG_ES, "Nueva partida multijugador" },    
};

// Message: Record battu => bien joué
const MultiLang HighPopup[] = {
    { LANG_EN, "Well done !" }, { LANG_FR, "Bien joué !" }, 
    { LANG_DE, "Gut gemacht" }, { LANG_ES, "Buen hecho" },      
};

// Menu de sélection du mode de jeu
const MultiLang title[] = {
    { LANG_EN, "Select game mode" },{ LANG_FR, "Mode de jeu" },
    { LANG_DE, "Einstellung de betriebweise " }, { LANG_ES, "Selección del modo del partida" }, 
};

const MultiLang entryOnePlayer[] = {
    { LANG_EN, "1 player mode" },  { LANG_FR, "Mode 1 joueur" }, 
    { LANG_DE, "1 Spieler" }, { LANG_ES, "1 jugador" }, 
};

const MultiLang entry2players[] = {
    { LANG_EN, "2 players mode" },  { LANG_FR, "Mode 2 joueurs" },
    { LANG_DE, "2 Spieler" }, { LANG_ES, "2 jugadores" }, 
};

const MultiLang entry3players[] = {
    { LANG_EN, "3 players mode" },  { LANG_FR, "Mode 3 joueurs" },
    { LANG_DE, "3 Spieler" }, { LANG_ES, "3 jugadores" }, 
};

const MultiLang entry4players[] = {
    { LANG_EN, "4 players mode" },  { LANG_FR, "Mode 4 joueurs" },
    { LANG_DE, "4 Spieler" }, { LANG_ES, "4 jugadores" }, 
};

const MultiLang* entries[] = {
      entryOnePlayer,
      entry2players,
      entry3players,
      entry4players,
};

const MultiLang lang_joueur[] = {
  { LANG_EN, "Player %d turn" },   { LANG_FR, "Tour du joueur %d " },
  { LANG_DE, "Spieler %d Runde" },  { LANG_ES, "Paseo del jugador %d " },
};

