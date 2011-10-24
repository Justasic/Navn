#ifndef TEXTFILE_H
#define TEXTFILE_H
#include "module.h"

class TextFile
{
private:
  Flux::string SingleLineBuffer, line;
  std::vector<Flux::string> lines;
public:
  std::vector<Flux::string> Contents;
  TextFile();
  TextFile(Flux::string&);
  TextFile(std::vector<Flux::string>);
  int NumberOfLines();
  void Edit(int,Flux::string);
  void Clear();
  bool Empty();
  bool WriteToDisk(Flux::string&);
  Flux::string SingleLine();
};

#endif