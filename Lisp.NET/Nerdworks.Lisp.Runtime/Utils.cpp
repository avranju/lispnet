#include "StdAfx.h"
#include "Utils.h"

tstring CUtils::FindReplace( const TCHAR *str, const TCHAR *find, const TCHAR *replace )
{
	tstring s = str;
	tstring::size_type pos = s.find( find );
	size_t findlen = _tcslen( find );

	while( pos != tstring::npos )
	{
		s.replace( pos, findlen, replace );
		pos = s.find( find );
	}

	return s;
}

wstring CUtils::AnsiToUnicode(string str)
{
	wstring wstr;
	wchar_t *pwszStr = NULL;
	int length = 0;

	//
	// find the length of the unicode string
	//
	length = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, str.c_str(), str.length(), NULL, 0 );
	if( length == 0 )
		return wstr;

	//
	// convert to unicode
	//
	pwszStr = new wchar_t[length + 1];
	if( ( length = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, str.c_str(), str.length(), pwszStr, length ) ) > 0 )
	{
		pwszStr[length] = L'\0';
		wstr = pwszStr;
	}

	delete [] pwszStr;
	return wstr;
}

string CUtils::UnicodeToAnsi(wstring wstr)
{
	string str;
	char *pszStr = NULL;
	int length;

	//
	// find the length of the ansi string
	//
	length = WideCharToMultiByte( CP_ACP, 0, wstr.c_str(), wstr.length(), NULL, 0, NULL, NULL );
	if( length == 0 )
		return str;

	//
	// convert to ansi
	//
	pszStr = new char[length + 1];
	if( ( length = WideCharToMultiByte( CP_ACP, 0, wstr.c_str(), wstr.length(), pszStr, length, NULL, NULL ) ) > 0 )
	{
		pszStr[length] = '\0';
		str = pszStr;
	}

	delete [] pszStr;
	return str;
}

wchar_t *CUtils::CopyString(const wchar_t *pwszStr)
{
	//
	// this routine copies "pwszStr" to fresh memory
	// allocated with CoTaskMemAlloc
	//
	int length = wcslen( pwszStr ) + 1;
	STRSAFE_LPWSTR pwszCopy = (STRSAFE_LPWSTR)CoTaskMemAlloc( length * sizeof( wchar_t ) );
	StringCchCopyW( pwszCopy, length, pwszStr );
	pwszCopy[length - 1] = L'\0';
	return pwszCopy;
}
