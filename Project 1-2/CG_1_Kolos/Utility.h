#pragma once

typedef int outcode;
//enum { INSIDE=0x0,TOP = 0x8, BOTTOM = 0x4, RIGHT = 0x2, LEFT = 0x1 };
//typedef int OutCode;

const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 4;  // 0100
const int BOTTOM = 8; // 1000
const int TOP = 2;    // 0010
ref class SceneContent
{
public:
	static System::Collections::Generic::List< System::Tuple<System::Drawing::Point, System::Drawing::Point>^> lines;
	static System::Collections::Generic::List
		<System::Collections::Generic::List<System::Drawing::Point>^> pixel_lines;
	static System::Collections::Generic::List<System::Drawing::Point> ^contour;
};
ref class Tool
{
public:
	System::Drawing::Rectangle frame;
	System::Drawing::Point current_point;
	int width, height;
	System::Drawing::Bitmap ^bm;
	bool regular = false;
	System::Windows::Forms::Cursor^ cursor;
	
	Tool(int w, int h){
		width = w;
		height = h;
	
	}
	virtual System::Windows::Forms::Cursor^ HandleMouseMove(System::Windows::Forms::MouseEventArgs^  e)
	{
		if (e->Button == System::Windows::Forms::MouseButtons::Left)
		{
			frame = System::Drawing::Rectangle(current_point.X, current_point.Y, e->X - current_point.X, e->Y - current_point.Y);
		}
		return System::Windows::Forms::Cursors::Arrow;
	}
	virtual void HandleMouseUp(System::Windows::Forms::MouseEventArgs^  e) {

		if (e->Button == System::Windows::Forms::MouseButtons::Left)
		{
			frame = System::Drawing::Rectangle(current_point.X, current_point.Y, e->X - current_point.X, e->Y - current_point.Y);
		}

	}
	virtual void HandleMouseDown(System::Windows::Forms::MouseEventArgs^  e)
	{
		if (e->Button == System::Windows::Forms::MouseButtons::Left)
		{
			current_point = System::Drawing::Point(e->X, e->Y);
		}
	}
	virtual void Draw(System::Windows::Forms::PaintEventArgs^  e,bool onMove) {};
	

};

	ref class EllipseTool : public Tool
	{
	public:		
		EllipseTool(System::Drawing::Bitmap ^b) :Tool(b->Width, b->Height)
		{
			bm = b;
		}

		
		void Draw(System::Windows::Forms::PaintEventArgs^  e,bool onMove) override;

		void drawEllipse(System::Drawing::Graphics^ g,
			System::Drawing::Point center,
			int width,
			int height,
			System::Drawing::Pen ^p);
	};

	ref class LineTool : public Tool
	{
	public:
		//constructor
		LineTool(System::Drawing::Bitmap ^b) :Tool(b->Width, b->Height)
		{
			bm = b;
			line_start = System::Drawing::Point(-1, -1);
			polystart = System::Drawing::Point(-1, -1);
		}
		//fields
		System::Drawing::Point line_start;
		System::Drawing::Point polystart;
		bool readyToClose = false;
		//overriden
		void Draw(System::Windows::Forms::PaintEventArgs^  e, bool onMove) override;
		System::Windows::Forms::Cursor^ HandleMouseMove(System::Windows::Forms::MouseEventArgs^  e) override;

		//tool functionality		
		void DrawPolygon(System::Drawing::Graphics^g, bool save);
		void ClosePolygon();
		void drawLine(System::Drawing::Graphics^g,
			System::Drawing::Point start,
			System::Drawing::Point end,
			bool save,
			bool xor);
		void checkProximity(System::Drawing::Point p);
		
		
	};
	
	
	
	/*ref class TransformTool : public Tool
	{

	};*/
	ref class FillTool : public Tool
	{
	public:
		//constructor
		FillTool(System::Drawing::Bitmap ^b) :Tool(b->Width, b->Height)
		{
			bm = b;
		}
		System::Collections::Generic::Stack<System::Drawing::Point> pixels;

		//overriden
		void Draw(System::Windows::Forms::PaintEventArgs^  e, bool onMove) override;
		
		//tool functionality
		void SeedFill(System::Drawing::Graphics ^g,
			System::Drawing::Point seed,
			System::Drawing::Color new_color);
		void _Fill(System::Drawing::Graphics ^g,
			System::Drawing::Color old_color,
			System::Drawing::Color new_color);
		void _CheckLine(int left,
			int right,
			int y,
			System::Drawing::Color old_color,
			System::Drawing::Color new_color);
		static bool compareColors(System::Drawing::Color r, System::Drawing::Color l)
		{
			return (r.R == l.R&&r.G == l.G&&r.B == l.B&&r.A == l.A);
		}
		static void XORFill(System::Drawing::Graphics^ g,System::Drawing::Color border_color,  System::Drawing::Bitmap ^b);
		static void XORFill_Border(System::Drawing::Graphics^ g, System::Drawing::Color border_color, System::Drawing::Bitmap ^b);
	};
	ref class ClippingTool : public Tool
	{
	public:
		ClippingTool(System::Drawing::Bitmap ^b):Tool(b->Width, b->Height)
		{
			bm = b;
		}
		int mode = 0;
		void Draw(System::Windows::Forms::PaintEventArgs^  e, bool onMove) override;


		void CohenSutherlandClipping(
			System::Drawing::Rectangle rect,
			System::Drawing::Graphics ^g,
			int mode);
		outcode computeOutcode(int x, int y,
			System::Drawing::Rectangle rect);
		System::Drawing::Pen^ getColor(int code1, int code2);


	};
