rm ./CMakeFiles -rf

export CC=/usr/bin/clang
export CXX=/usr/bin/clang
cmake .
make

CC=/usr/bin/clang CXX=/usr/bin/clang rm ./CMakeFiles -rf && cmake .