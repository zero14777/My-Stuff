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
	ifstream Ratings("Ratings.txt"), Titles("Titles.txt"), ProbZinit("ProbZ-init.txt"), ProbRGivenZinit("ProbRGivenZ-init.txt");
	ofstream outfile;
	outfile.open("out.txt");

	int k = 4;
	int iterations = 64;
	int me = 279; //apparently I was too late

	vector<double> seen;
	vector<string> titleList;
	vector<pair<double, string>> popularity;
	vector<vector<int>> studentMovieResponses;
	arma::mat ProbZ(k, 1);
	arma::mat ProbRGivenZ(50, k);

	if (Ratings.is_open() && Titles.is_open() && ProbZinit.is_open() && ProbRGivenZinit.is_open())
	{
		getline(Ratings, line);
		istringstream iss(line);
		string val;
		vector<int> temp;

		while (getline(iss, val, ' '))
		{
			getline(Titles, line);
			titleList.push_back(line);
			if (val.compare("1") == 0)
			{
				popularity.push_back(std::make_pair(1.0, line));
				temp.push_back(1);
			}
			else
			{
				if (val.compare("0") == 0)
				{
					temp.push_back(0);
				}
				else
				{
					temp.push_back(-1);
				}
				popularity.push_back(std::make_pair(0.0, line));
			}

			if (val.compare("?") == 0)
			{
				seen.push_back(0.0);
			}
			else
			{
				seen.push_back(1.0);
			}
		}

		studentMovieResponses.push_back(temp);

		while (getline(Ratings, line))
		{
			istringstream iss(line);
			string val;
			vector<int> temp;
			for (int i = 0; i < popularity.size(); i++)
			{
				getline(iss, val, ' ');
				if (val.compare("1") == 0)
				{
					popularity.at(i).first += 1.0;
					temp.push_back(1);
				}
				else
				{
					if (val.compare("0") == 0)
					{
						temp.push_back(0);
					}
					else
					{
						temp.push_back(-1);
					}
					popularity.at(i).first += 0.0;
				}

				if (val.compare("?") == 0)
				{
					seen.at(i) += 0.0;
				}
				else
				{
					seen.at(i) += 1.0;
				}
			}
			studentMovieResponses.push_back(temp);
		}

		for (int i = 0; i < popularity.size(); i++)
		{
			popularity.at(i).first = popularity.at(i).first / seen.at(i);
		}

		sort(popularity.begin(), popularity.end());

		for (int i = 0; i < popularity.size(); i++)
		{
			outfile << popularity.at(i).first << " " << popularity.at(i).second << "\n";
		}

		for (int i = 0; i < k; i++)
		{
			getline(ProbZinit, line);
			ProbZ(i, 0) = stod(line);
		}

		for (int j = 0; j < 50; j++)
		{
			getline(ProbRGivenZinit, line);
			istringstream iss(line);
			string val;
			getline(iss, val, ' ');
			for (int i = 0; i < k; i++)
			{
				getline(iss, val, ' ');
				ProbRGivenZ(j, i) = stod(val);
			}
		}

		arma::mat postProb(k, studentMovieResponses.size());
		// update
		for (int iterCount = 0; iterCount < iterations; iterCount++)
		{
			// Calculate Post-Prob Matrix
			for (int i = 0; i < k; i++)
			{
				for (int t = 0; t < studentMovieResponses.size(); t++)
				{
					double denominator = 0;
					for (int iPrime = 0; iPrime < k; iPrime++)
					{
						double temp = ProbZ(iPrime, 0);
						for (int j = 0; j < 50; j++)
						{
							if (studentMovieResponses.at(t).at(j) == 1)
							{
								temp *= ProbRGivenZ(j, iPrime);
							}
							else if (studentMovieResponses.at(t).at(j) == 0)
							{
								temp *= (1 - ProbRGivenZ(j, iPrime));
							}
						}
						denominator += temp;
					}

					double numerator = ProbZ(i, 0);
					for (int j = 0; j < 50; j++)
					{
						if (studentMovieResponses.at(t).at(j) == 1)
						{
							numerator *= ProbRGivenZ(j, i);
						}
						else if (studentMovieResponses.at(t).at(j) == 0)
						{
							numerator *= (1 - ProbRGivenZ(j, i));
						}
					}

					postProb(i, t) = numerator / denominator;
				}
			}

			// M-Step
			arma::mat ProbZtemp(k, 1);

			for (int i = 0; i < k; i++)
			{
				ProbZtemp(i, 0) = 0;
				for (int t = 0; t < studentMovieResponses.size(); t++)
				{
					ProbZtemp(i, 0) += (postProb(i, t) / studentMovieResponses.size());
				}
			}

			arma::mat ProbRGivenZtemp(50, k);
			for (int i = 0; i < k; i++)
			{
				for (int j = 0; j < 50; j++)
				{
					// calc denominator
					double demoninator = 0;
					for (int t = 0; t < studentMovieResponses.size(); t++)
					{
						demoninator += postProb(i, t);
					}

					// calc numeratior
					double numerator = 0;
					for (int t = 0; t < studentMovieResponses.size(); t++)
					{
						if (studentMovieResponses.at(t).at(j) == 1)
						{
							numerator += postProb(i, t);
						}
						else if (studentMovieResponses.at(t).at(j) == -1)
						{
							numerator += (postProb(i, t) * ProbRGivenZ(j, i));
						}
					}

					ProbRGivenZtemp(j, i) = (numerator / demoninator);
				}
			}

			ProbZ = ProbZtemp;
			ProbRGivenZ = ProbRGivenZtemp;
		}

		// calculating log likelihood
		double logLikelihood = 0;
		for (int t = 0; t < studentMovieResponses.size(); t++)
		{
			double prob1Student = 0;
			for (int i = 0; i < k; i++)
			{
				double temp = ProbZ(i, 0);
				for (int j = 0; j < 50; j++)
				{
					if (studentMovieResponses.at(t).at(j) == 1)
					{
						temp *= ProbRGivenZ(j, i);
					}
					else if (studentMovieResponses.at(t).at(j) == 0)
					{
						temp *= (1 - ProbRGivenZ(j, i));
					}
				}
				prob1Student += temp;
			}
			logLikelihood += (log(prob1Student) / studentMovieResponses.size());
		}

		cout << logLikelihood;

		popularity.clear();

		for (int j = 0; j < 50; j++)
		{
			if (studentMovieResponses.at(me).at(j) == -1)
			{
				double temp = 0;
				for (int i = 0; i < k; i++)
				{
					temp += ((ProbRGivenZ(j, i) * postProb(i, me)));
				}
				popularity.push_back(std::make_pair(temp, titleList.at(j)));
			}
			else
			{
				popularity.push_back(std::make_pair(studentMovieResponses.at(me).at(j), titleList.at(j)));
			}
		}
		outfile << "--\n\n--\n";
		sort(popularity.begin(), popularity.end());
		for (int i = 0; i < popularity.size(); i++)
		{
			outfile << popularity.at(i).first << " " << popularity.at(i).second << "\n";
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