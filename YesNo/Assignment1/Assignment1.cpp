#include "stdafx.h"
#include <iostream>
#include <fstream> 
#include <string>
#include <vector>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	ifstream fin;
	fin.open("../Input/abhik.txt");

	vector <long long int> energy; // stores energy
	vector <int> zcr; // stores zcr
	vector <pair <int, int>> word_boundary;  // Vector to store start/end index of every word.

	long long int amp; // stores amplitude
	int frame = 0; // counts each frame size 
	long long int en = 0; // en calculates energy of 320 samples at a time and stores in energy vector
	int count = 0; // This variable keeps track of sample size
	long long int prev_amp; // This stores the previous amplitude which helps in calculating zcr
	int z = 0; // z calculates zcr of 320 samples at a time and stores in zcr vector
	
	if (!fin) {
        cout << "File not available";
		return 0;
    }

	fin >> amp;
	count = 1;
	prev_amp = amp;
	en += amp*amp;

	for (;fin >> amp;) {

		// This checks if frame size is greater than 320. If yes then it stores energy and zcr
		if (count == 320){
			energy.push_back(en/320);
			zcr.push_back(z);
			en = 0;
			count = 0;
			z = 0;
		}

		count += 1;

		// calculating the energy 
		en += amp*amp;
		
		// calculating the zcr by multplying previous amplitude with current amplitude. If it is
		// negative then increment zcr by 1.
		if (prev_amp * amp < 0)
			z++;

		// Storing the current amplitude to prev_amp while will help in calculating zcr in next 
		// iteration
		prev_amp = amp;
	}

    fin.close();

	int start = -1; // This keeps track of start of word boundary.
	int end = -1; // This keeps track of end of word boundary.
	
	for (int i=1; i<energy.size(); i++){
		// If current energy is greater than 50000 then we can safely say that a word is getting spoken
		if (start == -1 && energy[i] >= 50000)
			start = i;
		
		// if current enery if less than 20000 then we can safely say that word has been completed
		else if (end == -1 && energy[i] < 20000 && start != -1)
			end = i;
		
		// If both start and end is not -1 that means we have detected a word then we can store them in 
		// word_boundary vector and reinitialize start and end to -1 to again start to search for next word
		if (start != -1 && end != -1){
			word_boundary.push_back(make_pair(start, end));
			start = -1;
			end = -1;
		}
	}

	// This print statement is for printing the number of words.
	cout << "Total " << word_boundary.size() << " " << "words are spoken" << endl;


	// This piece of code is for analysing ZCR for detecting yes/no. If ZCR of the last half of waveform
	// is greater than 120 then we can safely say that the word ends with fricative that is yes. Else no 
	for (int i=0; i<word_boundary.size(); i++){
		
		int begin = word_boundary[i].first; // Begin index of the word
		int end = word_boundary[i].second; // End index of the word
		int mid = begin + (end - begin)/2; // Mid of the word to analyse the second half of waveform of each word
		bool yes = false;

		for (int j=mid; j<=end; j++){

			// If ZCR is greater than 120 then we can safely say it is yes.
			if (zcr[j] > 120){
				yes = true; // make the yes flag true
				cout << "YES" << endl;
				break;
			}
		}
		// If yes flag is false then we can safely say it is no
		if (!yes){
			cout << "NO" << endl;
		}
	}
	system("pause");
	return 0;
}

