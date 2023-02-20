#include "globals.h"
using namespace Arboria;

int main(int argc, char* argv[]) {

	engine = new Engine();
	engine->init();
	engine->run();
	return 0;
}