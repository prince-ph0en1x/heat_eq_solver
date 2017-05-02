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
	

#include "part1.cpp"
#include "sparseMatrix.cpp"	// part 2
#include "part3.cpp"
#include "part4.cpp"

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
	
	 Heat<1> h(0.3125,3,0.1);
	// Heat<2> h(0.3125,3,0.1);
	
    return 0;
}
