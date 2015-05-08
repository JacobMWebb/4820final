#include "stdlib.h" //opengl
#include "stdio.h" 
#include "time.h" //rand seed
#include <iostream> //making printing easier

//For compiling OpenGL on Mac/Windows
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
//END opengl

using namespace std;

//g++ finalproject.cpp -L/System/Library/Frameworks -framework GLUT -framework OpenGL -w 

class Disease 
{
public:
	int disease_period; 
	// how many days are people infected
	int infectiousness; 
	// how likely are people to transfer the disease
	int number_killed;
	~Disease();
	Disease(int, int);
	// needs to receive its period and probability to spread infection
};
class Person 
{
public:
	bool susceptible;
	bool infected;
	bool removed;
	short days_infected;
	int number_of_contacts_per_day;
	// will be random distribution. important people have many contacts, some have many less
	int number_of_people_infected;
	~Person();
	Person(int);
};






//OpenGL functions 




GLint x_rotation, y_rotation = 0;
GLUquadric* my_person; //will be a sphere
Disease *Current_Disease;
Person *myPopulation[13]; //we have 10 people

void init(void)
{
   glClearColor(0.0, 0.0, 0.0, 0.0);//black background
   glShadeModel(GL_SMOOTH);
  

}
//SPHERE-PERSON DRAWING SCHEME
//1,2,3,4 are x-axis spheres
//5,6,7,8 are z-axis spheres
//9,10,11,12 are y-axis sphere
//13th sphere is origin sphere
//glcolor3f(R,G,B) is RGB values from 0 to 1. 
//blue = susceptible
//green = infected
//red = removed
void draw_spheres()
{
	glColor3f(1.0, 1.0, 1.0); 
	my_person = gluNewQuadric();
	gluQuadricDrawStyle(my_person, GLU_POINT);//draw filled green spheres
	//x-axis
	glPushMatrix();
		glTranslatef(-15,0,0);//far end of x-axis
		if(myPopulation[0]->infected == true) //if person is infected, draw them green
		{
			glColor3f(0,1.0,0);
		}
		else if(myPopulation[0]->susceptible == true) //if person is susceptible, draw them blue
		{
			glColor3f(0,0,1.0);
		}
		else if(myPopulation[0]->removed == true) //if person is removed, draw them red
		{
			glColor3f(1.0,0,0);
		}
		gluSphere(my_person, 1, 15, 15); //draw person (sphere)
	glPopMatrix();//pop translation matrix
	
	glColor3f(1.0, 1.0, 1.0); //
	glPushMatrix();
		glTranslatef(-7.5,0,0);
			if(myPopulation[1]->infected == true) //if person is infected, draw them green
		{
			glColor3f(0,1.0,0);
		}
		else if(myPopulation[1]->susceptible == true) //if person is susceptible, draw them blue
		{
			glColor3f(0,0,1.0);
		}
		else if(myPopulation[1]->removed == true) //if person is removed, draw them red
		{
			glColor3f(1.0,0,0);
		}
		gluSphere(my_person, 1, 15, 15);
	glPopMatrix();
	
	glColor3f(1.0, 1.0, 1.0); //
	//begin positive x
	glPushMatrix();
		glTranslatef(15,0,0);
			if(myPopulation[2]->infected == true) //if person is infected, draw them green
		{
			glColor3f(0,1.0,0);
		}
		else if(myPopulation[2]->susceptible == true) //if person is susceptible, draw them blue
		{
			glColor3f(0,0,1.0);
		}
		else if(myPopulation[2]->removed == true) //if person is removed, draw them red
		{
			glColor3f(1.0,0,0);
		}
		gluSphere(my_person, 1, 15, 15);
	glPopMatrix();
	
	glColor3f(1.0, 1.0, 1.0); //
	glPushMatrix();
		glTranslatef(7.5,0,0);
			if(myPopulation[3]->infected == true) //if person is infected, draw them green
		{
			glColor3f(0,1.0,0);
		}
		else if(myPopulation[3]->susceptible == true) //if person is susceptible, draw them blue
		{
			glColor3f(0,0,1.0);
		}
		else if(myPopulation[3]->removed == true) //if person is removed, draw them red
		{
			glColor3f(1.0,0,0);
		}
		gluSphere(my_person, 1, 15, 15);
	glPopMatrix();
//------------------------------------------------z-axis
	glColor3f(1.0, 1.0, 1.0); //
	glPushMatrix();
		glTranslatef(0,0,-15);
		gluSphere(my_person, 1, 15, 15);
	glPopMatrix();

	glColor3f(1.0, 1.0, 1.0); //
	glPushMatrix();
		glTranslatef(0,0,-7.5);
		gluSphere(my_person, 1, 15, 15);
	glPopMatrix();
	//begin positive z
	
	glColor3f(1.0, 1.0, 1.0); //
	glPushMatrix();
		glTranslatef(0,0,15);
		gluSphere(my_person, 1, 15, 15);
	glPopMatrix();
	
	glColor3f(1.0, 1.0, 1.0); //
	glPushMatrix();
		glTranslatef(0,0,7.5);
		gluSphere(my_person, 1, 15, 15);
	glPopMatrix();
//--------------------------------------y-axis
	
	glColor3f(1.0, 1.0, 1.0); //
	glPushMatrix();
		glTranslatef(0,15,0);
		gluSphere(my_person, 1, 15, 15);
	glPopMatrix();
	
	glColor3f(1.0, 1.0, 1.0); //
	glPushMatrix();
		glTranslatef(0,7.5,0);
		gluSphere(my_person, 1, 15, 15);
	glPopMatrix();
	//end positive y
	
	glColor3f(1.0, 1.0, 1.0); //
	glPushMatrix();
		glTranslatef(0,-7.5,0);
		gluSphere(my_person, 1, 15, 15);
	glPopMatrix();

	glColor3f(1.0, 1.0, 1.0); //
	glPushMatrix();
		glTranslatef(0,-15,0);
		gluSphere(my_person, 1, 15, 15);
	glPopMatrix();
	//--------------------------------------y-axis
	
	glColor3f(1.0, 1.0, 1.0); //
	glPushMatrix();
		//origin
		gluSphere(my_person, 1, 15, 15);
	glPopMatrix();
}
void display(void)
{
	int i = 0;

	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	glRotatef((GLfloat) x_rotation, 1.0, 0.0, 0.0);
	glRotatef((GLfloat) y_rotation, 0.0, 1.0, 0.0);
	
	//apply rotations to axes
	glLineWidth(3);
	glBegin(GL_LINES);
		glColor3f(1,1,1);//white lines
		glVertex3f(-15,0,0);
		glVertex3f(15,0,0);
		
		glVertex3f(0,-15,0);
		glVertex3f(0,15,0);

		glVertex3f(0,0,-15);
		glVertex3f(0,0,15);
	glEnd();

	//push rotations
	

	glColor3f(0.0, 1.0, 0.0);
	my_person = gluNewQuadric();
	gluQuadricDrawStyle(my_person, GLU_POINT);//draw filled green spheres
	
	draw_spheres();
	glPopMatrix();
	
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	if(key == 27)
	{
		exit(0);
	}
	else if(key == 'a')
	{
		x_rotation-=2;
		glutPostRedisplay();    
	}
	else if(key == 'd')
	{
		x_rotation+=2;
		glutPostRedisplay();    
	}
	else if(key == 'w')
	{
		y_rotation+=2;
		glutPostRedisplay();    
	}
	else if(key == 's')
	{
		y_rotation-=2;
		glutPostRedisplay();    
	}

}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   //because my control points roughly represent x^2
   glOrtho(-20, 20.0, -20, 20, -20.0, 20.0);
   
   glMatrixMode(GL_MODELVIEW);
}


//END OpenGL functions


Person::Person(int my_contacts_per_day) 
{
	susceptible = true;
	infected = false;
	removed = false;
	days_infected = 0;
	number_of_contacts_per_day = my_contacts_per_day; 
	// passed on object creation
	number_of_people_infected = 0;
	// starts at 0. really just a fun statistic
}




Disease::Disease(int my_disease_period, int my_infectiousness)
{
	disease_period = my_disease_period;
	infectiousness = my_infectiousness;
	number_killed = 0;
}
// globals
// globals
void Instantiate_Population()
{
	int i,x = 0;

	
	for(x=0; x<12; x++)
	{
		// want random contacts from 1-10
		myPopulation[x] = new Person(rand()%10);
	}
	
	// instantiate our population
}
void Free_Population_Memory()
{
	
	for(int x=0; x<12; x++)
	{
		
		free(myPopulation[x]);
	}
}


int main(int argc, char *argv[])
{
	
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (800, 800);//window size in pixels
	glutInitWindowPosition (0, 0); //initial position of window (0,0) is top left corner
	glutCreateWindow ("Jacob and Isaac SIR Visualization"); //title of window
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display); //display callback
	
	glutKeyboardFunc(keyboard); //keyboard callback
	
	srand(time(NULL)); //seed our random function
	
	if(argv[0] && argv[1] != NULL)
	{
		int period = atoi(argv[0]);
		int infectiousness = atoi(argv[1]);
		Current_Disease = new Disease(period,infectiousness);
	}
	else
	{
		cout << "Disease parameters not provided." << endl;
		Current_Disease = new Disease(8, 10);
		//infectious for 8 days, 1/10 chance to spread disease on contact
		
	}
	// launch program with disease peroid and infectiousness

	
	Instantiate_Population();
	Free_Population_Memory();
	free(Current_Disease);
	// 
	glutMainLoop();


	return 0;
}