// 250a 5.5.cpp : Defines the entry point for the console application.
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
	ifstream train3("train3.txt"), train5("train5.txt"), test3("test3.txt"), test5("test5.txt");
	ofstream outfile;
	outfile.open("out.txt");

	double learningRate = 0.2 / 1400;

	vector<vector<int>> trainingData;
	vector<vector<int>> testData;
	arma::mat matrixA(64, 64);
	arma::mat invMatrixA;
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			matrixA(i, j) = 0;
		}
	}
	double vectorB[64] = { 0 };
	double vectorW[64] = { 0 };

	if (train3.is_open() && train5.is_open() && test3.is_open() && test5.is_open())
	{
		// Read in training data for both sets of data
		while (getline(train3, line))
		{
			vector<int> imageVector;
			istringstream iss(line);
			string bit;
			while (getline(iss, bit, ' '))
			{
				imageVector.push_back(stoi(bit));
			}
			trainingData.push_back(imageVector);
		}
		while (getline(train5, line))
		{
			vector<int> imageVector;
			istringstream iss(line);
			string bit;
			while (getline(iss, bit, ' '))
			{
				imageVector.push_back(stoi(bit));
			}
			trainingData.push_back(imageVector);
		}

		// Calculate the matrix A
		for (size_t t = 0; t < trainingData.size(); t++)
		{
			for (int i = 0; i < 64; i++)
			{
				for (int j = 0; j < 64; j++)
				{
					matrixA(i, j) += (trainingData.at(t).at(i)*trainingData.at(t).at(j));
				}
			}
		}

		// Find the inverse of matrix A.
		invMatrixA = arma::inv(matrixA);

		// Then multiply the inverse of the matrix A by the vector b
		// which equals the sum of all test vectors multiplied by their outcomes
		// This gives us our weight vector to calculate the log likelihood.

		for (int t = 700; t < trainingData.size(); t++)
		{
			// We use the label y = 0 to represent 3 and y = 1 to represent 5
			// so we only have to sum up the sample vectors for 5 samples to get
			// the B vector.
			for (int i = 0; i < 64; i++)
			{
				vectorB[i] += trainingData.at(t).at(i);
			}
		}

		// Calculate the starting weight vector

		for (int i = 0; i < 64; i++)
		{
			for (int j = 0; j < 64; j++)
			{
				vectorW[i] += (invMatrixA(i, j) * vectorB[j]);
			}
			cout << "W: " << vectorW[i] << "\n";
		}

		// Use Gradient Ascent to update the weights
		int iter = 0;
		for (iter = 0; iter < 5000; iter++)
		{
			double gradientVect[64] = { 0 };
			for (int t = 0; t < trainingData.size(); t++)
			{
				int y = 0;
				if (t >= 700)
				{
					y = 1;
				}
				double vecWdotvecXt = 0;
				for (int a = 0; a < 64; a++)
				{
					vecWdotvecXt += (vectorW[a] * trainingData.at(t).at(a));
				}
				double sig = 1 / (1 + exp(-1 * vecWdotvecXt));
				for (int a = 0; a < 64; a++)
				{
					gradientVect[a] += ((y - sig) * trainingData.at(t).at(a));
				}
			}
			for (int a = 0; a < 64; a++)
			{
				vectorW[a] += (learningRate * gradientVect[a]);
			}
			double logLikelihood = 0;
			for (int t = 0; t < trainingData.size(); t++)
			{
				double vecWdotvecXt = 0;
				for (int a = 0; a < 64; a++)
				{
					vecWdotvecXt += (vectorW[a] * trainingData.at(t).at(a));
				}
				if (t < 700)// y = 0, so take sig of negative dot product
				{
					vecWdotvecXt = -1 * vecWdotvecXt;
				}

				double sig = 1 / (1 + exp(-1 * vecWdotvecXt));
				logLikelihood += log(sig);
			}
			outfile << "Trial " << (iter+1) << " Log Likelihood: " << logLikelihood << "\n";
		}

		outfile << "Trained Weight Matrix:\n";
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				outfile << vectorW[(i*8) + j] << " ";
			}
			outfile << "\n";
		}

		double incorrectPredictions = 0;
		for (int t = 0; t < trainingData.size(); t++)
		{
			double vecWdotvecXt = 0;
			for (int a = 0; a < 64; a++)
			{
				vecWdotvecXt += (vectorW[a] * trainingData.at(t).at(a));
			}
			double probYIsClass5 = 1 / (1 + exp(-1 * vecWdotvecXt));

			if ((probYIsClass5 > 0.5 && t < 700) || (probYIsClass5 < 0.5 && t >= 700))
			{
				incorrectPredictions++;
			}
		}
		outfile << "Training Errors: " << incorrectPredictions <<
			" Error Rate: " << (incorrectPredictions / 1400) << "\n";

		// Read in test data for both sets of data
		while (getline(test3, line))
		{
			vector<int> imageVector;
			istringstream iss(line);
			string bit;
			while (getline(iss, bit, ' '))
			{
				imageVector.push_back(stoi(bit));
			}
			testData.push_back(imageVector);
		}
		while (getline(test5, line))
		{
			vector<int> imageVector;
			istringstream iss(line);
			string bit;
			while (getline(iss, bit, ' '))
			{
				imageVector.push_back(stoi(bit));
			}
			testData.push_back(imageVector);
		}

		// For every image in our test data see if the function gives a 
		// probability >50% for the correct class.
		incorrectPredictions = 0;
		for (int t = 0; t < testData.size(); t++)
		{
			double vecWdotvecXt = 0;
			for (int a = 0; a < 64; a++)
			{
				vecWdotvecXt += (vectorW[a] * testData.at(t).at(a));
			}

			double probYIsClass5 = 1 / (1 + exp(-1 * vecWdotvecXt));
			if ((probYIsClass5 > 0.5 && t < 400) || (probYIsClass5 < 0.5 && t >= 400))
			{
				incorrectPredictions++;
			}
		}
		outfile << "Test Errors: " << incorrectPredictions <<
			" Error Rate: " << (incorrectPredictions / 800) << "\n";
	}
	else
	{
		cout << "Failed to open files.\n";
	}
	outfile.close();
	system("pause");
	return 0;
}