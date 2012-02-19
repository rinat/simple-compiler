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
#ifndef __Compiler_Stack_h__
#define __Compiler_Stack_h__

#include "ptrvector.h"

namespace compiler
{
    template <class T>
    class Stack
    {
    public:
        void Clear()
        { m_stack.Clear(); }

        void PushBack(T* element)
        { m_stack.PushBack(element); }
       
        T* Back()
        { return (m_stack.Empty() ? NULL : m_stack.Back()); }

        inline T* PopBack()
        {
            if (m_stack.Empty())
            { return (NULL); }

            T* result = NULL;
            result = m_stack.Back();
            m_stack.PopBack();
            return (result);
        }
    private:
        container::PtrVector<T, false> m_stack;
    };
}

#endif