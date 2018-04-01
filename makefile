INCLUDES:=-I/home/amon/software/emsdk/emscripten/1.37.36/system/include -I/usr/include/eigen3 -I./src
all : CMAES.js

CMAES.js : CMAES.cpp ./src/Engine.cpp ./src/Parameter.cpp
	em++ --bind $(INCLUDES) -O3 -std=c++14 -DNDEBUG -o $@ $^

clean:
	rm -f CMAES.js
