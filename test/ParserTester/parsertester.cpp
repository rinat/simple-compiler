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
#include "../../Compiler/src/stdafx.h"
#include "../../Compiler/include/parser.h"
#include "../../Compiler/include/astdumper.h"
#include "../../Compiler/include/memorypool.h"
#include "../../Compiler/include/stringpool.h"
#include "../../Compiler/include/astnodebuilder.h"
#include "../../Compiler/include/sourcefile.h"

using namespace compiler;

void PrintErrors(IErrorsContainer& container)
{
    std::cout << std::endl;
    for (size_t index = 0; index < container.GetErrorsCount(); ++index)
    {
        const IError &error = container.GetError(index);
        std::cout << error.GetErrorDescription() << " " << error.GetSourcePosition().GetSourcePath() 
			<< " Line: " << error.GetSourcePosition().GetLine() + 1 << std::endl;
    }
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cout << "parsertester.exe <file name>" << std::endl;
		return 0;
    }
    
    std::auto_ptr<SourceFile> source(new SourceFile(argv[1]));

    if (source.get())
    {
        MemoryPool mPool;
        StringPool strPool;
        AstNodeBuilder builder(mPool, strPool);
        Parser parser;
        if(parser.Parse(*source.get(), builder))
        {
            AstNode *root = builder.GetRootNode();
            if (root)
            {
                AstDumper dumper(2, std::cout);
                root->Accept(dumper);
            }
        }
        else
        {
            PrintErrors(parser);
        }
    }
    else
    {
        std::cout << "Unable to open " << argv[1] << std::endl;
        return -1;
    }
}
