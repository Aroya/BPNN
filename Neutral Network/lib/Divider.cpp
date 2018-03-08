#include<fstream>
#include<iostream>
#include<string>
using namespace std;
#include"Divider.h"

void Divider(const char*filename) {
	ifstream fin;
	fin.open(filename);
	if (!fin.is_open()) {
		cout << "Cannot open " << filename << endl;
		return;
	}
	ofstream fout_train, fout_validation;
	fout_train.open("data/train.csv");
	fout_validation.open("data/validation.csv");
	if (!fout_train.is_open()) {
		cout << "Cannot open data/train.csv\n";
		return;
	}
	if (!fout_validation.is_open()) {
		cout << "Cannot open data/validation.csv\n";
		return;
	}
	string str;
	//first line
	if (fin >> str) {
		fout_validation << str << endl;
		fout_train << str << endl;
	}
	int counter = 0;
	while (fin >> str) {
		if (counter > 15920) {
			fout_validation << str << endl;
			//counter = 0;
		}
		else {
			fout_train << str << endl;
			counter++;
		}
	}
}