/**
 * \file heatEqnSolver.cxx
 *
 * This file is part of the course wi4771tu:
 * Object Oriented Scientific Programming with C++
 *
 * \authors Aritra, Irina, Prashanth
 *
 */


#include <iostream>		// Include header file for standard input/output stream library
#include <initializer_list>	// Include header file for initializer list
#include <memory>		// Include header file for memory
#include <assert.h>
#include <math.h>

#include "sparseMatrix.cpp"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Part 1 Beg ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


template <typename T>
class Vector
{
	public:					// private:
	
	T* data;				// public: std::unique_ptr<T> data;
	int length;
    
	public:
	
	// Default constructor
    	Vector()
    	: data(nullptr), length(0) 
    	{
        	// std::cout << "Default Constructor" << std::endl;
        }	
	
    	// Constructor (sets length and allocates data)
    	Vector(int n)
    	: data(new T[n]), length(n)
   	{
        	// std::cout << "Parameterized Constructor" << std::endl;
        }

    	// Copy Constructor
    	Vector(const Vector<T>& v)
    	: Vector(v.length)
    	{
        	for (auto i = 0; i < v.length; i++)
			data[i] = v.data[i];
        	// std::cout << "Copy Constructor" << std::endl;
        }
        
	// Constructor (using initializer list)
    	Vector(std::initializer_list<T> list)
    	: Vector((int)list.size())
	{
		std::uninitialized_copy(list.begin(), list.end(), data);
		// std::cout << "Initialized List Constructor" << std::endl;
	}
    
    	// Destructor
   	~Vector()
	{
		length = 0;
		delete[] data;
	}

	// Copy assignment
	Vector<T>& operator=(const Vector& other)	// INT-DOUBLE
	{
		if (this != &other)
		{
			delete[] data;
			length = other.length;
			data   = new T(other.length);
			for (auto i = 0; i < other.length; i++)
				data[i] = static_cast<T>(other.data[i]);
		}
		// std::cout << "Copy Assignment" << std::endl;
		return *this;
	}

	// Move assignment
	Vector<T>& operator=(Vector&& other)
	{
		if (this != &other)
		{
			delete[] data;
			length = other.length;
			data = other.data;
			other.length = 0;
			other.data = nullptr;
		}
		// std::cout << "Move Assignment" << std::endl;
		return *this;
	}
    
	// Pointwise sum operator
	template<typename U>
	Vector<typename std::common_type<T, U>::type> operator+(const Vector<U>& b) const
	{
		assert(length == b.length);
		Vector<typename std::common_type<T, U>::type> z(length);
		for (int i = 0; i<length; ++i)
			z.data[i] = data[i] + b.data[i];
		std::cout << "Operator+" << std::endl;
		return z;
	}

	// Pointwise difference operator
	template<typename U>
	Vector<typename std::common_type<T, U>::type> operator-(const Vector<U>& b) const
	{
		assert(length == b.length);
		Vector<typename std::common_type<T, U>::type> z(length);
		for (int i = 0; i<length; ++i)
			z.data[i] = data[i] - b.data[i];
		std::cout << "Operator-" << std::endl;
		return z;
	}

	// Inner product function
	template<typename U>
	friend typename std::common_type<T, U>::type dot(const Vector<T>& l, const Vector<U>& r)
	{
		assert(l.length == r.length);
		typename std::common_type<T, U>::type d = 0;
		for (auto i = 0; i<l.length; i++)
			d += l.data[i] * r.data[i];
		std::cout << d << std::endl;
		return d;
	}

	void print(const std::string &info) const
	{
		// print the address of this instance, the attributes `length` and `data` and the `info` string
		std::cout << "  " << this << " " << length << " " << data << "  " << info << "\t";//<< std::endl;
		for (auto i = 0; i < length; i++)
			std::cout << " : " << data[i];
		std::cout << std::endl;
	}
	
	//template<typename <class T>, typename U>
	//friend Vector<typename std::common_type<T, U>::type> operator*(const Vector<T>& a, U scalar);
	//template<typename <class T>, typename U>
	//friend Vector<typename std::common_type<T, U>::type> operator*(U scalar, const Vector<T>& a);
};

// Multiply with a scalar - right hand
template<typename T, typename U>
Vector<typename std::common_type<T, U>::type> operator*(const Vector<T>& a, U scalar)
{
	Vector<typename std::common_type<T, U>::type> z(a.length);
	for (int i = 0; i<a.length; ++i)
		z.data[i] = a.data[i] * scalar;
	// std::cout << "Operator* rhs" << std::endl;
	return z;
}

// Multiply with a scalar - left hand
template<typename T, typename U>
Vector<typename std::common_type<T, U>::type> operator*(U scalar, const Vector<T>& a)
{
    Vector<typename std::common_type<T, U>::type> z(a.length);
    for (int i = 0; i<a.length; ++i)
		z.data[i] = a.data[i] * scalar;
	// std::cout << "Operator* lhs" << std::endl;
	return z;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Part 2 Beg ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// SparseMatrix header

template<typename T>
Vector<T> multiply(const SparseMatrix<T> &A, const Vector<T> & x)
{
	if (A.getColumnCount() != (int) x.length) {
		throw InvalidDimensionsException("Cannot multiply: Matrix column count and vector size don't match.");
	}

	Vector<T> result(A.getRowCount());


		for (int i = 0; i < A.getRowCount(); i++) {
			T sum = T();
			for(auto j=0; j<x.length;j++){
                sum += A.get(i+1,j+1)*x.data[j];
			}
			result.data[i] = sum;
		}


	return result;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Part 3 Beg ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//Conjugate gradient method
template<typename T>
int cg(const SparseMatrix<T> &A, const Vector<T> &b, Vector<T> &x, T tol, int maxiter)
{
    Vector<T> r(x.length), p(x.length), r_1(x.length);
    double alpha_k, beta_k;
    int k;

    r = (b - multiply(A,x));
    p = r;

    for(k=0; k<maxiter; k++)
    {
        alpha_k = dot(r, r) /  dot(multiply(A,p), p);
        x = x + alpha_k*p;
        r_1 = r - multiply(A, alpha_k*p );

        if (dot(r_1, r_1)< tol*tol)
            break;

        beta_k = dot(r_1, r_1)/dot (r,r);
        p = r_1 + beta_k*p;

        r = r_1;
    }

    std::cout << "Numerical solution" << std::endl;
    x.print("x");

    if(k<=maxiter)
        return k+1;
    else
        return -1;

}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Part 4 Beg ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <int n> // n-Dimension Boundary Value Problem
class Heat
{
	public:
	
	SparseMatrix<double> *M;	
	
	public:
	
	Heat (double alpha, int m, double dt)
	{
		double Mij, c = alpha*dt*((m+1)*(m+1));
		int Dkij, points = pow(m,n);
		M = new SparseMatrix<double> (points, points);
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
				M->set(Mij, i+1, j+1);
			}
		}
	}
	
	Vector<double> solve(double alpha, int m, double t_end, double dt) const
	{
		int points = pow(m,n);
		Vector<double> w_ic(points); // initial conditions vector
		Vector<double> w_icc(points); // initial conditions vector

		/*
		// CONSTRUCT IC VECTOR 1D
		for (auto j = 0; j < m; j++)
		{
			w_ic.data[j] = sin(pi*(j + 1) / (m + 1));
		}

		// CONSTRUCT IC VECTOR 2D
		for (auto j = 0; j < m; j++)
		{
			for (auto i = 0; i < m; i++)
			{
				int iter = i + m*j;
				w_ic.data[iter] = sin(pi*(i + 1) / (m + 1))*sin(pi*(j + 1) / (m + 1));
			}
		}
		
		// CONSTRUCT IC VECTOR 3D
		for (auto k = 0; k < m; k++)
		{
			for (auto j = 0; j < m; j++)
			{
				for (auto i = 0; i < m; i++)
				{
					int iter = i + m*j + m*m*k;
					w_ic.data[iter] = sin(pi*(i + 1) / (m + 1))*sin(pi*(j + 1) / (m + 1))*sin(pi*(k + 1) / (m + 1));
				}
			}
		}
		*/
		
		auto j = 0;
		// CONSTRUCT IC VECTOR nD
		for (auto iter = 0; iter < points; iter++)
		{
			w_icc.data[iter] = 1;
			j = 0;
			for (auto i = 1; i <= n; i++)
			{
				j = (iter % (int)pow(m,i) - j) / ((int)pow(m,i-1));
				
				w_icc.data[iter] *= sin(pi*(j + 1) / (m + 1));
			}
		}
		
		w_ic.print("wic");
		w_icc.print("wicc");
		
		return w_icc;
		
		double tol = 0.001;
		int maxiter = 10;
		double t = 0.5;
	
		// Vector<double> w_ic(points); // initial conditions
		Vector<double> w_r(points); // solution vector
		w_r = w_ic; // initial guess = ic
	
        	int nt = t_end / dt; // number of time steps
        	for (auto i = 0; i < nt; i++)
        	{
        	    cg(*M, w_ic, w_r, tol, maxiter);
        	    w_ic = w_r;
        	}
	}

	// Exact solution

	double pi = 3.14159265359;
	
	// nD
	Vector<double> exact(double alpha, int m, double t) const
	{
		int points = pow(m,n);
		Vector<double> soln(points);
		double c = exp(-n*pi*pi*alpha*t); // multiplier
		auto j = 0;
		// CONSTRUCT IC VECTOR nD
		for (auto iter = 0; iter < points; iter++)
		{
			soln.data[iter] = c;
			j = 0;
			for (auto i = 1; i <= n; i++)
			{
				j = (iter % (int)pow(m,i) - j) / ((int)pow(m,i-1));
				
				soln.data[iter] *= sin(pi*(j + 1) / (m + 1));
			}
		}
		
		std::cout << "Exact solution\n" ;
		soln.print("x");
		return soln;
	}
	
	
	/*
	// 1D
	Vector<double> exact(double alpha, int m, double t) const
	{
		Vector<double> exact(m);
		double mult = exp(-n*pi*pi*alpha*t); // multiplier
		for (auto i = 0; i < m; i++)
		{
			double ic = sin(pi*(i + 1) / (m + 1));
			exact.data[i] = ic*mult;
		}

		std::cout << "Exact solution\n" ;
		exact.print("x");
		return exact;
	}
	
	// 2D
	SparseMatrix<double> exact(double alpha, int m, double t) const
	{
		SparseMatrix<double> exact(m, m);
		double mult = exp(-n*pi*pi*alpha*t); // multiplier
		for (auto i = 0; i < m; i++)
		{
			for (auto j = 0; j < m; j++)
			{
				double ic = sin(pi*(i + 1) / (m + 1))*sin(pi*(j + 1) / (m + 1));
				exact.set(ic*mult, i + 1, j + 1);
			}
		}
		
		// Print check
		for (auto i = 0; i < 3; i++)
		{
		for (auto j = 0; j < 3; j++)
		{
			std::cout << exact.get(i+1, j+1) << "\t";
		}
		std::cout << std::endl;
		}
		
		return exact;
	}
	*/
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Part 0 Beg ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// The global main function that is the designated start of the program
int main()
{

	std::cout << "Proj Kickoff 2" << std::endl;
    
	// Part 1: Vector class
	    
	Vector<int> v1;
	Vector<int> v2(5);
	Vector<int> v3 = { 1, 1, 2, 3, 5, 8 };
	Vector<int> v4(v3);
	Vector<int> v5 = { 3, 6, 5, 3, 5, 8 };
	Vector<double> v6 = { 3.2, 6.5, 5, 3, 5, 8 };
	Vector<double> v7;
   	
   	v5.print("v5");
	v2 = v3;
	v2.print("v2");
	v1 = v3;
	v1.print("v1");
	v2 = v5 + v3;
	v2.print("v2");
	v5.print("v5");
	v2 = v1 * 10;
	v2.print("v2");
	v2 = ((int)5.2 * v1);
	v2.print("v2");

	v2 = v5 - v3;
	v2.print("v2");
	v5.print("v5");
	//dot(v1, v3);

	v7 = (v5 + v6);
	v7.print("v7");

	v7 = 2 * v7;
	v7.print("v7");

	Vector<int> v9 = { 1,1,1,1,1 };
	Vector<int> v10(5);
	int a = dot(v9, v9);
	cout << "This is a " << a <<endl;
	
	//SparseMatrix<int> matrix(4, 5);
	//int val;
	//matrix.set(-5, 2, 3); // sets -5 on 2nd row and 3rd column
	//val = matrix.get(2, 3);
	//cout << val <<endl;*/
	
	
	/*
	Vector<double> v11 = {1,2};
    	Vector<double> v12(2);
   	SparseMatrix<double> matrix1(2, 2);
	matrix1.set(4, 1,1);
	matrix1.set(1, 1,2);
	matrix1.set(1, 2,1);
	matrix1.set(3, 2,2);
	for(auto i=1; i<3; i++) {
		for(auto j=1; j<3; j++)
			std::cout << (matrix1.get(i, j))<<"\t";
		std::cout << '\n'; }
	//v12 = multiply(matrix1, v11);
	v12 = matrix1.multiply(v11);
	v12.print("v12");
	double f=1E-10;
	Vector<double> v13={1,2};
	Vector<double> xguess={2,1};
	int iter = cg(matrix1, v13, xguess, f, 3);
	std::cout  << "\nTotal iterations for cg: " << iter << std::endl;
	*/
	
	//Heat<1> h1(0.3125,3,0.1);
	//h1.solve(0.3125,3,0.5,0.1).print("SolveOpt");
	//h1.exact(0.3125,3,0.5); // Exact solution
	
	Heat<2> h2(0.3125,3,0.1);
	h2.solve(0.3125,3,0.5,0.1).print("SolveOpt");
	h2.exact(0.3125,3,0.5); // Exact solution
	
	//Heat<3> h3(0.3125,3,0.1);
	//h3.solve(0.3125,3,0.5,0.1).print("SolveOpt");
	
	// Print check
	for (auto i = 0; i < 3; i++)
	{
		for (auto j = 0; j < 3; j++)
		{
			//std::cout << h1.M->get(i+1, j+1) << "\t";
			//std::cout << h2.M->get(i+1, j+1) << "\t";
			//std::cout << h3.M->get(i+1, j+1) << "\t";
		}
		std::cout << std::endl;
	}
	//std::cout << h1.M << std::endl;
	
    return 0;
}
