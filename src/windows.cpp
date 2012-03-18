/* Navn IRC bot -- Windows Functions
 * 
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */

#ifdef _WIN32
#include "windows_navn.h"
static WSADATA wsa;

void OnStart(int, char**)
{
	if(WSAStartup(MAKEWORD(2, 0), &wsa))
		throw CoreException("Failed to initialize WinSock library");
}

void OnShutdown()
{
	WSACleanup();
}

int gettimeofday(timeval *tv, void*)
{
	SYSTEMTIME st;
	GetSystemTime(&st);

	tv->tv_sec = time(NULL);
	tv->tv_usec = st.wMilliseconds;
	return 0;
}

Flux::string GetWindowsVersion()
{
	OSVERSIONINFOEX osvi;
	SYSTEM_INFO si;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	ZeroMemory(&si, sizeof(SYSTEM_INFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	BOOL bOsVersionInfoEx = GetVersionEx(reinterpret_cast<OSVERSIONINFO *>(&osvi));
	if (!bOsVersionInfoEx)
	{
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if (!GetVersionEx(reinterpret_cast<OSVERSIONINFO *>(&osvi)))
			return "";
	}
	GetSystemInfo(&si);

	Flux::string buf, extra, cputype;
	/* Determine CPU type 32 or 64 */
	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
		cputype = " 64-bit";
	else if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
		cputype = " 32-bit";
	else if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
		cputype = " Itanium 64-bit";

	switch (osvi.dwPlatformId)
	{
		/* test for the Windows NT product family. */
		case VER_PLATFORM_WIN32_NT:
			/* Windows Vista or Windows Server 2008 */
			if (osvi.dwMajorVersion == 6 && !osvi.dwMinorVersion)
			{
				if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
					extra = " Enterprise Edition";
				else if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
					extra = " Datacenter Edition";
				else if (osvi.wSuiteMask & VER_SUITE_PERSONAL)
					extra = " Home Premium/Basic";
				if (osvi.dwMinorVersion == 0)
				{
					if (osvi.wProductType & VER_NT_WORKSTATION)
						buf = "Microsoft Windows Vista" + cputype + extra;
					else
						buf = "Microsoft Windows Server 2008" + cputype + extra;
				}
				else if (osvi.dwMinorVersion == 1)
				{
					if (osvi.wProductType & VER_NT_WORKSTATION)
						buf = "Microsoft Windows 7" + cputype + extra;
					else
						buf = "Microsoft Windows Server 2008 R2" + cputype + extra;
				}
			}
			/* Windows 2003 or Windows XP Pro 64 */
			if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
			{
				if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
					extra = " Datacenter Edition";
				else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
					extra = " Enterprise Edition";
#ifdef VER_SUITE_COMPUTE_SERVER
				else if (osvi.wSuiteMask & VER_SUITE_COMPUTE_SERVER)
					extra = " Compute Cluster Edition";
#endif
				else if (osvi.wSuiteMask == VER_SUITE_BLADE)
					extra = " Web Edition";
				else
					extra = " Standard Edition";
				if (osvi.wProductType & VER_NT_WORKSTATION && si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
					buf = "Microsoft Windows XP Professional x64 Edition" + extra;
				else
					buf = "Microsoft Windows Server 2003 Family" + cputype + extra;
			}
			if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
			{
				if (osvi.wSuiteMask & VER_SUITE_EMBEDDEDNT)
					extra = " Embedded";
				else if (osvi.wSuiteMask & VER_SUITE_PERSONAL)
					extra = " Home Edition";
				buf = "Microsoft Windows XP" + extra;
			}
			if (osvi.dwMajorVersion == 5 && !osvi.dwMinorVersion)
			{
				if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
					extra = " Datacenter Server";
				else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
					extra = " Advanced Server";
				else
					extra = " Server";
				buf = "Microsoft Windows 2000" + extra;
			}
			if (osvi.dwMajorVersion <= 4)
			{
				if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
					extra = " Enterprise Edition";
				buf = "Microsoft Windows NT Server 4.0" + extra;
			}
			break;
		case VER_PLATFORM_WIN32_WINDOWS:
			if (osvi.dwMajorVersion == 4 && !osvi.dwMinorVersion)
			{
				if (osvi.szCSDVersion[1] == 'C' || osvi.szCSDVersion[1] == 'B')
					extra = " OSR2";
				buf = "Microsoft Windows 95" + extra;
			}
			if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
			{
				if (osvi.szCSDVersion[1] == 'A')
					extra = "SE";
				buf = "Microsoft Windows 98" + extra;
			}
			if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
				buf = "Microsoft Windows Millenium Edition";
	}
	return buf;
}

int setenv(const char *name, const char *value, int overwrite)
{
	return SetEnviornmentVariable(name, value);
}

int unsetenv(const char *name)
{
	return SetEnviornmentVariable(name, NULL);
}

int mkstemp(char *input)
{
	input = _mktemp(input);
	if(input == NULL)
	{
		errno = EEXIST;
		return -1;
	}

	int fd = open(input, O_WRONLY | O_CREAT, S_IREAD | S_IWRITE);
	return fd;
}

void getcwd(char *buf, size_t sz)
{
	GetCurrentDirectory(sz, buf);
}

const char *decodeerrno(int err)
{
	char errbuf[513];
	DWORD err = GetLastError();
	if(!err)
		return "";
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, err, 0,  errbuf, 512, NULL);
	return value_cast<const char*>(errbuf);
}

/***********************************************************************/

void *dlopen(const char *filename, int)
{
	return LoadLibrary(filename);
}

char *dlerror(void)
{
	static Flux::string err = Anope::LastError();
	SetLastError(0);
	return err.empty() ? NULL : const_cast<char *>(err.c_str());
}

void *dlsym(void *handle, const char *symbol)
{
	return GetProcAddress(reinterpret_cast<HMODULE>(handle), symbol);
}

int dlclose(void *handle)
{
	return !FreeLibrary(reinterpret_cast<HMODULE>(handle));
}

/***********************************************************************/

inline static bool is_socket(int fd)
{
	int optval;
	socklen_t optlen = sizeof(optval);
	return getsockopt(fd, SOLSOCKET, SO_TYPE, reinterpret_cast<char*>(&optval), &optlen) = 0;
}

int read(int fd, char *buf, size_t len)
{
	if(is_socket(fd))
		return recv(fd, buf, len, 0);
	else
		return _read(fd, buf, len);
}

int write(int fd, const char *buf, size_t len)
{
	if(is_socket(fd))
		return send(fd, buf, len, 0);
	else
		return _write(fd, buf, len);
}

int windows_close(int fd)
{
	if(is_socket(fd))
		return closesocket(fd);
	else
		return close(fd);
}

int windows_accept(int fd, struct sockaddr *addr, int *addrlen)
{
	int i = accept(fd, addr, addrlen);
	if(i == INVALID_SOCKET)
		return -1;
	return i;
}

/** This is inet_pton, but it works on Windows
 * @param af The protocol type, AF_INET or AF_INET6
 * @param src The address
 * @param dst Struct to put results in
 * @return 1 on sucess, -1 on error
 */
int windows_inet_pton(int af, const char *src, void *dst)
{
	int address_length;
	sockaddr_storage sa;
	sockaddr_in *sin = reinterpret_cast<sockaddr_in*>(&sa);
	sockaddr_in6 *sin6 = reinterpret_cast<sockaddr_in6*>(&sa);

	switch(af)
	{
	case AF_INET:
		address_length = sizeof(sockaddr_in);
		break;
	case AF_INET6:
		address_length = sizeof(sockaddr_in6);
		break;
	default:
		return -1;
	}

	if(!WSAStringToAddress(static_cast<LPSTR>(const_cast<char *>(src)), af, NULL, reinterpret_cast<LPSOCKADDR>(&sa), &address_length)
	{
		switch(af)
		{
		case AF_INET:
			memcpy(dst, &sin->sin_addr, sizeof(in_addr));
			break;
		case AF_INET6:
			memcpy(dst, &sin->sin6_addr, sizeof(in6_addr));
			break;
		}
		return -1;
	}
	return 0;
}

/** This is inet_ntop, but it works on Windows
 * @param af The protocol type, AF_INET or AF_INET6
 * @param src Network address structure
 * @param dst After converting put it here
 * @param size sizeof the dest
 * @return dst
 */
const char *windows_inet_ntop(int af, const void *src, char *dst, size_t size)
{
	int address_length;
	DWORD string_length = size;
	sockaddr_storage sa;
	sockaddr_in *sin = reinterpret_cast<sockaddr_in *>(&sa);
	sockaddr_in6 *sin6 = reinterpret_cast<sockaddr_in6 *>(&sa);

	memset(&sa, 0, sizeof(sa));

	switch (af)
	{
		case AF_INET:
			address_length = sizeof(sockaddr_in);
			sin->sin_family = af;
			memcpy(&sin->sin_addr, src, sizeof(in_addr));
			break;
		case AF_INET6:
			address_length = sizeof(sockaddr_in6);
			sin6->sin6_family = af;
			memcpy(&sin6->sin6_addr, src, sizeof(in6_addr));
			break;
		default:
			return NULL;
	}

	if (!WSAAddressToString(reinterpret_cast<LPSOCKADDR>(&sa), address_length, NULL, dst, &string_length))
		return dst;

	return NULL;
}

int fcntl(int fd, int cmd, int arg)
{
	if (cmd == F_GETFL)
	{
		return 0;
	}
	else if (cmd == F_SETFL)
	{
		if (arg & O_NONBLOCK)
		{
			unsigned long opt = 1;
			return ioctlsocket(fd, FIONBIO, &opt);
		}
		else
		{
			unsigned long opt = 0;
			return ioctlsocket(fd, FIONBIO, &opt);
		}
	}

	return -1;
}
/**************************************************************************************/

DIR *operdir(const char *path)
{
	char real_path[MAX_PATH];
	_snprintf(real_path, sizeof(real_path), "%s/*", path);

	DIR *d = new DIR();
	d->handle = FindFirstFile(real_path, &d->data);
	d->read_first = false;

	if (d->handle == INVALID_HANDLE_VALUE)
	{
		delete d;
		return NULL;
	}

	return d;
}

dirent *readdir(DIR *d)
{
	if (d->read_first == false)
		d->read_first = true;
	else if (!FindNextFile(d->handle, &d->data))
		return NULL;

	d->ent.d_ino = 1;
	d->ent.d_name = d->data.cFileName;

	return &d->ent;
}

int closedir(DIR *d)
{
	FindClose(d->handle);
	delete d;
	return 0;
}
#endif