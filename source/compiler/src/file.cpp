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
#include "file.h"
#include <sys/stat.h>

using namespace compiler;

bool File::Exists(const std::string& inFilename)
{
    struct _stat buf;
    return (_stat(inFilename.c_str(), &buf) != -1);
}

bool File::Read(const std::string& inFilename,
                std::vector<unsigned char>& outBuffer)
{
    outBuffer.clear();
    std::ifstream file(inFilename.c_str(),
        std::ios::in | std::ios::binary | std::ios::ate);
    if (file.is_open())
    {
        size_t dataSize;
        if ((dataSize = file.tellg()) > 0)
        {
            outBuffer.resize(dataSize);
            file.seekg(0, std::ios::beg);
            file.read(reinterpret_cast<char*>(&outBuffer[0]), dataSize);
        }
        file.close();
        return (true);
    }
    return (false);
}