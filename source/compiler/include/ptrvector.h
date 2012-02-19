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
#ifndef __Container_PtrVector_h__
#define __Container_PtrVector_h__

#include "int2type.h"
#include "deleter.h"

namespace container
{
    template
    <
        typename T,
        bool ElementsOwner = true
    >
    class PtrVector
    {
    public:
        PtrVector()
        {}

        ~PtrVector()
        { Clear(); }

        void PushBack(T* element)
        { m_elements.push_back(element); }

        void Clear()
        { DestroyElements(utils::Int2Type<ElementsOwner>()); }

        T* Back()
        { return (m_elements.back()); }

        void PopBack()
        { m_elements.pop_back(); }

        bool Empty() const
        { return (Size() == 0); }

        size_t Size() const
        { return (m_elements.size()); }

    private:
        void DestroyElements(utils::Int2Type<true>)
        {
            std::for_each(m_elements.begin(), m_elements.end(),
                utils::deleter<T, false>());
            m_elements.clear();
        }

        void DestroyElements(utils::Int2Type<false>)
        { m_elements.clear(); }

    private:
        std::vector<T*> m_elements;

    private:
        PtrVector(const PtrVector&);
        PtrVector& operator = (const PtrVector&);
    };
}

#endif