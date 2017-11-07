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

float BezierTool::deCasteljauX(int i, int j, float t)
{
	if (i == 1) {
		return (1 - t) * points[j].X + t * points[j + 1].X;
	}
	return (1 - t) * deCasteljauX(i - 1, j, t) + t * deCasteljauX(i - 1, j + 1, t);
}

float BezierTool::deCasteljauY(int i, int j, float t)
{
	if (i == 1) {
		return (1 - t) * points[j].Y + t * points[j + 1].Y;
	}
	return (1 - t) * deCasteljauY(i - 1, j, t) + t * deCasteljauY(i - 1, j + 1, t);
}

void BezierTool::NBezier(System::Drawing::Graphics ^ g)
{
	if (points.Count < 2)
	{
		return;
	}
	if (!bezier)
	{
		bezier = gcnew System::Collections::Generic::List<System::Drawing::Point>();
	}
	else
	{
		bezier->Clear();
	}
	if (deCasteljau)
	{
		BezierDeCasteljau(g);
	}
	else
	{
		Bezier(g);
	}
}

void BezierTool::Bezier(System::Drawing::Graphics ^ g)
{
	int n = points.Count;
	float** coord = new float*[Utils::DIMENSIONS];
	float** coeffs = new float*[n];
	float step = 0.01f;

	for (int i = 0; i < Utils::DIMENSIONS; i++)
	{
		coord[i] = new float[n];
	}

	for (int i = 0; i < n; i++)
	{
		coord[0][i] = points[i].X;
		coord[1][i] = points[i].Y;
		coeffs[i] = new float[n];

	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			float k = Utils::BinCoeff(i, j, n - 1);
			coeffs[i][j] = k;
		}
	}
	int shape1[2] = { Utils::DIMENSIONS, n };
	int shape2[2] = { n, n };
	float** step1 = Utils::MatMul(coord, coeffs, shape1, shape2);

	float t = 0;
	int t_shape[2] = { n,1 };
	while (t < 1)
	{
		float**tt = new float*[n];
		for (int i = 0; i < n; i++)
		{
			tt[i] = new float[1]{ (float)System::Math::Pow(t,i) };

			float h = tt[i][0];
		}

		float** res = Utils::MatMul(step1, tt, shape1, t_shape);
		bezier->Add(System::Drawing::Point(res[0][0], res[1][0]));
		t += step;
	}
	drawCurve(g, bezier);
}

void BezierTool::CubicBezier(System::Drawing::Graphics ^ g, int offset)
{
	bezier->Clear();
	int n = 4;
	float** coord = new float*[Utils::DIMENSIONS];

	for (int i = 0; i < Utils::DIMENSIONS; i++)
	{
		coord[i] = new float[n];
	}
	for (int i = 0; i < n; i++)
	{
		if (offset < 0)
		{
			coord[0][i] = closure[i].X;
			coord[1][i] = closure[i].Y;
		}
		else {
			coord[0][i] = points[i + offset].X;
			coord[1][i] = points[i + offset].Y;
		}
	}
	int shape1[2] = { Utils::DIMENSIONS, n };
	int shape2[2] = { n, n };
	float** step1 = Utils::MatMul(coord, CubicMatrix, shape1, shape2);

	float t = 0;
	int t_shape[2] = { n,1 };
	while (t < 1)
	{
		float**tt = new float*[n];
		for (int i = 0; i < n; i++)
		{
			tt[i] = new float[1]{ (float)System::Math::Pow(t,i) };

			float h = tt[i][0];
		}

		float** res = Utils::MatMul(step1, tt, shape1, t_shape);
		//g->DrawRectangle(Pens::Black, (int)res[0][0], (int)res[1][0], 1, 1);
		bezier->Add(System::Drawing::Point(res[0][0], res[1][0]));
		t += 0.01f;
	}
	drawCurve(g, bezier);
}

void BezierTool::ComposedBezier(System::Drawing::Graphics ^ g)
{
	int n = points.Count;

	for (int i = 0; i <n - 3; i += 3)
	{
		CubicBezier(g, i);
	}
}

void BezierTool::ContinueLine()
{
	System::Drawing::Point last = points[points.Count - 1];
	System::Drawing::Point prelast = points[points.Count - 2];

	float deltaX = last.X - prelast.X;
	float deltaY = last.Y - prelast.Y;

	System::Drawing::Point next = System::Drawing::Point(last.X + deltaX, last.Y + deltaY);
	System::Drawing::Graphics^g = System::Drawing::Graphics::FromImage(bm);
	g->FillEllipse(System::Drawing::Brushes::Red, next.X, next.Y, 2, 2);
	//pictureBox->Invalidate();
	points.Add(next);
}

void BezierTool::CloseBezier(System::Drawing::Graphics ^ g)
{
	if (points.Count % 2 != 0)
	{
		ContinueLine();
	}

	float deltaX = points[0].X - points[1].X;
	float deltaY = points[0].Y - points[1].Y;

	System::Drawing::Point next = System::Drawing::Point(points[0].X + deltaX, points[0].Y + deltaY);

	closure.Clear();
	closure.Add(points[0]);
	closure.Add(next);
	closure.Add(points[points.Count - 1]);
	closure.Add(points[points.Count - 2]);

	CubicBezier(g, -1);
}

void BezierTool::BezierDeCasteljau(System::Drawing::Graphics ^ g)
{
	bezier->Clear();
	float delta = 0.01f;
	int order = points.Count - 1;
	for (float t = 0; t <= 1; t += delta) {

		bezier->Add(System::Drawing::Point(deCasteljauX(order, 0, t), deCasteljauY(order, 0, t)));
	}
	drawCurve(g, bezier);
}

bool MoveTool::Bounded(System::Drawing::Point &p, System::Drawing::Point &seed, int delta)
{
	if (p.X > seed.X - delta&&
		p.X<seed.X + delta&&
		p.Y>seed.Y - delta&&
		p.Y < seed.Y + delta)
	{
		return true;
	}
	return false;
}

int MoveTool::getClosest(System::Drawing::Point &current)
{
	for (int i = 0; i < points.Count; i++)
	{
		if (Bounded(current, points[i], 5))
		{
			return i;
		}
	}
	return -1;
}

System::Windows::Forms::Cursor ^ MoveTool::HandleMouseMove(System::Windows::Forms::MouseEventArgs ^ e)
{
	return System::Windows::Forms::Cursors::Arrow;
}

void MoveTool::HandleMouseDown(System::Windows::Forms::MouseEventArgs ^ e)
{
	if (e->Button == System::Windows::Forms::MouseButtons::Left)
	{
		int index = getClosest(System::Drawing::Point(e->X, e->Y));
		if (index > 0)

		{
			indexToMove = index;
			onMove = true;
		}
	}
	
}
