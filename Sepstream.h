#ifndef SEPSTREAM_H
#define SEPSTREAM_H
#include <string>
class sepstream
{
 private:
		Flux::string tokens;
		Flux::string::iterator last_starting_position;
		Flux::string::iterator n;
		char sep;
 public:
		sepstream(const Flux::string &source, char seperator);
		virtual ~sepstream() { }
		virtual bool GetToken(Flux::string &token);
		virtual const Flux::string GetRemaining();
		virtual bool StreamEnd();
};
#endif