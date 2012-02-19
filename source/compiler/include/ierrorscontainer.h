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
#ifndef __Compiler_IErrorsContainer_h__
#define __Compiler_IErrorsContainer_h__

#include "smartptr.h"

namespace compiler
{
    class IErrorsObserver;

    class IErrorsContainer
    {
    public:
        typedef smartptr::SmartPtr<IError> IErrorPtr;

        virtual void   AddError(IErrorPtr error) = 0;
        virtual size_t GetErrorsCount() const = 0;
        virtual bool   HasErrors() const = 0;
        virtual void   ClearErrors() = 0;
        virtual const IError& GetError(size_t index) const = 0;

        virtual void ClearObservers() = 0;
        virtual void AttachErrorsObserver(IErrorsObserver&) = 0;
        virtual void DetachErrorsObserver(IErrorsObserver&) = 0;

        virtual ~IErrorsContainer() {}
    };
}

#endif