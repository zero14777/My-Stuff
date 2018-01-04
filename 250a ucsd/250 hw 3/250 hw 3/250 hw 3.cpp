// 250 hw 3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <time.h>

double ZProbability(double zval, std::vector<int> bits, double noise)
{
	double bitFunction = 0;
	for (int i = 0; i < 10; i++)
	{
		bitFunction += (pow(2, i) * bits.at(i));
	}
	return (((1-noise)/(1+noise)) * pow(noise, (abs(zval - bitFunction))));
}

int main()
{
	int iBit = 5;
	int sampleTotal = 10000;
	srand(time(NULL));
	double numerator = 0;
	double denominator = 0;
	for (int Sample = 0; Sample < sampleTotal; Sample++)
	{
		std::vector<int> bits;
		for (int i = 0; i < 10; i++)
		{
			double rando0to1 = (rand() / (double)RAND_MAX);
			if (rando0to1 > .5)
			{
				bits.push_back(1);
			}
			else
			{
				bits.push_back(0);
			}
		}
		double probabilityZgivenBits = ZProbability(128, bits, .1);
		denominator = denominator + probabilityZgivenBits;
		if (bits.at(iBit-1) == 1)
		{
			numerator = numerator + probabilityZgivenBits;
		}
	}
	std::cout << (numerator / denominator) << "\n";
	system("pause");
    return 0;
}

