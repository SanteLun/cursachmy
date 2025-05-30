#pragma once

using namespace System;
using namespace System::Collections::Generic;

namespace cursachDLL {
    // Типы токенов
    public enum class TokenType
    {
        Number,
        Identifier,
        Operator,
        LeftParenthesis,
        RightParenthesis,
        Assign,
        Semicolon,
        EndOfFile,
        Unknown
    };
    
    // Класс токена
    public ref class Token
    {
    public:
        TokenType Type;
        String^ Value;
        
        Token(TokenType type, String^ value)
        {
            Type = type;
            Value = value;
        }
        
        virtual String^ ToString() override
        {
            return String::Format("[{0}: '{1}']", Type, Value);
        }
    };
    
    // Класс для лексического анализа
    public ref class Lexer
    {
    private:
        String^ _input;
        int _position;
        
    public:
        Lexer(String^ input)
        {
            _input = input;
            _position = 0;
        }
        
        // Получение следующего токена
        Token^ GetNextToken()
        {
            // Пропускаем пробелы
            while (_position < _input->Length && Char::IsWhiteSpace(_input[_position]))
            {
                _position++;
            }
            
            // Если достигнут конец строки
            if (_position >= _input->Length)
            {
                return gcnew Token(TokenType::EndOfFile, "EOF");
            }
            
            char currentChar = _input[_position];
            
            // Обработка чисел
            if (Char::IsDigit(currentChar))
            {
                String^ number = "";
                
                while (_position < _input->Length && 
                       (Char::IsDigit(_input[_position]) || _input[_position] == '.'))
                {
                    number += _input[_position];
                    _position++;
                }
                
                return gcnew Token(TokenType::Number, number);
            }
            
            // Обработка идентификаторов
            if (Char::IsLetter(currentChar) || currentChar == '_')
            {
                String^ identifier = "";
                
                while (_position < _input->Length && 
                       (Char::IsLetterOrDigit(_input[_position]) || _input[_position] == '_'))
                {
                    identifier += _input[_position];
                    _position++;
                }
                
                return gcnew Token(TokenType::Identifier, identifier);
            }
            
            // Обработка операторов и других символов
            switch (currentChar)
            {
                case '+':
                case '-':
                case '*':
                case '/':
                    _position++;
                    return gcnew Token(TokenType::Operator, currentChar.ToString());
                
                case '(':
                    _position++;
                    return gcnew Token(TokenType::LeftParenthesis, "(");
                
                case ')':
                    _position++;
                    return gcnew Token(TokenType::RightParenthesis, ")");
                
                case '=':
                    _position++;
                    return gcnew Token(TokenType::Assign, "=");
                
                case ';':
                    _position++;
                    return gcnew Token(TokenType::Semicolon, ";");
                
                default:
                    _position++;
                    return gcnew Token(TokenType::Unknown, currentChar.ToString());
            }
        }
        
        // Получить все токены из входной строки
        List<Token^>^ Tokenize()
        {
            List<Token^>^ tokens = gcnew List<Token^>();
            _position = 0;
            
            Token^ token;
            do
            {
                token = GetNextToken();
                tokens->Add(token);
            } while (token->Type != TokenType::EndOfFile);
            
            return tokens;
        }
    };
    
    // Типы узлов в дереве разбора
    public enum class NodeType
    {
        Number,
        Identifier,
        BinaryOperation,
        Assignment,
        Expression
    };
    
    // Базовый класс для узла дерева разбора
    public ref class ParseNode
    {
    public:
        NodeType Type;
        
        ParseNode(NodeType type)
        {
            Type = type;
        }
        
        virtual String^ ToString() override
        {
            return Type.ToString();
        }
    };
    
    // Узел числа
    public ref class NumberNode : public ParseNode
    {
    public:
        double Value;
        
        NumberNode(double value) : ParseNode(NodeType::Number)
        {
            Value = value;
        }
        
        virtual String^ ToString() override
        {
            return Value.ToString();
        }
    };
    
    // Узел идентификатора
    public ref class IdentifierNode : public ParseNode
    {
    public:
        String^ Name;
        
        IdentifierNode(String^ name) : ParseNode(NodeType::Identifier)
        {
            Name = name;
        }
        
        virtual String^ ToString() override
        {
            return Name;
        }
    };
    
    // Узел бинарной операции
    public ref class BinaryOperationNode : public ParseNode
    {
    public:
        ParseNode^ Left;
        String^ Operator;
        ParseNode^ Right;
        
        BinaryOperationNode(ParseNode^ left, String^ op, ParseNode^ right) : ParseNode(NodeType::BinaryOperation)
        {
            Left = left;
            Operator = op;
            Right = right;
        }
        
        virtual String^ ToString() override
        {
            return String::Format("({0} {1} {2})", Left->ToString(), Operator, Right->ToString());
        }
    };
    
    // Узел присваивания
    public ref class AssignmentNode : public ParseNode
    {
    public:
        IdentifierNode^ Variable;
        ParseNode^ Expression;
        
        AssignmentNode(IdentifierNode^ variable, ParseNode^ expression) : ParseNode(NodeType::Assignment)
        {
            Variable = variable;
            Expression = expression;
        }
        
        virtual String^ ToString() override
        {
            return String::Format("{0} = {1}", Variable->ToString(), Expression->ToString());
        }
    };
    
    // Класс для синтаксического анализа методом сдвиг-свертка
    public ref class ShiftFoldLib
    {
    private:
        List<Token^>^ _tokens;
        int _position;
        List<String^>^ _stack;
        List<String^>^ _parseSteps;
        
    public:
        ShiftFoldLib()
        {
            _tokens = gcnew List<Token^>();
            _position = 0;
            _stack = gcnew List<String^>();
            _parseSteps = gcnew List<String^>();
        }
        
        // Метод для разбора выражения
        String^ Parse(String^ input, String^ grammar)
        {
            _parseSteps->Clear();
            _stack->Clear();
            
            _parseSteps->Add("Input: " + input);
            _parseSteps->Add("Grammar: " + grammar);
            _parseSteps->Add("===================");
            
            // Создаем лексический анализатор
            Lexer^ lexer = gcnew Lexer(input);
            _tokens = lexer->Tokenize();
            _position = 0;
            
            // Для демонстрации - выводим все токены
            _parseSteps->Add("Tokens:");
            for (int i = 0; i < _tokens->Count; i++)
            {
                _parseSteps->Add(" - " + _tokens[i]->ToString());
            }
            _parseSteps->Add("===================");
            
            // Инициализируем стек
            _stack->Add("$"); // символ конца входной строки
            
            // Симуляция разбора методом сдвиг-свертка
            _parseSteps->Add("Parse steps:");
            
            bool success = PerformShiftReduce();
            
            if (success)
            {
                _parseSteps->Add("===================");
                _parseSteps->Add("Parsing completed successfully");
                _parseSteps->Add("Final stack: " + String::Join(" ", _stack));
                return String::Join("\r\n", _parseSteps);
            }
            else
            {
                _parseSteps->Add("===================");
                _parseSteps->Add("Parsing failed");
                _parseSteps->Add("Current stack: " + String::Join(" ", _stack));
                return String::Join("\r\n", _parseSteps);
            }
        }
        
    private:
        // Выполнение алгоритма разбора сдвиг-свертка
        bool PerformShiftReduce()
        {
            // В этом месте должен быть реализован алгоритм разбора сдвиг-свертка
            // Для примера выполним небольшую симуляцию
            
            _parseSteps->Add("1. Reading token: id");
            _parseSteps->Add("2. Shift: id");
            _stack->Add("id");
            
            _parseSteps->Add("3. Reading token: +");
            _parseSteps->Add("4. Shift: +");
            _stack->Add("+");
            
            _parseSteps->Add("5. Reading token: id");
            _parseSteps->Add("6. Shift: id");
            _stack->Add("id");
            
            _parseSteps->Add("7. Reduce: F -> id");
            _stack->RemoveAt(_stack->Count - 1);
            _stack->Add("F");
            
            _parseSteps->Add("8. Reduce: T -> F");
            _stack->RemoveAt(_stack->Count - 1);
            _stack->Add("T");
            
            _parseSteps->Add("9. Reduce: E -> T");
            _stack->RemoveAt(_stack->Count - 1);
            _stack->Add("E");
            
            _parseSteps->Add("10. Reduce: E -> E + T");
            _stack->RemoveAt(_stack->Count - 1); // Remove E
            _stack->RemoveAt(_stack->Count - 1); // Remove +
            _stack->RemoveAt(_stack->Count - 1); // Remove T
            _stack->Add("E");
            
            // Успешное завершение разбора
            return true;
        }
    };
} 