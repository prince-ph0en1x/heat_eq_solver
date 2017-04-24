# Force CMake version 3.1 or above
cmake_minimum_required (VERSION 3.1)

# This project has the name: WI4771_Final
project (WI4771_Final)

# Create an executable named 'heatEqnSolver' from the source file 'heatEqnSolver.cxx'
add_executable(heatEqnSolver src/heatEqnSolver.cxx)
target_compile_features(heatEqnSolver PRIVATE cxx_auto_type)

# C++11: Automatic function return type
target_compile_features(heatEqnSolver PRIVATE cxx_trailing_return_types)

# C++14: Deduced return types
target_compile_features(heatEqnSolver PRIVATE cxx_return_type_deduction)
