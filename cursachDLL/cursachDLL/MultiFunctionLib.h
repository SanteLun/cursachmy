#pragma once

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Text::RegularExpressions;

namespace cursachDLL {
    
    // класс для хранения нескольких возвращаемых значений
    public ref class MultiResult
    {
    private:
        // внутренний список для хранения результатов
        List<Object^>^ _values;
        
    public:
        // конструктор
        MultiResult()
        {
            _values = gcnew List<Object^>();
        }
        
        // добавление значения
        void Add(Object^ value)
        {
            _values->Add(value);
        }
        
        // получение значения по индексу
        Object^ Get(int index)
        {
            if (index >= 0 && index < _values->Count)
            {
                return _values[index];
            }
            
            throw gcnew IndexOutOfRangeException("Индекс выходит за границы результатов");
        }
        
        // получение количества результатов
        int Count()
        {
            return _values->Count;
        }
        
        // получение всех значений
        array<Object^>^ GetAll()
        {
            return _values->ToArray();
        }
        
        // переопределение ToString для удобного отображения
        virtual String^ ToString() override
        {
            String^ result = "[";
            
            for (int i = 0; i < _values->Count; i++)
            {
                result += _values[i] != nullptr ? _values[i]->ToString() : "null";
                
                if (i < _values->Count - 1)
                {
                    result += ", ";
                }
            }
            
            result += "]";
            return result;
        }
    };
    
    // класс для реализации языка с функциями, возвращающими несколько результатов
    public ref class MultiFunctionLib
    {
    private:
        // словарь для хранения функций (имя -> тело функции)
        Dictionary<String^, String^>^ _functions;
        
    public:
        // конструктор
        MultiFunctionLib()
        {
            _functions = gcnew Dictionary<String^, String^>();
            
            // регистрируем несколько встроенных функций
            RegisterFunction("meshgrid", "[X, Y]=meshgrid(-1:.5:1, -1:.5:1)");
            RegisterFunction("min_max", "[min, max]=min_max(values)");
            RegisterFunction("solve_equation", "[x1, x2]=solve_equation(a, b, c)");
        }
        
        // регистрация новой функции
        bool RegisterFunction(String^ name, String^ body)
        {
            if (_functions->ContainsKey(name))
            {
                return false; // функция с таким именем уже существует
            }
            
            _functions->Add(name, body);
            return true;
        }
        
        // удаление функции
        bool RemoveFunction(String^ name)
        {
            return _functions->Remove(name);
        }
        
        // проверка существования функции
        bool FunctionExists(String^ name)
        {
            return _functions->ContainsKey(name);
        }
        
        // получение тела функции
        String^ GetFunctionBody(String^ name)
        {
            if (_functions->ContainsKey(name))
            {
                return _functions[name];
            }
            
            return nullptr;
        }
        
        // возвращает имена всех зарегистрированных функций
        array<String^>^ GetRegisteredFunctions()
        {
            array<String^>^ keys = gcnew array<String^>(_functions->Count);
            _functions->Keys->CopyTo(keys, 0);
            return keys;
        }
        
        // разбор вызова функции, возвращающей несколько результатов
        // формат: meshgrid [X,Y]=meshgrid(-1:.5:1,-1:.5:1);
        MultiResult^ ParseFunctionCall(String^ functionCall)
        {
            MultiResult^ result = gcnew MultiResult();
            
            // проверка формата вызова meshgrid[X,Y]=meshgrid(-1:.5:1, -1:.5:1)
            Regex^ functionPattern = gcnew Regex("([a-zA-Z_][a-zA-Z0-9_]*)\\s*\\[([^\\]]+)\\]\\s*=\\s*([a-zA-Z_][a-zA-Z0-9_]*)\\s*\\(([^\\)]*)\\)");
            Match^ match = functionPattern->Match(functionCall);
            
            if (match->Success)
            {
                String^ functionName = match->Groups[3]->Value->Trim();
                String^ returnVars = match->Groups[2]->Value->Trim();
                String^ parameters = match->Groups[4]->Value->Trim();
                
                // проверяем, существует ли функция
                if (FunctionExists(functionName))
                {
                    // разбиваем переменные результатов
                    array<String^>^ returnVarArray = returnVars->Split(',');
                    
                    // добавляем имя функции
                    result->Add(functionName);
                    
                    // добавляем переменные результатов
                    for each (String^ var in returnVarArray)
                    {
                        result->Add(var->Trim());
                    }
                    
                    // добавляем параметры
                    result->Add(parameters);
                    
                    // добавляем информацию об успешности
                    result->Add(true);
                    
                    return result;
                }
                else
                {
                    // функция не найдена
                    result->Add("Ошибка: функция не найдена");
                    result->Add(false);
                }
            }
            else
            {
                // неверный формат вызова
                result->Add("Ошибка: неверный формат вызова функции");
                result->Add(false);
            }
            
            return result;
        }
        
        // моделирование выполнения функции meshgrid
        MultiResult^ ExecuteMeshgrid(String^ params)
        {
            MultiResult^ result = gcnew MultiResult();
            
            try
            {
                // разбор параметров вида -1:.5:1, -1:.5:1
                array<String^>^ paramArray = params->Split(',');
                
                if (paramArray->Length >= 2)
                {
                    // имитируем создание сетки (упрощенно)
                    array<double, 2>^ X = gcnew array<double, 2>(3, 3);
                    array<double, 2>^ Y = gcnew array<double, 2>(3, 3);
                    
                    // заполняем X и Y
                    for (int i = 0; i < 3; i++)
                    {
                        for (int j = 0; j < 3; j++)
                        {
                            X[i, j] = -1.0 + j * 0.5; // от -1 до 1 с шагом 0.5
                            Y[i, j] = -1.0 + i * 0.5; // от -1 до 1 с шагом 0.5
                        }
                    }
                    
                    // добавляем результаты
                    result->Add(X);
                    result->Add(Y);
                    result->Add(true);
                }
                else
                {
                    result->Add("Ошибка: недостаточно параметров для meshgrid");
                    result->Add(false);
                }
            }
            catch (Exception^ ex)
            {
                result->Add("Ошибка: " + ex->Message);
                result->Add(false);
            }
            
            return result;
        }
        
        // моделирование выполнения функции min_max
        MultiResult^ ExecuteMinMax(String^ params)
        {
            MultiResult^ result = gcnew MultiResult();
            
            try
            {
                // разбор параметров - массив чисел
                array<String^>^ values = params->Split(',');
                
                if (values->Length > 0)
                {
                    double min = Double::MaxValue;
                    double max = Double::MinValue;
                    
                    for each (String^ value in values)
                    {
                        double num = Double::Parse(value->Trim());
                        
                        if (num < min) min = num;
                        if (num > max) max = num;
                    }
                    
                    // добавляем результаты
                    result->Add(min);
                    result->Add(max);
                    result->Add(true);
                }
                else
                {
                    result->Add("Ошибка: пустой массив для min_max");
                    result->Add(false);
                }
            }
            catch (Exception^ ex)
            {
                result->Add("Ошибка: " + ex->Message);
                result->Add(false);
            }
            
            return result;
        }
        
        // моделирование выполнения решения квадратного уравнения
        MultiResult^ ExecuteSolveEquation(String^ params)
        {
            MultiResult^ result = gcnew MultiResult();
            
            try
            {
                // разбор параметров a, b, c для уравнения ax^2 + bx + c = 0
                array<String^>^ values = params->Split(',');
                
                if (values->Length >= 3)
                {
                    double a = Double::Parse(values[0]->Trim());
                    double b = Double::Parse(values[1]->Trim());
                    double c = Double::Parse(values[2]->Trim());
                    
                    // вычисляем дискриминант
                    double discriminant = b * b - 4 * a * c;
                    
                    if (discriminant >= 0)
                    {
                        double x1 = (-b + Math::Sqrt(discriminant)) / (2 * a);
                        double x2 = (-b - Math::Sqrt(discriminant)) / (2 * a);
                        
                        // добавляем результаты
                        result->Add(x1);
                        result->Add(x2);
                        result->Add(true);
                    }
                    else
                    {
                        result->Add("Ошибка: отрицательный дискриминант");
                        result->Add(false);
                    }
                }
                else
                {
                    result->Add("Ошибка: недостаточно параметров для решения уравнения");
                    result->Add(false);
                }
            }
            catch (Exception^ ex)
            {
                result->Add("Ошибка: " + ex->Message);
                result->Add(false);
            }
            
            return result;
        }
        
        // выполнение функции по имени
        MultiResult^ ExecuteFunction(String^ name, String^ parameters)
        {
            if (name->Equals("meshgrid", StringComparison::OrdinalIgnoreCase))
            {
                return ExecuteMeshgrid(parameters);
            }
            else if (name->Equals("min_max", StringComparison::OrdinalIgnoreCase))
            {
                return ExecuteMinMax(parameters);
            }
            else if (name->Equals("solve_equation", StringComparison::OrdinalIgnoreCase))
            {
                return ExecuteSolveEquation(parameters);
            }
            
            // неизвестная функция
            MultiResult^ result = gcnew MultiResult();
            result->Add("Ошибка: неизвестная функция " + name);
            result->Add(false);
            return result;
        }
    };
} 