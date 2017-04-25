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

template<typename T>
class Vector {
private:
    T* data;
    int length;
    
public:
    // Default constructor
    Vector()
    : data(nullptr), length(0) 
    {
        std::cout << "Constructor 1" << std::endl;
        }	
	
    // Constructor (sets length and allocates data)
    Vector(int n)
    : data(new T[n]), length(n)
    {
        std::cout << "Constructor 2" << std::endl;
        }

    // Copy Constructor
    explicit Vector(const Vector& v)
    : Vector(v.length)
    {
        for (auto i=0; i<v.length; i++)
            data[i] = v.data[i];
        std::cout << "Constructor 3" << std::endl;
        }
        
    // Constructor (using initializer list)
    Vector(std::initializer_list<T> list)
    : Vector((int)list.size())
    {
        std::uninitialized_copy(list.begin(), list.end(), data);
        std::cout << "Constructor 4" << std::endl;
    }
    
    // Destructor
    ~Vector()
    {
        length = 0;
        delete[] data;
    }

    // Copy assignment
    Vector& operator=(const Vector& other)
    {
        if (this != &other)
            {
                delete[] data;
                length = other.length;
                data   = new int(other.length);
                for (auto i=0; i<other.length; i++)
                    data[i] = other.data[i];
            }
        std::cout << "Copy Assignment" << std::endl;
        return *this;
    }

    // Move assignment
    Vector& operator=(Vector&& other)
    {
        if (this != &other)
            {
                delete[] data;
                length = other.length;
                data   = other.data;
                other.length = 0;
                other.data   = nullptr;
            }
        std::cout << "Move Assignment" << std::endl;
        return *this;
    }
    
    // Pointwise sum operator
    Vector& operator+(const Vector &v) //const
    {
        assert(length == v.length); 
        //Vector vnew(length);
        for (auto i = 0; i < length; i++)
                data[i] += v.data[i];
                //vnew.data[i] = data[i] + v.data[i];
        std::cout << "Pointwise Sum" << std::endl;
        return *this;
        //return vnew;
        }

    // Pointwise difference operator
    Vector& operator-(const Vector &v)
    {
        assert(length == v.length); 
        for (auto i = 0; i < length; i++)
                data[i] -= v.data[i];
        std::cout << "Pointwise Difference" << std::endl;
        return *this;
        }

    // Multiply with a scalar
    Vector& operator*(T scalar) 
    {
        //Vector<int> vn;
        for (auto i=0; i<length; i++)
          //  vn.data[i] = data[i] * scalar;
            data[i] *= scalar;
        return *this;
        }
    
    friend Vector& operator*(T scalar, Vector& rhs)
    {
    	return rhs*scalar;
        }
    
    template<typename S>
    friend S dot(const Vector<S>& l, const Vector<S>& r)
    {
        assert(l.length == r.length); 
        S d = 0;
        for (auto i=0; i<l.length; i++)
            d += l.data[i]*r.data[i];
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
    
};

// The global main function that is the designated start of the program
int main(){

    std::cout << "Proj Kickoff 1" << std::endl;
    
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
    std::cout << dot(v1,v2) << std::endl;

    return 0;
}
