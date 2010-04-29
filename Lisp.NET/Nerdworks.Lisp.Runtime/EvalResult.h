#pragma once

using namespace System;

namespace Nerdworks { namespace Lisp { namespace Runtime {

	public ref class EvalResult
	{
	protected:
		bool success;
		String ^value, ^standard, ^error, ^trace;

	public:
		//
        // Empty eval result object.
        //
        initonly static EvalResult^ Empty;

	public:
		EvalResult()
		{
			this->success = false;
		}

		static EvalResult()
		{
			Empty = gcnew EvalResult();
		}

		//
		// Gets/sets a flag to indicate whether evaluation was successful or not.
		//
		property bool Success
		{
			bool get() { return success; }
			void set( bool value ) { success = value; }
		}

		//
		// Gets or sets the value.
		//
		property String^ Value
		{
			String^ get() { return value; }
			void set( String^ val ) { value = val; }
		}

		//
        // Gets or sets the standard output.
        // 
        property String^ StandardOutput
		{
			String^ get() { return standard; }
			void set( String^ val ) { standard = val; }
		}

        //
        // Gets or sets the error output.
        // 
        property String^ ErrorOutput
		{
			String^ get() { return error; }
			void set( String^ val ) { error = val; }
		}

        //
        // Gets or sets the trace output.
        // 
        property String^ TraceOutput
		{
			String^ get() { return trace; }
			void set( String^ val ) { trace = val; }
		}

        //
        // Returns a string representation of this result object.
		//
        virtual String^ ToString() override;
	};

}; }; };
