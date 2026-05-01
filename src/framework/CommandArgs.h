#ifndef COMMANDARGS_H
#define COMMANDARGS_H

namespace Arboria {
	class CommandArgs {
	public:
		CommandArgs() : argc(0) {}
		CommandArgs(const char* text, bool keepAsString) { tokenizeString(text, keepAsString); }

		void operator=(const CommandArgs& args);

		int getArgc() const { return argc; }
		const char* getArgv(int arg) const { return (arg >= 0 && arg < argc) ? argv[arg] : ""; }

		void appendArg(const char* text);
		void clear() { argc = 0; }
		const char** getArgs(int* _argc);
		void tokenizeString(const char* text, bool keepAsString);
	private:
		static const int MAX_COMMAND_ARGS = 64;
		static const int MAX_COMMAND_STRING = 2048;

		int argc; //number of arguments
		char* argv[MAX_COMMAND_ARGS];
		char tokenized[MAX_COMMAND_STRING];
	};
}

#endif