rm ./build/*  -rf
cd build
cmake .. -DCMAKE_C_COMPILER=arm-linux-gcc
make 
scp main root@192.168.32.69:/root/shiyan/CODE
cd ..