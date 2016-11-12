/* Test mp*_class operators and functions.

Copyright 2001, 2002, 2003 Free Software Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MP Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA. */

#include <iostream>

#include "mpir.h"
#include "mpirxx.h"
#include "gmp-impl.h"
//#include "tests.h"

using namespace std;




#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

#include <complex>
#include <limits>

using std::complex;

using std::cout;
using std::endl;

using namespace std;






template <class T>
class matrix
{
public:
	matrix()
	{

		data = new T[sizeX*sizeY];
	}
	matrix(int M, int N)
	{
		sizeX = M;
		sizeY = N;
		data = new T[M*N];
	}

	~matrix()
	{
		if (out){
			delete out;
			out = 0;
		}
		delete[] data;
		data = 0;
	}

	inline T& GET(int r, int c)
	{
		return data[r*sizeY + c];
	}

	T& operator()(int r, int c)
	{
		return GET(r, c);
	}
	int NumRows()
	{
		return sizeX;
	}
	int NumCols()
	{
		return sizeY;
	}

	matrix& operator*(matrix &b)
	{
		if (this->NumCols() == b.NumRows())
		{
			if (out)delete out;
			out = new matrix(this->NumRows(), b.NumCols());

			for (int i = 0; i < this->NumRows(); i++)
			{
				for (int j = 0; j < b.NumCols(); j++)
				{
					(*out)(i, j) = 0.0;
					for (int k = 0; k < this->NumCols(); k++)
					{
						(*out)(i, j) += GET(i, k) * b(k, j);
					}
				}
			}

			return *out;
		}
		return matrix<T>(0, 0);
	}

	matrix& operator+(matrix<T> &b)
	{
		if (out)delete out;
		out = new matrix(this->NumRows(), this->NumCols());

		for (int i = 0; i < this->NumRows(); i++)
		{
			for (int j = 0; j < this->NumCols(); j++)
			{
				(*out)(i, j) = GET(i, j) + b(i, j);
			}
		}
		return *out;
	}

	void operator=(matrix<T> &b)
	{
		if (sizeX != b.NumRows() || sizeY != b.NumCols())
		{
			sizeX = b.NumRows();
			sizeY = b.NumCols();
			delete []data;
			data = new T[sizeX*sizeY];
		}

		for (int i = 0; i < this->NumRows(); i++)
		{
			for (int j = 0; j < this->NumCols(); j++)
			{
				GET(i, j) = b(i, j);
			}
		}

	}
	
	matrix* out = 0;
	int sizeX = 1;
	int sizeY = 1;
	T *data = 0;
};


int
main (void)
{
	cout << "Setting standard precision to 512 bit" << endl;
  mpf_set_default_prec(512);
  gmp_randclass random_generator(gmp_randinit_default);
  
  matrix<mpf_class> A(5, 5);
  for (int r = 0; r < 5; r++)
  {
	  for (int c = 0; c < 5; c++)
	  {
		  //A(r, c) = f;
		  A(r, c) = random_generator.get_f(512);
	  }
  }
  cout << endl;
  cout << endl;
  cout << "Creating a matrix A" << endl;
  cout << "----------------------------" << endl;
  for (int r = 0; r < 5; r++)
  {
	  for (int c = 0; c < 5; c++)
	  {
		  cout << A(r, c).get_prec() << " ";// = r1.get_f();
	  }
	  cout << endl;
  }

  for (int r = 0; r < 5; r++)
  {
	  for (int c = 0; c < 5; c++)
	  {
		  // mpf_class f(0, 512);
		  // A(r, c) = f;
		  cout << A(r, c) << " ";// = r1.get_f();
	  }
	  cout << endl;
  }

  cout << endl;
  cout << endl;
  cout << "Creating a matrix B" << endl;
  cout << "----------------------------" << endl;

  matrix<mpf_class> B(5, 5);
  for (int r = 0; r < 5; r++)
  {
	  for (int c = 0; c < 5; c++)
	  {
		  //A(r, c) = f;
		  B(r, c) = random_generator.get_f(512);
		  cout << B(r, c) << ",  ";

	  }
	  cout << endl;
  }

  cout << endl;
  cout << endl;
  cout << "Creating a matrix C=A+B" << endl;
  cout << "----------------------------" << endl;
  matrix<mpf_class> C(5, 5);

  C = A + B;

  for (int r = 0; r < 5; r++)
  {
	  for (int c = 0; c < 5; c++)
	  {

		  cout << C(r, c) << " ";
	  }
	  cout << endl;
  }

  cout << endl;
  cout << endl;
  cout << "Creating a matrix D=A*B*A*B*C" << endl;
  cout << "----------------------------" << endl;
  matrix<mpf_class> D;

  D = A*B*A*B*C;
 // D = D*D*D;

  for (int r = 0; r < 5; r++)
  {
	  for (int c = 0; c < 5; c++)
	  {

		  cout << D(r, c) << " ";
	  }
	  cout << endl;
  }


  return 0;
}
