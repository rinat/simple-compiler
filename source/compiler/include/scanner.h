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
#ifndef __Compiler_Scanner_h__
#define __Compiler_Scanner_h__

/*
    language tokens scanner
*/

#include "token.h"
#include "cerror.h"
#include "cerrorscontainer.h"

namespace compiler
{
    class ISourceFile;

    class Scanner :
        public CErrorsContainer,
        private utils::NonCopyable
    {
    public:
        Scanner();
        ~Scanner();

        void Reset(ISourceFile&);
        size_t GetNextTokenOffset() const;
        size_t GetTokenOffset() const;
        size_t GetTokenLength() const;

        const std::string& GetLiteralValue() const;
        Token::Kind Next();
        Token::Kind Peek() const;

    private:
        void ErrorReport(CError::ErrorType);
        Token::Kind Scan();

        void SkipWhiteSpaces();
        void SkipMultiLineComment();

        void SkipSingleLineComment();
        char PeekChar();
        char ReadChar();
        Token::Kind ReadIdentifier(char ch);
        Token::Kind ReadDigit(char ch);
        Token::Kind ReadDecDigits(bool match);
        Token::Kind ReadHexDigits(bool match);
        Token::Kind ReadString(char ch);
        Token::Kind ReadCharacter(char ch);
        void ReadEscapeChar(std::string& outStr);
        bool CheckChar(char ch);

    private:
        bool IsIdentifierBegin(char ch);
        bool IsIdentifierPart(char ch);
        bool IsHexDigit(char ch);
        bool IsValidCharacter(char ch);

    private:
        ISourceFile * m_sourceFile;
        ITextBuffer* m_inputBuffer;
        Token::Kind  m_nextToken;

        size_t m_nextTokenOffset;
        size_t m_currTokenOffset;
        size_t m_curTokenLength;

        std::string m_literalValue;
        std::string m_currLiteralValue;
    };
}

#endif