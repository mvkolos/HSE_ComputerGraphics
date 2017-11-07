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


ref class Tool
{
public:
	
	System::Drawing::Point current_point;
	int width, height;
	System::Drawing::Bitmap ^bm;
	bool regular = false;
	System::Windows::Forms::Cursor^ cursor;
	static System::Collections::Generic::List<System::Drawing::Point> points;
	

	Tool(System::Drawing::Bitmap ^b) {
		bm = b;
		width = b->Width;
		height = b->Height;

		//points = gcnew System::Collections::Generic::List<System::Drawing::Point>();

	}
	void clear() {
		System::Drawing::Graphics^g = System::Drawing::Graphics::FromImage(bm);

		g->Clear(System::Drawing::Color::White);
	}
	virtual System::Windows::Forms::Cursor^ HandleMouseMove(System::Windows::Forms::MouseEventArgs^  e)
	{
		if (e->Button == System::Windows::Forms::MouseButtons::Left)
		{
			
		}
		return System::Windows::Forms::Cursors::Arrow;
	}
	virtual void HandleMouseUp(System::Windows::Forms::MouseEventArgs^  e) {

		if (e->Button == System::Windows::Forms::MouseButtons::Left)
		{
			
		}

	}
	virtual void HandleMouseDown(System::Windows::Forms::MouseEventArgs^  e)
	{
		if (e->Button == System::Windows::Forms::MouseButtons::Left)
		{
			current_point = System::Drawing::Point(e->X, e->Y);
		}
	}
	virtual void Draw(System::Windows::Forms::PaintEventArgs^  e, bool onMove) {};
	void drawCurve(System::Drawing::Graphics ^ g, System::Collections::Generic::List<System::Drawing::Point>^ curve)
	{
		System::Drawing::Pen ^p = System::Drawing::Pens::Black;

		clear();
		for each (System::Drawing::Point node in points)
		{
			g->DrawEllipse(p, node.X, node.Y, 3, 3);

		}
		int n = curve->Count;
		for (int i = 0; i < n - 1; i++)
		{
			g->DrawLine(p, curve[i], curve[i + 1]);
		}
	}

};
ref class BezierTool :public Tool
{
public:	
	BezierTool(System::Drawing::Bitmap ^b) :Tool(b) {}
	//current curve
	System::Collections::Generic::List<System::Drawing::Point> ^bezier;
	
	//deCasteljau
	void BezierDeCasteljau(System::Drawing::Graphics^ g);
	float deCasteljauX(int i, int j, float t);
	float deCasteljauY(int i, int j, float t);
	bool deCasteljau = false;
	
	//draw Bezier
	void Bezier(System::Drawing::Graphics ^g);
	//bezier N
	void NBezier(System::Drawing::Graphics ^g);
	//cubic
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
	void HandleMouseDown(System::Windows::Forms::MouseEventArgs^  e) override {
		
		System::Drawing::Point p = System::Drawing::Point(e->X,e->Y);
		points.Add(p);
		System::Drawing::Graphics^ g = System::Drawing::Graphics::FromImage(bm);
		g->DrawEllipse(System::Drawing::Pens::Gray, p.X, p.Y, 3, 3);
		if (composed)
		{
			if (points.Count > 3 && points.Count % 2 == 0)
			{
				ContinueLine();
				ComposedBezier(g);
			}

		}
		else {
			//clear();
			NBezier(g);
		}
	}
};
ref class MoveTool :public Tool 
{
public:
	MoveTool(System::Drawing::Bitmap ^b) : Tool(b) {}
	bool Bounded(System::Drawing::Point &p, System::Drawing::Point &seed, int delta);
	int getClosest(System::Drawing::Point &current);

	System::Windows::Forms::Cursor^ HandleMouseMove(System::Windows::Forms::MouseEventArgs^  e) override;
	/*void HandleMouseUp(System::Windows::Forms::MouseEventArgs^  e) override;	*/
	void HandleMouseDown(System::Windows::Forms::MouseEventArgs^  e) override;
	
private:
	int indexToMove = -1;
	bool onMove = false;
	
	
};