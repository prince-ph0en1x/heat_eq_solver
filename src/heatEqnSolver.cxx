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

// Include header file for initializer list
#include <initializer_list>

// Include header file for memory
#include <memory>
	
// Include header file for complex number support
#include <complex>

#include <assert.h>
#include <math.h>

#include "sparseMatrix.h"
#include "sparseMatrix.cpp"

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
	Vector& operator=(Vector&& other)
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
	Vector<typename std::common_type<T, U>::type> operator+(const Vector<U>& b)
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
	Vector<typename std::common_type<T, U>::type> operator-(const Vector<U>& b)
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
	friend Vector<typename std::common_type<T, U>::type> dot(const Vector<T>& l, const Vector<U>& r)
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





    
// The global main function that is the designated start of the program
int main(){

	std::cout << "Proj Kickoff 2" << std::endl;
    
	// Part 1: Vector class
	    
	Vector<int> v1;
	Vector<int> v2(5);
	Vector<int> v3 = { 1, 1, 2, 3, 5, 8 };
	Vector<int> v4(v3);
	v2 = v3;
	v1 = v3;
	v2 = v2 + v3;
	v2 = v1 * 10;
	v2 = 1 * v1;		// correct original vector v1 getting changed
	
	v2.print("v2");
	v1.print("v1");
	// std::cout << dot(v1,v2) << std::endl;
	
	SparseMatrix<int> matrix(4, 5);
	int val;
	matrix.set(-5, 2, 3); // sets -5 on 2nd row and 3rd column
	val = matrix.get(2, 3);
	cout << val <<endl;
	
	// Heat<1> h(0.3125,3,0.1);
	// Heat<2> h(0.3125,3,0.1);
	
    return 0;
}
