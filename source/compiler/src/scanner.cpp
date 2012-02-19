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
#include "scanner.h"
#include "cerror.h"

using namespace compiler;
using namespace utils::conversions;

bool Scanner::IsValidCharacter(char ch)
{
    if ((safe_static_cast<unsigned>(ch + 1)) <= 256)
    {
        return (true);
    }
    ErrorReport(CError::Error_Lexical);
    return (false);
}

Scanner::Scanner()
: m_sourceFile(NULL)
, m_inputBuffer(NULL)
, m_nextToken(Token::Token_Eos)
, m_nextTokenOffset(0)
, m_currTokenOffset(0)
, m_curTokenLength(0)
{}

Scanner::~Scanner() {}

const std::string& Scanner::GetLiteralValue() const
{
    return (m_literalValue); 
}

size_t Scanner::GetNextTokenOffset() const
{
    return m_nextTokenOffset;
}

size_t Scanner::GetTokenOffset() const
{
    return m_currTokenOffset;
}

void Scanner::Reset(ISourceFile& sourceFile)
{
    ClearErrors();
    m_sourceFile = &sourceFile;
    m_inputBuffer = sourceFile.GetBuffer();
    
    assert(m_inputBuffer != NULL);
    assert(m_sourceFile != NULL);

    m_nextTokenOffset = 0;
    m_currTokenOffset = 0;
    m_curTokenLength = 0;

    m_literalValue.clear();
    m_nextToken = Scan();
}

size_t Scanner::GetTokenLength() const
{
    return m_curTokenLength;
}

Token::Kind Scanner::Next()
{
    Token::Kind curToken = m_nextToken;
    m_currLiteralValue = m_literalValue;

    m_currTokenOffset = m_nextTokenOffset;
    m_curTokenLength = m_inputBuffer->GetOffset() - m_currTokenOffset;

    bool cycle = true;
    do 
    {
        m_literalValue.clear();
        m_nextToken = Scan();
        if (m_nextToken == Token::Token_Illegal)
        {
            ErrorReport(CError::Error_Lexical);
        }
        else
        {
            break;
        }
    } while (cycle);

    return (curToken);
}

Token::Kind Scanner::Peek() const
{
    return (m_nextToken);
}

Token::Kind Scanner::Scan()
{
    SkipWhiteSpaces();
    m_nextTokenOffset = m_inputBuffer->GetOffset();

    char ch = ReadChar();
    switch (ch)
    {
    case '{':
        return (Token::Token_LBrace);
    case '[':
        return (Token::Token_LBracket);
    case '(':
        return (Token::Token_LParen);
    case '?':
        return (Token::Token_Question);
    case '}':
        return (Token::Token_RBrace);
    case ']':
        return (Token::Token_RBracket);
    case ')':
        return (Token::Token_RParen);
    case ';':
        return (Token::Token_Semicolon);
    case '+':
        if (CheckChar('+')) // ++
        {
            return (Token::Token_Inc);
        } else if (CheckChar('=')) // +=
        {
            return (Token::Token_AssignAdd);
        }
        return (Token::Token_Add); // +
    case '&':
        if (CheckChar('&')) // &&
        {
            return (Token::Token_And);
        } else if (CheckChar('=')) // &=
        {
            return (Token::Token_AssignBitwiseAnd);
        }
        return (Token::Token_BitwiseAnd); // &
    case '\"':
        return (ReadString(ch));
    case '=':
        if (CheckChar('=')) // ==
        {
            return (Token::Token_Equals);
        }
        return (Token::Token_Assign); // =
    case '|':
        if (CheckChar('|')) // ||
        {
            return (Token::Token_Or);
        } else if (CheckChar('=')) // |=
        {
            return (Token::Token_AssignBitwiseOr);
        }
        return (Token::Token_BitwiseOr); // |
    case '/':
        if (CheckChar('/')) // //
        {
            SkipSingleLineComment();
            return (Scan());
        } else if (CheckChar('*')) // /*
        {
            SkipMultiLineComment();
            return (Scan());
        } else if (CheckChar('=')) // /=
        {
            return (Token::Token_AssignDiv);
        }
        return (Token::Token_Div); // /
    case '<':
        if (CheckChar('=')) // <=
        {
            return (Token::Token_LTE);
        } else if (CheckChar('<'))
        {
            if (CheckChar('=')) // <<=
            {
                return (Token::Token_AssignLShift);
            }
            return (Token::Token_LShift); // <<
        }
        return (Token::Token_LT); // <
    case '%':
        if (CheckChar('=')) // %=
        {
            return (Token::Token_AssignMod);
        }
        return (Token::Token_Mod); // %
    case '*':
        if (CheckChar('=')) // *=
        {
            return (Token::Token_AssignMult);
        }
        return (Token::Token_Mult); // *
    case '-':
        if (CheckChar('-')) // --
        {
            return (Token::Token_Dec);
        } else if (CheckChar('=')) // -=
        {
            return (Token::Token_AssignSub);
        }
        return (Token::Token_Sub); // -
    case '~':
        return (Token::Token_BitwiseNot);
    case '^':
        if (CheckChar('=')) // ^=
        {
            return (Token::Token_AssignXor);
        }
        return (Token::Token_BitwiseXor); // ^
    case ':':
        return (Token::Token_Colon);
    case ',':
        return (Token::Token_Comma);
    case '.':
        if (CheckChar('.'))
        {
            if (CheckChar('.'))
            {
                return (Token::Token_Ellipsis); // ...
            }
            return (Token::Token_Illegal); // ..
        }
        if (IsValidCharacter(PeekChar()) && isdigit(PeekChar()))
        {
            m_literalValue += ch;
            return (ReadDigit('.'));
        }
        return (Token::Token_Dot); // .
    case '>':
        if (CheckChar('=')) // >=
        {
            return (Token::Token_GTE);
        } else if (CheckChar('>'))
        {
            if (CheckChar('=')) // >>=
            {
                return (Token::Token_AssignRShift);
            }
            return (Token::Token_RShift); // >>
        }
        return (Token::Token_GT); // >
    case '!':
        if (CheckChar('=')) // !=
        {
            return (Token::Token_NE);
        }
        return (Token::Token_Not); // !
    case 0:
        return (Token::Token_Eos);
    case '\'':
        return (ReadCharacter(ch));
    default:
        if (IsIdentifierBegin(ch))
        {
            m_literalValue += ch;
            return (ReadIdentifier(ch));
        }
        if (IsValidCharacter(ch) && isdigit(ch))
        {
            m_literalValue += ch;
            return (ReadDigit(ch));
        }
    }
    return (Token::Token_Illegal);
}

void Scanner::SkipSingleLineComment()
{
    char ch = ReadChar();
    while (ch && !(ch == '\r' || ch == '\n'))
    {
        ch = ReadChar();
    }
}

void Scanner::SkipMultiLineComment()
{
    char ch = ReadChar();
    while (ch && !(ch == '*' && CheckChar('/')))
    {
        ch = ReadChar();
    }
    // TODO: what if End of file;
}

bool Scanner::IsHexDigit(char ch)
{
    return (IsValidCharacter(ch) && isdigit(ch)) ||
        (ch >= 'a' && ch <= 'f') ||
        (ch >= 'A' && ch <= 'F');
}

Token::Kind Scanner::ReadHexDigits(bool match)
{
    if (IsHexDigit(PeekChar()))
    {
        do
        {
            ReadChar();
        } while (IsHexDigit(PeekChar()));
        return (Token::Token_NumberConstant);
    }

    if (match)
    {
        ErrorReport(CError::Error_Lexical);
    }
    return (Token::Token_Illegal);
}

Token::Kind Scanner::ReadDigit(char ch)
{
    assert(ch == '.' || isdigit(ch));
    if (ch == '0' &&
        (CheckChar('x') || CheckChar('x')))
    {
        return (ReadHexDigits(true));
    } else if (IsValidCharacter(ch) && isdigit(ch))
    {
        ReadDecDigits(false);
        if ((ch = PeekChar()) == '.')
        {
            ReadChar();
        }
    }
    if (ch == '.')
    {
        ReadDecDigits(false);
    }
    return (Token::Token_NumberConstant);
}

void Scanner::ErrorReport(CError::ErrorType type)
{
    std::string extendedInfo;
    extendedInfo += m_inputBuffer->Peek();
    AddError(IErrorsContainer::IErrorPtr(new CError(type, SourcePosition(m_sourceFile,
        m_nextTokenOffset), extendedInfo)));
}

Token::Kind Scanner::ReadDecDigits(bool match)
{
    if (IsValidCharacter(PeekChar()) && isdigit(PeekChar()))
    {
        do
        {
            ReadChar();
        } while (IsValidCharacter(PeekChar()) && isdigit(PeekChar()));
        return (Token::Token_NumberConstant);
    }

    if (match)
    {
        ErrorReport(CError::Error_Lexical);
    }
    return (Token::Token_Illegal);
}

Token::Kind Scanner::ReadCharacter(char ch)
{
    assert(ch == '\'');

    std::string charValue;
    
    bool hasNewlines = false;
    // TODO: may be need full support
    // http://msdn.microsoft.com/en-us/library/fwa1sfwk(VS.80).aspx

    ch = ReadChar();
    switch (ch)
    {
    case '\\':
        if (PeekChar())
        {
            ReadEscapeChar(charValue);
        }
        break;
    case 0:
        ErrorReport(CError::Error_LineTerminatorInCharacterConstant);
        break;
    case '\'':
        ErrorReport(CError::Error_EmptyCharacterConstant);
        break;
    case '\r':
    case '\n':
        hasNewlines = true;
        break;
    default:
        charValue += ch;
    }
    if (ReadChar() != '\'')
    {
        ErrorReport(CError::Error_Lexical);
    }
    if (hasNewlines)
    {
        ErrorReport(CError::Error_LineTerminatorInCharacterConstant);
    }

    m_literalValue = charValue;
    return (Token::Token_CharConstant);
}

Token::Kind Scanner::ReadString(char ch)
{
    assert(ch == '"');

    std::string stringValue;
    char ch0 = ch;

    bool hasNewlines = false;
    for (ch = ReadChar(); ch != ch0 && ch > 0;
        ch = ReadChar())
    {
        if (ch == '\\' && PeekChar())
        {
            ReadEscapeChar(stringValue);
        }
        else if (ch == '\r' || ch == '\n')
        {
            hasNewlines = true;
        }
        else
        {
            stringValue += ch;
        }
    }

    if (hasNewlines)
    {
        ErrorReport(CError::Error_LineTerminatorInStringLiteral);
    }
    else if (ch == 0)
    {
        ErrorReport(CError::Error_EndStreamInStringLiteral);
    }

    m_literalValue = stringValue;
    return (Token::Token_StringConstant);
}

void Scanner::ReadEscapeChar(std::string& outStr)
{
    char ch = ReadChar();
    switch (ch)
    {
    case '\r':
    case '\n':
        return;

    case 'b': ch = '\b'; break;
    case 'f': ch = '\f'; break;
    case 'n': ch = '\n'; break;
    case 'r': ch = '\r'; break;
    case 't': ch = '\t'; break;
    case 'v': ch = '\v'; break;
    case '\'': ch = '\''; break;
    case '\"': ch = '\"'; break;
    case '\\': ch = '\\'; break;
    case '\?': ch = '\?'; break;
    }
    outStr += ch;
}

Token::Kind Scanner::ReadIdentifier(char ch)
{
    assert(IsIdentifierBegin(ch));
    for (; IsIdentifierPart(ch = PeekChar()); ReadChar())
        ;

    return Token::FindKeyWord(m_literalValue);
}

bool Scanner::IsIdentifierPart(char ch)
{
    if (IsValidCharacter(ch))
    {
        return (IsIdentifierBegin(ch) || isdigit(ch));
    }
    return (false);
}

bool Scanner::IsIdentifierBegin(char ch)
{
    if (IsValidCharacter(ch))
    {
        return (ch == '_' || isalpha(ch));
    }
    return (false);
}

bool Scanner::CheckChar(char ch)
{
    if (PeekChar() == ch)
    {
        ReadChar();
        return (true);
    }
    return (false);
}

void Scanner::SkipWhiteSpaces()
{
    char ch = PeekChar();
    while (IsValidCharacter(ch) && isspace(ch))
    {
        ReadChar();
        ch = PeekChar();
    }
}

char Scanner::PeekChar()
{
    assert (m_inputBuffer != NULL);
    return (m_inputBuffer->Peek());
}

char Scanner::ReadChar()
{
    if (!m_literalValue.empty())
    {
        m_literalValue += m_inputBuffer->Peek();
    }
    return (m_inputBuffer->Next());
}