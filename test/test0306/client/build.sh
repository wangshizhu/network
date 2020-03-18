rm -rf ./build
rm -rf ./bin
mkdir build
cmake ./ -B ./build
cd build
make