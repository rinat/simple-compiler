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
#include "textbuffer.h"

using namespace compiler;
using namespace utils::conversions;

TextBuffer::TextBuffer()
: m_offset(0)
{}

TextBuffer::TextBuffer(const std::string& source)
: m_source(source)
, m_offset(0)
{}

size_t TextBuffer::GetLinesCount() const
{
    return (GetLines().size());
}

size_t TextBuffer::GetOffset() const
{
    return (m_offset);
}

size_t TextBuffer::GetLength() const
{
    return (m_source.length());
}

const std::string& TextBuffer::GetSource() const
{
    return (m_source);
}

char TextBuffer::Peek()
{
    assert(m_offset <= safe_static_cast<size_t>(m_source.length()));
    return (m_offset == safe_static_cast<size_t>(m_source.length()))
        ? 0
        : m_source[m_offset];
}

char TextBuffer::Next()
{
    char ch = Peek();
    if (ch != 0)
    {
        ++m_offset;
    }

    return ch;
}

const TextBuffer::Lines& TextBuffer::GetLines() const
{
    if (m_lines.empty())
    {
        m_lines.push_back(0);
        for (size_t i = 0; i < m_source.length(); ++i)
        {
            bool hasNext = ((i + 1) < m_source.length());
            if ((m_source[i] == '\r' &&
                hasNext && m_source[i + 1] == '\n'))
            {
                ++i;
                m_lines.push_back(i + 1);
            }
            else
            {
                if (m_source[i] == '\n' ||
                    m_source[i] == '\r')
                {
                    m_lines.push_back(i + 1);
                }
            }
        }
    }

    return m_lines;
}

size_t TextBuffer::GetLineNumber(size_t offset) const
{
    const Lines& lines = GetLines();
    const Lines::const_iterator location =
        std::lower_bound(lines.begin(), lines.end(), offset);

    if (location == lines.begin())
        return (0);
    if (location != lines.end())
        return ((location - lines.begin()) - 1);
    return (lines.size() - 1);
}

size_t TextBuffer::GetLineOffset(size_t lineNumber) const
{
    const Lines& lines = GetLines();

    assert(!lines.empty());
    assert(lineNumber < lines.size());

    return lines[lineNumber];
}

size_t TextBuffer::GetLineLength(size_t lineNumber) const
{
    const Lines& lines = GetLines();

    assert(!lines.empty());
    assert(lineNumber < lines.size());

    if (lineNumber == lines.size() - 1)
    {
        return m_source.length() - GetLineOffset(lineNumber);
    }
    return lines[lineNumber + 1] - lines[lineNumber];
}

void TextBuffer::Reset(const std::string& source)
{
    m_source = source;
    Reset();
}

void TextBuffer::Reset()
{
    m_offset = 0;
    m_lines.clear();
}