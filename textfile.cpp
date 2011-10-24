#include "textfile.h"

TextFile::TextFile() {}

TextFile::TextFile(Flux::string &FileName)
{
  std::ifstream in(FileName.c_str());
  while(std::getline(in,line.std_str()))
  {
    SingleLineBuffer += line;
    lines.push_back(line);
  }
  Contents = lines;
}

TextFile::TextFile(std::vector<Flux::string> conts) { lines = conts; Contents = conts; }

int TextFile::NumberOfLines() { return lines.size(); }

void TextFile::Edit(int ln, Flux::string fs) { lines[ln] = fs; Contents[ln] = fs; }

void TextFile::Clear() { std::vector<Flux::string> empty; lines = empty; Contents = empty; }

bool TextFile::Empty()
{
  for (unsigned i = 0; i < lines.size(); i++) { if (!lines[i].empty()) return false; }
  return true;
}

bool TextFile::WriteToDisk(Flux::string &FileName)
{
  std::ofstream f(FileName.c_str());
  if (f.is_open())
  {
    if (Contents != lines)
    {
      for (unsigned i = 0; i < Contents.size(); i++)
      {
	Contents[i].strip();
	f << Contents[i] << "\n";
      }
    }
    else
    {
      for (unsigned i = 0; i < lines.size(); i++)
      {
	lines[i].strip();
	f << lines[i] << "\n";
      }
    }
    f.close();
    return true;
  }
  else { return false; }
}

Flux::string TextFile::SingleLine() { return SingleLineBuffer; }