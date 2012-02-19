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
#include "InputBufferTest.h"
#include "ScannerTest.h"
#include "ParserTest.h"

#ifdef _WIN32
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

class Tester
{
public:
    ~Tester()
    { 
        #ifdef _MSC_VER
        _CrtDumpMemoryLeaks();
        #endif
    }
    void StartTest()
    {
        InputBufferTest inpBufferTest;
        ScannerTest scannerTest;
        PraserTest parserTest;

        inpBufferTest.StartTest();
        scannerTest.StartTest();
        parserTest.StartTest();
    }
};

static Tester tester;

int main()
{
    tester.StartTest();
}