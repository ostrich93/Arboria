#include "Lexer.h"
#include "CVariableSystem.h"
#include "CommandArgs.h"

namespace Arboria {
	void CommandArgs::operator=(const CommandArgs& args)
	{
		int i;

		argc = args.argc;
		memcpy(tokenized, args.tokenized, MAX_COMMAND_STRING);
		for (i = 0; i < argc; i++) {
			argv[i] = tokenized + (args.argv[i] - args.tokenized);
		}
	}
	void CommandArgs::appendArg(const char* text)
	{
		if (!argc) {
			argc = 1;
			argv[0] = tokenized;
			String::copyN(tokenized, text, sizeof(tokenized));
		}
		else {
			argv[argc] = argv[argc - 1] + strlen(argv[argc-1]) + 1;
			String::copyN(argv[argc - 1], text, sizeof(tokenized) - (argv[argc] - tokenized));
			argc++;
		}
	}

	const char** CommandArgs::getArgs(int* _argc)
	{
		*_argc = argc;
		return (const char**)&argv[0];
	}

	void CommandArgs::tokenizeString(const char* text, bool keepAsString)
	{
		Lexer lex;
		Token token, number;
		int len, totalLength;

		argc = 0;

		if (!text)
			return;

		lex.loadMemory(text, strlen(text), "Lexer::tokenizeString");
		lex.setFlags(LEXER_NOERRORS | LEXER_NOWARNINGS | LEXER_NOSTRINGCONCATS | LEXER_NOSTRINGESCAPEDCHARS | (keepAsString ? LEXER_ONLYSTRINGS : 0));

		totalLength = 0;

		while (1) {
			if (argc == MAX_COMMAND_ARGS)
				return;

			if (!lex.readToken(&token))
				return;

			if (!keepAsString && token == "-") {
				if (lex.checkTokenType(TOKENTYPE_NUMBER, 0, &number)) {
					token = "-" + number;
				}
			}

			if (token == "$") {
				if (!lex.readToken(&token)) {
					return;
				}
				if (cVariableSystem) {
					token = cVariableSystem->getVariableString(token.c_str());
				}
				else {
					token = "<unknown>";
				}
			}

			len = token.length();

			if (totalLength + len + 1 > sizeof(tokenized))
				return;

			argv[argc] = tokenized + totalLength;
			argc++;

			String::copyN(tokenized + totalLength, token.c_str(), sizeof(tokenized) - totalLength);

			totalLength += len + 1;
		}
	}
}