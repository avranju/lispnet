#include "StdAfx.h"
#include "LispWrap.h"
#include "LispRuntime.h"
#include "Utils.h"

void __stdcall LispBoot()
{
	CLispRuntime::Boot();
}

void __stdcall LispShutdown()
{
	CLispRuntime::Shutdown();
}

HANDLE __stdcall LispCreateRuntime()
{
	return reinterpret_cast<HANDLE>( new CLispRuntime() );
}

BOOL __stdcall LispEval( HANDLE hLispRuntime,
						 PWCHAR pszCode,
						 PWCHAR *ppszValue,
						 PWCHAR *ppszStandard,
						 PWCHAR *ppszError )
{
	//
	// validate input
	//
	if( hLispRuntime == NULL ||
		pszCode == NULL ||
		ppszValue == NULL ||
		ppszStandard == NULL ||
		ppszError == NULL )
	{
		return FALSE;
	}

	*ppszValue = *ppszStandard = *ppszError = NULL;

	CLispRuntime *pRuntime = reinterpret_cast<CLispRuntime *>( hLispRuntime );
	tstring strValue, strStandard, strError;

	bool result = pRuntime->Eval( CW2CT( pszCode ),
								  strValue,
								  strStandard,
								  strError );
	if( result )
	{
		if( strValue.empty() == false )
			*ppszValue = CUtils::CopyString( CT2CW( strValue.c_str() ) );
		if( strStandard.empty() == false )
			*ppszStandard = CUtils::CopyString( CT2CW( strStandard.c_str() ) );
		if( strError.empty() == false )
			*ppszError = CUtils::CopyString( CT2CW( strError.c_str() ) );
	}

	return ( result ) ? TRUE : FALSE;
}

void __stdcall LispDeleteRuntime( HANDLE hLispRuntime )
{
	delete reinterpret_cast<CLispRuntime *>( hLispRuntime );
}
