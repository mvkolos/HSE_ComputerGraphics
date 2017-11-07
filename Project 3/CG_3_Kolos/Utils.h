#pragma once
class Utils {
public:
	static const int DIMENSIONS = 2;
	static float** MatMul(float** m1, float **m2, int shape1[], int shape2[]);
	//static System::Drawing::Point bezierRoutine(float** m1, float **m2, int n, float t);
	static float BinCoeff(int i, int j, int n);
	static float C(int n, int k);
	static unsigned int factorial(int n);
	float** bspline_basis;



};