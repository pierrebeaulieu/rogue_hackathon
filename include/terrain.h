class Terrain
{
public:

Terrain(); //constructeur

void init();
void affiche(int xpos, int ypos, char thing, int color_pair = 0);
void setup();


/*void modif(int i, int val); //modifier une valeur dans le bg
void setup(); //initialisation des bordures
void clear(); // nettoyage complet de la console
void print(int score); // affiche tout dans la console 
int get_x(); // renvoie bg_nx
int get_y(); // renvoie bg_ny

 

private:
static const int bg_nx = 40;
static const int bg_ny = 20;
int bgTab[bg_nx*bg_ny]; */

private:

static const window_left = 1;
static const window_top = 2;
static const window_bottom ;
static const window_right ; 
static const bg_nx = window_right - window_left;
static const bg_ny = window_bottom - window_top;
int bgTab[bg_nx*bg_ny];

}