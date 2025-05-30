#pragma once

using namespace System;
using namespace System::Collections::Generic;

namespace cursach {
    
    // типы токенов
    enum class TokenType 
    {
        Keyword,        // ключевое слово
        Identifier,     // переменная
        Operator,       // оператор
        Number,         // число
        String,         // строка
        Constant,       // константа
        EndOfStatement, // конец оператора
        EndOfFile       // конец файла
    };
    
    // токен
    ref class Token 
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
            return String::Format("{0}: {1}", Type.ToString(), Value);
        }
    };
    
    // лексический анализатор
    ref class LexicalAnalyzer 
    {
    private:
        String^ _sourceCode;  // код
        int _position;        // позиция
        HashSet<String^>^ _keywords;  // ключевые слова
        HashSet<String^>^ _operators; // операторы
        
        // инициализация списков
        void Initialize() 
        {
            _keywords = gcnew HashSet<String^>();
            _keywords->Add("if");
            _keywords->Add("end");
            
            _operators = gcnew HashSet<String^>();
            _operators->Add("+");
            _operators->Add("-");
            _operators->Add("*");
            _operators->Add("/");
            _operators->Add("=");
            _operators->Add(":");
            
            // логические операторы
            _operators->Add("<");
            _operators->Add(">");
            _operators->Add("==");
            _operators->Add("!=");
            _operators->Add("<=");
            _operators->Add(">=");
        }
        
        // пропуск пробелов и комментов
        void SkipWhiteSpaceAndComments() 
        {
            while (_position < _sourceCode->Length) 
            {
                if (Char::IsWhiteSpace(_sourceCode[_position])) 
                {
                    _position++;
                    continue;
                }
                
                // однострочный комментарий
                if (_position + 1 < _sourceCode->Length &&
                    _sourceCode[_position] == '/' &&
                    _sourceCode[_position + 1] == '/') 
                {
                    while (_position < _sourceCode->Length && 
                         _sourceCode[_position] != '\n') 
                    {
                        _position++;
                    }
                    continue;
                }
                
                break;
            }
        }
        
        Token^ ReadNumber() 
        {
            String^ number = "";
            bool dotSeen = false;
            while (_position < _sourceCode->Length && 
                  (Char::IsDigit(_sourceCode[_position]) ||
                   (_sourceCode[_position] == '.' && !dotSeen)))
            {
                if (_sourceCode[_position] == '.') dotSeen = true;
                number += _sourceCode[_position];
                _position++;
            }
            return gcnew Token(TokenType::Number, number);
        }
        
        Token^ ReadIdentifierOrKeyword() 
        {
            String^ id = "";
            
            while (_position < _sourceCode->Length && 
                 (Char::IsLetterOrDigit(_sourceCode[_position]) || 
                  _sourceCode[_position] == '_')) 
            {
                id += _sourceCode[_position];
                _position++;
            }
            
            // проверка на ключевое слово
            if (_keywords->Contains(id)) 
                return gcnew Token(TokenType::Keyword, id);
            
            return gcnew Token(TokenType::Identifier, id);
        }
        
        Token^ ReadOperator() 
        {
            // составные операторы
            if (_position + 1 < _sourceCode->Length)
            {
                char current = _sourceCode[_position];
                char next = _sourceCode[_position + 1];
                
                if (current == '=' && next == '=')
                {
                    _position += 2;
                    return gcnew Token(TokenType::Operator, "==");
                }
                else if (current == '!' && next == '=')
                {
                    _position += 2;
                    return gcnew Token(TokenType::Operator, "!=");
                }
                else if (current == '<' && next == '=')
                {
                    _position += 2;
                    return gcnew Token(TokenType::Operator, "<=");
                }
                else if (current == '>' && next == '=')
                {
                    _position += 2;
                    return gcnew Token(TokenType::Operator, ">=");
                }
            }
            
            // одиночные операторы
            String^ op = _sourceCode[_position].ToString();
            _position++;
            
            return gcnew Token(TokenType::Operator, op);
        }
        
    public:
        LexicalAnalyzer(String^ sourceCode) 
        {
            _sourceCode = sourceCode;
            _position = 0;
            Initialize();
        }
        
        Token^ GetNextToken() 
        {
            SkipWhiteSpaceAndComments();
            
            if (_position >= _sourceCode->Length) 
                return gcnew Token(TokenType::EndOfFile, "EOF");
            
            char currentChar = _sourceCode[_position];
            
            if (Char::IsDigit(currentChar)) 
                return ReadNumber();
            
            if (Char::IsLetter(currentChar) || currentChar == '_') 
                return ReadIdentifierOrKeyword();
            
            if (currentChar == ';') 
            {
                _position++;
                return gcnew Token(TokenType::EndOfStatement, ";");
            }
            
            if (IsOperatorChar(currentChar)) 
                return ReadOperator();
            
            // неизвестный символ
            _position++;
            return gcnew Token(TokenType::Operator, currentChar.ToString());
        }
        
        bool IsOperatorChar(char c) 
        {
            return c == '+' || c == '-' || c == '*' || c == '/' || 
                   c == '=' || c == '<' || c == '>' || c == '!';
        }
        
        // Метод для разбора вызова функции в формате name(params)
        List<String^>^ ParseFunctionCall()
        {
            List<String^>^ result = gcnew List<String^>();
            
            // сбрасываем позицию в начало
            _position = 0;
            SkipWhiteSpaceAndComments();
            
            // ищем имя функции
            String^ functionName = "";
            while (_position < _sourceCode->Length && 
                 (Char::IsLetterOrDigit(_sourceCode[_position]) || 
                  _sourceCode[_position] == '_')) 
            {
                functionName += _sourceCode[_position];
                _position++;
            }
            
            if (String::IsNullOrEmpty(functionName)) {
                result->Add("Не найдено имя функции");
                return result;
            }
            
            result->Add(functionName);
            
            // пропускаем пробелы
            SkipWhiteSpaceAndComments();
            
            // проверяем на открывающую скобку
            if (_position >= _sourceCode->Length || _sourceCode[_position] != '(') {
                result->Add("Отсутствует открывающая скобка после имени функции");
                return result;
            }
            
            _position++; // пропускаем '('
            
            // считываем параметры
            String^ parameters = "";
            int bracketLevel = 1;
            
            while (_position < _sourceCode->Length && bracketLevel > 0) {
                char c = _sourceCode[_position];
                
                if (c == '(') {
                    bracketLevel++;
                } else if (c == ')') {
                    bracketLevel--;
                    
                    if (bracketLevel == 0) {
                        break; // нашли закрывающую скобку
                    }
                }
                
                parameters += c;
                _position++;
            }
            
            if (bracketLevel > 0) {
                result->Add("Отсутствует закрывающая скобка");
                return result;
            }
            
            _position++; // пропускаем ')'
            result->Add(parameters);
            
            return result;
        }
    };
} 