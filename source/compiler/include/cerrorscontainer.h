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
#ifndef __Compiler_CErrorsContainer_h__
#define __Compiler_CErrorsContainer_h__

#include "cerror.h"
#include "ierrorsobserver.h"
#include "ierrorscontainer.h"

namespace compiler
{
    class CErrorsContainer
        : public IErrorsContainer
    {
    public:
        virtual ~CErrorsContainer();

        virtual void   AddError(IErrorPtr error);
        virtual size_t GetErrorsCount() const;
        virtual bool   HasErrors() const;
        virtual void   ClearErrors();
        virtual const IError& GetError(size_t index) const;

        virtual void ClearObservers();
        virtual void AttachErrorsObserver(IErrorsObserver&);
        virtual void DetachErrorsObserver(IErrorsObserver&);

    private:
        std::vector<IErrorPtr> m_errors;
        std::set<IErrorsObserver*> m_listeners;
    };
}

#endif