#include "stdafx.h"
#include <iostream>
#include <math.h> 
#include <string>
#include <fstream> 
#include <string>
#include <vector>
#include <sstream>  
using namespace std;

long double bucketCentroid [32][12]; // Stores the centroid of every cell (bucket)
long double universe[6340][12];  // stores the universe in this array
long double tokhura_weights[12] = {1.0, 3.0, 7.0, 13.0, 19.0, 22.0, 25.0, 33.0, 42.0, 50.0, 56.0, 61.0}; // Stores the tokura weights
long double epsilon = 0.03; // Splits each codebook vector into 2
int indexTrack[32];// stores the size of every cell

// This method loads the Universe from excel sheet to universe array
void loadUniverse(){

	ifstream fin;
	fin.open("../Universe.csv");

	if (!fin.is_open()) {
		cout << "ERROR!! NO FILE HAS BEEN OPENED" << endl;
		exit(0);
	}

	string line; // Used to read the entire one row of excel sheet
	long double entry; // Stores each block of excel sheet
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

// This method runs the first step of LBG which is to find the centroid of universe and store it into codebook
void LBGFirstStep(){
	
	ofstream myfile;
	myfile.open ("../Codebook.txt");

	if (!myfile.is_open()) {
		cout << "ERROR!! NO FILE HAS BEEN OPENED" << endl;
		exit(0);
	}
	// Calculating centroid
	vector <long double> centroid(12);
	for (int i=0; i<6340; i++){
		for (int j=0; j<12; j++){
			centroid[j] += universe[i][j];
		}
	}
	// storing centroid to codebbok
	for (int i=0; i<12; i++){
		centroid[i] /= 6340;
		myfile << centroid[i] << " ";
	}

	myfile.close();
	long double Currentsum = 0;
	
	// Calculating average distortion
	for (int p=0; p<6340; p++){

		long double distance = 0;

		for (int r=0; r<12; r++){
			distance += (pow (universe[p][r] - centroid[r], 2))*tokhura_weights[r];
		}
	
		Currentsum += distance;
		
	}
	cout << "Checking for k = 1"<< " Splits" << endl;

	cout << "Iteration Number = 1"<< " Average Distortion Value = " << Currentsum<< endl;
	cout << "*****************************************************************************" << endl;

	return;
}

// Empty cell resolution method
void emptyCellResolution(int size){

	int maxIndex = -1;
	long long maxBucket = 0;

	// Finding the centroid with maximum cell size
	// indexTrack tracks the size of each cell
	for (int i=0; i<size; i++){
		if (indexTrack[i] > maxBucket){
			maxBucket = indexTrack[i];
			maxIndex = i;
		}
	}

	for (int j=0; j<size; j++){
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
	
	loadUniverse(); // loading universe
	LBGFirstStep();	 // run first step of LBG
	
	// Running LBG for rest of the steps
	for (int i=2; i<=8; i=i*2){

		vector<vector<long double>> currentCodebook;  // Loads the current codebook vectors to this array
		long double Currentsum = 0; // Used to calculate current average distortion
		long double Previoussum = 0; // Keeps track of iteration in previous iteration


		ifstream fin;
		fin.open("../Codebook.txt");

		if (!fin.is_open()) {
			cout << "ERROR!! NO FILE HAS BEEN OPENED" << endl;
			exit(0);
		}

		// Spliting each codevector into 2, so that the size of codebook becomes twice
		for (int j=0; j<i; j++){

			vector <long double> temp(12);
			
			for (int k=0; k<12; k++){
				fin >> temp[k];
			}
		
			vector <long double> temp1(12);
			vector <long double> temp2(12);

			// Splitting
			for (int k=0; k<12; k++){
				temp1[k] = temp[k]* (1 - epsilon);
				temp2[k] = temp[k] * (1 + epsilon);
			}

			// Loading into currentCodebook vector so that it is further stored in code book file
			currentCodebook.push_back(temp1);
			currentCodebook.push_back(temp2);
		}

		// Storing the splitted codevectors into code book file
		ofstream myfile;
		myfile.open("../Codebook.txt");

		if (!myfile.is_open()) {
			cout << "ERROR!! NO FILE HAS BEEN OPENED" << endl;
			return 0;
		}

		for (int p=0; p<i; p++){
			for (int q=0; q<12; q++){
				myfile << currentCodebook[p][q] << " ";
			}
			myfile << endl;
		}
		myfile.close();

		// Calculating average distortion for the first step in Kmeans
		for (int p=0; p<6340; p++){

			long double Mindistance = DBL_MAX;

			for (int q=0; q<i; q++){

				long double distance = 0;

				for (int r=0; r<12; r++){
					distance += (pow (universe[p][r] - currentCodebook[q][r], 2))*tokhura_weights[r];
				}
				if (Mindistance > distance){
					Mindistance = distance;
				}
			}
			Currentsum += Mindistance;

		}
		Previoussum = Currentsum;
		Currentsum = 0;
	
		long long iteration = 0;
		bool stop = false;
		cout << "Checking for k = " << i << " Splits" << endl;

		// Iterating over Kmeans
		while(!stop){
			
			vector<vector<long double>> bucket[32];  // Stores the vectors corresponding to every cell, maximum size is 32
			iteration ++;
			
			// Loading the codebook
			ifstream fin;
			fin.open("../Codebook.txt");

			if (!fin.is_open()) {
				cout << "ERROR!! NO FILE HAS BEEN OPENED" << endl;
				return 0;
			}

			for (int p=0; p<i; p++){
				for (int q=0; q<12; q++){
					fin >> currentCodebook[p][q];
				}
				
			}
			fin.close();
		
			memset(indexTrack, 0 , sizeof indexTrack);
			Currentsum = 0;
			
			// Calculating average distortion....Also finding Nearest neighbour
			for (int p=0; p<6340; p++){

				long long int index = -1;
				long double Mindistance = DBL_MAX;

				for (int q=0; q<i; q++){

					long double distance = 0;

					for (int r=0; r<12; r++){
						distance += (pow (universe[p][r] - currentCodebook[q][r], 2))*tokhura_weights[r];
					}
				
					if (Mindistance > distance){
						Mindistance = distance;
						index = q;
					}
				}
			
				Currentsum += Mindistance;

				// mapping the  training vector to appropriate cell / bucket
				vector <long double> temp;

				for (int r=0; r<12; r++){
					temp.push_back(universe[p][r]);
				}

				bucket[index].push_back(temp);			
				indexTrack[index]++;
			}
	
			Currentsum /= 6340;
			// Calculating the percent change in average distortion for termination condition
			long double percentChange = 100 * (Previoussum - Currentsum) / Previoussum;

			// If termination condition gets hit
			if (percentChange < (long double)0.00001){
				cout << endl;
				cout << "Threshold Reached !! " << endl;
				stop = true;
				break;
			}
		
			cout << "Iteration Number = " << iteration << " Average Distortion Value = " << Currentsum<< endl;

			Previoussum = Currentsum;
			bool emptyCell = false; // Keeps track of empty cell

			for (int p=0; p<i; p++){

				int bucketSize = bucket[p].size();
				long double centroid[12];
				memset (centroid, 0, sizeof centroid);	
				// If bucket size ==0 then empty cell which is handled below
				if (bucketSize == 0){
					emptyCell = true;
					continue;
				}

				// Updating the centroid of each bucket (cell)
				for (int q=0; q<bucketSize; q++){
					vector<long double> temp = bucket[p][q];

					for (int r=0; r<12; r++){
						centroid[r] += temp[r];
					}
				}
				
				// Finding centroid
				for (int r=0; r<12; r++){
					centroid[r] /= bucketSize;
				
				}
		
				// storing centroid of bucket (cell)
				for (int r=0; r<12; r++){
					bucketCentroid[p][r] = centroid[r];
				}
			}

			// If empty cell encountered
			if (emptyCell){
				emptyCellResolution(i);
			}
		
			// Storing the cenotrids in codebook.txt
			ofstream myfile;
			myfile.open ("../Codebook.txt");

			if (!myfile.is_open()) {
				cout << "ERROR!! NO FILE HAS BEEN OPENED" << endl;
				return 0;
			}

			for (int p=0; p<i; p++){
				for (int q=0; q<12; q++){
					myfile << bucketCentroid[p][q] << " ";
				}
				myfile << endl;
			}
			myfile.close();
	
		}
		cout << "*****************************************************************************" << endl;
	}
	system("pause");
	return 0;
}

