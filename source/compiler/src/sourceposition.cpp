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
#include "sourceposition.h"

using namespace compiler;

SourcePosition::SourcePosition()
: m_sourceFile(NULL)
, m_beginOffset(0)
, m_astNode(NULL)
{}

SourcePosition::SourcePosition(const ISourceFile* sourceFile, size_t offset)
: m_sourceFile(sourceFile)
, m_beginOffset(offset)
, m_astNode(NULL)
{
}

SourcePosition::SourcePosition(AstNode *node, size_t offset)
: m_sourceFile(NULL)
, m_beginOffset(offset)
{
    assert(node != NULL);
    assert(node->m_sourceFile != NULL);

    m_sourceFile = node->m_sourceFile;
    m_beginOffset = offset;
}

std::string SourcePosition::GetSourcePath() const
{
    return (m_sourceFile ? m_sourceFile->GetPath() : "");
}

size_t SourcePosition::GetLine() const
{
    return (m_sourceFile ? m_sourceFile->GetLine(m_beginOffset) : 0);
}

size_t SourcePosition::GetOffset() const
{
    return (m_beginOffset);
}