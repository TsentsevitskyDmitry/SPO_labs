#! /bin/bash
cd parent
cp ./../libs/extprocess.cpp ./extprocess.cpp 
cp ./../libs/esemaphore.cpp ./esemaphore.cpp 
cp ./../libs/extprocess.h ./extprocess.h 
cp ./../libs/esemaphore.h ./esemaphore.h 
g++ parent.cpp extprocess.cpp esemaphore.cpp -std=c++0x -o parent_e
mv ./parent_e ../parent_e

cd ..
cd child
cp ./../libs/extprocess.cpp ./extprocess.cpp 
cp ./../libs/esemaphore.cpp ./esemaphore.cpp 
cp ./../libs/extprocess.h ./extprocess.h 
cp ./../libs/esemaphore.h ./esemaphore.h 
g++ child.cpp extprocess.cpp esemaphore.cpp -std=c++0x -o child_e
mv ./child_e ../child_e

cd ..
cd controller
cp ./../libs/extprocess.cpp ./extprocess.cpp 
cp ./../libs/esemaphore.cpp ./esemaphore.cpp 
cp ./../libs/extprocess.h ./extprocess.h 
cp ./../libs/esemaphore.h ./esemaphore.h 
g++ controller.cpp extprocess.cpp esemaphore.cpp -std=c++0x -o controller_e
mv ./controller_e ../controller_e