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
#include "ParserTest.h"
#include "parser.h"
#include "astnodebuilder.h"
#include "memorypool.h"
#include "stringpool.h"
#include "sourcefile.h"

using namespace compiler;

#pragma warning(push)
#pragma warning(disable:4189) // warning about unused variables 

class ParserTestHelper : private utils::NonCopyable
{
public:
    ParserTestHelper(Parser &parser, AstNodeBuilder* builder)
        : m_parser(parser)
        , m_builder(builder)
    {};

    void operator () (const std::string& source)
    {
        m_sourceFile.reset(new SourceFile("test.as"));
        ITextBuffer *buffer = m_sourceFile->GetBuffer(source);
        buffer;
        TEST_CHECK(buffer != NULL);

        m_parser.Parse(*m_sourceFile.get(), *m_builder);
    }

private:
    Parser &m_parser;
    AstNodeBuilder* m_builder;
    std::auto_ptr<SourceFile> m_sourceFile;
};

void PraserTest::StartTest()
{
    ParserSimpleTestWithError();
    ParserSimpleTest();
    ParserMaintTest();
    ParserIdentifierTest();
    ParserIdentifierArrayTest();
    ParserStringLiteralTest();
    ParserWhileStatementTest();
    ParserDoWhileStatementTest();
}

void PraserTest::ParserSimpleTestWithError()
{
    Parser parser;
    MemoryPool pool;
    StringPool strPool;
    AstNodeBuilder builder(pool, strPool);
    ParserTestHelper tester(parser, &builder);
    tester(" \
           int id;\
           int main()\
           {\
           id = 2;\
           }\
           ");
    TEST_CHECK(parser.HasErrors());
}

void PraserTest::ParserSimpleTest()
{
    Parser parser;
    MemoryPool pool;
    StringPool strPool;
    AstNodeBuilder builder(pool, strPool);
    ParserTestHelper tester(parser, &builder);
    tester(" \
           int id;\
           main()\
           {\
           id = 2;\
           }\
           ");
    TEST_CHECK(!parser.HasErrors());
}

void PraserTest::ParserMaintTest()
{
    Parser parser;
    MemoryPool pool;
    StringPool strPool;
    AstNodeBuilder builder(pool, strPool);
    ParserTestHelper tester(parser, &builder);
    tester(" \
          main()\
          {\
          }\
          ");

    AstProgram *program = dynamic_cast<AstProgram*>(builder.GetRootNode());
    TEST_CHECK(program != NULL);
    TEST_CHECK(program->m_globalDecls == NULL);

    AstMain *main = program->m_mainFunction;
    TEST_CHECK(main != NULL);

    AstBlockStatement *blockStatement = main->m_block;
    blockStatement;
    TEST_CHECK(blockStatement != NULL);
    TEST_CHECK(blockStatement->m_statements == NULL);

    TEST_CHECK(!parser.HasErrors());
}

void PraserTest::ParserIdentifierTest()
{
    Parser parser;
    MemoryPool pool;
    StringPool strPool;
    AstNodeBuilder builder(pool, strPool);
    ParserTestHelper tester(parser, &builder);
    tester(" \
           main()\
           {\
               id = 12; \
           }\
           ");

    AstProgram *program = dynamic_cast<AstProgram*>(builder.GetRootNode());
    TEST_CHECK(program != NULL);
    TEST_CHECK(program->m_globalDecls == NULL);

    AstMain *main = program->m_mainFunction;
    TEST_CHECK(main != NULL);

    AstBlockStatement *blockStatement = main->m_block;
    blockStatement;
    TEST_CHECK(blockStatement != NULL);

    AstStatementList *statementList = blockStatement->m_statements;
    TEST_CHECK(statementList != NULL);

    AstAssignmentList *assignmentList = dynamic_cast<AstAssignmentList*>(statementList->m_statement);
    TEST_CHECK(assignmentList != NULL);

    AstAssignmentExpr *aasignmentExpr = dynamic_cast<AstAssignmentExpr*>(assignmentList->m_assignment);
    TEST_CHECK(aasignmentExpr != NULL);
    TEST_CHECK(aasignmentExpr->m_variable != NULL);
    TEST_CHECK(aasignmentExpr->m_expression != NULL);

    AstVariableExpr *varExpr = aasignmentExpr->m_variable;
    TEST_CHECK(varExpr->m_array == NULL);
    TEST_CHECK(varExpr->m_identifier != NULL);

    AstIdentifierExpr *id = varExpr->m_identifier;
    TEST_CHECK(id->m_name != NULL);

    TEST_CHECK(id->GetName() == "id");

    AstTerm *term = dynamic_cast<AstTerm*>(aasignmentExpr->m_expression);
    TEST_CHECK(term != NULL);
    TEST_CHECK(term->m_expression != NULL);
    
    AstUnaryExpr *unaryExpr = dynamic_cast<AstUnaryExpr*>(term->m_expression);
    TEST_CHECK(unaryExpr->m_factor != NULL);

    AstFactorExpr *factorExpr = unaryExpr->m_factor;
    TEST_CHECK(factorExpr->m_expression != NULL);

    AstNumberLiteralExpr *numLiteral = dynamic_cast<AstNumberLiteralExpr*>(factorExpr->m_expression);
    TEST_CHECK(numLiteral->m_value != NULL);
    TEST_CHECK(numLiteral->GetName() == "12");

    TEST_CHECK(!parser.HasErrors());
}

void PraserTest::ParserIdentifierArrayTest()
{
    Parser parser;
    MemoryPool pool;
    StringPool strPool;
    AstNodeBuilder builder(pool, strPool);
    ParserTestHelper tester(parser, &builder);
    tester(" \
           main()\
           {\
           id[13] = 12; \
           }\
           ");

    AstProgram *program = dynamic_cast<AstProgram*>(builder.GetRootNode());
    TEST_CHECK(program != NULL);
    TEST_CHECK(program->m_globalDecls == NULL);

    AstMain *main = program->m_mainFunction;
    TEST_CHECK(main != NULL);

    AstBlockStatement *blockStatement = main->m_block;
    blockStatement;
    TEST_CHECK(blockStatement != NULL);

    AstStatementList *statementList = blockStatement->m_statements;
    TEST_CHECK(statementList != NULL);

    AstAssignmentList *assignmentList = dynamic_cast<AstAssignmentList*>(statementList->m_statement);
    TEST_CHECK(assignmentList != NULL);

    AstAssignmentExpr *aasignmentExpr = dynamic_cast<AstAssignmentExpr*>(assignmentList->m_assignment);
    TEST_CHECK(aasignmentExpr != NULL);
    TEST_CHECK(aasignmentExpr->m_variable != NULL);
    TEST_CHECK(aasignmentExpr->m_expression != NULL);

    AstVariableExpr *varExpr = aasignmentExpr->m_variable;
    TEST_CHECK(varExpr->m_array != NULL);
    TEST_CHECK(varExpr->m_identifier != NULL);

    AstArrayExpr *arrayExpr = varExpr->m_array;
    TEST_CHECK(arrayExpr->m_nextArray == NULL);
    
    AstTerm *arrTerm = dynamic_cast<AstTerm*>(arrayExpr->m_expression);
    TEST_CHECK(arrTerm != NULL);
    TEST_CHECK(arrTerm != NULL);
    TEST_CHECK(arrTerm->m_expression != NULL);

    // array size check
    {
        AstUnaryExpr *unaryExpr = dynamic_cast<AstUnaryExpr*>(arrTerm->m_expression);
        TEST_CHECK(unaryExpr->m_factor != NULL);

        AstFactorExpr *factorExpr = unaryExpr->m_factor;
        TEST_CHECK(factorExpr->m_expression != NULL);

        AstNumberLiteralExpr *numLiteral = dynamic_cast<AstNumberLiteralExpr*>(factorExpr->m_expression);
        TEST_CHECK(numLiteral->m_value != NULL);
        TEST_CHECK(numLiteral->GetName() == "13");
    }

    AstIdentifierExpr *id = varExpr->m_identifier;
    TEST_CHECK(id->m_name != NULL);

    TEST_CHECK(id->GetName() == "id");

    AstTerm *term = dynamic_cast<AstTerm*>(aasignmentExpr->m_expression);
    TEST_CHECK(term != NULL);
    TEST_CHECK(term->m_expression != NULL);

    AstUnaryExpr *unaryExpr = dynamic_cast<AstUnaryExpr*>(term->m_expression);
    TEST_CHECK(unaryExpr->m_factor != NULL);

    AstFactorExpr *factorExpr = unaryExpr->m_factor;
    TEST_CHECK(factorExpr->m_expression != NULL);

    AstNumberLiteralExpr *numLiteral = dynamic_cast<AstNumberLiteralExpr*>(factorExpr->m_expression);
    TEST_CHECK(numLiteral->m_value != NULL);
    TEST_CHECK(numLiteral->GetName() == "12");

    TEST_CHECK(!parser.HasErrors());
}

void PraserTest::ParserStringLiteralTest()
{
    Parser parser;
    MemoryPool pool;
    StringPool strPool;
    AstNodeBuilder builder(pool, strPool);
    ParserTestHelper tester(parser, &builder);
    tester(" \
           main()\
           {\
           id = \"hello world\"; \
           }\
           ");

    AstProgram *program = dynamic_cast<AstProgram*>(builder.GetRootNode());
    TEST_CHECK(program != NULL);
    TEST_CHECK(program->m_globalDecls == NULL);

    AstMain *main = program->m_mainFunction;
    TEST_CHECK(main != NULL);

    AstBlockStatement *blockStatement = main->m_block;
    blockStatement;
    TEST_CHECK(blockStatement != NULL);

    AstStatementList *statementList = blockStatement->m_statements;
    TEST_CHECK(statementList != NULL);

    AstAssignmentList *assignmentList = dynamic_cast<AstAssignmentList*>(statementList->m_statement);
    TEST_CHECK(assignmentList != NULL);

    AstAssignmentExpr *aasignmentExpr = dynamic_cast<AstAssignmentExpr*>(assignmentList->m_assignment);
    TEST_CHECK(aasignmentExpr != NULL);
    TEST_CHECK(aasignmentExpr->m_variable != NULL);
    TEST_CHECK(aasignmentExpr->m_expression != NULL);

    AstVariableExpr *varExpr = aasignmentExpr->m_variable;
    TEST_CHECK(varExpr->m_array == NULL);
    TEST_CHECK(varExpr->m_identifier != NULL);


    AstIdentifierExpr *id = varExpr->m_identifier;
    TEST_CHECK(id->m_name != NULL);

    TEST_CHECK(id->GetName() == "id");

    AstTerm *term = dynamic_cast<AstTerm*>(aasignmentExpr->m_expression);
    TEST_CHECK(term != NULL);
    TEST_CHECK(term->m_expression != NULL);

    AstUnaryExpr *unaryExpr = dynamic_cast<AstUnaryExpr*>(term->m_expression);
    TEST_CHECK(unaryExpr->m_factor != NULL);

    AstFactorExpr *factorExpr = unaryExpr->m_factor;
    TEST_CHECK(factorExpr->m_expression != NULL);

    AstStringLiteralExpr *stringLit = dynamic_cast<AstStringLiteralExpr*>(factorExpr->m_expression);
    TEST_CHECK(stringLit->m_name != NULL);
    TEST_CHECK(stringLit->GetName() == "hello world");

    TEST_CHECK(!parser.HasErrors());
}

void PraserTest::ParserWhileStatementTest()
{
    Parser parser;
    MemoryPool pool;
    StringPool strPool;
    AstNodeBuilder builder(pool, strPool);
    ParserTestHelper tester(parser, &builder);
    tester(" \
           main()\
           {\
           while(1 < 2) {}\
           }\
           ");

    AstProgram *program = dynamic_cast<AstProgram*>(builder.GetRootNode());
    TEST_CHECK(program != NULL);
    TEST_CHECK(program->m_globalDecls == NULL);

    AstMain *main = program->m_mainFunction;
    TEST_CHECK(main != NULL);

    AstBlockStatement *blockStatement = main->m_block;
    blockStatement;
    TEST_CHECK(blockStatement != NULL);

    AstStatementList *statementList = blockStatement->m_statements;
    TEST_CHECK(statementList != NULL);
    TEST_CHECK(statementList->m_next == NULL);
    TEST_CHECK(statementList->m_statement != NULL);

    AstWhileStatement *dowhileStatement = dynamic_cast<AstWhileStatement*>(statementList->m_statement);
    TEST_CHECK(dowhileStatement != NULL);

    AstConditionExpression *codition = dowhileStatement->m_condition;
    TEST_CHECK(codition != NULL);
    TEST_CHECK(codition->m_expr1 != NULL);
    TEST_CHECK(codition->m_expr2 != NULL);
    TEST_CHECK(codition->m_operation == Token::Token_LT);

    // left part of condition
    {
        AstTerm *term = dynamic_cast<AstTerm*>(codition->m_expr1);
        TEST_CHECK(term != NULL);
        TEST_CHECK(term->m_expression != NULL);
        AstUnaryExpr *unaryExpr = dynamic_cast<AstUnaryExpr*>(term->m_expression);
        TEST_CHECK(unaryExpr != NULL);

        AstFactorExpr *factorExpr = unaryExpr->m_factor;
        TEST_CHECK(factorExpr->m_expression != NULL);

        AstNumberLiteralExpr *numberLiteral = dynamic_cast<AstNumberLiteralExpr*>(factorExpr->m_expression);
        TEST_CHECK(numberLiteral->m_value != NULL);
        TEST_CHECK(numberLiteral->GetName() == "1");
    }

    // right part of condition
    {
        AstTerm *term = dynamic_cast<AstTerm*>(codition->m_expr2);
        TEST_CHECK(term != NULL);
        TEST_CHECK(term->m_expression != NULL);
        AstUnaryExpr *unaryExpr = dynamic_cast<AstUnaryExpr*>(term->m_expression);
        TEST_CHECK(unaryExpr != NULL);

        AstFactorExpr *factorExpr = unaryExpr->m_factor;
        TEST_CHECK(factorExpr->m_expression != NULL);

        AstNumberLiteralExpr *numberLiteral = dynamic_cast<AstNumberLiteralExpr*>(factorExpr->m_expression);
        TEST_CHECK(numberLiteral->m_value != NULL);
        TEST_CHECK(numberLiteral->GetName() == "2");
    }

    // while block
    {
        AstBlockStatement *blockStatement = dowhileStatement->m_statement;
        TEST_CHECK(blockStatement != NULL);
        TEST_CHECK(blockStatement->m_statements == NULL);
    }

    TEST_CHECK(!parser.HasErrors());
}

void PraserTest::ParserDoWhileStatementTest()
{
    Parser parser;
    MemoryPool pool;
    StringPool strPool;
    AstNodeBuilder builder(pool, strPool);
    ParserTestHelper tester(parser, &builder);
    tester(" \
           main()\
           {\
           do \
           {} \
           while(1 < simpleIdentifier)\
           }\
           ");

    AstProgram *program = dynamic_cast<AstProgram*>(builder.GetRootNode());
    TEST_CHECK(program != NULL);
    TEST_CHECK(program->m_globalDecls == NULL);

    AstMain *main = program->m_mainFunction;
    TEST_CHECK(main != NULL);

    AstBlockStatement *blockStatement = main->m_block;
    blockStatement;
    TEST_CHECK(blockStatement != NULL);

    AstStatementList *statementList = blockStatement->m_statements;
    TEST_CHECK(statementList != NULL);
    TEST_CHECK(statementList->m_next == NULL);
    TEST_CHECK(statementList->m_statement != NULL);

    AstDoWhileStatement *dowhileStatement = dynamic_cast<AstDoWhileStatement*>(statementList->m_statement);
    TEST_CHECK(dowhileStatement != NULL);

    AstConditionExpression *codition = dowhileStatement->m_condition;
    TEST_CHECK(codition != NULL);
    TEST_CHECK(codition->m_expr1 != NULL);
    TEST_CHECK(codition->m_expr2 != NULL);
    TEST_CHECK(codition->m_operation == Token::Token_LT);

    // left part of condition
    {
        AstTerm *term = dynamic_cast<AstTerm*>(codition->m_expr1);
        TEST_CHECK(term != NULL);
        TEST_CHECK(term->m_expression != NULL);
        AstUnaryExpr *unaryExpr = dynamic_cast<AstUnaryExpr*>(term->m_expression);
        TEST_CHECK(unaryExpr != NULL);

        AstFactorExpr *factorExpr = unaryExpr->m_factor;
        TEST_CHECK(factorExpr->m_expression != NULL);

        AstNumberLiteralExpr *numberLiteral = dynamic_cast<AstNumberLiteralExpr*>(factorExpr->m_expression);
        TEST_CHECK(numberLiteral->m_value != NULL);
        TEST_CHECK(numberLiteral->GetName() == "1");
    }

    // right part of condition
    {
        AstTerm *term = dynamic_cast<AstTerm*>(codition->m_expr2);
        TEST_CHECK(term != NULL);
        TEST_CHECK(term->m_expression != NULL);
        AstUnaryExpr *unaryExpr = dynamic_cast<AstUnaryExpr*>(term->m_expression);
        TEST_CHECK(unaryExpr != NULL);

        AstFactorExpr *factorExpr = unaryExpr->m_factor;
        TEST_CHECK(factorExpr->m_expression != NULL);

        AstVariableExpr *variableExpr = dynamic_cast<AstVariableExpr*>(factorExpr->m_expression);
        TEST_CHECK(variableExpr != NULL);
        TEST_CHECK(variableExpr->m_identifier != NULL);
        TEST_CHECK(variableExpr->m_array == NULL);

        AstIdentifierExpr *idExpr = variableExpr->m_identifier;
        TEST_CHECK(idExpr->m_name != NULL);
        TEST_CHECK(idExpr->GetName() == "simpleIdentifier");
    }

    // while block
    {
        AstBlockStatement *blockStatement = dowhileStatement->m_statement;
        TEST_CHECK(blockStatement != NULL);
        TEST_CHECK(blockStatement->m_statements == NULL);
    }

    TEST_CHECK(!parser.HasErrors());
}

#pragma warning(pop)
