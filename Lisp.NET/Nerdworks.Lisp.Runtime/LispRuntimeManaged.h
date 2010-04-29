#pragma once

using namespace System;
using namespace System::Runtime::InteropServices;

namespace Nerdworks { namespace Lisp { namespace Runtime {

	public ref class LispRuntime
	{
	protected:
		CLispRuntime *runtime;

	public:
		LispRuntime()
		{
			runtime = new CLispRuntime();
		}

		~LispRuntime()
		{
			this->!LispRuntime();
		}

		!LispRuntime()
		{
			delete runtime;
		}

		static void Boot();
		static void Shutdown();
		EvalResult^ Eval(String^ code);
	};

}; }; };
