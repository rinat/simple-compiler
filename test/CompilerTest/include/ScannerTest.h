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
#ifndef __Test_ScannerTest_h__
#define __Test_ScannerTest_h__

#include "ITest.h"

class ScannerTest
    : public ITest
{
public:
    virtual void StartTest();

private:
    void ScannerSwitch();
    void ScannerFunction();
    void ScannerAri();
    void ScannerSimple();
    void ScannerSatart();
    void ScannerNextWithComments();
    void ScannerEscapeSymbols();
    void ScannerCharLiteral();
    void ScannerStringLiteral();
    void ScannerNext();
    void ScannerNotLatin();
};

#endif