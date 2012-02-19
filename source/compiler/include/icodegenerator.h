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
#ifndef __Compiler_ICodegenerator_h__
#define __Compiler_ICodegenerator_h__

namespace compiler
{
    class ICodegenerator
    {
    public:
        virtual bool Generate(std::ostream& outStream, AstNode& program) = 0;
        virtual ~ICodegenerator() {}
    };
}

#endif