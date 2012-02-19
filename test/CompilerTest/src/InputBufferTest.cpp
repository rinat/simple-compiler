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
#include "InputBufferTest.h"
#include "textbuffer.h"

using namespace compiler;

void InputBufferTest::StartTest()
{
    InputBufferStart();
    InputBufferWithData();
}

void InputBufferTest::InputBufferStart()
{
    TextBuffer buffer2;
    TextBuffer buffer;

    TEST_CHECK(buffer.GetOffset() == 0);
    TEST_CHECK(buffer.GetLength() == 0);
    TEST_CHECK(buffer.GetSource().empty() == true);

    TEST_CHECK(buffer.Peek() == 0);
    TEST_CHECK(buffer.Next() == 0);
}

void InputBufferTest::InputBufferWithData()
{
    std::string source = " int i = 0; ";

    TextBuffer buffer(source);

    TEST_CHECK(buffer.GetLength() == 12);
    TEST_CHECK(buffer.GetSource() == source);
    TEST_CHECK(buffer.GetOffset() == 0);

    TEST_CHECK(buffer.Peek() == ' ');
    TEST_CHECK(buffer.GetOffset() == 0);

    TEST_CHECK(buffer.Next() == ' ');
    TEST_CHECK(buffer.GetOffset() == 1);

    TEST_CHECK(buffer.Peek() == 'i');
    TEST_CHECK(buffer.Peek() == 'i');
    TEST_CHECK(buffer.Peek() == 'i');
    TEST_CHECK(buffer.GetOffset() == 1);

    TEST_CHECK(buffer.Next() == 'i');
    TEST_CHECK(buffer.GetOffset() == 2);
    TEST_CHECK(buffer.Peek() == 'n');
    TEST_CHECK(buffer.GetOffset() == 2);

    TEST_CHECK(buffer.Next() == 'n');
    TEST_CHECK(buffer.GetOffset() == 3);
    TEST_CHECK(buffer.Peek() == 't');
    TEST_CHECK(buffer.GetOffset() == 3);

    TEST_CHECK(buffer.Next() == 't');
    TEST_CHECK(buffer.GetOffset() == 4);
    TEST_CHECK(buffer.Peek() == ' ');
    TEST_CHECK(buffer.GetOffset() == 4);

    buffer.Reset();
    TEST_CHECK(buffer.GetLength() == 12);
    TEST_CHECK(buffer.GetOffset() == 0);

    std::string newSource("hello");
    buffer.Reset(newSource);
    TEST_CHECK(buffer.GetSource() == newSource);
}