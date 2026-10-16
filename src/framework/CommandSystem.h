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
		Queue<const char*> commandQueue;

	public:
		CommandSystem() {};
		~CommandSystem() = default;
		const char* getCommand() {
			if (commandQueue.isEmpty()) return NULL;
			return commandQueue.front();
		}
		void pushCommand(const char* cmd) { commandQueue.push(cmd); }
		void popCommand() { commandQueue.pop(); }
		void clearCommandQueue() { commandQueue.clear(); }
	};

	extern CommandSystem* commandSystem;
}

#endif