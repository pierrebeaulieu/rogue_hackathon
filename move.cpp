#include <iostream>
#include <string>
#include <ncurses.h>

// LE HÉRO
// =======

// la position du '@' qui est aussi la position du curseur de votre
// terminal
int xcursor = 3; // ligne
int ycursor = 4; // colonne

// un exemple de héro et d'une bébête quelconque
const char hero = '@';
const char bebete = 'E';

// LE TERMINAL
// ===========

// Le terminal est votre playground. Il est constitué d'un nombre de
// lignes et de colonnes (une sorte de matrice). CEs deux valeurs vous
// seront données par une fonction de ncurses.

// Dans cet exemple de code très simpliste, on repère, de manière
// basique, l'espace de jeu (celui où se baladera votre héro) par:
int window_top;
int window_left;
int window_bottom;
int window_right;


// LES TOUCHES DE DÉPLACEMENT
// ==========================
// On décide des 4 touches pour déplacer le héro (ici les 4 flêches en
// bas et à droite de votre clavier)
constexpr int left_arrow = 4;
constexpr int right_arrow = 5;
constexpr int top_arrow = 3;
constexpr int bottom_arrow = 2;
// On crée (ici par une énumération) un ensemble des 4 directions de
// déplacement.
enum DIRECTION
  {
   LEFT = left_arrow,
   RIGHT = right_arrow,
   TOP = top_arrow,
   BOTTOM = bottom_arrow,
  };

// AFFICHAGE DES MESSAGES
// ======================
// On va afficher des messages de jeu, par exemple ici à la première
// ligne première colonne du terminal.
int xmsg = 0;
int ymsg = 0;

// On doit effacer le message précédent i.e. le recouvrir par une
// chaine de caractères blancs, qu'on crée avec la fonction
// suivante. Faites mieux !
char* white_line;
char* init_white_line (int size) {
  char* buff = new char [size];
  for (int i = 0; i < size; ++i)
    buff[i] = ' ';
  buff[size-1] = '\0';
  return buff;
}

// On affiche le message msg dans la couleur YELLOW par défaut. à la
// position de la ligne de message décidée auparavant (remarquez
// l'horrible utilisation de variables globales dans cette
// fonction. Il faut faire une structure de classes.
void print_message (const char* msg, int color_pair = YELLOW) {
  // on charge la couleur color_pair.
  attron(COLOR_PAIR(color_pair));
  // move déplace le curseur à la position du message en ligne colonne
  // (ici ligne xmsg, colonne ymsg)
  move(xmsg, ymsg);
  // On affiche la ligne blanche (pour effacer le message précédent).
  addstr(white_line);
  // On revient à la position du message.
  move(xmsg, ymsg);
  // On affiche le message.
  addstr(msg);
  // On retourne à la position du héro.
  move(xcursor, ycursor);
  // On enlève la couleur choisie.
  attroff(COLOR_PAIR(color_pair));
  refresh();
}

void debug_print_hero_position () {
  std::string str = "the hero is at position line ";
  str += std::to_string(xcursor);
  str += ", column ";
  str += std::to_string(ycursor);
  attron(COLOR_PAIR(BLUE));
  move(1, 0);
  addstr(white_line);
  move(1, 0);
  addstr(str.c_str());
  move(xcursor, ycursor);
  attroff(COLOR_PAIR(BLUE));
  refresh();
}

// INITIALISATION DU TERMINAL
// ==========================
void init () {
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

// AFFICHER UN CARACTÈRE
// =====================

// Cette fonction déplace le curseur à la position (xpos, ypos)
// affiche le caractère thing et, comme l'affichage fait avancer le
// curseur, on le remet à la position initiale move it back to
// xcursor, ycursor
void affiche (int xpos, int ypos, char thing, int color_pair = 0) {
  attron(COLOR_PAIR(color_pair));
  move(xpos, ypos);
  addch(thing);
  move(xpos, ypos);
  attroff(COLOR_PAIR(color_pair));
  refresh();
}

// DÉPLACEMENT DU HÉRO
// ===================

// mon héro laisse une trace en marchant (aucun intérêt on pourrait
// juste écrire un step ' ' pour effacee le héro)
const char step = ' ';

// En argument on passe le caractère qui représente la mouvement du
// héro i.e LEFT, RIGHT, TOP ou BOTTOM. Le héro est à la position
// (xcursor, ycursor) après le mouvement, par exemple 'LEFT', il va à
// la nouvelle position (xcursor, ycursor-1) - si c'est possible -

//les fonctions ci-dessous renvoie true si le héro peut se déplacer 
//dans la direction demandée, false sinon

bool try_left(){
  if (ycursor != window_left){
    char truc = mvinch(xcursor-1, ycursor);
    if (truc == '+' || truc == '#' || truc == '.') {return true}
  }
  else {return false}
}

bool try_right(){
  if (ycursor != window_right){
    char truc = mvinch(xcursor-1, ycursor);
    if (truc == '+' || truc == '#' || truc == '.') {return true}
  }
  else {return false}
}

bool try_bottom(){
  if (xcursor != window_bottom){
    char truc = mvinch(xcursor-1, ycursor);
    if (truc == '+' || truc == '#' || truc == '.') {return true}
  }
  else {return false}
}

bool try_top(){
  if (xcursor != window_top){
    char truc = mvinch(xcursor-1, ycursor);
    if (truc == '+' || truc == '#' || truc == '.') {return true}
  }
  else {return false}
}


void move (char c) {
  // On affiche un step pour effacer le héro de la position qu'il va
  // quitter.
  affiche(xcursor, ycursor, step);
  
  // Le héro est en noir.
  int color = BLACK;

  if (c == LEFT) {
    print_message("the hero is heading left");
    if (try_left()) {
      // le mouvement est possible
      // on bouge le curseur à la position (xcursor, ycursor-1)
      ycursor-=1;
    } else {
      print_message("the hero reached the playground left side");
      color = RED; // il voit rouge
    }
  }
  else if (c == RIGHT) {
    print_message("the hero is heading right");
    if (try_right()) {
      // on modifie la position du curseur à la position (xcursor, ycursor+1)
      ycursor+=1;
    } else {
      print_message("the hero reached the playground right side");
      color = CYAN; // il est cyanosé 
    }
  }
  else if (c == BOTTOM) {
    print_message("the hero is heading down");

    if (try_bottom()) {
      // on modifie la position du curseur à la position (xcursor+1, ycursor)
      xcursor+=1;
    } else {
      print_message("the hero reached the playground bottom side");
      color = YELLOW; // il est malade
    }
  }
  else if (c == TOP) {
    print_message("the hero is heading up");
    if (try_top()) {
      // on bouge le curseur à la position (xcursor-1, ycursor)
      xcursor-=1;
    } else {
      print_message("the hero reached the playground top side");
      color = GREEN; // il a peur
    }
  }
  // On affiche le héro à la position (xcursor, ycursor)
  affiche(xcursor, ycursor, hero, color);
  // pour debugger on affiche la position du héro en bleu sur la
  // seconde ligne
  debug_print_hero_position();
}