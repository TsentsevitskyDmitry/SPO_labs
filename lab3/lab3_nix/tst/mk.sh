#! /bin/bash
g++ controller.cpp extprocess.cpp esemaphore.cpp -std=c++0x -o controller_e

g++ child.cpp extprocess.cpp esemaphore.cpp -std=c++0x -o child_e
