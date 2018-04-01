#include <emscripten/bind.h>

using namespace emscripten;

int fun(int a) {
	return a*2;
}

EMSCRIPTEN_BINDINGS(my_module) {
	function("fun", &fun);
}
