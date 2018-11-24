                                                                                                   /*
/##################################################################################################\
# Description : #                                                                                  #
#################                                                                                  #
#                                                                                                  #
#  Ce fichier est le point d'entrée du moteur ( le "main()" ).                                     #
#                                                                                                  #
#                                                                                                  #
\##################################################################################################/
                                                                                                     */

#include "window.h"
#include "input.h"
#include "timer.h"
#include "ogl.h"
#include "load.h"
#include "camera.h"
#include "maths_3d.h"



/****************************************************************************\
*                                                                            *
*                            Variables GLOBALES                              *
*                                                                            *
\****************************************************************************/
WINDOW    *win = NULL;
INPUT2     *inp = NULL;
TIMER     *tim = NULL;
float incrementAngleY = 0;

//////////////////// TP2 ////////////////////
Texture  *texture_arbre = NULL;	// à utiliser dans l'exercice 1.1 TP2
Texture  *texture_logo = NULL;	// à utiliser dans l'exercice 2.1 TP2
Texture  *texture_raptor = NULL; // à utiliser dans l'exercice 3.1 TP2
Texture *texture_apple = NULL;
Model3D  *geometrie_raptor = NULL; // à utiliser dans l'exercice 3.1 TP2
float *positions_arbres_X = NULL; // a utiliser pour créer la foret
float *positions_arbres_Z = NULL; // a utiliser pour créer la foret
int DIM_FOREST = 50;
int SIZE_LAND = 200;

//////////////////// TP3 ////////////////////
Image *heightmap = NULL; // à utiliser pour generer le geometrie du terrain 3D (exercice 1.1 TP3) 
Texture *texture_terrain=NULL; // à utiliser pour texturer le terrain 3D (exercice 1.1 TP3) 
float horisontal_resolution = 10; // à utiliser pour générer le terrain
float vertical_scale = 0.3; // à utiliser pour générer le terrain

// les variables suivantes sont à utiliser dans l'exercice 2.1 du TP3
CAMERA *cam = NULL;
float posX=0;
float posY=30;
float posZ=0;
float angleX=0;
float angleY=0;
float angleZ=0;
float pommeX = -40;
float pommeY = 30;
float pommeZ = -40;
float raptorX = 100;
float raptorY = 10;
float raptorZ = 100;
float angleRaptorX = 90;
float angleRaptorY = 180;
float angleRaptorZ = 0;
float previousAngleRaptorZ = 0;
float speedRaptorX = 0;
float speedRaptorY = 0;
float speedRaptorZ = 0;
float distance = 0;
float distanceRaptor = 0;
int score = -1;
char scoreChar[40];
char buffer[64];
bool end = false;
bool redMoon = false;
int speedCount = 1;



/****************************************************************************\
*                                                                            *
*                             Variables LOCALES                              *
*                                                                            *
\****************************************************************************/




/********************************************************************\
*                                                                    *
*  Démarre l'application (avant la main_loop). Renvoie false si      *
*  il y a une erreur.                                                *
*                                                                    *
\********************************************************************/

bool start()
{
	// initialisations de base
	// -----------------------
	score = 0;
	sprintf(scoreChar, "%d", score);
	win = new WINDOW();									// prépare la fenêtre
	win->create( 800, 600, 16 , 60 , false );			// crée la fenêtre

	tim = new TIMER();									// crée un timer
	cam= new CAMERA();

	inp = new INPUT2(win);								// initialise la gestion clavier souris
	create_context(*win);								// crée le contexte OpenGL sur la fenêtre
	init_font(*win, "Courier");							// initialise la gestion de texte avec OpenGL

  
	// initialisations d'OpenGL
	// ------------------------
	glClearColor(0.0f,0.0f,0.0f,1.0f);					// set clear color for color buffer (RGBA, black)
	glViewport(0,0,win->Xres,win->Yres);				// zone de rendu (tout l'écran)
 	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LESS);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// mapping quality = best
	glFrontFace(GL_CCW);								// front of face is defined counter clock wise
	glPolygonMode(GL_FRONT, GL_FILL);					// front of a face is filled
	glPolygonMode(GL_BACK,  GL_LINE);					// back of a face is made of lines
	glCullFace(GL_BACK);								// cull back face only
	glDisable(GL_CULL_FACE);						    // disable back face culling


	win->set_title("Predator");

	/////////////////// TP2 //////////////////
	texture_arbre=new Texture();
	
	/////////////////// TP2 Exercice 1.1 //////////////////
	texture_arbre->load_texture("texture_arbre.tga",NULL);

	/////////////////// TP2 Exercice 1.2 //////////////////	
	texture_arbre->load_texture("texture_arbre.tga","arbre_masque.tga");

	/////////////////// TP2 Exercice 1.1 et 1.2 //////////////////	
	glGenTextures(1, texture_arbre->OpenGL_ID);				// crée un "nom" de texture (un identifiant associé à la texture)


	/////////////////// TP2 Exercice 2.1 //////////////////	
	texture_logo = new Texture();
	texture_logo->load_texture("artemis.tga", "artemis_mask.tga");


	///////////////// TP2 bonus - la foret
	positions_arbres_X = new float[DIM_FOREST];
	positions_arbres_Z = new float[DIM_FOREST];	
	for (int i = 0; i< DIM_FOREST; i++){
		positions_arbres_X[i] = SIZE_LAND * (2* rand() - RAND_MAX )/ RAND_MAX;
		positions_arbres_Z[i] = SIZE_LAND * (2* rand() - RAND_MAX) / RAND_MAX;
	}
		
	/////////////////// TP2 Exercice 3.1 //////////////////	
	geometrie_raptor= new Model3D();
	geometrie_raptor->load_3d_model("RAPTOR.off");
	texture_raptor=new Texture();
	texture_raptor->load_texture("texture_raptor.tga",NULL);
	glGenTextures(1, texture_raptor->OpenGL_ID);				// crée un "nom" de texture (un identifiant associé à la texture)

	/////////////////// TP3 Exercice 1.1 //////////////////	
	heightmap = new Image();
	heightmap->load_tga("heightmap.tga");
	texture_terrain= new Texture();
	texture_terrain->load_texture("texture_terrain.tga",NULL);
	glGenTextures(1, texture_terrain->OpenGL_ID);				// crée un "nom" de texture (un identifiant associé à la texture)

	texture_apple = new Texture();
	texture_apple->load_texture("pomme2.tga", "pomme_masque2.tga");
	glGenTextures(1, texture_apple->OpenGL_ID);				// crée un "nom" de texture (un identifiant associé à la texture)

	return true;
}

/********************************************************************\
*                                                                    *
*  les formes geometriques                                              *
*                                                                    *
\********************************************************************/
int rand_a_b(int a, int b) {
	return rand() % (b - a) + a;
}
/////////////////// TP1 Exercice 3.1 //////////////////	
void display_pyramide(float x, float y, float z, float h, float c){

	glBegin(GL_TRIANGLES);
		glColor3f(0, 1, 0); glVertex3f(x + c / 2, y, z + c / 2);
		glColor3f(0,1,0); glVertex3f(x,y+h,z);		
		glColor3f(1, 0, 0); glVertex3f(x - c / 2, y, z + c / 2);

		glColor3f(0,1,0); glVertex3f(x-c/2,y, z+c/2);
		glColor3f(0,1,0); glVertex3f(x,y+h,z);		
		glColor3f(1,0,0); glVertex3f(x-c/2,y, z-c/2);

		glColor3f(1,0,0); glVertex3f(x-c/2,y, z-c/2);
		glColor3f(0,1,0); glVertex3f(x,y+h,z);		
		glColor3f(0,1,0); glVertex3f(x+c/2,y, z-c/2);

		glColor3f(1,0,0); glVertex3f(x+c/2,y, z-c/2);
		glColor3f(0,1,0); glVertex3f(x,y+h,z);		
		glColor3f(0,1,0); glVertex3f(x+c/2,y, z+c/2);
	glEnd();

	  glBegin(GL_QUADS);
		glColor3f(0, 1, 0); glVertex3f(x + c / 2, y, z+ c / 2);
		glColor3f(1, 0, 0); glVertex3f(x - c / 2, y, z+ c / 2);
		glColor3f(1, 0, 0); glVertex3f(x - c / 2, y, z - c / 2);
		glColor3f(0, 1, 0); glVertex3f(x + c / 2, y, z- c / 2);
	  glEnd();
}

/////////////////// TP1 Exercice 3.1 //////////////////	
////////////////// pyramide qui peut tourner autour de soi même //////////
void display_pyramide(float h, float c){

	glBegin(GL_TRIANGLES);
		glColor3f(0, 1, 0); glVertex3f(c / 2, 0,  c / 2);
		glColor3f(0, 1, 0); glVertex3f(0, h, 0);
		glColor3f(1, 0, 0); glVertex3f(- c / 2, 0,  c / 2);

		glColor3f(0, 1, 0); glVertex3f(- c / 2, 0,  c / 2);
		glColor3f(0, 1, 0); glVertex3f(0,  h, 0);
		glColor3f(1, 0, 0); glVertex3f(- c / 2, 0,  - c / 2);

		glColor3f(1, 0, 0); glVertex3f(- c / 2, 0,  - c / 2);
		glColor3f(0, 1, 0); glVertex3f(0,  h, 0);
		glColor3f(0, 1, 0); glVertex3f(c / 2, 0,  - c / 2);

		glColor3f(1, 0, 0); glVertex3f(c / 2, 0, - c / 2);
		glColor3f(0, 1, 0); glVertex3f(0,  h, 0);
		glColor3f(0, 1, 0); glVertex3f(c / 2, 0,  c / 2);
	glEnd();

	glBegin(GL_QUADS);
		glColor3f(0, 1, 0); glVertex3f(c / 2, 0,  c / 2);
		glColor3f(1, 0, 0); glVertex3f(- c / 2, 0,  c / 2);
		glColor3f(1, 0, 0); glVertex3f(- c / 2, 0, -c / 2);
		glColor3f(0, 1, 0); glVertex3f(+ c / 2, 0, - c / 2);
	glEnd();
}


/////////////////// TP1 Exercice 4.1 //////////////////	
////////////////// l'horloge //////////
void display_horloge(){
	
	glPushMatrix();
	float heure=(float)tim->get_heure();
	float angleH = heure/12*360;
	glRotatef(angleH,0,0,1);
	glColor3f(1,1,0); 
	glBegin(GL_TRIANGLES);
		glVertex3f(0.3,0, 0);
		glVertex3f(-0.3,0,0);
		glVertex3f(0,7,0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	float minute=(float)tim->get_minute();
	float angleM = minute/60*360;
	glRotatef(angleM,0,0,1);
	glBegin(GL_TRIANGLES);
		glColor3f(1,0,1); 
		glVertex3f(0.3,-0.3, 0);
		glVertex3f(-0.3,0.3, 0);
		glVertex3f(0,9,0);
		glEnd();
	glPopMatrix();
		
	glPushMatrix();
	float seconde=(float)tim->get_seconde();
	float angleS = seconde/60*360;
	glRotatef(angleS,0,0,1);
	glBegin(GL_LINES);
		glColor3f(1,0.5,0.5); 
		glVertex3f(0,0, 0);
		glVertex3f(0,6.75, 0);
		glEnd();
	glPopMatrix();

	int i;
	for(i=0;i<12;i++){
		glPushMatrix();
			glRotatef(30*i,0,0,1);
			glTranslatef(0,10,0);
			if(i%3==0)
				glScalef(1,1,1);
			else glScalef(0.5,0.5,1);
			glBegin(GL_QUADS);
				glColor3f(1,0,0);
				glVertex3f(-0.5,-1, 0);
				glVertex3f(-0.5,1, 0);
				glVertex3f(0.5,1, 0);
				glVertex3f(0.5,-1, 0);
			glEnd();	  
		glPopMatrix();
	}
}

/////////////////// end TP1 Exercice 4.1 //////////////////	




/////////////////// TP2 Exercice 1.1 et 1.2 //////////////////	
void display_arbre()
{
	
	glBindTexture(GL_TEXTURE_2D, texture_arbre->OpenGL_ID[0]);	// et on active ce "nom" comme texture courante (définie plus bas)

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// on répète la texture en cas de U,V > 1.0
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// ou < 0.0
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // indique qu'il faut mélanger la texture avec la couleur courante

	// charge le tableau de la texture en mémoire vidéo et crée une texture mipmap
	
	if (texture_arbre->isRGBA)
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, texture_arbre->img_color->lenx, texture_arbre->img_color->leny, GL_RGBA, GL_UNSIGNED_BYTE, texture_arbre->img_all);
	else
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8,  texture_arbre->img_color->lenx, texture_arbre->img_color->leny, GL_RGB,  GL_UNSIGNED_BYTE, texture_arbre->img_color->data);
	

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.4); // on utilise cette fonction pour controller la transparence dans l'exercice 1.2 de TP 2
		
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK,  GL_FILL);		
		
	glBegin(GL_QUADS);
		 glTexCoord2f(0,0); glVertex3f(-5,0, 0);
		 glTexCoord2f(0,1); glVertex3f(-5,20, 0);
		 glTexCoord2f(1,1); glVertex3f(5,20, 0);
		 glTexCoord2f(1,0); glVertex3f(5,0, 0);
	glEnd();

	glPushMatrix();
		glRotatef(90, 0, 1, 0);
		glBegin(GL_QUADS);
			 glTexCoord2f(0,0); glVertex3f(-5,0, 0);
			 glTexCoord2f(0,1); glVertex3f(-5,20, 0);
			 glTexCoord2f(1,1); glVertex3f(5,20, 0);
			 glTexCoord2f(1,0); glVertex3f(5,0, 0);
		glEnd();
	glPopMatrix();

	glPolygonMode(GL_FRONT, GL_FILL);					// front of a face is filled
	glPolygonMode(GL_BACK,  GL_LINE);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
}
/////////////////// end TP2 Exercice 1.1 et 1.2 //////////////////	

void display_pomme(float x, float y, float z) {
	glBindTexture(GL_TEXTURE_2D, texture_apple->OpenGL_ID[0]);	// et on active ce "nom" comme texture courante (définie plus bas)

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// on répète la texture en cas de U,V > 1.0
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// ou < 0.0
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // indique qu'il faut mélanger la texture avec la couleur courante

																// charge le tableau de la texture en mémoire vidéo et crée une texture mipmap

	if (texture_apple->isRGBA)
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, texture_apple->img_color->lenx, texture_apple->img_color->leny, GL_RGBA, GL_UNSIGNED_BYTE, texture_apple->img_all);
	else
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, texture_apple->img_color->lenx, texture_apple->img_color->leny, GL_RGB, GL_UNSIGNED_BYTE, texture_apple->img_color->data);


	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.4); // on utilise cette fonction pour controller la transparence dans l'exercice 1.2 de TP 2

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);

	glPushMatrix();
	//glScalef(0.5f, 0.5f, 0.5f);
	glBegin(GL_QUADS);
	//glTexCoord2f(0, 0); glVertex3f(-5, 0, 0);
	//glTexCoord2f(0, 1); glVertex3f(-5, 20, 0);
	//glTexCoord2f(1, 1); glVertex3f(5, 20, 0);
	//glTexCoord2f(1, 0); glVertex3f(5, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(x-5, y, z);
	glTexCoord2f(0, 1); glVertex3f(x-5, y+10, z);
	glTexCoord2f(1, 1); glVertex3f(x+5, y+10, z);
	glTexCoord2f(1, 0); glVertex3f(x+5, y, z);
	glEnd();
	glPopMatrix();

	/**
	glPushMatrix();
	glScalef(0.5f, 0.5f, 0.5f);
	glRotatef(90, 0, 1, 0);
	glTranslatef(x, y, z);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-5, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(-5, 20, 0);
	glTexCoord2f(1, 1); glVertex3f(5, 20, 0);
	glTexCoord2f(1, 0); glVertex3f(5, 0, 0);**/
	//glTexCoord2f(0, 0); glVertex3f(x, y, z);
	//glTexCoord2f(0, 1); glVertex3f(x, y, z);
	//glTexCoord2f(1, 1); glVertex3f(x, y, z);
	//glTexCoord2f(1, 0); glVertex3f(x, y, z);
	glEnd();
	glPopMatrix();

	glPolygonMode(GL_FRONT, GL_FILL);					// front of a face is filled
	glPolygonMode(GL_BACK, GL_LINE);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
}

///////////////////  TP2 Exercice 2.1 //////////////////
void display_sprite(){

	glRasterPos2i(75,-55);
	glPixelZoom(0.4, 0.4);
	glDisable(GL_DEPTH_TEST);

	if (texture_logo->isRGBA)
		glDrawPixels(texture_logo->img_color->lenx, texture_logo->img_color->leny, GL_RGBA, GL_UNSIGNED_BYTE, texture_logo->img_all);
	else 
		glDrawPixels(texture_logo->img_color->lenx, texture_logo->img_color->leny, GL_RGB, GL_UNSIGNED_BYTE, texture_logo->img_color->data);
	glEnable(GL_DEPTH_TEST);
}
///////////////////  TP2 end Exercice 2.1 //////////////////

///////////////////  TP2 bonus //////////////////
void display_optimized_forest()
{

	glBindTexture(GL_TEXTURE_2D, texture_arbre->OpenGL_ID[0]);	// et on active ce "nom" comme texture courante (définie plus bas)

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// on répète la texture en cas de U,V > 1.0
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// ou < 0.0
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // indique qu'il faut mélanger la texture avec la couleur courante

	// charge le tableau de la texture en mémoire vidéo et crée une texture mipmap

	if (texture_arbre->isRGBA)
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, texture_arbre->img_color->lenx, texture_arbre->img_color->leny, GL_RGBA, GL_UNSIGNED_BYTE, texture_arbre->img_all);
	else
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, texture_arbre->img_color->lenx, texture_arbre->img_color->leny, GL_RGB, GL_UNSIGNED_BYTE, texture_arbre->img_color->data);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.4); // on utilise cette fonction pour controller la transparence dans l'exercice 1.2 de TP 2

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);

	for (int ii = 0; ii < DIM_FOREST; ii++){
		glPushMatrix();
		//glTranslatef(positions_arbres_X[i], O, positions_arbres_Z[i]);

		// quand le terrain n'est pas plat, on doit positioner les arbres sur le terrain ... donc la position sur Y depend de l'hauteur du terrain
		float I = positions_arbres_X[ii] / horisontal_resolution;
		float J = positions_arbres_Z[ii] / horisontal_resolution;
		int i = (int)I + (int)heightmap->lenx / 2;
		int j = (int)J + (int)heightmap->leny / 2;
		
		float pp = ((float)heightmap->data[3 * (i + heightmap->lenx*(j + 0))] +
			(float)heightmap->data[3 * (i + heightmap->lenx*(j + 1))] +
			(float)heightmap->data[3 * (i + 1 + heightmap->lenx*(j + 0))] +
			(float)heightmap->data[3 * (i + 1 + heightmap->lenx*(j + 1))]) / 4;

		float Y_arbre = vertical_scale*pp;
		// end calcul positionement sur Y

		glTranslatef(positions_arbres_X[ii], Y_arbre, positions_arbres_Z[ii]);

		glBegin(GL_QUADS);
			glTexCoord2f(0, 0); glVertex3f(-5, 0, 0);
			glTexCoord2f(0, 1); glVertex3f(-5, 20, 0);
			glTexCoord2f(1, 1); glVertex3f(5, 20, 0);
			glTexCoord2f(1, 0); glVertex3f(5, 0, 0);
		glEnd();

		glPushMatrix();
			glRotatef(90, 0, 1, 0);
			glBegin(GL_QUADS);
				glTexCoord2f(0, 0); glVertex3f(-5, 0, 0);
				glTexCoord2f(0, 1); glVertex3f(-5, 20, 0);
				glTexCoord2f(1, 1); glVertex3f(5, 20, 0);
				glTexCoord2f(1, 0); glVertex3f(5, 0, 0);
			glEnd();
		glPopMatrix();
		glPopMatrix();
	}

	glPolygonMode(GL_FRONT, GL_FILL);					// front of a face is filled
	glPolygonMode(GL_BACK, GL_LINE);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
}
/////////////////// TP2 end bonus //////////////////	


/////////////////// TP2 exercice 3.1 le raptor //////////////////	
void display_raptor()
{
	glBindTexture(GL_TEXTURE_2D,texture_raptor->OpenGL_ID[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// on répète la texture en cas de U,V > 1.0
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// ou < 0.0
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // indique qu'il faut mélanger la texture avec la couleur courante

	// charge le tableau de la texture en mémoire vidéo et crée une texture mipmap

	if (texture_raptor->isRGBA)
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, texture_raptor->img_color->lenx, texture_raptor->img_color->leny, GL_RGBA, GL_UNSIGNED_BYTE, texture_raptor->img_all);
	else
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, texture_raptor->img_color->lenx, texture_raptor->img_color->leny, GL_RGB, GL_UNSIGNED_BYTE, texture_raptor->img_color->data);

	glEnable(GL_TEXTURE_2D);

	glPushMatrix();

	glTranslatef(raptorX, raptorY, raptorZ); // pour le positioner sur le terrain
	glRotatef(angleRaptorX,1,0,0);
	glRotatef(angleRaptorY,0,1,0);	
	glRotatef(angleRaptorZ, 0, 0, 1);
	glScalef(0.1,0.1,0.1);	


	glBegin(GL_TRIANGLES);
		for(int i=0;i<geometrie_raptor->nb_triangles;i++){
			glTexCoord2f(geometrie_raptor->points[geometrie_raptor->faces[i].a].ucol,geometrie_raptor->points[geometrie_raptor->faces[i].a].vcol);			
			glVertex3f(geometrie_raptor->points[geometrie_raptor->faces[i].a].x,geometrie_raptor->points[geometrie_raptor->faces[i].a].y,geometrie_raptor->points[geometrie_raptor->faces[i].a].z);
			
			glTexCoord2f(geometrie_raptor->points[geometrie_raptor->faces[i].b].ucol,geometrie_raptor->points[geometrie_raptor->faces[i].b].vcol);
			glVertex3f(geometrie_raptor->points[geometrie_raptor->faces[i].b].x,geometrie_raptor->points[geometrie_raptor->faces[i].b].y,geometrie_raptor->points[geometrie_raptor->faces[i].b].z);
			
			glTexCoord2f(geometrie_raptor->points[geometrie_raptor->faces[i].c].ucol,geometrie_raptor->points[geometrie_raptor->faces[i].c].vcol);
			glVertex3f(geometrie_raptor->points[geometrie_raptor->faces[i].c].x,geometrie_raptor->points[geometrie_raptor->faces[i].c].y,geometrie_raptor->points[geometrie_raptor->faces[i].c].z);
		}
	 glEnd();
	 glPopMatrix();
	 glDisable(GL_TEXTURE_2D);
}

void move_raptor() {
	speedRaptorX = (cam->position.x - raptorX)/80;
	speedRaptorZ = (cam->position.z - raptorZ)/80;
	if (redMoon) {
		raptorX += 2*(speedCount/2)*speedRaptorX;
		raptorZ += 2* (speedCount / 2)*speedRaptorZ;
	}
	else {
		raptorX += speedRaptorX;
		raptorZ += speedRaptorZ;
	}
	//angleRaptorZ = 40 + acos(((raptorX * cam->position.x) + (raptorZ * cam->position.z)) / (sqrt(pow(raptorX, 2)+pow(raptorZ, 2))*(sqrt(pow(cam->position.x, 2) + pow(cam->position.z, 2)))));
	angleRaptorZ = atan(((cam->position.x) - (raptorX)) / ((cam->position.z) - (raptorZ)));
	if (angleRaptorZ > 360) {
		angleRaptorZ = angleRaptorZ - 360;
	}
	float I = raptorX / horisontal_resolution;
	float J = raptorZ / horisontal_resolution;
	int i = (int)I + (int)heightmap->lenx / 2;
	int j = (int)J + (int)heightmap->leny / 2;

	float pp = ((float)heightmap->data[3 * (i + heightmap->lenx*(j + 0))] +
		(float)heightmap->data[3 * (i + heightmap->lenx*(j + 1))] +
		(float)heightmap->data[3 * (i + 1 + heightmap->lenx*(j + 0))] +
		(float)heightmap->data[3 * (i + 1 + heightmap->lenx*(j + 1))]) / 4;

	raptorY = vertical_scale * pp; // 
}

/////////////////// TP2 end exercice 3.1 le raptor //////////////////	

/////////////////// TP3 exercice 1.1 le terrain //////////////////	
void display_terrain()
{

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture_terrain->OpenGL_ID[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// on répète la texture en cas de U,V > 1.0
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// ou < 0.0
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // indique qu'il faut mélanger la texture avec la couleur courante

	// charge le tableau de la texture en mémoire vidéo et crée une texture mipmap

	if (texture_terrain->isRGBA)
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, texture_terrain->img_color->lenx, texture_terrain->img_color->leny, GL_RGBA, GL_UNSIGNED_BYTE, texture_terrain->img_all);
	else
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, texture_terrain->img_color->lenx, texture_terrain->img_color->leny, GL_RGB, GL_UNSIGNED_BYTE, texture_terrain->img_color->data);

	glBegin(GL_TRIANGLES);
	for (int i=0;i<(int)heightmap->lenx-1;i++)
	{
		for (int j=0;j<(int)heightmap->leny-1;j++)
		{
			float I = i-(int)heightmap->lenx/2;
			float J = j-(int)heightmap->leny/2;

			glTexCoord2f((float)i/(float)heightmap->lenx,(float)j/(float)heightmap->leny);
			glVertex3f((I+0)*horisontal_resolution, vertical_scale*(float)heightmap->data[3*(i+  heightmap->lenx*(j+0))],(J+0)*horisontal_resolution);

			glTexCoord2f((float)i/(float)heightmap->lenx,(float)(j+1)/(float)heightmap->leny);
			glVertex3f((I+0)*horisontal_resolution, vertical_scale*(float)heightmap->data[3*(i+  heightmap->lenx*(j+1))],(J+1)*horisontal_resolution);

			glTexCoord2f((float)(i+1)/(float)heightmap->lenx,(float)j/(float)heightmap->leny);
			glVertex3f((I+1)*horisontal_resolution, vertical_scale*(float)heightmap->data[3*(i+1+heightmap->lenx*(j+0))],(J+0)*horisontal_resolution);

			glTexCoord2f((float)i/(float)heightmap->lenx,(float)(j+1)/(float)heightmap->leny);
			glVertex3f((I+0)*horisontal_resolution, vertical_scale*(float)heightmap->data[3*(i+  heightmap->lenx*(j+1))],(J+1)*horisontal_resolution);

			glTexCoord2f((float)(i+1)/(float)heightmap->lenx,(float)(j+1)/(float)heightmap->leny);
			glVertex3f((I+1)*horisontal_resolution, vertical_scale*(float)heightmap->data[3*(i+1+heightmap->lenx*(j+1))],(J+1)*horisontal_resolution);

			glTexCoord2f((float)(i+1)/(float)heightmap->lenx,(float)j/(float)heightmap->leny);
			glVertex3f((I+1)*horisontal_resolution, vertical_scale*(float)heightmap->data[3*(i+1+heightmap->lenx*(j+0))],(J+0)*horisontal_resolution);
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

}
/////////////////// TP3 end exercice 1.1 le terrain //////////////////	

void scoreInc() {
	score++;
	sprintf(scoreChar, "%d", score);
	//sprintf(scoreChar, "You lose");

}

bool endGame() {
	if (sqrt(pow((posX - raptorX), 2) + pow((posZ - raptorZ), 2)) <= 10) {
		end = true;
	}
	return end;
}

void distance_pomme() {
	distance = sqrt(pow((pommeX - posX), 2) + pow((pommeZ - posZ), 2));
	//distance = sqrt(pow((pommeX - posX), 2) + pow((pommeY - posY), 2) + pow((pommeZ - posZ), 2));
	if (distance <= 10) {
		scoreInc();
		pommeX += rand_a_b(-100, 100);
		pommeZ += rand_a_b(-100, 100);
		while (sqrt(pow((pommeX - posX*2), 2) + pow((pommeZ - posZ), 2)) < 30) {
			pommeX += rand_a_b(-100, 100);
			pommeZ += rand_a_b(-100, 100);
		}
		float Ipomme = pommeX / horisontal_resolution;
		float Jpomme = pommeZ / horisontal_resolution;
		int ipomme = (int)Ipomme + (int)heightmap->lenx / 2;
		int jpomme = (int)Jpomme + (int)heightmap->leny / 2;

		float pppomme = ((float)heightmap->data[3 * (ipomme + heightmap->lenx*(jpomme + 0))] +
			(float)heightmap->data[3 * (ipomme + heightmap->lenx*(jpomme + 1))] +
			(float)heightmap->data[3 * (ipomme + 1 + heightmap->lenx*(jpomme + 0))] +
			(float)heightmap->data[3 * (ipomme + 1 + heightmap->lenx*(jpomme + 1))]) / 4;

		pommeY = 5 + vertical_scale * pppomme;  

	}
}

/********************************************************************\
*                                                                    *
*  Boucle principale, appelée pour construire une image, gérer les   *
*  entrées.                                                          *
*                                                                    *
\********************************************************************/
void main_loop()
{

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//				gestion des touches	et du temps													//
	//////////////////////////////////////////////////////////////////////////////////////////////////

	inp->refresh();
	tim->update_horloge();
	inp->get_mouse_movement();

	if (inp->keys[KEY_CODE_ESCAPE])
	{
		PostMessage(win->handle, WM_CLOSE, 0, 0);	// Stoppe la "pompe à message" en y envoyant le message "QUIT"
	}


	if (tim->global_timer_25_Hz)				// augmente incrementAngleY tous les 20ème de seconde
		incrementAngleY += 1.0f;
	if (incrementAngleY >= 360) incrementAngleY -= 360;





	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (endGame() == false) {
	// Clear The Screen And The Depth Buffer
		//////////////////////////////////////////////////////////////////////////////////////////////////
		//						ça commence ici															//
		//////////////////////////////////////////////////////////////////////////////////////////////////

		// votre code OpenGL ici


	// TP1 exercice 1.1
		if (redMoon)
			glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		else
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		// end TP1 exercice 1.1


		glMatrixMode(GL_PROJECTION);  //la matrice de projection sera celle selectionnee
		//composition d'une matrice de projection
		glLoadIdentity(); //on choisit la matrice identité
		gluPerspective(60, (double)win->Xres / (double)win->Yres, 1, 10000);   //mise en place d une proj angle de vue 60 deg near 10 far 30000


	//////////////// TP3 exercice 2.1 controle de la camera

		point dir = (cam->direction - cam->position) * 2;
		if (inp->keys[KEY_CODE_UP]) {
			posX += dir.x;
			posZ += dir.z;
		}
		if (inp->keys[KEY_CODE_DOWN]) {
			posX -= dir.x;
			posZ -= dir.z;
		}

		if (inp->keys[KEY_CODE_RIGHT]) {
			posX += produit_vectoriel(dir, cam->orientation).x;
			posZ += produit_vectoriel(dir, cam->orientation).z;
		}
		if (inp->keys[KEY_CODE_LEFT]) {
			posX -= produit_vectoriel(dir, cam->orientation).x;
			posZ -= produit_vectoriel(dir, cam->orientation).z;
		}
		if (inp->keys[KEY_CODE_A]) {
			debug(distance);
		}


		angleX -= 0.1*(float)inp->Yrelmouse;
		angleY -= 0.1*(float)inp->Xrelmouse;
		if (angleY > 360) angleY -= 360; if (angleY < 0) angleY += 360;
		if (angleX > 60) angleX = 60;	if (angleX < -60) angleX = -60;

		float I = posX / horisontal_resolution;
		float J = posZ / horisontal_resolution;
		int i = (int)I + (int)heightmap->lenx / 2;
		int j = (int)J + (int)heightmap->leny / 2;

		float pp = ((float)heightmap->data[3 * (i + heightmap->lenx*(j + 0))] +
			(float)heightmap->data[3 * (i + heightmap->lenx*(j + 1))] +
			(float)heightmap->data[3 * (i + 1 + heightmap->lenx*(j + 0))] +
			(float)heightmap->data[3 * (i + 1 + heightmap->lenx*(j + 1))]) / 4;

		posY = 10 + vertical_scale * pp; // 

		cam->update(point(posX, posY, posZ), angleX, angleY, angleZ);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(cam->position.x, cam->position.y, cam->position.z,		// position
			cam->direction.x, cam->direction.y, cam->direction.z,		// point cible
			cam->orientation.x, cam->orientation.y, cam->orientation.z);		// vecteur up


		//writeText(cam->direction.x, cam->direction.y, cam->direction.z, "bonjour");

		//////////// TP2: bonus - optimized forest
		display_optimized_forest();
		//////////// TP2: end bonus - optimized forest

		//////////// TP2: exercice 3.1 le raptor
		if (tim->global_timer_10s) {
			redMoon = !redMoon;
			speedCount++;
		}

		move_raptor();
		display_raptor();
		//////////// TP2: end exercice 3.1 le raptor

		//////////// TP3: exercice 1.1 le terrain
		display_terrain();
		//////////// TP3: end exercice 1.1 le terrain
		display_pomme(pommeX, pommeY, pommeZ);
		distance_pomme();
		writeText(cam->direction.x, cam->direction.y, cam->direction.z, scoreChar);
	}

	swap_buffer(win);	// affiche l'image composée à l'écran

	if (endGame() == true) {
		cam->position = point(0, 0, 0);
		cam->direction = cam->position + point(0, 0, -1);
		cam->orientation = point(0, 1, 0);
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

		glMatrixMode(GL_PROJECTION);  //la matrice de projection sera celle selectionnee
									  //composition d'une matrice de projection
		glLoadIdentity(); //on choisit la matrice identité
		gluPerspective(60, (double)win->Xres / (double)win->Yres, 10, 30000);   //mise en place d une proj angle de vue 60 deg near 10 far 30000
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0, 0, -400,		// position
			0, 0, 1,		// point cible
			0, 1, 0);		// vecteur up*/
		writeText(0,0,1,scoreChar);

		if (inp->keys[KEY_CODE_ENTER]) {
			posX = 0;
			posY = 30;
			posZ = 0;
			angleX = 0;
			angleY = 0;
			angleZ = 0;
			pommeX = 40;
			pommeY = 30;
			pommeZ = 40;
			raptorX = 100;
			raptorY = 10;
			raptorZ = 100;
			angleRaptorX = 90;
			angleRaptorY = 180;
			angleRaptorZ = 0;
			speedRaptorX = 0;
			speedRaptorY = 0;
			speedRaptorZ = 0;
			distance = 0;
			distanceRaptor = 0;
			score = -1;
			sprintf(scoreChar, "%d", score);
			buffer[64];
			end = false;
			redMoon = false;
			speedCount = 1;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////
	//						ça finit ici															//
	//////////////////////////////////////////////////////////////////////////////////////////////////
}


/********************************************************************\
*                                                                    *
* Arrête l'application                                               *
*                                                                    *
\********************************************************************/
void stop()
{

	delete geometrie_raptor;
	delete texture_arbre;
	delete texture_raptor;
	delete texture_logo;
	delete positions_arbres_X;
	delete positions_arbres_Z;

	delete heightmap;
	delete texture_terrain;
	delete cam;	
	delete inp;
	delete tim;

	if (win)
	{
		kill_font();
		kill_context(*win);
		delete win;
	}
}




/********************************************************************\
*                                                                    *
* Point d'entrée de notre programme pour Windows ("WIN32")           *
*                                                                    *
\********************************************************************/
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR szCmdLine,int iCmdShow)
{
	MSG  msg;

	win	= NULL;
	inp	= NULL;
	tim = NULL;

	if (start() == false)								// initialise la fenêtre et OpenGL
	{
		debug("start() : échec !");
		stop();
		return 1;
	}

	// main loop //
	// --------- //
	//   __
	//  /  \_
	//  |  \/ 
	//  \__/

	while (true)
	{
		if (PeekMessage(&msg, NULL,0,0,PM_NOREMOVE))	// s'il y a un message, appelle WndProc() pour le traiter
		{
			if (!GetMessage(&msg,NULL,0,0))				// "pompe à message"
				break;
			TranslateMessage(&msg);
			DispatchMessage (&msg);
		}
		else 
		{
			main_loop();								// sinon, appelle main_loop()
		}
	}


	stop();												// arrête OpenGL et ferme la fenêtre

	return 0;
}



/********************************************************************\
*                                                                    *
*  Boucle des messages                                               *
*                                                                    *
\********************************************************************/
LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{

  switch (msg)
    {

      case WM_MOVE:        win->Xpos = (dword) LOWORD(lParam);
                           win->Ypos = (dword) HIWORD(lParam);
                           return 0;

      case WM_CLOSE:       PostQuitMessage(0); // dit à GetMessage() de renvoyer 0
                           return 0;

      case WM_SYSCOMMAND:  // évite l'économiseur d'écran
                           switch (wParam)
                             {
                               case SC_SCREENSAVE:
                               case SC_MONITORPOWER:
                               return 0;
                             }
                           break;
/*
      case WM_CHAR:        la touche est traduite dans ce msg 
                           return 0;
*/

      case WM_KEYDOWN:     inp->set_key_down ((dword)wParam);
                           return 0;

      case WM_KEYUP:       inp->set_key_up   ((dword)wParam);
                           return 0;

      case WM_LBUTTONDOWN: inp->set_mouse_left_down();
                           return 0;

      case WM_LBUTTONUP:   inp->set_mouse_left_up();
                           return 0;

      case WM_RBUTTONDOWN: inp->set_mouse_right_down();
                           return 0;

      case WM_RBUTTONUP:   inp->set_mouse_right_up();
                           return 0;

      case WM_MBUTTONDOWN: inp->set_mouse_middle_down();
                           return 0;

      case WM_MBUTTONUP:   inp->set_mouse_middle_up();
                           return 0;

   }

  return DefWindowProc(hwnd,msg,wParam,lParam);
}

