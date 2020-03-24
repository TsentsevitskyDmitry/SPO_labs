#! /bin/bash
cd parent
g++ parent.cpp -std=c++0x Menu.cpp extprocess.cpp VendingMachine.cpp -o parent_e
mv ./parent_e ../parent_e

cd ..
cd child
g++ child.cpp -std=c++0x Storage.cpp -o child_e
mv ./child_e ../child_e

cd ..
cd controller
g++ main.cpp -std=c++0x extprocess.cpp -o controller_e
mv ./controller_e ../controller_e