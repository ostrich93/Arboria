#ifndef COMMANDSYSTEM_H
#define COMMANDSYSTEM_H

#include "CommandArgs.h"
#include "../containers/Queue.h"

namespace Arboria {
	typedef void (*cmdFunction)(CommandArgs& args);

	typedef struct commandDefinition {
		commandDefinition* next;
		char* name;
		cmdFunction function;
		int flags;
		char* description;
	};

	class CommandSystem {
	private:
		Queue<char*> commandQueue;

	public:
		CommandSystem() {};
		~CommandSystem() = default;
		const char* getCommand() { return commandQueue.front(); }
		void pushCommand(char* cmd) { commandQueue.push(cmd); }
		void popCommand() { commandQueue.pop(); }
		void clearCommandQueue() { commandQueue.clear(); }
	};

	extern CommandSystem* commandSystem;
}

#endif