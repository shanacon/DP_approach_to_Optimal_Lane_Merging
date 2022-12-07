#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>
#include <fstream>
using namespace std;

default_random_engine generator;

int main()
{
	// CaseLen
	cout << "Please input case length: " << endl;
	int CaseLen;
	cin >> CaseLen;
	// CaseNum
	cout << "Please input case number: " << endl;
	int CaseNum;
	cin >> CaseNum;
	// lamba
	cout << "Please input lambda: " << endl;
	float lamba;
	cin >> lamba;
	poisson_distribution<int> distribution(lamba);
	// Do Random and write data
	ofstream TestData;
	TestData.open("TestData.txt");
	for (int i = 0; i < CaseNum; i++)
	{
		vector<int> Arrtime;
		for (int j = 0; j < CaseLen; j++)
		{
			int number = distribution(generator);
			Arrtime.push_back(number);
		}
		vector<int> WriteTime;
		WriteTime.push_back(-1);
		for (int j = 0; j < CaseLen; j++)
		{
			for (int k = 0;k < Arrtime[j];k++)
				WriteTime.push_back(j + 1);
		}
		for (int j = 0; j < WriteTime.size(); j++)
			TestData << WriteTime[j] << " ";
		TestData << endl;
	}
}