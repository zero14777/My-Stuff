// 250a 9.4.cpp : Defines the entry point for the console application.
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
	ifstream Rewards("Rewards.txt"), ProbA1("ProbA1.txt"), ProbA2("ProbA2.txt"), ProbA3("ProbA3.txt"), ProbA4("ProbA4.txt");
	ofstream outfile;
	outfile.open("out.txt");
	double discountFactor = 0.9925;

	int Policy[81];
	for (int i = 0; i < 81; i++)
	{
		Policy[i] = 2;
	}
	int tempPolicy[81];
	double Reward[81];
	double stateValue[81] = {0};
	double tempStateValue[81];

	double transitionMatrix[4][81][81] = {0};

	if (Rewards.is_open() && ProbA1.is_open() && ProbA2.is_open() && ProbA3.is_open() && ProbA1.is_open())
	{
		for (int i = 0; i < 81; i++)
		{
			getline(Rewards, line);
			Reward[i] = stod(line);
		}

		while (getline(ProbA1, line))
		{
			istringstream iss(line);
			string val;
			int a, b;
			getline(iss, val, ' ');
			a = stoi(val);
			getline(iss, val, ' ');
			b = stoi(val);
			getline(iss, val, ' ');
			transitionMatrix[0][a-1][b-1] = stod(val);
		}

		while (getline(ProbA2, line))
		{
			istringstream iss(line);
			string val;
			int a, b;
			getline(iss, val, ' ');
			a = stoi(val);
			getline(iss, val, ' ');
			b = stoi(val);
			getline(iss, val, ' ');
			transitionMatrix[1][a - 1][b - 1] = stod(val);
		}

		while (getline(ProbA3, line))
		{
			istringstream iss(line);
			string val;
			int a, b;
			getline(iss, val, ' ');
			a = stoi(val);
			getline(iss, val, ' ');
			b = stoi(val);
			getline(iss, val, ' ');
			transitionMatrix[2][a - 1][b - 1] = stod(val);
		}

		while (getline(ProbA4, line))
		{
			istringstream iss(line);
			string val;
			int a, b;
			getline(iss, val, ' ');
			a = stoi(val);
			getline(iss, val, ' ');
			b = stoi(val);
			getline(iss, val, ' ');
			transitionMatrix[3][a - 1][b - 1] = stod(val);
		}

		// Policy Iteration

		while(true)
		{
			for (int i = 0; i < 81; i++)
			{
				double prevMax(0);
				int maxA = 1;
				for (int a = 1; a < 5; a++)
				{
					double temp = 0;
					for (int sPrime = 0; sPrime < 81; sPrime++)
					{
						temp += (stateValue[sPrime] * transitionMatrix[a - 1][i][sPrime]);
					}
					if (temp > prevMax || a == 1)
					{
						prevMax = temp;
						maxA = a;
					}
				}
				tempStateValue[i] = Reward[i] + (discountFactor * prevMax);
				tempPolicy[i] = maxA;
			}

			bool converged(true);
			for (int i = 0; i < 81; i++)
			{
				if (stateValue[i] != tempStateValue[i])
				{
					converged = false;
				}
			}

			for (int i = 0; i < 81; i++)
			{
				Policy[i] = tempPolicy[i];
				stateValue[i] = tempStateValue[i];
			}

			if (converged)
			{
				break;
			}
		}

		// Print

		outfile << "Policy Iteration\n\n";

		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				outfile << stateValue[(j*9) + i] << " ";
			}
			outfile << "\n";
		}
		outfile << "---\n";
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (stateValue[(j * 9) + i] == 0)
				{
					outfile << "X ";
				}
				else if (stateValue[(j * 9) + i] < 0)
				{
					outfile << "D ";
				}
				else
				{
					outfile << Policy[(j * 9) + i] << " ";
				}
			}
			outfile << "\n";
		}

		outfile << "\n\nValue Iteration\n\n";

		// Value Iteration

		while (true)
		{
			for (int i = 0; i < 81; i++)
			{
				double prevMax(0);
				for (int a = 1; a < 5; a++)
				{
					double temp = Reward[i];
					for (int sPrime = 0; sPrime < 81; sPrime++)
					{
						temp += (discountFactor * (stateValue[sPrime] * transitionMatrix[a - 1][i][sPrime]));
					}
					if (temp > prevMax || a == 1)
					{
						prevMax = temp;
					}
				}
				tempStateValue[i] = prevMax;
			}

			bool converged(true);
			for (int i = 0; i < 81; i++)
			{
				if (stateValue[i] != tempStateValue[i])
				{
					converged = false;
				}
			}

			for (int i = 0; i < 81; i++)
			{
				stateValue[i] = tempStateValue[i];
			}

			if (converged)
			{
				break;
			}
		}

		// Retrieve policy from OPT values

		for (int i = 0; i < 81; i++)
		{
			double prevMax(0);
			int maxA = 1;
			for (int a = 1; a < 5; a++)
			{
				double temp = 0;
				for (int sPrime = 0; sPrime < 81; sPrime++)
				{
					temp += (stateValue[sPrime] * transitionMatrix[a - 1][i][sPrime]);
				}
				if (temp > prevMax || a == 1)
				{
					prevMax = temp;
					maxA = a;
				}
			}
			Policy[i] = maxA;
		}

		// Print

		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				outfile << stateValue[(j * 9) + i] << " ";
			}
			outfile << "\n";
		}
		outfile << "---\n";
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (stateValue[(j * 9) + i] == 0)
				{
					outfile << "X ";
				}
				else if (stateValue[(j * 9) + i] < 0)
				{
					outfile << "D ";
				}
				else
				{
					outfile << Policy[(j * 9) + i] << " ";
				}
			}
			outfile << "\n";
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