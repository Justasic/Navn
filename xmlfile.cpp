#include "xmlfile.h"

XMLFile::XMLFile(Flux::string fn):TextFile(fn)
{
  for (unsigned i = 0; i < SingleLineBuffer.size(); i++)
  {
    char c = SingleLineBuffer.at(i);
    if (c == '<' && SingleLineBuffer.at(i+1) != '/')
    {
      std::string tag = "";
      std::string contents = "";
      for (unsigned j = 1;SingleLineBuffer.at(i+j) != '>' && SingleLineBuffer.at(i+j) != ' '; j++)
      {
	tag += SingleLineBuffer.at(i+j);
      }
      for (unsigned k = SingleLineBuffer.find('>',i)+1; k < SingleLineBuffer.find("</"+tag); k++)
      {
	contents += SingleLineBuffer.at(k);
      }
      Tag t(tag,contents);
      Tags[tag] = t;
      i += contents.size();
    }
  }
}

XMLFile::Tag::Tag() {}

XMLFile::Tag::Tag(Flux::string n,Flux::string conts)
{
  Name = n;
  Content = conts;
  for (unsigned i = 0; i < conts.size(); i++)
  {
    char c = conts.at(i);
    if (c == '<' && conts.at(i+1) != '/' && conts.at(i+1) != ' ')
    {
      std::string tag = "";
      std::string contents = "";
      for (unsigned j = 1;conts.at(i+j) != '>'; j++)
      {
	tag += conts.at(i+j);
      }
      for (unsigned k = conts.find('>',i)+1; k < conts.find("</"+tag); k++)
      {
	contents += conts.at(k);
      }
      Tag t(tag,contents);
      Tags[tag] = t;
      i += contents.size();
    }
  }
}

XMLFile::Tag::Attribute::Attribute(Flux::string n, Flux::string v)
{
  Name = n;
  Value = v;
}