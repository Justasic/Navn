/* Navn IRC bot -- Simple XML parser
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */
#pragma once
#ifndef XMLFILE_H
#define XMLFILE_H

#include "module.h"

class CoreExport XMLFile : public TextFile 
{
private:
  class CoreExport Tag
  {
  public:
    class CoreExport Attribute
    {
    private:
    public:
      Attribute();
      Attribute(Flux::string);
      Flux::string Name;
      Flux::string Value;
    };
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
