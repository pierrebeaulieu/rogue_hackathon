#include "../include/terrain.h"
#include <iostream>
#include <string>
#include <ncurses.h>


void Terrain::init () {
  // on initialise le terminal en mode curses
  initscr();
  // on initialise des couleurs
  init_color();

  clear();
  // on efface le stdscr (le terminal, the standard screen)

  // Comme nous allons taper des caractères pour les commandes du
  // héro, on ne veut pas qu'ils s'affichent (i.e. qu'ils aient un
  // écho sur le standard screen. noecho() switchs off echoing
  noecho();  

  // Par défaut, les caractères entrés sont stockés dans une mémoire
  // tampon (buffer) jusqu'au prochain retour à la ligne (enter). Pour
  // obtenir les caractères dès que nous les tapons, nous devons
  // désactiver la mise en mémoire tampon de la ligne.
  // Nous passons en mode brut (raw), cela empêche les
  // caractères de contrôle (CTRL-C) de générer leur signal (genre
  // tuer l'exécutable). Donc vous ne pouvez pas quitter le programme
  // avec CTRL-C ni le suspendre avec CTRL-Z.
  cbreak(); // or raw();

  // On active les clés F1, F2... et les flêches (puisqu'on veut
  // déplacer notre héro avec).
  keypad(stdscr, TRUE);
  refresh();

 // On demande la taille en nombre de lignes de la fenêtre.
  getmaxyx(stdscr, window_bottom, window_right);

  // On délimite notre playground n'importe comment dans cet exemple
  window_left = 1;
  window_top = 2;
  window_bottom = window_bottom - 15;
  window_right = window_right - 15;

  white_line = init_white_line(window_right+1);
}


void Terrain::affiche (int xpos, int ypos, char thing, int color_pair = 0) {
  attron(COLOR_PAIR(color_pair));
  move(xpos, ypos);
  addch(thing);
  move(xpos, ypos);
  attroff(COLOR_PAIR(color_pair));
  refresh();
}

void Terrain::setup()
{
int longueur = sizeof(bgTab)/sizeof(char);
  for(int i = 0; i < longueur ; i++)
  {
    int x = i % bg_nx;
    int y = i / bg_nx;
    char lettre = bgTab[i];
    affiche(x,y,char);
  }

}