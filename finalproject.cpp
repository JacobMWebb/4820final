#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include <iostream>

using namespace std;

class Person 
{
public:
	bool susceptible;
	bool infected;
	bool recovered;
	short days_infected;
	int number_of_contacts_per_day;
	// will be random distribution. important people have many contacts, some have many less
	int number_of_people_infected;
	~Person();
	Person(int);
};

Person::Person(int my_contacts_per_day) 
{
	susceptible = true;
	infected = false;
	recovered = false;
	days_infected = 0;
	number_of_contacts_per_day = my_contacts_per_day; 
	// passed on object creation
	number_of_people_infected = 0;
	// starts at 0. really just a fun statistic
}


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

Disease::Disease(int my_disease_period, int my_infectiousness)
{
	disease_period = my_disease_period;
	infectiousness = my_infectiousness;
	number_killed = 0;
}
// globals
Disease *Current_Disease;
Person *myPopulation[100][100];
// globals
void Instantiate_Population()
{
	int i,x = 0;

	for(i=0; i<100; i++) 
	{
		for(x=0; x<100; x++)
		{
			// want random contacts from 1-30
			myPopulation[i][x] = new Person(rand()%30);
		}
	}
	// instantiate our population
}
void Free_Population_Memory()
{
	for(int i=0; i<100; i++) 
	{
		for(int x=0; x<100; x++)
		{
			// want random contacts from 1-30
			free(myPopulation[i][x]);
		}
	}
}


int main(int argc, char const *argv[])
{
	srand(time(NULL));
	
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
		
	}
	// launch program with disease peroid and infectiousness

	cout << "Instantiating" << endl;
	Instantiate_Population();
	cout << "Instantiated" << endl;
	cout << "Freeing" << endl;
	Free_Population_Memory();
	cout << "Freed" << endl;

	free(Current_Disease);


	return 0;
}