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
#ifndef __Compiler_Counters_h__
#define __Compiler_Counters_h__

namespace compiler
{
    struct Counters 
    {        
        size_t m_elementsCounter;
        size_t m_ifelseCounter;
        size_t m_whileCounter;

        Counters()
            : m_elementsCounter(0)
            , m_ifelseCounter(0)
            , m_whileCounter(0)
        {}
    };
}

#endif