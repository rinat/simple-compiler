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
#ifndef __Compiler_CErrorsDispatcher_h__
#define __Compiler_CErrorsDispatcher_h__

#include "ierrorsobserver.h"
#include "cerrorscontainer.h"

namespace compiler
{
    class CErrorsObserver
        : public IErrorsObserver
        , private utils::NonCopyable
    {
    public:
        CErrorsObserver(CErrorsContainer& errorsContainer)
            : m_errorsContainer(errorsContainer)
        {}

        virtual void ErrorEvent(IErrorsContainer::IErrorPtr error)
        { m_errorsContainer.AddError(error); }

    private:
        CErrorsContainer &m_errorsContainer;
    };
}

#endif
