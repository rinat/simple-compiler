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
#ifndef __Test_PraserTest_h__
#define __Test_PraserTest_h__

#include "ITest.h"

class PraserTest
    : public ITest
{
public:
    virtual void StartTest();

private:
    void ParserSimpleTestWithError();
    void ParserSimpleTest();
    void ParserMaintTest();
    void ParserIdentifierTest();
    void ParserIdentifierArrayTest();
    void ParserStringLiteralTest();
    void ParserWhileStatementTest();
    void ParserDoWhileStatementTest();
};

#endif