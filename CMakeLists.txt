cmake_minimum_required(VERSION 3.15)
project(virttech)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

add_executable(virttech main.cpp VM.cpp VM.h code_generator.h interpreters/NaiveVM.cpp interpreters/NaiveVM.h interpreters/IndirectVM.cpp interpreters/IndirectVM.h interpreters/DirectVM.cpp interpreters/DirectVM.h interpreters/DirectSuperVM.cpp interpreters/DirectSuperVM.h code_generation/CodeGeneratorVM.cpp code_generation/CodeGeneratorVM.h code_generation/TranslationUnit.cpp code_generation/TranslationUnit.h code_generation/types.h)