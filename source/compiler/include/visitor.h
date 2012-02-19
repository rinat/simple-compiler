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
#ifndef __Compiler_Visitor_h__
#define __Compiler_Visitor_h__

#include "typelist.h"
#include "hierarchygenerator.h"

namespace visitor
{
    using namespace typelist;

    template <class T, typename R>
    class Visitor
    {
    public:
        typedef R ReturnType;
        virtual ~Visitor() {}
        virtual ReturnType Visit(T *) = 0;
    };

    template <typename R>
    struct VisitorBinder
    {
        template <class T>
        struct Result
            : public Visitor<T, R>
        {};
    };

    template <typename R, class TList>
    class TypeListVisitor
        : public HierarchyGenerator<TList, VisitorBinder<R>::Result>
    {
    public:
        typedef R ReturnType;

        template <class Visited>
        ReturnType Visit(Visited *host)
        {
            Visitor<Visited, ReturnType>& sobObj = *this;
            return (sobObj.Visit(host));
        }
    };
}

#endif