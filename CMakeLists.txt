cmake_minimum_required(VERSION 3.21)
project(bank_sysstem)

set(CMAKE_CXX_STANDARD 14)

add_executable(bank_sysstem main.cpp classes.h functions.cpp sqlite3.c sqlite3.h)
