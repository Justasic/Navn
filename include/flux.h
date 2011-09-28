/* All code is licensed under GNU General Public License GPL v3 (http://www.gnu.org/licenses/gpl.html) */
#ifndef FLUX_H
#define FLUX_H
#include <string>
#include <cstring>
#include <vector>
#include <set>
#include <map>
#include <algorithm> //For std::transform for Flux::string.tolower() and toupper()
#include <cctype>
#include <sstream>
#include <cstdio>
#include "config.h" /* we include the config header from ./configure */
#ifdef __GNUC__
#define DEPRECATED(func) func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED(func) __declspec(deprecated) func
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED(func) func
#endif
namespace Flux{
 class string; 
}
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

	/** Used to hash ci::strings for unordered_map
	 */
	struct hash
	{
		/* VS 2008 specific code */
		enum { bucket_size = 4, min_buckets = 8 };
		bool operator()(const Flux::string &s1, const Flux::string &s2) const;
		/* End VS 2008 specific code */

		/** Hash a ci::string for unordered_map
		 * @param s The string
		 * @return A hash value for the string
		 */
		size_t operator()(const ci::string &s) const;
		size_t operator()(const Flux::string &s) const;
	};
}

namespace std
{
	/** An overload for std::equal_to<ci::string> that uses Flux::string, passed for the fourth temmplate
	 * argument for unordered_map
	 */
	template<> struct equal_to<ci::string>
	{
	 public:
		/** Compare two Flux::strings as ci::strings
		 * @paarm s1 The first string
		 * @param s2 The second string
		 * @return true if they are equal
		 */
		bool operator()(const Flux::string &s1, const Flux::string &s2) const;
	};

	/** An overload for std::less<ci::string> that uses Flux::string, passed for the third template argument
	 * to std::map and std::multimap
	 */
	template<> struct less<ci::string>
	{
	 public:
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
	std::string tmp;
	is >> tmp;
	str = tmp.c_str();
	return is;
}

/* Define operators for + and == with ci::string to std::string for easy assignment
 * and comparison
 *
 * Operator +
 */
inline std::string operator+(std::string &leftval, ci::string &rightval)
{
	return leftval + std::string(rightval.c_str());
}

/* Define operators for + and == with ci::string to std::string for easy assignment
 * and comparison
 *
 * Operator +
 */
inline ci::string operator+(ci::string &leftval, std::string &rightval)
{
	return leftval + ci::string(rightval.c_str());
}

/* Define operators for + and == with ci::string to std::string for easy assignment
 * and comparison
 *
 * Operator ==
 */
inline bool operator==(const std::string &leftval, const ci::string &rightval)
{
	return leftval.c_str() == rightval;
}

/* Define operators for + and == with ci::string to std::string for easy assignment
 * and comparison
 *
 * Operator ==
 */
inline bool operator==(const ci::string &leftval, const std::string &rightval)
{
	return leftval == rightval.c_str();
}

/* Define operators != for ci::string to std::string for easy comparison
 */
inline bool operator!=(const ci::string &leftval, const std::string &rightval)
{
	return !(leftval == rightval.c_str());
}

/* Define operators != for ci::string to irc::string for easy comparison
 */
inline bool operator!=(const std::string &leftval, const ci::string &rightval)
{
	return !(leftval.c_str() == rightval);
}

/**
 *\file flux.h Flux::string header file
 *\brief The Flux::string namespace ported from Anope::string
 *\author Justasic and Anope
 * This class has all the inline string functions we needed
 * to make the string class a little bit more functioning.
 * most function programming for the string class goes to The Anope development team
 */
namespace Flux{
  /* This class was ported from Anope IRC services to work with Navn
   * most function programming credit goes to The Anope Team
   * See http://tinyurl.com/6btqne4
   */
  class string
  {
  private:
    std::string _string;
  public:
    typedef std::string::iterator iterator;
    typedef std::string::const_iterator const_iterator;
    typedef std::string::reverse_iterator reverse_iterator;
    typedef std::string::const_reverse_iterator const_reverse_iterator;
    typedef std::string::size_type size_type;
    static const size_type npos = static_cast<size_type>(-1);
    
    string() : _string("") { }
    string(char chr) : _string() { _string = chr; }
    string(size_type n, char chr) : _string(n, chr) { }
    string(const char *_str) : _string(_str) { }
    string(const std::string &_str) : _string(_str) { }
    string(const ci::string &_str) : _string(_str.c_str()) { }
    string(const string &_str, size_type pos = 0, size_type n = npos) : _string(_str._string, pos, n) { }
    template <class InputIterator> string(InputIterator first, InputIterator last) : _string(first, last) { }
    
    inline string &operator=(char chr) { this->_string = chr; return *this; }
    inline string &operator=(const char *_str) { this->_string = _str; return *this; }
    inline string &operator=(const std::string &_str) { this->_string = _str; return *this; }
    inline string &operator=(const ci::string &_str) { this->_string = _str.c_str(); return *this; }
    inline string &operator=(const string &_str) { if (this != &_str) this->_string = _str._string; return *this; }
    
    inline bool operator==(const char *_str) const { return this->_string == _str; }
    inline bool operator==(const std::string &_str) const { return this->_string == _str; }
    inline bool operator==(const ci::string &_str) const { return ci::string(this->_string.c_str()) == _str; }
    inline bool operator==(const string &_str) const { return this->_string == _str._string; }
    
    inline bool equals_cs(const char *_str) const { return this->_string == _str; }
    inline bool equals_cs(const std::string &_str) const { return this->_string == _str; }
    inline bool equals_cs(const ci::string &_str) const { return this->_string == _str.c_str(); }
    inline bool equals_cs(const string &_str) const { return this->_string == _str._string; }
    
    inline bool equals_ci(const char *_str) const { return ci::string(this->_string.c_str()) == _str; }
    inline bool equals_ci(const std::string &_str) const { return ci::string(this->_string.c_str()) == _str.c_str(); }
    inline bool equals_ci(const ci::string &_str) const { return _str == this->_string.c_str(); }
    inline bool equals_ci(const string &_str) const { return ci::string(this->_string.c_str()) == _str._string.c_str(); }
    
    inline bool operator!=(const char *_str) const { return !operator==(_str); }
    inline bool operator!=(const std::string &_str) const { return !operator==(_str); }
    inline bool operator!=(const ci::string &_str) const { return !operator==(_str); }
    inline bool operator!=(const string &_str) const { return !operator==(_str); }
    
    inline string &operator+=(char chr) { this->_string += chr; return *this; }
    inline string &operator+=(const char *_str) { this->_string += _str; return *this; }
    inline string &operator+=(const std::string &_str) { this->_string += _str; return *this; }
    inline string &operator+=(const ci::string &_str) { this->_string += _str.c_str(); return *this; }
    inline string &operator+=(const string &_str) { if (this != &_str) this->_string += _str._string; return *this; }
    
    inline const string operator+(char chr) const { return string(*this) += chr; }
    inline const string operator+(const char *_str) const { return string(*this) += _str; }
    inline const string operator+(const std::string &_str) const { return string(*this) += _str; }
    inline const string operator+(const ci::string &_str) const { return string(*this) += _str; }
    inline const string operator+(const string &_str) const { return string(*this) += _str; }
    
    friend const string operator+(char chr, const string &str);
    friend const string operator+(const char *_str, const string &str);
    friend const string operator+(const std::string &_str, const string &str);
    friend const string operator+(const ci::string &_str, const string &str);
    friend const string operator+(const string &str, const std::string &_str);
    
    inline bool operator<(const string &_str) const { return this->_string < _str._string; }
    
    inline const char *c_str() const { return this->_string.c_str(); }
    inline ci::string ci_str() const { return ci::string(this->_string.c_str()); }
    inline const std::string &std_str() const { return this->_string; }
    inline std::string &std_str() { return this->_string; }
    
    inline bool empty() const { return this->_string.empty(); }
    inline size_type length() const { return this->_string.length(); }
    inline size_type size() const { return this->_string.size(); }
    inline size_type capacity() const { return this->_string.capacity(); }
    inline void push_back(char c) { return this->_string.push_back(c); }
    inline void push_back(string &_str) { if (this != &_str) this->_string += _str._string; }
    inline void resize(size_type n) { return this->_string.resize(n); }
    inline void AddSpace() { return this->_string.push_back(' '); }
    
    inline string erase(size_t pos = 0, size_t n = std::string::npos) { return this->_string.erase(pos, n); }
    inline iterator erase(const iterator &i) { return this->_string.erase(i); }
    inline iterator erase(const iterator &first, const iterator &last) { return this->_string.erase(first, last); }
    //inline void erase(size_type pos = 0, size_type n = std::string::npos) { this->_string.erase(pos, n); }
    
    inline void trim()
    {
     while(!this->_string.empty() && isspace(this->_string[0]))
       this->_string.erase(this->_string.begin());
     while(!this->_string.empty() && isspace(this->_string[this->_string.length() - 1]))
       this->_string.erase(this->_string.length() - 1);
    }
    inline void tolower() { std::transform(_string.begin(), _string.end(), _string.begin(), ::tolower); }
    inline void toupper() { std::transform(_string.begin(), _string.end(), _string.begin(), ::toupper); }
    
    inline void clear() { this->_string.clear(); }
    
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
    
    inline string assign(const string &str) { return this->_string.assign(str._string); }
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
    
    inline char &operator[](size_type n) { return this->_string[n]; }
    inline const char &operator[](size_type n) const { return this->_string[n]; }
    
    inline string strip()
    { 
	char c = (*this)[(*this).size() - 1];
	while (c == '\n' || c == '\r'){
		(*this).erase((*this).end() - 1);
		c = (*this)[(*this).size() - 1];
	}
	return (*this);
    }
    inline string strip(const char &_str)
    {
	char c = (*this)[(*this).size() - 1];
	while ((c = _str)){
		(*this).erase((*this).end() - 1);
		c = (*this)[(*this).size() - 1];
	}
	return (*this);
    }
    
    friend std::ostream &operator<<(std::ostream &os, const string &_str);
    }; //end of string class
    template<typename T> class map : public std::map<string, T> { };
    template<typename T> class insensitive_map : public std::map<string, T, std::less<ci::string> > { };
    extern Flux::string Sanitize(const Flux::string&);
    inline std::ostream &operator<<(std::ostream &os, const string &_str) { return os << _str._string; }
    inline const string operator+(char chr, const string &str) { string tmp(chr); tmp += str; return tmp; }
    inline const string operator+(const char *_str, const string &str) { string tmp(_str); tmp += str; return tmp; }
    inline const string operator+(const ci::string &_str, const string &str) { string tmp(_str); tmp += str; return tmp; }
    
    template<typename T> inline Flux::string stringify(const T &x){
	std::ostringstream stream;
	if(!(stream << x))
		throw;
	return stream.str();
    }
    struct hash
	{
		/* VS 2008 specific code */
		enum { bucket_size = 4, min_buckets = 8 };
		bool operator()(const string &s1, const string &s2) const;
		/* End of 2008 specific code */

		/** Hash an Anope::string for unordered_map
		 * @param s The string
		 * @return A hash value for the string
		 */
		bool operator()(const string &s) const;
	};
    
}//end of namespace
class sepstream {
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
#endif