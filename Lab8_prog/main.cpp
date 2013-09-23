#include <windows.h>
#include <cmath>
#include <ctime>
#include "methods.h"
using namespace std;

int WindW, WindH;

GLfloat light_position0[] = { 100.0, 100.0, 100.0, 0.0 };
GLfloat light_diffuse0[] = {1.0, 1.0, 1.0, 0.0};
GLfloat light_position1[] = { 10.0, 10.0, 10.0, 0.0 };
GLfloat light_diffuse1[] = {0.0, 1, 0, 1.0};
GLfloat light_position2[] = { -10.0, -10.0, -10.0, 0.0 };
GLfloat light_diffuse2[] = {0, 1, 0, 1.0};

bool light0=false, light1=false, light2=false, pause=false, mouseover=true;
int virtualTime=0;
bool cameraToObserver;

double alpha=POSALPHA,beta=POSBETA;

Observer observer;
Polytope polytope;

void enable2D()
{
	glViewport(0, 0, WindW, WindH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1000, 1000, -1000, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void enable3D()
{
	glViewport(0, 0, WindW, WindH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, WindW/double(WindH), 1,100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void en_light()
{
	GLfloat light_position0[] = { observer.position.x, observer.position.y, observer.position.z, 0.0 };
    glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
	if(light0||light1||light2) glEnable(GL_LIGHTING);
	else glDisable(GL_LIGHTING);
    if(light0) glEnable(GL_LIGHT0);
	else glDisable(GL_LIGHT0);
	if(light1) glEnable(GL_LIGHT1);
	else glDisable(GL_LIGHT1);
	if(light2) glEnable(GL_LIGHT2);
	else glDisable(GL_LIGHT2);
}

void pos_light()
{
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse2);
    glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
}

void Controls(int key, int x, int y)
{
	if(!pause)
	{
	if(key==GLUT_KEY_UP)
  {
  }
	if(key==GLUT_KEY_DOWN)
  {
  }
	if(key==GLUT_KEY_RIGHT)
  {
  }
if(key==GLUT_KEY_LEFT)
  {
  }
	}
}

void Draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	en_light();                     
	glPushMatrix();

	if(!cameraToObserver)
		glTranslatef(0, 0, -20);

	glRotated(beta, 1, 0, 0);
	glRotated(alpha, 0, 1, 0);

	if(cameraToObserver)
		glTranslated(-observer.position.x, -observer.position.y, -observer.position.z);

	glBegin(GL_LINES);
	glColor3f(0, 0, 1);  // z axis(blue)
	glVertex3i(0,0,0);
	glVertex3i(0,0,5);
	glColor3f(1, 0, 0);  // y axis(red)
	glVertex3i(0,0,0);
	glVertex3i(0,5,0);
	glColor3f(0, 1, 0);  // x axis(green)
	glVertex3i(0,0,0);
	glVertex3i(5,0,0);
	glEnd();

	observer.draw();
	polytope.draw();


	glPopMatrix();


	enable2D();
	write_text_small(0,0,0,785,975,"Controls:"); 
	write_text_small(0,0,0,785,950,"W,S - move observer x+/x-"); 
	write_text_small(0,0,0,785,925,"A,D - move observer z+/z-"); 
	write_text_small(0,0,0,785,900,"Q,E - move observer up/down");
	write_text_small(0,0,0,785,875,"C - change camera");
	write_text_small(0,0,0,785,850,"Observer:"); 
	write_text_small(0,0,0,785,825, observer.position.ToString().c_str()); 

	glBegin(GL_POLYGON);
	glColor3f(1,1,0);
	glVertex2d(765,1000);
	glVertex2d(780,1000);
	glVertex2d(780,800);
	glVertex2d(765,800);
	glEnd();
	glBegin(GL_POLYGON);
	glColor3f(1,1,0);
	glVertex2d(765,820);
	glVertex2d(765,800);
	glVertex2d(1000,800);
	glVertex2d(1000,820);
	glEnd();

	enable3D();

	glutSwapBuffers();
	glutPostRedisplay();
}

void Reshape(int width, int height)
{
	WindW = width;
	WindH = height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, WindW/double(WindH), 1,100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void initwindow(int argc, char **argv)
{
	WindW = 1000;
	WindH = 700;
	glutInit(&argc, argv);
	glutInitWindowSize(WindW, WindH);
	glutInitWindowPosition(0,0);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("Visibility");

	glClearColor(1, 1, 1, 1.0f);              
	glClearDepth(1.0f);      

	glEnable(GL_DEPTH_TEST);                             
	glDepthFunc(GL_LEQUAL);                              
	glShadeModel(GL_SMOOTH);                             // Включается плавное затенение
	glEnable(GL_LINE_SMOOTH);                            // Включается сглаживание линий
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);              // Выбирается самый качественный режим сглаживания для линий
	glEnable(GL_BLEND);                                  // Включается смешение цветов
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);   // способ смешения
}

void timf(int value)
{
	glutPostRedisplay();
	if(mouseover)
	{
		SetCursorPos(WindW/2,WindH/2);
		ShowCursor(false);
	}
	if(!pause) virtualTime++;
	if(virtualTime==100000) virtualTime=0;
	glutTimerFunc(200, timf, 0);
}

void Visibility(int state)
{
	if (state == GLUT_NOT_VISIBLE) cout<<"Window not visible!\n";
  if (state == GLUT_VISIBLE) cout<<"Window visible!\n";
}

void OnKeyboardEvent(unsigned char keyCode, int x, int y)
{
	switch(keyCode)
	{
		case 's':
			observer.position.x-=STEP;
			polytope.findVisible(observer.position);
			break;
		case 'w':
			observer.position.x+=STEP;
			polytope.findVisible(observer.position);
			break;			
		case 'p':
			break;
		case '4':
			alpha-=1;
			break;
		case '6':
			alpha+=1;
			break;
		case 'q':
			observer.position.y+=STEP;
			polytope.findVisible(observer.position);
			break;
		case 'e':
			observer.position.y-=STEP;
			polytope.findVisible(observer.position);
			break;
		case '2':
			beta+=1;
			break;
		case '8':
			beta-=1;
			break;
		case '5':
			alpha=POSALPHA; beta=POSBETA;
			break;
		case '3':
			if(mouseover)
			{
				mouseover=false;
				ShowCursor(true);
			}
			else
			{
				mouseover=true;
				ShowCursor(false);
			}
			break;
		case 'a':
			observer.position.z+=STEP;
			polytope.findVisible(observer.position);
			break;
		case 'd':
			observer.position.z-=STEP;
			polytope.findVisible(observer.position);
			break;
		case 'g':
			glutFullScreen();
			break;
		case 'l':
			light0=!light0;
			break;
		case 'k':
			light1=!light1;
			break;
		case 'j':
			light2=!light2;
			break;
		case 'c':
			cameraToObserver=!cameraToObserver;
			break;
		case ' ':
			break;
		case Esc:
			exit(0);
	}
}

void OnMouseButton(int button, int state, int x, int y)
{
}

void OnMouse(int x, int y)
{
	if(mouseover)
	{
		beta+=double(y)/WindH*6-3;
		alpha+=double(x)/WindW*6-3;
	}
}

int main(int argc, char **argv)
{
	srand((unsigned int)time(0));

	Point3D observationPoint = Point3D(4, 4, 4);
	observer= Observer(observationPoint);
	//polytope.standart();
	//polytope.generateSome(10);
	polytope.ReadFromFile("input.txt");

	cameraToObserver=false;
	polytope.findVisible(observer.position);

	initwindow(argc, argv);
	glutDisplayFunc(Draw);
	pos_light();
	SetCursorPos(WindW/2,WindH/2);
	glutMouseFunc(OnMouseButton);
	glutPassiveMotionFunc(OnMouse);
	glutReshapeFunc(Reshape);
	glutSpecialFunc(Controls);
	glutTimerFunc(200, timf, 0);
	glutVisibilityFunc(Visibility);
	glutKeyboardFunc(OnKeyboardEvent);
	glutFullScreen();
	glutMainLoop();
	return 0; 
}
