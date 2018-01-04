// hw 250a 4.3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// basic file operations
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

int getWordPosition(string vocabArray[500], string word)
{
	for (int i = 0; i < 500; i++)
	{
		if (vocabArray[i].compare(word) == 0)
		{
			//cout << vocabArray[i] << " equals " << word << "\n";
			return i;
		}
	}
	return -1;
}

int main()
{
	string vocabLine, unigramLine, bigramLine;
	ifstream vocabFile("vocab.txt"), unigramFile("unigram.txt"), bigramFile("bigram.txt");
	static const int numWords = 500;
	double totalTokens = 0;
	double unigramArray[numWords];
	double uProbabilityArray[numWords];
	string vocabArray[numWords];
	double *bigramMatrix = new double[500 * 500];
	bigramMatrix[0] = { 0 };
	int THE;
	string testSentence1[10] = { "THE", "STOCK", "MARKET", "FELL", "BY", "ONE", "HUNDRED", "POINTS", "LAST", "WEEK"};
	string testSentence2[6] = { "THE", "SIXTEEN", "OFFICIALS", "SOLD", "FIRE", "INSURANCE"};
	if (vocabFile.is_open() && unigramFile.is_open() && bigramFile.is_open())
	{
		// A

		cout << "A\n";

		// Read in Unigram data and count the total number of tokens
		for (int i = 0; i < numWords; i++)
		{
			getline(unigramFile, unigramLine);
			unigramArray[i] = (double)stoi(unigramLine);
			totalTokens = totalTokens + unigramArray[i];
		}

		// Read in vocabulary and calculate the unigram probabilities
		for (int j = 0; j < numWords; j++)
		{
			getline(vocabFile, vocabLine);
			vocabArray[j] = vocabLine;
			uProbabilityArray[j] = (unigramArray[j] / totalTokens);

			// Print out words that start with M and their probabilities
			if (vocabLine.at(0) == 'm' || vocabLine.at(0) == 'M')
			{
				cout << vocabLine << setw(30-vocabLine.length()) << uProbabilityArray[j] << '\n';
			}
			// Find the position of the word THE
			if (vocabLine.compare("THE") == 0)
			{
				THE = j;
			}
		}
		
		// Read in the bigram data
		while (getline(bigramFile, bigramLine))
		{
			istringstream iss(bigramLine);
			string word, wordPrime, count;
			getline(iss, word, '\t');
			getline(iss, wordPrime, '\t');
			getline(iss, count, '\t');
			bigramMatrix[((stoi(word) - 1) * 500) + (stoi(wordPrime)-1)] = stoi(count);
		}

		// B

		cout << "\nB\n";

		// Print out the top 10 words that follow THE and their probabilities
		vector<int> top10words;
		int currMax;
		for (int k = 0; k < 10; k++)
		{
			currMax = -1;
			for (int p = 0; p < numWords; p++)
			{
				if ((currMax == -1 || bigramMatrix[(THE*500) + p] > bigramMatrix[(THE * 500) + currMax])
					&& std::find(top10words.begin(), top10words.end(), p) == top10words.end())
				{
					currMax = p;
				}
			}
			top10words.push_back(currMax);
			cout << vocabArray[currMax] << setw(30 - vocabArray[currMax].length()) << bigramMatrix[(THE * 500) + currMax]/ unigramArray[THE] << "\n";
		}

		//C
		
		cout << "\nC\n";

		double sentenceProb = 1.0;
		for (int a = 0; a < sizeof(testSentence1)/sizeof(testSentence1)[0]; a++)
		{
			int tempWord = getWordPosition(vocabArray, testSentence1[a]);
			sentenceProb += log(uProbabilityArray[tempWord]); // log rule : log (a*b) = log a + log b
			cout << "Prob of word" << testSentence1[a] << " is " << uProbabilityArray[tempWord] << "\n";

		}
		cout << "Log Unigram Probability = " << sentenceProb << "\n";

		sentenceProb = 1.0;
		for (int a = 0; a < sizeof(testSentence1) / sizeof(testSentence1)[0]; a++)
		{
			int tempWord = getWordPosition(vocabArray, testSentence1[a]);
			int tempWordPrev;
			if (a == 0)
			{
				tempWordPrev = 1; //<s>
			}
			else
			{
				tempWordPrev = getWordPosition(vocabArray, testSentence1[a-1]);
			}
			if (bigramMatrix[(tempWordPrev * 500) + tempWord] < 0)
			{//fixing wierd c++ behavior
				bigramMatrix[(tempWordPrev * 500) + tempWord] = 0;
			}
			sentenceProb += log(bigramMatrix[(tempWordPrev * 500) + tempWord] / unigramArray[tempWordPrev]); // log rule : log (a*b) = log a + log b

		}
		cout << "Log Bigram Probability = " << sentenceProb << "\n";

		//D

		cout << "\nD\n";

		sentenceProb = 1.0;
		for (int a = 0; a < sizeof(testSentence2) / sizeof(testSentence2)[0]; a++)
		{
			int tempWord = getWordPosition(vocabArray, testSentence2[a]);
			sentenceProb += log(uProbabilityArray[tempWord]); // log rule : log (a*b) = log a + log b
			cout << "Prob of word " << testSentence2[a] << " is " << uProbabilityArray[tempWord] << "\n";

		}
		cout << "Log Unigram Probability = " << sentenceProb << "\n";

		sentenceProb = 1.0;
		for (int a = 0; a < sizeof(testSentence2) / sizeof(testSentence2)[0]; a++)
		{
			int tempWord = getWordPosition(vocabArray, testSentence2[a]);
			int tempWordPrev;
			if (a == 0)
			{
				tempWordPrev = 1; //<s>
			}
			else
			{
				tempWordPrev = getWordPosition(vocabArray, testSentence2[a - 1]);
			}
			if (bigramMatrix[(tempWordPrev * 500) + tempWord] < 0)
			{//fixing wierd c++ behavior
				bigramMatrix[(tempWordPrev * 500) + tempWord] = 0;
			}
			cout << "Probability " << tempWord << " given " << tempWordPrev << " " << bigramMatrix[(tempWordPrev * 500) + tempWord] << "\n";
			sentenceProb += log(bigramMatrix[(tempWordPrev * 500) + tempWord] / unigramArray[tempWordPrev]); // log rule : log (a*b) = log a + log b
		}
		cout << "Log Bigram Probability = " << sentenceProb << "\n";

		// E
		cout << "\nE\n";

		double maxLogLikelihood = 0;
		double optimalLambda = 0.0;

		for (double y = 0; y <= 1; y = y + 0.01)
		{
			sentenceProb = 1.0;
			for (int a = 0; a < sizeof(testSentence2) / sizeof(testSentence2)[0]; a++)
			{
				int tempWord = getWordPosition(vocabArray, testSentence2[a]);
				int tempWordPrev;
				if (a == 0)
				{
					tempWordPrev = 1; //<s>
				}
				else
				{
					tempWordPrev = getWordPosition(vocabArray, testSentence2[a - 1]);
				}
				if (bigramMatrix[(tempWordPrev * 500) + tempWord] < 0)
				{//fixing wierd c++ behavior
					bigramMatrix[(tempWordPrev * 500) + tempWord] = 0;
				}
				sentenceProb += log(((1-y) * bigramMatrix[(tempWordPrev * 500) + tempWord] / unigramArray[tempWordPrev]) + (y * uProbabilityArray[tempWord])); // log rule : log (a*b) = log a + log b
				//cout << sentenceProb << "\n";
			}
			//cout << "Log Mixture Probability of Lambda " << y << " equals " << sentenceProb << "\n";
			if (y == 0 || sentenceProb > maxLogLikelihood)
			{
				maxLogLikelihood = sentenceProb;
				optimalLambda = y;
			}
		}
		cout << "optimalLambda equals " << optimalLambda << "\n";

		// Close files
		unigramFile.close();
		vocabFile.close();
		bigramFile.close();
	}
	else
	{
		cout << "Failed to open files.\n";
	}

	delete bigramMatrix;
	system("pause");

	return 0;
}