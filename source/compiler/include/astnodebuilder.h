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
#ifndef __Compiler_AstNodeBuilder_h__
#define __Compiler_AstNodeBuilder_h__

/*
    Abstract syntax tree structures builder
*/

#include "conversions.h"
#include "imemorypool.h"
#include "istringpool.h"

namespace compiler
{
    using namespace utils::conversions;

    class AstNodeBuilder
        : private utils::NonCopyable
    {
    public:
        AstNodeBuilder::AstNodeBuilder(IMemoryPool& memPool, IStringPool& stringPool)
            : m_memPool(memPool)
            , m_StringPool(stringPool)
            , m_rootNode(NULL)
        {}
        
        AstNode* GetRootNode()
        { return (m_rootNode); }

		const AstNode* GetRootNode() const
        { return (m_rootNode); }

        template <typename NodeType>
        inline NodeType* CreateAstNode(const std::string& inName)
        {
            return (new (AllocateNode<NodeType>(m_memPool))
                NodeType(m_StringPool.Allocate(inName)));
        }

        template <typename NodeType>
        inline NodeType* CreateAstNode()
        {
            return (new (AllocateNode<NodeType>(m_memPool))
                NodeType());
        }

        template <typename NodeType, typename Arg1>
        inline NodeType* CreateAstNode(Arg1 arg1)
        {
            return (new (AllocateNode<NodeType>(m_memPool))
                NodeType(arg1));
        }

        template <typename NodeType, typename Arg1, typename Arg2>
        inline NodeType* CreateAstNode(Arg1 arg1, Arg2 arg2)
        {
            return (new (AllocateNode<NodeType>(m_memPool))
                NodeType(arg1, arg2));
        }

        template <typename NodeType, typename Arg1, typename Arg2, typename Arg3>
        inline NodeType* CreateAstNode(Arg1 arg1, Arg2 arg2, Arg3 arg3)
        {
            return (new (AllocateNode<NodeType>(m_memPool))
                NodeType(arg1, arg2, arg3));
        }

        template <typename NodeType, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
        inline NodeType* CreateAstNode(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4)
        {
            return (new (AllocateNode<NodeType>(m_memPool))
                NodeType(arg1, arg2, arg3, arg4));
        }

        inline AstProgram* CreateAstNode(AstGlobalVarDeclarationList *arg1, AstMain *arg2)
        {
            m_rootNode = new (AllocateNode<AstProgram>(m_memPool))
                AstProgram(arg1, arg2);
            return (safe_static_cast<AstProgram*>(m_rootNode));
        }
        
    private:
        AstNodeBuilder();

        template<typename T>
        inline char *AllocateNode(IMemoryPool& memPool)
        { return (memPool.Allocate(sizeof(T))); }

    private:
        IMemoryPool& m_memPool;
        IStringPool& m_StringPool;
        AstNode *m_rootNode;
    };
}

#endif