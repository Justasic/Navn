/* Navn IRC bot -- Separation Stream Functions
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 *
 * Code in this file was imported from Anope IRC Services
 */

#include "flux.h"
#include "log.h"
#include "extern.h"
#include <new>

/**
 * \file  Sepstream.cpp
 * \brief Contains the Seperator Stream class.
 * This class allows for strings to be split into substrings
 * based on the charactor used to split those strings
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

/******************************************************************************/

/* The following code was ported from anope, all credits goes to the Anope Team
 *
 * This is an implementation of two special string classes:
 *
 * irc::string which is a case-insensitive equivalent to
 * std::string which is not only case-insensitive but
 * can also do scandanavian comparisons, e.g. { = [, etc.
 *
 * ci::string which is a case-insensitive equivalent to
 * std::string.
 *
 * These classes depend on ascii_case_insensitive_map
 *
 */


bool ci::ci_char_traits::eq(char c1st, char c2nd)
{
	return ascii_case_insensitive_map[static_cast<unsigned char>(c1st)] == ascii_case_insensitive_map[static_cast<unsigned char>(c2nd)];
}

bool ci::ci_char_traits::ne(char c1st, char c2nd)
{
	return ascii_case_insensitive_map[static_cast<unsigned char>(c1st)] != ascii_case_insensitive_map[static_cast<unsigned char>(c2nd)];
}

bool ci::ci_char_traits::lt(char c1st, char c2nd)
{
	return ascii_case_insensitive_map[static_cast<unsigned char>(c1st)] < ascii_case_insensitive_map[static_cast<unsigned char>(c2nd)];
}

int ci::ci_char_traits::compare(const char *str1, const char *str2, size_t n)
{
	for (unsigned i = 0; i < n; ++i)
	{
		if (ascii_case_insensitive_map[static_cast<unsigned char>(*str1)] > ascii_case_insensitive_map[static_cast<unsigned char>(*str2)])
			return 1;

		if (ascii_case_insensitive_map[static_cast<unsigned char>(*str1)] < ascii_case_insensitive_map[static_cast<unsigned char>(*str2)])
			return -1;

		if (!*str1 || !*str2)
			return 0;

		++str1;
		++str2;
	}
	return 0;
}

const char *ci::ci_char_traits::find(const char *s1, int n, char c)
{
	while (n-- > 0 && ascii_case_insensitive_map[static_cast<unsigned char>(*s1)] != ascii_case_insensitive_map[static_cast<unsigned char>(c)])
		++s1;
	return n >= 0 ? s1 : NULL;
}

/** Compare two Flux::strings as ci::strings and find which one is less
 * @param s1 The first string
 * @param s2 The second string
 * @return true if s1 < s2, else false
 */
bool ci::less::operator()(const Flux::string &s1, const Flux::string &s2) const
{
	return s1.ci_str().compare(s2.ci_str()) < 0;
}

std::vector<Base*> BaseReferences;

Base::Base()
{
//   Log(LOG_MEMORY) << "Base::+ @" << this;
  BaseReferences.push_back(this);
}

Base::~Base()
{
//   Log(LOG_MEMORY) << "Base::- @" << this;
  for(std::set<dynamic_reference_base*>::iterator it = this->References.begin(), it_end = this->References.end(); it != it_end; ++it)
    (*it)->Invalidate();

  for(std::vector<Base*>::iterator it = BaseReferences.begin(), it_end = BaseReferences.end(); it != it_end; ++it)
    if((*it) == this)
      BaseReferences.erase(it);
}

Flux::string CalculateByteSizes(size_t bytes)
{
  Flux::string sizes[] = { "Bytes", "KB", "MB", "GB", "TB" };
  double number = 0;
  int i;
  for(i = 0; static_cast<int>(bytes / 1024) > 0; i++, bytes /= 1024)
    number = bytes / 1024.0;

  return printfify("%f %s", static_cast<float>(number), sizes[i].c_str());
}

void ReportMemUsage()
{
  Log(LOG_MEMORY) << "Total memory allocated: " << CalculateByteSizes(memused);
  Log(LOG_MEMORY) << "Total memory allocated in bytes: " << memused;
  Log(LOG_MEMORY) << "Total objects allocated: " << memobjects;
  Log(LOG_MEMORY) << "Calls to new: " << newcalls;
  Log(LOG_MEMORY) << "Calls to delete: " << deletecalls;
}

// Overload the new and delete operators so we can track what's going on
void operator delete (void *ptr) throw ()
{
  if(!ptr)
  {
    std::cout << "[MEMORY] Warning: deletion of empty pointer!" << std::endl;
    return;
  }

  int *p = reinterpret_cast<int*>(ptr);
  memobjects--;
  deletecalls++;
  memused -= *p;

  free(ptr);
}

void * operator new (size_t size) throw (std::bad_alloc)
{
  // It is not safe to use Log() or any other function which uses strings
  // because the STL uses the new operator which will cause a infinite loop
  memused += size;
  memobjects++;
  newcalls++;
  void *ptr = malloc(size);

  if(!ptr)
  {
    std::cout << "[MEMORY] Unable to allocate " << size << " bytes!!";
    throw std::bad_alloc();
  }

  return ptr;
}




void Base::AddReference(dynamic_reference_base *r) { this->References.insert(r); }
void Base::DelReference(dynamic_reference_base *r) { this->References.erase(r); }
