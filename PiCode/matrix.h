
#include "globals.h"

#define MATACC(m,x,y) (m.Mat[y*m.Width+x])

typedef struct
{
	int Width;
	int Height;
	double *Mat;
} Matrix;

Matrix CreateMatrix(int width, int height);
void DestroyMatrix(Matrix mat);

Matrix MatrixAdd(Matrix mat1, Matrix mat2);
Matrix MatrixMul(Matrix mat1, Matrix mat2);
Matrix Matrix3x3Inv(Matrix mat);

void ProjectiveTransformation(
	double sx1, double sy1, double sx2, double sy2, double sx3, double sy3, double sx4, double sy4,
	double dx1, double dy1, double dx2, double dy2, double dx3, double dy3, double dx4, double dy4,
	double sx, double sy, double *dx, double *dy);

void PrintMatrix(Matrix mat);

