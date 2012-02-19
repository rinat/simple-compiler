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
#include "sourcefile.h"
#include "file.h"

using namespace compiler;

SourceFile::SourceFile(const std::string &inPath, size_t inFirstLineIndex)
: m_path(inPath)
, m_firstLineIndex(inFirstLineIndex)
{}

SourceFile::~SourceFile()
{}

size_t SourceFile::GetLine(size_t offset) const
{
    return (m_textBuffer.get() != NULL)
         ? (m_textBuffer->GetLineNumber(offset) + m_firstLineIndex)
         : m_firstLineIndex;
}

const std::string& SourceFile::GetPath() const
{
    return (m_path);
}

ITextBuffer* SourceFile::GetBuffer()
{
    if (m_textBuffer.get() == NULL)
    {
        m_textBuffer.reset(ReadFile(m_path));
    }
    return (m_textBuffer.get());
}

ITextBuffer* SourceFile::GetBuffer(const std::string& inSource)
{
    m_textBuffer.reset(new TextBuffer(inSource));
    return (m_textBuffer.get());
}

TextBuffer* SourceFile::ReadFile(const std::string& filename)
{
    std::vector<unsigned char> buffer;
    return (File::Read(filename, buffer) && buffer.size() > 0)
        ? (new TextBuffer(std::string(buffer.begin(), buffer.end())))
        : (new TextBuffer(std::string()));
}