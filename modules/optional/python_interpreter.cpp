/* Navn IRC bot -- CTCP handler
 *
 * (C) 2011-2012 Azuru
 * Contact us at Development@Azuru.net
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Anope by The Anope Team.
 * Based on python module of Sigyn by Alyx Wolcott
 */
#include "modules.h"
#include <python.h>

#define ERROR_IF_NULL(x) if (x == NULL) { fprintf(stderr, "Argument %s is undefined.", #x); PyErr_Format(PyExc_TypeError, "Argument %s is undefined.", #x); }


/*
 * Python shim section.
 * All PyObject functions are for the Python<->C++ API
 * and should not be used by anything not in a Python file.
 */

static PyObject * irc_pass(PyObject * self, PyObject * args)
{
    const char * password;
    PyArg_ParseTuple(args, "s", &password);
    py_return_err_if_null(password);
    ircproto->pass(password);
    Py_INCREF(Py_None); return Py_None;
}

static PyObject * irc_nick(PyObject * self, PyObject * args)
{
    const char * nick;
    PyArg_ParseTuple(args, "s", &nick);
    py_return_err_if_null(nick);
    ircproto->nick(nick);
    Py_INCREF(Py_None); return Py_None;
}

static PyObject * irc_user(PyObject * self, PyObject * args)
{
    const char * user, * host, * server, * real;
    PyArg_ParseTuple(args, "ssss", &user, &host, &server, &real);
    py_return_err_if_null(user);
    py_return_err_if_null(host);
    py_return_err_if_null(server);
    py_return_err_if_null(real);
    ircproto->user(user, host, server, real);
    Py_INCREF(Py_None); return Py_None;
}

static PyObject * irc_oper(PyObject * self, PyObject * args)
{
    const char * user, * password;
    PyArg_ParseTuple(args, "ss", &user, &password);
    py_return_err_if_null(user);
    py_return_err_if_null(password);
    ircproto->oper(user, password);
    Py_INCREF(Py_None); return Py_None;
}

static PyObject * irc_quit(PyObject * self, PyObject * args)
{
    const char * message;
    PyArg_ParseTuple(args, "|s", &message);
    ircproto->quit(message);
    Py_INCREF(Py_None); return Py_None;
}

static PyObject * irc_squit(PyObject * self, PyObject * args)
{
    const char * server, * message;
    PyArg_ParseTuple(args, "ss", &server, &message);
    py_return_err_if_null(server);
    py_return_err_if_null(message);
    ircproto->squit(server, message);
    Py_INCREF(Py_None); return Py_None;
}

static PyObject * irc_join(PyObject * self, PyObject * args)
{
    const char * channel, * key;
    PyArg_ParseTuple(args, "s|s", &channel, &key);
    py_return_err_if_null(channel);
    ircproto->join(channel, key);
    Py_INCREF(Py_None); return Py_None;
}

static PyObject * irc_part(PyObject * self, PyObject * args)
{
    const char * channel;
    PyArg_ParseTuple(args, "s", &channel);
    py_return_err_if_null(channel);
    ircproto->part(channel);
    Py_INCREF(Py_None); return Py_None;
}

static PyObject * irc_mode(PyObject * self, PyObject * args)
{
    const char * target, * modestring;
    PyArg_ParseTuple(args, "ss", &target, &modestring);
    py_return_err_if_null(target);
    py_return_err_if_null(modestring);
    ircproto->mode(target, modestring);
    Py_INCREF(Py_None); return Py_None;
}

static PyObject * irc_topic(PyObject * self, PyObject * args)
{
    const char * channel, * topic;
    PyArg_ParseTuple(args, "ss", &channel, &topic);
    py_return_err_if_null(channel);
    py_return_err_if_null(topic);
    ircproto->topic(channel, topic);
    Py_INCREF(Py_None); return Py_None;
}

static PyObject * irc_names(PyObject * self, PyObject * args)
{
    const char * channel;
    PyArg_ParseTuple(args, "s", &channel);
    py_return_err_if_null(channel);
    ircproto->names(channel);
    Py_INCREF(Py_None); return Py_None;
}

static PyObject * irc_invite(PyObject * self, PyObject * args)
{
    const char * user, * channel;
    PyArg_ParseTuple(args, "ss", &user, &channel);
    py_return_err_if_null(user);
    py_return_err_if_null(channel);
    ircproto->invite(user, channel);
    Py_INCREF(Py_None); return Py_None;
}

static PyObject * irc_kick(PyObject * self, PyObject * args)
{
    const char * channel, * user, * comment;
    PyArg_ParseTuple(args, "ss|s", &channel, &user, &comment);
    py_return_err_if_null(channel);
    py_return_err_if_null(user);
    ircproto->kick(channel, user, comment);
    Py_INCREF(Py_None); return Py_None;
}

static PyObject * irc_version (PyObject * self, PyObject * args)
{
    const char * server;
    PyArg_ParseTuple(args, "|s", &server);
    ircproto->version(server);
    Py_INCREF(Py_None); return Py_None;
}

static PyObject * irc_stats(PyObject * self, PyObject * args)
{
    const char * query, * server;
    PyArg_ParseTuple(args, "s|s", &query, &server);
    py_return_err_if_null(query);
    ircproto->stats(query, server);
    Py_INCREF(Py_None); return Py_None;
}

static PyObject * irc_links(PyObject * self, PyObject * args)
{
    const char * remote, * mask;
    PyArg_ParseTuple(args, "|ss", &remote, &mask);
    ircproto->links(remote, mask);
    Py_INCREF(Py_None); return Py_None;
}

static PyObject * irc_time(PyObject * self, PyObject * args)
{
    const char * server;
    PyArg_ParseTuple(args, "|s", &server);
    ircproto->time(server);
    Py_INCREF(Py_None); return Py_None;
}

static PyObject * irc_connect(PyObject * self, PyObject * args)
{
    const char * target, * remote;
    const int port;
    PyArg_ParseTuple(args, "s|is", &target, &port, &remote);
    py_return_err_if_null(target);
    ircproto->connect(target, port, remote);
    Py_INCREF(Py_None); return Py_None;
}

static PyObject * irc_admin(PyObject * self, PyObject * args)
{
    const char * server;
    PyArg_ParseTuple(args, "|s", &server);
    ircproto->admin(server);
    Py_INCREF(Py_None); return Py_None;
}

static PyObject * irc_info(PyObject * self, PyObject * args)
{
    const char * server;
    PyArg_ParseTuple(args, "|s", &server);
    ircproto->info(server);
    Py_INCREF(Py_None); return Py_None;
}

static PyObject * irc_privmsg(PyObject * self, PyObject * args)
{
    const char * reciever, * string;
    PyArg_ParseTuple(args, "ss", &reciever, &string);
    py_return_err_if_null(reciever);
    py_return_err_if_null(string);
    ircproto->privmsg(reciever, string);
    Py_INCREF(Py_None); return Py_None;
}

static PyObject * irc_notice(PyObject * self, PyObject * args)
{
    const char * reciever, * string;
    PyArg_ParseTuple(args, "ss", &reciever, &string);
    py_return_err_if_null(reciever);
    py_return_err_if_null(string);
    ircproto->notice(reciever, string);
    Py_INCREF(Py_None); return Py_None;
}

static PyObject * irc_who(PyObject * self, PyObject * args)
{
    const char * name, * o;
    PyArg_ParseTuple(args, "s|s", &name, &o);
    py_return_err_if_null(name);
    ircproto->who(name, o);
    Py_INCREF(Py_None); return Py_None;
}

static PyObject * irc_whois(PyObject * self, PyObject * args)
{
    const char * server, * nickmask;
    PyArg_ParseTuple(args, "s|s", &nickmask, &server);
    py_return_err_if_null(nickmask);
    ircproto->whois(server, nickmask);
    Py_INCREF(Py_None); return Py_None;
}

static PyObject * irc_whowas(PyObject * self, PyObject * args)
{
    const char * nickname, * server;
    const int    count;
    PyArg_ParseTuple(args, "s|is", &nickname, &count, &server);
    ircproto->whowas(nickname, (int)count, server);
    Py_INCREF(Py_None); return Py_None;
}

static PyObject * irc_kill(PyObject * self, PyObject * args)
{
    const char * nickname, * reason;
    PyArg_ParseTuple(args, "ss", &nickname, &reason);
    py_return_err_if_null(nickname);
    py_return_err_if_null(reason);
    ircproto->kill(nickname, reason);
    Py_INCREF(Py_None); return Py_None;
}

static PyObject * irc_ping(PyObject * self, PyObject * args)
{
    const char * server1, * server2;
    PyArg_ParseTuple(args, "s|s", &server1, &server2);
    py_return_err_if_null(server1);
    ircproto->ping(server1, server2);
    Py_INCREF(Py_None); return Py_None;
}

static PyObject * irc_pong(PyObject * self, PyObject * args)
{
    const char * daemon;
    PyArg_ParseTuple(args, "s", &daemon);
    py_return_err_if_null(daemon);
    ircproto->pong(daemon);
    Py_INCREF(Py_None); return Py_None;
}

static PyObject * irc_away(PyObject * self, PyObject * args)
{
    const char * message;
    PyArg_ParseTuple(args, "|s", &message);
    ircproto->away(message);
    Py_INCREF(Py_None); return Py_None;
}

static PyObject * irc_users(PyObject * self, PyObject * args)
{
    const char * server;
    PyArg_ParseTuple(args, "|s", &server);
    ircproto->users(server);
    Py_INCREF(Py_None); return Py_None;
}

static PyObject * irc_userhost(PyObject * self, PyObject * args)
{
    const char * nickname;
    PyArg_ParseTuple(args, "s", &nickname);
    py_return_err_if_null(nickname);
    ircproto->userhost(nickname);
    Py_INCREF(Py_None); return Py_None;
}


static PyMethodDef NavnMethods[] = {
  {"irc_pass", irc_pass, METH_VARARGS, "Send PASS to the server."},
  {"irc_nick", irc_nick, METH_VARARGS, "Send NICK to the server."},
  {"irc_user", irc_user, METH_VARARGS, "Send USER to the server."},
  {"irc_oper", irc_oper, METH_VARARGS, "Send OPER to the server."},
  {"irc_quit", irc_quit, METH_VARARGS, "Send QUIT to the server."},
  {"irc_squit", irc_squit, METH_VARARGS, "Send SQUIT to the server."},
  {"irc_join", irc_join, METH_VARARGS, "Send JOIN to the server."},
  {"irc_part", irc_part, METH_VARARGS, "Send PART to the server."},
  {"irc_mode", irc_mode, METH_VARARGS, "Send MODE to the server."},
  {"irc_topic", irc_topic, METH_VARARGS, "Send TOPIC to the server."},
  {"irc_names", irc_names, METH_VARARGS, "Send NAMES to the server."},
  {"irc_invite", irc_invite, METH_VARARGS, "Send INVITE to the server."},
  {"irc_kick", irc_kick, METH_VARARGS, "Send KICK to the server."},
  {"irc_version", irc_version, METH_VARARGS, "Send VERSION to the server."},
  {"irc_stats", irc_stats, METH_VARARGS, "Send STATS to the server."},
  {"irc_links", irc_links, METH_VARARGS, "Send LINKS to the server."},
  {"irc_time", irc_time, METH_VARARGS, "Send TIME to the server."},
  {"irc_connect", irc_connect, METH_VARARGS, "Send CONNECT to the server."},
  {"irc_admin", irc_admin, METH_VARARGS, "Send ADMIN to the server."},
  {"irc_info", irc_info, METH_VARARGS, "Send INFO to the server."},
  {"irc_privmsg", irc_privmsg, METH_VARARGS, "Send PRIVMSG to the server."},
  {"irc_notice", irc_notice, METH_VARARGS, "Send NOTICE to the server."},
  {"irc_who", irc_who, METH_VARARGS, "Send WHO to the server."},
  {"irc_whois", irc_whois, METH_VARARGS, "Send WHOIS to the server."},
  {"irc_whowas", irc_whowas, METH_VARARGS, "Send WHOWAS to the server."},
  {"irc_kill", irc_kill, METH_VARARGS, "Send KILL to the server."},
  {"irc_ping", irc_ping, METH_VARARGS, "Send PING to the server."},
  {"irc_pong", irc_pong, METH_VARARGS, "Send PONG to the server."},
  {"irc_away", irc_away, METH_VARARGS, "Send AWAY to the server."},
  {"irc_users", irc_users, METH_VARARGS, "Send USERS to the server."},
  {"irc_userhost", irc_userhost, METH_VARARGS, "Send USERHOST to the server."}
};

class CommandLoadPy : public Command
{
public:
  CommandLoadPy(module *m):Command(m, "LOADPY", C_PRIVATE, 1, 1)
  {
    this->SetDesc("Load a python file");
    this->SetSyntax("\37filepath\37");
  }

  void Run(CommandSource &source, const Flux::vector &params)
  {
    Flux::string file;
    User *u = source.u;

    if(params[1][1] == '/')
      file = params[1];
    else
      file = Config->Binary_Dir + Config->pydir + params[1];
    
    if(!u->IsOwner())
    {
      source.Reply(ACCESS_DENIED);
      return;
    }

    if(!TextFile::IsFile(file))
    {
      source.Reply("Cannot locate \2%s\2 for loading!", #file.c_str());
      return;
    }

    FILE *fd;

    if(!(fp = fopen(file, "r")))
    {
      source.Reply("Failed to open \2%s\2", #file.c_str());
      return;
    }

    PyRun_AnyFileExFlags(fp, file.c_str(), 0, NULL);
  }

  bool OnHelp(CommandSource &source, const Flux::string &nill)
  {
    // TODO: Help.
    return false;
  }
}

class PyInterp_mod : public module
{
  CommandLoadPy cmdloadpy;
public:
  PyInterp_mod():module(), cmdloadpy(this) 
  {
    // Load our Python API
    Py_Initialize();
    Py_InitModule("_navn", NavnMethods);
  }

  ~PyInterp_mod()
  {
    // Remove our Python API
    Py_Finalize();
  }
};

MODULE_INIT(PyInterp_mod)