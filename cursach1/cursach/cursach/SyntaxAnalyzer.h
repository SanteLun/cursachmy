#pragma once

#include "LexicalAnalyzer.h"

using namespace System;
using namespace System::Collections::Generic;

namespace cursach {
    
    // узел AST
    ref class ASTNode abstract
    {
    public:
        virtual String^ ToString() override = 0;
    };
    
    // узел присваивания
    ref class AssignmentNode : public ASTNode
    {
    public:
        String^ Variable;
        String^ Value;
        
        AssignmentNode(String^ variable, String^ value)
        {
            Variable = variable;
            Value = value;
        }
        
        virtual String^ ToString() override
        {
            return String::Format("Assignment: {0} = {1}", Variable, Value);
        }
    };
    
    // узел условия
    ref class ConditionalExpressionNode : public ASTNode
    {
    public:
        String^ LeftOperand;
        String^ Operator;
        String^ RightOperand;
        
        ConditionalExpressionNode(String^ leftOperand, String^ op, String^ rightOperand)
        {
            LeftOperand = leftOperand;
            Operator = op;
            RightOperand = rightOperand;
        }
        
        virtual String^ ToString() override
        {
            return String::Format("Condition: {0} {1} {2}", LeftOperand, Operator, RightOperand);
        }
    };
    
    // узел if-блока
    ref class IfStatementNode : public ASTNode
    {
    public:
        ConditionalExpressionNode^ Condition;
        List<ASTNode^>^ Body;
        
        IfStatementNode(ConditionalExpressionNode^ condition)
        {
            Condition = condition;
            Body = gcnew List<ASTNode^>();
        }
        
        virtual String^ ToString() override
        {
            String^ result = "If " + Condition->ToString() + "\r\n";
            
            for each (ASTNode^ node in Body)
            {
                result += "  " + node->ToString() + "\r\n";
            }
            
            result += "End";
            return result;
        }
    };
    
    // узел объявления массива
    ref class ArrayDeclarationNode : public ASTNode
    {
    public:
        String^ Name;
        double Start;
        double Step;
        double End;
        ArrayDeclarationNode(String^ name, double start, double step, double end)
            : ASTNode(), Name(name), Start(start), Step(step), End(end) {}
        virtual String^ ToString() override
        {
            return String::Format("ArrayDecl: {0} from {1} to {2} step {3}", Name, Start, End, Step);
        }
    };
    
    // синтаксический анализатор
    ref class SyntaxAnalyzer
    {
    private:
        LexicalAnalyzer^ _lexer;
        Token^ _currentToken;
        String^ _intermediateCode;
        
    public:
        SyntaxAnalyzer(LexicalAnalyzer^ lexer)
        {
            _lexer = lexer;
            _currentToken = _lexer->GetNextToken();
            _intermediateCode = "";
        }
        
        // анализ программы
        List<ASTNode^>^ Parse()
        {
            List<ASTNode^>^ program = gcnew List<ASTNode^>();
            
            while (_currentToken->Type != TokenType::EndOfFile)
            {
                ASTNode^ statement = ParseStatement();
                if (statement != nullptr)
                {
                    program->Add(statement);
                }
            }
            
            return program;
        }
        
        String^ GetIntermediateCode()
        {
            return _intermediateCode;
        }
        
        void ClearIntermediateCode()
        {
            _intermediateCode = "";
        }
        
    private:
        void NextToken()
        {
            _currentToken = _lexer->GetNextToken();
        }
        
        bool Match(TokenType type)
        {
            if (_currentToken->Type == type)
            {
                NextToken();
                return true;
            }
            
            return false;
        }
        
        // парсинг оператора
        ASTNode^ ParseStatement()
        {
            // array declaration: name = start:step:end;
            if (_currentToken->Type == TokenType::Identifier)
            {
                String^ varName = _currentToken->Value;
                Token^ saveToken = _currentToken;
                NextToken();
                if (_currentToken->Type == TokenType::Operator && _currentToken->Value == "=")
                {
                    NextToken();
                    if (_currentToken->Type == TokenType::Number)
                    {
                        double start = Double::Parse(_currentToken->Value, System::Globalization::CultureInfo::InvariantCulture);
                        NextToken();
                        if (_currentToken->Type == TokenType::Operator && _currentToken->Value == ":")
                        {
                            NextToken();
                            if (_currentToken->Type == TokenType::Number)
                            {
                                double step = Double::Parse(_currentToken->Value, System::Globalization::CultureInfo::InvariantCulture);
                                NextToken();
                                if (_currentToken->Type == TokenType::Operator && _currentToken->Value == ":")
                                {
                                    NextToken();
                                    if (_currentToken->Type == TokenType::Number)
                                    {
                                        double end = Double::Parse(_currentToken->Value, System::Globalization::CultureInfo::InvariantCulture);
                                        NextToken();
                                        // expect semicolon
                                        if (_currentToken->Type == TokenType::EndOfStatement)
                                        {
                                            NextToken();
                                        }
                                        return gcnew ArrayDeclarationNode(varName, start, step, end);
                                    }
                                }
                            }
                        }
                    }
                }
                // rollback to assignment if parse failed
                _currentToken = saveToken;
            }
            // проверка на if
            if (_currentToken->Type == TokenType::Keyword && _currentToken->Value == "if")
            {
                return ParseIfStatement();
            }
            
            // проверка на присваивание
            else if (_currentToken->Type == TokenType::Identifier)
            {
                return ParseAssignment();
            }
            
            // пропуск неизвестных токенов
            NextToken();
            return nullptr;
        }
        
        // парсинг присваивания
        ASTNode^ ParseAssignment()
        {
            if (_currentToken->Type != TokenType::Identifier)
                return nullptr;
            
            String^ variableName = _currentToken->Value;
            NextToken();
            
            if (_currentToken->Type != TokenType::Operator || _currentToken->Value != "=")
                return nullptr;
            
            NextToken();
            
            String^ value = "";
            
            // чтение до конца оператора
            while (_currentToken->Type != TokenType::EndOfStatement && 
                  _currentToken->Type != TokenType::EndOfFile)
            {
                value += _currentToken->Value;
                NextToken();
            }
            
            // пропуск ;
            if (_currentToken->Type == TokenType::EndOfStatement)
                NextToken();
            
            return gcnew AssignmentNode(variableName, value);
        }
        
        // парсинг if-оператора
        ASTNode^ ParseIfStatement()
        {
            // пропуск if
            NextToken();
            
            // парсинг условия
            ConditionalExpressionNode^ condition = ParseConditionalExpression();
            
            if (condition == nullptr)
                return nullptr;
            
            IfStatementNode^ ifNode = gcnew IfStatementNode(condition);
            
            // парсинг тела
            while (_currentToken->Type != TokenType::Keyword || _currentToken->Value != "end")
            {
                if (_currentToken->Type == TokenType::EndOfFile)
                    return ifNode; // конец файла
                
                ASTNode^ statement = ParseStatement();
                
                if (statement != nullptr)
                    ifNode->Body->Add(statement);
            }
            
            // пропуск end
            if (_currentToken->Type == TokenType::Keyword && _currentToken->Value == "end")
                NextToken();
            
            return ifNode;
        }
        
        // парсинг условного выражения
        ConditionalExpressionNode^ ParseConditionalExpression()
        {
            if (_currentToken->Type != TokenType::Identifier && _currentToken->Type != TokenType::Constant)
                return nullptr;
            
            String^ leftOperand = _currentToken->Value;
            NextToken();
            
            if (_currentToken->Type != TokenType::Operator)
                return nullptr;
                
            String^ op = _currentToken->Value;
            NextToken();
            
            if (_currentToken->Type != TokenType::Identifier && _currentToken->Type != TokenType::Constant)
                return nullptr;
                
            String^ rightOperand = _currentToken->Value;
            NextToken();
            
            return gcnew ConditionalExpressionNode(leftOperand, op, rightOperand);
        }
    };
} 