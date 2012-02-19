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
#include "llvmcodegenerator.h"

using namespace compiler;
using namespace utils::conversions;
using namespace std;

namespace
{
    const std::string RETURN_LABEL = "return";

    std::string CreateVariableName(std::string variableName, size_t counter)
    {        
        return ("%" + variableName + NumberToString<size_t>(counter));
    }

    Token::Kind DetectNumberType(const std::string& numberStr)
    {    
        bool isFloat = (numberStr.find('.') != std::string::npos);    

        Token::Kind resultKind = (isFloat) ? Token::Token_double
            : Token::Token_int;
        return resultKind;
    }

    std::string GetCmpOperationByType(Token::Kind type)
    {
        return ((type == Token::Token_int) || (type == Token::Token_char))
            ? "icmp"
            // double and float
            : "fcmp";
    }

    std::string GetConditionNameByKind(Token::Kind conditionKind, Token::Kind type)
    {
        std::map<Token::Kind, std::string> conditionsNames;
        conditionsNames[Token::Token_Equals] = (type == Token::Token_double) ? "oeq" : "eq";
        conditionsNames[Token::Token_NE] = (type == Token::Token_double) ? "oneq" : "ne";
        conditionsNames[Token::Token_GT] = (type == Token::Token_double) ? "ogt" : "sgt";
        conditionsNames[Token::Token_GTE] = (type == Token::Token_double) ? "oge" : "sge";
        conditionsNames[Token::Token_LT] = (type == Token::Token_double) ? "olt" : "slt";
        conditionsNames[Token::Token_LTE] = (type == Token::Token_double) ? "ole" : "sle";

        return (conditionsNames.find(conditionKind) != conditionsNames.end()) ? 
            conditionsNames[conditionKind] : "";
    }

    std::string GetOperationName(const ProgramElement& operation, Token::Kind operandType)
    {
        std::string resultName = "";
        switch (operation.m_kind)
        {
        case Token::Token_Add:
            resultName = ((operandType == Token::Token_int) || 
                (operandType == Token::Token_char)) ? 
                "add" :
                "fadd";
            break;

        case Token::Token_Sub:
            resultName = ((operandType == Token::Token_int) || 
                (operandType == Token::Token_char)) ?
                "sub" :
                "fsub";
            break;

        case Token::Token_Mult:
            resultName = ((operandType == Token::Token_int) || 
                (operandType == Token::Token_char)) ?
                "mul" :
                "fmul";
            break;

        case Token::Token_Div:
            resultName = ((operandType == Token::Token_int) || 
                (operandType == Token::Token_char)) ?
                "sdiv" :
                "fdiv";
            break;

        default:
            break;
        }

        return resultName;
    }

    std::string GetTypeNameByKind(Token::Kind kind)
    {
        static std::map<Token::Kind, std::string> typesNames;
        if (typesNames.size() == 0)
        {
            typesNames[Token::Token_char] = "i8";
            typesNames[Token::Token_int] = "i32";
            typesNames[Token::Token_float] = "float";
            typesNames[Token::Token_double] = "double";
        }

        return (typesNames.find(kind) != typesNames.end()) ? 
            typesNames[kind] : "";       
    }

    bool IsBinaryArithmeticOperation(const ProgramElement& operation)
    {
        static std::set<std::string> binaryOperations;
        if (binaryOperations.size() == 0)
        {
            binaryOperations.insert("+");
            binaryOperations.insert("-");
            binaryOperations.insert("*");
            binaryOperations.insert("/");
        }

        return (binaryOperations.find(operation.m_value) != binaryOperations.end());
    }

    void SerializeGlobalDeclarationIdentifier(std::ostream& outStream, 
        std::string identifierName,
        const Token::Kind& identifierKind)
    {
        outStream << "@" << identifierName
            << " = common global " 
            << GetTypeNameByKind(identifierKind)
            << " " 
            << (((identifierKind == Token::Token_int) || 
                (identifierKind == Token::Token_char)) ? "0" : "0.000000e+00")
            << std::endl;
    }

    void SerializeGlobalDeclarationArray(std::ostream& outStream, 
        std::string identifierName,
        std::string size,
        const Token::Kind& identifierKind)
    {
        outStream << "@" << identifierName
            << " = global "
            << "[" << size
            << " x " << GetTypeNameByKind(identifierKind) << "]"
            << " zeroinitializer"
            << std::endl;
    }

    std::string GetFormatStringNameByType(Token::Kind kind)
    {
        return "@" + GetTypeNameByKind(kind) + ".str";
    }

    void SerializeDeclaringOfFunction(std::ostream& outStream, std::string functionName,
        std::string returnType, std::string arguments)
    {
        outStream << "declare "
            << returnType 
            << " "
            << "@" << functionName
            << "(" << arguments << ")"
            << " nounwind"
            << std::endl;
    }

    void SerializeWriteConstantFormatStrings(std::ostream& outStream)
    {
        static std::string charStringName(GetFormatStringNameByType(Token::Token_char));
        static std::string intStringName(GetFormatStringNameByType(Token::Token_int));
        static std::string floatStringName(GetFormatStringNameByType(Token::Token_float));
        static std::string doubleStringName(GetFormatStringNameByType(Token::Token_double));

        outStream << charStringName << ".write = private constant [3 x i8] " << "c\"%c\\00\"" << std::endl;
        outStream << intStringName << ".write = private constant [3 x i8] " << "c\"%d\\00\"" << std::endl;
        outStream << floatStringName << ".write = private constant [3 x i8] " << "c\"%f\\00\"" << std::endl;
        outStream << doubleStringName << ".write = private constant [4 x i8] " << "c\"%lf\\00\"" << std::endl;            
    }

    void SerializeReadConstantFormatStrings(std::ostream& outStream)
    {
        static std::string charStringName(GetFormatStringNameByType(Token::Token_char));
        static std::string intStringName(GetFormatStringNameByType(Token::Token_int));
        static std::string floatStringName(GetFormatStringNameByType(Token::Token_float));
        static std::string doubleStringName(GetFormatStringNameByType(Token::Token_double));

        outStream << charStringName << ".read = private constant [3 x i8] " << "c\"%c\\00\"" << std::endl;
        outStream << intStringName << ".read = private constant [3 x i8] " << "c\"%d\\00\"" << std::endl;
        outStream << floatStringName << ".read = private constant [3 x i8] " << "c\"%f\\00\"" << std::endl;
        outStream << doubleStringName << ".read = private constant [4 x i8] " << "c\"%lf\\00\"" << std::endl;            
    }

    void SerializeStartMainFunction(std::ostream& outStream)
    {
        outStream << "define i32 @main() nounwind {" << std::endl;
        outStream << "entry:" << std::endl;
        outStream << "\t%retval = alloca i32" << std::endl;
        outStream << "\t%\"alloca point\" = bitcast i32 0 to i32" << std::endl;
    }

    void SerializeEndMainFunction(std::ostream& outStream, std::string endLabel)
    {    
        outStream << "\tstore i32 0, i32* %retval, align 4" << std::endl;
        outStream << "\tbr label %" << endLabel << std::endl;
        outStream << endLabel << ":" << std::endl;
        outStream << "\t%retval1 = load i32* %retval" << std::endl;
        outStream << "\tret i32 %retval1" << std::endl;
        outStream << "}" << std::endl;
    }
}

void LLVMCodeGenerator::ReadAllVariables()
{
    std::vector<ProgramElement> reverseReadArgs;    
    while (!m_programStack.empty())
    {
        ProgramElement topElement = m_programStack.top();
        reverseReadArgs.push_back(topElement);        
        m_programStack.pop();
    }

    // Reverse read stack
    std::vector<ProgramElement>::reverse_iterator rIterator = reverseReadArgs.rbegin();
    for(; rIterator != reverseReadArgs.rend(); ++rIterator)
    {	
        ReadVariable(*rIterator);
    }
}

void LLVMCodeGenerator::ReadVariable(const ProgramElement& identifier)
{
    if (!CheckVariable(identifier))
    {
        return;
    }
    // Check variable-array comparability
    if (!identifier.IsArray() && m_identifiersTable[identifier.m_value].IsArray())
    {
        ErrorReport(CError::Error_ReadArray, identifier.m_beginOffset, identifier.m_value);
        return;
    }

    Token::Kind kind = m_identifiersTable[identifier.m_value].m_kind;
    bool isArray = !m_identifiersTable[identifier.m_value].m_arraySize.empty();

    if (isArray)
    {
        std::string typeName = GetTypeNameByKind(kind);
        std::string arrayElementVariable = CreateVariableName(identifier.m_value, m_identifiersTable[identifier.m_value].m_counter);
        ++m_identifiersTable[identifier.m_value].m_counter;
        *m_outStream << "\t"
            << arrayElementVariable
            << " = " << "getelementptr inbounds ["
            << m_identifiersTable[identifier.m_value].m_arraySize
            << " x " << typeName << "]*"
            << " @" << identifier.m_value
            << ", " << typeName << " 0, "
            << typeName << " " << identifier.m_index << std::endl;


        *m_outStream << "\t"
            << CreateVariableName("", m_counters.m_elementsCounter)
            << " = call i32 (i8*, ...)* @scanf(i8* getelementptr inbounds ([" 
            << ((kind == Token::Token_double) ? "4" : "3") << " x i8]* "
            << GetFormatStringNameByType(kind) << ".read"
            << ", i32 0, i32 0), "
            << GetTypeNameByKind(kind) << "* " << arrayElementVariable
            << ") nounwind"
            << std::endl;

        ++m_counters.m_elementsCounter;	
    }
    else
    {
        *m_outStream << "\t"
            << CreateVariableName("", m_counters.m_elementsCounter)
            << " = call i32 (i8*, ...)* @scanf(i8* getelementptr inbounds ([" 
            << ((kind == Token::Token_double) ? "4" : "3") << " x i8]* "
            << GetFormatStringNameByType(kind) << ".read"
            << ", i32 0, i32 0), "
            << GetTypeNameByKind(kind) << "* @" << identifier.m_value
            << ") nounwind"
            << std::endl;

        ++m_counters.m_elementsCounter;
    }	
}

void LLVMCodeGenerator::WriteAllStackElements()
{
    std::vector<ProgramElement> reverseWriteArgs;    
    while (m_programStack.size() != 0)
    {        
        ProgramElement element = PopAndLoadProgramElement();
        switch (element.m_kind)
        {
        case Token::Token_Id:
            {
                std::vector<ProgramElement> elements = LoadArrayToVariables(element.m_value);
                reverseWriteArgs.insert(reverseWriteArgs.end(), elements.rbegin(), elements.rend());
            }
            break;
        case Token::Token_StringConstant:
            {
                const std::string stringLiteral = element.m_value;
                for (size_t i = 0; i < stringLiteral.size(); ++i)
                {
                    std::string valueStr = NumberToString<int>((int)(stringLiteral[i]));
                    m_programStack.push(ProgramElement(valueStr, Token::Token_char, element.m_beginOffset));   
                }
            }
            break;
        default:
            reverseWriteArgs.push_back(element);
        }
    }

    std::vector<ProgramElement>::reverse_iterator itWriteArgs = reverseWriteArgs.rbegin();
    for (; itWriteArgs != reverseWriteArgs.rend(); ++itWriteArgs)
    {
        WriteProgramElement(*itWriteArgs);
    }
}

void LLVMCodeGenerator::WriteProgramElement(const ProgramElement& element)
{
    std::string elementValue = element.m_value;
    Token::Kind kind = element.m_kind;
    std::string typeName = GetTypeNameByKind(kind);
    
    *m_outStream << "\t"
        << CreateVariableName("", m_counters.m_elementsCounter)
        << " = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([" 
        << ((kind == Token::Token_double) ? "4" : "3") << " x i8]* "
        << GetFormatStringNameByType(kind) << ".write"
        << ", i32 0, i32 0), "
        << typeName << " " << elementValue 
        << ") nounwind"
        << std::endl;

    ++m_counters.m_elementsCounter;
}

std::string LLVMCodeGenerator::LoadVariable(std::string variableName)
{
    std::string result = CreateVariableName(variableName, m_identifiersTable[variableName].m_counter);
    ++m_identifiersTable[variableName].m_counter;

    *m_outStream << "\t"
        << result
        << " = load "
        << GetTypeNameByKind(m_identifiersTable[variableName].m_kind) << "*"
        << " "
        << "@" << variableName            
        << std::endl;

    return result;
}

std::string LLVMCodeGenerator::LoadFromArray(std::string arrayName, std::string index)
{
    //   %1 = load i32* getelementptr inbounds ([20 x i32]* @arr, i32 0, i32 2), align 4 ; <i32> [#uses=1]

    // %4 = getelementptr inbounds [15 x i32]* @arr, i32 0, i32 %3 ; <i32*> [#uses=1]
    // %5 = load i32* %4, align 4                      ; <i32> [#uses=1]

    std::string typeName = GetTypeNameByKind(m_identifiersTable[arrayName].m_kind);
    std::string resultTmpVariable = CreateVariableName(arrayName, m_identifiersTable[arrayName].m_counter);
    ++m_identifiersTable[arrayName].m_counter;

    if (index[0] != '%')
    // index - number
    {
        *m_outStream << "\t"
            << resultTmpVariable
            << " = load " << typeName << "* "
            << "getelementptr inbounds (["
            << m_identifiersTable[arrayName].m_arraySize
            << " x " << typeName << "]*"
            << " @" << arrayName
            << ", " << GetTypeNameByKind(Token::Token_int) << " 0, "
            << GetTypeNameByKind(Token::Token_int) << " " << index << ")"
            << std::endl;
    }
    else
    // index - temp variable
    {
        *m_outStream << "\t"
            << resultTmpVariable
            << " = " << "getelementptr inbounds ["
            << m_identifiersTable[arrayName].m_arraySize
            << " x " << typeName << "]*"
            << " @" << arrayName
            << ", " << GetTypeNameByKind(Token::Token_int) << " 0, "
            << GetTypeNameByKind(Token::Token_int) << " " << index
            << std::endl;

        std::string arrayElementVariable = resultTmpVariable;
        resultTmpVariable = CreateVariableName(arrayName, m_identifiersTable[arrayName].m_counter);
        ++m_identifiersTable[arrayName].m_counter;

        *m_outStream << "\t"
            << resultTmpVariable
            << " = load " << typeName << "* " << arrayElementVariable
            << std::endl;
    }
    

    return resultTmpVariable;
}

std::vector<ProgramElement> LLVMCodeGenerator::LoadArrayToVariables(std::string arrayName)
{
    assert(m_identifiersTable.find(arrayName) != m_identifiersTable.end());
    
    std::vector<ProgramElement> elements;
    int countElements = atoi(m_identifiersTable[arrayName].m_arraySize.c_str());
    ProgramElement arrayElement("", m_identifiersTable[arrayName].m_kind, 0);
    for (int i = 0; i < countElements; ++i)
    {
        arrayElement.m_value = LoadFromArray(arrayName, NumberToString<int>(i));
        elements.push_back(arrayElement);
    }

    return elements;
}

void LLVMCodeGenerator::StoreVariable(std::string variableName, std::string variableValue)
{
    *m_outStream << "\t"
        << "store "
        << GetTypeNameByKind(m_identifiersTable[variableName].m_kind)
        << " "
        << variableValue << ","
        << " "
        << GetTypeNameByKind(m_identifiersTable[variableName].m_kind) << "* "
        << "@" << variableName
        << std::endl;
}

void LLVMCodeGenerator::StoreValueInArray(std::string arrayName, std::string index, std::string variableValue)
{
    //  %6 = getelementptr inbounds [15 x i32]* @arr, i32 0, i32 %1 ; <i32*> [#uses=1]
    //	store i32 %5, i32* %6, align 4

    std::string typeName = GetTypeNameByKind(m_identifiersTable[arrayName].m_kind);
    if (index[0] != '%')
    // index - number
    {
        *m_outStream << "\t"
            << "store "	<< typeName	<< " "
            << variableValue << ", "
            << typeName << "* "	<< "getelementptr inbounds ([" 
            << m_identifiersTable[arrayName].m_arraySize
            << " x " << typeName << "]*"
            << " @" << arrayName
            << ", " << GetTypeNameByKind(Token::Token_int) << " 0,"
            << " " << GetTypeNameByKind(Token::Token_int) << " " << index << ")"
            << std::endl;
    }
    else
    // index - temp variable
    {
        std::string arrayElementVariable = CreateVariableName(arrayName, m_identifiersTable[arrayName].m_counter);
        ++m_identifiersTable[arrayName].m_counter;

        *m_outStream << "\t"
            << arrayElementVariable	<< " = "
            << "getelementptr inbounds ["
            << m_identifiersTable[arrayName].m_arraySize
            << " x " << typeName << "]*"
            << " @" << arrayName
            << ", " << GetTypeNameByKind(Token::Token_int) << " 0, "
            << GetTypeNameByKind(Token::Token_int) << " " << index
            << std::endl;		

        *m_outStream << "\t"
            << "store " << typeName << " " << variableValue
            << ", " << typeName << "* " << arrayElementVariable
            << std::endl;
    }
    
}

LLVMCodeGenerator::LLVMCodeGenerator()
{
    m_errosObserver.reset(new CErrorsObserver(*this));    
}

bool LLVMCodeGenerator::Generate(std::ostream& outStream, AstNode& program)
{
    ClearErrors();

    m_counters = Counters();
    m_outStream = &outStream;
    m_rootProgramNode = &program;
    program.Accept(*this); 
    return !this->HasErrors();
}

ProgramElement LLVMCodeGenerator::ExecuteAssignmentOperation()
{
    ProgramElement result;

    if (m_programStack.size() >= 2)
    {
        ProgramElement operand = PopAndLoadProgramElement();
        if (operand.m_value == "")
        {
            return result;
        }
        if (operand.m_kind == Token::Token_Id)
        {
            ErrorReport(CError::Error_VariableToArray, operand.m_beginOffset, operand.m_value);
            return result;
        }

        ProgramElement identifier = m_programStack.top();
        m_programStack.pop();     

        if (!CheckVariable(identifier))
        {
            return result;
        }
        
        // Check variable-array comparability
        if (!identifier.IsArray() && m_identifiersTable[identifier.m_value].IsArray())
        {
            ErrorReport(CError::Error_VariableToArray, identifier.m_beginOffset, identifier.m_value);
            return result;
        }

        // Store value
        if (operand.m_kind == m_identifiersTable[identifier.m_value].m_kind)            
        {   
            if (identifier.IsArray())
            {
                StoreValueInArray(identifier.m_value, identifier.m_index, operand.m_value);				
            }
            else
            {
                StoreVariable(identifier.m_value, operand.m_value);

                // Assignment "real value"
                m_identifiersTable[identifier.m_value].m_value = operand.m_value;
            }

            result.m_kind = operand.m_kind;
            result.m_value = identifier.m_value;
            result.m_beginOffset = identifier.m_beginOffset;
        }
        else
        {
            ErrorReport(CError::Error_TypesMismatch, identifier.m_beginOffset,
                CreateTypeErrorMessage(operand.m_kind, m_identifiersTable[identifier.m_value].m_kind));
        }        
    }
    else
    {
        assert(false);        
    }

    return result;
}

ProgramElement LLVMCodeGenerator::PopAndLoadProgramElement()
{
    if (m_programStack.size() == 0)
    {
        assert(false);
    }

    ProgramElement result = m_programStack.top();
    m_programStack.pop();

    ProgramElement unaryOperator;
    if (result.m_isUnaryOperator)
    {
        unaryOperator = result;
        result = m_programStack.top();
        m_programStack.pop();
    }

    ProgramElement firstTopElement = result;
    
    if(IsBinaryArithmeticOperation(firstTopElement))
    {
        result = ExecuteBinaryOperation(firstTopElement);        
    }
    else if (firstTopElement.m_kind == Token::Token_Id)       
    {
        if (!CheckVariable(result))
        {
            return result;
        }

        bool isArrayWithoutIndex = (!firstTopElement.IsArray() 
            && m_identifiersTable[firstTopElement.m_value].IsArray());

        if (!isArrayWithoutIndex)
        {
            result.m_kind = m_identifiersTable[result.m_value].m_kind;
            result.m_value = (result.IsArray()) ? 
                LoadFromArray(result.m_value, result.m_index)
                : LoadVariable(result.m_value);
            result.m_index.clear();
        }
        else
        // Return array identifier
        {
            return result;
        }
    }

    if ((unaryOperator.m_isUnaryOperator) && (unaryOperator.m_value == "-"))
    {
        if ((firstTopElement.m_kind == Token::Token_int) ||
            (firstTopElement.m_kind == Token::Token_double))
        {
            result.m_value.insert(0, "-");
        }
        else 
        // result * -1
        {
            std::string minusValueByType = (result.m_kind == Token::Token_double) ? 
                "-1.0" : "-1";
            std::string tmpVariable = CreateVariableName("", m_counters.m_elementsCounter);
            *m_outStream << "\t"
                << tmpVariable
                << " = "
                << GetOperationName(ProgramElement("*", Token::Token_Mult, result.m_beginOffset), result.m_kind)
                << " "
                << GetTypeNameByKind(result.m_kind)
                << " " << minusValueByType
                << ", " << result.m_value
                << std::endl;

            ++m_counters.m_elementsCounter;

            result.m_value = tmpVariable;            
        }
    }

    return result;
}

ProgramElement LLVMCodeGenerator::ExecuteBinaryOperation(const ProgramElement& operation)
{
    ProgramElement result;

    if (m_programStack.size() >= 2)
    {
        ProgramElement operand1 = PopAndLoadProgramElement();        
        ProgramElement operand2 = PopAndLoadProgramElement();

        if ((operand1.m_value == "") || (operand2.m_value == ""))
        {
            // Error: Execution error
            return result;
        }

        if (operand1.m_kind == operand2.m_kind)
        {            
            std::string tmpVariable = CreateVariableName("", m_counters.m_elementsCounter);
            *m_outStream << "\t"
                << tmpVariable
                << " = "
                << GetOperationName(operation, operand1.m_kind)
                << " "
                << GetTypeNameByKind(operand1.m_kind)
                << " "
                << operand2.m_value
                << ", "
                << operand1.m_value
                << "" << std::endl;

            ++m_counters.m_elementsCounter;

            result.m_kind = operand1.m_kind;
            result.m_value = tmpVariable;            
            result.m_beginOffset = operand1.m_beginOffset;
        }
        else
        {
            ErrorReport(CError::Error_TypesMismatch,
                operand1.m_beginOffset,
                CreateTypeErrorMessage(operand1.m_kind, operand2.m_kind));
        }        
    }
    else
    {
        assert(false);
    }

    return result;
}

void LLVMCodeGenerator::ErrorReport(CError::ErrorType type, size_t beginOffset, const std::string& extendedInfo)
{
    AddError(IErrorsContainer::IErrorPtr(
        new CError(type, SourcePosition(m_rootProgramNode->m_sourceFile, beginOffset), extendedInfo)));
}

bool LLVMCodeGenerator::CheckVariable(ProgramElement identifier)
{
    // Check identifier for existing
    bool isDefinedVariable = (m_identifiersTable.find(identifier.m_value) != m_identifiersTable.end());
    if (!isDefinedVariable)
    {
        ErrorReport(CError::Error_UndefinedIdentifier, identifier.m_beginOffset, identifier.m_value);
        return false;
    }   

    // Check array-variable comparability
    if (identifier.IsArray() && !m_identifiersTable[identifier.m_value].IsArray())
    {
        ErrorReport(CError::Error_NotArray, identifier.m_beginOffset, identifier.m_value);
        return false;
    }
        
    return true;
}

std::string LLVMCodeGenerator::CreateTypeErrorMessage(Token::Kind left, Token::Kind right)
{
    std::string typeErrorInfo("cannot convert from ");
    typeErrorInfo += Token::TokenToString(left);
    typeErrorInfo += " to ";
    typeErrorInfo += Token::TokenToString(right);

    return (typeErrorInfo);
}

void LLVMCodeGenerator::Accept(AstNode *node)
{
    if (!this->HasErrors())
    {
        AstNode::AcceptChild(node, *this);
    }
}

bool LLVMCodeGenerator::Visit(AstProgram *node) 
{    
    Accept(node->m_globalDecls);
    Accept(node->m_mainFunction);
    return (false);
}

bool LLVMCodeGenerator::Visit(AstMain *node) 
{
    SerializeReadConstantFormatStrings(*m_outStream);
    SerializeWriteConstantFormatStrings(*m_outStream);
    *m_outStream << std::endl;

    SerializeStartMainFunction(*m_outStream);    
    
    Accept(node->m_block);    
        
    m_currentEndLabel = RETURN_LABEL;
    SerializeEndMainFunction(*m_outStream, m_currentEndLabel);
    *m_outStream << std::endl;

    SerializeDeclaringOfFunction(*m_outStream, "printf", 
        GetTypeNameByKind(Token::Token_int), "i8*, ...");
    SerializeDeclaringOfFunction(*m_outStream, "scanf", 
        GetTypeNameByKind(Token::Token_int), "i8*, ...");
    
    return (false);
}

bool LLVMCodeGenerator::Visit(AstGlobalVarDeclarationList *node) 
{
    Accept(node->m_declaration);
    for (node = node->m_next; node != NULL; node = node->m_next)
    {
        m_isDeclaration = true;
        Accept(node->m_declaration);        
        m_isDeclaration = false;
    }    
    *m_outStream << std::endl;
    return (false);
}

bool LLVMCodeGenerator::Visit(AstDeclaration *node) 
{
    Accept(node->m_variable);
    if (!m_programStack.empty())
    {
        ProgramElement element = m_programStack.top();
        m_programStack.pop();

        bool isElementExists = 
            (m_identifiersTable.find(element.m_value) != m_identifiersTable.end());
        if (isElementExists)
        {
            ErrorReport(CError::Error_Redefinition, element.m_beginOffset, element.m_value);
        }

        Token::Kind elementKind = (node->m_type == Token::Token_float) ? 
            Token::Token_double 
            : node->m_type;

        bool isArrayElement = !element.m_index.empty();
        if (isArrayElement)
        {
            if (atoi(element.m_index.c_str()) <= 0)
            {
                ErrorReport(CError::Error_UnknownSizeOfArray, element.m_beginOffset, element.m_value);
            }
            else
            {
                SerializeGlobalDeclarationArray(*m_outStream, 
                    element.m_value, element.m_index, elementKind);
                m_identifiersTable[element.m_value].m_arraySize = element.m_index;
            }
        }
        else
        {
            SerializeGlobalDeclarationIdentifier(*m_outStream, element.m_value, elementKind);
        }
        m_identifiersTable[element.m_value].m_kind = elementKind;
    }    

    return (false);
}

bool LLVMCodeGenerator::Visit(AstBlockStatement *node)
{
    Accept(node->m_statements);
    return (false);
}

bool LLVMCodeGenerator::Visit(AstStatementList *node)
{
    Accept(node->m_statement);
    for (node = node->m_next; node != NULL; node = node->m_next)
    {
        Accept(node->m_statement);
    }
    return (false);
}

bool LLVMCodeGenerator::Visit(AstIdentifierExpr *expr) 
{
    m_programStack.push(ProgramElement(expr->GetName(), Token::Token_Id, expr->GetStartOffset()));

    return (false);
}

bool LLVMCodeGenerator::Visit(AstArrayExpr *node) 
{
    Accept(node->m_expression);  

    if (node->m_nextArray)
    {
        // TODO: support two dimension in arrays
        ProgramElement firstIndex = m_programStack.top();
        m_programStack.pop();
        ErrorReport(CError::Error_TwoDimensionArrayNotSupported, node->m_beginOffset);
        m_programStack.push(firstIndex);

        return (false);

        //Accept(node->m_nextArray);
    }
    
    // Create array element from stack
    ProgramElement index = PopAndLoadProgramElement();
    
    if (m_isDeclaration)
    {
        if (index.m_value[0] == '%')
        {
            ErrorReport(CError::Error_SizeOfArrayNotNumber, node->m_beginOffset, m_programStack.top().m_value);
            return (false);
        }
    }
    
    if (index.m_kind != Token::Token_int)
    {
        ErrorReport(CError::Error_WrongArrayIndexType, index.m_beginOffset, m_programStack.top().m_value);
        return (false);
    }

    ProgramElement identifier = m_programStack.top();
    m_programStack.pop();	
    identifier.m_index = index.m_value;

    // Add array element to program stack
    m_programStack.push(identifier);

    return (false);
}

bool LLVMCodeGenerator::Visit(AstAssignmentList *node) 
{
    Accept(node->m_assignment);
    for (node = node->m_next; node != NULL; node = node->m_next)
    {
        Accept(node->m_assignment);
    }
    return (false);
}

bool LLVMCodeGenerator::Visit(AstAssignmentExpr *expr) 
{
    Accept(expr->m_variable);    
    Accept(expr->m_expression);

    if (this->GetErrorsCount() == 0)
    {
        ExecuteAssignmentOperation();
    }

    return (false);
}

bool LLVMCodeGenerator::Visit(AstVariableExpr *node) 
{
    Accept(node->m_identifier);    
    Accept(node->m_array);
    return (false);
}

bool LLVMCodeGenerator::Visit(AstTerm *node) 
{
    Accept(node->m_expression);
    return (false);
}

bool LLVMCodeGenerator::Visit(AstMultExpr *node) 
{
    Accept(node->m_unaryExpr);
    Accept(node->m_term);

    m_programStack.push(ProgramElement("*", Token::Token_Mult, node->GetStartOffset()));

    return (false);
}

bool LLVMCodeGenerator::Visit(AstDivExpr *node) 
{
    Accept(node->m_unaryExpr);
    Accept(node->m_term);

    m_programStack.push(ProgramElement("/", Token::Token_Div, node->GetStartOffset()));

    return (false);
}

bool LLVMCodeGenerator::Visit(AstSubExpr *node) 
{
    Accept(node->m_expression);
    Accept(node->m_term);

    m_programStack.push(ProgramElement("-", Token::Token_Sub, node->GetStartOffset()));

    return (false);
}

bool LLVMCodeGenerator::Visit(AstAddExpr *node) 
{
    Accept(node->m_expression);
    Accept(node->m_term);

    m_programStack.push(ProgramElement("+", Token::Token_Add, node->GetStartOffset()));

    return (false);
}

bool LLVMCodeGenerator::Visit(AstUnaryExpr *expr) 
{
    Accept(expr->m_factor);
    
    ProgramElement unaryExprElement = ProgramElement(Token::TokenToString(expr->m_unaryOperator), Token::Token_Sub, expr->GetStartOffset());
    unaryExprElement.m_isUnaryOperator = true;
    m_programStack.push(unaryExprElement);

    return (false);
}

bool LLVMCodeGenerator::Visit(AstFactorExpr *node) 
{
    Accept(node->m_expression);
    return (false);
}

bool LLVMCodeGenerator::Visit(AstNumberLiteralExpr *node) 
{        
    m_programStack.push(ProgramElement(node->GetName(), 
        DetectNumberType(node->GetName()), node->GetStartOffset()));   
    return (false);
}

bool LLVMCodeGenerator::Visit(AstCharacterConstant *node) 
{        
    std::string valueStr = NumberToString<int>((int)(node->GetName()[0]));
    m_programStack.push(ProgramElement(valueStr, Token::Token_char, node->GetStartOffset()));   
    return (false);
}

bool LLVMCodeGenerator::Visit(AstWriteStatement *node) 
{
    Accept(node->m_arguments);
    if (this->GetErrorsCount() == 0)
    {
        WriteAllStackElements();
    }
    return (false);
}

bool LLVMCodeGenerator::Visit(AstWriteArgumentList *node) 
{    
    Accept(node->m_expression);
    for (node = node->m_next; node != NULL; node = node->m_next)
    {     
        Accept(node->m_expression);
    }
    return (false);
}

bool LLVMCodeGenerator::Visit(AstReadStatement *node) 
{
    Accept(node->m_argumentList);
    ReadAllVariables();
    return (false);
}

bool LLVMCodeGenerator::Visit(AstReadArgumentList *node) 
{
    Accept(node->m_id);
    for (node = node->m_next; node != NULL; node = node->m_next)
    {
        Accept(node->m_id);
    }
    return (false);
}

bool LLVMCodeGenerator::Visit(AstIfStatement *node) 
{
    Accept(node->m_condition);
    
    ProgramElement comparison = PopAndLoadProgramElement();
    ProgramElement operand2 = PopAndLoadProgramElement();
    ProgramElement operand1 = PopAndLoadProgramElement();

    if (operand1.m_kind == operand2.m_kind)
    {
        std::string ifLabel = "IfBlock." + NumberToString<size_t>(m_counters.m_ifelseCounter);
        std::string elseLabel = "ElseBlock." + NumberToString<size_t>(m_counters.m_ifelseCounter);
        std::string endLabel = "IfElseEndBlock." + NumberToString<size_t>(m_counters.m_ifelseCounter);
        std::string conditionName = "ifelseCondition." + NumberToString<size_t>(m_counters.m_ifelseCounter);
        ++m_counters.m_ifelseCounter;
        
        *m_outStream << "\t"
            << "%" << conditionName
            << " = " << GetCmpOperationByType(operand1.m_kind) << " "
            << GetConditionNameByKind(comparison.m_kind, operand1.m_kind)
            << " " << GetTypeNameByKind(operand1.m_kind)
            << " " << operand1.m_value
            << ", " << operand2.m_value
            << std::endl;

        *m_outStream << "\t"
            << "br i1 %" << conditionName
            << ", label %" << ifLabel
            << ", label %" << ((node->m_elseBlock) ? elseLabel : endLabel)
            << std::endl;        

        *m_outStream << ifLabel << ":" << std::endl;
        Accept(node->m_thenBlock);
        *m_outStream << "\tbr label %" << endLabel << std::endl;

        if (node->m_elseBlock)
        {
            *m_outStream << elseLabel << ":" << std::endl;
            Accept(node->m_elseBlock);
            *m_outStream << "\tbr label %" << endLabel << std::endl;
        }        
        *m_outStream << endLabel << ":" << std::endl;
    }
    else
    {
        ErrorReport(CError::Error_TypesMismatch,
            node->m_beginOffset, CreateTypeErrorMessage(operand1.m_kind, operand2.m_kind));
    }
    
    return (false);
}

bool LLVMCodeGenerator::Visit(AstConditionExpression *node)
{    
    Accept(node->m_expr1);
    ProgramElement expression1 = PopAndLoadProgramElement();
    
    Accept(node->m_expr2);
    ProgramElement expression2 = PopAndLoadProgramElement();

    m_programStack.push(expression1);
    m_programStack.push(expression2);

    m_programStack.push(ProgramElement(Token::TokenToString(node->m_operation), 
        node->m_operation, node->GetStartOffset()));

    return (false);
}

bool LLVMCodeGenerator::Visit(AstWhileStatement *node) 
{
    std::string conditionLabel = "WhileCondition." + NumberToString<size_t>(m_counters.m_whileCounter);

    *m_outStream << "\tbr label %" << conditionLabel << std::endl;        
    *m_outStream << conditionLabel << ":" << std::endl;

    Accept(node->m_condition);

    ProgramElement comparison = PopAndLoadProgramElement();
    ProgramElement operand2 = PopAndLoadProgramElement();
    ProgramElement operand1 = PopAndLoadProgramElement();

    if (operand1.m_kind == operand2.m_kind)
    {
        std::string bodyLabel = "WhileBodyBlock." + NumberToString<size_t>(m_counters.m_whileCounter);        
        std::string endLabel = "WhileEndBlock." + NumberToString<size_t>(m_counters.m_whileCounter);
        m_currentEndLabel = endLabel;

        std::string conditionName = "whileCondition." + NumberToString<size_t>(m_counters.m_whileCounter);
        ++m_counters.m_whileCounter;

        *m_outStream << "\t"
            << "%" << conditionName
            << " = " << GetCmpOperationByType(operand1.m_kind) << " "
            << GetConditionNameByKind(comparison.m_kind, operand1.m_kind)
            << " " << GetTypeNameByKind(operand1.m_kind)
            << " " << operand1.m_value
            << ", " << operand2.m_value
            << std::endl;

        *m_outStream << "\t"
            << "br i1 %" << conditionName
            << ", label %" << bodyLabel
            << ", label %" << endLabel
            << std::endl;        

        *m_outStream << bodyLabel << ":" << std::endl;        
        Accept(node->m_statement);
        *m_outStream << "\tbr label %" << conditionLabel << std::endl;

        *m_outStream << endLabel << ":" << std::endl;

        m_currentEndLabel = "";        
    }
    else
    {
        ErrorReport(CError::Error_TypesMismatch,
            node->m_beginOffset, CreateTypeErrorMessage(operand1.m_kind, operand2.m_kind));
    }

    return (false);
}

bool LLVMCodeGenerator::Visit(AstBreakStatement *node) 
{
    if (m_currentEndLabel == "")
    {
        ErrorReport(CError::Error_Break, node->m_beginOffset);
    }
    else
    {
        *m_outStream << "\t"
            << "br label %"
            <<  m_currentEndLabel
            << std::endl;    
    }    

    return (false);
}

bool LLVMCodeGenerator::Visit(AstStringLiteralExpr *expr) 
{
    m_programStack.push(ProgramElement(expr->GetName(),
        Token::Token_StringConstant, expr->GetStartOffset()));

    return (false);
}

bool LLVMCodeGenerator::Visit(AstDoWhileStatement *node) 
{
    ErrorReport(CError::Error_DoWhileNoSupported, node->m_beginOffset);
    return (false);
}

bool LLVMCodeGenerator::Visit(AstForStatement *node) 
{
    ErrorReport(CError::Error_ForNotSupported, node->m_beginOffset);
    return (false);
}