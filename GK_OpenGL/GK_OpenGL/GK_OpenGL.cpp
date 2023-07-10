#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <GL/glut.h>
#include <math.h>
#include <ctime> 
#include <algorithm> 
#include <vector> 
#include <fstream>
#include <math.h>

using namespace std;
static int year = 0, day = 0, meryear = 0, venyear = 0, maryear = 0, jupyear = 0, satyear = 0, urayear = 0, nepyear = 0;
int winWidth = 1000, winHeight = 800;
float posX = 4, posY = 6, posZ = 5, targetX = 0, targetY = 0, targetZ = 0, upX = 0, upY = 1, upZ = 0;
float rot = 1;
GLuint texture[9];
GLUquadricObj* sun = NULL;
GLUquadricObj* mercury = NULL;
GLUquadricObj* venus = NULL;
GLUquadricObj* earth = NULL;
GLUquadricObj* mars = NULL;
GLUquadricObj* jupiter = NULL;
GLUquadricObj* saturn = NULL;
GLUquadricObj* uranus = NULL;
GLUquadricObj* neptune = NULL;
GLUquadricObj* saturnRings = NULL;
GLfloat sunAngle = 0.0;
GLfloat distanc = 0.0;

GLfloat sunLightPosition[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat sunLightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat sunLightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat sunLightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };




struct Image
{
    unsigned long sizeX;
    unsigned long sizeY;
    GLubyte* data;
};

static unsigned int getint(FILE* fp)
{
    unsigned int c, c1, c2, c3;
    c = ((unsigned int)getc(fp));
    c1 = ((unsigned int)getc(fp)) << 8;
    c2 = ((unsigned int)getc(fp)) << 16;
    c3 = ((unsigned int)getc(fp)) << 24;
    return c | c1 | c2 | c3;
}

static unsigned short getshort(FILE* fp)
{
    unsigned short c, c1;
    c = ((unsigned short)getc(fp));
    c1 = ((unsigned short)getc(fp)) << 8;
    return c | c1;
}

bool ImageLoad(char* filename, Image* image)
{
    FILE* file;
    unsigned long size;
    size_t i, j, k, linediff;
    unsigned short int planes;
    unsigned short int bpp;
    char temp;

    if ((file = fopen(filename, "rb")) == NULL) {
        printf("File Not Found : %s\n", filename);
        return false;
    }

    fseek(file, 18, SEEK_CUR);

    image->sizeX = getint(file);
    printf("Width of %s: %lu\n", filename, image->sizeX);

    image->sizeY = getint(file);
    printf("Height of %s: %lu\n", filename, image->sizeY);

    size = 4.0 * ceil(image->sizeX * 24.0 / 32.0) * image->sizeY;


    planes = getshort(file);
    if (planes != 1) {
        printf("Planes from %s is not 1: %u\n", filename, planes);
        return false;
    }


    bpp = getshort(file);
    if (bpp != 24) {
        printf("Bpp from %s is not 24: %u\n", filename, bpp);
        return 0;
    }

    fseek(file, 24, SEEK_CUR);

    image->data = new GLubyte[size];
    if (image->data == NULL) {
        printf("Error allocating memory for color-corrected image data");
        return false;
    }

    i = fread(image->data, size, 1, file);
    if (i != 1) {
        printf("Error reading image data from %s.\n", filename);
        return false;
    }

    linediff = 4.0 * ceil(image->sizeX * 24.0 / 32.0) - image->sizeX * 3.0;
    k = 0;
    for (j = 0; j < image->sizeY; j++) {
        for (i = 0; i < image->sizeX; i++) {
            temp = image->data[k];
            image->data[k] = image->data[k + 2];
            image->data[k + 2] = temp;
            k += 3;
        }
        k += linediff;
    }
    return true;
}

void LoadGLTextures() {

    Image* image1;
    Image* image2;
    Image* image3;
    Image* image4;
    Image* image5;
    Image* image6;
    Image* image7;
    Image* image8;
    Image* image9;

    image1 = new Image();
    if (image1 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    char texname[] = "suntex.bmp";

    if (!ImageLoad(texname, image1)) {
        exit(1);
    }

    image2 = new Image();
    if (image2 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    char texname1[] = "mercurytex.bmp";

    if (!ImageLoad(texname1, image2)) {
        exit(1);
    }

    image3 = new Image();
    if (image3 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    char texname2[] = "venustex.bmp";

    if (!ImageLoad(texname2, image3)) {
        exit(1);
    }

    image4 = new Image();
    if (image4 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    char texname3[] = "earthtex.bmp";

    if (!ImageLoad(texname3, image4)) {
        exit(1);
    }

    image5 = new Image();
    if (image5 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    char texname4[] = "marstex.bmp"; 

    if (!ImageLoad(texname4, image5)) {
        exit(1);
    }

    image6 = new Image();
    if (image6 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    char texname5[] = "jupitertex.bmp"; 

    if (!ImageLoad(texname5, image6)) {
        exit(1);
    }

    image7 = new Image();
    if (image7 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    char texname6[] = "saturntex.bmp"; 

    if (!ImageLoad(texname6, image7)) {
        exit(1);
    }

    image8 = new Image();
    if (image8 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    char texname7[] = "uranustex.bmp";

    if (!ImageLoad(texname7, image8)) {
        exit(1);
    }

    image9 = new Image();
    if (image9 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    char texname8[] = "neptunetex.bmp"; 

    if (!ImageLoad(texname8, image9)) {
        exit(1);
    }

    glGenTextures(1, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    glTexImage2D(GL_TEXTURE_2D,
        0,
        3,
        image1->sizeX,
        image1->sizeY,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        image1->data
    );

    glGenTextures(1, &texture[1]);
    glBindTexture(GL_TEXTURE_2D, texture[1]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    glTexImage2D(GL_TEXTURE_2D,
        0,
        3,
        image2->sizeX,
        image2->sizeY,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        image2->data
    );

    glGenTextures(1, &texture[2]);
    glBindTexture(GL_TEXTURE_2D, texture[2]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    glTexImage2D(GL_TEXTURE_2D,
        0,
        3,
        image3->sizeX,
        image3->sizeY,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        image3->data
    );

    glGenTextures(1, &texture[3]); 
    glBindTexture(GL_TEXTURE_2D, texture[3]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    glTexImage2D(GL_TEXTURE_2D,
        0,
        3,
        image4->sizeX,
        image4->sizeY,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        image4->data
    );

    glGenTextures(1, &texture[4]);
    glBindTexture(GL_TEXTURE_2D, texture[4]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    glTexImage2D(GL_TEXTURE_2D,
        0,
        3,
        image5->sizeX,
        image5->sizeY,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        image5->data
    );

    glGenTextures(1, &texture[5]); 
    glBindTexture(GL_TEXTURE_2D, texture[5]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    glTexImage2D(GL_TEXTURE_2D,
        0,
        3,
        image6->sizeX,
        image6->sizeY,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        image6->data
    );

    glGenTextures(1, &texture[6]);
    glBindTexture(GL_TEXTURE_2D, texture[6]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    glTexImage2D(GL_TEXTURE_2D,
        0,
        3,
        image7->sizeX,
        image7->sizeY,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        image7->data
    );

    glGenTextures(1, &texture[7]); 
    glBindTexture(GL_TEXTURE_2D, texture[7]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    glTexImage2D(GL_TEXTURE_2D,
        0,
        3,
        image8->sizeX,
        image8->sizeY,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        image8->data
    );

    glGenTextures(1, &texture[8]);
    glBindTexture(GL_TEXTURE_2D, texture[8]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    glTexImage2D(GL_TEXTURE_2D,
        0,
        3,
        image9->sizeX,
        image9->sizeY,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        image9->data
    );
};

void init() {
    LoadGLTextures();
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    sun = gluNewQuadric();
    gluQuadricDrawStyle(sun, GLU_FILL);
    gluQuadricTexture(sun, GL_TRUE);
    gluQuadricNormals(sun, GLU_SMOOTH);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_POSITION, sunLightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, sunLightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sunLightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, sunLightSpecular);
    glEnable(GL_LIGHT0);



    mercury = gluNewQuadric();
    gluQuadricDrawStyle(mercury, GLU_FILL);
    gluQuadricTexture(mercury, GL_TRUE);
    gluQuadricNormals(mercury, GLU_SMOOTH);

    venus = gluNewQuadric();
    gluQuadricDrawStyle(venus, GLU_FILL);
    gluQuadricTexture(venus, GL_TRUE);
    gluQuadricNormals(venus, GLU_SMOOTH);

    earth = gluNewQuadric();
    gluQuadricDrawStyle(earth, GLU_FILL);
    gluQuadricTexture(earth, GL_TRUE);
    gluQuadricNormals(earth, GLU_SMOOTH);

    mars = gluNewQuadric(); 
    gluQuadricDrawStyle(mars, GLU_FILL);
    gluQuadricTexture(mars, GL_TRUE);
    gluQuadricNormals(mars, GLU_SMOOTH);

    jupiter = gluNewQuadric(); 
    gluQuadricDrawStyle(jupiter, GLU_FILL);
    gluQuadricTexture(jupiter, GL_TRUE);
    gluQuadricNormals(jupiter, GLU_SMOOTH);

    saturn = gluNewQuadric(); 
    gluQuadricDrawStyle(saturn, GLU_FILL);
    gluQuadricTexture(saturn, GL_TRUE);
    gluQuadricNormals(saturn, GLU_SMOOTH);

    uranus = gluNewQuadric(); 
    gluQuadricDrawStyle(uranus, GLU_FILL);
    gluQuadricTexture(uranus, GL_TRUE);
    gluQuadricNormals(uranus, GLU_SMOOTH);

    neptune = gluNewQuadric(); 
    gluQuadricDrawStyle(neptune, GLU_FILL);
    gluQuadricTexture(neptune, GL_TRUE);
    gluQuadricNormals(neptune, GLU_SMOOTH);

    saturnRings = gluNewQuadric();
    gluQuadricDrawStyle(saturnRings, GLU_FILL);
    gluQuadricTexture(saturnRings, GL_TRUE);
    gluQuadricNormals(saturnRings, GLU_SMOOTH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 4.0 / 3.0, 1, 40);

    glMatrixMode(GL_MODELVIEW);
}

void drawAxis(double length) {

    glPushMatrix();
    glBegin(GL_LINES);
    glVertex3d(0, 0, -length);
    glVertex3d(0, 0, length);
    glEnd();
    glTranslated(0, 0, length - 0.2);
    glutWireCone(1, 1.75, 12, 9);
    glPopMatrix();
}
void displayFunc() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(posX, posY, posZ, targetX, targetY, targetZ, upX, upY, upZ);
    glDisable(GL_LIGHTING);
    //sun
    glPushMatrix();
    glColor3f(1, 1, 1);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    gluSphere(sun, 1.4, 100, 100);
    glLightfv(GL_LIGHT0, GL_POSITION, sunLightPosition);
    glPopMatrix();
    glEnable(GL_LIGHTING);
    //mercury
    glPushMatrix();
    glRotatef((GLfloat)meryear, 0.0, 1.0, 0.0);
    glTranslatef(2.0, 0.0, 0.0);
    glRotatef((GLfloat)day, 0.0, 1.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    gluSphere(mercury, 0.2, 10, 8);
    glPopMatrix();
    //venus
    glPushMatrix();
    glRotatef((GLfloat)venyear, 0.0, 1.0, 0.0);
    glTranslatef(3.5, 0.0, 0.0); 
    glRotatef((GLfloat)day, 0.0, 1.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, texture[2]); 
    gluSphere(venus, 0.4, 15, 12); 
    glPopMatrix();
    //earth
    glPushMatrix();
    glRotatef((GLfloat)year, 0.0, 1.0, 0.0);
    glTranslatef(5.0, 0.0, 0.0); 
    glRotatef((GLfloat)day, 0.0, 1.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, texture[3]); 
    gluSphere(earth, 0.5, 15, 12);
    glPopMatrix();
    //mars 
    glPushMatrix();
    glRotatef((GLfloat)maryear, 0.0, 1.0, 0.0);
    glTranslatef(6.5, 0.0, 0.0); 
    glRotatef((GLfloat)day, 0.0, 1.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, texture[4]); 
    gluSphere(mars, 0.4, 15, 12); 
    glPopMatrix();
    //jupiter
    glPushMatrix();
    glRotatef((GLfloat)jupyear, 0.0, 1.0, 0.0);
    glTranslatef(8.0, 0.0, 0.0); 
    glRotatef((GLfloat)day, 0.0, 1.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, texture[5]); 
    gluSphere(jupiter, 0.8, 15, 12);
    glPopMatrix();

    //saturn
    glPushMatrix();
    glRotatef((GLfloat)satyear, 0.0, 1.0, 0.0);
    glTranslatef(9.5, 0.0, 0.0); 
    glRotatef((GLfloat)day, 0.0, 1.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, texture[6]); 
    gluSphere(saturn, 0.7, 15, 12); 
    glPopMatrix();

    glPushMatrix();
    glRotatef((GLfloat)satyear, 0.0, 1.0, 0.0); 
    glTranslatef(9.5, 0.0, 0.0);
    glRotatef(90.0f, 1.0, 0.0, 0.0);
    glColor4f(1.0f, 0.96f, 0.89f, 1.0f); 
    gluDisk(saturnRings, 0.8f, 1.4f, 32, 32); 
    glPopMatrix();

    //uranus
    glPushMatrix();
    glRotatef((GLfloat)urayear, 0.0, 1.0, 0.0);
    glTranslatef(11.0, 0.0, 0.0);
    glRotatef((GLfloat)day, 0.0, 1.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, texture[7]);
    gluSphere(uranus, 0.6, 15, 12);
    glPopMatrix();
    //neptune
    glPushMatrix();
    glRotatef((GLfloat)nepyear, 0.0, 1.0, 0.0);
    glTranslatef(13.0, 0.0, 0.0);
    glRotatef((GLfloat)day, 0.0, 1.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    gluSphere(neptune, 0.55, 15, 12);
    glPopMatrix();

  

    glutSwapBuffers();
}
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);

    if (distanc * cos(sunAngle) < 0.0) {
        glColor4f(0.3, 0.3, 0.3, 1.0);
    }
    else {
        glColor4f(0.8, 0.8, 0.8, 1.0);
    }

    glPushMatrix();
    glutWireSphere(1.0, 20, 16);
    glRotatef((GLfloat)year, 0.0, 1.0, 0.0);
    glTranslatef(2.0, 0.0, 0.0);
    glRotatef((GLfloat)day, 0.0, 1.0, 0.0);
    glutWireSphere(0.2, 10, 8);
    glPopMatrix();
    glutSwapBuffers();
}

void update(int value) {

    year = (year + 1);
    day += 1.0f;
    meryear = year + year * 1.6;
    venyear = year + year * 1.12;
    maryear = year + year * 0.8;
    jupyear = year - year * 0.44;
    satyear = year - year * 0.33;
    urayear = year - year * 0.23;
    nepyear = year - year * 0.18;

    glutPostRedisplay();
    glutTimerFunc(25, update, 0);
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Solar system");

    init();
    glutReshapeFunc(reshape);
    glutDisplayFunc(displayFunc);
    glutTimerFunc(25, update, 0);

    glutMainLoop();
}