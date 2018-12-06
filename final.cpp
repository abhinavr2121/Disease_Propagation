#include <vector>
#include <math.h>
#include <iostream>
#include <string>
#include <algorithm>

using std::cout;
using std::endl;
using std::vector;
using std::cin;
using std::string;
using std::find;

class Person {
	public:
		string status;
		int infected_duration;

		Person() {status = "susceptible"; infected_duration = 0;}
		void infect(int duration) {
			if(status == "susceptible") {
				status = "infected";
				infected_duration = duration;
			}
		}
		void set_status(string new_status) {
		    status = new_status;
		}
		string status_string() {
			return status;
		}
		int getPersonDuration() {
			return infected_duration;
		}
		bool is_stable() {
			if(status == "recovered" || status == "inoculated") {
				return true;
			}
			else {
				return false;
			}
		}
		void update() {
			if(status == "infected") {
				if(infected_duration - 1 == 0) {
					status = "recovered";
				}
				else {
					infected_duration--;
				}
			}
		}
};

class Contagion {
	public:
		float probability;
		int duration;
		Contagion(float p, int d) {
			probability = p;
			duration = d;
		}
		float getProbability() {
			return probability;
		}
		int getDuration() {
			return duration;
		}
};

class Population {
	public:
	    int size;
	    int days;
	    vector<Person> population;
	    Population(int n) {
		    size = n;
		    days = 0;
		    Person p;
		    population = vector<Person>(size, p);
	    }
	    void interact(int r1, int r2, Contagion c) {
		Person one = population[r1];
		Person two = population[r2];
		    	if(one.status_string() == "susceptible" || two.status_string() == "susceptible") {
				int potential = 1 + (rand() % 100);
				if(potential < c.getProbability() * 100) {
					if(one.status_string() == "infected") {
						two.infect(c.getDuration());
					}
					else if(two.status_string() == "infected") {
						one.infect(c.getDuration());
					}
				}
			}	
		population[r1] = one;
		population[r2] = two;
	    }
	    int getDays() {
		    return days;
	    }
	    Person getElement(int i) {
	        return population[i];
	    }
	    void random_inoculation(float j) {
	    	vector<int> initials((int)(size * j), 0);
		for(int i = 0; i < (int)(size * j); i++) {
			int random = rand() % size;
			while(find(initials.begin(), initials.end(), random) != initials.end()) {
		    		random = rand() % size;
			}
			initials.push_back(random);
			population[random].set_status("inoculated");
		    }
	    }
	    void random_infection(int d) {
		int random = rand() % size;
		Person person = population[random];
		while(person.status_string() == "inoculated") {
			random = rand() % size;
			person = population[random];
		}
		person.infect(d);
		population[random] = person;
	    }
	    int count_infected() {
		    int counter = 0;
		    for(int i = 0; i < size; i++) {
			    if(population[i].status_string() == "infected") {
				    counter++;
			    }
		    }
		    return counter;
	    }
	int count_susceptible() {
		int counter = 0;
		for (int i = 0; i < size; i++) {
			if(population[i].status_string() == "susceptible") {
				counter++;
			}
		}
		return counter;
	}
	    void update() {
	    	days++;
	    	for(int i = 0; i < population.size(); i++) {
	    		population[i].update();
	    	}
	    
	    }
};

int main() {
	float INFECTION_PROBABILITY;
	int POPULATION_SIZE;
	int N_INTERACTIONS;
	int DISEASE_DURATION;
	float PCT_INOCULATED;
	int number_infected = 0;

	cout << "Infection Probability: ";
	cin >> INFECTION_PROBABILITY;

	cout << "Disease Duration: ";
	cin >> DISEASE_DURATION;

	cout << "Population Size: ";
	cin >> POPULATION_SIZE;

	cout << "Number of Interactions: ";
	cin >> N_INTERACTIONS;

	cout << "Inoculation Proportion: ";
	cin >> PCT_INOCULATED;

	Population population(POPULATION_SIZE);
	Contagion contagion(INFECTION_PROBABILITY, DISEASE_DURATION);
	
	population.random_inoculation(PCT_INOCULATED);
	population.random_infection(DISEASE_DURATION);
	
	number_infected = population.count_infected();
	
	int day_counter = 0;
	while(number_infected > 0) {
	    	cout << "Day " << population.getDays() << ": " << population.count_infected() << " infected." << endl;
		number_infected = population.count_infected();
		day_counter++;
		for(int i = 0; i < POPULATION_SIZE; i++) {
			if(population.getElement(i).status_string() == "infected" && population.getElement(i).getPersonDuration() != DISEASE_DURATION) {
				for(int j = 0; j < N_INTERACTIONS; j++) {
					int r1 = i;
					int r2 = rand() % POPULATION_SIZE;
					population.interact(r1, r2, contagion);
				}
			}
		}
		population.update();
		//for(int i = 0; i < POPULATION_SIZE; i++) {
		//	cout << population.getElement(i).status_string() << ", ";
		//}
	}
	
	cout << "Disease terminated after " << day_counter << " days." << endl;
	cout << "Number untouched: " << population.count_susceptible() << "." <<  endl;

}
