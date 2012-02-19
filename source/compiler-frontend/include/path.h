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
#ifndef __Compiler_Path_h__
#define __Compiler_Path_h__

/*
    path utility class
*/

namespace compiler
{
    class Path
    {
    public:
        static std::string GetFileName(const std::string& inFullPath);
    };
}

#endif