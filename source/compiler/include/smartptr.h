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
#ifndef __SmartPtr_SmartPtr_h__
#define __SmartPtr_SmartPtr_h__

#include "int2type.h"

namespace smartptr
{
    template <class T, bool IsArray = false>
    class SmartPtr
    {
    public:
        SmartPtr(T* pointer)
            : m_pointer(pointer)
            , m_counter(new long(1))
        {}

        SmartPtr(const SmartPtr& smartpointer)
            : m_pointer(smartpointer.m_pointer)
            , m_counter(smartpointer.m_counter)
        { ++*m_counter; }

        ~SmartPtr()
        {
            if (!--*m_counter)
            {
                delete m_counter;
                DestroyPtr(utils::Int2Type<IsArray>());
            }
        }
        
        SmartPtr& operator = (const SmartPtr& rightPointer)
        {
            if (this != &rightPointer)
            {
                if (!--*m_counter)
                {
                    DestroyPtr(utils::Int2Type<IsArray>());
                }
                m_pointer = rightPointer.m_pointer;
                ++*(m_counter = rightPointer.m_counter);
            }
            return (*this);
        }

        const T& operator * () const 
        { return (*m_pointer); }

        const T* operator -> () const
        { return (m_pointer); }

    private:
        void DestroyPtr(utils::Int2Type<true>)
        { delete[] m_pointer }

        void DestroyPtr(utils::Int2Type<false>)
        { delete m_pointer; }

    private:
        T *m_pointer;
        long *m_counter;
    };
}

#endif