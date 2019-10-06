
#include "matrix.h"

Matrix CreateMatrix(int width, int height)
{
	double *mat = malloc(width*height*sizeof(double));
	return (Matrix){.Width=width,.Height=height,.Mat=mat};
}
void DestroyMatrix(Matrix mat)
{
	free(mat.Mat);
}

Matrix MatrixAdd(Matrix mat1, Matrix mat2)
{
	if (mat1.Width != mat2.Width || mat1.Height != mat2.Height) { return (Matrix){}; }
	Matrix out = CreateMatrix(mat1.Width,mat1.Height);

	for (int y = 0; y < mat1.Height; y++)
	{
		for (int x = 0; x < mat1.Width; x++)
		{
			MATACC(out,x,y) = MATACC(mat1,x,y) + MATACC(mat2,x,y);
		}
	}

	return out;
}
Matrix MatrixMul(Matrix mat1, Matrix mat2)
{
	if (mat1.Width != mat2.Height) { return (Matrix){}; }
	Matrix out = CreateMatrix(mat2.Width,mat1.Height);

	for (int y = 0; y < mat1.Height; y++)
	{
		for (int x = 0; x < mat2.Width; x++)
		{
			MATACC(out,x,y) = 0;
			for (int i = 0; i < mat1.Width; i++)
			{
				MATACC(out,x,y) += MATACC(mat1,i,y) * MATACC(mat2,x,i);
			}
		}
	}

	return out;
}
Matrix Matrix3x3Inv(Matrix mat)
{
	double a=MATACC(mat,0,0), b=MATACC(mat,1,0), c=MATACC(mat,2,0), d=MATACC(mat,0,1), e=MATACC(mat,1,1), f=MATACC(mat,2,1), g=MATACC(mat,0,2), h=MATACC(mat,1,2), i=MATACC(mat,2,2);
	Matrix out = CreateMatrix(mat.Width,mat.Height);
	//TL
	MATACC(out,0,0)=(e*i-f*h) / (a*(e*i-f*h) - b*(d*i-f*g) + c*(d*h-e*g));
	//TM
	MATACC(out,1,0)=-(b*i-c*h) / (a*(e*i-f*h) - b*(d*i-f*g) + c*(d*h-e*g));
	//TR
	MATACC(out,2,0)=(b*f-c*e) / (a*(e*i-f*h) - b*(d*i-f*g) + c*(d*h-e*g));

	//ML
	MATACC(out,0,1)=-(d*i-f*g) / (a*(e*i-f*h) - b*(d*i-f*g) + c*(d*h-e*g));
	//MM
	MATACC(out,1,1)=((b*i-c*h)*(d*i-f*g)) / ((a*(e*i-f*h) - b*(d*i-f*g) + c*(d*h-e*g))*(e*i-f*h)) + i/(e*i-f*h);
	//MR
	MATACC(out,2,1)=(-(b*i-c*e)*(d*i-f*g)) / ((a*(e*i-f*h) - b*(d*i-f*g) + c*(d*h-e*g))*(e*i-f*h)) - f/(e*i-f*h);

	//BL
	MATACC(out,0,2)=(d*h-e*g) / (a*(e*i-f*h) - b*(d*i-f*g) + c*(d*h-e*g));
	//BM
	MATACC(out,1,2)=-(a*h-b*g) / (a*(e*i-f*h) - b*(d*i-f*g) + c*(d*h-e*g));
	//BR
	MATACC(out,2,2)=(a*e-b*d) / (a*(e*i-f*h) - b*(d*i-f*g) + c*(d*h-e*g));

	return out;
}

void ProjectiveTransformation(
	double sx1, double sy1, double sx2, double sy2, double sx3, double sy3, double sx4, double sy4,
	double dx1, double dy1, double dx2, double dy2, double dx3, double dy3, double dx4, double dy4,
	double sx, double sy, double *dx, double *dy)
{
	Matrix sls = (Matrix){.Width=3,.Height=3,.Mat=(double[]){sx1,sx2,sx3,sy1,sy2,sy3,1,1,1}};
	Matrix dls = (Matrix){.Width=3,.Height=3,.Mat=(double[]){dx1,dx2,dx3,dy1,dy2,dy3,1,1,1}};


	Matrix slsi = Matrix3x3Inv(sls);
	Matrix shomog = MatrixMul(slsi,(Matrix){.Width=1,.Height=3,.Mat=(double[]){sx4,sy4,1}});
	DestroyMatrix(slsi);
	Matrix dlsi = Matrix3x3Inv(dls);
	Matrix dhomog = MatrixMul(Matrix3x3Inv(dls),(Matrix){.Width=1,.Height=3,.Mat=(double[]){dx4,dy4,1}});
	DestroyMatrix(dlsi);
	

	Matrix A = (Matrix){.Width=3,.Height=3,.Mat=(double[])
		{
			MATACC(shomog,0,0)*sx1,MATACC(shomog,0,1)*sx2,MATACC(shomog,0,2)*sx3,
			MATACC(shomog,0,0)*sy1,MATACC(shomog,0,1)*sy2,MATACC(shomog,0,2)*sy3,
			MATACC(shomog,0,0),MATACC(shomog,0,1),MATACC(shomog,0,2)
		}};
	Matrix B = (Matrix){.Width=3,.Height=3,.Mat=(double[])
		{
			MATACC(dhomog,0,0)*dx1,MATACC(dhomog,0,1)*dx2,MATACC(dhomog,0,2)*dx3,
			MATACC(dhomog,0,0)*dy1,MATACC(dhomog,0,1)*dy2,MATACC(dhomog,0,2)*dy3,
			MATACC(dhomog,0,0),MATACC(dhomog,0,1),MATACC(dhomog,0,2)
		}};

	Matrix Ai = Matrix3x3Inv(A);
	Matrix C = MatrixMul(B, Ai);
	DestroyMatrix(Ai);


	Matrix P = MatrixMul(C, (Matrix){.Width=1,.Height=3,.Mat=(double[]){sx,sy,1}});

	*dx=MATACC(P,0,0)/MATACC(P,0,2);
	*dy=MATACC(P,0,1)/MATACC(P,0,2);

	DestroyMatrix(P);
	DestroyMatrix(C);
}