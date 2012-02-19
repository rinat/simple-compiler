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
#ifndef __Compiler_MemoryPool_h__
#define __Compiler_MemoryPool_h__

/*
    MemoryPool - container of MemoryBlock
*/

#include "imemorypool.h"
#include "ptrvector.h"

namespace compiler
{
    namespace
    {
        class MemoryBlock
        {
        public:
            MemoryBlock(size_t size)
                : m_size(size)
                , m_current(0)
            { m_block.resize(size); }

            char *Allocate(size_t size)
            {
                char *ptr = &m_block[m_current];
                m_current += size;
                return ptr;
            }

            size_t GetFreeSpace() const
            { return (m_size - m_current); }

            size_t GetSize() const
            { return (m_size); }

        private:
            size_t m_size;
            size_t m_current;
            std::vector<char> m_block;
        };
    }

    class MemoryPool
        : public IMemoryPool
        , private utils::NonCopyable
    {
        enum { kDefaultMemBlockSize = 1024,
               kGrowthCoefficient  = 1 };
    public:
        MemoryPool()
        { m_blocks.PushBack(new MemoryBlock(kDefaultMemBlockSize)); }

        virtual char *Allocate(size_t size)
        {
            MemoryBlock *block = m_blocks.Back();
            if(size <= block->GetFreeSpace())
            {
                return block->Allocate(size);
            }
            else
            {
                m_blocks.PushBack(new MemoryBlock((block->GetSize() << kGrowthCoefficient) + size));
                return m_blocks.Back()->Allocate(size);
            }
        }

    private:
        container::PtrVector<MemoryBlock> m_blocks;
    };
}

#endif