#pragma once
#include "pch.h"
#include <cstdlib>
#include <vector>
#include <ctime>
#include <string>

using namespace std;

double* convert(vector<double> right);
double** convert(vector<vector<double>> right);
vector<double> Convert(double* right, int size);
vector<vector<double>> Convert(double** right, int size);

extern "C" _declspec(dllexport) double HowLong(int size, int repeats);
extern "C" _declspec(dllexport) double* Answer(double* raw, double* col, double* right, int size);

class Matrix
{
private:
	vector<double> raw;
	vector<double> col;

public:

	Matrix(unsigned int);
	Matrix(vector<double>, vector<double>);
	Matrix(const Matrix& matrix) { this->col = matrix.col; this->raw = matrix.raw; }

	~Matrix() { raw.clear(); col.clear(); }

	Matrix& operator =(const Matrix& matrix) { this->col = matrix.col; this->raw = matrix.raw; }

	vector<double> Equation(vector<double>);
};