#pragma once

#define W2CT( s )	CW2CT( s )
#define CT2CA( s)	T2CA( s )
#define W2CA( s )	( CUtils::UnicodeToAnsi( wstring( s ) ).c_str() )
#define A2CW( s )	( CUtils::AnsiToUnicode( string( s ) ).c_str() )

#ifdef _UNICODE

#define T2CA( s )	W2CA( s )
#define A2CT( s )	A2CW( s )
#define CW2CT( s )	s
#define CT2CW( s )	s

#else

#define T2CA( s )	s
#define A2CT( s )	s
#define CW2CT( s )	W2CA( s )
#define CT2CW( s )	A2CW( s )

#endif

class CUtils
{
public:
	static tstring FindReplace( const TCHAR *str, const TCHAR *find, const TCHAR *replace );
	static wstring AnsiToUnicode( string str );
	static string UnicodeToAnsi( wstring wstr );
	static wchar_t *CopyString( const wchar_t *pwszStr );
};
