// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#include <windows.h>
#include <tchar.h>
#include <ecl\ecl.h>
#include <msclr\marshal.h>
#include <string>
#include <strsafe.h>

using namespace std;

//
// Charset independent STL string.
//
typedef basic_string<TCHAR> tstring;
