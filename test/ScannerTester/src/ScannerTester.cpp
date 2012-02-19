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
#include "../../Compiler/include/scanner.h"
#include "../../Compiler/include/sourcefile.h"

using namespace compiler;

void PrintErrors(IErrorsContainer& container)
{
    std::cout << std::endl;
    for (size_t index = 0; index < container.GetErrorsCount(); ++index)
    {
        const IError &error = container.GetError(index);
        std::cout << error.GetSourcePosition().GetSourcePath() << " (Line: "
            << error.GetSourcePosition().GetLine() + 1
            << " - "
            << error.GetErrorDescription() << '\n';
    }
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        std::cout << "ScannerTester.exe [inputfile]" << std::endl;
    }
    if (argc > 1)
    {
        std::auto_ptr<SourceFile> source(new SourceFile(argv[1]));
        if (source.get() == NULL)
        {
            std::cout << "Unable to open " << argv[1] << std::endl;
            return (-1);
        }
        TextBuffer *buffer = dynamic_cast<TextBuffer*>(source->GetBuffer());
        assert(buffer != NULL);
		&buffer;
        Scanner scanner;
        scanner.Reset(*source);

        do 
        {
            switch (scanner.Peek())
            {
            case Token::Token_StringConstant:
                std::cout << "StringLiteral: " << scanner.GetLiteralValue() << std::endl;
                break;
            case Token::Token_CharConstant:
                std::cout << "CharacterConstant: " << scanner.GetLiteralValue() << std::endl;
                break;
            case Token::Token_NumberConstant:
                std::cout << "Number: " << scanner.GetLiteralValue() << std::endl;
                break;
            case Token::Token_Id:
                std::cout << "Identifier: " << scanner.GetLiteralValue() << std::endl;
                break;
            default:
                std::cout << "Token: " << Token::TokenToString(scanner.Peek()) << std::endl;
            }
        } while (scanner.Next()!= Token::Token_Eos);

        PrintErrors(scanner);
    }
}