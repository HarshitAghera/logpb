#pragma once

#ifndef _MSC_VER
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include <fcntl.h>

#include <google/protobuf/io/io_win32.h>


#ifdef _WIN32
// Win32 lseek is broken:  If invoked on a non-seekable file descriptor, its
// return value is undefined.  We re-define it to always produce an error.
#define lseek(fd, offset, origin) ((off_t)-1)
namespace win_io {
using google::protobuf::io::win32::fopen;
using google::protobuf::io::win32::access;
using google::protobuf::io::win32::chdir;
using google::protobuf::io::win32::close;
using google::protobuf::io::win32::dup;
using google::protobuf::io::win32::dup2;
using google::protobuf::io::win32::mkdir;
using google::protobuf::io::win32::open;
using google::protobuf::io::win32::read;
using google::protobuf::io::win32::setmode;
using google::protobuf::io::win32::stat;
using google::protobuf::io::win32::write;
}
#endif

#ifdef _MSC_VER
using namespace win_io;
#endif
