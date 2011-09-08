#ifndef FLUX_H
#define FLUX_H
#include <string>
#include <cstring>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <cstdio>
#ifdef __GNUC__
#define DEPRECATED(func) func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED(func) __declspec(deprecated) func
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED(func) func
#endif
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
    string(const string &_str, size_type pos = 0, size_type n = npos) : _string(_str._string, pos, n) { }
    template <class InputIterator> string(InputIterator first, InputIterator last) : _string(first, last) { }
    
    inline string &operator=(char chr) { this->_string = chr; return *this; }
    inline string &operator=(const char *_str) { this->_string = _str; return *this; }
    inline string &operator=(const std::string &_str) { this->_string = _str; return *this; }
    inline string &operator=(const string &_str) { if (this != &_str) this->_string = _str._string; return *this; }
    
    inline bool operator==(const char *_str) const { return this->_string == _str; }
    inline bool operator==(const std::string &_str) const { return this->_string == _str; }
    inline bool operator==(const string &_str) const { return this->_string == _str._string; }
    
    inline bool equals_cs(const char *_str) const { return this->_string == _str; }
    inline bool equals_cs(const std::string &_str) const { return this->_string == _str; }
    inline bool equals_cs(const string &_str) const { return this->_string == _str._string; }
    
    inline bool operator!=(const char *_str) const { return !operator==(_str); }
    inline bool operator!=(const std::string &_str) const { return !operator==(_str); }
    inline bool operator!=(const string &_str) const { return !operator==(_str); }
    
    inline string &operator+=(char chr) { this->_string += chr; return *this; }
    inline string &operator+=(const char *_str) { this->_string += _str; return *this; }
    inline string &operator+=(const std::string &_str) { this->_string += _str; return *this; }
    inline string &operator+=(const string &_str) { if (this != &_str) this->_string += _str._string; return *this; }
    
    inline const string operator+(char chr) const { return string(*this) += chr; }
    inline const string operator+(const char *_str) const { return string(*this) += _str; }
    inline const string operator+(const std::string &_str) const { return string(*this) += _str; }
    inline const string operator+(const string &_str) const { return string(*this) += _str; }
    
    friend const string operator+(char chr, const string &str);
    friend const string operator+(const char *_str, const string &str);
    friend const string operator+(const std::string &_str, const string &str);
    friend const string operator+(const string &str, const std::string &_str);
    
    inline bool operator<(const string &_str) const { return this->_string < _str._string; }
    
    inline const char *c_str() const { return this->_string.c_str(); }
    
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
    
    inline std::string &tostd() { return this->_string; }
    inline const std::string &tostd() const { return this->_string; }
    
    inline void clear() { this->_string.clear(); }
    
    inline size_type find(const string &_str, size_type pos = 0) const { return this->_string.find(_str._string, pos); }
    inline size_type find(char chr, size_type pos = 0) const { return this->_string.find(chr, pos); }
    
    inline size_type rfind(const string &_str, size_type pos = npos) const { return this->_string.rfind(_str._string, pos); }
    inline size_type rfind(char chr, size_type pos = npos) const { return this->_string.rfind(chr, pos); }
    
    inline size_type find_first_of(const string &_str, size_type pos = 0) const { return this->_string.find_first_of(_str._string, pos); }
    inline size_type find_first_not_of(const string &_str, size_type pos = 0) const { return this->_string.find_first_not_of(_str._string, pos); }
    inline size_type find_last_of(const string &_str, size_type pos = npos) const { return this->_string.find_last_of(_str._string, pos); }
    inline size_type find_last_not_of(const string &_str, size_type pos = npos) const { return this->_string.find_last_not_of(_str._string, pos); }
    
    inline bool is_number_only() const { return this->find_first_not_of("0123456789.-") == npos; }
    inline bool is_pos_number_only() const { return this->find_first_not_of("0123456789.") == npos; }
    
    inline string replace(size_type pos, size_type n, const string &_str) { return string(this->_string.replace(pos, n, _str._string)); }
    inline string replace(size_type pos, size_type n, const string &_str, size_type pos1, size_type n1) { return string(this->_string.replace(pos, n, _str._string, pos1, n1)); }
    inline string replace(size_type pos, size_type n, size_type n1, char chr) { return string(this->_string.replace(pos, n, n1, chr)); }
    inline string replace(iterator first, iterator last, const string &_str) { return string(this->_string.replace(first, last, _str._string)); }
    
    inline string append(const string &_str) { return this->_string.append(_str._string); }
    //inline string append(const Flux::string &_str) { return this->_string.append(_str); }
    inline string append(const string &_str, size_t pos, size_t n) { return this->_string.append(_str._string, pos, n); }
    inline string append(const char* s, size_t n) { return this->_string.append(s, n); }
    inline string append(const char* s) { return this->_string.append(s); }
    inline string append(size_t n, char c) { return this->_string.append(n, c); }
    
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
    inline string strip(const char _str)
    {
	char c = (*this)[(*this).size() - 1];
	while ((c = _str)){
		(*this).erase((*this).end() - 1);
		c = (*this)[(*this).size() - 1];
	}
	return (*this);
    }
    inline void stripc()
    { 
      Flux::string &newbuf = *this;
      char c = newbuf[newbuf.size() - 1];
      while (c == '\n' || c == '\r'){
	      newbuf.erase(newbuf.end() - 1);
	      c = newbuf[newbuf.size() - 1];
      }
      (*this) == newbuf;
    }
    inline void stripc(const char _chr)
    {
      Flux::string &newbuf = *this;
     char c = newbuf[newbuf.size() - 1];
	while ((c = _chr)){
		newbuf.erase(newbuf.end() - 1);
		c = newbuf[newbuf.size() - 1];
	}
	(*this) == newbuf;
    }
    
    friend std::ostream &operator<<(std::ostream &os, const string &_str);
    }; //end of string class
    template<typename T> class map : public std::map<string, T> { };
    extern Flux::string Sanitize(const Flux::string&);
    inline std::ostream &operator<<(std::ostream &os, const string &_str) { return os << _str._string; }
    inline const string operator+(char chr, const string &str) { string tmp(chr); tmp += str; return tmp; }
    inline const string operator+(const char *_str, const string &str) { string tmp(_str); tmp += str; return tmp; }
    template<typename T> inline Flux::string stringify(const T &x){
	std::ostringstream stream;
	if(!(stream << x))
		throw;
	return stream.str();
    }
    
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