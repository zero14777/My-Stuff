// 250a 7.1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


// For matrix inversion I used the Armadillo library
// Citation below.
#include "armadillo"
// Conrad Sanderson and Ryan Curtin.
// Armadillo: a template - based C++ library for linear algebra.
// Journal of Open Source Software, Vol. 1, pp. 26, 2016.

using namespace std;

int main()
{
	string line;
	ifstream emissMatFile("emissMat.txt"), transMatFile("transMat.txt"), initStateDist("initStateDist.txt"), obsFile("Observations.txt");
	ofstream outfile;
	outfile.open("out.txt");

	int T = 240000;

	arma::mat emissMat(27, 2);
	arma::mat transMat(27, 27);
	arma::mat initialDist(27, 1);
	arma::mat observations(1, T);
	arma::mat lStar(27, T);
	arma::mat Phi(27, T);

	if (emissMatFile.is_open() && transMatFile.is_open() && initStateDist.is_open() && obsFile.is_open())
	{
		// Read in emisssion Matrix
		for (int i = 0; i < 27; i++)
		{
			getline(emissMatFile, line);
			istringstream iss(line);
			string val;
			getline(iss, val, '\t');
			emissMat(i, 0) = stod(val);
			getline(iss, val, '\t');
			emissMat(i, 1) = stod(val);
		}

		// Read in transmission Matrix
		for (int i = 0; i < 27; i++)
		{
			getline(transMatFile, line);
			istringstream iss(line);
			string val;
			for (int j = 0; j < 27; j++)
			{
				getline(iss, val, ' ');
				transMat(i, j) = stod(val);
			}
		}

		// Read in initial State Dist
		for (int i = 0; i < 27; i++)
		{
			getline(emissMatFile, line);
			initialDist(i, 0) = stod(line);
		}

		// Read in observations
		getline(obsFile, line);
		istringstream iss(line);
		string bit;
		for (int i = 0; i < T; i++)
		{
			getline(iss, bit, ' ');
			observations(0, i) = stoi(bit);
		}

		// lStar matrix base case
		for (int i = 0; i < 27; i++)
		{
			lStar(i, 0) = log(initialDist(i, 0)) + log(emissMat(i, observations(0, 0)));
		}
		
		for (int t = 1; t < T; t++)
		{
			for (int j = 0; j < 27; j++)
			{
				double maxVal = lStar(0, t - 1) + log(transMat(0, j));
				for (int k = 1; k < 27; k++)
				{
					maxVal = max(lStar(k, t - 1) + log(transMat(k, j)), maxVal);
				}
				lStar(j, t) = maxVal + log(emissMat(j, observations(0, t)));
			}
		}

		// Phi
		for (int t = 1; t < T; t++)
		{
			for (int j = 0; j < 27; j++)
			{
				double maxVal = lStar(0, t - 1) + log(transMat(0, j));
				double maxArg = 0;
				for (int k = 1; k < 27; k++)
				{
					double temp = lStar(k, t - 1) + log(transMat(k, j));
					if (temp > maxVal)
					{
						maxVal = temp;
						maxArg = k;
					}
				}
				Phi(j, t) = maxArg;
			}
		}

		int state = 0;
		double maxprob = lStar(0, T-1);
		for (int i = 1; i < 27; i++)
		{
			if (lStar(i, T-1) > maxprob)
			{
				maxprob = lStar(i, T-1);
				state = i;
			}
		}

		int count = 1;
		char temp;
		for (int t = (T-1); t > 0; t--)
		{
			int prevState = Phi(state, t);

			if (prevState == state)
			{
				count++;
				if (t == 1)
				{
					temp = 'a';
					if (state == 26)
					{
						temp = ' ';
					}
					else
					{
						temp = temp + state;
					}
					outfile << temp << "(" << count << ")\n";
				}
			}
			else
			{
				temp = 'a';
				if (state == 26)
				{
					temp = ' ';
				} else
				{
					temp = temp + state;
				}
				outfile << temp << "(" << count << ")\n";
				state = prevState;
				count = 1;
			}
		}
	}
	else
	{
		cout << "Failed to open files.\n";
	}
	outfile.close();
	system("pause");
	return 0;
}