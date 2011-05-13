#include "includes.h"
#include "Sepstream.h"
sepstream::sepstream(const std::string &source, char seperator) : tokens(source), sep(seperator)
{
		last_starting_position = n = tokens.begin();
}

bool sepstream::GetToken(std::string &token)
{
		std::string::iterator lsp = last_starting_position;
		while (n != tokens.end())
		{
			if (*n == sep || n + 1 == tokens.end())
			{
				last_starting_position = n + 1;
				token = std::string(lsp, n + 1 == tokens.end() ? n + 1 : n);
				
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

const std::string sepstream::GetRemaining()
{
		return std::string(n, tokens.end());
}
bool sepstream::StreamEnd()
{
		return n == tokens.end();
}