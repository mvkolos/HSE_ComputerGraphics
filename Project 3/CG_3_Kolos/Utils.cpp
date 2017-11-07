#include "Utils.h"

float ** Utils::MatMul(float ** m1, float ** m2, int shape1[], int shape2[])
{
	if (shape1[1] != shape2[0])
	{
		return nullptr;
	}

	float** res = new float*[shape1[0]];
	for (int i = 0; i < shape1[0]; i++)
	{
		res[i] = new float[shape2[1]];
	}
	for (int i = 0; i < shape1[0]; i++)
	{
		for (int j = 0; j < shape2[1]; j++)
		{
			res[i][j]=0;
		}
	}
	for (int col = 0; col < shape2[1]; col++)
	{
		for (int row = 0; row < shape1[0]; row++)
		{
			for (int inner = 0;inner < shape1[1]; inner++)
			{
				float r= m1[row][inner] * m2[inner][col];
				res[row][col] += r;
			}
		}
	}
	return res;
}


float Utils::BinCoeff(int i, int j, int n)
{
	int sign = (j - i) % 2 == 0 ? 1 : -1;
	float q = C(n, j);
	float b = C(j, i);
	float res = sign*q*b;
	return res;
}

float Utils::C(int n, int k)
{
	if (k == 0 || k == n)
	{
		return 1;
	}
	if (n<k)
	{
		return 0;
	}

	// Recur
	return  C(n - 1, k - 1) + C(n - 1, k);
}


//int Utils::BinCoeff(int i, int j, int n)
//{
//	int sign = (j - i) % 2 == 0 ? 1 : -1;
//	int q = C(j, n);
//	int b = C(i, j);
//	int res = sign*q*b;
//	return res;
//}

//int Utils::C(int n, int k)
//{
//	if (k == 0 || k == n)
//	{
//		return 1;
//	}
//	if (k > n)
//	{
//		return 0;
//	}
// 
//  // Recur
//  return  C(n-1, k-1) + C(n-1, k);
//}

unsigned int Utils::factorial(int n)
{
	int res = 1;
	int i;
	for (i = 1; i <= n; i++)
	{
		res *= i;
	}
	return res;

}


