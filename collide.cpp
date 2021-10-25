/*Collide

Attempts to find SHA256 collisions and keeps the maximum sized partial match.
	-A partial match is difined as any byte length that is sequentially identical, starting with the lowest significant bytes.

Goal: To find highest reasonably attainable partial matches, given the limitations of the running hardware

This program explores the Difficulty of Bitcoin mining and other cryptocurrency mining that use similar algorithms.
"Difficulty" in Bitcoin mining directly correlates to a minimum legth of matching bytes to a Hashed block

Expected to run indefinitely.

Written By: Kyle Arango

*/

#include <stdio.h>
#include <iostream>
//#include <fstream> 
#include <set>
#include <time.h>
#include <stdlib.h>
//#include <conio.h>
#include <iomanip>
#include <fstream>
#include <openssl/sha.h>

#include "Data.h"

//#include <openssl/sha.h>

//Returns the number of matching tail bytes, given two 256-bit hashes
int NumMatches(const unsigned char* a, const unsigned char* b)
{
	int counter = 0;
	for (int i = 31; i >= 0; i--)
	{
		if (a[i] == b[i]) counter++;
		else break;
	}
	return counter;
}


//Prints a match, including input data and output hash
void PrintMatch(int bestMatch, const Data* match1, const Data* match2)
{
	if (!match1 || !match2)
	{
		std::cout << "No Match!!\n";
		return;
	}

	std::cout << "Matches: " << bestMatch << '\n';

	std::cout << "Data IN 1: ";
	for (int i = 0; i < 32; i++) std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)(match1->in[i]);
	std::cout << '\n';
	std::cout << "Data IN 2: ";
	for (int i = 0; i < 32; i++) std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)(match2->in[i]);
	std::cout << '\n';

	std::cout << '\n';

	std::cout << "Data OUT 1: ";
	for (int i = 0; i < 32; i++) std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)(match1->out[i]);
	std::cout << '\n';
	std::cout << "Data OUT 2: ";
	for (int i = 0; i < 32; i++) std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)(match2->out[i]);
	std::cout << '\n';
	std::cout << '\n';
}

void SaveHighestMatchingInputs(const Data* match1, const Data* match2)
{
	std::ofstream x("X.bin");
	std::ofstream y("Y.bin");
	if (x.is_open() && y.is_open())
	{
		//x << match1->in;
		//y << match2->in;
		x.write((char*)match1->in, 32);
		y.write((char*)match2->in, 32);
		x.flush();
		y.flush();
		x.close();
		y.close();
	}
	else  std::cout << "Failed to open file.\n";
}


int main()
{
	srand(time(NULL));



	//Set to hold hashed data (both input data & ouput hashes)
	std::set<Data> SHASet;

	//Randomized Initial Input
	Data SHAData;
	SHAData.Rand();

	//Tracks 
	int bestMatch = 0;
	const Data* match1 = NULL;
	const Data* match2 = NULL;

	const Data* tempD = NULL;

	int loopCount = 0;
	while (1)
	{
		int currentMatch = 0;

		//Randimized start for input data to increment
		SHAData.Rand();
		for (int i = 0; i < 262144; i++)//0x4000
		{
			SHAData.Hash();

			//test if match exists in set(unlikely)
			auto SetIter = SHASet.find(SHAData);
			if (SetIter != SHASet.end())
			{
				//If found a match already in set, check length of match and save
				currentMatch = NumMatches(SHAData.out, (*SetIter).out);
				//PrintMatch(0, &*SetIter, &SHAData);
				if (currentMatch > bestMatch)
				{
					match1 = tempD;
					match2 = &(*(SetIter));
					bestMatch = currentMatch;
					PrintMatch(bestMatch, match1, match2);
					SaveHighestMatchingInputs(match1, match2);
				}
			}

			//Insert Data in set and Check closest neighboors for partial matches
			auto var = SHASet.insert(Data(SHAData));//<iterator, bool> to the inserted spot

			tempD = &(*(var.first));//address of inserted SHAData location
			if (var.first != SHASet.begin())
			{
				//check left neighboor
				var.first--;
				currentMatch = NumMatches(tempD->out, (*(var.first)).out);
				if (currentMatch > bestMatch)
				{
					match1 = tempD;
					match2 = &(*(var.first));
					bestMatch = currentMatch;
					PrintMatch(bestMatch, match1, match2);
					SaveHighestMatchingInputs(match1, match2);
				}
				var.first++;
			}
			var.first++;
			if (var.first != SHASet.end())
			{
				//check right neighboor
				currentMatch = NumMatches(tempD->out, (*(var.first)).out);
				if (currentMatch > bestMatch)
				{
					match1 = tempD;
					match2 = &(*(var.first));
					bestMatch = currentMatch;
					PrintMatch(bestMatch, match1, match2);
					SaveHighestMatchingInputs(match1, match2);
				}
			}
			SHAData.Inc();


			loopCount++;
			//If set size is too large remove one entry  &  periodically print status
			if (SHASet.size() > 0x1a00000) //for laptop 0x1a00000, tested maximun set size for a given system before memory performance issues begin
			{
				auto rm = SHASet.begin();
				SHASet.erase(rm);
				if (loopCount % 0x400000 == 0)
				{
					std::cout << bestMatch << ' ';
					std::cout.flush();
				}
			}
			else if (loopCount % 0x40000 == 0)
			{
				std::cout << SHASet.size() << '\n';
			}
		}
	}


	//_getch();

	//f << "Hello World!\n";
	return 0;
}
