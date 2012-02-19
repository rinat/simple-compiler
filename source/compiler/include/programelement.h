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
#ifndef __Compiler_ProgramElement_h__
#define __Compiler_ProgramElement_h__

namespace compiler
{
    struct ProgramElement
    {
        Token::Kind m_kind;
        std::string m_value;
        size_t m_beginOffset;
        std::string m_index;
        bool m_isUnaryOperator;

        ProgramElement()
            : m_kind(Token::Token_Eos)			
            , m_beginOffset(0)
            , m_isUnaryOperator(false)
        {}
        ProgramElement(std::string elementValue, Token::Kind kind, size_t offset)
            : m_kind(kind)
            , m_value(elementValue)
            , m_beginOffset(offset)
            , m_isUnaryOperator(false)
        {}

    public:
        bool IsArray() const
        {
            return !m_index.empty();
        }
    };
}

#endif