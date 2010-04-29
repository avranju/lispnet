#include "StdAfx.h"
#include "LispRuntime.h"
#include "EvalResult.h"
#include "LispRuntimeManaged.h"

using namespace System::Runtime::InteropServices;
using namespace msclr::interop;
using namespace Nerdworks::Lisp::Runtime;

void LispRuntime::Boot()
{
	CLispRuntime::Boot();
}

void LispRuntime::Shutdown()
{
	CLispRuntime::Shutdown();
}

EvalResult^ LispRuntime::Eval(System::String ^code)
{
	marshal_context ctx;
	tstring strValue, strStandard, strError;
	EvalResult^ result = gcnew EvalResult();

	result->Success = runtime->Eval( ctx.marshal_as<const TCHAR *>( code ), strValue, strStandard, strError );
	if( result->Success == true )
	{
		//
		// convert to managed strings
		//
		result->Value = marshal_as<String^>( strValue.c_str() );
		result->StandardOutput = marshal_as<String^>( strStandard.c_str() );
		result->ErrorOutput = marshal_as<String^>( strError.c_str() );
	}

	return result;
}
