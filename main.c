#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
/* Definisemo osobine tajmera, ako želite sporiju simulaciju povečajte interval */
#define TIMER_INTERVAL 10
#define TIMER_ID 0
#define pi 3.141259
/* Deklaracije callback funkcija.*/
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void on_timer(int);
/*brojke za poziciju kamere, voza i cevi*/
static float r=5.0,alfa=90.0,beta=45.0,z=0,gama=-90,delta=45,eta=0;
/*koordinate đuleta. potrebno za detekciju kolizije*/
static float loptax,loptay,loptaz;
/*broje koje nam daju stanje meta*/
static float kockas=1,sferas=1,mkocka=1,msfera=1;
/*brojke za vetar*/
static int ugaovetra=0;
static float brzinavetra=0;
//da li je đule ispaljeno
static bool ispalio=0;
//string poruke korisniku
static char compoundstring[300];
//poeni
static int poeni=0;
//pomoćni string za brojeve
static char tmp[50];
/* Proteklo vreme */
static float t;        
/* Fleg koji određuje da li je animacija u toku. */         
static int animation_ongoing;

int main(int argc, char **argv)
{
    //main je većinom kopiran sa petog časa, primer sa ravnom površinom koja se menja uživo
    /* Inicijalizacija promenljivih */
    t = 0;
    animation_ongoing = 0;
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
    //naravno, sa izuzetkom ova dva reda
    ugaovetra=random()%360;
    brzinavetra=0.01*(random()%10);
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
            poeni-=10;
            glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
            animation_ongoing = 1;
        }
        break;
    //rotiraj kameru oko centra simulacije horizontalno
    case '4':
        alfa+=0.05;
        glutPostRedisplay();
        break;
    case '6':
        alfa-=0.05;
        glutPostRedisplay();
        break;
    //rotiraj kameru oko centra simulacije vertikalno
    case '2':
        beta-=0.05;
        glutPostRedisplay();
        break;
    case '8':
        beta+=0.05;
        glutPostRedisplay();
        break;
    //podesi blizinu kamere od centra simulacije
    case '+':
        r-=0.1;
        glutPostRedisplay();
        break;
    case '-':
        r+=0.1;
        glutPostRedisplay();
        break;
    //pomeri voz po šinama
    case 's':
    case 'S':
        z+=0.05;
        glutPostRedisplay();
        break;
    case 'w':
    case 'W':
        z-=0.05;
        glutPostRedisplay();
        break;
    //rotiraj dev horizontalno
    case 'a':
    case 'A':
        gama+=1;
        glutPostRedisplay();
        break;
    case 'd':
    case 'D':
        gama-=1;
        glutPostRedisplay();
        break;
    //rotiraj cev vertikalno
    case '5':
        delta+=1;
        glutPostRedisplay();
        break;
    case '0':
        delta-=1;
        glutPostRedisplay();
        break;
    }
}
//ova funkcija nije menjana sa časa
static void on_reshape(int width, int height)
{
    /* Podesava se viewport. */
    glViewport(0, 0, width, height);

    /* Podesava se projekcija. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float) width / height, 1, 10);
}
//funkcija za detekciju kolizije sa kockom, korišćenjem koordinata kocke i đuleta
static bool detekcijakocka(){
    if(abs(2-loptax)<=0.05 && abs(-loptay)<=0.05 && abs(-loptaz)<=0.05) return true;
    return false;
}
//funkcija za detekciju kolizije sa loptom, putem jednostavnog rastojanja između centra sfera
static bool detekcijasfera(){
    if(pow(-(2+loptax),2)+pow(0.1-loptay,2)+pow(2-loptaz,2)<=0.01) return true;
    return false;
}
static void on_timer(int value)
{ 
    /*
     * Proverava se da li callback dolazi od odgovarajuceg tajmera.
     */
    if (value != TIMER_ID) return;
    //tokom letenja đuleta redovno se update-uje lokacija njega i da li je pogodio išta
    t += 1;
    loptax+=0.2*sin(gama*2*pi/360)*cos(delta*2*pi/360)+brzinavetra*sin(ugaovetra*2*pi/360.0);
    loptay+=0.2*sin(delta*2*pi/360)-0.01*t;
    loptaz+=0.2*cos(gama*2*pi/360)*cos(delta*2*pi/360)+brzinavetra*cos(ugaovetra*2*pi/360.0);
    if(kockas==1 && mkocka==1 && detekcijakocka()){
        kockas=0;
        glutPostRedisplay();
    }
    else if(sferas==1 && msfera==1 && detekcijasfera()){
        sferas=0;
    }
    //ako je pogodio zemlju, naznači da sfera više nije u vazduhu
    else if(loptay<=0 && ispalio==0){
        animation_ongoing=0;
        t=0;
    }
    //ako je kocka pogođena, naznači da đule više nije u vazduhu i daj poene
    if(kockas==0 && mkocka==1){
        ispalio=0;
        mkocka=0;
        animation_ongoing=0;
        poeni+=100;
        t=0;
        glutPostRedisplay();
        printf("Pogodili ste kocku!\n");
    }
    //ako je sfera pogođena, naznači da đule više nije u vazduhu i daj poene
    if(sferas==0 && msfera==1){
        ispalio=0;
        msfera=0;
        animation_ongoing=0;
        poeni+=100;
        t=0;
        glutPostRedisplay();
        printf("Pogodili ste sferu!\n");
    }
    /* Forsira se ponovno iscrtavanje prozora. */
    glutPostRedisplay();

    /* Po potrebi se ponovo postavlja tajmer. */
    if (animation_ongoing) {
        glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
    }
}
/* Crtamo ravnu zemlju, funkcije su kopirane i adaptirane za sam program */
void plot_plane()
{
    //Koeficijenti ambijentalne refleksije materijala
    GLfloat ambient_coeffs[] = { 0.3, 0.7, 0.3, 1 };
    //Koeficijenti difuzne refleksije materijala
    GLfloat diffuse_coeffs[] = { 0.2, 1, 0.2, 1 };
    //Koeficijenti spekularne refleksije materijala
    GLfloat specular_coeffs[] = { 1, 0, 0, 1 };
    //Koeficijent glatkosti materijala
    GLfloat shininess = 30;    
    //Uključivanje parametara materijala
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    //ravan je izuzetno tanak kvadar
    glShadeModel(GL_SMOOTH);
    glPushMatrix();
    glTranslatef(0,-0.1,0);
    glScalef(30,0.01,30);
    glutSolidCube(1);
    glPopMatrix();
}
//kocka
void draw_cube(){
    //Koeficijenti ambijentalne refleksije materijala
    GLfloat ambient_coeffs[] = { 0.3, 0.3, 1, 1 };
    //Koeficijenti difuzne refleksije materijala
    GLfloat diffuse_coeffs[] = { 0.2, 0.2, 1, 1 };
    //Koeficijenti spekularne refleksije materijala
    GLfloat specular_coeffs[] = { 1, 0, 0, 1 };
    //Koeficijent glatkosti materijala
    GLfloat shininess = 30;    
    //Uključivanje parametara materijala
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    glShadeModel(GL_SMOOTH);
    //kontruisanje kocke
    glPushMatrix();
    glTranslatef(-2,0.1,0);
    glScalef(1,kockas,1);
    glutSolidCube(0.2);
    glPopMatrix();
}
//sfera
void draw_sphere(){
    //Koeficijenti ambijentalne refleksije materijala
    GLfloat ambient_coeffs[] = { 0.3, 0.3, 1, 1 };
    //Koeficijenti difuzne refleksije materijala
    GLfloat diffuse_coeffs[] = { 0.2, 0.2, 1, 1 };
    //Koeficijenti spekularne refleksije materijala
    GLfloat specular_coeffs[] = { 1, 0, 0, 1 };
    //Koeficijent glatkosti materijala
    GLfloat shininess = 30;    
    //Uključivanje parametara materijala
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    //konstruisanje sfere
    glShadeModel(GL_SMOOTH);
    glPushMatrix();
    glTranslatef(-2,0,2);
    glutSolidSphere(0.1,32,32);
    glPopMatrix();
}
//šina, zovemo je 4 puta da bismo dobili čitavu traku
void draw_rails(){
    //Koeficijenti ambijentalne refleksije materijala
    GLfloat ambient_coeffs[] = { 0.3, 0.3, 0.3 };
    //Koeficijenti difuzne refleksije materijala
    GLfloat diffuse_coeffs[] = { 0.4, 0.4, 0.4};
    //Koeficijenti spekularne refleksije materijala
    GLfloat specular_coeffs[] = { 0.775,0.775,0.775};
    //Koeficijent glatkosti materijala
    GLfloat shininess = 70;    
    //Uključivanje parametara materijala
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    //konstruisanje šine kao izuzetno uskog kvadra
    glShadeModel(GL_SMOOTH);
    glPushMatrix();
    glScalef(0.005,0.01,100);
    glutSolidCube(1);
    glPopMatrix();
}
//daske koje se stavljaju ispod šina
void draw_plank(){
    //Koeficijenti ambijentalne refleksije materijala
    GLfloat ambient_coeffs[] = { 0.7, 0.7, 0, 1 };
    //Koeficijenti difuzne refleksije materijala
    GLfloat diffuse_coeffs[] = { 0.7, 0.7, 0, 1 };
    //Koeficijenti spekularne refleksije materijala
    GLfloat specular_coeffs[] = { 0, 0, 0.7, 1 };
    //Koeficijent glatkosti materijala
    GLfloat shininess = 40;    
    //Uključivanje parametara materijala
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    //konstruisanje daske, tanak kvadar
    glShadeModel(GL_SMOOTH);
    glPushMatrix();
    glScalef(0.2,0.05,0.1);
    glutSolidCube(1);
    glPopMatrix();
}
//točak voza
void draw_wheel(){
    //Koeficijenti ambijentalne refleksije materijala
    GLfloat ambient_coeffs[] = { 1,0,0, 1 };
    //Koeficijenti difuzne refleksije materijala
    GLfloat diffuse_coeffs[] = { 1,0,0, 1 };
    //Koeficijenti spekularne refleksije materijala
    GLfloat specular_coeffs[] = { 1,0,0, 1 };
    //Koeficijent glatkosti materijala
    GLfloat shininess = 20;    
    //Uključivanje parametara materijala
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glShadeModel(GL_SMOOTH);
    //kontruisanje točka, eta služi za rotaciju točka pri pomeranju voza. imamo 2 diska i cilindar
    glPushMatrix();
    GLUquadric* quad=gluNewQuadric();
    glRotatef(eta,1,0,0);
    gluCylinder(quad,0.04,0.04,0.01,32,32);
    gluDisk(quad,0.01,0.05,32,32);
    glTranslatef(0,0.01,0);
    gluDisk(quad,0.01,0.05,32,32);
    glPopMatrix();
}
//na ovo su zakačeni točkovi
void draw_sidehold(){
    //Koeficijenti ambijentalne refleksije materijala
    GLfloat ambient_coeffs[] = { 0.3, 0.3, 0.3 };
    //Koeficijenti difuzne refleksije materijala
    GLfloat diffuse_coeffs[] = { 0.4, 0.4, 0.4};
    //Koeficijenti spekularne refleksije materijala
    GLfloat specular_coeffs[] = { 0.775,0.775,0.775};
    //Koeficijent glatkosti materijala
    GLfloat shininess = 20;    
    //Uključivanje parametara materijala
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glShadeModel(GL_SMOOTH);
    //konstruisanje
    glPushMatrix();
    glScalef(0.05,0.075,0.4);
    glutSolidCube(1);
    glPopMatrix();
}
//šasija voza
void draw_chassey(){
    //Koeficijenti ambijentalne refleksije materijala
    GLfloat ambient_coeffs[] = { 0.8, 0, 0, 1 };
    //Koeficijenti difuzne refleksije materijala
    GLfloat diffuse_coeffs[] = { 0.8, 0, 0, 1 };
    //Koeficijenti spekularne refleksije materijala
    GLfloat specular_coeffs[] = { 0.7,0.7,0, 1 };
    //Koeficijent glatkosti materijala
    GLfloat shininess = 50;    
    //Uključivanje parametara materijala
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    //šasija
    glShadeModel(GL_SMOOTH);
    glPushMatrix();
    glScalef(0.2,0.05,0.3);
    glutSolidCube(1);
    glPopMatrix();
}
//držač za top
void draw_hold(){
    //Koeficijenti ambijentalne refleksije materijala
    GLfloat ambient_coeffs[] = { 0.3, 0.3, 0.3 };
    //Koeficijenti difuzne refleksije materijala
    GLfloat diffuse_coeffs[] = { 0.4, 0.4, 0.4};
    //Koeficijenti spekularne refleksije materijala
    GLfloat specular_coeffs[] = { 0.775,0.775,0.775};
    //Koeficijent glatkosti materijala
    GLfloat shininess = 20;    
    //Uključivanje parametara materijala
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glShadeModel(GL_SMOOTH);
    //konstruiši
    glPushMatrix();
    glScalef(0.2,0.075,0.2);
    glutSolidCube(1);
    glPopMatrix();
}
//zaštita za voz, u pitanju je gomila kockica oko topa, inspirisano Gustav Max-om
void draw_guard_part(){
    //Koeficijenti ambijentalne refleksije materijala
    GLfloat ambient_coeffs[] = { 0, 0.7, 0, 1 };
    //Koeficijenti difuzne refleksije materijala
    GLfloat diffuse_coeffs[] = { 0, 0.7, 0, 1 };
    //Koeficijenti spekularne refleksije materijala
    GLfloat specular_coeffs[] = { 0.7,0.7,0, 1 };
    //Koeficijent glatkosti materijala
    GLfloat shininess = 80;    //Uključivanje parametara materijala
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glShadeModel(GL_SMOOTH);
    //tinušne kockice
    glPushMatrix();
    glScalef(0.03,0.03,0.03);
    glutSolidCube(1);
    glPopMatrix();
}
//sferni hangar iz kog izlazi top
void draw_hangar(){
    //Koeficijenti ambijentalne refleksije materijala
    GLfloat ambient_coeffs[] = { 0.3, 0.3, 0.3 };
    //Koeficijenti difuzne refleksije materijala
    GLfloat diffuse_coeffs[] = { 0.4, 0.4, 0.4};
    //Koeficijenti spekularne refleksije materijala
    GLfloat specular_coeffs[] = { 0.775,0.775,0.775};
    //Koeficijent glatkosti materijala
    GLfloat shininess = 20;    
    //Uključivanje parametara materijala
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glShadeModel(GL_SMOOTH);
    //jednostavna sfera
    glPushMatrix();
    glutSolidSphere(0.03,32,32);
    glPopMatrix();
}
//topovsko đule, tinušna sfera
void draw_cannonball(){
    //Koeficijenti ambijentalne refleksije materijala
    GLfloat ambient_coeffs[] = { 0.4, 0.4, 0.4 };
    //Koeficijenti difuzne refleksije materijala
    GLfloat diffuse_coeffs[] = { 0.4, 0.4, 0.4};
    //Koeficijenti spekularne refleksije materijala
    GLfloat specular_coeffs[] = { 0.775,0.775,0.775};
    //Koeficijent glatkosti materijala
    GLfloat shininess = 20;    
    //Uključivanje parametara materijala
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    //još jedna mala sfera, veličine kao i njena cev
    glShadeModel(GL_SMOOTH);
    glPushMatrix();
    glutSolidSphere(0.03,64,64);
    glPopMatrix();
}
void draw_cannon(){
    //Koeficijenti ambijentalne refleksije materijala
    GLfloat ambient_coeffs[] = { 0.3, 0.3, 0.3 };
    //Koeficijenti difuzne refleksije materijala
    GLfloat diffuse_coeffs[] = { 0.4, 0.4, 0.4};
    //Koeficijenti spekularne refleksije materijala
    GLfloat specular_coeffs[] = { 0.775,0.775,0.775};
    //Koeficijent glatkosti materijala
    GLfloat shininess = 80;    
    //Uključivanje parametara materijala
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glShadeModel(GL_SMOOTH);
    //cev je cilindar
    glPushMatrix();
    GLUquadric* quad=gluNewQuadric();
    gluCylinder(quad,0.03,0.03,1,32,32);
    glPopMatrix();
}
//konstruisanje displeja
static void on_display(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //kontruisanje stringa u konzoli
    for(int i=0;i<300;i++) compoundstring[i]=0;
    strcat(compoundstring,"Dobrodosli!\n");
    strcat(compoundstring,"Poena imate:");
    sprintf(tmp,"%d\n",poeni);
    strcat(compoundstring,tmp);
    strcat(compoundstring,"Brzina vetra ");
    sprintf(tmp,"%.0f",brzinavetra*100);
    strcat(compoundstring,tmp);
    strcat(compoundstring,"km/h\nUgao vetra ");
    sprintf(tmp,"%d ",(ugaovetra+180)%360);
    strcat(compoundstring,tmp);
    strcat(compoundstring,"stepeni\n");
    if(sferas==0 && kockas==0) strcat(compoundstring,"Pobedili ste!\n");
    //Pozicija svetla (u pitanju je direkcionalno svetlo)
    //do dela gde se generiše ambijent je kopirano i adaptirano s onog časa, sem glulookat
    GLfloat light_position[] = { 100, 100, 100, 0 };
    //Ambijentalna boja svetla
    GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1 };
    //Difuzna boja svetla
    GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1 };
    //Spekularna boja svetla
    GLfloat light_specular[] = { 0.9, 0.9, 0.9, 1 };
    gluLookAt(r*sin(alfa),r*sin(beta),r*cos(alfa),0,0,0,0,1,0);
    //Briše se prethodni sadržaj prozora
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //Uključuje se osvjetljenje i podesavaju parametri svetla
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glEnable(GL_CULL_FACE);
    //Kreira se scena
    glPushMatrix();
    //ravan
    plot_plane();
    //ako stoje objekti, generiši ih
    if(mkocka==1) draw_cube();
    if(msfera==1) draw_sphere();
    glPushMatrix();
    //šine
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
    //daske
    for(int i=0;i<=40;i++){
	    draw_plank();
	    glTranslatef(0,0,0.5);
    }
    glPopMatrix();
    glPushMatrix();
    //točkovi, rotirano scenu da bismo imali vertikalne točkove
    glRotatef(90,0,0,1);
    glTranslatef(0.05,-2.805,z);
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
    //držači za točkove
    glTranslatef(2.805,0.075,0.125+z);
    draw_sidehold();
    glTranslatef(0.195,0,0);
    draw_sidehold();
    glPopMatrix();
    glPushMatrix();
    //šasija i nosač topa
    glTranslatef(2.9,0.1,0.1+z);
    draw_chassey();
    glTranslatef(0,0.05,0);
    draw_hold();
    glTranslatef(0,0.05,0);
    //zaštita topa
    for(int i=gama;i<360+gama;i+=30){
        glPushMatrix();
        glRotatef(i,0,1,0);
        glTranslatef(0.05,0,0);
        draw_guard_part();
        glPopMatrix();
    }
    for(int i=0;i<3;i++){
        glTranslatef(0,0.02,0);
        for(int i=gama-40;i<240+gama;i+=30){
            glPushMatrix();
            glRotatef(i,0,1,0);
            glTranslatef(0.05,0,0);
            draw_guard_part();
            glPopMatrix();
        }
    }
    glPushMatrix();
    //hangar i top
    glTranslatef(0,-0.06,0); 
    glRotatef(gama,0,1,0);
    glRotatef(-delta,1,0,0);
    draw_hangar();
    draw_cannon();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPushMatrix();
    //nezavisno konstruišemo đule pošto ono treba da leti, imamo dve konstrukcije u zavisnosti od stanja đuleta, da li leti ili ne
    //ne leti
    if(!animation_ongoing){
        loptax=2.9+sin(gama*2*pi/360)*cos(delta*2*pi/360);
        loptay=0.2+sin(delta*2*pi/360);
        loptaz=0.1+z+cos(gama*2*pi/360)*cos(delta*2*pi/360);
        glTranslatef(loptax,loptay,loptaz);
        draw_cannonball();
    }
    //leti
    else{
        glTranslatef(loptax,loptay,loptaz);
        draw_cannonball();
    }
    glPopMatrix();
    printf("%s\n",compoundstring);
    glPopMatrix();
    //Nova slika se šalje na ekran
    glutSwapBuffers();
}
