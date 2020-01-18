#include "pch.h"
#include "Matrix.h"


double HowLong(int size, int repeats)
{

	if (size < 1 || repeats < 1)
	{
		throw exception("non-positive size or repeats");
	}

    Matrix A(size);
    vector<double> right;
    vector<double> answer;
    right.resize(size);

    for (size_t i = 0; i < size; i++)
    {
        right[i] = (double)((rand() % 20 - 10) / 3);
    }

    double start = clock(), end;

    for (int i = 0; i < repeats; i++)
    {
        answer = A.Equation(right);
    }

    end = clock();

	right.clear();
	answer.clear();
	A.~Matrix();

    return end - start;
}


double* Answer(double* raw, double* col, double* right, int size)
{
	if (size < 1)
	{
		throw exception("non-positive size");
	}

    Matrix A(Convert(raw,size), Convert(col,size));

    vector<double> answer = A.Equation(Convert(right,size));

	A.~Matrix();

    return convert(answer);
}

Matrix::Matrix(unsigned int size)
{
	raw.push_back((double)((rand() % 20 - 10) / 3));
	while (raw[0] == 0)
	{
		raw[0] = (double)((rand() % 20 - 10) / 3);
	}
	col.push_back(raw[0]);

	for (int i = 1; i < size; i++)
	{
		raw.push_back((double)((rand() % 20 - 10) / 3));
		col.push_back((double)((rand() % 20 - 10) / 3));
	}
}

Matrix::Matrix(vector<double> raw, vector<double> col)
{
	if (raw.size() == 0 || raw.size() != col.size())
	{
		throw exception("Invalid size of vectors.");
	}

	if (col[0] == 0 || raw[0] == 0 || col[0] != raw[0])
	{
		throw exception("Invalid value of first element");
	}

	this->col = col;
	this->raw = raw;
}



vector<double> Matrix::Equation(vector<double> right)
{
	vector<double> res;
	int size = right.size();
	vector<double>X, Y, Xbuff, Ybuff;

	double zero = 1 / col[0];

	double F, G, r, s, t;

	X.push_back(zero);
	Y.push_back(zero);

	int k = 1;

	while (k != size)
	{
		F = 0;
		G = 0;

		for (int i = 0; i < k; i++)
		{
			F += col[k - i] * X[i];
			G += raw[i + 1] * Y[i];
		}

		r = 1 / (1 - G * F);
		s = -r * F;
		t = -r * G;

		Xbuff.resize(k + 1);
		Ybuff.resize(k + 1);

		Xbuff[0] = X[0] * r;
		Ybuff[0] = X[0] * t;

		for (int i = 1; i < k; i++)
		{
			Xbuff[i] = X[i] * r + Y[i - 1] * s;
			Ybuff[i] = X[i] * t + Y[i - 1] * r;
		}

		Xbuff[k] = Y[k - 1] * s;
		Ybuff[k] = Y[k - 1] * r;

		X = Xbuff;
		Y = Ybuff;
		k++;
	}

	vector<vector<double>> inv;

	inv.resize(size);
	for (int i = 0; i < size; i++)
	{
		inv[i].resize(size);
	}

	vector<vector<double>> B1 = inv;
	vector<vector<double>> B2 = inv;
	vector<vector<double>> B3 = inv;
	vector<vector<double>> B4 = inv;

	bool check;

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			check = true;

			if (i + j < size)
			{
				B1[i + j][j] = X[i];
				B2[i][i + j] = Y[size - 1 - j];
				check = false;
			}

			if (i + j + 1 < size)
			{
				check = false;
				B3[i + 1 + j][j] = Y[i];
				B4[i][i + 1 + j] = X[size - 1 - j];
			}

			if (check)
			{
				break;
			}

		}

	}

	double buff12 = 0, buff34 = 0;

	double** b1 = convert(B1);
	double** b2 = convert(B2);
	double** b3 = convert(B3);
	double** b4 = convert(B4);
	double** invar = convert(inv);

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			buff12 = 0;
			buff34 = 0;
			for (int l = 0; l < size; l++)
			{
				buff12 += b1[i][l] * b2[l][j];
				buff34 += b3[i][l] * b4[l][j];
			}
			inv[i][j] = 1 / X[0] * (buff12 - buff34);
		}
	}

	delete[] b1;
	delete[] b2;
	delete[] b3;
	delete[] b4;

	res.resize(size);

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			res[i] += inv[i][j] * right[j];
		}
	}

	return res;
}

double* convert(vector<double> right)
{
	double* res = new double[right.size()];

	for (int i = 0; i < right.size(); i++)
	{
		res[i] = right[i];
	}

	return res;
}

double** convert(vector<vector<double>> right)
{
	double** res = new double* [right.size()];
	for (int i = 0; i < right.size(); i++)
	{
		res[i] = new double[right[i].size()];
		for (int j = 0; j < right[i].size(); j++)
		{
			res[i][j] = right[i][j];
		}
	}

	return res;
}

vector<double> Convert(double* right,int size)
{
	vector<double> res;
	
	for (int i = 0; i < size; i++)
	{
		res.push_back(right[i]);
	}

	return res;
}

vector<vector<double>> Convert(double** right, int size)
{
	vector<vector<double>> res;

	res.resize(size);

	for (int i = 0; i < size; i++)
	{
		res[i].resize(size);

		for (int j = 0; j < size; j++)
		{
			res[i][j] = right[i][j];
		}
	}

	return res;
}