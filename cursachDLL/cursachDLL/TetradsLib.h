#pragma once

using namespace System;
using namespace System::Collections::Generic;

namespace cursachDLL {
    
    // Класс для представления тетрады (4 поля кода)
    public ref class Tetrad
    {
    public:
        // Оператор
        String^ Operator;
        
        // Первый операнд
        String^ Operand1;
        
        // Второй операнд
        String^ Operand2;
        
        // Результат
        String^ Result;
        
        // Индекс тетрады
        int Index;
        
        // Конструктор
        Tetrad(int index, String^ op, String^ op1, String^ op2, String^ result)
        {
            Index = index;
            Operator = op;
            Operand1 = op1;
            Operand2 = op2;
            Result = result;
        }
        
        // Метод для строкового представления тетрады
        virtual String^ ToString() override
        {
            return String::Format("({0}) {1} := {2} {3} {4}", 
                Index, Result, Operand1, Operator, Operand2);
        }
    };
    
    // Класс для генерации и работы с тетрадами
    public ref class TetradsLib
    {
    private:
        // Список тетрад
        List<Tetrad^>^ _tetrads;
        
        // Счетчик для временных переменных
        int _tempVarCounter;
        
        // Счетчик для меток
        int _labelCounter;
        
        // Текущий индекс тетрады
        int _currentIndex;
        
    public:
        // Конструктор
        TetradsLib()
        {
            _tetrads = gcnew List<Tetrad^>();
            _tempVarCounter = 0;
            _labelCounter = 0;
            _currentIndex = 0;
        }
        
        // Получение списка всех тетрад
        List<Tetrad^>^ GetTetrads()
        {
            return _tetrads;
        }
        
        // Создание новой временной переменной
        String^ CreateTempVar()
        {
            return String::Format("t{0}", ++_tempVarCounter);
        }
        
        // Создание новой метки
        String^ CreateLabel()
        {
            return String::Format("L{0}", ++_labelCounter);
        }
        
        // Добавление тетрады для бинарной операции
        String^ AddBinaryOperation(String^ op, String^ op1, String^ op2)
        {
            String^ result = CreateTempVar();
            Tetrad^ tetrad = gcnew Tetrad(_currentIndex++, op, op1, op2, result);
            _tetrads->Add(tetrad);
            return result;
        }
        
        // Добавление тетрады для операции присваивания
        void AddAssignment(String^ variable, String^ value)
        {
            Tetrad^ tetrad = gcnew Tetrad(_currentIndex++, ":=", value, "", variable);
            _tetrads->Add(tetrad);
        }
        
        // Добавление тетрады для операции сравнения
        String^ AddComparison(String^ op, String^ op1, String^ op2)
        {
            String^ result = CreateTempVar();
            Tetrad^ tetrad = gcnew Tetrad(_currentIndex++, op, op1, op2, result);
            _tetrads->Add(tetrad);
            return result;
        }
        
        // Добавление тетрады для условного перехода
        void AddConditionalJump(String^ condition, String^ label)
        {
            Tetrad^ tetrad = gcnew Tetrad(_currentIndex++, "JZ", condition, "", label);
            _tetrads->Add(tetrad);
        }
        
        // Добавление тетрады для безусловного перехода
        void AddJump(String^ label)
        {
            Tetrad^ tetrad = gcnew Tetrad(_currentIndex++, "JMP", "", "", label);
            _tetrads->Add(tetrad);
        }
        
        // Добавление метки
        void AddLabel(String^ label)
        {
            Tetrad^ tetrad = gcnew Tetrad(_currentIndex++, "LABEL", "", "", label);
            _tetrads->Add(tetrad);
        }
        
        // Генерация тетрад для условного оператора if
        // Возвращает метку конца блока if
        String^ GenerateIfStatement(String^ condition)
        {
            String^ endLabel = CreateLabel();
            
            // Добавляем переход к концу if, если условие ложно
            AddConditionalJump(condition, endLabel);
            
            return endLabel;
        }
        
        // Завершение блока if
        void EndIf(String^ endLabel)
        {
            // Добавляем метку конца if
            AddLabel(endLabel);
        }
        
        // Очистка всех тетрад
        void Clear()
        {
            _tetrads->Clear();
            _tempVarCounter = 0;
            _labelCounter = 0;
            _currentIndex = 0;
        }
        
        // Получение строкового представления всех тетрад
        String^ GetTextRepresentation()
        {
            String^ result = "";
            
            for each (Tetrad^ tetrad in _tetrads)
            {
                result += tetrad->ToString() + "\r\n";
            }
            
            return result;
        }
    };
} 