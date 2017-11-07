#pragma once

/*Колос Мария Владимировна, БПИ141, 
04.10.17, 
Проект 2. растр,
– Visual Studio 2017 Community, Windows 10
Реализация заливки
Реализация отсечения
.*/
#using <System.dll>
#using <System.Windows.Forms.dll>
#using <System.Drawing.dll>

#include "Utility.h"
//#include "v.h"
namespace CG_1_Kolos {


	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	

	/// <summary>
	/// Сводка для BresForm
	/// </summary>
	public ref class BresForm : public System::Windows::Forms::Form
	{


	public:
		BresForm(void)
		{
			InitializeComponent();

			pointCount = 0;
			bm = gcnew Bitmap(pictureBox->Width, pictureBox->Height);
			pictureBox->Image = bm;
			refresh();

			lastPoint = Point(pictureBox->Width / 2, pictureBox->Height / 2);//намечаем оси для квадрантов
			bresenham = false;//реализация Graphics
			checkBoxBres->Checked = true;
			thickness = 1;
			//radius = 20;
			height = 20;
			width = 10;
			DoubleBuffered = true;
			int a = 8;
			int b = 2;
			//current_tool = gcnew EllipseTool(bm);
			
			//textBoxDebug->Text = (bool(a & b)).ToString();// +" " + (boolean(a | b)).ToString();
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~BresForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;
		Bitmap^ bm;
	private: System::Windows::Forms::CheckBox^  checkBoxBres;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::PictureBox^  pictureBox;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  aboutToolStripMenuItem;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Button^  buttonCircle;
	private: System::Windows::Forms::Button^  buttonEllipse;
	private: System::Windows::Forms::Button^  buttonLine;


	private: System::Windows::Forms::Button^  buttonFill;
	private: System::Windows::Forms::TextBox^  textBoxDebug;
	private: System::Windows::Forms::Button^  buttonRect;
	private: System::Windows::Forms::Button^  buttonIntersect;
	private: System::Windows::Forms::Button^  buttonPolygon;

	public:
		Tool ^current_tool;
		bool onMove = false;
		bool onRefresh = false;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	public:

	public:
		int pointCount;

		void drawLine(Graphics^g,
			Point start,
			Point end,
			bool save,
			bool xor);

		void drawEllipse(Graphics^ g,
			Point center,
			int width,
			int height);
		void drawFourPoints(Pen^ pen,
			Point center,
			int x,
			int y);
		void drawBuiltIn();
		void refresh();
		void redraw();
		void SeedFill(Graphics ^g,
			Point seed,
			Color new_color);
		void _Fill(Graphics ^g,
			Color old_color,
			Color new_color);
		void paint(Graphics ^g, bool save);
		void _CheckLine(int left,
			int right,
			int y,
			Color old_color,
			Color new_color);
		bool compareColors(Color r, Color l);

		//void push(int x, int y);

		void XORFill();


		void CohenSutherlandClipping(
			System::Drawing::Rectangle rect,
			Graphics ^g,
			int mode);
		outcode computeOutcode(int x, int y,
			System::Drawing::Rectangle rect);
		Pen^ getColor(int code1, int code2);

		bool intersect(Point c1, Point c2, Point d1, Point d2);
		bool ccw(Point a, Point b, Point c);

		void checkProximity(Point p);

		Point currentPoint;
		Point lastPoint;
		bool bresenham;//whether bresenham algorithm is used, otherwise built-in functions
		int tool = 0;


		int thickness;//line thickness
		bool fill = false;


		int height;//ellipse height/width
		int width;

		//int radius;
		System::Drawing::Rectangle mRect;
		//SolidBrush ^_ghostBrush;
		bool draw;
		System::Collections::Generic::Stack<Point> pixels;
		int recCount = 0;
		Color border;
		bool contains(Point p);
		Point line_start = Point(-1, -1);
		Point polystart;
		bool readyToClose = false;

		System::Collections::Generic::List< System::Tuple<Point, Point>^> lines;
		System::Collections::Generic::List<Point> ^contour;

		// Tool^ currentTool;// = gcnew EllipseTool();

		 //System::Tuple<Point, Point> p;

		 //System::Drawing::

#pragma region Windows Form Designer generated code
			 /// <summary>
			 /// Требуемый метод для поддержки конструктора — не изменяйте 
			 /// содержимое этого метода с помощью редактора кода.
			 /// </summary>
		void InitializeComponent(void)
		{
			this->checkBoxBres = (gcnew System::Windows::Forms::CheckBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->pictureBox = (gcnew System::Windows::Forms::PictureBox());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->aboutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->buttonPolygon = (gcnew System::Windows::Forms::Button());
			this->buttonIntersect = (gcnew System::Windows::Forms::Button());
			this->buttonRect = (gcnew System::Windows::Forms::Button());
			this->buttonFill = (gcnew System::Windows::Forms::Button());
			this->buttonLine = (gcnew System::Windows::Forms::Button());
			this->buttonEllipse = (gcnew System::Windows::Forms::Button());
			this->buttonCircle = (gcnew System::Windows::Forms::Button());
			this->textBoxDebug = (gcnew System::Windows::Forms::TextBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox))->BeginInit();
			this->menuStrip1->SuspendLayout();
			this->panel1->SuspendLayout();
			this->SuspendLayout();
			// 
			// checkBoxBres
			// 
			this->checkBoxBres->AutoSize = true;
			this->checkBoxBres->Location = System::Drawing::Point(22, 173);
			this->checkBoxBres->Name = L"checkBoxBres";
			this->checkBoxBres->Size = System::Drawing::Size(79, 17);
			this->checkBoxBres->TabIndex = 3;
			this->checkBoxBres->Text = L"Bresenham";
			this->checkBoxBres->UseVisualStyleBackColor = true;
			this->checkBoxBres->CheckedChanged += gcnew System::EventHandler(this, &BresForm::checkBox1_CheckedChanged);
			// 
			// button1
			// 
			this->button1->BackColor = System::Drawing::SystemColors::AppWorkspace;
			this->button1->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button1->Location = System::Drawing::Point(16, 357);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(85, 25);
			this->button1->TabIndex = 4;
			this->button1->Text = L"clear";
			this->button1->UseVisualStyleBackColor = false;
			this->button1->Click += gcnew System::EventHandler(this, &BresForm::button1_Click);
			// 
			// pictureBox
			// 
			this->pictureBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->pictureBox->Location = System::Drawing::Point(165, 45);
			this->pictureBox->MaximumSize = System::Drawing::Size(600, 600);
			this->pictureBox->MinimumSize = System::Drawing::Size(200, 200);
			this->pictureBox->Name = L"pictureBox";
			this->pictureBox->Size = System::Drawing::Size(600, 600);
			this->pictureBox->TabIndex = 5;
			this->pictureBox->TabStop = false;
			this->pictureBox->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &BresForm::pictureBox_Paint);
			this->pictureBox->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &BresForm::pictureBox_MouseDown);
			this->pictureBox->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &BresForm::pictureBox_MouseMove);
			this->pictureBox->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &BresForm::pictureBox_MouseUp);
			this->pictureBox->Resize += gcnew System::EventHandler(this, &BresForm::BresForm_Load);
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->aboutToolStripMenuItem });
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(795, 24);
			this->menuStrip1->TabIndex = 22;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// aboutToolStripMenuItem
			// 
			this->aboutToolStripMenuItem->Alignment = System::Windows::Forms::ToolStripItemAlignment::Right;
			this->aboutToolStripMenuItem->Name = L"aboutToolStripMenuItem";
			this->aboutToolStripMenuItem->Size = System::Drawing::Size(50, 20);
			this->aboutToolStripMenuItem->Text = L"about";
			this->aboutToolStripMenuItem->Click += gcnew System::EventHandler(this, &BresForm::aboutToolStripMenuItem_Click);
			// 
			// panel1
			// 
			this->panel1->Controls->Add(this->label2);
			this->panel1->Controls->Add(this->label1);
			this->panel1->Controls->Add(this->buttonPolygon);
			this->panel1->Controls->Add(this->buttonRect);
			this->panel1->Controls->Add(this->checkBoxBres);
			this->panel1->Controls->Add(this->buttonFill);
			this->panel1->Controls->Add(this->button1);
			this->panel1->Controls->Add(this->buttonLine);
			this->panel1->Controls->Add(this->buttonEllipse);
			this->panel1->Controls->Add(this->buttonCircle);
			this->panel1->Location = System::Drawing::Point(23, 45);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(117, 465);
			this->panel1->TabIndex = 23;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(33, 234);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(43, 13);
			this->label2->TabIndex = 10;
			this->label2->Text = L"TOOLS";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(33, 26);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(50, 13);
			this->label1->TabIndex = 9;
			this->label1->Text = L"SHAPES";
			// 
			// buttonPolygon
			// 
			this->buttonPolygon->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->buttonPolygon->Location = System::Drawing::Point(16, 142);
			this->buttonPolygon->Name = L"buttonPolygon";
			this->buttonPolygon->Size = System::Drawing::Size(85, 25);
			this->buttonPolygon->TabIndex = 8;
			this->buttonPolygon->Text = L"Polygon";
			this->buttonPolygon->UseVisualStyleBackColor = true;
			this->buttonPolygon->Click += gcnew System::EventHandler(this, &BresForm::buttonPolygon_Click);
			// 
			// buttonIntersect
			// 
			this->buttonIntersect->Location = System::Drawing::Point(49, 636);
			this->buttonIntersect->Name = L"buttonIntersect";
			this->buttonIntersect->Size = System::Drawing::Size(57, 23);
			this->buttonIntersect->TabIndex = 7;
			this->buttonIntersect->Text = L"intersect";
			this->buttonIntersect->UseVisualStyleBackColor = true;
			this->buttonIntersect->Click += gcnew System::EventHandler(this, &BresForm::buttonIntersect_Click);
			// 
			// buttonRect
			// 
			this->buttonRect->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->buttonRect->Location = System::Drawing::Point(16, 257);
			this->buttonRect->Name = L"buttonRect";
			this->buttonRect->Size = System::Drawing::Size(85, 25);
			this->buttonRect->TabIndex = 6;
			this->buttonRect->Text = L"clip";
			this->buttonRect->UseVisualStyleBackColor = true;
			this->buttonRect->Click += gcnew System::EventHandler(this, &BresForm::buttonRect_Click);
			// 
			// buttonFill
			// 
			this->buttonFill->Location = System::Drawing::Point(16, 288);
			this->buttonFill->Name = L"buttonFill";
			this->buttonFill->Size = System::Drawing::Size(85, 25);
			this->buttonFill->TabIndex = 5;
			this->buttonFill->Text = L"fill";
			this->buttonFill->UseVisualStyleBackColor = true;
			this->buttonFill->Click += gcnew System::EventHandler(this, &BresForm::buttonFill_Click);
			// 
			// buttonLine
			// 
			this->buttonLine->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->buttonLine->Location = System::Drawing::Point(16, 111);
			this->buttonLine->Name = L"buttonLine";
			this->buttonLine->Size = System::Drawing::Size(85, 25);
			this->buttonLine->TabIndex = 2;
			this->buttonLine->Text = L"Line";
			this->buttonLine->UseVisualStyleBackColor = true;
			this->buttonLine->Click += gcnew System::EventHandler(this, &BresForm::buttonLine_Click);
			// 
			// buttonEllipse
			// 
			this->buttonEllipse->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->buttonEllipse->Location = System::Drawing::Point(16, 80);
			this->buttonEllipse->Name = L"buttonEllipse";
			this->buttonEllipse->Size = System::Drawing::Size(85, 25);
			this->buttonEllipse->TabIndex = 1;
			this->buttonEllipse->Text = L"Ellipse";
			this->buttonEllipse->UseVisualStyleBackColor = true;
			this->buttonEllipse->Click += gcnew System::EventHandler(this, &BresForm::buttonEllipse_Click);
			// 
			// buttonCircle
			// 
			this->buttonCircle->BackColor = System::Drawing::SystemColors::Control;
			this->buttonCircle->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->buttonCircle->Location = System::Drawing::Point(16, 49);
			this->buttonCircle->Name = L"buttonCircle";
			this->buttonCircle->Size = System::Drawing::Size(85, 25);
			this->buttonCircle->TabIndex = 0;
			this->buttonCircle->Text = L"Circle";
			this->buttonCircle->UseVisualStyleBackColor = false;
			this->buttonCircle->Click += gcnew System::EventHandler(this, &BresForm::buttonCircle_Click);
			// 
			// textBoxDebug
			// 
			this->textBoxDebug->Location = System::Drawing::Point(176, 667);
			this->textBoxDebug->Name = L"textBoxDebug";
			this->textBoxDebug->Size = System::Drawing::Size(400, 20);
			this->textBoxDebug->TabIndex = 26;
			// 
			// BresForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(795, 726);
			this->Controls->Add(this->textBoxDebug);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->pictureBox);
			this->Controls->Add(this->buttonIntersect);
			this->Controls->Add(this->menuStrip1);
			this->Name = L"BresForm";
			this->Text = L"BresForm";
			this->Load += gcnew System::EventHandler(this, &BresForm::BresForm_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox))->EndInit();
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void BresForm_Load(System::Object^  sender, System::EventArgs^  e) {
		//_ghostBrush = %SolidBrush(Color::FromArgb(200, 200, 200, 255)); //This creates a slightly blue, transparent brush for the ghost preview
	}

	private: System::Void checkBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		bresenham = !bresenham;
	}
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
		refresh();
	}


	private: System::Void buttonCircle_Click(System::Object^  sender, System::EventArgs^  e)
	{
		tool = 1;
		current_tool = gcnew EllipseTool(bm);
		current_tool->regular = true;
	}
	private: System::Void buttonLine_Click(System::Object^  sender, System::EventArgs^  e)
	{
		tool = 0;
		current_tool = gcnew LineTool(bm);
		current_tool->regular = true;

	}

	private: System::Void buttonEllipse_Click(System::Object^  sender, System::EventArgs^  e)
	{
		//tool = 2;
		current_tool = gcnew EllipseTool(bm);
	}

	private: System::Void trackBarThick_Scroll(System::Object^  sender, System::EventArgs^  e) {

	}

	private: System::Void menuStrip1_ItemClicked(System::Object^  sender, System::Windows::Forms::ToolStripItemClickedEventArgs^  e) {
	}
	private: System::Void aboutToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		String ^text = "Колос Мария Владимировна, БПИ141\n25.09.17" +
			"\nПроект 1. Алгоритмы Брезенхема" +
			"\nVisual Studio 2017 Community, Windows 10" +
			"\nРеализация прямой" +
			"\nРеализация окружности и эллипса(эллипс - обобщенный случай)";
		MessageBox::Show(text, "О программе", MessageBoxButtons::OK,
			MessageBoxIcon::Asterisk);
	}


	private: System::Void trackBar1_Scroll(System::Object^  sender, System::EventArgs^  e) {
		//thickness = trackBarThick->Value;
	}



	private: System::Void pictureBox_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		textBoxDebug->Text = "down";
		if (!current_tool)
		{
			return;
		}
		if (e->Button == System::Windows::Forms::MouseButtons::Left)
		{
			//onMove = false;
			current_tool->HandleMouseDown(e); 
		}
		
		
		//Point p = pictureBox->PointToClient(Cursor->Position);

		//if (e->Button == System::Windows::Forms::MouseButtons::Left)
		//{
		//	currentPoint = Point(e->X, e->Y);
		//}
		//switch (tool)
		//{
		//case 4:
		//	//if ()
		//	break;
		//default:
		//	break;
		//}

	}

	private: System::Void pictureBox_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		if (!current_tool)
		{
			return;
		}
		Cursor=current_tool->HandleMouseMove(e);
		if (e->Button == System::Windows::Forms::MouseButtons::Left)
		{
			onMove = true;
			
			pictureBox->Invalidate();
		}
		
		//current_tool->Draw()
		//if (tool == 4)
		//{
		//	/*Point p = pictureBox->PointToClient(Cursor->Position);
		//	textBoxDebug->Text = polystart.ToString();*/
		//	checkProximity(Point(e->X, e->Y));
		//	if (readyToClose)
		//	{
		//		Cursor = Cursors::Hand;
		//	}
		//	else
		//	{
		//		Cursor = Cursors::Arrow;
		//	}
		//}
		//if (e->Button == System::Windows::Forms::MouseButtons::Left)
		//{
		//	draw = true;
		//	mRect = System::Drawing::Rectangle(currentPoint.X, currentPoint.Y, e->X - currentPoint.X, e->Y - currentPoint.Y);

		//}
	}
	private: System::Void pictureBox_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e)
	{
		//textBoxDebug->Text ="paint";
		if (onRefresh) {
			e->Graphics->DrawImage(bm, 0, 0);
			onRefresh = false;
		}
		else {
			current_tool->Draw(e, onMove);
		}
		//if (fill)
		//{
		//	
		//	current_tool->current_point = currentPoint;
		//	current_tool->Draw(e);
		//	//Bitmap ^tmp = gcnew Bitmap(pictureBox->Width, pictureBox->Height);// bm);

		//	//Graphics ^g = Graphics::FromImage(tmp);
		//	//SeedFill(g, currentPoint, Color::Magenta);
		//	//e->Graphics->DrawImage(tmp, 0, 0);
		//		
		//	fill = false;
		//}
		//if (draw)
		//{
		//	Bitmap ^tmp = gcnew Bitmap(pictureBox->Width, pictureBox->Height);// bm);

		//	Graphics ^g = Graphics::FromImage(tmp);
		//	paint(e->Graphics, false);

		//	//drawEllipse(g, currentPoint, mRect.Width, mRect.Height);
		//	e->Graphics->DrawImage(tmp, 0, 0);
		//}
		//if (!fill && !draw)
		//{
		//	e->Graphics->DrawImage(bm, 0, 0);
		//}

		//e->Graphics->DrawImage(bm, 0, 0);

	}
	private: System::Void pictureBox_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		textBoxDebug->Text = "up";
		if (!current_tool)
		{
			return;
		}
		
		if (e->Button == System::Windows::Forms::MouseButtons::Left)
		{
			onMove = false;
			current_tool->HandleMouseUp(e);
			pictureBox->Invalidate();
		}
		
		//if (e->Button == System::Windows::Forms::MouseButtons::Left && !fill)
		//{
		//	//Create a Graphics for the offscreen bitmap
		//	Graphics ^g = Graphics::FromImage(bm);
		//	System::Drawing::Rectangle rect = System::Drawing::Rectangle(currentPoint.X, currentPoint.Y, e->X - currentPoint.X, e->Y - currentPoint.Y);

		//	paint(g, true);
		//	if (tool == 3)
		//	{
		//		CohenSutherlandClipping(mRect, g, 0);
		//	}
		//	if (tool == 0)
		//	{
		//		line_start = Point(-1, -1);
		//	}
		//	else if (tool == 4)
		//	{
		//		line_start = Point(pictureBox->PointToClient(Cursor->Position));
		//	}
		//}

		//draw = false;

		//This queues up a redraw call for the form
		

	}
	private: System::Void buttonFill_Click(System::Object^  sender, System::EventArgs^  e) {
		current_tool= gcnew FillTool(bm);
		/*fill = !fill;
		draw = false;*/
	}

	private: System::Void buttonRect_Click(System::Object^  sender, System::EventArgs^  e) {
		tool = 3;
		current_tool = gcnew ClippingTool(bm);
	}
	private: System::Void buttonIntersect_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if (lines.Count > 1)
		{
			auto t = lines[lines.Count - 1];
			Point a = Point(t->Item1.X, t->Item1.Y);
			Point b = Point(t->Item2.X, t->Item2.Y);

			t = lines[lines.Count - 2];

			Point c = Point(t->Item1.X, t->Item1.Y);
			Point d = Point(t->Item2.X, t->Item2.Y);

			//textBoxDebug->Text = intersect(a, b, c, d).ToString();
		}
	}
	private: System::Void buttonPolygon_Click(System::Object^  sender, System::EventArgs^  e)
	{
		tool = 4;
		current_tool =gcnew LineTool(bm);
		current_tool->cursor = Cursor;
		//currentTool = gcnew PolygonTool();
	}

	
};
}
