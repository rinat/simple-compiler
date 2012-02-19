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
#include "ScannerTest.h"
#include "scanner.h"
#include "sourcefile.h"

using namespace compiler;

class ScannerTester : private utils::NonCopyable
{
public:
    ScannerTester(Scanner &scanner, const std::string &source)
        : _scanner(scanner)
    {
        m_sourceFile.reset(new SourceFile("test.our"));
        ITextBuffer *buffer = m_sourceFile->GetBuffer(source);
        buffer;
        TEST_CHECK(buffer != NULL);

        _scanner.Reset(*m_sourceFile);
    }

private:
    Scanner &_scanner;
    std::auto_ptr<SourceFile> m_sourceFile;
};

void PrintErrors(IErrorsContainer& container)
{
    for (size_t index = 0; index < container.GetErrorsCount(); ++index)
    {
        const IError &error = container.GetError(index);
        std::cout << error.GetSourcePosition().GetSourcePath() << "(Line: "
            << error.GetSourcePosition().GetLine() + 1
            << " - "
            << error.GetErrorDescription() << '\n';
    }
}

void ScannerTest::StartTest()
{
    ScannerSwitch();
    ScannerFunction();
    ScannerAri();
    ScannerSimple();
    ScannerSatart();
    ScannerNextWithComments();
    ScannerEscapeSymbols();
    ScannerCharLiteral();
    ScannerStringLiteral();
    ScannerNext();
    ScannerNotLatin();
}

void ScannerTest::ScannerNotLatin()
{
    Scanner scanner;
    ScannerTester tester(scanner, "mainñêàíåð () {}");

    TEST_CHECK(scanner.GetTokenOffset() == 0);

    TEST_CHECK(scanner.Next() == Token::Token_main);

    TEST_CHECK(scanner.HasErrors());
}

void ScannerTest::ScannerNext()
{
    Scanner scanner;
    ScannerTester tester(scanner, "auto break case char const continue\
                                  default /*comment skip*/ do   double else enum /*comment skip*/ \
                                  extern float for goto if int\
                                  long  register return /*comment skip*/short signed /*comment skip*/\
                                  sizeof static struct switch\
                                  typedef         union  /*comment skip*/unsigned void volatile /*comment skip*/\
                                  while");

    TEST_CHECK(scanner.GetTokenOffset() == 0);

    TEST_CHECK(scanner.Next() == Token::Token_auto);
    TEST_CHECK(scanner.Next() == Token::Token_break);
    TEST_CHECK(scanner.Next() == Token::Token_case);
    TEST_CHECK(scanner.Next() == Token::Token_char);
    TEST_CHECK(scanner.Next() == Token::Token_const);
    TEST_CHECK(scanner.Next() == Token::Token_continue);
    TEST_CHECK(scanner.Next() == Token::Token_default);
    TEST_CHECK(scanner.Next() == Token::Token_do);
    TEST_CHECK(scanner.Next() == Token::Token_double);
    TEST_CHECK(scanner.Next() == Token::Token_else);
    TEST_CHECK(scanner.Next() == Token::Token_enum);
    TEST_CHECK(scanner.Next() == Token::Token_extern);
    TEST_CHECK(scanner.Next() == Token::Token_float);
    TEST_CHECK(scanner.Next() == Token::Token_for);
    TEST_CHECK(scanner.Next() == Token::Token_goto);
    TEST_CHECK(scanner.Next() == Token::Token_if);
    TEST_CHECK(scanner.Next() == Token::Token_int);
    TEST_CHECK(scanner.Next() == Token::Token_long);
    TEST_CHECK(scanner.Next() == Token::Token_register);
    TEST_CHECK(scanner.Next() == Token::Token_return);
    TEST_CHECK(scanner.Next() == Token::Token_short);
    TEST_CHECK(scanner.Next() == Token::Token_signed);
    TEST_CHECK(scanner.Next() == Token::Token_sizeof);
    TEST_CHECK(scanner.Next() == Token::Token_static);
    TEST_CHECK(scanner.Next() == Token::Token_struct);
    TEST_CHECK(scanner.Next() == Token::Token_switch);
    TEST_CHECK(scanner.Next() == Token::Token_typedef);
    TEST_CHECK(scanner.Next() == Token::Token_union);
    TEST_CHECK(scanner.Next() == Token::Token_unsigned);
    TEST_CHECK(scanner.Next() == Token::Token_void);
    TEST_CHECK(scanner.Next() == Token::Token_volatile);
    TEST_CHECK(scanner.Next() == Token::Token_while);
    TEST_CHECK(scanner.Next() == Token::Token_Eos);
}

void ScannerTest::ScannerStringLiteral()
{
    Scanner scanner;
    ScannerTester tester(scanner, " \"hello\" ");

    TEST_CHECK(scanner.GetTokenOffset() == 0);

    TEST_CHECK(scanner.Peek() == Token::Token_StringConstant);
    TEST_CHECK(scanner.GetLiteralValue() == "hello");
    TEST_CHECK(scanner.Next() == Token::Token_StringConstant);
    TEST_CHECK(scanner.Peek() == Token::Token_Eos);
}

void ScannerTest::ScannerCharLiteral()
{
    Scanner scanner;
    ScannerTester tester(scanner, " 'c''b' ");

    TEST_CHECK(scanner.GetTokenOffset() == 0);

    TEST_CHECK(scanner.Peek() == Token::Token_CharConstant);
    TEST_CHECK(scanner.GetLiteralValue() == "c");
    TEST_CHECK(scanner.Next() == Token::Token_CharConstant);
    TEST_CHECK(scanner.Peek() == Token::Token_CharConstant);
    TEST_CHECK(scanner.GetLiteralValue() == "b");
    TEST_CHECK(scanner.Next() == Token::Token_CharConstant);
    TEST_CHECK(scanner.Peek() == Token::Token_Eos);
}

void ScannerTest::ScannerEscapeSymbols()
{
    Scanner scanner;
    ScannerTester helper(scanner, "main return;\n   break\n ;\n;\n;;    x = \n 123\\char;");

    TEST_CHECK(scanner.Next() == Token::Token_main);
    TEST_CHECK(scanner.Next() == Token::Token_return);
    TEST_CHECK(scanner.Next() == Token::Token_Semicolon);
    TEST_CHECK(scanner.Next() == Token::Token_break);
    TEST_CHECK(scanner.Next() == Token::Token_Semicolon);
    TEST_CHECK(scanner.Next() == Token::Token_Semicolon);
    TEST_CHECK(scanner.Next() == Token::Token_Semicolon);
    TEST_CHECK(scanner.Next() == Token::Token_Semicolon);
    TEST_CHECK(scanner.Next() == Token::Token_Id);
    TEST_CHECK(scanner.Next() == Token::Token_Assign);
    TEST_CHECK(scanner.Next() == Token::Token_NumberConstant);
    TEST_CHECK(scanner.Next() == Token::Token_char);
    TEST_CHECK(scanner.Next() == Token::Token_Semicolon);
    TEST_CHECK(scanner.Peek() == Token::Token_Eos);
}

void ScannerTest::ScannerNextWithComments()
{
    Scanner scanner;
    ScannerTester tester(scanner, "// int i = 0;\n float j = 200");
    TEST_CHECK(scanner.Next() == Token::Token_float);
    TEST_CHECK(scanner.Peek() == Token::Token_Id);
    TEST_CHECK(scanner.GetLiteralValue() == "j");
    TEST_CHECK(scanner.Next() == Token::Token_Id);
    TEST_CHECK(scanner.Next() == Token::Token_Assign);
    TEST_CHECK(scanner.Peek() == Token::Token_NumberConstant);
    TEST_CHECK(scanner.GetLiteralValue() == "200");
    TEST_CHECK(scanner.Next() == Token::Token_NumberConstant);
    TEST_CHECK(scanner.Peek() == Token::Token_Eos);
}

void ScannerTest::ScannerSatart()
{
    Scanner scanner;

    ScannerTester tester(scanner, "a++;auto/*hello*/;break//hello");

    TEST_CHECK(scanner.GetTokenOffset() == 0);

    TEST_CHECK(scanner.Peek() == Token::Token_Id);
    TEST_CHECK(scanner.GetLiteralValue() == "a");
    TEST_CHECK(scanner.Next() == Token::Token_Id);
    TEST_CHECK(scanner.Peek() == Token::Token_Inc);
    TEST_CHECK(scanner.Next() == Token::Token_Inc);
    TEST_CHECK(scanner.Peek() == Token::Token_Semicolon);
    TEST_CHECK(scanner.Next() == Token::Token_Semicolon);
    TEST_CHECK(scanner.Peek() == Token::Token_auto);
    TEST_CHECK(scanner.Next() == Token::Token_auto);
    TEST_CHECK(scanner.Peek() == Token::Token_Semicolon);
    TEST_CHECK(scanner.Next() == Token::Token_Semicolon);
    TEST_CHECK(scanner.Next() == Token::Token_break);
    TEST_CHECK(scanner.Peek() == Token::Token_Eos);
}

void ScannerTest::ScannerSimple()
{
    Scanner scanner;

    ScannerTester tester(scanner, "for (int i = 0; i < 10; ++i);");

    TEST_CHECK(scanner.GetTokenOffset() == 0);

    TEST_CHECK(scanner.Next() == Token::Token_for);
    TEST_CHECK(scanner.Next() == Token::Token_LParen);
    TEST_CHECK(scanner.Next() == Token::Token_int);

    TEST_CHECK(scanner.Peek() == Token::Token_Id);
    TEST_CHECK(scanner.GetLiteralValue() == "i");
    TEST_CHECK(scanner.Next() == Token::Token_Id);

    TEST_CHECK(scanner.Next() == Token::Token_Assign);

    TEST_CHECK(scanner.Peek() == Token::Token_NumberConstant);
    TEST_CHECK(scanner.GetLiteralValue() == "0");
    TEST_CHECK(scanner.Next() == Token::Token_NumberConstant);

    TEST_CHECK(scanner.Next() == Token::Token_Semicolon);

    TEST_CHECK(scanner.Peek() == Token::Token_Id);
    TEST_CHECK(scanner.GetLiteralValue() == "i");
    TEST_CHECK(scanner.Next() == Token::Token_Id);

    TEST_CHECK(scanner.Next() == Token::Token_LT);

    TEST_CHECK(scanner.Peek() == Token::Token_NumberConstant);
    TEST_CHECK(scanner.GetLiteralValue() == "10");
    TEST_CHECK(scanner.Next() == Token::Token_NumberConstant);

    TEST_CHECK(scanner.Next() == Token::Token_Semicolon);
    TEST_CHECK(scanner.Next() == Token::Token_Inc);

    TEST_CHECK(scanner.Peek() == Token::Token_Id);
    TEST_CHECK(scanner.GetLiteralValue() == "i");
    TEST_CHECK(scanner.Next() == Token::Token_Id);

    TEST_CHECK(scanner.Next() == Token::Token_RParen);
    TEST_CHECK(scanner.Next() == Token::Token_Semicolon);
    TEST_CHECK(scanner.Peek() == Token::Token_Eos);
}

void ScannerTest::ScannerAri()
{
    Scanner scanner;

    ScannerTester tester(scanner, "float i != 20; \n new_value <= id += \"hello\"");

    TEST_CHECK(scanner.Next() == Token::Token_float);

    TEST_CHECK(scanner.Peek() == Token::Token_Id);
    TEST_CHECK(scanner.GetLiteralValue() == "i");
    TEST_CHECK(scanner.Next() == Token::Token_Id);

    TEST_CHECK(scanner.Next() == Token::Token_NE);

    TEST_CHECK(scanner.Peek() == Token::Token_NumberConstant);
    TEST_CHECK(scanner.GetLiteralValue() == "20");
    TEST_CHECK(scanner.Next() == Token::Token_NumberConstant);

    TEST_CHECK(scanner.Next() == Token::Token_Semicolon);

    TEST_CHECK(scanner.Peek() == Token::Token_Id);
    TEST_CHECK(scanner.GetLiteralValue() == "new_value");
    TEST_CHECK(scanner.Next() == Token::Token_Id);

    TEST_CHECK(scanner.Next() == Token::Token_LTE);

    TEST_CHECK(scanner.Peek() == Token::Token_Id);
    TEST_CHECK(scanner.GetLiteralValue() == "id");
    TEST_CHECK(scanner.Next() == Token::Token_Id);

    TEST_CHECK(scanner.Next() == Token::Token_AssignAdd);

    TEST_CHECK(scanner.Peek() == Token::Token_StringConstant);
    TEST_CHECK(scanner.GetLiteralValue() == "hello");
    TEST_CHECK(scanner.Next() == Token::Token_StringConstant);

    TEST_CHECK(scanner.Peek() == Token::Token_Eos);
}

void ScannerTest::ScannerFunction()
{
    Scanner scanner;
    ScannerTester tester(scanner, "\
                                  void SkipWhiteSpaces() \
                                  { \
                                  char ch = PeekChar(); \
                                  while (isspace(ch)) \
                                  { \
                                  ReadChar(); \
                                  ch = PeekChar(); \
                                  } \
                                  } \
                                  ");
    TEST_CHECK(scanner.Next() == Token::Token_void);

    TEST_CHECK(scanner.Peek() == Token::Token_Id);
    TEST_CHECK(scanner.GetLiteralValue() == "SkipWhiteSpaces");
    TEST_CHECK(scanner.Next() == Token::Token_Id);

    TEST_CHECK(scanner.Next() == Token::Token_LParen);
    TEST_CHECK(scanner.Next() == Token::Token_RParen);
    TEST_CHECK(scanner.Next() == Token::Token_LBrace);
    TEST_CHECK(scanner.Next() == Token::Token_char);

    TEST_CHECK(scanner.Peek() == Token::Token_Id);
    TEST_CHECK(scanner.GetLiteralValue() == "ch");
    TEST_CHECK(scanner.Next() == Token::Token_Id);

    TEST_CHECK(scanner.Next() == Token::Token_Assign);

    TEST_CHECK(scanner.Peek() == Token::Token_Id);
    TEST_CHECK(scanner.GetLiteralValue() == "PeekChar");
    TEST_CHECK(scanner.Next() == Token::Token_Id);

    TEST_CHECK(scanner.Next() == Token::Token_LParen);
    TEST_CHECK(scanner.Next() == Token::Token_RParen);
    TEST_CHECK(scanner.Next() == Token::Token_Semicolon);
    TEST_CHECK(scanner.Next() == Token::Token_while);
    TEST_CHECK(scanner.Next() == Token::Token_LParen);

    TEST_CHECK(scanner.Peek() == Token::Token_Id);
    TEST_CHECK(scanner.GetLiteralValue() == "isspace");
    TEST_CHECK(scanner.Next() == Token::Token_Id);

    TEST_CHECK(scanner.Next() == Token::Token_LParen);

    TEST_CHECK(scanner.Peek() == Token::Token_Id);
    TEST_CHECK(scanner.GetLiteralValue() == "ch");
    TEST_CHECK(scanner.Next() == Token::Token_Id);

    TEST_CHECK(scanner.Next() == Token::Token_RParen);
    TEST_CHECK(scanner.Next() == Token::Token_RParen);
    TEST_CHECK(scanner.Next() == Token::Token_LBrace);

    TEST_CHECK(scanner.Peek() == Token::Token_Id);
    TEST_CHECK(scanner.GetLiteralValue() == "ReadChar");
    TEST_CHECK(scanner.Next() == Token::Token_Id);

    TEST_CHECK(scanner.Next() == Token::Token_LParen);
    TEST_CHECK(scanner.Next() == Token::Token_RParen);
    TEST_CHECK(scanner.Next() == Token::Token_Semicolon);

    TEST_CHECK(scanner.Peek() == Token::Token_Id);
    TEST_CHECK(scanner.GetLiteralValue() == "ch");
    TEST_CHECK(scanner.Next() == Token::Token_Id);

    TEST_CHECK(scanner.Next() == Token::Token_Assign);

    TEST_CHECK(scanner.Peek() == Token::Token_Id);
    TEST_CHECK(scanner.GetLiteralValue() == "PeekChar");
    TEST_CHECK(scanner.Next() == Token::Token_Id);

    TEST_CHECK(scanner.Next() == Token::Token_LParen);
    TEST_CHECK(scanner.Next() == Token::Token_RParen);
    TEST_CHECK(scanner.Next() == Token::Token_Semicolon);

    TEST_CHECK(scanner.Next() == Token::Token_RBrace);
    TEST_CHECK(scanner.Next() == Token::Token_RBrace);

    TEST_CHECK(scanner.Peek() == Token::Token_Eos);
}

void ScannerTest::ScannerSwitch()
{
    Scanner scanner;
    ScannerTester tester(scanner, "\
                                  switch (ch)\
                                  {\
                                  case '{':\
                                  return (Token_LBrace);\
                                  }\
                                  ");
    TEST_CHECK(scanner.Next() == Token::Token_switch);
    TEST_CHECK(scanner.Next() == Token::Token_LParen);

    TEST_CHECK(scanner.Peek() == Token::Token_Id);
    TEST_CHECK(scanner.GetLiteralValue() == "ch");
    TEST_CHECK(scanner.Next() == Token::Token_Id);

    TEST_CHECK(scanner.Next() == Token::Token_RParen);
    TEST_CHECK(scanner.Next() == Token::Token_LBrace);
    TEST_CHECK(scanner.Next() == Token::Token_case);

    TEST_CHECK(scanner.Peek() == Token::Token_CharConstant);
    TEST_CHECK(scanner.GetLiteralValue() == "{");
    TEST_CHECK(scanner.Next() == Token::Token_CharConstant);

    TEST_CHECK(scanner.Next() == Token::Token_Colon);
    TEST_CHECK(scanner.Next() == Token::Token_return);

    TEST_CHECK(scanner.Next() == Token::Token_LParen);

    TEST_CHECK(scanner.Peek() == Token::Token_Id);
    TEST_CHECK(scanner.GetLiteralValue() == "Token_LBrace");
    TEST_CHECK(scanner.Next() == Token::Token_Id);

    TEST_CHECK(scanner.Next() == Token::Token_RParen);
    TEST_CHECK(scanner.Next() == Token::Token_Semicolon);
    TEST_CHECK(scanner.Next() == Token::Token_RBrace);

    TEST_CHECK(scanner.Peek() == Token::Token_Eos);
}