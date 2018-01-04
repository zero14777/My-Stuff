// 250a 6.3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int main()
{
	string line;
	ifstream Xin("X.txt"), Yin("Y.txt");

	double vectorX[267][23];
	double vectorY[267];
	// Set starting Pi values
	double vectorPi[23];
	double tempVecPi[23];

	for (int i = 0; i < 23; i++)
	{
		vectorPi[i] = 0.05;
	}

	// Set iter count for this run
	int iterations = 256;

	if (Xin.is_open() && Yin.is_open())
	{
		// Read in X and Y data
		for (int t = 0; t < 267; t++)
		{
			getline(Xin, line);
			istringstream iss(line);
			string bit;
			int i = 0;
			while (getline(iss, bit, ' '))
			{
				vectorX[t][i] = stoi(bit);
				i++;
			}
			if (i != 23)
			{
				cout << "ERROR READING IN X\n";
			}
			getline(Yin, line);
			vectorY[t] = stoi(line);
		}

		// Update the specified number of times
		for (int iter = 0; iter < iterations; iter++)
		{
			// Update each of the parameters
			for (int i = 0; i < 23; i++)
			{
				double TiCount = 0;
				double newPi = 0;
				for (int t = 0; t < 267; t++)
				{
					if (vectorX[t][i] == 1)
					{
						TiCount++;
					}
					// Calculate P(Zi = 1, Xi = 1|X = x, Y = y)
					// Ignoring when Y = 0 or Xi = 0
					double Pit = 0;
					if (vectorY[t] == 1 && vectorX[t][i] == 1)
					{
						double temp = 1;
						for (int j = 0; j < 23; j++)
						{
							if (vectorX[t][j] == 1)
							{
								temp = temp * (1 - vectorPi[j]);
							}
						}
						Pit = vectorPi[i] / (1 - temp);
					}
					newPi = newPi + Pit;
				}
				// save the new Pi vector
				tempVecPi[i] = newPi / TiCount;
			}
			// Update the Pi vector
			for (int i = 0; i < 23; i++)
			{
				vectorPi[i] = tempVecPi[i];
			}
		}

		// Calculate Log Likelihood
		double logLikelihood = 0;
		int mistakes = 0;
		for (int t = 0; t < 267; t++)
		{
			double temp = 1;
			for (int j = 0; j < 23; j++)
			{
				if (vectorX[t][j] == 1)
				{
					temp = temp * (1 - vectorPi[j]);
				}
			}
			if (vectorY[t] == 1)
			{
				logLikelihood = logLikelihood + log(1 - temp);
				if ((1 - temp) < .5)
				{
					mistakes++;
				}
			}
			else
			{
				logLikelihood = logLikelihood + log(temp);
				if (temp < .5)
				{
					mistakes++;
				}
			}
		}
		logLikelihood = logLikelihood / 267;
		cout << "Log Likelihood: " << logLikelihood << "\n";
		cout << "Mistakes: " << mistakes << "\n";
	}
	else
	{
		cout << "Failed to open files.\n";
	}

	system("pause");
	return 0;
}