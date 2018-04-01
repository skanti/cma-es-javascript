INCLUDES:=/home/amon/software/emsdk/emscripten/1.37.36/system/include
all : CMAES.js

CMAES.js : CMAES.cpp
	emcc --bind -I$(INCLUDES) -o $@ $^

clean:
	rm -f CMAES.js
