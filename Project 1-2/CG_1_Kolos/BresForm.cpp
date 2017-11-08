#include "BresForm.h"
#include <Windows.h>

using namespace CG_1_Kolos;

/*Kolos Maria
BSE141
Project1 - Bresenham*/
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew BresForm);
	return 0;
}


void CG_1_Kolos::BresForm::drawLine(Graphics^g,Point start, Point end, bool save,bool xor)
{
	
	if(start==end)
	{
		return;
	}
	int signY = 1;
	int signX = 1;
	

	int x0 = start.X-pictureBox->Width/2;
	int y0 = start.Y - pictureBox->Height/2;

	int x1 = end.X -pictureBox->Width / 2;
	int y1 = end.Y - pictureBox->Height / 2;

	

	bool swap = 0;
	bool draw = 0;
	float error = 0.;


	int x = x0;
	int y = y0;


	int s1 = Math::Sign(x1 - x0);
	int s2 = Math::Sign(y1 - y0);

	int deltaX = Math::Abs(x0 - x1);
	int deltaY = Math::Abs(y0 - y1);

	if (deltaY > deltaX)
	{
		int temp = deltaX;
		deltaX = deltaY;
		deltaY = temp;
		swap = true;
	}

	error = 2 * deltaY - deltaX;
	int i = 0;

	while (i <= deltaX)
	{
		if (draw)
		{
			
			if (xor)
			{
				contour->Add(Point(x + pictureBox->Width / 2, y + pictureBox->Height / 2));
			}
			else
			{
				g->DrawRectangle(Pens::Red, x + pictureBox->Width / 2, y + pictureBox->Height / 2, thickness, thickness);
			}
		}
		if (error >= 0)
		{
			if (swap)
			{
				x += s1;
			}
			else
			{
				y += s2;
			}
			error = error - 2 * deltaX;
			draw = false;
		}
		else
		{
			if (swap)
			{
				y += s2;
			}
			else
			{
				x += s1;
			}
			i++;
			error += 2 * deltaY;
			draw = true;
		}


	}

	if (save)
	{
		lines.Add(gcnew System::Tuple<Point, Point>(start, end));
		//textBoxDebug->Text = lines.Count.ToString();
	}
}



void CG_1_Kolos::BresForm::drawEllipse(Graphics ^g,Point center, int width,int height)
{
	//Graphics^g = Graphics::FromImage(bm);
	if (width == 0 || height == 0)
	{
		return;
	}
	int a = width;
	int b = height;

	int x = 0;
	int y = b;

	int a_sq = a*a;
	int b_sq = b*b;

	int delta = b_sq+a_sq-2*a_sq*a/b;
	int etta = 0;
	int limit = 0;

	

	while (y > limit)
	{
		g->DrawRectangle(Pens::Blue, x + center.X, y + center.Y, 1, 1);
		g->DrawRectangle(Pens::Blue, -x + center.X, -y + center.Y, 1, 1);
		g->DrawRectangle(Pens::Blue, -x + center.X, y + center.Y, 1, 1);
		g->DrawRectangle(Pens::Blue, x + center.X, -y + center.Y, 1, 1);
		if (delta < 0)
		{
			etta = 2 * delta +(2*y - 1)*a_sq;
			if (etta <= 0)
			{
				x++;
				delta += (2 * x + 1)*b_sq;
				continue;
			}

		}
		else
		{
			if (delta > 0)
			{
				etta = 2 * delta -(2*x+1)*b_sq;
				if (etta >0)
				{
					y--;
					delta += (1 - 2 * y)*a_sq;
					continue;
				}
			}
		}
		x++;
		y--;
		delta += (2 * x + 1) * b_sq + (1 - 2 * y) *a_sq;
		

	}
	//delete g;
	//pictureBox->Refresh();
}

void CG_1_Kolos::BresForm::refresh()
{
	Graphics ^g = Graphics::FromImage(bm);
	g->Clear(Color::White);
	onRefresh = true;
	pictureBox->Invalidate();
	
	lastPoint = Point(pictureBox->Width / 2, pictureBox->Height / 2);
}

void CG_1_Kolos::BresForm::redraw()
{
	//throw gcnew System::NotImplementedException();
	Bitmap ^tmp = gcnew Bitmap(pictureBox->Width, pictureBox->Height);
	Graphics^ g = Graphics::FromImage(tmp);
	g->Clear(Color::White);
	if (bm != nullptr)
	{
		g->DrawImage(bm, 0, 0);
		//bm->Dispose();
	}
	
	bm = tmp;
}

void CG_1_Kolos::BresForm::SeedFill(Graphics ^g,Point seed, Color new_color)
{
	pixels.Push(seed);

	Color old_color = bm->GetPixel(seed.X, seed.Y);
	if (compareColors(old_color, new_color))
	{
		return;
	}
	_Fill(g, old_color, new_color);

	
}
//void CG_1_Kolos::BresForm::push(int x, int y)
//{
//	Point p = Point(x, y);
//	if (!pixels.Contains(p))
//	{
//		pixels.Push(p);
//
//	}
//}
void CG_1_Kolos::BresForm::XORFill()
{
	int len = lines.Count;
	for (int k = 0; k < len; k++)
	{
		auto t = lines[k];

		float x0 = t->Item1.X;// Item1.X;
		float x1 = t->Item2.X;
		float y0 = t->Item1.Y;
		float y1 = t->Item2.Y;
		Point p1 = Point(x0, y0);

		Point p2 = Point(x1, y1);

		contour = gcnew System::Collections::Generic::List<Point>();
		//drawLine(gcnew Graphics::FromImage(bm), p1, p2, false, true, border);
		
		

		//for()
	}

}


void CG_1_Kolos::BresForm::CohenSutherlandClipping(System::Drawing::Rectangle rect, Graphics ^g,int mode)
{
	refresh();
	g->DrawRectangle(Pens::Black, rect);
	int len = lines.Count;
	//textBoxDebug->Text = "cohen";
	outcode outcode0, outcode1;// , outcodeout;
	bool accept = false;// , done = false;
	for(int i=0;i<len;i++)
	{
		auto t = lines[i];
		float x0 = t->Item1.X;
		float x1 = t->Item2.X;
		float y0 = t->Item1.Y;
		float y1 = t->Item2.Y;

		Point p1 = Point(x0, y0);
		Point p2 = Point(x1, y1);

		outcode0 = computeOutcode(x0, y0, rect);
		outcode1 = computeOutcode(x1, y1, rect);

		outcode init0 = outcode0;
		outcode init1 = outcode1;

		//textBoxDebug->Text = outcode0.ToString() + " " + outcode1.ToString();
		while (true)
		{
			if (!(outcode0 | outcode1))
			{
				accept = true;
				break;
			}
			else if (outcode0 & outcode1)
			{
				break;
			}
			else
			{
				float x, y;
				int ocd = outcode0 ? outcode0 : outcode1;
				if (ocd & TOP)
				{
					x = x0 + (x1 - x0)*(rect.Top - y0) / (y1 - y0);
					y = rect.Top;
				}
				else if (ocd & BOTTOM)
				{
					x = x0 + (x1 - x0)*(rect.Bottom - y0) / (y1 - y0);
					y = rect.Bottom;
				}

				else if (ocd & LEFT)
				{
					y = y0 + (y1 - y0)*(rect.Left - x0) / (x1 - x0);
					x = rect.Left;
				}
				else if (ocd&RIGHT)
				{
					y = y0 + (y1 - y0)*(rect.Right - x0) / (x1 - x0);
					x = rect.Right;
				}
				if (ocd == outcode0)
				{
					x0 = x;
					y0 = y;
					outcode0 = computeOutcode(x0, y0, rect);
				}
				else
				{
					x1 = x;
					y1 = y;
					outcode1 = computeOutcode(x1, y1, rect);
				}
			}

		}
		switch (mode)
		{
		case 0:
			g->DrawLine(getColor(init0,init1), p1,p2);
			break;
		case 1:
			g->DrawLine(Pens::Black, x0, y0, x1, y1);
			break;
		case 2:
			g->DrawLine(Pens::Black, p1, p2);
		default:
			break;
		}

		if (accept)
		{
			
			
		}
		
	}
}
outcode CG_1_Kolos::BresForm::computeOutcode(int x, int y, System::Drawing::Rectangle rect)
{
	outcode code;
	code= INSIDE;
	
	if (x < rect.Left)          // to the left of clip window
		code |= LEFT;
	else if (x > rect.Right)      // to the right of clip window
		code |= RIGHT;
	if (y > rect.Bottom)           // below the clip window
		code |= BOTTOM;
	else if (y < rect.Top)      // above the clip window
		code |= TOP;
	//textBoxDebug->Text = x.ToString() + " " + y.ToString() + " " + rect.ToString()+" "+code.ToString();
	return code;
}
Pen^ CG_1_Kolos::BresForm::getColor(int code1,int code2)
{
	if (code1&code2)
	{
		return gcnew Pen(Color::Green);//invisible
	}
	if (!code1 & !code2)
	{
		return gcnew Pen(Color::Blue);//visible
	}
	if (!code1|!code2)
	{
		return gcnew Pen(Color::Pink);//partly seen
	}
	
	
	return gcnew Pen(Color::Yellow);//needs check
}
void CG_1_Kolos::BresForm::_Fill(Graphics ^ g, Color old_color, Color new_color)
{
	while(pixels.Count>0) {
		
		Point p = pixels.Pop();
		int left = p.X;
		
		while (left >= 0 && compareColors(bm->GetPixel(left, p.Y), old_color))
		{
			bm->SetPixel(left, p.Y, new_color);
			left--;
		}
		
		left++;
		int right = p.X+1;
		while (right < pictureBox->Width && compareColors(bm->GetPixel(right, p.Y), old_color))
		{
			bm->SetPixel(right, p.Y, new_color);
			right++;

		}
		
		right--;		
		
		_CheckLine(left, right, p.Y - 1, old_color,new_color);
		_CheckLine(left, right, p.Y + 1, old_color,new_color);

	}

}

void CG_1_Kolos::BresForm::_CheckLine(int left, int right,int y,Color old_color, Color new_color)
{
	if (y >= pictureBox->Height || y < 0||left<0||right>=pictureBox->Width) 
	{
		return;
	}

	int x = left+1;
	while (x <right) {
		int x0 = x;

		// filling area
		while (x < right && compareColors(bm->GetPixel(x, y), old_color)) {
			x++;
		}

		// movement
		if (x0 != x) 
		{
			pixels.Push(Point(x0,y));
			
		}

		// skipping 
		while (x < right && !compareColors(bm->GetPixel(x, y), old_color)) {
			x++;
		}
	}


}

bool CG_1_Kolos::BresForm::compareColors(Color r, Color l)
{
	return (r.R == l.R&&r.G == l.G&&r.B == l.B&&r.A == l.A);
}

bool CG_1_Kolos::BresForm::contains(Point p)
{
	int len = pixels.Count;
	for (int i = 0; i < len; i++)
	{
		//if (pixels.=.X == p.X&&pixels[i].Y==p.Y)
	}
	return false;
}

bool CG_1_Kolos::BresForm::intersect(Point c1, Point c2, Point d1, Point d2)
{
	return ccw(c1, d1, d2) != ccw(c2, d1, d2) & ccw(c1, c2, d1) != ccw(c1, c2, d2);
}

bool CG_1_Kolos::BresForm::ccw(Point a, Point b, Point c)
{
	return (c.Y - a.Y) * (b.X - a.X) > (b.Y - a.Y) * (c.X - a.X);
}

void CG_1_Kolos::BresForm::checkProximity(Point p)
{
	readyToClose = false;
	int delta = 20;
	if (p.X > polystart.X - delta&&p.X < polystart.X + delta&&p.Y > polystart.Y - delta&&p.Y < polystart.Y +delta)
	{
		readyToClose= true;
	}
	
	
}


void CG_1_Kolos::BresForm::drawFourPoints(Pen^ pen, Point center,int x, int y)
{
	Graphics ^g = Graphics::FromImage(bm);
	g->DrawRectangle(pen, center.X + x, center.Y + y, 1, 1);
	g->DrawRectangle(pen, center.X - x, center.Y + y, 1, 1);
	g->DrawRectangle(pen, center.X - x, center.Y - y, 1, 1);
	g->DrawRectangle(pen, center.X + x, center.Y - y, 1, 1);
	delete g;
	pictureBox->Refresh();
}
void CG_1_Kolos::BresForm::paint(Graphics ^ g, bool save)
{
	if (bresenham)
	{
		switch (tool)
		{
		case 0:
		{
			//Point p = pictureBox->PointToClient(Cursor->Position);
			if (line_start.X >= 0) 
			{
				drawLine(g, line_start, pictureBox->PointToClient(Cursor->Position),save,false);
				//line_start = Point(-1, -1);
			}
			else
			{
				line_start = pictureBox->PointToClient(Cursor->Position);
				//g->DrawRectangle(Pens::Red, line_start.X,line_start.Y,1,1);
			}
			break;

		}
		case 1:
		{
			int radius = Math::Max(mRect.Width, mRect.Height);
			drawEllipse(g, currentPoint, radius, radius);
			//e->Graphics->DrawRectangle(Pens::Blue,mRect);
			break;
		}
		case 2:
		{
			drawEllipse(g, currentPoint, mRect.Width, mRect.Height);
			//e->Graphics->DrawRectangle(Pens::Blue, mRect);
			break;
		}
		case 3:
		{
			//refresh();
			g->DrawRectangle(Pens::Black,mRect);
			
			break;
		}
		case 4:
		{
			if (line_start.X >= 0)
			{
				drawLine(g, line_start, pictureBox->PointToClient(Cursor->Position), save, false);
				//line_start = Point(-1, -1);
			}
			else
			{
				line_start = pictureBox->PointToClient(Cursor->Position);
				polystart = line_start;
				
				//g->DrawRectangle(Pens::Red, line_start.X,line_start.Y,1,1);
			}
			break;
		}
		default:
			break;
		}
	}
	else
	{
		drawBuiltIn();
	}
}
void CG_1_Kolos::BresForm::drawBuiltIn()
{
	Graphics ^g = Graphics::FromImage(bm);
	switch (tool)
	{
	case 0:
	{
		g->DrawLine(Pens::Black,currentPoint, lastPoint);
		break;

	}
	case 2:
	{
		g->DrawEllipse(Pens::Black, currentPoint.X- width/2, currentPoint.Y- height/2, width, height);
		break;
	}
	case 1:
	{
		//g->DrawEllipse(Pens::Black, currentPoint.X-radius/2, currentPoint.Y - radius / 2, radius,radius);
	}
	default:
		break;
	}
	delete g;
	
}
