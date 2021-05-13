#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream> 
#include <string>
#include <vector>
#include <sstream>  
using namespace std;

long double universe[6340][12];  // stores the universe in this array
long double currentCodebook[8][12];  // stores the current codebook by reading it from codebook file
long double Currentsum = 0;  // Stores the current distortion
long double Previoussum = 0; // Stores the distortion of previous iteration
long double tokhura_weights[12] = {1.0, 3.0, 7.0, 13.0, 19.0, 22.0, 25.0, 33.0, 42.0, 50.0, 56.0, 61.0}; // Stores the tokura weights
long long int indexTrack[8]; // stores the size of every cell
long double centroid[12]; // used to calculate the centroid
long double bucketCentroid [8][12]; // Stores the centroid of every cell (bucket)
long double epsilon = 0.03; // This is used to split the centroid in case of empty cell problem

// This method loads the Universe from excel sheet to universe array
void loadUniverse(){

	ifstream fin;
	fin.open("../Universe.csv");

	if (!fin.is_open()) {
		cout << "ERROR!! NO FILE HAS BEEN OPENED" << endl;
		exit(0);
	}

	
	string line; // Used to read the entire one row of excel sheet
	long double entry;  // Stores each block of excel sheet
	long long trainingSample = 0; // Stores the training set number

	// Reading the universe and storing it to universe array
	while (!fin.eof()){
		getline(fin, line);
		stringstream s(line);
		int k = 0;
		while (s.good()) {
			string substr;
			getline(s, substr, ',');
			entry = atof(substr.c_str());
			universe[trainingSample][k] = entry;
			k++;
		}
		
		trainingSample++;
	}
	fin.close();
}

// This is the first iteration of K means

void KMeanFirstStep(){
	
	long long int randomNumbers[8];

	// Generating random vectors and using them at initial codebook vector
	for (int i=0; i<8; i++){
		randomNumbers[i] = rand() % 6339;
	}


	ofstream myfile;
	myfile.open ("../Codebook.txt");

	if (!myfile.is_open()) {
		cout << "ERROR!! NO FILE HAS BEEN OPENED" << endl;
		exit(0);
	}

	// Store random 8 vectors in codebook
	for (int i=0; i<8; i++){
		for (int j=0; j<12; j++){
			currentCodebook[i][j] = universe[randomNumbers[i]][j];
			myfile << currentCodebook[i][j] << " ";
		}
		myfile << endl;
	}

	myfile.close();

	// Calculating average distortion of initial codebook
	for (int i=0; i<6340; i++){

		long double Mindistance = DBL_MAX;

		for (int j=0; j<8; j++){

			long double distance = 0;

			for (int k=0; k<12; k++){
				distance += (pow (universe[i][k] - currentCodebook[j][k], 2))*tokhura_weights[k];
			}
			if (Mindistance > distance){
				Mindistance = distance;
			}
		}
		Currentsum += Mindistance;

	}
	Previoussum = Currentsum;
	Currentsum = 0;
	
	return;
}

// This is used to load the codebook to currentCodebook array for every iteration
void loadCurrentCodebook(){

	ifstream fin;
	fin.open("../Codebook.txt");

	if (!fin.is_open()) {
		cout << "ERROR!! NO FILE HAS BEEN OPENED" << endl;
		exit(0);
	}

	memset(currentCodebook, 0, sizeof currentCodebook);

	vector <long double> temp;

	for (int i=0; i<8; i++){
		for (int j=0; j<12; j++){
			fin >> currentCodebook[i][j];
		}		
	}
	fin.close();


}
// This resolutes the empty cell problem
void emptyCellResolution(){

	int maxIndex = -1;
	long long maxBucket = 0;

	// Finding the centroid with maximum cell size
	// indexTrack tracks the size of each cell
	for (int i=0; i<8; i++){
		if (indexTrack[i] > maxBucket){
			maxBucket = indexTrack[i];
			maxIndex = i;
		}
	}

	for (int j=0; j<8; j++){
		if (indexTrack[j] == 0){
			for (int i=0; i<12; i++){
				// Updating the centroid of empty cell
				bucketCentroid[j][i] = (1-epsilon) * bucketCentroid[maxIndex][i];
				bucketCentroid[maxIndex][i] = (1+epsilon) * bucketCentroid[maxIndex][i];
			}
		}
	}
}

int _tmain(int argc, _TCHAR* argv[]){
	
	loadUniverse(); // loading the universe
	KMeanFirstStep();	// First step of Kmeans
	bool stop = false;  // tracks the stopping criteria of k means
	long long int iteration = 0; // tracks the iteration number of k means

	while (!stop){
		iteration ++;
		vector<vector <long double>> bucket[8]; // Stores the vectors corresponding to every cell
		loadCurrentCodebook(); // loading current codebook to currentCodebook array

		memset(indexTrack, 0 , sizeof indexTrack); // Initializeing indexTrack to zero so that in every fresh new iteration it tracks the correct size of each cell
		Currentsum = 0;
		
		// Calculating distortion and deciding in which cell the each of universe vector maps to.... Also finds nearest neighbour
		for (int i=0; i<6340; i++){

			long long int index = -1;
			long double Mindistance = DBL_MAX;

			for (int j=0; j<8; j++){

				long double distance = 0;

				for (int k=0; k<12; k++){
					distance += (pow (universe[i][k] - currentCodebook[j][k], 2))*tokhura_weights[k];
				}
				// Keeps track of Nearest neighbour
				if (Mindistance > distance){
					Mindistance = distance;
					index = j;
				}
			}
			Currentsum += Mindistance;

			vector <long double> temp;

			// Update the bucket with the vector which maps to it
			for (int j=0; j<12; j++){
				temp.push_back(universe[i][j]);
			}

			bucket[index].push_back(temp);			
			indexTrack[index]++;
		}

		// Calculating change in distortion for the algorithm termination condition
		Currentsum /= 6340;
		double percentChange = 100 * (Previoussum - Currentsum) / Previoussum;

		if (percentChange < (long double)0.00001 ){
			cout << endl;
			cout << "Threshold Reached !!! " << endl;
			stop = true;
			break;
		}

	
		cout << "For Iteration :" << iteration << " Average Distortion Value = " << Currentsum << endl;

		Previoussum = Currentsum;

		bool emptyCell = false; // keeps track of empty cell problem

		for (int i=0; i<8; i++){
	
			memset (centroid, 0, sizeof centroid);
			int bucketSize = bucket[i].size();

			// it bucketSize == 0, it means empty cell encountered and hence emptyCell = true, this empty cell problem is handled below
			if (bucketSize == 0){
				emptyCell = true;
				continue;
			}

			// Updating the centroid of each bucket (cell)
			for (int j=0; j<bucketSize; j++){
				vector<long double> temp = bucket[i][j];

				for (int k=0; k<12; k++){
					centroid[k] += temp[k];
				}
			}
			// Finding centroid
			for (int k=0; k<12; k++){
				centroid[k] /= bucketSize;
			}
			// storing centroid of bucket (cell)
			for (int k=0; k<12; k++){
				bucketCentroid[i][k] = centroid[k];
			}
		}

		// If empty cell encountered
		if (emptyCell){
			emptyCellResolution();
		}
		
		// Storing the cenotrids in codebook.txt
		ofstream myfile;
		myfile.open ("../Codebook.txt");

		if (!myfile.is_open()) {
				cout << "ERROR!! NO FILE HAS BEEN OPENED" << endl;
				return 0;
		}


		for (int i=0; i<8; i++){
			for (int j=0; j<12; j++){
				myfile << bucketCentroid[i][j] << " ";
			}
			myfile << endl;
		}
		myfile.close();
	
	}
	system("pause");
	return 0;
}

