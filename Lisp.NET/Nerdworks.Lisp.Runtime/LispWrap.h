#pragma once

#ifdef LISPWRAP_EXPORTS
	#define LISPWRAP_EXIMP		__declspec(dllexport)
#else
	#define LISPWRAP_EXIMP		__declspec(dllimport)
#endif

extern "C" {
	void LISPWRAP_EXIMP __stdcall LispBoot();
	void LISPWRAP_EXIMP __stdcall LispShutdown();
	HANDLE LISPWRAP_EXIMP __stdcall LispCreateRuntime();
	BOOL LISPWRAP_EXIMP __stdcall LispEval(
						 HANDLE hLispRuntime,
						 PWCHAR pszCode,
						 PWCHAR *ppszValue,
						 PWCHAR *ppszStandard,
						 PWCHAR *ppszError );
	void LISPWRAP_EXIMP __stdcall LispDeleteRuntime( HANDLE hLispRuntime );
};
