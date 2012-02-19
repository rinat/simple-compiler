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
#include "stdafx.h"
#include "path.h"

using namespace compiler;

std::string Path::GetFileName(const std::string& inFullPath)
{
    size_t slashPosition = inFullPath.rfind('\\', inFullPath.length());
    if (slashPosition != std::wstring::npos)
    {
        return (inFullPath.substr(slashPosition + 1, inFullPath.length() - slashPosition));
    }
    else
    {
        return (inFullPath);
    }
}