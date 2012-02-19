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
#ifndef __Frontend_CommandLine_h__
#define __Frontend_CommandLine_h__

#include "option.h"
#include <map>

namespace commandline
{
    class CommandLine
    {
    public:
        static size_t npos;

        CommandLine(const std::string& commandPrefix = std::string());
        
        bool InitOptions(int argc, char *argv[]);
        const std::vector<Option>& GetOptions() const;
        size_t FindOption(const std::string& inAction) const;

    private:
        bool EqualCommandStartWithPrefix(const std::string& inCommand);

    private:
        typedef int OptionIndex;
        typedef std::map<Option::Action, OptionIndex> OptionToFullInfo;

        std::string m_commandPrefix;
        std::vector<Option> m_actions;
        OptionToFullInfo m_actionToFullInfo;
    };
}

#endif