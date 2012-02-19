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
#include "stdafx.h"
#include "cerrorscontainer.h"

using namespace compiler;

CErrorsContainer::~CErrorsContainer()
{
    ClearErrors();
    ClearObservers();
}

void CErrorsContainer::AddError(IErrorPtr error)
{
    m_errors.push_back(error);
    std::set<IErrorsObserver*>::iterator it = m_listeners.begin();
    for (; it != m_listeners.end(); ++it)
    {
        (*it)->ErrorEvent(error);
    }
}

bool CErrorsContainer::HasErrors() const
{
    return (m_errors.size() > 0);
}

void CErrorsContainer::AttachErrorsObserver(IErrorsObserver& dispatcher)
{
    m_listeners.insert(&dispatcher);
}

void CErrorsContainer::DetachErrorsObserver(IErrorsObserver& dispatcher)
{
    m_listeners.erase(std::remove(m_listeners.begin(), m_listeners.end(), &dispatcher), m_listeners.end());
}

const IError& CErrorsContainer::GetError(size_t index) const
{
    assert(index < GetErrorsCount());
    return (*m_errors[index]);
}

size_t CErrorsContainer::GetErrorsCount() const
{
    return (m_errors.size());
}

void CErrorsContainer::ClearObservers()
{
    m_listeners.clear();
}

void CErrorsContainer::ClearErrors()
{
    m_errors.clear();
    m_errors.clear();
}