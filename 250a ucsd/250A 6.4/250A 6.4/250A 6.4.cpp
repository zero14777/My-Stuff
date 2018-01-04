// 250A 6.4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <math.h>

double Rfunct(double X, double Xn)
{
	double temp = 0;
	
	// g'(y)
	for (int k = 1; k < 11; k++)
	{
		temp = temp + (tanh(Xn + (2 / sqrt(k))) / 10);
	}

	temp *= (X - Xn);

	// g(y)
	for (int k = 1; k < 11; k++)
	{
		temp = temp + (log(cosh(Xn + (2 / sqrt(k)))) / 10);
	}

	// .5(x-y)^2
	temp = temp + (0.5 * (X - Xn) * (X - Xn));

	return temp;
}

int main()
{
	double Xn = -1;
	int updates = 25;

	for (int i = 0; i < updates; i++)
	{
		double temp = 0;
		for (int k = 1; k < 11; k++)
		{
			temp += ((tanh(Xn + (2 / sqrt(k)))) / 10);
		}
		Xn = Rfunct((Xn - temp), Xn);
	}

	std::cout << Xn << "\n";

	system("pause");

    return 0;
}

/*
double Qfunct(double X, double Xn)
{
double temp = log(cosh(Xn)) + (tanh(Xn) * (X - Xn)) + (0.5 * (X - Xn) * (X - Xn));
return temp;
}

int main()
{
double Xn = 3;
int updates = 4;

for (int i = 0; i < updates; i++)
{
Xn = Qfunct((Xn - tanh(Xn)), Xn);
}

std::cout << Xn << "\n";

system("pause");

return 0;
}
*/