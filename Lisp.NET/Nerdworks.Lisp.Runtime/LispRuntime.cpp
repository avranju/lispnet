#include "StdAfx.h"
#include "LispRuntime.h"
#include "Utils.h"

//
// static member used to track if the Lisp environment
// has been initialized via "CLispRuntime::Boot"
//
bool CLispRuntime::s_bInitialized = false;

//
// win32 thread id of the thread that called "CLispRuntime::Boot"
//
DWORD CLispRuntime::s_dwInitThreadId = (DWORD)-1;

//
// gensymed name of the safe eval macro
//
tstring	CLispRuntime::s_SafeEvalMacro;

//
// template safe eval form
//
tstring	CLispRuntime::s_SafeEvalForm;

void CLispRuntime::Boot()
{
	if( CLispRuntime::s_bInitialized == false )
	{
		//
		// initialize lisp env
		//
		char *argv[1];
		argv[0] = "";
		cl_boot( 1, argv );

		//
		// save the thread id
		//
		CLispRuntime::s_dwInitThreadId = GetCurrentThreadId();

		//
		// define the "redir-io-safe-eval" macro using a dynamically
		// generated symbol as its name
		//
		s_SafeEvalMacro = CLispRuntime::GenSym();
		CLispRuntime::InternalEval( CUtils::FindReplace( REDIR_IO_SAFE_EVAL, _T( "<<s>>" ), s_SafeEvalMacro.c_str() ).c_str() );

		//
		// build the safe eval template
		//
		s_SafeEvalForm = CUtils::FindReplace( SAFE_EVAL_FORM, _T( "<<s1>>" ), s_SafeEvalMacro.c_str() );
		CLispRuntime::s_bInitialized = true;
	}
}

void CLispRuntime::Shutdown()
{
	if( CLispRuntime::s_bInitialized == true )
	{
		cl_shutdown();
		CLispRuntime::s_bInitialized = false;
	}
}

void CLispRuntime::InternalEval( const TCHAR *pszForm )
{
	si_safe_eval( 3, c_string_to_object( T2CA( pszForm ) ), Cnil, OBJNULL );
}

tstring CLispRuntime::ToString(cl_object obj)
{
	//
	// if "obj" is a string then convert to a "tstring"
	//
	if( cl_stringp( obj ) != Cnil )
	{
		return W2CT( ecl_string_pointer_safe( ecl_null_terminated_string( obj ) ) );
	}

	return _T( "" );
}

tstring CLispRuntime::GenSym()
{
	return A2CT( ecl_base_string_pointer_safe( cl_symbol_name( cl_gensym( 0 ) ) ) );
}

bool CLispRuntime::Eval( const TCHAR *pszForm,
						 tstring& strValue,
						 tstring& strStandardOutput,
						 tstring& strError )
{
	//
	// if the lisp environment has not been initialized then do nothing
	//
	if( CLispRuntime::s_bInitialized == false )
	{
		strError = _T( "Lisp environment has not been initialized.  First call \"CLispRuntime::Boot\"." );
		return false;
	}

	//
	// if the current thread is not the same as the thread that invoked "CLispRuntime::Boot"
	// then call "ecl_import_current_thread"
	//
	if( GetCurrentThreadId() != CLispRuntime::s_dwInitThreadId )
		ecl_import_current_thread( Cnil, Cnil );

	//
	// build the safe eval form and eval
	//
	tstring strForm = CUtils::FindReplace( CLispRuntime::s_SafeEvalForm.c_str(), _T( "<<s2>>" ), pszForm );
	cl_object form = c_string_to_object( CT2CA( strForm.c_str() ) );
	cl_object result = cl_eval( form );

	//
	// "result" is a list with 4 strings - value, std out, err out, trace out
	//
	strValue = CLispRuntime::ToString( cl_first( result ) );
	strStandardOutput = CLispRuntime::ToString( cl_second( result ) );
	strError = CLispRuntime::ToString( cl_third( result ) );
	//tstring trc = CLispRuntime::ToString( cl_fourth( result ) );

	//
	// release the lisp resources that might have been allocated earlier
	//
	if( GetCurrentThreadId() != CLispRuntime::s_dwInitThreadId )
		ecl_release_current_thread();

	return true;
}

cl_object CLispRuntime::ecl_check_type_string(cl_object fun, cl_object p)
{
	cl_type t;
	while( ( ( t = type_of(p) ) != t_base_string ) &&
		   ( t != t_string ) )
	{
		p = ecl_type_error(fun,"",p,ECL_SYM("STRING",803));
	}

	return p;
}
wstring CLispRuntime::ecl_string_pointer_safe(cl_object f)
{
	int32_t *s;
	/* FIXME! Is there a better function name? */
	f = ecl_check_type_string(ECL_SYM("si::make-foreign-data-from-array", 1321), f);
	s = (int32_t *)(f->string.self);

	//
	// copy the null-terminated array of int32s to a wstring;
	// WARNING:
	//	We are assuming that all unicode characters are only 2
	//	bytes long.  If any code point in this string uses the
	//	3rd or the 4th bytes then they are simply going to be
	//	ignored.
	//
	wstring str;
	if( s != NULL )
		while( s < ( f->string.self + f->string.fillp ) )
			str.append( 1, (wchar_t)*s++ );

	if (ECL_ARRAY_HAS_FILL_POINTER_P(f) && f->string.self[f->string.fillp] != 0) {
		FEerror("Cannot coerce a string with fill pointer to (char *)", 0);
	}

	return str;
}

cl_object CLispRuntime::ecl_null_terminated_string(cl_object f)
{
	/* FIXME! Is there a better function name? */
	f = ecl_check_type_string(ECL_SYM("si::make-foreign-data-from-array", 1321), f);
	if (ECL_ARRAY_HAS_FILL_POINTER_P(f) &&
            f->string.self[f->string.fillp] != 0) {
		return cl_copy_seq(f);
	} else {
		return f;
	}
}
