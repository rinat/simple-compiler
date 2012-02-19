/*
* This code is under GNU Lesser General Public License.
* Simple C Compiler to Low Level Virtual Machine representation
*
* Authors: Rinat Shaikhutdinov (aka Sagit) - architecture, compiler core
*          Alexei Zhdanov (aka Alex) - parser
*          Timofey Tatarinov (aka Tikers) - code generator
*
* University Mari State University
* Faculty Informatics and Computer Science
* Course 4
* Specialization PS-41
* Subject Theory of programming languages and translation methods
* Yoshkar-Ola 2010
*/
#include "stdafx.h"
#include "cerror.h"

using namespace compiler;

std::map<CError::ErrorType, std::string> CError::m_errorDescriptions;

CError::CError(ErrorType errorType,
               const SourcePosition& range,
               const std::string &extendedInfo)
: m_errorType(errorType)
, m_sourceRange(range)
{
    InitErrorInfo();
    m_errorDescription = m_errorDescriptions[m_errorType];
    size_t index = m_errorDescription.find("%s");
    if (index != -1)
    {
        m_errorDescription.replace(index, 2, extendedInfo);
    }
}

void CError::InitErrorInfo()
{
    if (m_errorDescriptions.size() == 0)
    {
        m_errorDescriptions[Error_Lexical]                           = "Lexical error.";
        m_errorDescriptions[Error_EndStreamInCharacterConstant]      = "End stream in character constant.";
        m_errorDescriptions[Error_EmptyCharacterConstant]            = "Empty character constant.";
        m_errorDescriptions[Error_LineTerminatorInCharacterConstant] = "Line terminator in character constant.";
        m_errorDescriptions[Error_LineTerminatorInStringLiteral]     = "Line terminator in string literal.";
        m_errorDescriptions[Error_EndStreamInStringLiteral]          = "End stream in string literal.";
		m_errorDescriptions[Error_GenericParserError]				 = "Syntax error. %s";
        m_errorDescriptions[Error_TypesMismatch]			    	 = "Types mismatch: %s";
        m_errorDescriptions[Error_UndefinedIdentifier]		    	 = "Identifier '%s' not found.";
        m_errorDescriptions[Error_Break]		                	 = "\"break\" is not placed in cycle.";
		m_errorDescriptions[Error_NotArray]		                	 = "Identifier '%s' is not array.";
        m_errorDescriptions[Error_WrongArrayIndexType]            	 = "Type of array index must be 'int'. Array: '%s'";
		m_errorDescriptions[Error_ReadArray]						 = "Arrays can't be read. '%s' hasn't been read.";
		m_errorDescriptions[Error_TwoDimensionArrayNotSupported]	 = "Two dimension array is not supported.";
        m_errorDescriptions[Error_VariableToArray]					 = "Can't use array like variable. Array: '%s'.";
        m_errorDescriptions[Error_Redefinition]    					 = "Redefinition of variable '%s'.";
        m_errorDescriptions[Error_StringsNotSupported]    			 = "Strings are not supported.";
        m_errorDescriptions[Error_DoWhileNoSupported]       		 = "Instruction 'do while' is not supported.";
        m_errorDescriptions[Error_ForNotSupported]  				 = "Instruction 'for' is not supported.";
        m_errorDescriptions[Error_UnknownSizeOfArray]  				 = "Unknown size of array '%s'. Array size must be integer number > 0.";
        m_errorDescriptions[Error_SizeOfArrayNotNumber]				 = "Size of array can't consists of variables. Array size must be integer number. Array: '%s'";

    }
}

CError::ErrorType CError::GetErrorType() const
{
    return (m_errorType);
}

const std::string& CError::GetErrorDescription() const
{
    return (m_errorDescription);
}

const ISourcePosition& CError::GetSourcePosition() const
{
    return (m_sourceRange);
}