#include "StdAfx.h"
#include "EvalResult.h"

using namespace Nerdworks::Lisp::Runtime;

//
// Returns a string representation of this result object.
//
String^ EvalResult::ToString()
{
	return String::Format("{0}{1}{2}{3}",
		(StandardOutput != nullptr && StandardOutput->Length > 0) ? StandardOutput + Environment::NewLine : String::Empty,
		(ErrorOutput != nullptr && ErrorOutput->Length > 0) ? "ERROR: " + ErrorOutput + Environment::NewLine : String::Empty,
		(TraceOutput != nullptr && TraceOutput->Length > 0) ? "TRACE: " + TraceOutput + Environment::NewLine : String::Empty,
		(Value != nullptr && Value->Length > 0) ? Value : String::Empty);
}
