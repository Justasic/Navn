/* Navn IRC bot -- Flux::string class
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */
#pragma once
#ifndef FLUX_H
#define FLUX_H
#include <cstring>
#include <vector>
#include <set>
#include <map>
#include <algorithm> //For std::transform for Flux::string.tolower() and toupper()
#include <cctype>
#include <sstream>
#include <cstdio>
#include <iomanip>
#include <memory>
#include <bitset>
#include <ios>
#include <typeinfo>
#include "config.h" /* we include the config header from ./configure */

/* Deprecation crap */
#ifdef __GNUC__
# define DEPRECATED(func) func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
# define DEPRECATED(func) __declspec(deprecated) func
#else
# pragma message("WARNING: You need to implement DEPRECATED for this compiler")
# define DEPRECATED(func) func
#endif
/* stupid windows class crap for the Modules */
#ifdef _WIN32
# define CoreExport __declspec(dllimport)
# define DllExport __declspec(dllexport)
#else
# define CoreExport
# define DllExport
#endif
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > sstr;
extern CoreExport bool protocoldebug;

/**
 * \fn template<typename T, typename V> inline T value_cast(const V &y)
 * \brief A casting function which will attempt to safely cast a value into another, then force it if it fails. this cast is different from other casting operators in that it doesnt accept a pointer.
 * \param type-name What it should be casted as
 * \param expression What's to be casted
 * \return the casted value
 */
template<typename T, typename V> inline T value_cast(const V &y)
{
  std::stringstream stream; //Try safe casting with a stringstream.
  T x;
  if(!(stream << std::setprecision(800) << y)) //we use setprecision so scientific notation does not get in the way.
    throw;

  if(!(stream >> x)) //If stringstream fails, force the cast.
    x = *reinterpret_cast<T*>(const_cast<V*>(&(y)));

  return x;
}

namespace Flux
{
 class string;
 typedef std::vector<string> vector;
 Flux::string ConcatinateVector(const vector &p);
}

extern CoreExport std::vector<Flux::string> ParamitizeString(const Flux::string &src, char delim);
Flux::string CondenseString(const std::vector<Flux::string> &p);
/** Case insensitive map, ASCII rules.
 * That is;
 * [ != {, but A == a.
 */
unsigned const char ascii_case_insensitive_map[256] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,                                   /* 0-19 */
	20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,                         /* 20-39 */
	40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,                         /* 40-59 */
	60, 61, 62, 63, 64, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,             /* 60-79 */
	112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 91, 92, 93, 94, 95, 96, 97, 98, 99,              /* 80-99 */
	100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119,     /* 100-119 */
	120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139,     /* 120-139 */
	140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159,     /* 140-159 */
	160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179,     /* 160-179 */
	180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199,     /* 180-199 */
	200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219,     /* 200-219 */
	220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,     /* 220-239 */
	240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255                          /* 240-255 */
};
/** Percent-encoding map for HTML output **/
static const char* url_escape_table[256] = {
  "%00", "%01", "%02", "%03", "%04", "%05", "%06", "%07", "%08", "%09",
  "%0a", "%0b", "%0c", "%0d", "%0e", "%0f", "%10", "%11", "%12", "%13",
  "%14", "%15", "%16", "%17", "%18", "%19", "%1a", "%1b", "%1c", "%1d",
  "%1e", "%1f", "%20", "%21", "%22", "%23", "%24", "%25", "%26", "%27",
  0, 0, "%2a", "%2b", "%2c", "%2d", 0, "%2f", 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, "%3a", "%3b", "%3c", "%3d", "%3e", "%3f", "%40", 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  "%5b", "%5c", "%5d", "%5e", 0, "%60", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "%7b", "%7c", "%7d", 0,
  "%7f", "%80", "%81", "%82", "%83", "%84", "%85", "%86", "%87", "%88",
  "%89", "%8a", "%8b", "%8c", "%8d", "%8e", "%8f","%90", "%91", "%92",
  "%93", "%94", "%95", "%96", "%97", "%98", "%99","%9a", "%9b", "%9c",
  "%9d", "%9e", "%9f", "%a0", "%a1", "%a2", "%a3", "%a4", "%a5", "%a6",
  "%a7", "%a8", "%a9", "%aa", "%ab", "%ac", "%ad", "%ae", "%af", "%b0",
  "%b1", "%b2", "%b3", "%b4", "%b5", "%b6", "%b7", "%b8", "%b9", "%ba",
  "%bb", "%bc", "%bd", "%be", "%bf", "%c0", "%c1", "%c2", "%c3", "%c4",
  "%c5", "%c6", "%c7", "%c8", "%c9", "%ca", "%cb", "%cc", "%cd", "%ce",
  "%cf", "%d0", "%d1", "%d2", "%d3", "%d4", "%d5", "%d6", "%d7", "%d8",
  "%d9", "%da", "%db", "%dc", "%dd", "%de", "%df", "%e0", "%e1", "%e2",
  "%e3", "%e4", "%e5", "%e6", "%e7", "%e8", "%e9", "%ea", "%eb", "%ec",
  "%ed", "%ee", "%ef", "%f0", "%f1", "%f2", "%f3", "%f4", "%f5", "%f6",
  "%f7", "%f8", "%f9", "%fa", "%fb", "%fc", "%fd", "%fe", "%ff"
};

/** The ci namespace contains a number of helper classes.
 */
namespace ci
{
	/** The ci_char_traits class is used for ASCII-style comparison of strings.
	 * This class is used to implement ci::string, a case-insensitive, ASCII-
	 * comparing string class.
	 */
	struct ci_char_traits : std::char_traits<char>
	{
		/** Check if two chars match.
		 * @param c1st First character
		 * @param c2nd Second character
		 * @return true if the characters are equal
		 */
		static bool eq(char c1st, char c2nd);

		/** Check if two chars do NOT match.
		 * @param c1st First character
		 * @param c2nd Second character
		 * @return true if the characters are unequal
		 */
		static bool ne(char c1st, char c2nd);

		/** Check if one char is less than another.
		 * @param c1st First character
		 * @param c2nd Second character
		 * @return true if c1st is less than c2nd
		 */
		static bool lt(char c1st, char c2nd);

		/** Compare two strings of size n.
		 * @param str1 First string
		 * @param str2 Second string
		 * @param n Length to compare to
		 * @return similar to strcmp, zero for equal, less than zero for str1
		 * being less and greater than zero for str1 being greater than str2.
		 */
		static int compare(const char *str1, const char *str2, size_t n);

		/** Find a char within a string up to position n.
		 * @param s1 String to find in
		 * @param n Position to search up to
		 * @param c Character to search for
		 * @return Pointer to the first occurance of c in s1
		 */
		static const char *find(const char *s1, int n, char c);
	};

	/** This typedef declares ci::string based upon ci_char_traits.
	 */
	typedef std::basic_string<char, ci_char_traits, std::allocator<char> > string;

	struct less
	{
	  /** Compare two Flux::strings as ci::strings and find which one is less
	   * @param s1 The first string
	   * @param s2 The second string
	   * @return true if s1 < s2, else false
	   */
	  bool operator()(const Flux::string &s1, const Flux::string &s2) const;
	};
}

/** Operator >> for ci::string
 */
inline std::istream &operator>>(std::istream &is, ci::string &str)
{
	sstr tmp;
	is >> tmp;
	str = tmp.c_str();
	return is;
}

/** Define operators for + and == with ci::string to sstr for easy assignment
 * and comparison
 *
 * Operator +
 */
inline sstr operator+(sstr &leftval, ci::string &rightval)
{
  return leftval + sstr(rightval.c_str());
}

/** Define operators for + and == with ci::string to sstr for easy assignment
 * and comparison
 *
 * Operator +
 */
inline ci::string operator+(ci::string &leftval, sstr &rightval)
{
	return leftval + ci::string(rightval.c_str());
}

/** Define operators for + and == with ci::string to sstr for easy assignment
 * and comparison
 *
 * Operator ==
 */
inline bool operator==(const sstr &leftval, const ci::string &rightval)
{
	return leftval.c_str() == rightval;
}

/** Define operators for + and == with ci::string to sstr for easy assignment
 * and comparison
 *
 * Operator ==
 */
inline bool operator==(const ci::string &leftval, const sstr &rightval)
{
	return leftval == rightval.c_str();
}

/** Define operators != for ci::string to sstr for easy comparison
 */
inline bool operator!=(const ci::string &leftval, const sstr &rightval)
{
	return !(leftval == rightval.c_str());
}

/** Define operators != for ci::string to irc::string for easy comparison
 */
inline bool operator!=(const sstr &leftval, const ci::string &rightval)
{
	return !(leftval.c_str() == rightval);
}

/**
 *\file flux.h Flux::string header file
 *\brief The Flux::string namespace ported from Anope::string
 *\author Justasic and Anope
 * This class has all the inline string functions we needed
 * to make the string class a little bit more functioning.
 * most function programming for the string class goes to The Anope development team but
 * a lot (casting operator for example) is done by Justasic
 */
namespace Flux
{
  /** This class was ported from Anope IRC services to work with Navn
   * most function programming credit goes to The Anope Team
   * See http://tinyurl.com/6btqne4
   *
   * All casting functions are written by Justasic
   */
  class string
  {
  private:
    sstr _string;
  public:
    typedef sstr::iterator iterator;
    typedef sstr::const_iterator const_iterator;
    typedef sstr::reverse_iterator reverse_iterator;
    typedef sstr::const_reverse_iterator const_reverse_iterator;
    typedef sstr::size_type size_type;
    static const size_type npos = static_cast<size_type>(-1);

    string() : _string("") { }
    explicit string(float f) : _string() { _string = value_cast<sstr>(f); }
    explicit string(double d) : _string() { _string = value_cast<sstr>(d); }
    explicit string(int i) : _string() { _string = value_cast<sstr>(i); }
    string(char chr) : _string() { _string = chr; }
    string(const Flux::vector &params) : _string() { _string = value_cast<sstr>(ConcatinateVector(params)); }
    string(const char *_str) : _string(_str) { }
    string(const sstr &_str) : _string(_str) { }
    string(const ci::string &_str) : _string(_str.c_str()) { }
    string(size_type n, char chr) : _string(n, chr) { }
    string(const string &_str, size_type pos = 0, size_type n = npos) : _string(_str._string, pos, n) { }
    template <class InputIterator> string(InputIterator first, InputIterator last) : _string(first, last) { }

    inline string &operator=(char chr) { this->_string = chr; return *this; }
    inline string &operator=(const Flux::vector &params) { this->_string = value_cast<sstr>(ConcatinateVector(params)); return *this; }
    inline string &operator=(const char *_str) { this->_string = _str; return *this; }
    inline string &operator=(const sstr &_str) { this->_string = _str; return *this; }
    inline string &operator=(const ci::string &_str) { this->_string = _str.c_str(); return *this; }
    inline string &operator=(const string &_str) { if (this != &_str) this->_string = _str._string; return *this; }

    inline bool operator==(const char *_str) const { return this->_string == _str; }
    inline bool operator==(const sstr &_str) const { return this->_string == _str; }
    inline bool operator==(const ci::string &_str) const { return ci::string(this->_string.c_str()) == _str; }
    inline bool operator==(const string &_str) const { return this->_string == _str._string; }

    inline bool equals_cs(const char *_str) const { return this->_string == _str; }
    inline bool equals_cs(const sstr &_str) const { return this->_string == _str; }
    inline bool equals_cs(const ci::string &_str) const { return this->_string == _str.c_str(); }
    inline bool equals_cs(const string &_str) const { return this->_string == _str._string; }

    inline bool equals_ci(const char *_str) const { return ci::string(this->_string.c_str()) == _str; }
    inline bool equals_ci(const sstr &_str) const { return ci::string(this->_string.c_str()) == _str.c_str(); }
    inline bool equals_ci(const ci::string &_str) const { return _str == this->_string.c_str(); }
    inline bool equals_ci(const string &_str) const { return ci::string(this->_string.c_str()) == _str._string.c_str(); }

    inline bool operator!=(const char *_str) const { return !operator==(_str); }
    inline bool operator!=(const sstr &_str) const { return !operator==(_str); }
    inline bool operator!=(const ci::string &_str) const { return !operator==(_str); }
    inline bool operator!=(const string &_str) const { return !operator==(_str); }

    inline string &operator+=(char chr) { this->_string += chr; return *this; }
    inline string &operator+=(const Flux::vector &params) { this->_string += value_cast<sstr>(ConcatinateVector(params)); return *this; }
    inline string &operator+=(const char *_str) { this->_string += _str; return *this; }
    inline string &operator+=(const sstr &_str) { this->_string += _str; return *this; }
    inline string &operator+=(const ci::string &_str) { this->_string += _str.c_str(); return *this; }
    inline string &operator+=(const string &_str) { if (this != &_str) this->_string += _str._string; return *this; }

    inline const string operator+(char chr) const { return string(*this) += chr; }
    inline const string operator+(const Flux::vector &params) const { return string(*this) += ConcatinateVector(params); }
    inline const string operator+(const char *_str) const { return string(*this) += _str; }
    inline const string operator+(const sstr &_str) const { return string(*this) += _str; }
    inline const string operator+(const ci::string &_str) const { return string(*this) += _str; }
    inline const string operator+(const string &_str) const { return string(*this) += _str; }

    friend const string operator+(char chr, const string &str);
    friend const string operator+(const Flux::vector &params);
    friend const string operator+(const char *_str, const string &str);
    friend const string operator+(const sstr &_str, const string &str);
    friend const string operator+(const ci::string &_str, const string &str);
    friend const string operator+(const string &str, const sstr &_str);

    inline bool operator<(const string &_str) const { return this->_string < _str._string; }

    inline const char *c_str() const { return this->_string.c_str(); }
    inline const char *data() const { return this->_string.data(); }
    inline ci::string ci_str() const { return ci::string(this->_string.c_str()); }
    inline const sstr &std_str() const { return this->_string; }
    inline sstr &std_str() { return this->_string; }
    inline string url_str() const
    {
      string ret;
      const char *t = this->_string.c_str();
      while(t && *t)
      {
	int c = *t;
	const char *e = url_escape_table[c];
	if(e)
	  ret += e;
	else
	  ret += c;
	t++;
      }
      return ret;
    }

    inline bool empty() const { return this->_string.empty(); }
    inline size_type length() const { return this->_string.length(); }
    inline size_type size() const { return this->_string.size(); }
    inline size_type capacity() const { return this->_string.capacity(); }
    inline size_type max_size() const { return this->_string.max_size(); }
    inline void swap(string &_str) { this->_string.swap(_str._string); }
    inline void push_back(char c) { return this->_string.push_back(c); }
    inline void push_back(const string &_str) { if (this != &_str) this->_string += _str._string; }
    inline void resize(size_type n) { return this->_string.resize(n); }

    inline string erase(size_t pos = 0, size_t n = sstr::npos) { return this->_string.erase(pos, n); }
    inline iterator erase(const iterator &i) { return this->_string.erase(i); }
    inline iterator erase(const iterator &first, const iterator &last) { return this->_string.erase(first, last); }
    //inline void erase(size_type pos = 0, size_type n = sstr::npos) { this->_string.erase(pos, n); }

    inline void trim()
    {
     while(!this->_string.empty() && isspace(this->_string[0]))
       this->_string.erase(this->_string.begin());
     while(!this->_string.empty() && isspace(this->_string[this->_string.length() - 1]))
       this->_string.erase(this->_string.length() - 1);
    }

    inline void tolower() { std::transform(_string.begin(), _string.end(), _string.begin(), ::tolower); }
    inline void toupper() { std::transform(_string.begin(), _string.end(), _string.begin(), ::toupper); }
    inline string capwords() const
    {
      std::vector<Flux::string> params = ParamitizeString(_string, ' ');
      for(std::vector<Flux::string>::iterator it = params.begin(); it != params.end(); ++it)
	(*it)[0] = ::toupper((*it)[0]);
      return CondenseString(params);
    }

    inline void clear() { this->_string.clear(); }

    inline bool search(const string &_str) { if(_string.find(_str._string) != sstr::npos) return true; return false; }
    inline bool search(const string &_str) const { if(_string.find(_str._string) != sstr::npos) return true; return false; }
    inline bool search_ci(const string &_str) { if(ci::string(this->_string.c_str()).find(ci::string(_str.c_str())) != ci::string::npos) return true; return false; }
    inline bool search_ci(const string &_str) const { if(ci::string(this->_string.c_str()).find(ci::string(_str.c_str())) != ci::string::npos) return true; return false; }

    inline size_type find(const string &_str, size_type pos = 0) const { return this->_string.find(_str._string, pos); }
    inline size_type find(char chr, size_type pos = 0) const { return this->_string.find(chr, pos); }
    inline size_type find_ci(const string &_str, size_type pos = 0) const { return ci::string(this->_string.c_str()).find(ci::string(_str._string.c_str()), pos); }
    inline size_type find_ci(char chr, size_type pos = 0) const { return ci::string(this->_string.c_str()).find(chr, pos); }

    inline size_type rfind(const string &_str, size_type pos = npos) const { return this->_string.rfind(_str._string, pos); }
    inline size_type rfind(char chr, size_type pos = npos) const { return this->_string.rfind(chr, pos); }
    inline size_type rfind_ci(const string &_str, size_type pos = npos) const { return ci::string(this->_string.c_str()).rfind(ci::string(_str._string.c_str()), pos); }
    inline size_type rfind_ci(char chr, size_type pos = npos) const { return ci::string(this->_string.c_str()).rfind(chr, pos); }

    inline size_type find_first_of(const string &_str, size_type pos = 0) const { return this->_string.find_first_of(_str._string, pos); }
    inline size_type find_first_of_ci(const string &_str, size_type pos = 0) const { return ci::string(this->_string.c_str()).find_first_of(ci::string(_str._string.c_str()), pos); }

    inline size_type find_first_not_of(const string &_str, size_type pos = 0) const { return this->_string.find_first_not_of(_str._string, pos); }
    inline size_type find_first_not_of_ci(const string &_str, size_type pos = 0) const { return ci::string(this->_string.c_str()).find_first_not_of(ci::string(_str._string.c_str()), pos); }

    inline size_type find_last_of(const string &_str, size_type pos = npos) const { return this->_string.find_last_of(_str._string, pos); }
    inline size_type find_last_of_ci(const string &_str, size_type pos = npos) const { return ci::string(this->_string.c_str()).find_last_of(ci::string(_str._string.c_str()), pos); }

    inline size_type find_last_not_of(const string &_str, size_type pos = npos) const { return this->_string.find_last_not_of(_str._string, pos); }
    inline size_type find_last_not_of_ci(const string &_str, size_type pos = npos) const { return ci::string(this->_string.c_str()).find_last_not_of(ci::string(_str._string.c_str()), pos); }

    inline bool is_number_only() const { return this->find_first_not_of("0123456789.-") == npos; }
    inline bool is_pos_number_only() const { return this->find_first_not_of("0123456789.") == npos; }

    inline string replace(size_type pos, size_type n, const string &_str) { return string(this->_string.replace(pos, n, _str._string)); }
    inline string replace(size_type pos, size_type n, const string &_str, size_type pos1, size_type n1) { return string(this->_string.replace(pos, n, _str._string, pos1, n1)); }
    inline string replace(size_type pos, size_type n, size_type n1, char chr) { return string(this->_string.replace(pos, n, n1, chr)); }
    inline string replace(iterator first, iterator last, const string &_str) { return string(this->_string.replace(first, last, _str._string)); }

    inline string append(const string &_str) { return this->_string.append(_str._string); }
    inline string append(const string &_str, size_t pos, size_t n) { return this->_string.append(_str._string, pos, n); }
    inline string append(const char* s, size_t n) { return this->_string.append(s, n); }
    inline string append(const char* s) { return this->_string.append(s); }
    inline string append(size_t n, char c) { return this->_string.append(n, c); }

    inline int compare(const string &_str) const { return this->_string.compare(_str._string); }
    inline int compare(const char *s) const { return this->_string.compare(s); }
    inline int compare(size_t pos1, size_t n1, const string &_str) const { return this->_string.compare(pos1, n1, _str._string); }
    inline int compare(size_t pos1, size_t n1, const char *s) const { return this->_string.compare(pos1, n1, s); }
    inline int compare(size_t pos1, size_t n1, const string &_str, size_t pos2, size_t n2) const { return this->_string.compare(pos1, n1, _str._string, pos2, n2); }
    inline int compare(size_t pos1, size_t n1, const char *s, size_t n2) const { return this->_string.compare(pos1, n1, s, n2); }

    inline string insert(size_t pos1, const string &_str) { return this->_string.insert(pos1, _str._string); }
    inline string insert(size_t pos1, const string &_str, size_t pos2, size_t n) { return this->_string.insert(pos1, _str._string, pos2, n); }
    inline string insert(size_t pos1, const char* s, size_t n) { return this->_string.insert(pos1, s, n); }
    inline string insert(size_t pos1, const char* s) { return this->_string.insert(pos1, s); }
    inline string insert(size_t pos1, size_t n, char c) { return this->_string.insert(pos1, n, c); }
    inline iterator insert(iterator p, char c) { return this->_string.insert(p, c); }
    inline void insert(iterator p, size_t n, char c) { return this->_string.insert(p, n, c); }
    template<class InputIterator> inline void insert(iterator p, InputIterator first, InputIterator last) { return this->_string.insert(p, first, last); }

    inline string assign(const string &str) { return this->_string.assign(str._string); }
    inline string assign(const Flux::vector &params) { return this->_string.assign(value_cast<sstr>(ConcatinateVector(params))); }
    inline string assign(const string &str, size_t pos, size_t n) { return this->_string.assign(str._string, pos, n); }
    inline string assign(const char* s, size_t n) { return this->_string.assign(s, n); }
    inline string assign(const char* s) { return this->_string.assign(s); }
    inline string assign(size_t n, char c) { return this->_string.assign(n, c); }
    template <class InputIterator> inline string assign(InputIterator first, InputIterator last) { return this->_string.assign(first, last); }

    inline string replace(iterator first, iterator last, size_type n, char chr) { return string(this->_string.replace(first, last, n, chr)); }
    template <class InputIterator> inline string replace(iterator first, iterator last, InputIterator f, InputIterator l) { return string(this->_string.replace(first, last, f, l)); }
    inline string replace_all_cs(const string &_orig, const string &_repl)
	{
		Flux::string new_string = *this;
		size_type pos = new_string.find(_orig), orig_length = _orig.length(), repl_length = _repl.length();
		while (pos != npos)
		{
			new_string = new_string.substr(0, pos) + _repl + new_string.substr(pos + orig_length);
			pos = new_string.find(_orig, pos + repl_length);
		}
		return new_string;
	}
     inline string replace_all_ci(const string &_orig, const string &_repl)
	{
		Flux::string new_string = *this;
		size_type pos = new_string.find_ci(_orig), orig_length = _orig.length(), repl_length = _repl.length();
		while (pos != npos)
		{
			new_string = new_string.substr(0, pos) + _repl + new_string.substr(pos + orig_length);
			pos = new_string.find_ci(_orig, pos + repl_length);
		}
		return new_string;
	}
    inline string substr(size_type pos = 0, size_type n = npos) const { return string(this->_string.substr(pos, n)); }

    inline iterator begin() { return this->_string.begin(); }
    inline const_iterator begin() const { return this->_string.begin(); }
    inline iterator end() { return this->_string.end(); }
    inline const_iterator end() const { return this->_string.end(); }
    inline reverse_iterator rbegin() { return this->_string.rbegin(); }
    inline const_reverse_iterator rbegin() const { return this->_string.rbegin(); }
    inline reverse_iterator rend() { return this->_string.rend(); }
    inline const_reverse_iterator rend() const { return this->_string.rend(); }

    inline const char &at(size_t pos) const { return this->_string.at(pos); }
    inline char &at(size_t pos) { return this->_string.at(pos); }
    inline std::allocator<char> get_allocator() const { return this->_string.get_allocator(); }
    inline char &operator[](size_type n) { return this->_string[n]; }
    inline const char &operator[](size_type n) const { return this->_string[n]; }

    inline string isolate(char b, char e) const
    {
      string to_find;
      size_type pos = _string.find(b);
      pos += 1;
      for (unsigned i = pos; i < _string.length(); i++)
      {
	if (_string[i] == e)
	  break;
	else
	  to_find = to_find+_string[i];
      }
      return to_find;
    }

    /* Strip Return chars */
    inline string strip()
    {
	string new_buf = *this;
	new_buf = new_buf.replace_all_cs("\n", "");
	new_buf = new_buf.replace_all_cs("\r", "");
	return new_buf;
    }
    /* Strip specific chars */
    inline string strip(const char &_delim)
    {
	string new_buf = *this;
	new_buf = new_buf.replace_all_cs(_delim, "");
	return new_buf;
    }
    //Transform from Text to Base64
    inline string b64encode()
    {
      static const string Base64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
      static const char Pad64 = '=';
      string output, src = _string;
      size_t src_pos = 0, src_len = src.length();
      unsigned char input[3];

      while (src_len - src_pos > 2)
      {
	input[0] = src[src_pos++];
	input[1] = src[src_pos++];
	input[2] = src[src_pos++];

	output += Base64[input[0] >> 2];
	output += Base64[((input[0] & 0x03) << 4) + (input[1] >> 4)];
	output += Base64[((input[1] & 0x0f) << 2) + (input[2] >> 6)];
	output += Base64[input[2] & 0x3f];
      }
      /* Now we worry about padding */
      if (src_pos != src_len)
      {
	input[0] = input[1] = input[2] = 0;
	for (size_t i = 0; i < src_len - src_pos; ++i)
		input[i] = src[src_pos + i];

	output += Base64[input[0] >> 2];
	output += Base64[((input[0] & 0x03) << 4) + (input[1] >> 4)];
	if (src_pos == src_len - 1)
		output += Pad64;
	else
		output += Base64[((input[1] & 0x0f) << 2) + (input[2] >> 6)];
	output += Pad64;
      }
      return output;
    }
    //Transform from Base64 to Human readable text
    inline string b64decode()
    {
      static const string Base64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
      static const char Pad64 = '=';
      string output, src = _string;
      unsigned state = 0;
      string::const_iterator ch = src.begin(), send = src.end();
      for (; ch != send; ++ch)
      {
	if (isspace(*ch)) /* Skip whitespace anywhere */
	  continue;

	if (*ch == Pad64)
	  break;

	size_t pos = Base64.find(*ch);
	if (pos == string::npos) /* A non-base64 character */
	  return "";
	switch (state)
	{
	  case 0:
	    output += pos << 2;
	    state = 1;
	    break;
	  case 1:
	    output[output.length() - 1] |= pos >> 4;
	    output += (pos & 0x0f) << 4;
	    state = 2;
	    break;
	  case 2:
	    output[output.length() - 1] |= pos >> 2;
	    output += (pos & 0x03) << 6;
	    state = 3;
	    break;
	  case 3:
	    output[output.length() - 1] |= pos;
	    state = 0;
	}
      }
      if (!output[output.length() - 1])
	output.erase(output.length() - 1);
      return output;
    }
    //Transform into Hexidecimal text string
    inline string hex_str()
    {
      const char hextable[] = "0123456789ABCDEF";

      size_t l = _string.length();
      string rv;
      for (size_t i = 0; i < l; ++i)
      {
	unsigned char c = _string[i];
	rv += hextable[c >> 4];
	rv += hextable[c & 0xF];
      }
      return rv;
    }
    //Transform into human readable text from hexidecimal
    inline string unhex()
    {
      size_t len = _string.length();
	string rv;
	for (size_t i = 0; i < len; i += 2)
	{
	  char h = _string[i], l = _string[i + 1];
	  unsigned char byte = (h >= 'a' ? h - 'a' + 10 : h - '0') << 4;
	  byte += (l >= 'a' ? l - 'a' + 10 : l - '0');
	  rv += byte;
	}
	return rv;
    }
    //Transform into binary equivalent of the string
    inline string bin_str()
    {
      string bin;
      for(unsigned j = 0; j < _string.size(); ++j){
	bin += std::bitset<10>(_string[j]).to_string();
	bin += ' ';
      }
      bin.trim();
      return bin;
    }
    /* Cast into an integer */
    inline operator int() { return value_cast<int>(this->_string); }
    /* Cast into a float */
    inline operator float() { return value_cast<float>(this->_string); }
    /* Cast into a double */
    inline operator double() { return value_cast<double>(this->_string); }
    /* Cast into a long integer */
    inline operator long() { return value_cast<long>(this->_string); }
    /* Cast into a unsigned integer */
    inline operator unsigned() { return value_cast<unsigned>(this->_string); }

    friend std::ostream &operator<<(std::ostream &os, const string &_str);
    friend std::istream &operator>>(std::istream &os, string &_str);
  }; //end of string class

  template<typename T> class map : public std::map<string, T> { };
  template<typename T> class insensitive_map : public std::map<string, T, ci::less> { };
  extern CoreExport Flux::string Sanitize(const Flux::string&);
  extern CoreExport Flux::string RandomString(size_t);
  extern CoreExport Flux::string RandomNickString(size_t);
  inline std::ostream &operator<<(std::ostream &os, const string &_str) { return os << _str._string; }
  inline std::istream &operator>>(std::istream &os, string &_str) { return os >> _str._string; }
  inline const string operator+(char chr, const string &str) { string tmp(chr); tmp += str; return tmp; }
//   inline const string operator+(const Flux::vector &params) { string tmp; temp += ConcatinateVector(params); return tmp; }
  inline const string operator+(const char *_str, const string &str) { string tmp(_str); tmp += str; return tmp; }
  inline const string operator+(const sstr &_str, const string &str) { string tmp(_str); tmp += str; return tmp; }
  inline const string operator+(const ci::string &_str, const string &str) { string tmp(_str); tmp += str; return tmp; }
}//end of namespace

class CoreExport sepstream
{
 private:
  Flux::string tokens;
  Flux::string::iterator last_starting_position;
  Flux::string::iterator n;
  char sep;
 public:
  sepstream(const Flux::string &source, char seperator);
  virtual bool GetToken(Flux::string &token);
  virtual const Flux::string GetRemaining();
  virtual bool StreamEnd();
};

class dynamic_reference_base;
class CoreExport Base
{
  std::set<dynamic_reference_base*> References;
public:
  Base();
  virtual ~Base();
  void AddReference(dynamic_reference_base*);
  void DelReference(dynamic_reference_base*);
};

class dynamic_reference_base : public Base
{
protected:
  bool invalid;
public:
  dynamic_reference_base() : invalid(false) {}
  virtual ~dynamic_reference_base() {}
  inline void Invalidate() { this->invalid = true; }
};
#endif
