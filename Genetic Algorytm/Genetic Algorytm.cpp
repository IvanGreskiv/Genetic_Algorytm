// Genetic Algorytm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>					// for enter from the screen
#include <vector>					// for class vector
#include <string>					// for class string
#include <algorithm>				// for sort algorithm
#include <time.h>					// for randomize
#include <math.h>

#define GA_POPSIZE		2048		// size of population
#define GA_MAXITER		16384		// maximum numbers of iteration
#define GA_ELITRATE		0.10f		// elitarizm
#define GA_MUTATIONRATE	0.25f		// chance for mutation
#define GA_MUTATION		RAND_MAX * GA_MUTATIONRATE  //mutation
#define GA_TARGET		std::string("Hello!")  //word which we want to generate 

using namespace std;

struct ga_struct
{
	string str;								// cap say: "String!"
	unsigned int fitness;					// fitness function, or function which generate chance to mutation
};

typedef vector<ga_struct> ga_vector;			//definition of vector by structure

void init_population(ga_vector &population, ga_vector &buffer) //initialization of start population
{
	int tsize = GA_TARGET.size(); //we use a global variable. We define them on line 12. Our population write to the vector

	for (int i = 0; i<GA_POPSIZE; i++) {
		ga_struct citizen;

		citizen.fitness = 0;
		citizen.str.erase();

		for (int j = 0; j<tsize; j++)
			citizen.str += (rand() % 90) + 32;

		population.push_back(citizen);
	}

	buffer.resize(GA_POPSIZE); //to economy of memory, we resize global variable and set the size of our population
}

void calc_fitness(ga_vector &population)  //calculation of fitness. You can see the definition of fitness function in the .doc file. 
{
	string target = GA_TARGET;
	int tsize = target.size();
	unsigned int fitness;

	for (int i = 0; i<GA_POPSIZE; i++) {
		fitness = 0;
		for (int j = 0; j<tsize; j++) {
			fitness += abs(int(population[i].str[j] - target[j])); 
		}

		population[i].fitness = fitness;
	}
}

bool fitness_sort(ga_struct x, ga_struct y) //sort of new generation
{
	return (x.fitness < y.fitness);
}

inline void sort_by_fitness(ga_vector &population)
{
	sort(population.begin(), population.end(), fitness_sort);
}

void elitism(ga_vector &population,
	ga_vector &buffer, int esize)
{
	for (int i = 0; i<esize; i++) {
		buffer[i].str = population[i].str;
		buffer[i].fitness = population[i].fitness;
	}
}

void mutate(ga_struct &member)
{
	int tsize = GA_TARGET.size();
	int ipos = rand() % tsize;
	int delta = (rand() % 90) + 32;

	member.str[ipos] = ((member.str[ipos] + delta) % 122);
}

void mate(ga_vector &population, ga_vector &buffer)
{
	int esize = GA_POPSIZE * GA_ELITRATE;
	int tsize = GA_TARGET.size(), spos, i1, i2;

	elitism(population, buffer, esize);

	// Mate the rest
	for (int i = esize; i<GA_POPSIZE; i++) {
		i1 = rand() % (GA_POPSIZE / 2);
		i2 = rand() % (GA_POPSIZE / 2);
		spos = rand() % tsize;

		buffer[i].str = population[i1].str.substr(0, spos) +
			population[i2].str.substr(spos, esize - spos);

		if (rand() < GA_MUTATION) mutate(buffer[i]);
	}
}

inline void print_best(ga_vector &gav)
{
	cout << "Best: " << gav[0].str << " (" << gav[0].fitness << ")" << endl;
}

inline void swap(ga_vector *&population,
	ga_vector *&buffer)
{
	ga_vector *temp = population; population = buffer; buffer = temp;
}



int _tmain(int argc, _TCHAR* argv[])
{
	srand(unsigned(time(NULL)));

	ga_vector pop_alpha, pop_beta;
	ga_vector *population, *buffer;

	init_population(pop_alpha, pop_beta);
	population = &pop_alpha;
	buffer = &pop_beta;

	for (int i = 0; i<GA_MAXITER; i++) {
		calc_fitness(*population);		// calc fitness
		sort_by_fitness(*population);		// sort population
		print_best(*population);		// print pest population

		if ((*population)[0].fitness == 0) break;

		mate(*population, *buffer);		// generation of next generation of population
		swap(population, buffer);		// clear buffers
	}
	system("pause");
	return 0;
}

