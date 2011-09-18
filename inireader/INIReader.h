// Read an INI file into easy-to-access name/value pairs.

// inih and INIReader are released under the New BSD license (see LICENSE.txt).
// Go to the project home page for more info:
//
// http://code.google.com/p/inih/

#ifndef __INIREADER_H__
#define __INIREADER_H__
#include "flux.h"

// Read an INI file into easy-to-access name/value pairs. (Note that I've gone
// for simplicity here rather than speed, but it should be pretty decent.)
class INIReader
{
public:
    // Construct INIReader and parse given filename. See ini.h for more info
    // about the parsing.
    INIReader(const Flux::string&);
    
    int Parse(const Flux::string &filename);

    // Return the result of ini_parse(), i.e., 0 on success, line number of
    // first error on parse error, or -1 on file open error.
    int ParseError();

    // Get a string value from INI file, returning default_value if not found.
    Flux::string Get(const Flux::string&, const Flux::string&, const Flux::string&);

    // Get an integer (long) value from INI file, returning default_value if
    // not found.
    long GetInteger(const Flux::string&, const Flux::string&, long default_value);
    
    ~INIReader();

private:
    int _error;
    Flux::map<Flux::string> _values;
    static Flux::string MakeKey(const Flux::string&, const Flux::string&);
};

#endif  // __INIREADER_H__
