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
#include "scanner.h"
#include "parser.h"
#include "compiler.h"
#include "astnodebuilder.h"
#include "astdumper.h"
#include "sourcefile.h"
#include "llvmcodegenerator.h"
#include "sourcefile.h"
#include "conversions.h"
#include "memorypool.h"
#include "stringpool.h"
#include <sys/stat.h>
#include "file.h"

using namespace compiler;
using namespace utils::conversions;

Compiler::Compiler()
{
    CErrorsContainer *errorsContainer = new CErrorsContainer();
    m_errorsContainer.reset(errorsContainer);
    m_errosObserver.reset(new CErrorsObserver(*errorsContainer));
}

const IErrorsContainer& Compiler::GetErrors() const
{
    return (*m_errorsContainer.get());
}

bool Compiler::Compile(const std::string& inSourceFile,
                       std::ostream& outGenerated,
                       bool printAstTree)
{
    if (!outGenerated.bad() &&
        File::Exists(inSourceFile))
    {
        m_errorsContainer->ClearObservers();
        m_errorsContainer->ClearErrors();

        m_sourceFile.reset(new SourceFile(inSourceFile));
        MemoryPool memPool;
        StringPool stringPool;
        AstNodeBuilder builder(memPool, stringPool);

        Parser parser;
        parser.AttachErrorsObserver(*m_errosObserver.get());

        if (parser.Parse(*m_sourceFile.get(), builder))
        {
            if (builder.GetRootNode())
            {
                LLVMCodeGenerator generator;
                generator.AttachErrorsObserver(*m_errosObserver.get());

                if (generator.Generate(outGenerated, *builder.GetRootNode()))
                {
                    if (printAstTree)
                    {
                        AstDumper dumper(2, std::cout);
                        builder.GetRootNode()->Accept(dumper);
                    }
                    return (true);
                }
            }
        }
    }
    return (false);
}