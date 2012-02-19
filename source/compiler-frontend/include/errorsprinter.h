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
#ifndef __Compiler_ErrorsPrinter_h__
#define __Compiler_ErrorsPrinter_h__

#include "path.h"

namespace compiler
{
    using namespace compiler;

    class ErrorsPrinter : public IErrorsPrinter
    {
    public:
        ErrorsPrinter(bool printOnlyFirst, std::ostream& outStream)
            : m_printAll(!printOnlyFirst)
            , m_outStream(outStream)
        {}
        virtual void operator ()(const IErrorsContainer& container)
        {
            for (size_t index = 0; 
                index < (m_printAll ? container.GetErrorsCount() : 1);
                ++index)
            {
                const IError& error = container.GetError(index);
                std::cout << error.GetErrorDescription() << " " <<
                    Path::GetFileName(error.GetSourcePosition().GetSourcePath());
                std::cout << " Line: " << error.GetSourcePosition().GetLine() << std::endl;
            }
        }

    private:
        bool m_printAll;
        std::ostream& m_outStream;

    private:
        ErrorsPrinter(const ErrorsPrinter&);
        ErrorsPrinter& operator = (const ErrorsPrinter&);
    };
}

#endif