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
#ifndef __Compiler_IInputBuffer_h__
#define __Compiler_IInputBuffer_h__

namespace compiler
{
    class ITextBuffer
    {
    public:
        virtual char Next() = 0;
        virtual char Peek() = 0;

        virtual void Reset() = 0;
        virtual void Reset(const std::string& src) = 0;

        virtual size_t GetOffset() const = 0;
        virtual size_t GetLineNumber(size_t offset) const = 0;

        virtual ~ITextBuffer() {}
    };
}

#endif