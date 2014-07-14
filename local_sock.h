/*local include file for socket programs*/
#ifndef LOCAL_SOCK_H
#define LOCAL_SOCK_H
#define _GNU_SOURCE
#include <iostream>
#include <sys/ioctl.h>
#include <cstdio>
#include <string>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>

const int PORT = 65300;
const int BUFSIZE = 1024;
static char buf[BUFSIZE];
const char *SERVER_FILE ="server_socket";
#endif
using namespace std;
