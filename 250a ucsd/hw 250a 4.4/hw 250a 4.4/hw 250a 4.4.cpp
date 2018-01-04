// hw 250a 4.4.cpp : Defines the entry point for the console application.
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
	ifstream nasdaq00("nasdaq00.txt"), nasdaq01("nasdaq01.txt");
	double matrixA[3][3] = { {0} };
	double inverseMatrixA[3][3] = { { 0 } };
	double vectorB[3] = { 0 };
	double vectorW[3] = { 0 };
	vector<double> trainingData;
	vector<double> testData;
	if (nasdaq00.is_open() && nasdaq01.is_open())
	{
		// Read in training data
		while (getline(nasdaq00, line))
		{
			trainingData.push_back(stod(line));
			//cout << stod(line) << "\n";
		}

		// Calculate the matrix A
		for (int i = 3; i < trainingData.size(); i++)
		{
			for (int j = -3; j < 0; j++)
			{
				matrixA[3 + j][0] += (trainingData.at(i + j)*trainingData.at(i - 3));
				matrixA[3 + j][1] += (trainingData.at(i + j)*trainingData.at(i - 2));
				matrixA[3 + j][2] += (trainingData.at(i + j)*trainingData.at(i - 1));
			}
		}

		// Calculate the inverse of matrix A
		for (int r = 0; r < 3; r++)
		{
			for (int c = 0; c < 3; c++)
			{
				cout << fixed << matrixA[r][c] << " ";
			}
			cout << "\n";
		}

		// Calculate the inverse of matrix A
		for (int r = 0; r < 3; r++)
		{
			for (int c = 0; c < 3; c++)
			{
				// Find the determinant for each minor
				vector<double> minorMatrix;
				for (int a = 0; a < 3; a++)
				{
					for (int b = 0; b < 3; b++)
					{
						if (a != r && b != c)
						{
							minorMatrix.push_back(matrixA[a][b]);
						}
					}
				}

				double detOfMinors = (minorMatrix.at(0)*minorMatrix.at(2)) - (minorMatrix.at(1)*minorMatrix.at(3));

				if ((r + c) % 2 == 1)
				{
					inverseMatrixA[r][c] = -1*detOfMinors;
				}
				else
				{
					inverseMatrixA[r][c] = detOfMinors;
				}
				cout << inverseMatrixA[r][c] << " ";
			}
			cout << "\n";
		}

		// Calculate the vector b
		for (int i = 3; i < trainingData.size(); i++)
		{
			vectorB[0] += (trainingData.at(i)*trainingData.at(i - 3));
			vectorB[1] += (trainingData.at(i)*trainingData.at(i - 2));
			vectorB[2] += (trainingData.at(i)*trainingData.at(i - 1));
		}

		// Weights equal the inverse of matrix A times vector b
		for (int i = 0; i < 3; i++)
		{
			vectorW[i] += vectorB[i] * inverseMatrixA[0][i];
			vectorW[i] += vectorB[i] * inverseMatrixA[1][i];
			vectorW[i] += vectorB[i] * inverseMatrixA[2][i];
			cout << "Weight[" << i << "] = " << vectorW[i] << "\n";
		}

		// Calculate mean squared error for training data
		double error = 0.0;
		for (int i = 3; i < trainingData.size(); i++)
		{
			error += pow((trainingData.at(i) - (trainingData.at(i - 3)*vectorW[0])
												  - (trainingData.at(i - 2)*vectorW[1])
											  	  - (trainingData.at(i - 1)*vectorW[2])), 2);
		}
		error = error / (trainingData.size() - 3);
		cout << "TRAINING DATA MEAN SQUARED ERROR: " << error << "\n";

		// Read in test data
		while (getline(nasdaq01, line))
		{
			testData.push_back(stod(line));
		}

		// Calculate mean squared error for test data (Starting from day 4 of 2001)
		error = 0.0;
		for (int i = 3; i < testData.size(); i++)
		{
			error += pow((testData.at(i) - (testData.at(i - 3)*vectorW[0])
										 - (testData.at(i - 2)*vectorW[1])
										 - (testData.at(i - 1)*vectorW[2])), 2);
		}
		error = error / (testData.size() - 3);
		cout << "TEST DATA MEAN SQUARED ERROR:     " << error << "\n";
	}
	else
	{
		cout << "Failed to open files.\n";
	}

	system("pause");

    return 0;
}

