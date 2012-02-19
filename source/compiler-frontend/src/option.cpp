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
#include "option.h"

using namespace commandline;

Option::Option()
{}

Option::Option(const std::string& inOption,
               const std::vector<std::string>& inArguments)
: m_option(inOption)
, m_arguments(inArguments)
{}

const std::string& Option::GetOption() const
{
    return (m_option);
}

const std::vector<Option::Argument>&
Option::GetArguments() const
{
    return (m_arguments);
}