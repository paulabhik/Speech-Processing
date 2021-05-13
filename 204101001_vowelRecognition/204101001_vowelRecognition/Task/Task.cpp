#include "stdafx.h"
#include <iostream>
#include <fstream> 
#include <string>
#include <vector>

using namespace std;

vector <string> location_train [5]; // Used for storing location of training files
vector <string> location_test; // Used for storing location of testing file
long double dc_shift = 0; // Stores the dc shift

// Matrix for storing the cepstral coefficients of vowels which will later be dumped into  file
long double Cepstral_per_vowel[25][13];


// This method stores the location of all training file location in location_train array
void loadTrainingDataLocaion(){

	// For vowel 'A'
	location_train[0].push_back("../Input/Train/204101001_a_1.txt");
	location_train[0].push_back("../Input/Train/204101001_a_2.txt");
	location_train[0].push_back("../Input/Train/204101001_a_3.txt");
	location_train[0].push_back("../Input/Train/204101001_a_4.txt");
	location_train[0].push_back("../Input/Train/204101001_a_5.txt");
	location_train[0].push_back("../Input/Train/204101001_a_6.txt");
	location_train[0].push_back("../Input/Train/204101001_a_7.txt");
	location_train[0].push_back("../Input/Train/204101001_a_8.txt");
	location_train[0].push_back("../Input/Train/204101001_a_9.txt");
	location_train[0].push_back("../Input/Train/204101001_a_10.txt");


	// For vowel 'E'
	location_train[1].push_back("../Input/Train/204101001_e_1.txt");
	location_train[1].push_back("../Input/Train/204101001_e_2.txt");
	location_train[1].push_back("../Input/Train/204101001_e_3.txt");
	location_train[1].push_back("../Input/Train/204101001_e_4.txt");
	location_train[1].push_back("../Input/Train/204101001_e_5.txt");
	location_train[1].push_back("../Input/Train/204101001_e_6.txt");
	location_train[1].push_back("../Input/Train/204101001_e_7.txt");
	location_train[1].push_back("../Input/Train/204101001_e_8.txt");
	location_train[1].push_back("../Input/Train/204101001_e_9.txt");
	location_train[1].push_back("../Input/Train/204101001_e_10.txt");

	// For vowel 'I'
	location_train[2].push_back("../Input/Train/204101001_i_1.txt");
	location_train[2].push_back("../Input/Train/204101001_i_2.txt");
	location_train[2].push_back("../Input/Train/204101001_i_3.txt");
	location_train[2].push_back("../Input/Train/204101001_i_4.txt");
	location_train[2].push_back("../Input/Train/204101001_i_5.txt");
	location_train[2].push_back("../Input/Train/204101001_i_6.txt");
	location_train[2].push_back("../Input/Train/204101001_i_7.txt");
	location_train[2].push_back("../Input/Train/204101001_i_8.txt");
	location_train[2].push_back("../Input/Train/204101001_i_9.txt");
	location_train[2].push_back("../Input/Train/204101001_i_10.txt");

	// For Voewl 'O'
	location_train[3].push_back("../Input/Train/204101001_o_1.txt");
	location_train[3].push_back("../Input/Train/204101001_o_2.txt");
	location_train[3].push_back("../Input/Train/204101001_o_3.txt");
	location_train[3].push_back("../Input/Train/204101001_o_4.txt");
	location_train[3].push_back("../Input/Train/204101001_o_5.txt");
	location_train[3].push_back("../Input/Train/204101001_o_6.txt");
	location_train[3].push_back("../Input/Train/204101001_o_7.txt");
	location_train[3].push_back("../Input/Train/204101001_o_8.txt");
	location_train[3].push_back("../Input/Train/204101001_o_9.txt");
	location_train[3].push_back("../Input/Train/204101001_o_10.txt");

	// For vowel 'U'
	location_train[4].push_back("../Input/Train/204101001_u_1.txt");
	location_train[4].push_back("../Input/Train/204101001_u_2.txt");
	location_train[4].push_back("../Input/Train/204101001_u_3.txt");
	location_train[4].push_back("../Input/Train/204101001_u_4.txt");
	location_train[4].push_back("../Input/Train/204101001_u_5.txt");
	location_train[4].push_back("../Input/Train/204101001_u_6.txt");
	location_train[4].push_back("../Input/Train/204101001_u_7.txt");
	location_train[4].push_back("../Input/Train/204101001_u_8.txt");
	location_train[4].push_back("../Input/Train/204101001_u_9.txt");
	location_train[4].push_back("../Input/Train/204101001_u_10.txt");

	
}


// This calculates the dc shift
void dcShiftCalculation(){

	ifstream fin;
	fin.open("../silence.txt");
	long long int no_of_samples = 0;
	long long int amp;
	if (!fin) {
        cout << "File not available";
		return;
    }
	for (;fin >> amp;) {
		no_of_samples ++;
		dc_shift += amp;
	}

    fin.close();
	dc_shift = dc_shift/no_of_samples;
	cout << "DC Shift is " <<dc_shift << endl;
}


// This method finds the normatization factor
long double normalizationFactor(string loc){

	ifstream fin;
	long long int max_amp = LLONG_MIN;
	fin.open(loc);
	long long int amp;

	if (!fin) {
		cout << "File not available";
		return 0;
	}
	for (;fin >> amp;) {
		if (amp < 0)
			amp = -amp;

		amp = amp - dc_shift;
		max_amp = max(max_amp, amp);
	}

	return (long double) 10000/max_amp;
}

// This method is used for training the cepstral coefficients
void Train(){

	// Loads the location of all training data
	loadTrainingDataLocaion();

	int vowel_no = 0;  // Keeps track of current vowel number
	int no_of_vowels = 5;  // Total number of vowels

	memset(Cepstral_per_vowel, 0, sizeof Cepstral_per_vowel); // Initializing Cepstral Coefficient matrix to 0
	
	while (vowel_no < no_of_vowels){

		for (int i=0; i<10; i++){

			// Finding Normalization Factor and then Normalizing 
			long double factor = normalizationFactor(location_train[vowel_no][i]);
			ifstream fin;
			fin.open(location_train[vowel_no][i]);

			vector <long double> sample;
			long long int amp;
			long double sam;

			for (;fin >> amp;){
				amp = amp - dc_shift;
				sam = amp * factor;
				sample.push_back(sam);
			}

			fin.close();

			// Calculating the index of steady frane
			long long energy = 0;
			long long max_energy = 0;
			int count = 0;
			int index = -1;

			for (int i=0; i<sample.size(); i++) {

				if (count == 320){
					
					if (energy > max_energy){
						max_energy = energy;
						index = i;
					}
			
					energy = 0;
					count = 0;
				}

				count += 1;
				energy += sample[i]*sample[i];
		
			}

			int leftFrameNo = (index-320)/ 320 - 1; // Stores the left frame no

			// Checking whether the left frame no under analysis less than 0
			if (leftFrameNo < 0){
				leftFrameNo = 0;
			}

			int rightFrameNo = leftFrameNo + 5;

			// Checking whether the Right frame no under anlysis has crossed the max frame possible
			if (rightFrameNo > 16000/320){
				rightFrameNo = 16000/320;
				leftFrameNo = rightFrameNo - 5;
			}

			
			// Applying Hamming window

			for (int i=0; i<sample.size(); i++){
				sample[i] = (0.54 - 0.46*cos((2*3.14*(i%320))/319))* sample[i];
			}

	
			long long int frame_no = leftFrameNo;
			int idx = vowel_no * 5; // This is used to store the correct frame no in Cepstral matrix.
			
			// Iterating for all steady frames.

			while (frame_no < rightFrameNo){
				// Applying Durbins Algorithm

				// Calculate R
				vector <long double> R(13,0);
				int gap = 0;

				for (int gap=0; gap<13; gap++){
					for (int j=0; j<320-gap; j++){
						R[gap] = R[gap] + sample[j + frame_no*320] * sample[j + gap + frame_no*320];
					}
				}

				// Calculate A

				long double A[13][13];
				memset(A, 0, sizeof A); // Initializing to 0

				long double E[13];
				memset(E, 0, sizeof E); // Initializing to 0

				long double K[13];
				memset(K, 0, sizeof K); // Initializing to 0

				E[0] = R[0];
				K[1] = R[1]/R[0];
				A[1][1] = K[1];

				E[1] = (1 - K[1]*K[1])* E[0];

				for (int i=2; i<=12; i++){
	
					long double val = 0;
					for (int j=1; j<=i-1; j++){
						val += A[i-1][j]*R[i-j];
					}
					K[i] = (R[i] - val) / E[i-1];

					A[i][i] = K[i];

					for (int j=1; j<=i-1; j++){
						A[i][j] = A[i-1][j] - K[i]*A[i-1][i-j];
					}
					E[i] = (1 - K[i]*K[i])*E[i-1];
				}
				
				// Calculate C

				long double C[13];
				memset(C, 0, sizeof C);

				long double val;

				C[0] = log(R[0]*R[0]); 

				for (int i=1; i<=12; i++){

					val = 0;
					for (int j=1; j<=i-1; j++){
						val += (j * C[j] * A[12][i-j]);
						// Not inverting the sign of Ai after discussing with sir in class
					}
					C[i] = A[12][i] + val/i;
				}

				// First adding across all 5 frames and then averaging in later code
				for (int i=1; i<=12; i++){
					Cepstral_per_vowel[idx][i] += C[i];
				}

			frame_no ++;
			idx ++;

			}

		}

		
		// Averaging the cepstral coefficient across all 5 frames and also applying raised sine window
		for (int i=vowel_no * 5; i<=vowel_no * 5 + 4; i++){
			for (int j=0; j<=12; j++){
				Cepstral_per_vowel[i][j] = (Cepstral_per_vowel[i][j]/10) * (1+6*sin((3.14 * j)/12));
				
			}

		}

		vowel_no++;
	}

	// Dumping the cepstral coefficient in file
	ofstream myfile;
	myfile.open ("../Cepstral.txt");

	for (int i=0; i<25; i++){
		for (int j=1; j<=12; j++){
			myfile << Cepstral_per_vowel[i][j] << " ";
		}
		myfile << endl;
		if ((i+1)%5 == 0){
			myfile << endl;
		}
	}
	myfile.close();

}


void loadTestingDataLocaion(){

	location_test.push_back("../Input/Test/204101001_a_11.txt");
	location_test.push_back("../Input/Test/204101001_a_12.txt");
	location_test.push_back("../Input/Test/204101001_a_13.txt");
	location_test.push_back("../Input/Test/204101001_a_14.txt");
	location_test.push_back("../Input/Test/204101001_a_15.txt");
	location_test.push_back("../Input/Test/204101001_a_16.txt");
	location_test.push_back("../Input/Test/204101001_a_17.txt");
	location_test.push_back("../Input/Test/204101001_a_18.txt");
	location_test.push_back("../Input/Test/204101001_a_19.txt");
	location_test.push_back("../Input/Test/204101001_a_20.txt");

	location_test.push_back("../Input/Test/204101001_e_11.txt");
	location_test.push_back("../Input/Test/204101001_e_12.txt");
	location_test.push_back("../Input/Test/204101001_e_13.txt");
	location_test.push_back("../Input/Test/204101001_e_14.txt");
	location_test.push_back("../Input/Test/204101001_e_15.txt");
	location_test.push_back("../Input/Test/204101001_e_16.txt");
	location_test.push_back("../Input/Test/204101001_e_17.txt");
	location_test.push_back("../Input/Test/204101001_e_18.txt");
	location_test.push_back("../Input/Test/204101001_e_19.txt");
	location_test.push_back("../Input/Test/204101001_e_20.txt");


	location_test.push_back("../Input/Test/204101001_i_11.txt");
	location_test.push_back("../Input/Test/204101001_i_12.txt");
	location_test.push_back("../Input/Test/204101001_i_13.txt");
	location_test.push_back("../Input/Test/204101001_i_14.txt");
	location_test.push_back("../Input/Test/204101001_i_15.txt");
	location_test.push_back("../Input/Test/204101001_i_16.txt");
	location_test.push_back("../Input/Test/204101001_i_17.txt");
	location_test.push_back("../Input/Test/204101001_i_18.txt");
	location_test.push_back("../Input/Test/204101001_i_19.txt");
	location_test.push_back("../Input/Test/204101001_i_20.txt");


	location_test.push_back("../Input/Test/204101001_o_11.txt");
	location_test.push_back("../Input/Test/204101001_o_12.txt");
	location_test.push_back("../Input/Test/204101001_o_13.txt");
	location_test.push_back("../Input/Test/204101001_o_14.txt");
	location_test.push_back("../Input/Test/204101001_o_15.txt");
	location_test.push_back("../Input/Test/204101001_o_16.txt");
	location_test.push_back("../Input/Test/204101001_o_17.txt");
	location_test.push_back("../Input/Test/204101001_o_18.txt");
	location_test.push_back("../Input/Test/204101001_o_19.txt");
	location_test.push_back("../Input/Test/204101001_o_20.txt");


	location_test.push_back("../Input/Test/204101001_u_11.txt");
	location_test.push_back("../Input/Test/204101001_u_12.txt");
	location_test.push_back("../Input/Test/204101001_u_13.txt");
	location_test.push_back("../Input/Test/204101001_u_14.txt");
	location_test.push_back("../Input/Test/204101001_u_15.txt");
	location_test.push_back("../Input/Test/204101001_u_16.txt");
	location_test.push_back("../Input/Test/204101001_u_17.txt");
	location_test.push_back("../Input/Test/204101001_u_18.txt");
	location_test.push_back("../Input/Test/204101001_u_19.txt");
	location_test.push_back("../Input/Test/204101001_u_20.txt");
	

}


int _tmain(int argc, _TCHAR* argv[]){

	// Calculation of DC Shift 
	dcShiftCalculation();
	
	// For training
	Train();

	int choice;
	cout << "Press 1 for testing already recorded 50 files or Press 2 for live recording and hit enter" << endl;

	cin >> choice;
	
	// Testing

	cout << "Testing Time !!" << endl;

	// Storing cepstral coefficient from the file back into matrix to test
	long double Cep[25][13];
	ifstream myfil;
	myfil.open ("../Cepstral.txt");

	for (int i=0; i<25; i++){
		for (int j=1; j<=12; j++){
			myfil >> Cep[i][j];
		}
	}
	myfil.close();

	long double tokhura_weights[12] = {1.0, 3.0, 7.0, 13.0, 19.0, 22.0, 25.0, 33.0, 42.0, 50.0, 56.0, 61.0};

	if (choice == 1){
		loadTestingDataLocaion();
	}
	else{
		cout << "Please record within 2 seconds" << endl;
		system("Recording_Module.exe 2 input_file.wav yesnoinbuilt.txt");
		location_test.push_back("../Task/yesnoinbuilt.txt");
		
	}


	
	// Iterating for every file
	for (int i=0; i<location_test.size(); i++){

		long double Cepstral_coeff[5][13]; //Stores the cepstral coefficient of every file (vowel)
		memset(Cepstral_coeff, 0, sizeof Cepstral_coeff); // Initializing to zero
			
		// Finding Normalization Factor and then Normalizing 
		long double factor = normalizationFactor(location_test[i]);
		ifstream fin;
		fin.open(location_test[i]);

		vector <long double> sample;
		long long int amp;
		long double sam;

		for (;fin >> amp;){
			amp = amp - dc_shift;
			sam = amp * factor;
			sample.push_back(sam);
		}

		fin.close();

		// Calculating the index of steady frane
		long long energy = 0;
		long long max_energy = 0;
		int count = 0;
		int index = -1;

		for (int i=0; i<sample.size(); i++) {

			if (count == 320){
					
				if (energy > max_energy){
					max_energy = energy;
					index = i;
				}
			
				energy = 0;
				count = 0;
			}

			count += 1;
			energy += sample[i]*sample[i];
		
		}

		int leftFrameNo = (index-320)/ 320 - 1; // Stores the left frame no

		// Checking whether the left frame no under analysis less than 0
		if (leftFrameNo < 0){
			leftFrameNo = 0;
		}

		int rightFrameNo = leftFrameNo + 5;

		// Checking whether the Right frame no under anlysis has crossed the max frame possible
		if (rightFrameNo > 16000/320){
			rightFrameNo = 16000/320;
			leftFrameNo = rightFrameNo - 5;
		}

			
		// Applying Hamming window

		for (int i=0; i<sample.size(); i++){
			sample[i] = (0.54 - 0.46*cos((2*3.14*(i%320))/319))* sample[i];
		}

	
		long long int frame_no = leftFrameNo;
		int idx = 0; // This is used to store the correct frame no.
			
		// Iterating for all steady frames.

		while (frame_no < rightFrameNo){
			
			// Applying Durbins Algorithm

			// Calculate R
			vector <long double> R(13,0);
			int gap = 0;

			for (int gap=0; gap<13; gap++){
				for (int j=0; j<320-gap; j++){
					R[gap] = R[gap] + sample[j + frame_no*320] * sample[j + gap + frame_no*320];
				}
			}

			// Calculate A

			long double A[13][13];
			memset(A, 0, sizeof A); // Initializing to 0

			long double E[13];
			memset(E, 0, sizeof E); // Initializing to 0

			long double K[13];
			memset(K, 0, sizeof K); // Initializing to 0

			E[0] = R[0];
			K[1] = R[1]/R[0];
			A[1][1] = K[1];

			E[1] = (1 - K[1]*K[1])* E[0];

			for (int i=2; i<=12; i++){
	
				long double val = 0;
				for (int j=1; j<=i-1; j++){
					val += A[i-1][j]*R[i-j];
				}
				K[i] = (R[i] - val) / E[i-1];

				A[i][i] = K[i];

				for (int j=1; j<=i-1; j++){
					A[i][j] = A[i-1][j] - K[i]*A[i-1][i-j];
				}
				E[i] = (1 - K[i]*K[i])*E[i-1];
			}
				
			// Calculate C

			long double C[13];
			memset(C, 0, sizeof C);

			long double val;

			C[0] = log(R[0]*R[0]); 

			for (int i=1; i<=12; i++){

				val = 0;

				for (int j=1; j<=i-1; j++){
					val += (j * C[j] * A[12][i-j]);
					// Not inverting the sign of Ai after discussing with sir in class
				}

				C[i] = A[12][i] + val/i;
			}

			// Storing cepstral coefficient from every vowel (5 frames) and also applying raised sine window
			for (int i=1; i<=12; i++){
				Cepstral_coeff[idx][i] = C[i] * (1+6*sin((3.14 * i)/12));
			}
  
			frame_no ++;
			idx ++;

		}

		cout << endl;
			
		long double distance[13];
		long double dist = 0;
		long double min_distance = LLONG_MAX;
		char vowel;


		cout << "FOR FILE NO " << i + 1<< endl;

		// Checking distance from vowel 'A'
		memset(distance, 0, sizeof distance);
		dist=0;

		for (int i=0; i<5; i++){
			for (int j=1; j<=12; j++){
				distance[j] += pow(abs(Cepstral_coeff[i%5][j] -  Cepstral_per_vowel[i][j]),2);
			}
		}

		for (int i=1; i<=12; i++){
			distance[i] = distance[i]/5;
			dist += distance[i] * tokhura_weights[i-1]; // tokhura_weights index start from 0 hence i-1
		}

		if (dist < min_distance){
			vowel = 'a';
			min_distance = dist;
		}
		cout << "THE DISTANCE FROM VOWEL 'A' : " << dist << endl;


		// Checking distance from vowel 'E'
		memset(distance, 0, sizeof distance);
		dist=0;

		for (int i=5; i<10; i++){
			for (int j=1; j<=12; j++){
				distance[j] += pow(abs(Cepstral_coeff[i%5][j] -  Cepstral_per_vowel[i][j]),2);
			}
		}

		for (int i=1; i<=12; i++){
			distance[i] = distance[i]/5;
			dist += distance[i] * tokhura_weights[i-1];  // tokhura_weights index start from 0 hence i-1
		}

		if (dist < min_distance){
			vowel = 'e';
			min_distance = dist;
		}
		cout << "THE DISTANCE FROM VOWEL 'E' : " << dist << endl;


		// Checking distance from vowel 'I'
		memset(distance, 0, sizeof distance);
		dist=0;

		for (int i=10; i<15; i++){
			for (int j=1; j<=12; j++){
				distance[j] += pow(abs(Cepstral_coeff[i%5][j] -  Cepstral_per_vowel[i][j]),2);
			}
		}

		for (int i=1; i<=12; i++){
			distance[i] = distance[i]/5;
			dist += distance[i] * tokhura_weights[i-1];  // tokhura_weights index start from 0 hence i-1
		}

		if (dist < min_distance){
			vowel = 'i';
			min_distance = dist;
		}
		cout << "THE DISTANCE FROM VOWEL 'I' : " << dist << endl;


		// Checking distancr from vowel 'O'
		memset(distance, 0, sizeof distance);
		dist=0;

		for (int i=15; i<20; i++){
			for (int j=1; j<=12; j++){
				distance[j] += pow(abs(Cepstral_coeff[i%5][j] -  Cepstral_per_vowel[i][j]),2);
			}
		}

		for (int i=1; i<=12; i++){
			distance[i] = distance[i]/5;
			dist += distance[i] * tokhura_weights[i-1]; // tokhura_weights index start from 0 hence i-1
		}

		if (dist < min_distance){
			vowel = 'o';
			min_distance = dist;
		}
		cout << "THE DISTANCE FROM VOWEL 'O' : " << dist << endl;


		// Checking distance from vowel 'U'
		memset(distance, 0, sizeof distance);
		dist=0;
		for (int i=20; i<25; i++){
			for (int j=1; j<=12; j++){
				distance[j] += pow(abs(Cepstral_coeff[i%5][j] -  Cepstral_per_vowel[i][j]),2);
			}
		}

		for (int i=1; i<=12; i++){
			distance[i] = distance[i]/5;
			dist += distance[i] * tokhura_weights[i-1];  // tokhura_weights index start from 0 hence i-1
		}
			
		if (dist < min_distance){
			vowel = 'u';
			min_distance = dist;
		}

		cout << "THE DISTANCE FROM VOWEL 'U' : " << dist << endl;
		
		if (choice == 1){
			if (i>=0 && i<10)
				cout << "THE ACTUAL VOWEL IS : a"<< endl;

			else if (i>=10 && i<20)
				cout << "THE ACTUAL VOWEL IS : e"<< endl;

			else if (i>=20 && i<30)
				cout << "THE ACTUAL VOWEL IS : i"<< endl;

			else if (i>=30 && i<40)
				cout << "THE ACTUAL VOWEL IS : o"<< endl;

			else if (i>=40 && i<50)
				cout << "THE ACTUAL VOWEL IS : u"<< endl;
		}

		cout << "THE PREDICTED VOWEL IS : " << vowel << endl;
		cout << "#######################################################################" << endl;
			
	}
	
	

	system("pause");
	return 0;
}

