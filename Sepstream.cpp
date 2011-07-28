#include "flux.h"
/**
 *\file  Sepstream.cpp 
 *\brief Contains the Seperator Stream class.
 */
sepstream::sepstream(const Flux::string &source, char seperator) : tokens(source), sep(seperator)
{
		last_starting_position = n = tokens.begin();
}

bool sepstream::GetToken(Flux::string &token)
{
		Flux::string::iterator lsp = last_starting_position;
		while (n != tokens.end())
		{
			if (*n == sep || n + 1 == tokens.end())
			{
				last_starting_position = n + 1;
				token = Flux::string(lsp, n + 1 == tokens.end() ? n + 1 : n);
				
				while (token.length() && token.rfind(sep) == token.length() - 1)
                                 token.erase(token.end() - 1);

				++n;
				
				return true;
			}
			++n;
		}
		token.clear();
		return false;
}
const Flux::string sepstream::GetRemaining()
{
		return Flux::string(n, tokens.end());
}
bool sepstream::StreamEnd()
{
		return n == tokens.end();
}