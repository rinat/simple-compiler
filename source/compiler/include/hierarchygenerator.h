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
#ifndef __TypeList_HierarchyGenerator_h__
#define __TypeList_HierarchyGenerator_h__

namespace typelist
{
    template <class TList, template <class> class Unit>
    class HierarchyGenerator;

    template <template <class> class Unit>
    class HierarchyGenerator<NullType, Unit>
    {
    };

    template <class U, template <class> class Unit>
    class HierarchyGenerator : public Unit<U>
    {
    public:
        typedef Unit<U> LeftBase;
    };

    template <class T1, class T2, template <class> class Unit>
    class HierarchyGenerator<TypeList<T1, T2>, Unit>
        : public HierarchyGenerator<T1, Unit>
        , public HierarchyGenerator<T2, Unit>
    {
    public:
        typedef TypeList<T1, T2> TList;
        typedef HierarchyGenerator<T1, Unit> LeftBase;
        typedef HierarchyGenerator<T2, Unit> RightBase;
    };
}

#endif