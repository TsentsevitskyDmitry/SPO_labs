#! /bin/bash
cd mThread
g++ parent.cpp child.cpp controller.cpp esync.cpp conio.cpp -pthread -std=c++0x -o thread
mv ./thread ../thread