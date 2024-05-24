#include <stdio.h>    
#include <string.h>    
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

GLfloat headRotationAngle = 90.0;
GLfloat armRotationAngle = 0.0;
GLfloat forearmRotationAngle = 0.0;

float xpos = 0, ypos = 0, zpos = 0, xrot = 0, yrot = 0, angle = 0.0;
float lastx, lasty;
double limbAngle = 0.0; //Walking angle
bool walk = false;

enum CameraView {
	FIRST_PERSON,
	THIRD_PERSON,
	TOP_VIEW
};
CameraView currentView = THIRD_PERSON; // Initial camera view

float cameraAngleX = 0.0f; // Angle to rotate the camera around the x-axis
float cameraAngleY = 0.0f; // Angle to rotate the camera around the y-axis
float cameraPosX = 0.0f;  // Camera position X
float cameraPosY = 1.6f;  // Camera position Y (head height)
float cameraPosZ = 0.0f;  // Camera position Z
float cameraSpeed = 0.1f; // Speed of camera movement

float characterPosX = 0.0f; // Character position X
float characterPosZ = 0.0f; // Character position Z
float characterPosY = 0.0f; // Character position Z

int lastMouseX = 0;        // Last known X position of the mouse
int lastMouseY = 0;        // Last known Y position of the mouse
bool isDragging = false;   // Flag to check if mouse is being dragged
int currentCharacter = 1;

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

void body(void)
{
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.2);
	glScalef(2.5, 3, 1.5);
	glColor3f(0.0, 0.75, 0.75);
	glutSolidCube(0.3);
	glPopMatrix();
}

void head() {
	//neck
	glPushMatrix();
	glTranslatef(0.0, 0.525, 0.24);
	glScalef(0.5,0.5,0.5);
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
	glTranslatef(0.15,( 3 + 1 + 2 * 2 / 3 )* 0.3 * 0.5, 0.45);
	glScalef(0.15, 0.15, 0.05);
	glColor3f(0.7, 0.7, 0.7);
	glutSolidCube(0.3);
	glPopMatrix();

	//left eye
	glPushMatrix();
	glTranslatef(-0.15,( 3 + 1 + 2 * 2 / 3 )* 0.3 * 0.5, 0.45);
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
	glTranslatef(0, 0.66, 0.45); //until here
	glScalef(0.85, 0.15, 0);
	glColor3f(0, 0, 0);
	glutSolidCube(0.3);
	glPopMatrix();
}

void leftArm()
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

void rightArm()
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

void leftLeg()
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

	glScalef(0.625, 0.25, 1.0);
	glTranslatef(0, -0.2625, 0);
	glColor3f(0.3, 0, 0);
	glutSolidCube(0.3);
	glPopMatrix();
}

void rightLeg()
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

	glScalef(0.625, 0.25, 1.0);
	glTranslatef(0, -0.2625, 0);
	glColor3f(0.3, 0, 0);
	glutSolidCube(0.3);
	glPopMatrix();
}

void mouseMotion(int x, int y) {
	if (isDragging) {
		int dx = x - lastMouseX; // Change in x
		int dy = y - lastMouseY; // Change in y

		// Update camera angles based on mouse movement
		cameraAngleY += dx * 0.2f; // Adjust sensitivity as needed
		cameraAngleX += dy * 0.2f; // Adjust sensitivity as needed

		// Clamp the x-axis angle to prevent flipping
		if (cameraAngleX > 89.0f) cameraAngleX = 89.0f;
		if (cameraAngleX < -89.0f) cameraAngleX = -89.0f;

		lastMouseX = x; // Update last mouse positions
		lastMouseY = y;

		glutPostRedisplay(); // Redraw the scene
	}
}

// Function to handle mouse button events
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

void camera(void) {
	glRotatef(xrot, 1.0, 0.0, 0.0); //rotate our camera on teh x - axis(left and right)
	glRotatef(yrot, 0.0, 1.0, 0.0); //rotate our camera on the y - axis(up and down)
	glTranslated(-xpos, -ypos, -zpos); //translate the screento the position of our camera
}

void keyboard(unsigned char key, int x, int y) {
	float xrotrad, yrotrad;
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

	case 'w':
		characterPosX += sin(cameraAngleY * M_PI / 180) * cameraSpeed * 10;
		characterPosZ += cos(cameraAngleY * M_PI / 180) * cameraSpeed * 10;
		yrotrad = (yrot / 180 * 3.141592654f);
		xrotrad = (xrot / 180 * 3.141592654f);
		xpos += float(sin(yrotrad));
		zpos -= float(cos(yrotrad));
		ypos -= float(sin(xrotrad));
		break;
	case 's':
		characterPosX -= sin(cameraAngleY * M_PI / 180) * cameraSpeed * 10;
		characterPosZ -= cos(cameraAngleY * M_PI / 180) * cameraSpeed * 10;
		yrotrad = (yrot / 180 * 3.141592654f);
		xrotrad = (xrot / 180 * 3.141592654f);
		xpos -= float(sin(yrotrad));
		zpos += float(cos(yrotrad));
		ypos += float(sin(xrotrad));
		if (walk == false) {
			walk = true;
		}
		else if (walk == true) {
			walk = false;
		}
		break;
	case 'a':
		characterPosX += cos(cameraAngleY * M_PI / 180) * cameraSpeed * 10;
		characterPosZ -= sin(cameraAngleY * M_PI / 180) * cameraSpeed * 10;
		yrot -= 1;
		if (yrot < -360)yrot += 360;
		break;
	case 'd':
		characterPosX -= cos(cameraAngleY * M_PI / 180) * cameraSpeed * 10;
		characterPosZ += sin(cameraAngleY * M_PI / 180) * cameraSpeed * 10;
		yrot += 1;
		if (yrot > 360) yrot -= 360;
		yrot = 0.0;
		xrot = 0.0;
		walk = false;
		break;
	}
	/*sets a flag so that when the mainloop is ready it will call the real "display" callback once per mainloop iteration*/
	glutPostRedisplay();
}

void arrowKeyPress(int key, int x, int y) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (key) {
	case GLUT_KEY_RIGHT:
		xrot += 2;
		break;
	case GLUT_KEY_LEFT:
		xrot -= 2;
		break;
	case GLUT_KEY_UP:
		yrot -= 2;
		break;
	case GLUT_KEY_DOWN:
		yrot += 2;
		break;
	}

	glutPostRedisplay();		// redraw the image now
}

void display(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.50, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	camera();

	if (walk == true) {
		//Rotating around Robot
		glPushMatrix();
		glRotated(yrot, 0.1, 0.0, 0.0f);
		glRotated(xrot, 0.0, 0.1, 0.0f);
		body();
		head();
		glPopMatrix();
		//Left arm walking movement
		glPushMatrix();
		glRotated(yrot, 0.1, 0.0, 0.0f);
		glRotated(xrot, 0.0, 0.1, 0.0f);
		glTranslated(-0.30, 0.1, 0.0);
		glRotated(limbAngle, 0.1, 0.0, 0.0f);
		glTranslated(0.30, -0.10, 0.0);
		leftArm();
		glPopMatrix();
		//Right arm walking movement
		glPushMatrix();
		glRotated(yrot, 0.1, 0.0, 0.0f);
		glRotated(xrot, 0.0, 0.1, 0.0f);
		glTranslated(0.30, 0.1, 0.0);
		glRotated(-limbAngle, 1.0, 0.0, 0.0f);
		glTranslated(-0.30, -0.1, 0.0);
		rightArm();
		glPopMatrix();
		//Left leg walking movement
		glPushMatrix();
		glRotated(yrot, 0.1, 0.0, 0.0f);
		glRotated(xrot, 0.0, 0.1, 0.0f);
		glRotated(-limbAngle, 0.1, 0.0, 0.0f);
		leftLeg();
		glPopMatrix();
		//Right leg walking movement
		glPushMatrix();
		glRotated(yrot, 0.1, 0.0, 0.0f);
		glRotated(xrot, 0.0, 0.1, 0.0f);
		glRotated(limbAngle, 0.1, 0.0, 0.0f);
		rightLeg();
		glPopMatrix();
	}
	else if (walk == false) {
		//Rotating around Robot and no Walking
		glPushMatrix();
		glRotated(yrot, 0.1, 0.0, 0.0f);
		glRotated(xrot, 0.0, 0.1, 0.0f);
		body();
		head();
		rightArm();
		leftArm();
		rightLeg();
		leftLeg();
		glPopMatrix();
	}

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2.0 * (64 / 48.0), 2.0 * (64 / 48.0), -2.0, 2.0, 0.1, 100.0); //left, right, bottom, top, near, far Parallel Projection
	glViewport(0, 0, 800, 640);
}

void Walk(void)
{
	static float mf = 0.05;	//limb movement factor

	limbAngle += mf;

	if (limbAngle > 30.0) {
		mf = -0.10;
	}
	if (limbAngle < -30.0) {
		mf = 0.10;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 700);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Robot");
	glutDisplayFunc(display);
	glutIdleFunc(Walk);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);       // Register mouse button callback function
	glutMotionFunc(mouseMotion); // Register mouse motion callback function
	glutKeyboardFunc(keyboard); //check the keyboard
	glutSpecialFunc(arrowKeyPress);
	init();
	glutMainLoop();
	return 0;
}

