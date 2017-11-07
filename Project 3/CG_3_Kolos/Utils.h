#pragma once
class Utils {
public:
	static const int DIMENSIONS = 2;
	static float** MatMul(float** m1, float **m2, int shape1[], int shape2[]);
	//static System::Drawing::Point bezierRoutine(float** m1, float **m2, int n, float t);
	static float BinCoeff(int i, int j, int n);
	static float C(int n, int k);
	static unsigned int factorial(int n);
};

ref class Tool
{
public:
	//constructor
	Tool(System::Drawing::Bitmap ^b)
	{
		bm = b;
		width = b->Width;
		height = b->Height;
		spline = gcnew System::Collections::Generic::List<System::Drawing::Point>();
	}
	//display
	void drawCurve(System::Drawing::Graphics ^ g, System::Collections::Generic::List<System::Drawing::Point>^ curve)
	{
		System::Drawing::Pen ^p = System::Drawing::Pens::Black;

		clear();
		drawPoints(g);
		int n = curve->Count;
		for (int i = 0; i < n - 1; i++)
		{
			g->DrawLine(p, curve[i], curve[i + 1]);
		}
		g->DrawImage(bm, 0, 0);

	}
	void drawPoints(System::Drawing::Graphics ^ g)
	{
		System::Drawing::Pen ^p = System::Drawing::Pens::Black;
		for (int i = 0; i < points.Count; i++)
		{
			if (special_indices.Contains(i))
			{
				g->DrawEllipse(System::Drawing::Pens::Blue, points[i].X, points[i].Y, 4, 4);
			}
			else
			{
				g->DrawEllipse(p, points[i].X, points[i].Y, 3, 3);
			}
			if (i < points.Count - 1)
			{
				g->DrawLine(System::Drawing::Pens::LightGray, points[i], points[i + 1]);
			}
		}

	}

	//points on the screen
	static System::Collections::Generic::List<System::Drawing::Point> points;
	int point_count = 0;
	System::Collections::Generic::List<int> special_indices;
	//interactivity
	int indexToMove = -1;
	bool onMove = false;
	bool canMove = false;
	bool Bounded(System::Drawing::Point &p, System::Drawing::Point &seed, int delta);
	int getClosest(System::Drawing::Point &current, int radius);
	System::Drawing::Point current_point;
	virtual void updateSpline(System::Drawing::Point &p)
	{
		points[indexToMove] = p;
		//drawSpline();
	}
	

	//bitmap info
	int width, height;
	System::Drawing::Bitmap ^bm;

	//Cursor to change when moving
	System::Windows::Forms::Cursor^ cursor;
	bool deCasteljau = false;

	System::Collections::Generic::List<System::Drawing::Point> ^spline;
	void clear()
	{
		System::Drawing::Graphics^g = System::Drawing::Graphics::FromImage(bm);
		g->Clear(System::Drawing::Color::White);
	}
	virtual void drawSpline(System::Drawing::Graphics^g) {
		int b = 6;
	}
	
	//mouse event handlers
	virtual System::Windows::Forms::Cursor^ HandleMouseMove(System::Windows::Forms::MouseEventArgs^  e)
	{
		System::Windows::Forms::Cursor ^c = System::Windows::Forms::Cursors::Arrow;
		System::Drawing::Point p = System::Drawing::Point(e->X, e->Y);
		if (canMove)
		{
			if (getClosest(p, 5) >= 0)
			{
				c = System::Windows::Forms::Cursors::Hand;
			}
		}
		if (e->Button == System::Windows::Forms::MouseButtons::Left&&onMove)
		{			
			updateSpline(p);
			//points[indexToMove] = p;
			return System::Windows::Forms::Cursors::Hand;

		}
		return c;
	}
	virtual void HandleMouseUp(System::Windows::Forms::MouseEventArgs^  e) {
		if (e->Button == System::Windows::Forms::MouseButtons::Left&&onMove)
		{
			if (onMove&&canMove)
			{
				points[indexToMove] = System::Drawing::Point(e->X, e->Y);

				System::Drawing::Graphics ^g = System::Drawing::Graphics::FromImage(bm);
				clear();
				drawCurve(g, spline);

				System::Drawing::Graphics::FromImage(bm)->DrawImage(bm, 0, 0);
				onMove = false;
			}
		}
	}
	virtual void HandleMouseDown(System::Windows::Forms::MouseEventArgs^  e)
	{
		if (e->Button == System::Windows::Forms::MouseButtons::Left)
		{
			if (canMove)
			{

				int index = getClosest(System::Drawing::Point(e->X, e->Y), 5);
				if (index >= 0)

				{
					indexToMove = index;
					onMove = true;
				}

			}
			else {
				System::Drawing::Point p = System::Drawing::Point(e->X, e->Y);
				point_count++;
				points.Add(p);

				System::Drawing::Graphics^ g = System::Drawing::Graphics::FromImage(bm);
				g->DrawEllipse(System::Drawing::Pens::Gray, p.X, p.Y, 3, 3);

			}
		}
	}
	virtual void Draw(System::Windows::Forms::PaintEventArgs ^ e)
	{
		System::Drawing::Bitmap ^tmp;
		System::Drawing::Graphics ^g;
		if (canMove&&onMove)
		{
			//tmp = gcnew System::Drawing::Bitmap(bm);
			clear();
			
			//drawSpline(g);
		}
		else
		{
			tmp = bm;
		}
		g = System::Drawing::Graphics::FromImage(bm);
		drawSpline(g);
		e->Graphics->DrawImage(bm, 0, 0);
	}

};
ref class NBezierTool :public Tool
{
public:		
	NBezierTool(System::Drawing::Bitmap ^b) :Tool(b) {}
	//current curve
	
	
	//deCasteljau
	void BezierDeCasteljau(System::Drawing::Graphics^ g);
	float deCasteljauX(int i, int j, float t);
	float deCasteljauY(int i, int j, float t);
		
	//draw Bezier
	//void drawBezier(System::Drawing::Graphics ^g);
	void Bezier(System::Drawing::Graphics ^g);
	//bezier N
	//overriden control	
	void drawSpline(System::Drawing::Graphics^g) override;

	//void Draw(System::Windows::Forms::PaintEventArgs ^ e) override;

};
ref class ComposedBezierTool :public Tool
{
public:
	
	ComposedBezierTool(System::Drawing::Bitmap ^b) :Tool(b) {}

	void CubicBezier(System::Drawing::Graphics ^g, int offset);
	float** CubicMatrix = new float*[4]{
		new float[4]{ 1,-3,3,-1 },
		new float[4]{ 0,3,-6,3 },
		new float[4]{ 0,0,3,-3 },
		new float[4]{ 0,0,0,1 } };
	//composed of cubic
	void ComposedBezier(System::Drawing::Graphics ^g);
	
	bool composed = false;
	void ContinueLine();
	//close bezier
	void CloseBezier(System::Drawing::Graphics ^ g);
	System::Collections::Generic::List<System::Drawing::Point> closure;

	//overriden control	
	void drawSpline(System::Drawing::Graphics^g) override;

};
