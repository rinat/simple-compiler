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
#include "commandline.h"

using namespace commandline;

size_t CommandLine::npos = -1;

CommandLine::CommandLine(const std::string& inCommandPrefix)
: m_commandPrefix(inCommandPrefix)
{}

bool CommandLine::EqualCommandStartWithPrefix(const std::string& inCommand)
{
    if (inCommand.size() > m_commandPrefix.size())
    {
        return (m_commandPrefix == inCommand.substr(0, m_commandPrefix.size()));
    }
    return (false);
}

bool CommandLine::InitOptions(int argc, char *argv[])
{
    for (int argcIndex = 1; argcIndex < argc;)
    {
        std::string currentCommand(argv[argcIndex]);
        std::vector<std::string> commandArguments;

        if (EqualCommandStartWithPrefix(currentCommand))
        {
            ++argcIndex;
            for (; argcIndex < argc; ++argcIndex)
            {
                if (EqualCommandStartWithPrefix(std::string(argv[argcIndex])))
                {
                    break;
                }
                commandArguments.push_back(std::string(argv[argcIndex]));
                continue;
            }
            m_actions.push_back(Option(currentCommand, commandArguments));
            m_actionToFullInfo.insert(OptionToFullInfo::value_type(currentCommand, m_actions.size() - 1));
        }
        else
        {
            return (false);
        }
    }
    return (true);
}

const std::vector<Option>& CommandLine::GetOptions() const
{
    return (m_actions);
}

size_t CommandLine::FindOption(const std::string& inOption) const
{
    OptionToFullInfo::const_iterator it = 
        m_actionToFullInfo.find(inOption);
    if (it != m_actionToFullInfo.end())
    {
        return (it->second);
    }
    return (CommandLine::npos);
}