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
//g++ 3dSIRSimulation.cpp -L/System/Library/Frameworks -framework GLUT -framework OpenGL -w 

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
	// will be random distribution. some people have many contacts(airports, service industry), some have many less(stay at home worker)
	int number_of_people_infected;
	~Person();
	Person(int);
};

//globals
int prev_random[13]; //this is for drawing the same lines whenever screen is rotating
//if I didn't save the random values, it would recalculate random values at every frame, which is about 1200 per second!
//and we wouldn't see the same contact lines when we rotate the axis!
GLint x_rotation, y_rotation = 0; //values to rotate axes
GLUquadric* my_person; //openGL quadric pointer, this will be for the sphere latr
Disease *Current_Disease; //our disease, you can change parameters on program launch, see main function
Person *myPopulation[13]; //we have 13 people
int do_not_redraw_contact_lines = -1; //this is to tell the display function to keep spinnging, but don't recalculate random contacts
int keep_spinning = 1; //this is to stop or start the spinning that is always happening
//globals
//OpenGL functions 
void init(void) 
{
   glClearColor(0, 0, 0, 0.0);//black background
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
void another_day_infected()
{
	int z = 0;
	for(z=0; z<13; z++)
	{
		if(myPopulation[z]->infected == 1)
		{
			if(myPopulation[z]->days_infected == Current_Disease->disease_period)
			{
				myPopulation[z]->infected = 0;
				myPopulation[z]->susceptible = 0;
				myPopulation[z]->removed = 1;
			}
			else
			{
				myPopulation[z]->days_infected++;//add one day to days infected
			}
			
		}
	}
}
void draw_contact_lines() //this will be in the display function
{
	int x = 0;
	
	if(do_not_redraw_contact_lines == -1) 
	{
		x = 0;
	
		for(x=1; x<13; x++)//loop through our population
		{//don't start with ourself
			//first person contacting all population
			//successful spread will draw a thick green line
			//regular contacts are blue thin lines
			int random = rand()%Current_Disease->infectiousness; //1 in 12
			prev_random[x] = random; //store if we move axis we don't recontact
			
			if(random == 0)
			{
				if(myPopulation[x]->infected == 1)
				{
					if(myPopulation[x]->days_infected == Current_Disease->disease_period)
					{
						myPopulation[x]->infected = 0;
						myPopulation[x]->susceptible = 0;
						myPopulation[x]->removed = 1;
					}
					myPopulation[x]->days_infected++;
				}
				else if(myPopulation[x]->removed != 1) //if not previously infected
				{
					myPopulation[x]->infected = 1;
					myPopulation[x]->susceptible = 0;
				}
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
		do_not_redraw_contact_lines = 1;
	}//we're moving axis, we want old contact lines
	else
	{	
		x=0;
		for(x=1; x<13; x++)//loop through our population
		{//don't start with ourself
			//first person contacting all population
			//successful spread will draw a thick green line
			//regular contacts are blue thin lines
			if(prev_random[x] == 0)
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

}


void draw_spheres()
{
	my_person = gluNewQuadric(); //need to initialize opengl quadric pointer
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
		glTranslatef(7.5,0,0);
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
		glTranslatef(15,0,0);
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
		if(myPopulation[4]->infected == 1) //if person is infected, draw them green
		{
			glColor3f(0, 1.0, 0);
		}
		else if(myPopulation[4]->susceptible == 1) //if person is susceptible, draw them blue
		{
			glColor3f(0, 0, 1.0);
		}
		else if(myPopulation[4]->removed == 1) //if person is removed, draw them red
		{
			glColor3f(1.0, 0, 0);
		}
		gluSphere(my_person, 1, 40, 40);
	glPopMatrix();

	
	glPushMatrix();
		glTranslatef(0, 0, -7.5);
		if(myPopulation[5]->infected == 1) //if person is infected, draw them green
		{
			glColor3f(0, 1.0, 0);
		}
		else if(myPopulation[5]->susceptible == 1) //if person is susceptible, draw them blue
		{
			glColor3f(0, 0, 1.0);
		}
		else if(myPopulation[5]->removed == 1) //if person is removed, draw them red
		{
			glColor3f(1.0, 0, 0);
		}
		gluSphere(my_person, 1, 40, 40);
	glPopMatrix();
	//begin positive z
	
	
	glPushMatrix();
		glTranslatef(0, 0, 7.5);
		if(myPopulation[6]->infected == 1) //if person is infected, draw them green
		{
			glColor3f(0, 1.0, 0);
		}
		else if(myPopulation[6]->susceptible == 1) //if person is susceptible, draw them blue
		{
			glColor3f(0, 0, 1.0);
		}
		else if(myPopulation[6]->removed == 1) //if person is removed, draw them red
		{
			glColor3f(1.0, 0, 0);
		}
		gluSphere(my_person, 1, 40, 40);
	glPopMatrix();
	
	
	glPushMatrix();
		glTranslatef(0, 0, 15);
		if(myPopulation[7]->infected == 1) //if person is infected, draw them green
		{
			glColor3f(0, 1.0, 0);
		}
		else if(myPopulation[7]->susceptible == 1) //if person is susceptible, draw them blue
		{
			glColor3f(0, 0, 1.0);
		}
		else if(myPopulation[7]->removed == 1) //if person is removed, draw them red
		{
			glColor3f(1.0, 0, 0);
		}
		gluSphere(my_person, 1, 40, 40);
	glPopMatrix();
//--------------------------------------y-axis
	
	
	glPushMatrix();
		glTranslatef(0, -15, 0);
		if(myPopulation[8]->infected == 1) //if person is infected, draw them green
		{
			glColor3f(0, 1.0, 0);
		}
		else if(myPopulation[8]->susceptible == 1) //if person is susceptible, draw them blue
		{
			glColor3f(0, 0, 1.0);
		}
		else if(myPopulation[8]->removed == 1) //if person is removed, draw them red
		{
			glColor3f(1.0, 0, 0);
		}
		gluSphere(my_person, 1, 40, 40);
	glPopMatrix();
	
	
	glPushMatrix();
		glTranslatef(0, -7.5, 0);
		if(myPopulation[9]->infected == 1) //if person is infected, draw them green
		{
			glColor3f(0, 1.0, 0);
		}
		else if(myPopulation[9]->susceptible == 1) //if person is susceptible, draw them blue
		{
			glColor3f(0, 0, 1.0);
		}
		else if(myPopulation[9]->removed == 1) //if person is removed, draw them red
		{
			glColor3f(1.0, 0, 0);
		}
		gluSphere(my_person, 1, 40, 40);
	glPopMatrix();
	//end positive y
	
	
	glPushMatrix();
		glTranslatef(0, 7.5, 0);
		if(myPopulation[10]->infected == 1) //if person is infected, draw them green
		{
			glColor3f(0, 1.0, 0);
		}
		else if(myPopulation[10]->susceptible == 1) //if person is susceptible, draw them blue
		{
			glColor3f(0, 0, 1.0);
		}
		else if(myPopulation[10]->removed == 1) //if person is removed, draw them red
		{
			glColor3f(1.0, 0, 0);
		}
		gluSphere(my_person, 1, 40, 40);
	glPopMatrix();

	
	glPushMatrix();
		glTranslatef(0, 15, 0);
		if(myPopulation[11]->infected == 1) //if person is infected, draw them green
		{
			glColor3f(0, 1.0, 0);
		}
		else if(myPopulation[11]->susceptible == 1) //if person is susceptible, draw them blue
		{
			glColor3f(0, 0, 1.0);
		}
		else if(myPopulation[11]->removed == 1) //if person is removed, draw them red
		{
			glColor3f(1.0, 0, 0);
		}
		gluSphere(my_person, 1, 40, 40);
	glPopMatrix();
	//--------------------------------------y-axis
	
	
	glPushMatrix();
		//origin
		if(myPopulation[12]->infected == 1) //if person is infected, draw them green
		{
			glColor3f(0, 1.0, 0);
		}
		else if(myPopulation[12]->susceptible == 1) //if person is susceptible, draw them blue
		{
			glColor3f(0, 0, 1.0);
		}
		else if(myPopulation[12]->removed == 1) //if person is removed, draw them red
		{
			glColor3f(1.0, 0, 0);
		}
		gluSphere(my_person, 1, 40, 40);
	glPopMatrix();
	
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt (0.0, 0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glPushMatrix(); //push modelview matrix
	glRotatef((GLfloat) x_rotation, 1.0, 0.0, 0.0); //x rotation
	glRotatef((GLfloat) y_rotation, 0.0, 1.0, 0.0); //y rotation
	//apply rotations to axes
	
	glLineWidth(4);
	glBegin(GL_LINES);
		glColor3f(1, 1, 1);//white axis lines
		glVertex3f(-15,0,0);
		glVertex3f(15,0,0);
		
		glVertex3f(0,-15,0);
		glVertex3f(0,15,0);

		glVertex3f(0,0,-15);
		glVertex3f(0,0,15);
	glEnd();
	//push rotations
	draw_contact_lines(); //draw some lines!
	draw_spheres();	//draw some spheres!
	glPopMatrix();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   //glTranslatef(-5, 0, 0);
   glFrustum(-1, 1, -1, 1, 1.5, 45.0);//use a 20x20x20 cube for viewport
   
   glMatrixMode(GL_MODELVIEW);
}
//END OpenGL functions
//constructors
Person::Person(int my_contacts_per_day) 
{
	susceptible = true; //start susceptible.
	infected = false;
	removed = false;
	days_infected = 0;
	number_of_contacts_per_day = my_contacts_per_day; 
	// passed on object creation
	number_of_people_infected = 0;
	// starts at 0. really just a fun statistic
}
//constructors
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

	
	for(x=0; x<13; x++)
	{
		// want random contacts from 1-10
		myPopulation[x] = new Person(rand()%10);
		myPopulation[x]->susceptible = 1; //just in case constructor doesn't do its job
		myPopulation[x]->infected = 0;
		myPopulation[x]->removed = 0;
	}
	
	// instantiate our population
}
void Free_Population_Memory() //no memory leaks for later expanded program
{
	for(int x=0; x<12; x++)
	{
		free(myPopulation[x]);
	}
}

void keyboard(unsigned char key, int x, int y)
{
	if(key == 27) //27 is ascii for escape key
	{
		Free_Population_Memory(); //free memory on escape
		free(Current_Disease);
		exit(0);
	}
	else if(key == 'a')
	{
		x_rotation-=2; //rotate x-axis 2 degrees counter clockwise
		
	}
	else if(key == 'd')
	{
		x_rotation+=2; //x-axis clockwise
		
	} 
	else if(key == 'w')
	{
		y_rotation+=2; //y-axis clockwise
		   
	}
	else if(key == 's')
	{
		y_rotation-=2; //rotate y-axis 2 degrees counter-clockwise
		
	}
	else if(key == 'r')
	{
		do_not_redraw_contact_lines*=-1;//make it bounce between -1 and 1 to toggle
		another_day_infected();
	}
	else if(key == 'x') //press x to stop spin
	{
		keep_spinning*=-1; //toggle spin display by bouncing value from -1 to 1
	}
	glutPostRedisplay(); 

}
void spinDisplay(void) {
	if(keep_spinning == 1)
	{
		x_rotation+=1;	
		y_rotation+=1;
	}
	else
	{
		//don't spin!
	}
   glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	Instantiate_Population();
	myPopulation[0]->infected = 1; //infect our first person
	myPopulation[0]->susceptible = 0;
	
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (800, 800);//window size in pixels
	glutInitWindowPosition (0, 0); //initial position of window (0,0) is top left corner
	glutCreateWindow ("Jacob and Isaac SIR Visualization"); //title of window
	init();
	glutIdleFunc(spinDisplay); //idle function spins display
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
		//infectious for 8 days, 1/12 chance to spread disease on contact
		
	}
	
	glutMainLoop();


	return 0;
}