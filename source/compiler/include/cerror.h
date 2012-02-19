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
#ifndef __Compiler_CError_h__
#define __Compiler_CError_h__

#include "sourceposition.h"
#include "ierror.h"

namespace compiler
{
    class CError : public IError
    {
    public:
        enum ErrorType
        {
            Error_Lexical = 0,
            Error_EndStreamInCharacterConstant,
            Error_EmptyCharacterConstant,
            Error_LineTerminatorInCharacterConstant,
            Error_LineTerminatorInStringLiteral,
            Error_EndStreamInStringLiteral,
            Error_GenericParserError,

            Error_TypesMismatch,           
            Error_UndefinedIdentifier,
            Error_Break,
            Error_NotArray,
            Error_WrongArrayIndexType,
            Error_ReadArray,
            Error_TwoDimensionArrayNotSupported,
            Error_VariableToArray,
            Error_Redefinition,
            Error_StringsNotSupported,
            Error_DoWhileNoSupported,
            Error_ForNotSupported,
            Error_UnknownSizeOfArray,
            Error_SizeOfArrayNotNumber,
        };

        CError(ErrorType errorType,
            const SourcePosition& range = SourcePosition(),
            const std::string &extendedInfo = std::string());

		ErrorType GetErrorType() const;
        
        virtual const std::string& GetErrorDescription() const;
        virtual const ISourcePosition& GetSourcePosition() const;

    private:
        void InitErrorInfo();

    private:
		ErrorType m_errorType;
        SourcePosition m_sourceRange;
        std::string m_errorDescription;
        static std::map<ErrorType, std::string> m_errorDescriptions;
    };
}

#endif