/*
ASSASIN'S CREED
SIM MAN NI B032210503
LOW XIAO QIAN B032210182
LIM JIA CHEAN B02210216
*/

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <gl/glut.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int cw;
int ch;
int cTurn = 180; //current facing

bool isStand = true;
float curDistanceX = 0, curDistanceZ = 0;//current dist
float legDis = 0;

void timerCallback(int value); // Declaration of the timer callback function
GLfloat headRotationAngle = 90.0;
GLfloat armRotationAngle = 0.0;
GLfloat forearmRotationAngle = 0.0;

enum CameraView {
    FIRST_PERSON,
    THIRD_PERSON,
    TOP_VIEW
};
CameraView currentView = THIRD_PERSON; // Initial camera view

float cameraAngleX = 0.0f; // Angle rotate camera around x-axis
float cameraAngleY = 0.0f; // Angle rotate camera around y-axis
float cameraPosX = 0.0f; // Camera position X
float cameraPosY = 1.6f; // Camera position Y 
float cameraPosZ = 0.0f; // Camera position Z
float cameraSpeed = 0.1f; // Speed of camera movement

float characterPosX = 0.0f; // Character position X
float characterPosZ = 0.0f; // Character position Z
float characterPosY = 0.0f; // Character position Z

int lastMouseX = 0;    // Last known X position of the mouse
int lastMouseY = 0;    // Last known Y position of the mouse
bool isDragging = false;  //check if mouse is being dragged
int currentCharacter = 1;
bool jump = false;  // character jump or not
float jumpVelocity = 0.0f; // initial jump velocity
float gravity = 0.0005f;
float jumpSpeed = 0.05f;  //jump speed
bool isMovingUp = false;  //check if the character is moving up
bool isMovingDown = false; // check if the character is moving down
float originalY = 0.0f;
bool walk = false;
float walkingLegAngle = 0.0f;
float walkingLegSpeed = 0.1f;

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case '1':
        currentView = FIRST_PERSON;
        break;
    case '2':
        currentView = THIRD_PERSON;
        break;
    case '3':
        currentView = TOP_VIEW;
        break;

    case 'p':
        currentCharacter = 1;
        break;
    case 'o':
        currentCharacter = 2;
        break;
    case'i':
        currentCharacter = 3;
        break;
    case 'w': //run
        characterPosX += sin(cameraAngleY * M_PI / 180) * cameraSpeed * 10;
        characterPosZ += cos(cameraAngleY * M_PI / 180) * cameraSpeed * 10;
        walk = true;
        break;
    case 's': //run
        characterPosX -= sin(cameraAngleY * M_PI / 180) * cameraSpeed * 10;
        characterPosZ -= cos(cameraAngleY * M_PI / 180) * cameraSpeed * 10;
        walk = true;
        break;
    case 'a': //run
        characterPosX += cos(cameraAngleY * M_PI / 180) * cameraSpeed * 10;
        characterPosZ -= sin(cameraAngleY * M_PI / 180) * cameraSpeed * 10;
        walk = true;
        break;
    case 'd': //run
        characterPosX -= cos(cameraAngleY * M_PI / 180) * cameraSpeed * 10;
        characterPosZ += sin(cameraAngleY * M_PI / 180) * cameraSpeed * 10;
        walk = true;
        break;
    case 'j': // jumping
        if (!jump) {
            jump = true;
            jumpVelocity = jumpSpeed;
            glutTimerFunc(5000, timerCallback, 0);
        }
        break;
    default:
        walk = false;
        break;
    }

    glutPostRedisplay();
}

void keyboardUp(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':
    case 'a':
    case 's':
    case 'd':
        walk = false;
        break;
    default:
        break;
    }
}

void specialKeys(int keys, int x, int y) {
    float speed = 0.1;
    switch (keys) { //walk
    case GLUT_KEY_UP:
        characterPosX += sin(cameraAngleY * M_PI / 180) * cameraSpeed;
        characterPosZ += cos(cameraAngleY * M_PI / 180) * cameraSpeed;
        walk = true;
        break;

    case GLUT_KEY_DOWN:
        characterPosX -= sin(cameraAngleY * M_PI / 180) * cameraSpeed;
        characterPosZ -= cos(cameraAngleY * M_PI / 180) * cameraSpeed;
        walk = true;
        break;

    case GLUT_KEY_LEFT:
        characterPosX += cos(cameraAngleY * M_PI / 180) * cameraSpeed;
        characterPosZ -= sin(cameraAngleY * M_PI / 180) * cameraSpeed;
        walk = true;
        break;
    case GLUT_KEY_RIGHT:
        characterPosX -= cos(cameraAngleY * M_PI / 180) * cameraSpeed;
        characterPosZ += sin(cameraAngleY * M_PI / 180) * cameraSpeed;
        walk = true;
        break;
    default:
        walk = false;
        break;
    }

    glutPostRedisplay();
}
void specialKeysUp(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        walk = false;
        break;
    case GLUT_KEY_DOWN:
        walk = false;
        break;
    case GLUT_KEY_LEFT:
        walk = false;
        break;
    case GLUT_KEY_RIGHT:
        walk = false;
        break;
    default:
        break;
    }
}

GLfloat vertices[8][3] = {
  {-0.5, -0.5, 0.5},
  {0.5, -0.5, 0.5},
  {0.5, 0.5, 0.5},
  {-0.5, 0.5, 0.5},
  {-0.5, -0.5, -0.5},
  {0.5, -0.5, -0.5},
  {0.5, 0.5, -0.5},
  {-0.5, 0.5, -0.5}
};
GLfloat vertice1[8][3] = {
  {-0.6, -0.3, 0.6},
  {0.6, -0.3, 0.6},
  {0.6, 0.3, 0.6},
  {-0.6, 0.3, 0.6},
  {-0.6, -0.3, -0.6},
  {0.6, -0.3, -0.6},
  {0.6, 0.3, -0.6},
  {-0.6, 0.3, -0.6}
};

void enable() {
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
}

void cylinder(int segments, float radius, float height) {
    int k;
    float angles;
    float x, z;

    glPushMatrix();
    glTranslatef(0.0, height / 2, 0.0);
    glBegin(GL_POLYGON);
    for (k = 0; k < segments; k++) {
        angles = 2.0 * 3.141592654f * k / segments;
        x = radius * cos(angles);
        z = radius * sin(angles);
        glVertex3f(x, 0, z);
    }
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, -height / 2, 0.0);
    glBegin(GL_POLYGON);
    for (k = 0; k < segments; k++) {
        angles = 2.0 * 3.141592654f * k / segments;
        x = radius * cos(angles);
        z = radius * sin(angles);
        glVertex3f(x, 0, z);
    }
    glEnd();
    glPopMatrix();


    glBegin(GL_QUAD_STRIP);
    for (k = 0; k <= segments; k++) {
        angles = 2.0 * 3.141592654f * k / segments;
        x = radius * cos(angles);
        z = radius * sin(angles);
        glVertex3f(x, -height / 2, z);
        glVertex3f(x, height / 2, z);
    }
    glEnd();
}

void polygon(int a, int b, int c, int d)
{
    glBegin(GL_POLYGON);
    glVertex3fv(vertices[a]);
    glVertex3fv(vertices[b]);
    glVertex3fv(vertices[c]);
    glVertex3fv(vertices[d]);
    glEnd();
}

void polygon1(int a, int b, int c, int d)
{
    glBegin(GL_POLYGON);
    glVertex3fv(vertice1[a]);
    glVertex3fv(vertice1[b]);
    glVertex3fv(vertice1[c]);
    glVertex3fv(vertice1[d]);
    glEnd();
}

void cube()
{
    polygon(0, 1, 2, 3);
    polygon(4, 5, 6, 7);
    polygon(0, 4, 7, 3);
    polygon(1, 5, 6, 2);
    polygon(3, 2, 6, 7);
    polygon(0, 1, 5, 4);
}

void cube1()
{
    polygon1(0, 1, 2, 3);
    polygon1(4, 5, 6, 7);
    polygon1(0, 4, 7, 3);
    polygon1(1, 5, 6, 2);
    polygon1(3, 2, 6, 7);
    polygon1(0, 1, 5, 4);
}

void square()
{
    glBegin(GL_POLYGON);
    glVertex3f(-0.3, 0.0, 0.5); // bottom left
    glVertex3f(-0.1, 0, 0.5); // bottom right
    glVertex3f(-0.1, 0.2, 0.5); // top right
    glVertex3f(-0.3, 0.2, 0.5); // top left
    glEnd();
}

void mouth()
{
    glBegin(GL_POLYGON);
    glVertex3f(-0.35, -0.1, 0.5); // bottom left
    glVertex3f(-0.2, -0.25, 0.5); // bottom right
    glVertex3f(0.1, -0.25, 0.5); // top right
    glVertex3f(0.25, -0.1, 0.5); // top left
    glVertex3f(0.2, -0.1, 0.5); // bottom left
    glVertex3f(0.1, -0.2, 0.5); // bottom right
    glVertex3f(-0.2, -0.2, 0.5); // bottom left
    glVertex3f(-0.3, -0.1, 0.5); // bottom right

    glEnd();
}

void ManniCharacter() {

    glPushMatrix();
    glScalef(0.5, 0.5, 0.5);

    glPushMatrix();

    glRotatef((GLfloat)cTurn, 0.0, 1.0, 0.0); //turn

    glTranslatef(0.375, 0.0, 0.0);

    //rl
    glTranslatef(0.0, 0.7, 0.0);
    glColor3f(0.5, 0.8, 0.3);
    //rl
    glPushMatrix();
    glScalef(0.5, 0.8, 0.5);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();

    //ll
    glPushMatrix();
    glRotatef((GLfloat)cTurn, 0.0, 1.0, 0.0); //turn

    //ll
    glTranslatef(-0.375, 0.0, 0.0);
    glTranslatef(0.0, 0.7, 0.0);
    glColor3f(0.5, 0.8, 0.3);
    glPushMatrix();
    glScalef(0.5, 0.8, 0.5);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();


    glPushMatrix();
    glTranslatef(curDistanceX, 0.0, curDistanceZ);
    glRotatef((GLfloat)cTurn, 0.0, 1.0, 0.0);
    //body
    glTranslatef(0.0, 2.0, 0.0);
    glColor3f(0.0, 0.0, 0.5);
    glPushMatrix();
    glScalef(1.4, 1.75, 1.0);
    glutSolidCube(1.0);
    glPopMatrix();
    //head
    glTranslatef(0.0, 1.55, 0.0);
    glColor3f(1.0, 0.75, 0.0);
    glPushMatrix();
    glScalef(0.8, 0.8, 0.8);
    glutSolidSphere(0.75, 20, 20);
    glPopMatrix();

    //eyes
    glTranslatef(0.125, 0.1, -0.6);
    glColor3f(0.2, 0.2, 0.2);
    glPushMatrix();
    glScalef(0.1, 0.1, 0.01);
    glutSolidCube(1.0);
    glPopMatrix();

    glTranslatef(-0.25, 0.0, 0.0);
    glColor3f(0.2, 0.2, 0.2);
    glPushMatrix();
    glScalef(0.1, 0.1, 0.01);
    glutSolidCube(1.0);
    glPopMatrix();

    //mouth
    glTranslatef(0.125, -0.25, 0.02);
    glColor3f(0.8, 0.2, 0.2);
    glPushMatrix();
    glScalef(0.3, 0.05, 0.01);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();

    //ra
    glPushMatrix();
    glTranslatef(curDistanceX, 0.0, curDistanceZ);
    glRotatef((GLfloat)cTurn, 0.0, 1.0, 0.0);
    //ra
    glTranslatef(0.85, 1.80, 0.0);

    glTranslatef(0.0, 0.5, 0.0);
    glColor3f(1.0, 0.75, 0.0);
    glPushMatrix();
    glScalef(0.35, 1.0, 0.45);
    glutSolidCube(1.0);
    glPopMatrix();
    glPopMatrix();

    //la
    glPushMatrix();
    glTranslatef(curDistanceX, 0.0, curDistanceZ);
    glRotatef((GLfloat)cTurn, 0.0, 1.0, 0.0);
    //la
    glTranslatef(-0.85, 1.8, 0.0);

    glTranslatef(0.0, 0.5, 0.0);
    glColor3f(1.0, 0.75, 0.0);
    glPushMatrix();
    glScalef(0.35, 1.0, 0.45);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();

    //legs
    glPushMatrix();
    glTranslatef(curDistanceX, 0.0, curDistanceZ);
    glRotatef((GLfloat)cTurn, 0.0, 1.0, 0.0);
    //rl
    glTranslatef(0.375, 0.0, 0.0);
    glTranslatef(0.0, 0.5, 0.0);
    glColor3f(0.5, 0.8, 0.3);
    //rl
    glPushMatrix();
    glScalef(0.5, 0.8, 0.5);
    if (walk) {
        // Adjust leg angle for walking animation
        glRotatef(walkingLegAngle / 3, 1.0, 0.0, 0.0);
    }
    glutSolidCube(1.0);
    glPopMatrix();
    //ll
    glTranslatef(-0.75, 0.0, 0.0);
    if (walk) {
        // Adjust leg angle for walking animation
        glRotatef(-walkingLegAngle / 3, 1.0, 0.0, 0.0);
    }
    glTranslatef(0.0, -0.05, 0.0);
    glColor3f(0.5, 0.8, 0.3);
    glPushMatrix();
    glScalef(0.5, 0.8, 0.5);

    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();

    glPopMatrix();
}

void drawManniCharacter() {
    ManniCharacter();
}

void drawLowCharacter() {

    glPushMatrix();
    glColor3f(1.0, 1.0, 0.0); // Yellow
    glTranslatef(0.0, 1.0, 0.10);
    glScalef(0.1, 0.1, 0.1);
    cylinder(24, 1.0, 2.0);
    glPopMatrix();

    glPushMatrix(); //body
    glTranslatef(0, 0.6, 0.1);
    glScalef(0.45, 0.45, 0.45);
    glColor3f(0.0, 0.0, 1.0); // blue
    cube();
    glPopMatrix();

    glPushMatrix();//right hand 1
    glTranslatef(0.3, 0.6, 0.1);
    glScalef(0.17, 0.17, 0.17);
    glColor3f(1.0, 1.0, 0.0); // yellow
    cube();
    glPopMatrix();

    glPushMatrix();//right hand 2
    glTranslatef(0.3, 0.45, 0.125);
    glScalef(0.17, 0.17, 0.17);
    glColor3f(1.0, 1.0, 0.0); // yellow
    glRotatef(22.5, -1.0, 0.0, 0.0);
    cube();
    glPopMatrix();

    glPushMatrix();//left hand 1
    glTranslatef(-0.3, 0.6, 0.1);
    glScalef(0.17, 0.17, 0.17);
    glColor3f(1.0, 1.0, 0.0); // yellow
    cube();
    glPopMatrix();

    glPushMatrix();//left hand 2
    glTranslatef(-0.3, 0.45, 0.125);
    glScalef(0.17, 0.17, 0.17);
    glColor3f(1.0, 1.0, 0.0); // yellow
    glRotatef(22.5, -1.0, 0.0, 0.0);
    cube();
    glPopMatrix();

    glPushMatrix();//right leg 1
    glTranslatef(0.14, 0.3, 0.04);
    glScalef(0.17, 0.17, 0.17);
    glColor3f(0.0f, 0.5f, 0.5f); // green

    cube();
    glPopMatrix();

    glPushMatrix();//right leg 3
    glTranslatef(0.14, 0.15, 0.04);
    glScalef(0.17, 0.17, 0.17);
    glColor3f(0.0f, 0.5f, 0.5f); // green
    cube();
    glPopMatrix();

    glPushMatrix();//right leg 2
    glTranslatef(0.14, 0.00, 0.05);
    glScalef(0.19, 0.19, 0.19);
    glColor3f(0.5f, 1.0f, 1.0f); // green
    if (walk) {
        // Adjust leg angle for walking animation
        glRotatef(walkingLegAngle, 1.0, 0.0, 0.0);
    }
    cube1();
    glPopMatrix();

    glPushMatrix();//left leg 1
    glTranslatef(-0.14, 0.3, 0.04);
    glScalef(0.17, 0.17, 0.17);
    glColor3f(0.0f, 0.5f, 0.5f); // green
    cube();
    glPopMatrix();

    glPushMatrix();//left leg 3
    glTranslatef(-0.14, 0.15, 0.04);
    glScalef(0.17, 0.17, 0.17);
    glColor3f(0.0f, 0.5f, 0.5f); // green
    cube();
    glPopMatrix();

    glPushMatrix();//left leg 2
    glTranslatef(-0.14, 0.0, 0.05);
    glScalef(0.19, 0.19, 0.19);
    glColor3f(0.5f, 1.0f, 1.0f); // green
    if (walk) {
        // Adjust leg angle for walking animation
        glRotatef(-walkingLegAngle, 1.0, 0.0, 0.0);
    }
    cube1();
    glPopMatrix();

    glPushMatrix();//eye 1
    glColor3f(0, 0, 0.0); // black
    glTranslatef(0.0, 1, 0.12);
    glScalef(0.17, 0.17, 0.17);
    square();
    glPopMatrix();

    glPushMatrix();//eye 2
    glColor3f(0, 0, 0.0); // black
    glTranslatef(0.05, 1, 0.12);
    glScalef(0.17, 0.17, 0.17);
    square();
    glPopMatrix();

    glPushMatrix();//mouth
    glColor3f(0, 0, 0.0); // black
    glTranslatef(0.00, 1, 0.12);
    glScalef(0.17, 0.17, 0.17);
    mouth();
    glPopMatrix();

}

void body3(void)
{
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.2);
    glScalef(2.5, 3, 1.5);
    glColor3f(0.0, 0.75, 0.75);
    glutSolidCube(0.3);
    glPopMatrix();
}

void head3() {
    //neck
    glPushMatrix();
    glTranslatef(0.0, 0.525, 0.24);
    glScalef(0.5, 0.5, 0.5);
    glColor3f(1, 0.68, 0.68);
    glutSolidCube(0.3);
    glPopMatrix();

    //head
    glPushMatrix();
    glTranslatef(0.0, 0.75, 0.24);
    glScalef(1.5, 1.0, 1.35);
    glColor3f(1, 0.68, 0.68);
    glutSolidCube(0.3);
    glPopMatrix();

    //right eye
    glPushMatrix();
    glTranslatef(0.15, (3 + 1 + 2 * 2 / 3) * 0.3 * 0.5, 0.45);
    glScalef(0.15, 0.15, 0.05);
    glColor3f(0.7, 0.7, 0.7);
    glutSolidCube(0.3);
    glPopMatrix();

    //left eye
    glPushMatrix();
    glTranslatef(-0.15, (3 + 1 + 2 * 2 / 3) * 0.3 * 0.5, 0.45);
    glScalef(0.15, 0.15, 0.05);
    glColor3f(0.7, 0.7, 0.7);
    glutSolidCube(0.3);
    glPopMatrix();

    //mouth n nose
    glPushMatrix();
    glTranslatef(0, 0.735, 0.45);
    glScalef(0.15, 0.15, 0);
    glColor3f(0, 0, 0);
    glutSolidCube(0.3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0.66, 0.45);
    glScalef(0.85, 0.15, 0);
    glColor3f(0, 0, 0);
    glutSolidCube(0.3);
    glPopMatrix();
}

void leftArm3()
{
    //left
    glPushMatrix();
    glTranslatef(-0.45, 0.2625, 0.225);
    glScalef(0.5, 1.25, 0.5);
    glColor3f(0.0, 0.85, 0.85);
    glutSolidCube(0.3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.45, -0.15, 0.225);
    glScalef(0.5, 1.25, 0.5);
    glColor3f(1, 0.7, 0.7);
    glutSolidCube(0.3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.45, 0.0375, 0.225);
    glScalef(0.5, 0.25, 0.5);
    glColor3f(1, 0.5, 0.5);
    glutSolidCube(0.3);
    glPopMatrix();
}

void rightArm3()
{
    glPushMatrix();
    glTranslatef(0.45, 0.2625, 0.225);
    glScalef(0.5, 1.25, 0.5);
    glColor3f(0.0, 0.85, 0.85);
    glutSolidCube(0.3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.45, -0.15, 0.225);
    glScalef(0.5, 1.25, 0.5);
    glColor3f(1, 0.7, 0.7);
    glutSolidCube(0.3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.45, 0.0375, 0.225);
    glScalef(0.5, 0.25, 0.5);
    glColor3f(1, 0.5, 0.5);
    glutSolidCube(0.3);
    glPopMatrix();
}

void leftLeg3()
{
    //left
    glPushMatrix();
    glTranslatef(-0.2625, -0.675, 0.225);
    glScalef(0.75, 1.5, 0.5);
    glColor3f(0, 0, 0.7);
    glutSolidCube(0.3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.2625, -1.05, 0.225);
    glScalef(0.75, 1.5, 0.5);
    glColor3f(1, 0.7, 0.7);
    glutSolidCube(0.3);

    glScalef(1.25, 0.25, 1.0);
    glTranslatef(0, -0.75, 0);
    glColor3f(0.3, 0, 0);
    if (walk) {
        // Adjust leg angle for walking animation
        glRotatef(-walkingLegAngle, 1.0, 0.0, 0.0);
    }
    glutSolidCube(0.3);
    glPopMatrix();
}

void rightLeg3()
{
    glPushMatrix();
    glTranslatef(0.2625, -0.675, 0.225);
    glScalef(0.75, 1.5, 0.5);
    glColor3f(0, 0, 0.7);
    glutSolidCube(0.3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.2625, -1.05, 0.225);
    glScalef(0.75, 1.5, 0.5);
    glColor3f(1, 0.7, 0.7);
    glutSolidCube(0.3);

    glScalef(1.25, 0.25, 1.0);
    glTranslatef(0, -0.75, 0);
    glColor3f(0.3, 0, 0);
    if (walk) {
        // Adjust leg angle for walking animation
        glRotatef(walkingLegAngle, 1.0, 0.0, 0.0);
    }
    glutSolidCube(0.3);
    glPopMatrix();
}

void drawLimCharacter()
{
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f);
    glScalef(0.5f, 0.5f, 0.5f);
    body3();
    head3();
    leftArm3();
    rightArm3();
    leftLeg3();
    rightLeg3();
    glPopMatrix();
}

void drawFloor() {
    glColor3f(1.0f, 0.8f, 1.0f); //pink floor
    glBegin(GL_QUADS);
    glVertex3f(-5.0f, 0.0f, -5.0f);
    glVertex3f(5.0f, 0.0f, -5.0f);
    glVertex3f(5.0f, 0.0f, 5.0f);
    glVertex3f(-5.0f, 0.0f, 5.0f);
    glEnd();
}

void timerCallback(int value);

void updateJump() {
    if (jump) {
        // Update vertical position
        characterPosY += jumpVelocity;

        if (jumpVelocity > 0.0f && characterPosY >= 2.0f) {
            jumpVelocity -= gravity * 2.0f; // Decrease the velocity faster
        }

        if (jumpVelocity < 0.0f && characterPosY <= 0.0f) {
            characterPosY = 0.0f; // Reset position
            jump = false; // Stop jumping
        }

        // Apply gravity
        jumpVelocity -= gravity;
    }

    if (isMovingUp) {
        characterPosY += cameraSpeed;
        if (characterPosY >= 2.0f) {
            isMovingUp = false;
            isMovingDown = true;
        }
    }
    else if (isMovingDown) {
        characterPosY -= cameraSpeed;
        if (characterPosY <= originalY) {
            characterPosY = originalY; // Reset position to original
            isMovingDown = false;
        }
    }
    if (walk) {
        // Animate leg angle for walking
        walkingLegAngle += walkingLegSpeed;
        if (walkingLegAngle > 45.0f || walkingLegAngle < -45.0f) {
            walkingLegSpeed *= -1.0f;
        }
    }
    else {
        // Reset leg angle when not walking
        walkingLegAngle = 0.0f;
    }
    glutPostRedisplay();
}

void timerCallback(int value) {

    if (jump && jumpVelocity > 0.0f && characterPosY >= 2.0f) {
        jumpVelocity = 0.0f;
        glutTimerFunc(1000, timerCallback, 0); // 1sec delay 
    }
    else {
        jump = false;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glLoadIdentity();
    switch (currentView) {
    case FIRST_PERSON:
    {
        float cameraOffset = 0.5f;
        float cameraX = characterPosX + sin((cameraAngleY + 180) * M_PI / 180) * cameraOffset;
        float cameraY = characterPosY + 1.6f;
        float cameraZ = characterPosZ + cos((cameraAngleY + 180) * M_PI / 180) * cameraOffset;
        float lookAtX = characterPosX + sin(cameraAngleY * M_PI / 180);
        float lookAtY = characterPosY + 1.6f;
        float lookAtZ = characterPosZ + cos(cameraAngleY * M_PI / 180);

        gluLookAt(cameraX, cameraY, cameraZ, lookAtX, lookAtY, lookAtZ, 0.0, 1.0, 0.0);
        break;
    }

    case THIRD_PERSON:
    {
        float cameraX = characterPosX - sin(cameraAngleY * M_PI / 180) * 5.0f;
        float cameraY = cameraPosY + 2.0f;
        float cameraZ = characterPosZ - cos(cameraAngleY * M_PI / 180) * 5.0f;
        float lookAtX = characterPosX;
        float lookAtY = cameraPosY;
        float lookAtZ = characterPosZ;

        gluLookAt(cameraX, cameraY, cameraZ, lookAtX, lookAtY, lookAtZ, 0.0, 1.0, 0.0);
        break;
    }
    case TOP_VIEW:
    {
        float cameraX = characterPosX;
        float cameraY = characterPosY + 5.0f;
        float cameraZ = characterPosZ;
        float lookAtX = characterPosX;
        float lookAtY = characterPosY;
        float lookAtZ = characterPosZ + 1.0f;

        gluLookAt(cameraX, cameraY, cameraZ, lookAtX, lookAtY, lookAtZ, 0.0, 1.0, 0.0);
        break;
    }
    }

    drawFloor();


    glPushMatrix();
    glTranslatef(characterPosX, characterPosY, characterPosZ);

    // change character
    switch (currentCharacter) {
    case 1:
        drawLowCharacter();
        break;
    case 2:
        drawManniCharacter();
        break;
    case 3:
        drawLimCharacter();
        break;
    }

    glPopMatrix();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void mouseMotion(int x, int y) {
    if (isDragging) {
        int dx = x - lastMouseX;
        int dy = y - lastMouseY;


        cameraAngleY += dx * 0.2f;
        cameraAngleX += dy * 0.2f;


        if (cameraAngleX > 89.0f) cameraAngleX = 89.0f;
        if (cameraAngleX < -89.0f) cameraAngleX = -89.0f;

        lastMouseX = x;
        lastMouseY = y;

        glutPostRedisplay();
    }
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = true;
            lastMouseX = x;
            lastMouseY = y;
        }
        else if (state == GLUT_UP) {
            isDragging = false;
        }
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Lab Test 2 by Assasin's Creed Group");
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialKeys);
    glutSpecialUpFunc(specialKeysUp);
    glutIdleFunc(updateJump);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutMainLoop();
    return 0;
}
