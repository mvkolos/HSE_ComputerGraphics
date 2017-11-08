#include "Utility.h"
/*==========================Fill==========================*/
void FillTool::Draw(System::Windows::Forms::PaintEventArgs^  e, bool onMove)
{

	//System::Drawing::Bitmap ^tmp = gcnew System::Drawing::Bitmap(width,height);// bm);

	System::Drawing::Graphics ^g = System::Drawing::Graphics::FromImage(bm);
	SeedFill(g, current_point, System::Drawing::Color::Magenta);
	e->Graphics->DrawImage(bm, 0, 0);
}

void FillTool::SeedFill(System::Drawing::Graphics ^ g, System::Drawing::Point seed, System::Drawing::Color new_color)
{
	pixels.Push(seed);

	System::Drawing::Color old_color = bm->GetPixel(seed.X, seed.Y);
	if (compareColors(old_color, new_color))
	{
		return;
	}
	_Fill(g, old_color, new_color);
}

void FillTool::_Fill(System::Drawing::Graphics ^ g, System::Drawing::Color old_color, System::Drawing::Color new_color)
{
	while (pixels.Count>0) {

		System::Drawing::Point p = pixels.Pop();
		int left = p.X;

		while (left >= 0 && compareColors(bm->GetPixel(left, p.Y), old_color))
		{
			bm->SetPixel(left, p.Y, new_color);
			left--;
		}

		left++;
		int right = p.X + 1;
		while (right < width && compareColors(bm->GetPixel(right, p.Y), old_color))
		{
			bm->SetPixel(right, p.Y, new_color);
			right++;

		}

		right--;

		_CheckLine(left, right, p.Y - 1, old_color, new_color);
		_CheckLine(left, right, p.Y + 1, old_color, new_color);

	}
}

void FillTool::_CheckLine(int left, int right, int y, System::Drawing::Color old_color, System::Drawing::Color new_color)
{
	if (y >= height || y < 0 || left<0 || right >= width)
	{
		return;
	}

	int x = left + 1;
	while (x <right) {
		int x0 = x;

		// filling area
		while (x < right && compareColors(bm->GetPixel(x, y), old_color)) {
			x++;
		}

		// movement
		if (x0 != x)
		{
			pixels.Push(System::Drawing::Point(x0, y));

		}

		// skipping 
		while (x < right && !compareColors(bm->GetPixel(x, y), old_color)) {
			x++;
		}
	}
}

void FillTool::XORFill(System::Drawing::Graphics^ g,System::Drawing::Color border_color, System::Drawing::Bitmap ^b)
{
	System::Drawing::Color current_color = System::Drawing::Color::Aquamarine;
	g = System::Drawing::Graphics::FromImage(b);
	/*find right x border*/
	int xmax = -1;
	int len = SceneContent::pixel_lines.Count;
	for (int i = 0; i < len; i++)
	{
		
		System::Collections::Generic::List<System::Drawing::Point>^ t = SceneContent::pixel_lines[i];
		int line_len = t->Count;
		if (t[0].X > xmax)
			xmax = t[0].X;
		if (t[line_len-1].X > xmax)
			xmax = t[line_len - 1].X;
	}
	
	for (int i = 0; i <len; i++)
	{
		System::Collections::Generic::List<System::Drawing::Point>^ t = SceneContent::pixel_lines[i];
		int line_len = t->Count;
		int old_y = -1;
		for (int j = 0; j < line_len; j++)
		{
			int cur_y = t[j].Y;
			
			if (t[j].Y == old_y)
			{
				continue;
			}
			if (cur_y == System::Math::Max(t[0].Y, t[line_len-1].Y)) {
				continue;
			}
			old_y = cur_y;
			for (int x = t[j].X + 1; x < xmax; x++)
			{
				System::Drawing::Color cur_pixel_color = b->GetPixel(x, t[j].Y);
				if (compareColors(cur_pixel_color, border_color))
				{
					continue;
				}

				if (!compareColors(cur_pixel_color,current_color))//fill the pixel if it is not painted
				{
					//g->FillRectangle(gcnew System::Drawing::SolidBrush(current_color), (int)x, (int)t[j].Y, 1, 1);
					b->SetPixel((int)x, (int)t[j].Y, current_color);
					System::Drawing::Color c = b->GetPixel((int)x, (int)t[j].Y);
					int h = 0;
				}
				else
				{
					b->SetPixel((int)x, (int)t[j].Y, System::Drawing::Color::White);
					//g->FillRectangle(gcnew System::Drawing::SolidBrush(System::Drawing::Color::White), (int)x, (int)t[j].Y, 1, 1);
				}

			}
		}
	}


}

void FillTool::XORFill_Border(System::Drawing::Graphics ^ g, System::Drawing::Color border_color, System::Drawing::Bitmap ^ b)
{
	System::Drawing::Color current_color = System::Drawing::Color::Aquamarine;

	/*find both x borders*/
	int xmax = -1, xmin = 3000;
	int len = SceneContent::pixel_lines.Count;
	for (int i = 0; i < len; i++)
	{
		System::Collections::Generic::List<System::Drawing::Point>^ t = SceneContent::pixel_lines[i];
		int line_len = t->Count;
		//max
		if (t[0].X > xmax)
		{
			xmax = t[0].X;
		}
		if (t[line_len - 1].X > xmax)
		{
			xmax = t[line_len - 1].X;
		}
		//min
		if (t[0].X < xmin)
		{
			xmin = t[0].X;
		}
		if (t[line_len - 1].X < xmin)
		{
			xmin = t[line_len - 1].X;
		}
	}
	int xmid = (xmax + xmin) / 2;//border

	for (int i = 0; i < len; i++)
	{
		System::Collections::Generic::List<System::Drawing::Point>^ t = SceneContent::pixel_lines[i];
		int line_len = t->Count;
		int old_y = -1;
		for (int j = 0; j < line_len; j++)
		{
			int cur_y = t[j].Y;

			if (t[j].Y == old_y)
			{
				continue;
			}
			if (cur_y == System::Math::Max(t[0].Y, t[line_len - 1].Y)) {
				continue;
			}
			old_y = cur_y;

			int x = t[j].X, border = xmid;
			if (x > border)
			{
				int tmp = x;
				x = border;
				border = tmp;
			}
			for (x; x < border; x++)
			{
				System::Drawing::Color cur_pixel_color = b->GetPixel(x, t[j].Y);
				if (compareColors(cur_pixel_color, border_color))
				{
					continue;
				}

				if (!compareColors(cur_pixel_color, current_color))//fill the pixel if it is not painted
				{
					//g->FillRectangle(gcnew System::Drawing::SolidBrush(current_color), (int)x, (int)t[j].Y, 1, 1);
					b->SetPixel((int)x, (int)t[j].Y, current_color);
					System::Drawing::Color c = b->GetPixel((int)x, (int)t[j].Y);
					int h = 0;
				}
				else
				{
					b->SetPixel((int)x, (int)t[j].Y, System::Drawing::Color::White);
					//g->FillRectangle(gcnew System::Drawing::SolidBrush(System::Drawing::Color::White), (int)x, (int)t[j].Y, 1, 1);
				}

			}

		}

	}
}


/*==========================Ellipse==========================*/


void EllipseTool::Draw(System::Windows::Forms::PaintEventArgs^  e, bool onMove)
{
	int radius;
	System::Drawing::Graphics ^g;
	System::Drawing::Bitmap ^tmp;
	System::Drawing::Pen ^p = System::Drawing::Pens::Black;
	if (onMove)
	{
		tmp = gcnew System::Drawing::Bitmap(bm);// bm);

		g = System::Drawing::Graphics::FromImage(tmp);
	}
	else 
	{
		g = System::Drawing::Graphics::FromImage(bm);
		tmp = bm;
	}
	if (regular)
	{
		radius = System::Math::Max(frame.Width, frame.Height);
		drawEllipse(g, current_point, radius, radius, p);
	}
	else {
		drawEllipse(g, current_point, frame.Width, frame.Height, p);
	}
	e->Graphics->DrawImage(tmp, 0, 0);
}

void EllipseTool::drawEllipse(System::Drawing::Graphics ^ g, System::Drawing::Point center, int width, int height, System::Drawing::Pen ^p)
{
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

	int delta = b_sq + a_sq - 2 * a_sq * b;
	int etta = 0;
	int limit = 0;



	while (y > limit)
	{
		//System::Drawing::Pen ^p = System::Drawing::Pens::Blue;
		g->DrawRectangle(p, x + center.X, y + center.Y, 1, 1);
		g->DrawRectangle(p, -x + center.X, -y + center.Y, 1, 1);
		g->DrawRectangle(p, -x + center.X, y + center.Y, 1, 1);
		g->DrawRectangle(p, x + center.X, -y + center.Y, 1, 1);
		if (delta < 0)
		{
			etta = 2 * delta + (2 * y - 1)*a_sq;
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
				etta = 2 * delta - (2 * x + 1)*b_sq;
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
}

/*==========================Line==========================*/

void LineTool::Draw(System::Windows::Forms::PaintEventArgs ^ e, bool onMove)
{
	System::Drawing::Graphics ^g;
	System::Drawing::Bitmap ^tmp;
	System::Drawing::Pen ^p = System::Drawing::Pens::Black;
	if (onMove)
	{
		tmp = gcnew System::Drawing::Bitmap(bm);// bm);
		g = System::Drawing::Graphics::FromImage(tmp);
	}
	else {
		g = System::Drawing::Graphics::FromImage(bm);
		tmp = bm;

		//e->Graphics->DrawImage(bm, 0, 0);
	}
	if (regular)
	{
		if (line_start.X >= 0)
		{
			drawLine(g, current_point, System::Drawing::Point(frame.Right, frame.Bottom), !onMove, false);
			//line_start = current_point;

		}

		else
		{
			line_start = current_point;
			//g->DrawRectangle(Pens::Red, line_start.X,line_start.Y,1,1);
		}
	}
	else
	{
		if (!onMove)
		{
			DrawPolygon(g, !onMove);
		}
	}

	
	//line_start = System::Drawing::Point(-1, -1);
	e->Graphics->DrawImage(tmp, 0, 0);
}
System::Windows::Forms::Cursor^ LineTool::HandleMouseMove(System::Windows::Forms::MouseEventArgs ^ e)
{
	Tool::HandleMouseMove(e);
	if (!regular)
	{
		checkProximity(System::Drawing::Point(e->X, e->Y));
		if (readyToClose)		{

			return System::Windows::Forms::Cursors::Hand;

		}
		
	}
	return System::Windows::Forms::Cursors::Arrow;
}
void LineTool::DrawPolygon(System::Drawing::Graphics^g, bool save)
{
	if (readyToClose)
	{
		int idx = SceneContent::lines.Count - 1;
		drawLine(g,SceneContent::lines[idx]->Item2, polystart, save, false);
		ClosePolygon();
		return;
	}
	if (line_start.X >= 0)
	{
		drawLine(g, line_start, current_point, save, false);
		line_start = System::Drawing::Point(current_point.X,current_point.Y);
	}
	else
	{
		line_start = current_point;
		polystart = line_start;
		g->DrawEllipse(System::Drawing::Pens::Black, polystart.X, polystart.Y, 2, 2);
	}
}
void LineTool::ClosePolygon()
{
	line_start = System::Drawing::Point(-1, -1);
	polystart = System::Drawing::Point(-1, -1);
	FillTool::XORFill_Border(System::Drawing::Graphics::FromImage(bm), System::Drawing::Color::Black, bm);
	SceneContent::pixel_lines.Clear();
	
}
void LineTool::drawLine(System::Drawing::Graphics ^ g, System::Drawing::Point start, System::Drawing::Point end, bool save, bool xor)
{
	System::Collections::Generic::List<System::Drawing::Point>^pixels = gcnew System::Collections::Generic::List<System::Drawing::Point>();
	System::Drawing::Pen ^p = System::Drawing::Pens::Black;

	if (start == end)
	{
		return;
	}

	int signY = 1;
	int signX = 1;


	int x0 = start.X - width / 2;
	int y0 = start.Y - height / 2;

	int x1 = end.X - width / 2;
	int y1 = end.Y - height / 2;



	bool swap = false;
	//bool draw = 0;
	float error = 0.;


	int x = x0;
	int y = y0;


	int s1 = System::Math::Sign(x1 - x0);
	int s2 = System::Math::Sign(y1 - y0);

	int deltaX = System::Math::Abs(x0 - x1);
	int deltaY = System::Math::Abs(y0 - y1);

	if (deltaY > deltaX)
	{
		/*int temp = deltaY;
		deltaY = deltaY;
		deltaY = temp;
		swap = true;*/
		int temp = deltaX;
		deltaX = deltaY;
		deltaY = temp;
		swap = true;
	}

	error = 2 * deltaY - deltaX;
	int i = 0;

	while (i <= deltaX)
	{

		g->DrawRectangle(p, x + width / 2, y + height / 2, 1, 1);
		if (save)
		{
			pixels->Add(System::Drawing::Point(x + width / 2, y + height / 2));
		}

		while (error >= 0)
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
			
		}
		
			if (swap)
			{
				y += s2;
			}
			else
			{
				x += s1;
			}
			
			error += 2 * deltaY;
			i++;
			
		


	}

	if (save)
	{
		SceneContent::lines.Add(gcnew System::Tuple<System::Drawing::Point, System::Drawing::Point>(start, end));
		SceneContent::pixel_lines.Add(pixels);
		//textBoxDebug->Text = lines.Count.ToString();
	}
}

void ClippingTool::Draw(System::Windows::Forms::PaintEventArgs ^ e, bool onMove)
{
	System::Drawing::Graphics ^g;
	System::Drawing::Bitmap ^tmp;
	System::Drawing::Pen ^p = System::Drawing::Pens::Black;
	if (onMove)
	{
		tmp = gcnew System::Drawing::Bitmap(bm);// bm);

		g = System::Drawing::Graphics::FromImage(tmp);
		
	}
	else
	{
		g = System::Drawing::Graphics::FromImage(bm);
		tmp = bm;
	}
	//g->DrawRectangle(p, frame);
	g->Clear(System::Drawing::Color::White);
	CohenSutherlandClipping(frame, g, mode);
	e->Graphics->DrawImage(tmp, 0, 0);
}

void ClippingTool::CohenSutherlandClipping(System::Drawing::Rectangle rect, System::Drawing::Graphics ^ g, int mode)
{
	System::Drawing::Pen ^p = System::Drawing::Pens::Black;
	g->DrawRectangle(p, rect);
	int len = SceneContent::lines.Count;
	//textBoxDebug->Text = "cohen";
	outcode outcode0, outcode1;// , outcodeout;
	bool accept = false;// , done = false;
	for (int i = 0; i < len; i++)
	{
		auto t = SceneContent::lines[i];
		float x0 = t->Item1.X;
		float x1 = t->Item2.X;
		float y0 = t->Item1.Y;
		float y1 = t->Item2.Y;

		System::Drawing::Point p1 = System::Drawing::Point(x0, y0);
		System::Drawing::Point p2 = System::Drawing::Point(x1, y1);

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
			g->DrawLine(getColor(init0, init1), p1, p2);
			break;
		case 1:
			g->DrawLine(System::Drawing::Pens::Black, x0, y0, x1, y1);
			break;
		case 2:
			g->DrawLine(System::Drawing::Pens::Black, p1, p2);
		default:
			break;
		}
	}
}

outcode ClippingTool::computeOutcode(int x, int y, System::Drawing::Rectangle rect)
{
	outcode code;
	code = INSIDE;

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

System::Drawing::Pen ^ ClippingTool::getColor(int code1, int code2)
{
	if (code1&code2)
	{
		return gcnew System::Drawing::Pen(System::Drawing::Color::Green);//invisible
	}
	if (!code1 & !code2)
	{
		return gcnew System::Drawing::Pen(System::Drawing::Color::Blue);//visible
	}
	if (!code1 | !code2)
	{
		return gcnew System::Drawing::Pen(System::Drawing::Color::Pink);//partly seen
	}


	return gcnew System::Drawing::Pen(System::Drawing::Color::Yellow);//needs check
}

void LineTool::checkProximity(System::Drawing::Point p)
{
	readyToClose = false;
	if (p.X < 0)
	{
		return;
	}
	int delta = 20;
	//System::Drawing::Graphics::FromImage(bm)->DrawEllipse(System::Drawing::Pens::Red, p.X, p.Y, 4, 4);
	if (p.X > polystart.X - delta
		&&p.X < polystart.X + delta
		&&p.Y > polystart.Y - delta&&p.Y < polystart.Y + delta)
	{
		readyToClose= true;
	}
	
}
