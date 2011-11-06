#ifndef XMLFILE_H
#define XMLFILE_H
#include "module.h"

class XMLFile : public TextFile
{
private:
  class Tag
  {
  private:
    class Attribute
    {
    private:
    public:
      Attribute(Flux::string,Flux::string);
      Flux::string Name;
      Flux::string Value;
    };
  public:
    Tag();
    Tag(Flux::string,Flux::string);
    Flux::string Name;
    Flux::string Content;
    std::map<Flux::string,Attribute> Attributes;
    std::map<Flux::string,Tag> Tags;
  };
public:
  XMLFile(Flux::string);
  std::map<Flux::string,Tag> Tags;
};

#endif