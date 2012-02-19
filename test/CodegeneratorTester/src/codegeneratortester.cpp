/*
* This code is under GNU Lesser General Public License
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
#include "compilerlib.h"

using namespace compiler;

void PrintErrors(const IErrorsContainer& container)
{
    if (container.GetErrorsCount() > 0)
    {
        const IError &error = container.GetError(0);
        std::cout << error.GetErrorDescription() << " " <<
            error.GetSourcePosition().GetSourcePath();
        std::cout << " Line: " << error.GetSourcePosition().GetLine() << std::endl;
    }
}

int main(int argc, char *argv[])
{        
    if(argc != 3)
    {
        std::cout << "codegeneratortester.exe <source name> <output file>" << std::endl;
        return -1;
    }
    std::ostringstream outStream;
    Compiler compiler;
    if (!compiler.Compile(argv[1], outStream))
    {
        PrintErrors(compiler.GetErrors());
    }
    else
    {
        std::ofstream generated(argv[2]);
        if (!generated.bad())
        {
            generated << outStream.str();
            generated.close();
        }
        else
        {
            generated.close();
            std::cout << "Unable to open " << argv[1] << std::endl;
            return -1;
        }
    }
}