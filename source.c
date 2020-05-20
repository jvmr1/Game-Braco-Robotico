#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

#define PI 3.1415

int shoulder = -45, elbow = 120, hand = 45, rot_shoulder = 0;

float posx=0.0, posy=-8.0, xg, yg, zg, garrax1, garray1, garraz1, garrax2, garray2, garraz2, garrax3, garray3, garraz3;

float xq1 = -5.0, yq1 = 0.0, zq1=0.25;

float xq2 = 0.0, yq2 = 0.0, zq2=0.25;

float xq3 = 5.0, yq3 = 0.0, zq3=0.25;

float xc = 0.0, yc = 8.0, zc=0.5;

int pegou1=0, pegou2=0, pegou3=0;

GLint WIDTH =1300;
GLint HEIGHT=780;

// Variáveis para controles de navegação
GLfloat angle, fAspect;
GLfloat rotX, rotY, rotX_ini, rotY_ini;
GLfloat obsX, obsY, obsZ, obsX_ini, obsY_ini, obsZ_ini;
int x_ini, y_ini, bot;

int w,h;
GLuint texid1,texid2;

GLint fatias=30;
GLint pilhas=30;
GLint raioEsfera=1.5;

GLfloat obs[3]={0.0,7.0,0.0};
GLfloat olho[3]={0.0,3.0,0.0};

GLfloat plano_difusa[]    = { 0.5, 0.5, 0.0, 1.0 };
GLfloat plano_especular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat plano_brilho[]    = { 50.0 };

GLfloat mat_a_difusa[]    = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_a_especular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_a_brilho[]    = { 50.0 };

GLfloat mat_b_difusa[]    = { 0.7, 0.7, 0.7, 0.5 };
GLfloat mat_b_especular[] = { 1.0, 1.0, 1.0, 0.5 };
GLfloat mat_b_brilho[]    = { 50.0 };

GLfloat posicao_luz0[]    = { 0.0, 10.0, 0.0, 1.0};
GLfloat cor_luz0[]        = { 1.0, 1.0, 1.0, 1.0};
GLfloat cor_luz0_amb[]    = { 0.3, 0.3, 0.3, 1.0};

GLfloat posicao_luz1[]    = { 0.0, 10.0, 5.0, 1.0};
GLfloat cor_luz1[]        = { 0.0, 0.0, 1.0, 1.0};
GLfloat direcao_luz1[]    = { 0.0, -10.0, -5.0, 1.0};
GLint   spot_luz1         = 30;

GLfloat sem_cor[]         = { 0.0, 0.0, 0.0, 1.0};
  
GLint gouraud=0;

GLfloat tetaxz=0;
GLfloat raioxz=6;


void PosicionaObservador(void) {
    // Especifica sistema de coordenadas do modelo
    glMatrixMode(GL_MODELVIEW);
    // Inicializa sistema de coordenadas do modelo
    glLoadIdentity();
    // Posiciona e orienta o observador
    glTranslatef(-obsX, -obsY, -obsZ);
    glRotatef(rotX, 1, 0, 0);
    glRotatef(rotY, 0, 1, 0);
}


void reshape(int width, int height){
  WIDTH=width;
  HEIGHT=height;
  glViewport(0,0,(GLint)width,(GLint)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(angle,width/(float)height,0.1,120.0);
  glMatrixMode(GL_MODELVIEW);
  PosicionaObservador();
}


// Função callback para eventos de botões do mouse

void GerenciaMouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        // Salva os parâmetros atuais
        x_ini = x;
        y_ini = y;
        obsX_ini = obsX;
        obsY_ini = obsY;
        obsZ_ini = obsZ;
        rotX_ini = rotX;
        rotY_ini = rotY;
        bot = button;
    } else bot = -1;
}

// Função callback para eventos de movimento do mouse   
#define SENS_ROT 5.0
#define SENS_OBS 15.0
#define SENS_TRANSL 30.0
void GerenciaMovim(int x, int y) {
    // Botão esquerdo ?
    if (bot == GLUT_LEFT_BUTTON) {
        // Calcula diferenças
        int deltax = x_ini - x;
        int deltay = y_ini - y;
        // E modifica ângulos
        rotY = rotY_ini - deltax / SENS_ROT;
        rotX = rotX_ini - deltay / SENS_ROT;
    }// Botão direito ?
    else if (bot == GLUT_RIGHT_BUTTON) { //tentar fazer com a roda do mouse
        // Calcula diferença
        int deltaz = y_ini - y;
        // E modifica distância do observador
        obsZ = obsZ_ini + deltaz / SENS_OBS;
    }// Botão do meio ?
    else if (bot == GLUT_MIDDLE_BUTTON) {
        // Calcula diferenças
        int deltax = x_ini - x;
        int deltay = y_ini - y;
        // E modifica posições
        obsX = obsX_ini + deltax / SENS_TRANSL;
        obsY = obsY_ini - deltay / SENS_TRANSL;
    }
    PosicionaObservador();
    glutPostRedisplay();
}

char *carrega_bmp(char const * name) {
    FILE *f = fopen(name, "rb");
    if (f == NULL) {
        printf("Erro ao carregar o arquivo %s!", name);
        exit(1);
    }
    int of;
    fseek(f, 10, SEEK_SET);
    fread(&of, sizeof (int), 1, f);
    fseek(f, 4, SEEK_CUR);
    fread(&w, sizeof (int), 1, f);
    fread(&h, sizeof (int), 1, f);

    fseek(f, of, SEEK_SET);

    int by = ((w * 3 + 3) / 4) * 4 - (w * 3 % 4);
    char *tmp_m = (char *) malloc(sizeof (char) * by * h);
    char *m = (char *) malloc(sizeof (char) * w * h * 3);
    fread(tmp_m, sizeof (char) * by * h, 1, f);
    int x, y, i;
    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            for (i = 0; i < 3; i++) {
                m[3 * (w * y + x) + i] = tmp_m[3 * (w * y + x) + (2 - i)];
            }
        }
    }
    free(tmp_m);
    return m;
}

GLuint carregaTextura(char const *c) {

    char *wa = carrega_bmp(c);
    GLuint texid;
    glGenTextures(1, &texid);
    glBindTexture(GL_TEXTURE_2D, texid);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, wa);
    free(wa);
    return texid;
}

//Carrega textura do piso
void colocaMetal() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texid1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void display(void){
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);

  glDepthMask(GL_TRUE);
  glClearColor(1.0,1.0,1.0,1.0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  if(gouraud){
    glShadeModel(GL_SMOOTH);
  }
  else{
    glShadeModel(GL_FLAT);
  }    
  
  glPushMatrix();

  // propriedades do material do plano
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, plano_difusa);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, plano_especular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, plano_brilho);

  glMaterialfv(GL_FRONT, GL_EMISSION, sem_cor);

  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_a_difusa);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_a_especular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_a_brilho);

  //desenha braco
  glPushMatrix();
  glTranslatef(-posy,0.0,-posx);

		glTranslatef (0.0, 0.5, 0.0); // fazer base
			glPushMatrix();
			glScalef (1.0, 1.0, 1.0);
			glutSolidCube (1.0);
			glPopMatrix ();
		glTranslatef (0.0, -0.5, 0.0); 

		glTranslatef (0.0, 1.0, 0.0); // eixo no ombro 
			glRotatef ((GLfloat) -rot_shoulder, 0.0, 1.0, 0.0);
			glRotatef ((GLfloat) shoulder, 0.0, 0.0, 1.0);
		
		glTranslatef (0.0, 1.0, 0.0); // fazer braço 
			glPushMatrix();
			glScalef (0.5, 2.0, 0.5);
			glutSolidCube (1.0);
			glPopMatrix ();

		glTranslatef (0.0, 1.0, 0.0); // eixo no cotovelo
			glRotatef ((GLfloat) elbow, 0.0, 0.0, 1.0);
		
		glTranslatef (0.0, 1.0, 0.0); // fazer antebraço
			glPushMatrix();
			glScalef (0.5, 2.0, 0.5);
			glutSolidCube (1.0);
			glPopMatrix ();

		glTranslatef (0.0, 1.0, 0.0); // eixo no pulso
			glRotatef ((GLfloat) 90, 0.0, 1.0, 0.0);
	
		glPushMatrix(); // fazer mao
			glRotatef ((GLfloat) hand, 0.0, 0.0, 1.0);
			glRotatef (135, 0.0, 0.0, 1.0);
			glTranslatef (0.25, 0.0, 0.0);
				glPushMatrix();
				glScalef (0.5, 0.125, 0.125);
				glutSolidCube (1.0);
				glPopMatrix ();
			glTranslatef (0.25, 0.0, 0.0);
			glRotatef (-90, 0.0, 0.0, 1.0);
			glTranslatef (0.25, 0.0, 0.0);
				glPushMatrix();
				glScalef (0.5, 0.125, 0.125);
				glutSolidCube (1.0);
				glPopMatrix ();
			glTranslatef (-0.25, 0.0, 0.0);
			glRotatef (90, 0.0, 0.0, 1.0);
			glTranslatef (-0.5, 0.0, 0.0);
			glRotatef (-45, 0.0, 0.0, 1.0);
		glPopMatrix ();
		
		glPushMatrix();
			glRotatef ((GLfloat) -hand, 0.0, 0.0, 1.0);
			glRotatef (45, 0.0, 0.0, 1.0);
			glTranslatef (0.25, 0.0, 0.0);
				glPushMatrix();
				glScalef (0.5, 0.125, 0.125);
				glutSolidCube (1.0);
				glPopMatrix ();
			glTranslatef (0.25, 0.0, 0.0);
			glRotatef (90, 0.0, 0.0, 1.0);
			glTranslatef (0.25, 0.0, 0.0);
				glPushMatrix();
				glScalef (0.5, 0.125, 0.125);
				glutSolidCube (1.0);
				glPopMatrix ();
			glTranslatef (-0.25, 0.0, 0.0);
			glRotatef (-90, 0.0, 0.0, 1.0);
			glTranslatef (-0.5, 0.0, 0.0);
			glRotatef (45, 0.0, 0.0, 1.0);
		glPopMatrix ();
	
  glPopMatrix();

  //fazer caixinha 1
        glPushMatrix();
        glTranslatef(-yq1, zq1, -xq1);
        glColor3f(0.4, 0.3, 0.0);
        glScalef(0.5, 0.5, 0.5);
        glutSolidCube(1.0);
        glPopMatrix();

  //fazer caixinha 2
        glPushMatrix();
        glTranslatef(-yq2, zq2, -xq2);
        glColor3f(0.4, 0.3, 0.0);
        glScalef(0.5, 0.5, 0.5);
        glutSolidCube(1.0);
        glPopMatrix();

  //fazer caixinha 3
        glPushMatrix();
        glTranslatef(-yq3, zq3, -xq3);
        glColor3f(0.4, 0.3, 0.0);
        glScalef(0.5, 0.5, 0.5);
        glutSolidCube(1.0);
        glPopMatrix();

  //fazer caixona
        glPushMatrix();
        glTranslatef(-yc, zc, -xc);
        glColor3f(0.4, 0.3, 0.0);
        glScalef(1, 1.0, 3);
        glutSolidCube(1.0);
        glPopMatrix();

  //desenha plano com textura
		glPushMatrix();
			colocaMetal();

			glColor3f(0.2,0.2,0.1);
			glPushMatrix();
			glRotatef(90,1,0,0);
			glBegin(GL_QUADS);
				glTexCoord2f(1.0f, 0.0f);glVertex2i(-10.0, -10.0);
				glTexCoord2f(1.0f, 1.0f);glVertex2i(10.0,-10.0);
				glTexCoord2f(0.0f, 1.0f);glVertex2i(10.0, 10.0);
				glTexCoord2f(0.0f, 0.0f);glVertex2i(-10.0, 10.0);
			glEnd();

			glPopMatrix();

		glPopMatrix();
	
		glDisable(GL_TEXTURE_2D);

  glPopMatrix();

  glutSwapBuffers();

}


void keyboard(unsigned char key, int x, int y){
	if (key==27){
	    exit(0);
	}

	else if (key=='1'){
		rot_shoulder = (rot_shoulder + 5);
    }
	else if (key=='!'){
		rot_shoulder = (rot_shoulder - 5);
	}
	else if (key=='2'){
		if (shoulder<90){
	    	shoulder = (shoulder + 5) % 360;
		}
	}
	else if (key=='@'){
		if (shoulder>-90){
			shoulder = (shoulder - 5) % 360;
		}
	}
	else if (key=='3'){
		if (elbow<160){
			elbow = (elbow + 5) % 360;
		}
    }

	else if (key=='#'){
		if (elbow>-160){
			elbow = (elbow - 5) % 360;
		}
    }
	else if (key=='4'){
		if (hand==45){		
			hand=0;
	
			if (((xq1-0.25)<xg) && (xg<(xq1+0.25)) && ((yq1-0.25)<yg) && (yg<(yq1+0.25)) && ((zq1-0.25)<zg) && (zg<(zq1+0.25))){
				pegou1=1;
			}
			if (((xq2-0.25)<xg) && (xg<(xq2+0.25)) && ((yq2-0.25)<yg) && (yg<(yq2+0.25)) && ((zq2-0.25)<zg) && (zg<(zq2+0.25))){
				pegou2=1;
			}
			if (((xq3-0.25)<xg) && (xg<(xq3+0.25)) && ((yq3-0.25)<yg) && (yg<(yq3+0.25)) && ((zq3-0.25)<zg) && (zg<(zq3+0.25))){
				pegou3=1;
			}
		}
	}
 
	else if (key=='$'){
		if (hand==0){
			hand=45;

			if (((xq1-0.25)<xg) && (xg<(xq1+0.25)) && ((yq1-0.25)<yg) && (yg<(yq1+0.25)) && ((zq1-0.25)<zg) && (zg<(zq1+0.25))){
				pegou1=0;
			}
			if (((xq2-0.25)<xg) && (xg<(xq2+0.25)) && ((yq2-0.25)<yg) && (yg<(yq2+0.25)) && ((zq2-0.25)<zg) && (zg<(zq2+0.25))){
				pegou2=0;
			}
			if (((xq3-0.25)<xg) && (xg<(xq3+0.25)) && ((yq3-0.25)<yg) && (yg<(yq3+0.25)) && ((zq3-0.25)<zg) && (zg<(zq3+0.25))){
				pegou3=0;
			}
		}
	}

	else if (key=='a'){
		if (posx>-10){
			posx = (posx - 0.5);
		}
    }
	else if (key=='d'){
		if (posx<10){
			posx = (posx + 0.5);	
		}
    }
	else if (key=='w'){
		if (posy<10){
			posy = (posy + 0.5);
		}
    }
	else if (key=='s'){
		if (posy>-10){
			posy = (posy - 0.5);
		}
    }

	garrax1=posx;
	garray1=posy;
	garraz1=1.0;

	garrax2=2.0*sin(shoulder*PI/180)*sin(rot_shoulder*PI/180);
	garray2=2.0*sin(shoulder*PI/180)*cos(rot_shoulder*PI/180);
	garraz2=2.0*cos(shoulder*PI/180);

	garrax3=2.5*sin(elbow*PI/180+shoulder*PI/180)*sin(rot_shoulder*PI/180); 
	garray3=2.5*sin(elbow*PI/180+shoulder*PI/180)*cos(rot_shoulder*PI/180); 
	garraz3=2.5*cos(elbow*PI/180+shoulder*PI/180); 

	xg=garrax1+garrax2+garrax3;
	yg=garray1+garray2+garray3;
	zg=garraz1+garraz2+garraz3;

	if(pegou1==1){
		xq1=garrax1+garrax2+garrax3;
		yq1=garray1+garray2+garray3;
		zq1=garraz1+garraz2+garraz3;
	}
	if(pegou2==1){
		xq2=garrax1+garrax2+garrax3;
		yq2=garray1+garray2+garray3;
		zq2=garraz1+garraz2+garraz3;
	}	
	if(pegou3==1){
		xq3=garrax1+garrax2+garrax3;
		yq3=garray1+garray2+garray3;
		zq3=garraz1+garraz2+garraz3;
	}		

	glutPostRedisplay();

}


void init(){

	gouraud=1;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, cor_luz0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, cor_luz0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz0_amb);
	glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz0);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, cor_luz1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, cor_luz1);
	glLightf (GL_LIGHT1, GL_SPOT_CUTOFF, spot_luz1);
	glLightfv(GL_LIGHT1, GL_POSITION, posicao_luz1);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direcao_luz1);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	glutMouseFunc(GerenciaMouse); // Registra a função callback para eventos de botões do mouse
	glutMotionFunc(GerenciaMovim); // Registra a função callback para eventos de movimento do mouse	

	angle = 60; // Inicializa a variável que especifica o ângulo da projeção perspectiva
	rotX = 60; 	// Inicializa as variáveis usadas para alterar a posição do observador virtual
	rotY = -90;
	obsX = 0;
	obsY = 0;
	obsZ = 25;

	texid1 = carregaTextura("floor.bmp");
	 
}

int main(int argc,char **argv){
	glutInitWindowPosition(0,0);
	glutInitWindowSize(WIDTH,HEIGHT);
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);

	glutCreateWindow("Game Braco Robotico");

	init();
	  
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutMainLoop();
	return(0);
}
