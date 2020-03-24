#! /bin/bash
cd parent
g++ parent.cpp Menu.cpp extprocess.cpp VendingMachine.cpp -o parent_e
mv ./parent_e ../parent_e

cd ..
cd child
g++ child.cpp Storage.cpp -o child_e
mv ./child_e ../child_e
