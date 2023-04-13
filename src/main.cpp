#include "framework/Engine.h"
//#include "framework/String.h"
#include <cstdio>
using namespace Arboria;

int main(int argc, char* argv[]) {

	Engine* engine = new Engine();
	if (_isQuit)
		return 0;
	engine->initializeDisplay();
	engine->run();
	return 0;
}