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
#include "token.h"

using namespace compiler;

Token::KeyWords Token::m_keyWords;

Token::Kind Token::FindKeyWord(const std::string& inStr)
{
    const KeyWords& keywords = GetKeyWords();
    KeyWords::const_iterator it = keywords.find(inStr);
    return ((it != keywords.end()) ? it->second : Token_Id);
}

const Token::KeyWords& Token::GetKeyWords()
{
    if (m_keyWords.empty())
    {
        m_keyWords["read"] = Token::Token_read;
        m_keyWords["write"] = Token::Token_write;
        m_keyWords["main"] = Token::Token_main;
        m_keyWords["auto"] = Token::Token_auto;
        m_keyWords["break"] = Token::Token_break;
        m_keyWords["case"] = Token::Token_case;
        m_keyWords["char"] = Token::Token_char;
        m_keyWords["const"] = Token::Token_const;
        m_keyWords["continue"] = Token::Token_continue;
        m_keyWords["default"] = Token::Token_default;
        m_keyWords["do"] = Token::Token_do;
        m_keyWords["double"] = Token::Token_double;
        m_keyWords["else"] = Token::Token_else;
        m_keyWords["enum"] = Token::Token_enum;
        m_keyWords["extern"] = Token::Token_extern;
        m_keyWords["float"] = Token::Token_float;
        m_keyWords["for"] = Token::Token_for;
        m_keyWords["goto"] = Token::Token_goto;
        m_keyWords["if"] = Token::Token_if;
        m_keyWords["int"] = Token::Token_int;
        m_keyWords["long"] = Token::Token_long;
        m_keyWords["register"] = Token::Token_register;
        m_keyWords["return"] = Token::Token_return;
        m_keyWords["short"] = Token::Token_short;
        m_keyWords["signed"] = Token::Token_signed;
        m_keyWords["sizeof"] = Token::Token_sizeof;
        m_keyWords["static"] = Token::Token_static;
        m_keyWords["struct"] = Token::Token_struct;
        m_keyWords["switch"] = Token::Token_switch;
        m_keyWords["typedef"] = Token::Token_typedef;
        m_keyWords["union"] = Token::Token_union;
        m_keyWords["unsigned"] = Token::Token_unsigned;
        m_keyWords["void"] = Token::Token_void;
        m_keyWords["volatile"] = Token::Token_volatile;
        m_keyWords["while"] = Token::Token_while;
    }
    return (m_keyWords);
}

std::string Token::TokenToString(Token::Kind token)
{
    switch (token)
    {
    // operators
    case Token_Add: return std::string("+");
    case Token_AssignAdd: return std::string("+=");
    case Token_Inc: return std::string("++");

    case Token_BitwiseAnd: return std::string("&");
    case Token_And: return std::string("&&");
    case Token_AssignBitwiseAnd: return std::string("&=");

    case Token_Assign: return std::string("=");
    case Token_Equals: return std::string("==");

    case Token_BitwiseOr: return std::string("|");
    case Token_Or: return std::string("||");
    case Token_AssignBitwiseOr: return std::string("|=");

    case Token_Div: return std::string("/");
    case Token_AssignDiv: return std::string("/=");

    case Token_LT: return std::string("<");
    case Token_LTE: return std::string("<=");
    case Token_LShift: return std::string("<<");
    case Token_AssignLShift: return std::string("<<=");

    case Token_Mod: return std::string("%");
    case Token_AssignMod: return std::string("%=");

    case Token_Mult: return std::string("*");
    case Token_AssignMult: return std::string("*=");

    case Token_Sub: return std::string("-");
    case Token_Dec: return std::string("--");
    case Token_AssignSub: return std::string("-=");

    case Token_BitwiseXor: return std::string("^");
    case Token_AssignXor: return std::string("^=");

    case Token_Dot: return std::string(".");
    case Token_Ellipsis: return std::string("...");

    case Token_GT: return std::string(">");
    case Token_GTE: return std::string(">=");
    case Token_RShift: return std::string(">>");
    case Token_AssignRShift: return std::string(">>=");

    case Token_Not: return std::string("!");
    case Token_NE: return std::string("!=");

    case Token_LBrace: return std::string("{");
    case Token_LBracket: return std::string("[");
    case Token_LParen: return std::string("(");
    case Token_Question: return std::string("?");
    case Token_RBrace: return std::string("}");
    case Token_RBracket: return std::string("]");
    case Token_RParen: return std::string(")");
    case Token_Semicolon: return std::string(";");
    case Token_Colon: return std::string(":");
    case Token_Comma: return std::string(",");
    case Token_BitwiseNot: return std::string("~");

    case Token::Token_StringConstant: return std::string("string constant");

    // keywords
    case Token::Token_read: return std::string("read");
    case Token::Token_write: return std::string("write");
    case Token::Token_main: return std::string("main");
    case Token::Token_auto: return std::string("auto");
    case Token::Token_break: return std::string("break");
    case Token::Token_case: return std::string("case");
    case Token::Token_char: return std::string("char");
    case Token::Token_const: return std::string("const");
    case Token::Token_continue: return std::string("continue");
    case Token::Token_default: return std::string("default");
    case Token::Token_do: return std::string("do");
    case Token::Token_double: return std::string("double");
    case Token::Token_else: return std::string("else");
    case Token::Token_enum: return std::string("enum");
    case Token::Token_extern: return std::string("extern");
    case Token::Token_float: return std::string("float");
    case Token::Token_for: return std::string("for");
    case Token::Token_goto: return std::string("goto");
    case Token::Token_if: return std::string("if");
    case Token::Token_int: return std::string("int");
    case Token::Token_long: return std::string("long");
    case Token::Token_register: return std::string("register");
    case Token::Token_return: return std::string("return");
    case Token::Token_short: return std::string("short");
    case Token::Token_signed: return std::string("signed");
    case Token::Token_sizeof: return std::string("sizeof");
    case Token::Token_static: return std::string("static");
    case Token::Token_struct: return std::string("struct");
    case Token::Token_switch: return std::string("switch");
    case Token::Token_typedef: return std::string("typedef");
    case Token::Token_union: return std::string("union");
    case Token::Token_unsigned: return std::string("unsigned");
    case Token::Token_void: return std::string("void");
    case Token::Token_volatile: return std::string("volatile");
    case Token::Token_while: return std::string("while");

    case Token_Eos: return std::string("end of program");
    }
    return std::string();
}