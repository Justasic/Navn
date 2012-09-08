/* Navn IRC bot -- Exceptions header
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 */
/**
 *\file  SocketException.h
 *\brief Contains the Exception throw classes.
 */
#pragma once
#ifndef SOCKET_EXCEPTION_H
#define SOCKET_EXCEPTION_H
#include <exception>
#include "flux.h"
/** SocketExceptions are to be thrown with anything Socket related
 * Throws Socket Exception
 * @param throw SocketException(message)
 */
/*class SocketException
{
 public:
  SocketException(Flux::string s):m_s(s) {};
  ~SocketException(){};

  Flux::string description() { return m_s; }
 private:
  Flux::string m_s;
};*/

/**CoreExceptions are to be thrown with everything else
 * Throws Core Exception
 * @param throw CoreException(message)
 */

#endif
