#include "../../Compiler/src/stdafx.h"
#include "../../Compiler/include/parser.h"
#include "../../Compiler/include/scanner.h"

using namespace compiler;

void PrintErrors(IErrorsContainer& container)
{
    for (size_t index = 0; index < container.GetErrorsCount(); ++index)
    {
        const IError &error = container.GetError(index);
		std::cout <<  error.GetErrorDescription() << " " << error.GetErrorRange().GetSourcePath() << " Line: "
			      << error.GetErrorRange().GetLine() + 1 << std::endl;
	}
}

int main(int argc, char *argv[])
{
	//*
    if(argc != 2)
    {
        std::cout << "ParserTester.exe <file name>" << std::endl;
		return 0;
    }
	
	std::auto_ptr<SourceFile> source(new SourceFile(argv[1]));
	//*/

	//std::auto_ptr<SourceFile> source(new SourceFile("d:\\test.c"));

	Parser parser;

	AstNodeBuilder nodeBuilder;
	if(!parser.Parse(source.get(), &nodeBuilder))
	{
		std::cout << std::endl;
		PrintErrors(parser);
	}
	else
	{
		AstNode *root = nodeBuilder.GetRootNode();
		AstVisitor visitor;
		root->Accept(&visitor);
	}

	return 0;
}