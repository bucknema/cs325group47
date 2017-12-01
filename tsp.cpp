/*
*Traveling Salesman Project - CS 325
*
*
* tsp.cpp
* compile with: g++ -std=c++11 tsp.cpp -o tsp
* run with: tsp
* the program will prompt you for an input file name at run time
*/


#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <math.h> 
#include <sstream>

using namespace std;

//Each city has an id, x-coordinate, and y-coordinate, and a visited value (0 for unvisited, 1 for visited)
struct city {

	int id;
	int x;
	int y;
	bool visited;

};

struct tour {
	std::vector<city> cities;
	int distance;
};

/*************************************************************************************/
/*HELPER FUNCTIONS*/
/*************************************************************************************/

//DESCRIPTION: This function prompts the user for a filename, and returns a string of the filename
/*
*inputs: user input
*outputs: string of the filename
*/
string getFilename()
{
	//string for the input file name
	string filename;

	/*Read-in file name from user from terminal*/
	cout << "What is the input file name?" << endl;
	cout << "Type in the exact file name and then press enter: " << endl;
	cin >> filename;

	return filename;
}

// DESCRIPTION: This function stores the cities from the input file line by line in "city" structs
//The cities are pushed back to a vector "cities" in the same order they are read from the file
/*
*inputs: input filestream, pointer to a vector of cities
*outputs: the vector of cities is filled with the city data from the input file
*/
void processInput(ifstream& input, vector<city>* cities) {

	int city_id;
	int x_coordinate;
	int y_coordinate;

	while (input >> city_id) {

		city newCity;

		input >> x_coordinate;
		input >> y_coordinate;

		newCity.id = city_id;
		newCity.x = x_coordinate;
		newCity.y = y_coordinate;
		newCity.visited = false;

		cities->push_back(newCity);
	}
	
	return;
}

//DESCRIPTION: This function outputs the files to the "...txt.tour" file
/*
*inputs: output stream, pointer to a vector of cities
*outputs: the tour is output to a file appended with ".tour" extension
*/
void processOutput(ofstream& output, int tourLength, vector<city>* cities) {

	output << tourLength << endl;
	for (city c : *cities)
		output << c.id << endl;
}


//DESCRIPTION: This function calculates the distance between two cities
/*
*inputs: two city structs
*outputs: the distance between the two cities, rounded to the nearest integer
*/
int distance(city c1, city c2) {

	int dist;
	double distance;

	//calculate distance between two points using distance formula
	distance = sqrt(pow((c2.x - c1.x), 2) + pow((c2.y - c1.y), 2));

	//round to nearest integer
	dist = round(distance);

	return dist;
}




/*************************************************************************************/
/*MAIN*/
/*************************************************************************************/
int main(int argc, char** argv) {

	/*Variables for filenames*/
	string input_file; //c++ string
	string output_file; //c++ string
	char const* inputFile; //c string
	char const* outputFile; //c string

	if (argc < 2) {
		cout << "Enter a filename as a command line arg" << endl;
		return(1);
	}

	input_file = argv[1];

	//generate output filename by appending ".tour"
	output_file = input_file + ".tour";

	//convert to c-string to prevent errors with fstream library functions
	inputFile = input_file.c_str();
	outputFile = output_file.c_str();

	//open input file stream object
	ifstream inputStream;
	inputStream.open(inputFile);

	vector<city> cities;
	vector<city>* citiesPtr = &cities;

	//store cities line by line in a vector of city structs

	processInput(inputStream, citiesPtr);

	/*Next: Build a graph using the cities vector by calculating the distances between all cities*/

	/*ALGORITHM IMPLEMENTATION OF TSP Nearest Neighbor algorithm*/	
	std::vector<tour*> tours;
	city start, curr;
	int n = cities.size();
	int total = 0; //total distance travelled
	int minDist, minIdx; //minimum distance to closest unvisited vertex, and index of closest unvisited vertex

	//start at first vertex in vector, record which is first city visited so we can return to it when finished
	for(int i = 0; i < n; i++){
		start = cities[i]; 
		curr = cities[i];
		cities[i].visited = true;
		//add current city to visited vector
		tours.push_back(new tour);
		tours[i]->cities.push_back(curr);

		//iterate through vector, where n is the number of cities in the vector
		for (int j = 1; j < n; j++) {
			//find closest city
			minDist = 100000000; //this is intended to be higher than any potential distance between cities
			for (int k = 0; k < n; k++) {
				//only checks unvisited cities
				if (!cities[k].visited) {
					int d = distance(curr, cities[k]);
					if (d < minDist) {
						minDist = d;
						minIdx = k;
					}
				}
			}
			//set current city to the closest one
			cities[minIdx].visited = true;
			curr = cities[minIdx];
			//add current to visited vector
			tours[i]->cities.push_back(curr);
			//add distance to total
			total += minDist; 
				
		}
		total += distance(curr, start);
		tours[i]->distance = total;
		total = 0;

		for(int j = 0; j < cities.size(); j++){
			cities[j].visited = false;
		}
}

	// checks to see which of the tours has the shortest distance
	tour* bestTour = tours[0];

	for(int i = 1; i < tours.size(); i++){
		if(tours[i]->distance < bestTour->distance){
			bestTour = tours[i];
		}
		
	}

	//open output file stream object
	ofstream outputStream;
	outputStream.open(outputFile);

	//write solution tour to "...txt.tour" file
	processOutput(outputStream, bestTour->distance, &(bestTour->cities));
	outputStream.close();

	//end of program
	return 0;
}
