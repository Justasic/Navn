/* Navn IRC bot -- Text File parser wrapper
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */

#include "textfile.h"

/** TextFile parser
 * \class TextFile A File I/O wrapper for C++
 * \brief This wrapper makes it easy to parse files and get lines from them. It first parses them, then keeps them in a vector for later use.
 * \fn TextFile::TextFile(const Flux::string &FileName)
 * \param Flux::string The filename you want to parse
 */
TextFile::TextFile(const Flux::string &FileName) : filename(FileName), lasterror(static_cast<FileIOErrors>(NULL))
{
  std::ifstream in(FileName.c_str());
  while(std::getline(in,line.std_str()))
  {
    SingleLineBuffer += line;
    lines.push_back(line);
  }
  Contents = lines;
}
/** \overload TextFile::TextFile(std::vector<Flux::string> conts) */
TextFile::TextFile(std::vector<Flux::string> conts) { lines = conts; Contents = conts; }

TextFile::~TextFile() {}
/** TextFile Error Decoder
 * \brief This is just a simple error code to string (like strerror) for TextFile
 * \fn Flux::string TextFile::DecodeLastError()
 * \return the last set error code into a human readable message
 */
Flux::string TextFile::DecodeLastError()
{
  switch(this->lasterror)
  {
    case FILE_IO_OK:
      return "Success (FILE_IO_OK)";
    case FILE_IO_MEMORY:
      return "Insufficient Memory (FILE_IO_MEMORY)";
    case FILE_IO_PARAMS:
      return "Insufficient Parameters (FILE_IO_PARAMS)";
    case FILE_IO_NOEXIST:
      return "File does not exist (FILE_IO_NOEXIST)";
    case FILE_IO_FAILURE:
      return "I/O Failure (FILE_IO_FAILURE)";
    case FILE_IO_EXISTS:
      return "File Exists (FILE_IO_EXISTS)";
    case FILE_IO_UNKNOWN:
      return "Unknown File Error (FILE_IO_UNKNOWN)";
    case FILE_IO_EXCEPTION:
      return "Exception was caught (FILE_IO_EXCEPTION)";
    default:
      return "Unknown file error.";
  }
  return "Unknown file error.";
}
/** Number of Lines
 * \brief return the number of lines that the file contains
 * \fn int TextFile::NumberOfLines()
 * \return Number of lines
 */
int TextFile::NumberOfLines() { return lines.size(); }
/** Last Error Code
 * \brief Returns the last error code set
 * \fn FileIOErrors TextFile::GetLastError()
 * \return last set error code
 */
FileIOErrors TextFile::GetLastError() { return lasterror; }
/** File Name
 * \brief Returns the filename of the file being parsed.
 * \fn Flux::string TextFile::GetFilename()
 * \return filename
 */
Flux::string TextFile::GetFilename() { return filename; }
/** Edit File Lines
 * \brief Edit a specific line in the file
 * \fn void TextFile::Edit(int ln, Flux::string fs)
 * \param int Line Number to edit
 * \param Flux::string line to replace
 */
void TextFile::Edit(int ln, Flux::string fs) { lines[ln] = fs; Contents[ln] = fs; }
/** Clear all data
 * \brief Clears all the file lines and data for those lines, making the class empty
 * \fn void TextFile::Clear()
 */
void TextFile::Clear() { lines.clear(); Contents.clear(); }
/** Empty Check
 * \brief Check if the file is empty or not
 * \fn bool TextFile::Empty()
 * \return if the class is empty
 */
bool TextFile::Empty()
{
  if(lines.empty())
    return true;
  for (unsigned i = 0; i < lines.size(); i++) { if (!lines[i].empty()) return false; }
  return true;
}
/** File Copy
 * \brief Copies a file, binary or text, to a new location
 * \fn FileIOErrors TextFile::Copy(const Flux::string &dest)
 * \return error code (LastError will also be set)
 * \param Flux::string The destination
 */
//This function is REQUIRED by the bot to load modules, !! DO NOT MODIFY !!
FileIOErrors TextFile::Copy(const Flux::string &dest)
{
  if(dest.empty() || this->filename.empty())
    return (this->lasterror = FILE_IO_PARAMS);

  struct stat s;
  if (stat(dest.c_str(), &s) == -1)
    return (this->lasterror = FILE_IO_NOEXIST);
  
  if(stat(this->filename.c_str(), &s) == -1)
    return (this->lasterror = FILE_IO_NOEXIST);
  else if (!S_ISREG(s.st_mode))
    return (this->lasterror = FILE_IO_NOEXIST);

  std::ifstream source(this->filename.c_str(), std::ios_base::in | std::ios_base::binary);
  if (!source.is_open())
    return (this->lasterror = FILE_IO_NOEXIST);
  
  std::ofstream target(dest.c_str(), std::ios_base::in | std::ios_base::binary);
  if(!target.is_open())
  {
    source.close();
    return (this->lasterror = FILE_IO_NOEXIST);
  }

  int want = s.st_size;
  char *buffer = new char[s.st_size];
  while (want > 0 && !source.fail() && !target.fail())
  {
    source.read(buffer, want);
    int read_len = source.gcount();

    target.write(buffer, read_len);
    want -= read_len;
  }
  delete [] buffer;

  source.close();
  target.close();

  return !source.fail() && !target.fail() ? (this->lasterror = FILE_IO_OK) : (this->lasterror = FILE_IO_FAILURE);
}

/**
 * \fn Flux::string TempFile(const Flux::string &file)
 * \brief Creates a temporary file name for use in navn, can be quite useful.
 * \param Flux::string The Flux::string of the file location/name
 * \return the randomly generated string for the filename
 * NOTE: THIS _MUST_ HAVE 6 X's (XXXXXX) to work properly.
 */
Flux::string TextFile::TempFile(const Flux::string &file)
{
  char *tmp_output = strdup(file.c_str());
  int target_fd = mkstemp(tmp_output);
  if (target_fd == -1 || close(target_fd) == -1)
  {
    free(tmp_output);
    return "";
  }
  Flux::string filestring = tmp_output;
  free(tmp_output);
  return filestring;
}

/** Check if a file exists
 * \param filename The file
 * \return true if the file exists, false if it doens't
 * \return true if file is a normal file, false if else
 */
bool TextFile::IsFile(const Flux::string &filename)
{
  const char *file = filename.c_str();
  struct stat sb;
  if (stat(file, &sb) == -1)
    return false;
  
  if ((sb.st_mode & S_IFDIR) > 0)
    return false;
  
  FILE *input = fopen(file, "r");
  if (input == NULL)
    return false;
  else
    fclose(input);
  return true;
}

Flux::vector TextFile::DirectoryListing(const Flux::string &directory)
{
  Flux::vector nil;
  if(!TextFile::IsDirectory(directory))
    return nil;

  DIR *dp;
  struct dirent *drip;
  if((dp = opendir(directory.c_str())) == NULL)
    return nil;

  Flux::vector files;
  while ((drip = readdir(dp)) != NULL)
  {
    if(!drip->d_ino)
      continue;
    if(Flux::string(drip->d_name).equals_cs(".") || Flux::string(drip->d_name).equals_cs(".."))
      continue;
    files.push_back(Flux::string(drip->d_name));
  }

  closedir(dp);
  return files;
}

bool TextFile::IsDirectory(const Flux::string &dirname)
{
  struct stat fileinfo;
  if(stat(dirname.c_str(), &fileinfo) == 0)
    return true;
  return false;
}

/** Write to disk
 * \brief Write text to the file
 * \fn bool TextFile::WriteToDisk(const Flux::string &FileName)
 * \param Flux::string Filename to write
 * \return whether it wrote or not
 */
bool TextFile::WriteToDisk(const Flux::string &FileName)
{
  std::ofstream f(FileName.c_str());
  if (f.is_open())
  {
    if (Contents != lines)
    {
      for (unsigned i = 0; i < Contents.size(); i++)
	f << Contents[i].strip() << "\n";
    }
    else
    {
      for (unsigned i = 0; i < lines.size(); i++)
	f << lines[i].strip() << "\n";
    }
    f.close();
    this->lasterror = FILE_IO_OK;
    return true;
  }
  else { this->lasterror = FILE_IO_NOEXIST; return false; }
}
/** Single Line Buffer
 * \brief Returns the whole text file in one Flux::string
 * \fn Flux::string TextFile::SingleLine()
 * \return File Buffer in one Flux::string
 */
Flux::string TextFile::SingleLine() { return SingleLineBuffer; }
/** File Extension
 * \brief Returns the file extention, if there is one
 * \fn Flux::string TextFile::Extension()
 * \return File Extension
 */
Flux::string TextFile::Extension()
{
  if (filename.search('.'))
  {
    Flux::string ext = "";
    for (unsigned i = filename.length()-1; filename[i] != '.' && i < filename.size(); i--)
      ext += filename[i];
    return ext;
  }
  else { return ""; }
}
