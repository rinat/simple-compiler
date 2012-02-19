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
#define WIN32_LEAN_AND_MEAN
#include "stlincludes.h"
#include "conversions.h"
#include "noncopyable.h"
#include "smartptr.h"
#include "astnode.h"
#include "int2type.h"
#include "deleter.h"
#include "ptrvector.h"