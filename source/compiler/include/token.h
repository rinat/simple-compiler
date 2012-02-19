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
#ifndef __Compiler_Token_h__
#define __Compiler_Token_h__

/*
    simple C tokens
*/

#include "noncopyable.h"

namespace compiler
{
    class Token : private utils::NonCopyable
    {
    public:
        enum Kind
        {
            // keywords
            Token_Illegal  = -1,
            Token_Eos      = 0,
            Token_NumberConstant = 1,
            Token_StringConstant = 2,
            Token_LParen = 3,
            Token_RParen = 4,
            Token_LBracket = 5,
            Token_RBracket = 6,
            Token_LBrace = 7,
            Token_RBrace = 8,
            Token_Semicolon = 9,
            Token_char = 10,
            Token_break = 11,
            Token_float = 12,
            Token_int = 13,
            Token_double = 14,
            Token_main = 15,
            Token_GT = 16,
            Token_GTE = 17,
            Token_NE = 18,
            Token_Equals = 19,
            Token_LT = 20,
            Token_LTE = 21,
            Token_Id = 22,
            Token_Mult = 23,
            Token_Div = 24,
            Token_Comma = 25,
            Token_Add = 26,
            Token_Assign = 27,
            Token_Sub = 28,
            Token_if = 29,
            Token_else = 30,
            Token_for = 31,
            Token_do = 32,
            Token_while = 33,
            Token_read = 34,
            Token_write = 35,
            Token_void = 36,

            Token_CharConstant,

            Token_auto,
            Token_case,
            Token_const,
            Token_continue,
            Token_default,
            Token_enum,
            Token_extern,
            Token_goto,
            Token_long,
            Token_register,
            Token_return,
            Token_short,
            Token_signed,
            Token_sizeof,
            Token_static,
            Token_struct,
            Token_switch,
            Token_typedef,
            Token_union,
            Token_unsigned,
            Token_volatile,
            // operators
            Token_And,
            Token_AssignAdd,
            Token_AssignBitwiseAnd,
            Token_AssignBitwiseOr,
            Token_AssignDiv,
            Token_AssignLShift,
            Token_AssignRShift,
            Token_AssignMod,
            Token_AssignMult,
            Token_AssignSub,
            Token_AssignURShift,
            Token_BitwiseAnd,
            Token_BitwiseNot,
            Token_BitwiseOr,
            Token_BitwiseXor,
            Token_Colon,
            Token_Dec,
            Token_Dot,
            Token_Inc,
            Token_LShift,
            Token_Mod,
            Token_Not,
            Token_Or,
            Token_Question,
            Token_RShift,
            Token_Ellipsis,
            Token_AssignXor,
        };

        static Token::Kind FindKeyWord(const std::string& inStr);
        static std::string TokenToString(Token::Kind token);

    private:
        typedef std::map<std::string, Kind> KeyWords;
        static const KeyWords& GetKeyWords();
        static KeyWords m_keyWords;
    };
}

#endif