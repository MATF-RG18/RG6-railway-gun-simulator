#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
//USER ADDON: Kod trenutno je vecinom kopiran sa jednog od casova i editovan za potrebe projekta, 
//s izuzetkom funkcija za pravljenje ambijenta i jedne mete. Ovo ce se menjati
//po potrebi tako da cu naznaciti sta je ostalo od pocetnog koda kasnije
/* Definisemo osobine tajmera */
#define TIMER_INTERVAL 10
#define TIMER_ID 0

/* Definisemo makro koji vraca maksimalnu vrednost */
#define max(A, B) (((A) > (B)) ? (A) : (B))

/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void on_timer(int);
static float r=5.0,alfa=0.0,beta=45.0;
static float t;                 /* Proteklo vreme */
static int animation_ongoing;   /* Fleg koji odredjuje da li je
                                 * animacija u toku. */
static int active_function;     /* identifikator aktuelne funkcije */

int main(int argc, char **argv)
{
    /* Inicijalizacija promenljivih */
    t = 0;
    animation_ongoing = 0;
    active_function = 0;

    /* Inicijalizuje se GLUT. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /* Kreira se prozor. */
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    /* Registruju se callback funkcije. */
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    /* Obavlja se OpenGL inicijalizacija. */
    glClearColor(0.7, 0.7, 1, 0);
    glEnable(GL_DEPTH_TEST);

    /* Ukljucujemo normalizaciju vektora normala */
    glEnable(GL_NORMALIZE);

    /* Program ulazi u glavnu petlju. */
    glutMainLoop();

    return 0;
}

static void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        /* Zavrsava se program. */
        exit(0);
        break;

    case 'g':
    case 'G':
        /* Pokrece se animacija. */
        if (!animation_ongoing) {
            glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
            animation_ongoing = 1;
        }
        break;

    case 's':
    case 'S':
        /* Zaustavlja se animacija. */
        animation_ongoing = 0;
        break;

    case 'r':
    case 'R':
        /* Resetuje se proteklo vreme */
        t = 0;
        alfa=0;
        beta=0;
        glutPostRedisplay();
        break;
    case '4':
        alfa+=0.1;
        glutPostRedisplay();
        break;
    case '6':
        alfa-=0.1;
        glutPostRedisplay();
        break;
    case '2':
        beta-=0.1;
        glutPostRedisplay();
        break;
    case '8':
        beta+=0.1;
        glutPostRedisplay();
        break;
    case '+':
        r+=0.1;
        glutPostRedisplay();
        break;
    case '-':
        r-=0.1;
        glutPostRedisplay();
        break;
    }
}

static void on_reshape(int width, int height)
{
    /* Podesava se viewport. */
    glViewport(0, 0, width, height);

    /* Podesava se projekcija. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float) width / height, 1, 10);
}

static void on_timer(int value)
{
    /*
     * Proverava se da li callback dolazi od odgovarajuceg tajmera.
     */
    if (value != TIMER_ID) return;

    t += 5;

    /* Forsira se ponovno iscrtavanje prozora. */
    glutPostRedisplay();

    /* Po potrebi se ponovo postavlja tajmer. */
    if (animation_ongoing) {
        glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
    }
}
/* Crtamo funkciju */
void plot_plane()
{

    
    /* Koeficijenti ambijentalne refleksije materijala. */
    GLfloat ambient_coeffs[] = { 0.3, 0.7, 0.3, 1 };

    /* Koeficijenti difuzne refleksije materijala. */
    GLfloat diffuse_coeffs[] = { 0.2, 1, 0.2, 1 };

    /* Koeficijenti spekularne refleksije materijala. */
    GLfloat specular_coeffs[] = { 1, 0, 0, 1 };

    /* Koeficijent glatkosti materijala. */
    GLfloat shininess = 30;


    /* Podesavaju se parametri materijala. */
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    glShadeModel(GL_SMOOTH);
    glPushMatrix();
    glTranslatef(0,-0.1,0);
    glScalef(30,0.01,30);
    glutSolidCube(1);
    glPopMatrix();
}
void draw_cube(){
    /* Koeficijenti ambijentalne refleksije materijala. */
    GLfloat ambient_coeffs[] = { 0.3, 0.3, 1, 1 };

    /* Koeficijenti difuzne refleksije materijala. */
    GLfloat diffuse_coeffs[] = { 0.2, 0.2, 1, 1 };

    /* Koeficijenti spekularne refleksije materijala. */
    GLfloat specular_coeffs[] = { 1, 0, 0, 1 };

    /* Koeficijent glatkosti materijala. */
    GLfloat shininess = 30;


    /* Podesavaju se parametri materijala. */
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    glShadeModel(GL_SMOOTH);
    glPushMatrix();
    glTranslatef(-2,0,0);
    glutSolidCube(0.1);
    glPopMatrix();
}
void draw_sphere(){
    /* Koeficijenti ambijentalne refleksije materijala. */
    GLfloat ambient_coeffs[] = { 0.3, 0.3, 1, 1 };

    /* Koeficijenti difuzne refleksije materijala. */
    GLfloat diffuse_coeffs[] = { 0.2, 0.2, 1, 1 };

    /* Koeficijenti spekularne refleksije materijala. */
    GLfloat specular_coeffs[] = { 1, 0, 0, 1 };

    /* Koeficijent glatkosti materijala. */
    GLfloat shininess = 30;


    /* Podesavaju se parametri materijala. */
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    glShadeModel(GL_SMOOTH);
    glPushMatrix();
    glTranslatef(-2,0,2);
    glutSolidSphere(0.1,32,32);
    glPopMatrix();
}
void draw_rails(){
    /* Koeficijenti ambijentalne refleksije materijala. */
    GLfloat ambient_coeffs[] = { 0.3, 0.3, 0.3 };

    /* Koeficijenti difuzne refleksije materijala. */
    GLfloat diffuse_coeffs[] = { 0.4, 0.4, 0.4};

    /* Koeficijenti spekularne refleksije materijala. */
    GLfloat specular_coeffs[] = { 0.775,0.775,0.775};

    /* Koeficijent glatkosti materijala. */
    GLfloat shininess = 70;


    /* Podesavaju se parametri materijala. */
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    glShadeModel(GL_SMOOTH);
    glPushMatrix();
    glScalef(0.005,0.01,100);
    glutSolidCube(1);
    glPopMatrix();
}
void draw_plank(){
    /* Koeficijenti ambijentalne refleksije materijala. */
    GLfloat ambient_coeffs[] = { 0.7, 0.7, 0, 1 };

    /* Koeficijenti difuzne refleksije materijala. */
    GLfloat diffuse_coeffs[] = { 0.7, 0.7, 0, 1 };

    /* Koeficijenti spekularne refleksije materijala. */
    GLfloat specular_coeffs[] = { 0, 0, 0.7, 1 };

    /* Koeficijent glatkosti materijala. */
    GLfloat shininess = 40;


    /* Podesavaju se parametri materijala. */
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    glShadeModel(GL_SMOOTH);
    glPushMatrix();
    glScalef(0.2,0.05,0.1);
    glutSolidCube(1);
    glPopMatrix();
}
void draw_wheel(){
    /* Koeficijenti ambijentalne refleksije materijala. */
    GLfloat ambient_coeffs[] = { 1,0,0, 1 };

    /* Koeficijenti difuzne refleksije materijala. */
    GLfloat diffuse_coeffs[] = { 1,0,0, 1 };

    /* Koeficijenti spekularne refleksije materijala. */
    GLfloat specular_coeffs[] = { 1,0,0, 1 };

    /* Koeficijent glatkosti materijala. */
    GLfloat shininess = 20;


    /* Podesavaju se parametri materijala. */
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glDisable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glPushMatrix();
    GLUquadric* quad=gluNewQuadric();
    gluCylinder(quad,0.05,0.05,0.01,32,32);
    gluDisk(quad,0.01,0.05,32,32);
    glTranslatef(0,0.01,0);
    gluDisk(quad,0.01,0.05,32,32);
    glPopMatrix();
}
void draw_sidehold(){
    /* Koeficijenti ambijentalne refleksije materijala. */
    GLfloat ambient_coeffs[] = { 0.3, 0.3, 0.3 };

    /* Koeficijenti difuzne refleksije materijala. */
    GLfloat diffuse_coeffs[] = { 0.4, 0.4, 0.4};

    /* Koeficijenti spekularne refleksije materijala. */
    GLfloat specular_coeffs[] = { 0.775,0.775,0.775};

    /* Koeficijent glatkosti materijala. */
    GLfloat shininess = 20;


    /* Podesavaju se parametri materijala. */
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    glShadeModel(GL_SMOOTH);
    glPushMatrix();
    glScalef(0.05,0.075,0.4);
    glutSolidCube(1);
    glPopMatrix();
}
void draw_chassey(){
    /* Koeficijenti ambijentalne refleksije materijala. */
    GLfloat ambient_coeffs[] = { 0.8, 0, 0, 1 };

    /* Koeficijenti difuzne refleksije materijala. */
    GLfloat diffuse_coeffs[] = { 0.8, 0, 0, 1 };

    /* Koeficijenti spekularne refleksije materijala. */
    GLfloat specular_coeffs[] = { 0.7,0.7,0, 1 };

    /* Koeficijent glatkosti materijala. */
    GLfloat shininess = 50;


    /* Podesavaju se parametri materijala. */
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    glShadeModel(GL_SMOOTH);
    glPushMatrix();
    glScalef(0.2,0.05,0.3);
    glutSolidCube(1);
    glPopMatrix();
}
void draw_hold(){
    /* Koeficijenti ambijentalne refleksije materijala. */
    GLfloat ambient_coeffs[] = { 0.3, 0.3, 0.3 };

    /* Koeficijenti difuzne refleksije materijala. */
    GLfloat diffuse_coeffs[] = { 0.4, 0.4, 0.4};

    /* Koeficijenti spekularne refleksije materijala. */
    GLfloat specular_coeffs[] = { 0.775,0.775,0.775};

    /* Koeficijent glatkosti materijala. */
    GLfloat shininess = 20;


    /* Podesavaju se parametri materijala. */
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    glShadeModel(GL_SMOOTH);
    glPushMatrix();
    glScalef(0.2,0.075,0.2);
    glutSolidCube(1);
    glPopMatrix();
}
void draw_guard_part(){
    /* Koeficijenti ambijentalne refleksije materijala. */
    GLfloat ambient_coeffs[] = { 0, 0.7, 0, 1 };

    /* Koeficijenti difuzne refleksije materijala. */
    GLfloat diffuse_coeffs[] = { 0, 0.7, 0, 1 };

    /* Koeficijenti spekularne refleksije materijala. */
    GLfloat specular_coeffs[] = { 0.7,0.7,0, 1 };

    /* Koeficijent glatkosti materijala. */
    GLfloat shininess = 80;


    /* Podesavaju se parametri materijala. */
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    glShadeModel(GL_SMOOTH);
    glPushMatrix();
    glScalef(0.03,0.03,0.03);
    glutSolidCube(1);
    glPopMatrix();
}
void draw_hangar(){
    /* Koeficijenti ambijentalne refleksije materijala. */
    GLfloat ambient_coeffs[] = { 0.3, 0.3, 0.3 };

    /* Koeficijenti difuzne refleksije materijala. */
    GLfloat diffuse_coeffs[] = { 0.4, 0.4, 0.4};

    /* Koeficijenti spekularne refleksije materijala. */
    GLfloat specular_coeffs[] = { 0.775,0.775,0.775};

    /* Koeficijent glatkosti materijala. */
    GLfloat shininess = 20;


    /* Podesavaju se parametri materijala. */
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    glShadeModel(GL_SMOOTH);
    glPushMatrix();
    glutSolidSphere(0.03,32,32);
    glPopMatrix();
}
void draw_cannonball(){
    /* Koeficijenti ambijentalne refleksije materijala. */
    GLfloat ambient_coeffs[] = { 0.4, 0.4, 0.4 };

    /* Koeficijenti difuzne refleksije materijala. */
    GLfloat diffuse_coeffs[] = { 0.4, 0.4, 0.4};

    /* Koeficijenti spekularne refleksije materijala. */
    GLfloat specular_coeffs[] = { 0.775,0.775,0.775};

    /* Koeficijent glatkosti materijala. */
    GLfloat shininess = 20;


    /* Podesavaju se parametri materijala. */
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    glShadeModel(GL_SMOOTH);
    glPushMatrix();
    glutSolidSphere(0.01,64,64);
    glPopMatrix();
}
void draw_cannon(){
    /* Koeficijenti ambijentalne refleksije materijala. */
    GLfloat ambient_coeffs[] = { 0.3, 0.3, 0.3 };

    /* Koeficijenti difuzne refleksije materijala. */
    GLfloat diffuse_coeffs[] = { 0.4, 0.4, 0.4};

    /* Koeficijenti spekularne refleksije materijala. */
    GLfloat specular_coeffs[] = { 0.775,0.775,0.775};

    /* Koeficijent glatkosti materijala. */
    GLfloat shininess = 80;


    /* Podesavaju se parametri materijala. */
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glDisable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glPushMatrix();
    GLUquadric* quad=gluNewQuadric();
    gluCylinder(quad,0.01,0.01,0.75,32,32);
    glPopMatrix();
}
static void on_display(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    /* Pozicija svetla (u pitanju je direkcionalno svetlo). */
    GLfloat light_position[] = { 100, 100, 100, 0 };

    /* Ambijentalna boja svetla. */
    GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1 };

    /* Difuzna boja svetla. */
    GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1 };

    /* Spekularna boja svetla. */
    GLfloat light_specular[] = { 0.9, 0.9, 0.9, 1 };
    gluLookAt(r*sin(alfa),r*sin(beta),r*cos(alfa),0,0,0,0,1,0);
    /* Brise se prethodni sadrzaj prozora. */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /* Ukljucuje se osvjetljenje i podesavaju parametri svetla. */
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    /* Kreira se objekat. */
    glPushMatrix();
    glEnable(GL_CULL_FACE);
    plot_plane();
    draw_cube();
    draw_sphere();
    glPushMatrix();
    glTranslatef(2.8,0.02,0);
    draw_rails();
    glTranslatef(0.01,0,0);
    draw_rails();
    glTranslatef(0.18,0,0);
    draw_rails();
    glTranslatef(0.01,0,0);
    draw_rails();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(2.9,0,-10);
    for(int i=0;i<=40;i++){
	    draw_plank();
	    glTranslatef(0,0,0.5);
    }
    glPopMatrix();
    glPushMatrix();
    glRotatef(90,0,0,1);
    glTranslatef(0.05,-2.805,0);
    glRotatef(90,1,0,0);
    draw_wheel();
    glTranslatef(0,0.25,0);
    draw_wheel();
    glTranslatef(0,0,0.19);
    draw_wheel();
    glTranslatef(0,-0.25,0);
    draw_wheel();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(2.805,0.075,0.125);
    draw_sidehold();
    glTranslatef(0.195,0,0);
    draw_sidehold();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(2.9,0.1,0.1);
    draw_chassey();
    glTranslatef(0,0.05,0);
    draw_hold();
    glTranslatef(0,0.05,0);
    glPushMatrix();
    for(int i=0;i<360;i+=20){
        glPushMatrix();
        glRotatef(i,0,1,0);
        glTranslatef(0.05,0,0);
        draw_guard_part();
        glPopMatrix();
    }
    for(int i=0;i<3;i++){
        glTranslatef(0,0.02,0);
        for(int i=40;i<320;i+=20){
            glPushMatrix();
            glRotatef(i,0,1,0);
            glTranslatef(0.05,0,0);
            draw_guard_part();
            glPopMatrix();
        }
    }
    glPushMatrix();
    glTranslatef(0,-0.05,0); 
    glRotatef(90,0,1,0);
    glRotatef(-45,1,0,0);
    draw_hangar();
    draw_cannon();
    glPopMatrix();
    glTranslatef(0,-0.05,0);
    glTranslatef(0.75/sqrt(2),0.75/sqrt(2),0);
    draw_cannonball();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    /* Nova slika se salje na ekran. */
    glutSwapBuffers();
}





















