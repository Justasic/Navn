#ifndef SEPSTREAM_H
#define SEPSTREAM_H
#include <string>
class sepstream
{
 private:
		std::string tokens;
		std::string::iterator last_starting_position;
		std::string::iterator n;
		char sep;
 public:
		sepstream(const std::string &source, char seperator);
		virtual ~sepstream() { }
		virtual bool GetToken(std::string &token);
		virtual const std::string GetRemaining();
		virtual bool StreamEnd();
};
#endif