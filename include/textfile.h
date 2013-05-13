/* Navn IRC bot -- Text file parser header
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */
#pragma once
#ifndef TEXTFILE_H
#define TEXTFILE_H
#include "flux.h"
#include "includes.h"
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

class CoreExport TextFile : public Base
{
private:
	Flux::string line, filename;
	FileIOErrors lasterror;
	std::vector<Flux::string> lines;
protected:
	Flux::string SingleLineBuffer;
public:
	std::vector<Flux::string> Contents;
	~TextFile();
	explicit TextFile(const Flux::string &);
	TextFile(std::vector<Flux::string>);
	FileIOErrors Copy(const Flux::string &);
	FileIOErrors GetLastError();
	static Flux::string TempFile(const Flux::string &);
	static bool IsFile(const Flux::string &);
	static Flux::vector DirectoryListing(const Flux::string &);
	static bool IsDirectory(const Flux::string &);
	Flux::string DecodeLastError();
	Flux::string GetFilename();
	Flux::string FileExtension();
	int NumberOfLines();
	void Edit(int, Flux::string);
	void Clear();
	bool Empty();
	bool WriteToDisk(const Flux::string &);
	Flux::string SingleLine();
};

#endif
