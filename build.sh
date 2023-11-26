if [ ! -d "/Bin/" ];then
  mkdir /Bin
fi
if [ ! -d "/Build/" ];then
  mkdir /Build
fi
cd Build
cmake ..
make -j4
cd ../Bin
./main