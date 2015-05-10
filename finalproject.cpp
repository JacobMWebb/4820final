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
//				***TO COMPILE ON MAC OS DEVICES ***
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
	int susceptible;
	int infected;
	int removed;
	int days_infected;
	int number_of_contacts_per_day;
	// will be random distribution. important people have many contacts, some have many less
	int number_of_people_infected;
	~Person();
	Person(int);
};






//OpenGL functions 
int prev_random = 0;
GLint x_rotation, y_rotation = 0;
GLUquadric* my_person; //will be a sphere
Disease *Current_Disease;
Person *myPopulation[13]; //we have 13 people
int do_not_redraw_contact_lines = -1;

void init(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0);//white background
   glShadeModel(GL_SMOOTH); //use smooth shading
}
//Going to store sphere coordinates here to do math for drawing contact lines
GLfloat sphere_coordinates[13][3] = //13 spheres each with 3 x,y,z coordinates
{
	{-15, 0, 0}, {-7.5, 0, 0}, {7.5, 0, 0}, {15, 0, 0}, //x
	{0, 0, -15}, {0, 0, -7.5}, {0, 0, 7.5}, {0, 0, 15}, //z
	{0, -15, 0}, {0, -7.5, 0}, {0, 7.5, 0}, {0, 15, 0}  //y
};

//
//SPHERE-PERSON DRAWING SCHEME
//1,2,3,4 are x-axis spheres
//5,6,7,8 are z-axis spheres
//9,10,11,12 are y-axis sphere
//13th sphere is origin sphere
//glcolor3f(R,G,B) is RGB values from 0 to 1. 
//blue = susceptible
//green = infected
//red = removed
void population_contact()
{
	int x = 0;
	int random = rand()%Current_Disease->infectiousness; //1 in 12
	for(x=1; x<13; x++)
	{
		if(myPopulation[0]->infected == 1 && random == 0) //1 in 12
		{
			myPopulation[x]->infected = 1;//infect
			myPopulation[x]->susceptible = 0;
		}

	}
}
void draw_contact_lines() //this will be in the display function
{
	int x = 0;
	int random = rand()%Current_Disease->infectiousness; //1 in 12
	prev_random = random;
	for(x=1; x<13; x++)//loop through our population
	{//don't start with ourself
		//first person contacting all population
		//successful spread will draw a thick green line
		//regular contacts are blue thin lines
		
		
		if(do_not_redraw_contact_lines == -1) //infectiousness is a value from 1-12 if no params given
		{//so if a random number % 12 = 0 (1/12 chance) is true we spread. (13 people, 1/12 should spread. should.)
			if(random == 0)
			{
				glLineWidth(6);
				glBegin(GL_LINES);
					glColor3f(0, 1.0, 0);
					if(x == 1 || x == 2 || x == 3 || x == 12)
					{
						//need a midpoint to see line drawn to people on current axis
						GLfloat midpoint = sphere_coordinates[0][0] + sphere_coordinates[x][0];
						midpoint /=2;
						glVertex3f(sphere_coordinates[0][0], sphere_coordinates[0][1], sphere_coordinates[0][2]); //first point
						glVertex3f(midpoint, sphere_coordinates[x][1]+2, sphere_coordinates[x][2]); //second point
						//second point 2 units higher halfway inbetween
						glVertex3f(midpoint, sphere_coordinates[x][1]+2, sphere_coordinates[x][2]); //second point
						glVertex3f(sphere_coordinates[x][0], sphere_coordinates[x][1], sphere_coordinates[x][2]); //third point

					}
					else
					{
						glVertex3f(sphere_coordinates[0][0], sphere_coordinates[0][1], sphere_coordinates[0][2]); //first person
						glVertex3f(sphere_coordinates[x][0], sphere_coordinates[x][1], sphere_coordinates[x][2]); 
					}
					//REMEMBER, x is our loop coordinate, draw a line from first person to person who made contact 
				glEnd();
			}	
			else  //draw blue line
			{
				glLineWidth(2);
				glColor3f(0, 0, 1.0);
				glBegin(GL_LINES);
					if(x == 1 || x == 2 || x == 3 || x == 12)
					{
						//need a midpoint to see line drawn to people on current axis
						GLfloat midpoint = sphere_coordinates[0][0] + sphere_coordinates[x][0];
						midpoint /=2;
						glVertex3f(sphere_coordinates[0][0], sphere_coordinates[0][1], sphere_coordinates[0][2]); //first point
						glVertex3f(midpoint, sphere_coordinates[x][1]+2, sphere_coordinates[x][2]); //second point
						//second point 2 units higher halfway inbetween
						glVertex3f(midpoint, sphere_coordinates[x][1]+2, sphere_coordinates[x][2]); //second point
						glVertex3f(sphere_coordinates[x][0], sphere_coordinates[x][1], sphere_coordinates[x][2]); //third point

					}
					else
					{
						glVertex3f(sphere_coordinates[0][0], sphere_coordinates[0][1], sphere_coordinates[0][2]); //first person
						glVertex3f(sphere_coordinates[x][0], sphere_coordinates[x][1], sphere_coordinates[x][2]); 
					}
					
				glEnd();
			}
		}
	}
	do_not_redraw_contact_lines = 1;
}
void draw_spheres()
{
	
	my_person = gluNewQuadric();
	gluQuadricDrawStyle(my_person, GLU_POINT);//draw filled green spheres
	//x-axis
	glPushMatrix();
		glTranslatef(-15, 0, 0);//far end of x-axis
		if(myPopulation[0]->infected == 1) //if person is infected, draw them green
		{	
			glColor3f(0, 1.0, 0);
		}
		else if(myPopulation[0]->susceptible == 1) //if person is susceptible, draw them blue
		{
			glColor3f(0, 0, 1.0);
		}
		else if(myPopulation[0]->removed == 1) //if person is removed, draw them red
		{
			glColor3f(1.0, 0, 0);
		}
		gluSphere(my_person, 1, 40, 40); //draw person (sphere)
	glPopMatrix();//pop translation matrix
	
	
	glPushMatrix();
		glTranslatef(-7.5, 0, 0);
		if(myPopulation[1]->infected == 1) //if person is infected, draw them green
		{
			glColor3f(0, 1.0, 0);
		}
		else if(myPopulation[1]->susceptible == 1) //if person is susceptible, draw them blue
		{
			glColor3f(0, 0, 1.0);
		}
		else if(myPopulation[1]->removed == 1) //if person is removed, draw them red
		{
			glColor3f(1.0, 0, 0);
		}
		gluSphere(my_person, 1, 40, 40);
	glPopMatrix();
	
	
	//begin positive x
	glPushMatrix();
		glTranslatef(15,0,0);
		if(myPopulation[2]->infected == 1) //if person is infected, draw them green
		{
			glColor3f(0, 1.0, 0);
		}
		else if(myPopulation[2]->susceptible == 1) //if person is susceptible, draw them blue
		{
			glColor3f(0, 0, 1.0);
		}
		else if(myPopulation[2]->removed == 1) //if person is removed, draw them red
		{
			glColor3f(1.0, 0, 0);
		}
		gluSphere(my_person, 1, 40, 40);
	glPopMatrix();
	
	
	glPushMatrix();
		glTranslatef(7.5,0,0);
		if(myPopulation[3]->infected == 1) //if person is infected, draw them green
		{
			glColor3f(0, 1.0, 0);
		}
		else if(myPopulation[3]->susceptible == 1) //if person is susceptible, draw them blue
		{
			glColor3f(0, 0, 1.0);
		}
		else if(myPopulation[3]->removed == 1) //if person is removed, draw them red
		{
			glColor3f(1.0, 0, 0);
		}
		gluSphere(my_person, 1, 40, 40);
	glPopMatrix();
//------------------------------------------------z-axis
	
	glPushMatrix();
		glTranslatef(0, 0, -15);
		gluSphere(my_person, 1, 40, 40);
	glPopMatrix();

	
	glPushMatrix();
		glTranslatef(0, 0, -7.5);
		gluSphere(my_person, 1, 40, 40);
	glPopMatrix();
	//begin positive z
	
	
	glPushMatrix();
		glTranslatef(0, 0, 15);
		gluSphere(my_person, 1, 40, 40);
	glPopMatrix();
	
	
	glPushMatrix();
		glTranslatef(0, 0, 7.5);
		gluSphere(my_person, 1, 40, 40);
	glPopMatrix();
//--------------------------------------y-axis
	
	
	glPushMatrix();
		glTranslatef(0, 15, 0);
		gluSphere(my_person, 1, 40, 40);
	glPopMatrix();
	
	
	glPushMatrix();
		glTranslatef(0, 7.5, 0);
		gluSphere(my_person, 1, 40, 40);
	glPopMatrix();
	//end positive y
	
	
	glPushMatrix();
		glTranslatef(0, -7.5, 0);
		gluSphere(my_person, 1, 40, 40);
	glPopMatrix();

	
	glPushMatrix();
		glTranslatef(0, -15, 0);
		gluSphere(my_person, 1, 40, 40);
	glPopMatrix();
	//--------------------------------------y-axis
	
	
	glPushMatrix();
		//origin
		gluSphere(my_person, 1, 40, 40);
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
	
	glLineWidth(4);
	glBegin(GL_LINES);
		glColor3f(0, 0, 0);//black axis lines
		glVertex3f(-15,0,0);
		glVertex3f(15,0,0);
		
		glVertex3f(0,-15,0);
		glVertex3f(0,15,0);

		glVertex3f(0,0,-15);
		glVertex3f(0,0,15);
	glEnd();
	//push rotations
	

	//glColor3f(0.0, 1.0, 0.0);
	//glClear(GL_COLOR_BUFFER_BIT);
	my_person = gluNewQuadric();
	gluQuadricDrawStyle(my_person, GLU_POINT);//draw filled 
	draw_spheres();
	draw_contact_lines();
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
		x_rotation-=2; //rotate x-axis 2 degrees counter clockwise
		glutPostRedisplay(); //this tell the program to redraw window with new rotation
	}
	else if(key == 'd')
	{
		x_rotation+=2; //x-axis clockwise
		glutPostRedisplay();    
	} 
	else if(key == 'w')
	{
		y_rotation+=2; //y-axis clockwise
		glutPostRedisplay();    
	}
	else if(key == 's')
	{
		y_rotation-=2; //rotate y-axis 2 degrees counter-clockwise
		glutPostRedisplay();    
	}
	else if(key == 'r')
	{
		do_not_redraw_contact_lines*=-1;//make it bounce between -1 and 1 to toggle
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
		myPopulation[x]->susceptible = 1;
		myPopulation[x]->infected = 0;
		myPopulation[x]->removed = 0;
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
		Current_Disease = new Disease(8, 12);
		//infectious for 8 days, 1/10 chance to spread disease on contact
		
	}
	// launch program with disease peroid and infectiousness

	
	Instantiate_Population();
	myPopulation[0]->infected = 1;
	myPopulation[0]->susceptible = 0;
	
	// 
	glutMainLoop();
	Free_Population_Memory();
	free(Current_Disease);

	return 0;
}