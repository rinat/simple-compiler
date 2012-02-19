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
#include "parser.h"
#include "astnodebuilder.h"

using namespace compiler;
using namespace utils::conversions;

Parser::Parser()
: m_sourceFile(NULL)
, tos(0)
, stack_size(0)
{
    m_errosObserver.reset(new CErrorsObserver(*this));
    m_scanner.AttachErrorsObserver(*m_errosObserver.get());
}

Parser::~Parser()
{}

void Parser::ReallocateStack()
{
    (!stack_size) ? stack_size = 128 : stack_size <<= 1;
    m_states.resize(stack_size + 1);
}

size_t Parser::NextOffset()
{
    return m_scanner.GetTokenOffset();
}

void Parser::UpdateNodePositions(AstNode *node, size_t startOffset)
{
    assert(node != NULL);
    node->m_beginOffset = startOffset;
    node->m_sourceFile = m_sourceFile;
}

void Parser::UpdateNodePositions(AstNode *node, AstNode *fromStart, AstNode *fromEnd)
{
    assert(node != NULL);
    if (fromStart)
    {
        node->m_beginOffset = fromStart->GetStartOffset();
        node->m_sourceFile = m_sourceFile;
    }
    if (fromEnd)
    {
        node->m_sourceFile = m_sourceFile;
    }
    else
    {
        node->m_sourceFile = m_sourceFile;
    }
}

void Parser::UpdateNodePositions(AstNode *node, AstNode *from)
{
    if (from)
    {
        node->m_beginOffset = from->GetStartOffset();
        node->m_sourceFile = m_sourceFile;
    }
    else
    {
        node->m_beginOffset = m_scanner.GetTokenOffset();
        node->m_sourceFile = m_sourceFile;
    }
}

bool Parser::Parse(ISourceFile& sourceFile, AstNodeBuilder& nodeBuilder)
{
    m_sourceFile = &sourceFile;

    ClearErrors();
    m_scanner.Reset(sourceFile);

    m_nodes.Clear();
    m_states.clear();

    const int INITIAL_STATE = 0;
    int yytoken = -1;
    int saved_yytoken = -1;

    ReallocateStack();

    tos = 0;
    m_states[++tos] = INITIAL_STATE;

    Token::Kind currentType = Token::Token_char; 
    Token::Kind currentComparision = Token::Token_Equals;
    Stack<AstIdentifierExpr> idStack;

    bool cycle = true;
    size_t offset = 0;
    while (cycle)
    {
        const int state = m_states [tos];
        if (yytoken == -1 && - TERMINAL_COUNT != action_index [state])
        {
            if (saved_yytoken == -1)
            {
                switch(m_scanner.Peek())
                {
                case Grammar::Token_Id:
                    idStack.PushBack(nodeBuilder.CreateAstNode<AstIdentifierExpr>(m_scanner.GetLiteralValue()));
                    UpdateNodePositions(idStack.Back(), m_scanner.GetNextTokenOffset());
                    break;
                case Grammar::Token_NumberConstant:
                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstNumberLiteralExpr>(m_scanner.GetLiteralValue()));
                    UpdateNodePositions(m_nodes.Back(), m_scanner.GetNextTokenOffset());
                    break;    
                case Grammar::Token_CharConstant:
                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstCharacterConstant>(m_scanner.GetLiteralValue()));
                    UpdateNodePositions(m_nodes.Back(), m_scanner.GetNextTokenOffset());
                    break;
                case Grammar::Token_StringConstant:
                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstStringLiteralExpr>(m_scanner.GetLiteralValue()));
                    UpdateNodePositions(m_nodes.Back(), m_scanner.GetNextTokenOffset());
                    break;
                }
                offset = NextOffset();
                yytoken = m_scanner.Next();
            }
            else
            {
                yytoken = saved_yytoken;
                saved_yytoken = -1;
            }
        }

        int act = t_action (state, yytoken);

        if (act == ACCEPT_STATE)
        {
            return (!HasErrors());
        }
        else if (act > 0)
        {
            if (++tos == stack_size)
                ReallocateStack();

            m_states [tos] = act;
            yytoken = -1;
        }
        else if (act < 0)
        {
            int rule = - act - 1;

            tos -= rhs [rule];
            act = m_states [tos++];

            //std::cout << (rule + 1) << " \n";

            switch (rule)
            {
            case 0: 
                {
                    // program ::= global_var_declarations main_function ;
                    AstMain *mainFunction = safe_static_cast<AstMain *, AstNode*>(m_nodes.PopBack());
                    AstGlobalVarDeclarationList *globalDecls = safe_static_cast<AstGlobalVarDeclarationList *>(m_nodes.PopBack());			
                    
                    nodeBuilder.CreateAstNode(globalDecls, mainFunction);
                    UpdateNodePositions(nodeBuilder.GetRootNode(), globalDecls, mainFunction);
                } 
                break;

            case 1: 
                {
                    // global_var_declarations ::= declaration Semicolon global_var_declarations ;
                    AstGlobalVarDeclarationList *next = safe_static_cast<AstGlobalVarDeclarationList *>(m_nodes.PopBack());
                    AstDeclaration *declaration = safe_static_cast<AstDeclaration *>(m_nodes.PopBack());

                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstGlobalVarDeclarationList>(declaration, next));
                    UpdateNodePositions(m_nodes.Back(), declaration, next);
                } 
                break;

            case 2: 
                {
                    // global_var_declarations ::= ;
                    m_nodes.PushBack(NULL);
                } 
                break;

            case 3: 
                {
                    // variable ::= Id array ;
                    AstArrayExpr *arr =  safe_static_cast<AstArrayExpr *>(m_nodes.PopBack());
                    AstIdentifierExpr *id = safe_static_cast<AstIdentifierExpr *>(idStack.PopBack());

                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstVariableExpr>(id, arr));	
                    UpdateNodePositions(m_nodes.Back(), id, arr);
                } 
                break;

            case 4: 
                {
                    // declaration ::= type variable ;
                    AstVariableExpr *var = safe_static_cast<AstVariableExpr *>(m_nodes.PopBack());
                    
                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstDeclaration>(currentType, var)); 
                    UpdateNodePositions(m_nodes.Back(), var);
                } 
                break;

            case 5: 
                {
                    // array ::= LBracket expression RBracket array_next ;
                    AstArrayExpr *next = safe_static_cast<AstArrayExpr *>(m_nodes.PopBack());
                    AstExpression *expr = safe_static_cast<AstExpression *>(m_nodes.PopBack());

                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstArrayExpr>(expr, next));
                    UpdateNodePositions(m_nodes.Back(), expr, next);
                } 
                break;

            case 6: 
                {
                    // array ::= ;
                    m_nodes.PushBack(NULL);
                }
                break;

            case 7: 
                {
                    // array_next ::= LBracket expression RBracket ;
                    AstExpression *expr = safe_static_cast<AstExpression *>(m_nodes.PopBack());
                    AstArrayExpr *next = NULL;

                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstArrayExpr>(expr, next));  
                    UpdateNodePositions(m_nodes.Back(), expr);
                } 
                break;

            case 8: 
                {
                    // array_next ::= ;
                    m_nodes.PushBack(NULL);
                }
                break;

            case 9: 
                {
                    // block ::= LBrace statements RBrace ;
                    AstStatementList *statements = safe_static_cast<AstStatementList *>(m_nodes.PopBack());
                    
                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstBlockStatement>(statements));
                    UpdateNodePositions(m_nodes.Back(), statements);
                } 
                break;

            case 10: 
                {
                    // block ::= Semicolon ;
                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstBlockStatement, AstStatementList*>(NULL));  
                    UpdateNodePositions(m_nodes.Back(), m_scanner.GetNextTokenOffset());
                } 
                break;

            case 11: 
                {
                    // main_function ::= main LParen RParen block ;
                    AstBlockStatement *block = safe_static_cast<AstBlockStatement *>(m_nodes.PopBack());

                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstMain>(block));
                    UpdateNodePositions(m_nodes.Back(), block);
                } 
                break;

            case 12: 
                {
                    // type ::= int ;
                    currentType = Token::Token_int;
                } 
                break;

            case 13: 
                {
                    // type ::= char ;
                    currentType = Token::Token_char;
                } 
                break;

            case 14: 
                {
                    // type ::= float ;
                    currentType = Token::Token_float;
                } 
                break;

            case 15: 
                {
                    // type ::= double ;
                    currentType = Token::Token_double;
                } 
                break;

            case 16: 
                {
                    // statements ::= statement statements ;
                    AstStatementList *next = safe_static_cast<AstStatementList *>(m_nodes.PopBack());
                    AstStatement *statement = safe_static_cast<AstStatement *>(m_nodes.PopBack());

                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstStatementList>(statement, next));
                    UpdateNodePositions(m_nodes.Back(), statement, next);
                } 
                break;

            case 17: 
                {
                    // statements ::= ;
                    m_nodes.PushBack(NULL);
                } 
                break;

            case 18: 
                {
                    // statement ::= if_statement ;
                } 
                break;

            case 19: 
                {
                    // statement ::= for_statement ;
                } 
                break;

            case 20: 
                {
                    // statement ::= while_statement ;
                } 
                break;

            case 21: 
                {
                    // statement ::= do_while_statement ;
                } 
                break;

            case 22: 
                {
                    // statement ::= assignment_statement Semicolon ;
                }
                break;

            case 23: 
                {
                    // statement ::= read_statement Semicolon ;
                } 
                break;

            case 24: 
                {
                    // statement ::= write_statement Semicolon ;
                } 
                break;

            case 25: 
                {
                    // statement ::= break_statement Semicolon ;
                } 
                break;

            case 26: 
                {
                    // for_statement ::= for LParen for_part Semicolon  condition Semicolon for_part RParen block ;
                    AstBlockStatement *statement = safe_static_cast<AstBlockStatement *>(m_nodes.PopBack());
                    AstAssignmentList *expression = safe_static_cast<AstAssignmentList *>(m_nodes.PopBack());
                    AstConditionExpression *condition = safe_static_cast<AstConditionExpression *>(m_nodes.PopBack());
                    AstAssignmentList *initializer = safe_static_cast<AstAssignmentList *>(m_nodes.PopBack());

                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstForStatement>(initializer, condition, expression, statement));
                    UpdateNodePositions(m_nodes.Back(), condition, statement);
                } 
                break;

            case 27: 
                {
                    // while_statement ::= while LParen condition RParen block ;
                    AstBlockStatement *statement = safe_static_cast<AstBlockStatement *>(m_nodes.PopBack());
                    AstConditionExpression *condition = safe_static_cast<AstConditionExpression *>(m_nodes.PopBack());
                    
                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstWhileStatement>(condition, statement));
                    UpdateNodePositions(m_nodes.Back(), condition, statement);
                } 
                break;

            case 28: 
                {
                    // do_while_statement ::= do block while LParen condition RParen ;
                    AstConditionExpression *condition = safe_static_cast<AstConditionExpression *>(m_nodes.PopBack());
                    AstBlockStatement *statement = safe_static_cast<AstBlockStatement *>(m_nodes.PopBack());				
                    
                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstDoWhileStatement>(statement, condition));
                    UpdateNodePositions(m_nodes.Back(), statement, condition);
                } 
                break;

            case 29: 
                {
                    // read_statement ::= read LParen read_args RParen ;
                    AstReadArgumentList *arguments = safe_static_cast<AstReadArgumentList *>(m_nodes.PopBack());				
                    
                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstReadStatement>(arguments));
                    UpdateNodePositions(m_nodes.Back(), arguments);
                } 
                break;

            case 30: 
                {
                    // write_statement ::= write LParen write_args RParen ;
                    AstWriteArgumentList *arguments = safe_static_cast<AstWriteArgumentList *>(m_nodes.PopBack());				
                    
                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstWriteStatement>(arguments));
                    UpdateNodePositions(m_nodes.Back(), arguments);
                } 
                break;

            case 31: 
                {
                    // if_statement ::= if LParen condition RParen block else_block ;
                    AstBlockStatement *elseBlock = safe_static_cast<AstBlockStatement *>(m_nodes.PopBack());
                    AstBlockStatement *thenBlock = safe_static_cast<AstBlockStatement *>(m_nodes.PopBack());
                    AstConditionExpression *condition = safe_static_cast<AstConditionExpression *>(m_nodes.PopBack());									
                    
                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstIfStatement>(condition, thenBlock, elseBlock));
                    if(elseBlock)
                    {
                        UpdateNodePositions(m_nodes.Back(), condition, elseBlock);
                    }
                    else
                    {
                        UpdateNodePositions(m_nodes.Back(), condition, thenBlock);
                    }
                } 
                break;

            case 32: 
                {
                    // break_statement ::= break ;
                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstBreakStatement>());
                    UpdateNodePositions(m_nodes.Back(), m_scanner.GetNextTokenOffset());
                } 
                break;

            case 33: 
                {
                    // condition ::= expression comparison expression ;
                    AstExpression *expr2 = safe_static_cast<AstExpression *>(m_nodes.PopBack());
                    AstExpression *expr1 = safe_static_cast<AstExpression *>(m_nodes.PopBack());

                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstConditionExpression>(expr1, expr2, currentComparision));
                    UpdateNodePositions(m_nodes.Back(), expr1, expr2);
                } 
                break;

            case 34: 
                {
                    // else_block ::= else block ;
                } 
                break;

            case 35: 
                {
                    // else_block ::= ;
                    m_nodes.PushBack(NULL);
                } 
                break;

            case 36: 
                {
                    // for_part ::= assignment_statement ;
                } 
                break;

            case 37: 
                {
                    // for_part ::= ;
                    m_nodes.PushBack(NULL);
                } 
                break;

            case 38: 
                {
                    // assignment_statement ::= assignment Comma assignment_statement ;
                    AstAssignmentList *next = safe_static_cast<AstAssignmentList *>(m_nodes.PopBack());
                    AstAssignmentExpr *assignment = safe_static_cast<AstAssignmentExpr *>(m_nodes.PopBack());

                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstAssignmentList>(assignment, next));
                    UpdateNodePositions(m_nodes.Back(), assignment, next);
                } 
                break;

            case 39: 
                {
                    // assignment_statement ::= assignment ;
                    AstAssignmentExpr *assignment = safe_static_cast<AstAssignmentExpr *>(m_nodes.PopBack());
                    AstAssignmentList *next = NULL;

                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstAssignmentList>(assignment, next));
                    UpdateNodePositions(m_nodes.Back(), assignment);
                } 
                break;

            case 40: 
                {
                    // assignment ::= variable Assign expression ;
                    AstExpression *expr = safe_static_cast<AstExpression *>(m_nodes.PopBack());
                    AstVariableExpr *var = safe_static_cast<AstVariableExpr *>(m_nodes.PopBack());

                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstAssignmentExpr>(var, expr));
                    UpdateNodePositions(m_nodes.Back(), var, expr);
                } 
                break;

            case 41: 
                {
                    // read_args ::= variable ;
                    AstVariableExpr *id = safe_static_cast<AstVariableExpr *>(m_nodes.PopBack());
                    AstReadArgumentList *next = NULL;

                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstReadArgumentList>(id, next));
                    UpdateNodePositions(m_nodes.Back(), id);
                } 
                break;

            case 42: 
                {
                    // read_args ::= variable Comma read_args ;
                    AstReadArgumentList *next = safe_static_cast<AstReadArgumentList *>(m_nodes.PopBack());
                    AstVariableExpr *id = safe_static_cast<AstVariableExpr *>(m_nodes.PopBack());

                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstReadArgumentList>(id, next));
                    UpdateNodePositions(m_nodes.Back(), id, next);
                } 
                break;

            case 43: 
                {
                    // write_args ::= write_arg ;
                    AstExpression *expr = safe_static_cast<AstExpression *>(m_nodes.PopBack());
                    AstWriteArgumentList *next = NULL;
                        
                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstWriteArgumentList>(expr, next));
                    UpdateNodePositions(m_nodes.Back(), expr);
                } 
                break;

            case 44: 
                {
                    // write_args ::= write_arg Comma write_args ;
                    AstWriteArgumentList *next = safe_static_cast<AstWriteArgumentList *>(m_nodes.PopBack());
                    AstExpression *expr = safe_static_cast<AstExpression *>(m_nodes.PopBack());

                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstWriteArgumentList>(expr, next));
                    UpdateNodePositions(m_nodes.Back(), expr, next);
                } 
                break;

            case 45: 
                {
                    // comparison ::= GT ;
                    currentComparision = Token::Token_GT;                     
                }
                break;

            case 46: 
                {
                    // comparison ::= LT ;
                    currentComparision = Token::Token_LT;                          
                } 
                break;

            case 47: 
                {
                    // comparison ::= GTE ;
                    currentComparision = Token::Token_GTE;                          
                } 
                break;

            case 48: 
                {
                    // comparison ::= LTE ;
                    currentComparision = Token::Token_LTE;                      
                } 
                break;

            case 49: 
                {
                    // comparison ::= NE ;
                    currentComparision = Token::Token_NE;                          
                } 
                break;

            case 50: 
                {
                    // comparison ::= Equals ;
                    currentComparision = Token::Token_Equals;                          
                } 
                break;

            case 51: 
                {
                    // expression ::= term ;
                } 
                break;

            case 52: 
                {
                    // expression ::= add_expression ;
                } 
                break;

            case 53: 
                {
                    // expression ::= sub_expression ;
                } 
                break;

            case 54: 
                {
                    // add_expression ::= expression Add term 
                    AstTerm *term = safe_static_cast<AstTerm *>(m_nodes.PopBack());
                    AstExpression *expr = safe_static_cast<AstExpression *>(m_nodes.PopBack());

                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstAddExpr>(expr, term));
                    UpdateNodePositions(m_nodes.Back(), term, expr);
                } 
                break;

            case 55:
                {
                    // sub_expression ::= expression Sub term;
                    AstTerm *term = safe_static_cast<AstTerm *>(m_nodes.PopBack());
                    AstExpression *expr = safe_static_cast<AstExpression *>(m_nodes.PopBack());

                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstSubExpr>(expr, term));
                    UpdateNodePositions(m_nodes.Back(), term, expr);
                } 
                break;

            case 56:
                {
                    // term ::= unary_expression ;
                    AstUnaryExpr *expr = safe_static_cast<AstUnaryExpr *>(m_nodes.PopBack());

                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstTerm>(expr));
                    UpdateNodePositions(m_nodes.Back(), expr);
                } 
                break;

            case 57:
                {
                    // term ::= mult_expression ;
                    AstExpression *expr = safe_static_cast<AstMultExpr *>(m_nodes.PopBack());

                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstTerm>(expr));
                    UpdateNodePositions(m_nodes.Back(), expr);
                } 
                break;

            case 58:
                {
                    // term ::= div_expression ;
                    AstExpression *expr = safe_static_cast<AstDivExpr *>(m_nodes.PopBack());

                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstTerm>(expr));
                    UpdateNodePositions(m_nodes.Back(), expr);
                } 
                break;

            case 59:
                {
                    // mult_expression ::= unary_expression Mult term ;
                    AstTerm *term = safe_static_cast<AstTerm *>(m_nodes.PopBack());
                    AstUnaryExpr *expr = safe_static_cast<AstUnaryExpr *>(m_nodes.PopBack());

                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstMultExpr>(expr, term));
                    UpdateNodePositions(m_nodes.Back(), expr, term);
                } 
                break;

            case 60:
                {
                    // div_expression ::= unary_expression Div term ;
                    AstTerm *term = safe_static_cast<AstTerm *>(m_nodes.PopBack());
                    AstUnaryExpr *expr = safe_static_cast<AstUnaryExpr *>(m_nodes.PopBack());

                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstDivExpr>(expr, term));
                    UpdateNodePositions(m_nodes.Back(), expr, term);
                } 
                break;

            case 61:
                {
                    // unary_expression ::= unary_operator factor ;
                    AstFactorExpr *factor = safe_static_cast<AstFactorExpr *>(m_nodes.PopBack());

                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstUnaryExpr>(Token::Token_Sub, factor));
                    UpdateNodePositions(m_nodes.Back(), factor);
                     
                } break;

            case 62:
                {
                    // unary_expression ::= factor ;
                    AstFactorExpr *factor = safe_static_cast<AstFactorExpr *>(m_nodes.PopBack());

                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstUnaryExpr>(Token::Token_Add, factor));
                    UpdateNodePositions(m_nodes.Back(), factor);
                     
                } 
                break;

            case 63:
                {
                    // factor ::= variable ;
                    AstExpression *expr = safe_static_cast<AstVariableExpr *>(m_nodes.PopBack());

                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstFactorExpr>(expr));
                    UpdateNodePositions(m_nodes.Back(), expr);
                } 
                break;

            case 64:
                {
                    // factor ::= NumberConstant ;
                    AstExpression *expr = safe_static_cast<AstNumberLiteralExpr *>(m_nodes.PopBack());

                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstFactorExpr>(expr));
                    UpdateNodePositions(m_nodes.Back(), expr);
                } 
                break;

            case 65:
                {
                    // factor ::= LParen expression RParen ;
                    AstExpression *expr = safe_static_cast<AstExpression *>(m_nodes.PopBack());

                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstFactorExpr>(expr));
                    UpdateNodePositions(m_nodes.Back(), expr);
                } 
                break;

            case 66:
                {
                    // unary_operator ::= Sub ;
                } 
                break;

            case 67: 
                {
                    // factor ::= StringConstant ;
                    AstExpression *expr = safe_static_cast<AstStringLiteralExpr *>(m_nodes.PopBack());

                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstFactorExpr>(expr));
                    UpdateNodePositions(m_nodes.Back(), expr);
                } 
                break;

            case 68: 
                {
                    // factor ::= CharConstant ;
                    AstExpression *expr = safe_static_cast<AstCharacterConstant *>(m_nodes.PopBack());

                    m_nodes.PushBack(nodeBuilder.CreateAstNode<AstFactorExpr>(expr));     
                    UpdateNodePositions(m_nodes.Back(), expr);
                } 
                break;
            }
            
            m_states [tos] = nt_action (act, lhs [rule] - TERMINAL_COUNT);
        }

      else
      {
          int ers = state;
          int shifts = 0;
          int reduces = 0;
          int expected_tokens [3];
          for (int tk = 0; tk < TERMINAL_COUNT; ++tk)
          {
              int k = t_action (ers, tk);

              if (! k)
                  continue;
              else if (k < 0)
                  ++reduces;
              else if (spell [tk])
              {
                  if (shifts < 3)
                      expected_tokens [shifts] = tk;
                  ++shifts;
              }
          }
          std::string errorMessage;
          if (shifts && shifts < 3)
          {
              bool first = true;
              for (int s = 0; s < shifts; ++s)
              {
                  if (first)
                  {
                      errorMessage += "Expected ";
                  }
                  else
                      errorMessage += ", ";

                  first = false;
                  errorMessage += "'";
                  errorMessage += spell [expected_tokens [s]];
                  errorMessage += "'";
              }

          }
          ErrorReport(CError::Error_GenericParserError, errorMessage);
          return (false);
        }
    }

    return (!HasErrors());
}

void Parser::ErrorReport(CError::ErrorType type, const std::string& errorMessage)
{
    AddError(IErrorsContainer::IErrorPtr(
        new CError(type, SourcePosition(m_sourceFile, m_scanner.GetTokenOffset()), errorMessage)));
}
