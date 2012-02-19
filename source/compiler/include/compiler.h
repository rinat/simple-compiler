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
#ifndef __Compiler_Compiler_h__
#define __Compiler_Compiler_h__

/*
    main compiler interface
*/

namespace compiler
{
    class ISourceFile;
    class IErrorsObserver;
    class IErrorsContainer;

    class Compiler
    {
    public:
        Compiler();

        bool Compile(const std::string& inSourceFile,
            std::ostream& outGenerated, bool printAstTree = false);
        const IErrorsContainer& GetErrors() const;

    private:
        std::auto_ptr<IErrorsObserver> m_errosObserver;
        std::auto_ptr<ISourceFile> m_sourceFile;
        std::auto_ptr<IErrorsContainer> m_errorsContainer;

    private:
        Compiler(const Compiler&);
        Compiler& operator = (const Compiler&);
    };
}

#endif