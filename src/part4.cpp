/**
 * \file heatEqnSolver.cxx
 *
 * This file is part of the course wi4771tu:
 * Object Oriented Scientific Programming with C++
 *
 * \authors Aritra, Irina, Prashanth
 *
 */
 
// Include header file for standard input/output stream library
#include <iostream>

#include <math.h>

#include "sparseMatrix.cpp"	// part 2

template <int n> // n-Dimension Boundary Value Problem
class Heat
{
	public:
	
	SparseMatrix<double> M();
	// Matrix <double> M;
	//SparseMatrix<double>* M;
	
	public:
	
	Heat (double alpha, int m, double dt)
	{
		double Mij, c = alpha*dt*((m+1)*(m+1));
		int Dkij, points = pow(m,n);
		SparseMatrix<double> M(points, points);
		
		for (auto i = 0; i < points; i++)
		{
			for (auto j = 0; j < points; j++)
			{
				Mij = 0;		
				if (i == j)
				{
					Mij += 1 - c*(-2)*n;
				}
				else
				{
					Dkij = 0;
					for (auto k = 0; k < n; k++)
					{
						int jj = j/pow(m,k+1);
						int ii = i/pow(m,k+1);
						
						if (j == i+pow(m,k) &&  jj == ii) 
						{
							Dkij = 1;
						}
						if (j == i-pow(m,k) && jj == ii)
						{
							Dkij = 1;
						}
						
					}
					Mij += -c*Dkij;
				}
				M.set(Mij, i+1, j+1);
			}
		}
		
		// Print check
		for (auto i = 0; i < points; i++)
		{
			for (auto j = 0; j < points; j++)
			{
				std::cout << M.get(i+1, j+1) << "\t";
				
			}
			std::cout << std::endl;		
		}
	
	
	}
	
};
