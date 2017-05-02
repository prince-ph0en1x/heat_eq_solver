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

template <int n> // n-Dimension Boundary Value Problem
class Heat
{
	private:
	
	// Matrix <double> M;
	
	public:
	
	Heat (double alpha, int m, double dt)
	{
		double c = alpha*dt*((m+1)*(m+1));
		int Dkij, points = pow(m,n);
		//std::cout << Dkij << " Hi " << std::endl;
		
		for (auto i = 0; i < points; i++)
		{
			for (auto j = 0; j < points; j++)
			{
				float MM = 0;		
				if (i == j)
				{
					MM += 1 - c*(-2)*n;
				}
				else
				{
					Dkij = 0;
					for (auto k = 0; k < n; k++)
					{
						int jj = j/pow(m,k+1);
						int ii = i/pow(m,k+1);
						//std::cout << k << " Hi " << std::endl;
						
						if (j == i+pow(m,k) &&  jj == ii) 
						{
							//std::cout << " Node : " << i << " < " << j << " in Dim " << k << std::endl;
							Dkij = 1;
						}
						if (j == i-pow(m,k) && jj == ii)
						{
							//std::cout << " Node : " << i << " > " << j << " in Dim " << k << std::endl;
							Dkij = 1;
						}
						
					}
					MM += -c*Dkij;
				}
				std::cout << i << " - " << " - " << MM << std::endl;
				
			}
			
		}
	
	
	}
	
};
