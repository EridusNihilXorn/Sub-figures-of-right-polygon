#include "draw.h"
#include "stdafx.h"
#include <math.h>
#include "Objects.h"

#define RGB32(r, g, b) static_cast<uint32_t>((((static_cast<uint32_t>(b) << 8) | g) << 8) | r)

Matrix transformation(information transforms);
double calculateAngle(int nOfAngle, double l, double delta);
void lineBresenham(SDL_Surface *s, int x1, int y1, int x2, int y2, int color);
void drawFigure(double c, double b, int color, int countSQ, SDL_Surface *s);

Matrix matrixTransformation;
extern int amount;
int baseX = 240, baseY = 240;

void put_pixel32(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	Uint32 *pixels = (Uint32 *)surface->pixels;
	pixels[(y * surface->w) + x] = pixel;
}

Uint32 get_pixel32(SDL_Surface *surface, int x, int y)
{
	Uint32 *pixels = (Uint32 *)surface->pixels;
	return pixels[(y * surface->w) + x];
}

//Draws right polygon with fixed amount of sides.
void drawFigure(double c, double b, int color, int sides, SDL_Surface *s)
{
	int coefficient = 1;
	Vector vector1 = Vector(), vector2 = Vector();
		vector1.x = (int)(baseX + c * cos(b + coefficient * M_PI / sides));
		vector1.y = (int)(baseY + c * sin(b + coefficient * M_PI / sides));
		coefficient += 2;
		for (int i = 0; i < sides; i++) {
			vector2.x = (int)(baseX + c * cos(b + coefficient * M_PI / sides));
			vector2.y = (int)(baseY + c * sin(b + coefficient * M_PI / sides));
			lineBresenham(s, vector1.x, vector1.y, vector2.x, vector2.y, color);
			coefficient += 2;
			vector1 = vector2;
		}
}
//Draw line with method of Bresenham.
void lineBresenham(SDL_Surface *s, int x1, int y1, int x2, int y2, int color){
	const int dX = abs(x2 - x1);
	const int dY = abs(y2 - y1);
	const int sX = x1 < x2 ? 1 : -1;
	const int sY = y1 < y2 ? 1 : -1;
	int error = dX - dY;
	while (x1 != x2 || y1 != y2)
	{
		Vector vectorCurrent = matrixTransformation.transform(Vector(x1-baseX, y1-baseY, 0));
		if(vectorCurrent.x + baseX > 0 && vectorCurrent.x + baseX < 2*baseX && vectorCurrent.y + baseY > 0 && vectorCurrent.y + baseY < 2*baseY)
		put_pixel32(s, vectorCurrent.x+baseX, vectorCurrent.y+baseY, color);
		const int doubleError = error * 2;
		if (doubleError > -dY)
		{
			error -= dY;
			x1 += sX;
		}
		if (doubleError < dX)
		{
			error += dX;
			y1 += sY;
		}
	}
}

//Agile modification of current angle to make no "spaces" between lines with complexity growth.
double calculateAngle(int amountOfAngles, double distanceToCentre, double alpha) {
	double beta = M_PI * (amountOfAngles - 2) / amountOfAngles - 0.01;
	double discriminant = (2 * distanceToCentre*(1 + cos(beta)))*(2 * distanceToCentre*(1 + cos(beta))) - (distanceToCentre*distanceToCentre * 8 * (1-(alpha*alpha)) * (1 + cos(beta)));
	double x = (2 * distanceToCentre*(1 + cos(beta)) - sqrt(discriminant)) / (4 * (1 + cos(beta)));
	double alpha2 = acos((distanceToCentre - (2 * x) + (alpha*alpha*distanceToCentre)) / (2 * alpha*(distanceToCentre - x)));
	return alpha2;
}

// Affine transformations based on current arguments.
Matrix transformation(information information) {
	double xz = information.alphaY / 57.3;    // Fi/(1Rad)
	double yz = information.alphaX / 57.3;
	double xy = information.alphaZ / 57.3;
	Matrix xzRotationMatrix = Matrix(new double[9]
	{	
		cos(xz),  0, sin(xz),
		0,        1, 0,
		-sin(xz), 0, cos(xz)
	});
	Matrix yzRotationMatrix = Matrix(new double[9]
	{
		1, 0,        0,
		0, cos(yz),  sin(yz),
		0, -sin(yz), cos(yz)
	});
	Matrix xyRotationMatrix = Matrix(new double[9]
	{
		cos(xy),  sin(xy), 0,
		-sin(xy), cos(xy), 0,
		0,        0,       1
	});
	Matrix scalingMatrix = Matrix(new double[9]
	{
		information.size, 0, 0,
		0, information.size, 0,
		0, 0, information.size
	});
	return xzRotationMatrix.multiply(yzRotationMatrix).multiply(xyRotationMatrix).multiply(scalingMatrix);
}

// Part of program which makes a query to functions required to finally draw figure.
void draw(SDL_Surface *s, information information)
{
	for (int i = 0; i < 640; i++)
		for (int w = 0; w < 480; w++)	{put_pixel32(s, i, w, RGB(0,0,0));}
	
	matrixTransformation = transformation(information);
	
	int color = RGB(170, 120, 90), sides = information.sidesCount;

	double alphaCurrent = 0, distanceToCentre = 224;
	double alpha = calculateAngle(sides, distanceToCentre, sin(information.angle));
	for (int i = 1; i <= amount; i++)
	{
		drawFigure(distanceToCentre, alphaCurrent, color, sides, s);
		alphaCurrent +=  alpha;
		distanceToCentre = distanceToCentre*sin(information.angle);
	}
}
