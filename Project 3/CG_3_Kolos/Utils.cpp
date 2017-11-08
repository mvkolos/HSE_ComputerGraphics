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

float NBezierTool::deCasteljauX(int i, int j, float t)
{
	if (i == 1) {
		return (1 - t) * points[j].X + t * points[j + 1].X;
	}
	return (1 - t) * deCasteljauX(i - 1, j, t) + t * deCasteljauX(i - 1, j + 1, t);
}

float NBezierTool::deCasteljauY(int i, int j, float t)
{
	if (i == 1) {
		return (1 - t) * points[j].Y + t * points[j + 1].Y;
	}
	return (1 - t) * deCasteljauY(i - 1, j, t) + t * deCasteljauY(i - 1, j + 1, t);
}

//void NBezierTool::NBezier(System::Drawing::Graphics ^ g)
//{
//}

void NBezierTool::drawSpline(System::Drawing::Graphics ^ g)
{
	if (points.Count < 2)
	{
		return;
	}
	if (spline) {
		spline->Clear();
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

//void NBezierTool::drawBezier(System::Drawing::Graphics ^ g)
//{
//	/*if (composed)
//	{
//		if (point_count > 3&& point_count % 2 == 0)
//		{
//			ContinueLine();
//			ComposedBezier(g);
//		}
//
//	}*/
//
//	NBezier(g);
//
//}

void NBezierTool::Bezier(System::Drawing::Graphics ^ g)
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
		spline->Add(System::Drawing::Point(res[0][0], res[1][0]));
		t += step;
	}
	drawCurve(g, spline);
}

void ComposedBezierTool::CubicBezier(System::Drawing::Graphics ^ g, int offset)
{	
	//bezier->Clear();
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
		spline->Add(System::Drawing::Point(res[0][0], res[1][0]));
		t += 0.01f;
	}
	drawCurve(g, spline);
}

void ComposedBezierTool::ComposedBezier(System::Drawing::Graphics ^ g)
{
	

	int n = points.Count;// point_count;
	spline->Clear();
	for (int i = 0; i <n-3; i += 3)
	{
		CubicBezier(g, i);
	}
}

void ComposedBezierTool::ContinueLine()
{
	if (special_indices.Contains(points.Count - 1))
	{
		return;
	}
	System::Drawing::Point last = points[points.Count - 1];
	
	System::Drawing::Point prelast = points[points.Count - 2];

	float deltaX = last.X - prelast.X;
	float deltaY = last.Y - prelast.Y;

	System::Drawing::Point next = System::Drawing::Point(last.X + deltaX, last.Y + deltaY);
	System::Drawing::Graphics^g = System::Drawing::Graphics::FromImage(bm);
	g->FillEllipse(System::Drawing::Brushes::Red, next.X, next.Y, 2, 2);
	special_indices.Add(points.Count);
	//poin
	points.Add(next);
}


void ComposedBezierTool::CloseBezier(System::Drawing::Graphics ^ g)
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

void ComposedBezierTool::rotatePointAround(int offset, int point)
{
	throw gcnew System::NotImplementedException();
}

void ComposedBezierTool::parallelDisplacement(int offset, int point)
{
	throw gcnew System::NotImplementedException();
}

void ComposedBezierTool::drawSpline(System::Drawing::Graphics ^ g)
{
	if (point_count >3&&point_count % 2 == 0)
	{
		ContinueLine();
		ComposedBezier(g);
	}
}

void ComposedBezierTool::updateSpline(System::Drawing::Point & p)
{
	System::Drawing::Point tmp = points[indexToMove];
	points[indexToMove] = p;
	/*if (indexToMove < 3)
	{
		return;
	}*/
	if (special_indices.Contains(indexToMove))
	{
		int delta_x = tmp.X - points[indexToMove - 1].X;
		int delta_y = tmp.Y - points[indexToMove - 1].Y;
		points[indexToMove - 2] = System::Drawing::Point(points[indexToMove - 1].X - delta_x,
			points[indexToMove - 1].Y - delta_y);
		return;
	}
	else {
		if (special_indices.Contains(indexToMove + 1))
		{
			
			//int a = p.Y - tmp.Y;
			int x = p.X - points[indexToMove - 1].X;
			int y = p.Y - points[indexToMove - 1].Y;
			//float dist1 = System::Math::Sqrt(x*x + y*y);
			
			points[indexToMove + 1]= System::Drawing::Point(p.X+x ,
				p.Y+y);
			return;
		}
		else if (special_indices.Contains(indexToMove + 2)) {
			int delta_x = points[indexToMove].X - points[indexToMove + 1].X;
			int delta_y = points[indexToMove].Y - points[indexToMove + 1].Y;
			points[indexToMove + 2] = System::Drawing::Point(points[indexToMove +1].X - delta_x,
				points[indexToMove +1].Y - delta_y);
		}
	}
}



//System::Windows::Forms::Cursor ^ ComposedBezierTool::HandleMouseMove(System::Windows::Forms::MouseEventArgs ^ e)
//{
//	throw gcnew System::NotImplementedException();
//	// TODO: вставьте здесь оператор return
//}

void NBezierTool::BezierDeCasteljau(System::Drawing::Graphics ^ g)
{
	spline->Clear();
	float delta = 0.01f;
	int order = points.Count - 1;
	for (float t = 0; t <= 1; t += delta) {

		spline->Add(System::Drawing::Point(deCasteljauX(order, 0, t), deCasteljauY(order, 0, t)));
	}
	drawCurve(g, spline);
}

bool Tool::Bounded(System::Drawing::Point &p, System::Drawing::Point &seed, int delta)
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

int Tool::getClosest(System::Drawing::Point &current,int radius)
{
	for (int i = 0; i < points.Count; i++)
	{
		if (Bounded(current, points[i], radius))
		{
			return i;
		}
	}
	return -1;
}

//System::Windows::Forms::Cursor ^ NBezierTool::HandleMouseMove(System::Windows::Forms::MouseEventArgs ^ e)
//{
//	System::Windows::Forms::Cursor ^c = System::Windows::Forms::Cursors::Arrow;
//	System::Drawing::Point p = System::Drawing::Point(e->X, e->Y);
//	if (canMove)
//	{
//		if (getClosest(p,5) >= 0)
//		{
//			c = System::Windows::Forms::Cursors::Hand;
//		}
//	}
//	if (e->Button == System::Windows::Forms::MouseButtons::Left&&onMove)
//	{
//		points[indexToMove] = p;
//		
//		return System::Windows::Forms::Cursors::Hand;
//
//	}
//	return c;
//}

//void NBezierTool::HandleMouseUp(System::Windows::Forms::MouseEventArgs ^ e)
//{
//	if (e->Button == System::Windows::Forms::MouseButtons::Left&&onMove)
//	{
//		if (onMove&&canMove)
//		{
//			points[indexToMove] = System::Drawing::Point(e->X, e->Y);
//			
//			System::Drawing::Graphics ^g = System::Drawing::Graphics::FromImage(bm);
//			clear();
//			drawCurve(g, spline);
//
//			System::Drawing::Graphics::FromImage(bm)->DrawImage(bm, 0, 0);
//			onMove = false;
//		}
//	}
//}

//void NBezierTool::HandleMouseDown(System::Windows::Forms::MouseEventArgs ^ e)
//{
//	
//}
//
//void NBezierTool::Draw(System::Windows::Forms::PaintEventArgs ^ e)
//{
//	if (canMove&&onMove)
//	{
//		System::Drawing::Bitmap ^tmp = gcnew System::Drawing::Bitmap(bm);
//		System::Drawing::Graphics ^g = System::Drawing::Graphics::FromImage(tmp);
//		//clear();
//		drawSpline(g);
//
//		e->Graphics->DrawImage(tmp, 0, 0);
//	}
//	else
//	{
//		//clear();
//		drawSpline(System::Drawing::Graphics::FromImage(bm));
//		e->Graphics->DrawImage(bm, 0, 0);
//
//	}
//}



