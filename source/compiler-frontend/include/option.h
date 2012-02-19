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
#ifndef __Frontend_CommandlineAction_h__
#define __Frontend_CommandlineAction_h__

#include <string>
#include <vector>

namespace commandline
{
    class Option
    {
    public:
        typedef std::string Action;
        typedef std::string Argument;

        Option();
        Option(const std::string& inOption,
            const std::vector<std::string>& inArguments);

        const std::string& GetOption() const;
        const std::vector<std::string>& GetArguments() const;

    private:
        Action m_option;
        std::vector<Argument> m_arguments;
    };
}

#endif