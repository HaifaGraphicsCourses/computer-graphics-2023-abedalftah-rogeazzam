void Renderer::DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color)
{
	// TODO: Implement bresenham algorithm
	// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

	int dx = abs(p2[0] - p1[0]);
	int dy = abs(p2[1] - p1[1]);

	int e = 2 * dy - dx;

	int x1 = p1[0];
	int y1 = p1[1];

	int x2 = p2[0];
	int y2 = p2[1];

	bool trueFalse = true; //Tells us if slope(m) > 1 or < 1

	if (dx < dy) { //If the slope(m) > 1 then we need to just modify the points (to swap each x with y) and change the error  
		e = 2 * dx - dy;

		int temp = dx; 
		dx = dy;
		dy = temp;

		temp = y1;
		y1 = x1;
		x1 = temp;

		temp = y2;
		y2 = x2;
		x2 = temp;



		trueFalse = false;
	}

	for (int i = 0; i <= dx ; i++) {
		if (x1 < x2) x1++; else x1--;
		
		if (e < 0) {
			if (trueFalse) //tells us if we're in the second situation
				PutPixel(x1, y1, color);
			else
				PutPixel(y1, x1, color);
			e += 2 * dy;
		}
		else {
			if (y1 < y2) y1++; else y1--;

			if (trueFalse)//tells us if we're in the second situation
				PutPixel(x1, y1, color);
			else
				PutPixel(y1, x1, color);

			e += 2 * dy - 2 * dx;
		}
	}
}

![alt text](Screenshot(65).png)