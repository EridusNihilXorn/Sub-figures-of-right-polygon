#pragma once     //To be connected only once.
struct Vector
{
	double x;
	double y;
	double z;
	Vector() = default;                          //Default constuctor.
	Vector(double x, double y, double z) {
		(*this).x = x;
		(*this).y = y;
		(*this).z = z;
	}
};
struct Matrix
{
	double values[9];
	Matrix() = default;
	Matrix(double values[9]) 
	{
		for (int i = 0; i < 9; i++)
		{
			(*this).values[i] = values[i];
		}
	}
	Vector transform(Vector object) {
		return Vector(
			object.x * values[0] + object.y * values[3] + object.z * values[6],
			object.x * values[1] + object.y * values[4] + object.z * values[7],
			object.x * values[2] + object.y * values[5] + object.z * values[8]
		);
	}
	 Matrix multiply(Matrix object) 
	 {
		double result[9];
		for (int i = 0; i < 9; i++)
			result[i] = 0;
		for (int row = 0; row < 3; row++) {
			for (int column = 0; column < 3; column++) {
				for (int i = 0; i < 3; i++) {
					result[row * 3 + column] +=	values[row * 3 + i] * object.values[i * 3 + column];
				}
			}
		}
		return Matrix(result);
	}
};
struct information
{
	double alphaY, alphaX, alphaZ, angle, size;
	int sidesCount;
	information(double angleY, double angleX, double angleZ, double angle, double size, int sides)
	{
		(*this).alphaX = angleX;
		(*this).alphaY = angleY;
		(*this).alphaZ = angleZ;
		(*this).angle = angle;
		(*this).size = size;
		(*this).sidesCount = sides;
	}
	~information() {}   //Destructor.
};

