#pragma once

//
// the "redir-io-safe-eval" macro
//
#define REDIR_IO_SAFE_EVAL \
_T( "(defmacro <<s>> (&body forms)" ) \
_T( "  (let ((std (gensym))" ) \
_T( "	(err (gensym))" ) \
_T( "	(trc (gensym))" ) \
_T( "	(val (gensym))" ) \
_T( "	(erri (gensym)))" ) \
_T( "	`(progn" ) \
_T( "	   (setf ,std (make-string-output-stream))" ) \
_T( "	   (setf ,err (make-string-output-stream))" ) \
_T( "	   (setf ,trc (make-string-output-stream))" ) \
_T( "	   (setf ,val nil)" ) \
_T( "	   (let ((*standard-output* ,std)" ) \
_T( "		 (*error-output* ,err)" ) \
_T( "		 (*trace-output* ,trc))" ) \
_T( "	     (handler-case" ) \
_T( "		(setf ,val (ignore-errors (progn ,@forms)))" ) \
_T( "		(error (,erri) (format *error-output* \"~a\" ,erri))))" ) \
_T( "	   (values ,val ,std ,err ,trc))))" )

//
// lisp form for evaluating expressions using "redir-io-safe-eval"
//
#define SAFE_EVAL_FORM \
_T( "(multiple-value-bind" ) \
_T( "  (val std err trc)" ) \
_T( "  (<<s1>> <<s2>>)" ) \
_T( "  (list" ) \
_T( "     (format nil \"~a\" val) " ) \
_T( "	  (get-output-stream-string std)" ) \
_T( "	  (get-output-stream-string err)" ) \
_T( "	  (get-output-stream-string trc)))" )

class CLispRuntime
{
protected:
	static bool		s_bInitialized;		// used to track if the lisp env has been initialized
	static DWORD	s_dwInitThreadId;	// win32 thread id of the thread that called "Boot"
	static tstring	s_SafeEvalMacro;	// gensymed name of the safe eval macro
	static tstring	s_SafeEvalForm;		// template safe eval form

	//
	// evaluates a lisp form without error checks; form MUST be
	// valid and MUST NOT throw errors
	//
	static void InternalEval( const TCHAR *pszForm );

	static wstring ecl_string_pointer_safe(cl_object f);
	static cl_object ecl_null_terminated_string(cl_object f);
	static cl_object ecl_check_type_string(cl_object fun, cl_object p);

public:
	static void Boot();
	static void Shutdown();
	static tstring GenSym();
	static tstring ToString( cl_object obj );
	bool Eval( const TCHAR *pszForm,
			   tstring& strValue,
			   tstring& strStandardOutput,
			   tstring& strError );
};
