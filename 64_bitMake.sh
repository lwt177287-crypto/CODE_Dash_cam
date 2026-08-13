rm ./build/*  -rf
cd build
cmake .. -DCMAKE_C_COMPILER=arm-linux-gcc
make -j4
scp main root@192.168.32.69:/root/shiyan/CODE
cd ..