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
#pragma once

#include "targetver.h"
#include "stdafx.h"
#include "compilerlib.h"
#include <cassert>

#ifdef _MSC_VER
#define TEST_CHECK(expr) \
    if (!(expr)) \
    { \
        std::cout << "Test failed: " \
        << "line " << __LINE__ \
        << ", file " << __FILE__ << std::endl;  \
    }
#else
    #define TEST_CHECK(expr) assert((expr));
#endif