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
#include "compilerlib.h"
#include "commandline.h"
#include "grammarhelp.h"
#include "errorsprinter.h"
#include "file.h"

using namespace compiler;
using namespace commandline;

void ProcessCommandLine(const CommandLine&);
void CompileProgram(const std::string&, const std::string&, bool);
void PrintHelp();

int main(int argc, char *argv[])
{     
    CommandLine commandLine("-");
    if (commandLine.InitOptions(argc, argv))
    {
        ProcessCommandLine(commandLine);
        return (0);
    }
    PrintHelp();
    return (0);
}

void PrintHelp()
{
    std::cout << "PS41 (R) 32-bit Simple C compiler version 0.555 for Low Level Virtual Machine\n";
    std::cout << "Copyright (C) PS41 Corporation. All rights reserved.\n\n";
    std::cout << "Allowed options:\n"
        << "  " << "-i input file [Required]\n"
        << "  " << "-o output file[Required]\n"
        << "  " << "-p print AST tree [Optional]\n"
        << "  " << "-g print grammar [Optional]\n"
        << "  " << "-h produces help message [Optional]"
        << std::endl;
}

void CompileProgram(const std::string& inputFile,
                    const std::string& outputFile,
                    bool dumpAstTree)
{
    std::ostringstream outStream;
    Compiler compiler;
    if (compiler.Compile(inputFile, outStream, dumpAstTree))
    {
        std::ofstream generated(outputFile.c_str());
        if (!generated.bad())
        {
            generated << outStream.str();
            generated.close();
        }
        else
        {
            generated.close();
            std::cout << "Unable to open " << inputFile << std::endl;
        }
        return;
    }
    ErrorsPrinter printer(false, std::cout);
    printer(compiler.GetErrors());
}

void PrintGrammar()
{
    std::cout << grammar::g_grammar << std::endl;
}

void ProcessCommandLine(const CommandLine& commandLine)
{
    const std::vector<Option>& options = commandLine.GetOptions();
    if (options.size() > 0)
    {
        bool printAst = (commandLine.FindOption(std::string("-p")) != CommandLine::npos);
        bool printGrammar = (commandLine.FindOption(std::string("-g")) != CommandLine::npos);
        size_t inputFileOption = commandLine.FindOption("-i");
        size_t outputFileOption = commandLine.FindOption("-o");

        if (inputFileOption != CommandLine::npos &&
            outputFileOption != CommandLine::npos &&
            options[inputFileOption].GetArguments().size() > 0 &&
            options[outputFileOption].GetArguments().size() > 0)
        {
            std::string inputFile = options[inputFileOption].GetArguments()[0];
            std::string outputFile = options[outputFileOption].GetArguments()[0];
            if (File::Exists(inputFile))
            {
                printGrammar ? PrintGrammar() : true;
                CompileProgram(inputFile, outputFile, printAst);
                return;
            }
        }
        printGrammar ? PrintGrammar() :
            (commandLine.FindOption(std::string("-h")) != CommandLine::npos)
            ? PrintHelp() : PrintHelp();
        return;
    }
    PrintHelp();
}