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
#ifndef __TypeList_TypelistMacros_h__
#define __TypeList_TypelistMacros_h__

#include "nulltype.h"

namespace typelist
{
    template
    <
        class Type01 = NullType, class Type02 = NullType,
        class Type03 = NullType, class Type04 = NullType,
        class Type05 = NullType, class Type06 = NullType,
        class Type07 = NullType, class Type08 = NullType,
        class Type09 = NullType, class Type10 = NullType,
        class Type11 = NullType, class Type12 = NullType,
        class Type13 = NullType, class Type14 = NullType,
        class Type15 = NullType, class Type16 = NullType,
        class Type17 = NullType, class Type18 = NullType,
        class Type19 = NullType, class Type20 = NullType,
        class Type21 = NullType, class Type22 = NullType,
        class Type23 = NullType, class Type24 = NullType,
        class Type25 = NullType, class Type26 = NullType,
        class Type27 = NullType, class Type28 = NullType,
        class Type29 = NullType, class Type30 = NullType,
        class Type31 = NullType, class Type32 = NullType,
        class Type33 = NullType, class Type34 = NullType
    >
    class CreateTypeList
    {
    private:
        typedef typename CreateTypeList
        <
            Type02, Type03, Type04, Type05, Type06, Type07, Type08, Type09, Type10,
            Type11, Type12, Type13, Type14, Type15, Type16, Type17, Type18, Type19,
            Type20, Type21, Type22, Type23, Type24, Type25, Type26, Type27, Type28,
            Type29, Type30, Type31, Type32, Type33, Type34
        >::Result TailResult;
    public:
        typedef TypeList<Type01, TailResult> Result;
    };

    template <>
    struct CreateTypeList<>
    {
        typedef NullType Result;
    };
}

#endif