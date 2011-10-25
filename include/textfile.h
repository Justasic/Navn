#ifndef TEXTFILE_H
#define TEXTFILE_H
#include "module.h"
enum FileIOErrors
{
  FILE_IO_OK,
  FILE_IO_MEMORY,
  FILE_IO_PARAMS,
  FILE_IO_EXISTS,
  FILE_IO_NOEXIST,
  FILE_IO_UNKNOWN,
  FILE_IO_FAILURE,
  FILE_IO_EXCEPTION
};
class TextFile
{
private:
  Flux::string SingleLineBuffer, line, filename;
  FileIOErrors lasterror;
  std::vector<Flux::string> lines;
public:
  std::vector<Flux::string> Contents;
  ~TextFile();
  TextFile(const Flux::string&);
  TextFile(std::vector<Flux::string>);
  FileIOErrors Copy(const Flux::string&);
  FileIOErrors GetLastError();
  Flux::string DecodeLastError(), GetFilename();
  int NumberOfLines();
  void Edit(int,Flux::string);
  void Clear();
  bool Empty();
  bool WriteToDisk(const Flux::string&);
  Flux::string SingleLine();
};

#endif