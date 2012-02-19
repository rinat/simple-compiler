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
#ifndef __Utils_Deleter_h__
#define __Utils_Deleter_h__

namespace utils
{
    template <class T, bool IsArray = false>
    struct deleter
        : std::unary_function <T*, void>
    {
        void operator()(T *pointer) const
        { IsArray ? delete[] pointer : delete pointer; }
    };
}

#endif