#include "SplinesForm.h"
#include <Windows.h>

using namespace CG_3_Kolos;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew SplinesForm);
	return 0;
}

void CG_3_Kolos::SplinesForm::NBezier(System::Drawing::Graphics ^ g)//выбор алгоритма
{
	if (points.Count < 2)
	{
		return;
	}
	if (!bezier)
	{
		bezier = gcnew System::Collections::Generic::List<Point>();
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
void CG_3_Kolos::SplinesForm::clear()//очистка экрана
{
	Graphics^g = Graphics::FromImage(bm);

	g->Clear(Color::FromArgb(255));
	pictureBox->Invalidate();
	//points.Clear();
	//bezier->Clear();
}
//Point CG_3_Kolos::SplinesForm::DeBoor(int k, int degree, int i, float t, float* knots, System::Collections::Generic::List<Point>^ ctrlPoints)
//{
//	if (k == 0)
//		return ctrlPoints[i];
//	else
//	{
//		double alpha = (t - knots[i]) / (knots[i + degree + 1 - k] - knots[i]);
//		Point p1 = DeBoor(k - 1, degree, i - 1, x, knots, ctrlPoints);
//
//		Point p2 = DeBoor(k - 1, degree, i, x, knots, ctrlPoints);
//		return Point(p2.X*alpha + p1.X*(1 - alpha), p2.Y*alpha + p1.Y*(1 - alpha));
//	}
//}
//void CG_3_Kolos::SplinesForm::drawDeBoor(Graphics ^ g)
//{
//	for (int i = 4; i <= points.Count; i += 1)
//	{
//		for (float cur_t = 0; cur_t <= 1; cur_t += 0.005f)
//		{
//			Point point = DeBoor(i,4,
//			//im->DrawRectangle(gcnew Pen(Color::Black), point->X, point->Y, 1.0f, 1.0f);
//		}
//	}
//}
void CG_3_Kolos::SplinesForm::Bezier(System::Drawing::Graphics ^g)//классическая реализация
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
			float k = Utils::BinCoeff(i, j, n-1);
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
			tt[i] = new float[1] {(float)Math::Pow( t,i)};
			
			float h = tt[i][0];
		}
		
		float** res = Utils::MatMul(step1, tt, shape1, t_shape);
		bezier->Add(Point(res[0][0], res[1][0]));
		t +=step;
	}
	drawCurve(g,bezier);
	
}

void CG_3_Kolos::SplinesForm::CubicBezier(System::Drawing::Graphics ^ g,  int offset)//кубическая кривая
{
	bezier->Clear();
	int n = 4;
	float** coord = new float*[Utils::DIMENSIONS];
	
	for (int i = 0; i < Utils::DIMENSIONS;i++)
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
			tt[i] = new float[1]{ (float)Math::Pow(t,i) };

			float h = tt[i][0];
		}

		float** res = Utils::MatMul(step1, tt, shape1, t_shape);
		//g->DrawRectangle(Pens::Black, (int)res[0][0], (int)res[1][0], 1, 1);
		bezier->Add(Point(res[0][0], res[1][0]));
		t += 0.01f;
	}
	drawCurve(g,bezier);
}

void CG_3_Kolos::SplinesForm::ComposedBezier(System::Drawing::Graphics ^ g)//составная кривая с добавочной точкой
{
	int n = points.Count;
	
	for (int i = 0; i <n - 3; i += 3)
	{
		CubicBezier(g, i);
	}
}

void CG_3_Kolos::SplinesForm::ContinueLine()//добавление точки для соблюдения условий гладкости
{
	Point last = points[points.Count - 1];
	Point prelast = points[points.Count - 2];

	float deltaX = last.X - prelast.X;
	float deltaY = last.Y - prelast.Y;

	Point next = Point(last.X + deltaX, last.Y + deltaY);
	Graphics^g = Graphics::FromImage(bm);
	g->FillEllipse(Brushes::Red, next.X, next.Y, 2, 2);
	pictureBox->Invalidate();
	points.Add(next);
}


void CG_3_Kolos::SplinesForm::CloseBezier(System::Drawing::Graphics ^ g)//замыкание с помощью добавочных точек
{
	if (pointCount % 2 != 0)
	{
		ContinueLine();
	}

	float deltaX = points[0].X - points[1].X;
	float deltaY = points[0].Y - points[1].Y;

	Point next = Point(points[0].X+ deltaX, points[0].Y+ deltaY);

	closure.Clear();
	closure.Add(points[0]);
	closure.Add(next);
	closure.Add(points[points.Count - 1]);
	closure.Add(points[points.Count - 2]);

	CubicBezier(g, -1);
	
}

void CG_3_Kolos::SplinesForm::drawCurve(System::Drawing::Graphics ^ g, 
	System::Collections::Generic::List<Point> ^curve)//отрисовка кривой по массиву
{
	Pen ^p = deCasteljau ? Pens::Red : Pens::Black;
	
	clear();
	for each (Point node in points)
	{
		g->DrawEllipse(Pens::Orange, node.X, node.Y, 3, 3);

	}
	int n = curve->Count;
	for (int i = 0; i < n-1; i++)
	{
		g->DrawLine(p, curve[i], curve[i + 1]);
	}
	
}



/*рекурсивный алгоритм Костельжо разделен на два этапа из-за разной скорости сходимости по X и Y*/
void CG_3_Kolos::SplinesForm::BezierDeCasteljau(System::Drawing::Graphics ^ g)//реализация Кастельжо
{
	bezier->Clear();
	float delta = 0.01f;
	int order = points.Count-1;
	for (float t = 0; t <= 1; t += delta) {
		
		bezier->Add(Point(deCasteljauX(order, 0, t), deCasteljauY(order, 0, t)));
	}
	drawCurve(g,bezier);

}

void CG_3_Kolos::SplinesForm::ElemntaryBSpline(System::Collections::Generic::List<Point> ^p)
{
	if (!b_spline)
	{
		b_spline = gcnew System::Collections::Generic::List<Point>();
	}
	for (int i = 0; i < B_Spline_Count; i++)
	{
		Point next = CoordBSpline(p, i);
		b_spline->Add(next);
	};
	//Point next=(CoordBSpline({p0.X,p1.X, p2.X, p0.X,i}))

}


float* CG_3_Kolos::SplinesForm::CoeffsBSpline(float t)
{
	float t_2 = t*t;
	float**v = new float*[4]{
		new float[1]{1},
		new float[1]{ t },
		new float[1]{ t_2 },
		new float[1]{ t_2*t}
	};
	float* coeffs = new float[4];
	float**res = Utils::MatMul(bspline_basis,v, new int[2]{ 4,4 }, new int[2]{ 4,1 });
	//float* coeffs = new float[4];
	for (int i = 0; i < 4; i++)
	{
		coeffs[i] = res[i][0];
	}
	/*float t_3 = t_2*t;
	coeffs[0] = (1 - t)*(1 - t)*(1 - t);
	coeffs[1] = (3 * t_3 - 6 * t_2 + 4);
	coeffs[2] = 3 * (t_2 + t - t_3) + 1;
	coeffs[3] = t_3;*/
	return coeffs;
}

Point CG_3_Kolos::SplinesForm::CoordBSpline(System::Collections::Generic::List<Point> ^p, int ix)
{
	float resx = 0;
	float resy = 0;
	for (int i = 0; i < 4; i++)
	{
		resx += b_spline_coeffs[ix][i] * p[i].X;
		resy += b_spline_coeffs[ix][i] * p[i].Y;
	}
	resx /= 6;
	resy/= 6;
	return Point(resx, resy);
}

void CG_3_Kolos::SplinesForm::CacheBSplineCoeffs(float step)
{
	

	int len = 1 / step + 1;
	b_spline_coeffs = new float*[len];
	float t = 0;
	int i = 0;
	for (i; i < len; i++)
	{
		b_spline_coeffs[i] = CoeffsBSpline(t);
		t += step;
		if (t > 1)
		{
			break;
		}
	}
	B_Spline_Count = i;
}




float CG_3_Kolos::SplinesForm::deCasteljauX(int i, int j, float t)//рекурсивный поиск по X
{
	if (i == 1) {
		return (1 - t) * points[j].X + t * points[j + 1].X;
	}
	return (1 - t) * deCasteljauX(i - 1, j, t) + t * deCasteljauX(i - 1, j + 1, t);
}

float CG_3_Kolos::SplinesForm::deCasteljauY(int i, int j, float t)//рекурсивный поиск по Y
{
	if (i == 1) {
		return (1 - t) * points[j].Y + t * points[j + 1].Y;
	}
	return (1 - t) * deCasteljauY(i - 1, j, t) + t * deCasteljauY(i - 1, j + 1, t);
}






