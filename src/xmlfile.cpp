/* Navn IRC bot -- Simple XML Parser
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */

#include "xmlfile.h"

XMLFile::XMLFile(const Flux::string fn):TextFile(fn)
{
  for (unsigned i = 0; i < SingleLineBuffer.size(); i++)
  {
    char c = SingleLineBuffer.at(i);
    if (c == '<' && SingleLineBuffer.at(i+1) != '/')
    {
      bool SelfContained = false;
      std::string tag = "";
      std::string contents = "";
      for (unsigned j = 1;SingleLineBuffer.at(i+j) != '>' && SingleLineBuffer.at(i+j) != ' '; j++)
      {
	tag += SingleLineBuffer.at(i+j);
      }
      for (unsigned o = 1; SingleLineBuffer.at(i+o) != '>'; o++)
      {
	if ((SingleLineBuffer.at(i+o) == '/' || SingleLineBuffer.at(i+o) == '?') && SingleLineBuffer.at(i+o+1) == '>')
	{
	  SelfContained = true;
	  break;
	}
      }
      if (!SelfContained)
      {
	for (unsigned k = SingleLineBuffer.find('>',i)+1; k < SingleLineBuffer.find("</"+tag,i); k++)
	{
	  contents += SingleLineBuffer.at(k);
	}
      }
      Tag t(tag,contents);
      for (unsigned l = 1; SingleLineBuffer.at(i+l) != '>'; l++)
      {
	char cc = SingleLineBuffer.at(i+l);
	if (cc == '=' && SingleLineBuffer.at(i+l+1) == '"')
	{
	  std::string attribName = "";
	  std::string attribVal = "";
	  for (int m = -1; SingleLineBuffer.at(i+l+m) != ' '; m--)
	  {
	    attribName.insert(0,1,SingleLineBuffer.at(i+l+m));
	  }
	  for (unsigned n = 2; SingleLineBuffer.at(i+l+n) != '"'; n++)
	  {
	    attribVal += SingleLineBuffer.at(i+l+n);
	  }
	  Tag::Attribute att(attribVal);
	  t.Attributes[attribName] = att;
	}
      }
      Tags[tag] = t;
      i += contents.size();
    }
  }
}

XMLFile::Tag::Attribute::Attribute() {}

XMLFile::Tag::Tag() {}

XMLFile::Tag::Tag(const Flux::string n, Flux::string conts)
{
  Name = n;
  Content = conts;
  for (unsigned i = 0; i < conts.size(); i++)
  {
    char c = conts.at(i);
    if (c == '<' && conts.at(i+1) != '/' && conts.at(i+1) != ' ')
    {
      bool SelfContained = false;
      std::string tag = "";
      std::string contents = "";
      for (unsigned j = 1;conts.at(i+j) != '>' && conts.at(i+j) != ' '; j++)
      {
	tag += conts.at(i+j);
      }
      for (unsigned o = 1; conts.at(i+o) != '>'; o++)
      {
	if ((conts.at(i+o) == '/' || conts.at(i+o) == '?') && conts.at(i+o+1) == '>')
	{
	  SelfContained = true;
	  break;
	}
      }
      if (!SelfContained)
      {
	for (unsigned k = conts.find('>',i)+1; k < conts.find("</"+tag,i); k++)
	{
	  contents += conts.at(k);
	}
      }
      Tag t(tag,contents);
      for (unsigned l = 1; conts.at(i+l) != '>'; l++)
      {
	char cc = conts.at(i+l);
	if (cc == '=' && conts.at(i+l+1) == '"')
	{
	  std::string attribName = "";
	  std::string attribVal = "";
	  for (int m = -1; conts.at(i+l+m) != ' '; m--)
	  {
	    attribName.insert(0,1,conts.at(i+l+m));
	  }
	  for (unsigned nn = 2; conts.at(i+l+nn) != '"'; nn++)
	  {
	    attribVal += conts.at(i+l+nn);
	  }
	  Tag::Attribute att(attribVal);
	  t.Attributes[attribName] = att;
	}
      }
      Tags[tag] = t;
      i += contents.size();
    }
  }
}

XMLFile::Tag::Attribute::Attribute(const Flux::string v)
{
  Value = v;
}
