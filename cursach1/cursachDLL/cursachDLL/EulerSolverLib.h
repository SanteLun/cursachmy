#pragma once

// Используем только необходимые namespace
#include <vcclr.h>

namespace cursachDLL {
    // Класс для решения систем обыкновенных дифференциальных уравнений методом Эйлера
    public ref class EulerSolverLib
    {
    private:
        // матрица A для системы
        MatrixLib^ _matrixA;
        
        // матрица B для системы
        MatrixLib^ _matrixB;
        
        // матрица (вектор) C для системы
        MatrixLib^ _matrixC;
        
        // начальное значение вектора состояния
        MatrixLib^ _initialValue;
        
        // параметры для ступенчатой функции
        double _stepT0;
        double _stepT;
        
        // параметры времени
        double _startTime;
        double _endTime;
        double _timeStep;
        
        // функция для вычисления правой части ОДУ
        MatrixLib^ CalculateYFunction(double t, MatrixLib^ y)
        {
            // проверка инициализации матриц
            if (_matrixA == nullptr || _matrixB == nullptr || _matrixC == nullptr)
            {
                throw gcnew System::InvalidOperationException("Matrix parameters are not initialized");
            }

            // вычисляем правую часть уравнения y' = A*y + B*y*u(t) + C*f(t)
            // где u(t) - ступенчатая функция со значением 1 при t >= _stepT0 и 0 при t < _stepT0
            // f(t) - ступенчатая функция со значением t при t >= _stepT и 0 при t < _stepT
            
            // получаем значение A*y
            MatrixLib^ result = _matrixA->Multiply(y);
            
            // если t >= _stepT0, добавляем B*y*u(t)
            if (t >= _stepT0)
            {
                MatrixLib^ byProduct = _matrixB->Multiply(y);
                result = result->Add(byProduct);
            }
            
            // если t >= _stepT, добавляем C*f(t)
            if (t >= _stepT)
            {
                MatrixLib^ cProduct = _matrixC->MultiplyByScalar(t);
                result = result->Add(cProduct);
            }
            
            return result;
        }
        
    public:
        // конструктор без параметров
        EulerSolverLib()
        {
            _matrixA = nullptr;
            _matrixB = nullptr;
            _matrixC = nullptr;
            _initialValue = nullptr;
            _stepT0 = 1.0;
            _stepT = 1.0;
            _startTime = 0.0;
            _endTime = 10.0;
            _timeStep = 0.1;
        }
        
        // конструктор с параметрами
        EulerSolverLib(MatrixLib^ A, MatrixLib^ B, MatrixLib^ C)
        {
            _matrixA = A;
            _matrixB = B;
            _matrixC = C;
            _initialValue = nullptr;
            _stepT0 = 1.0;
            _stepT = 1.0;
            _startTime = 0.0;
            _endTime = 10.0;
            _timeStep = 0.1;
        }
        
        // установка параметров ступенчатой функции
        void SetStepFunction(double t0, double t)
        {
            _stepT0 = t0;
            _stepT = t;
        }
        
        // установка параметров времени
        void SetTimeParameters(double startTime, double endTime, double timeStep)
        {
            _startTime = startTime;
            _endTime = endTime;
            _timeStep = timeStep;
        }
        
        // получение матрицы A
        MatrixLib^ GetMatrixA()
        {
            return _matrixA;
        }
        
        // получение матрицы B
        MatrixLib^ GetMatrixB()
        {
            return _matrixB;
        }
        
        // получение матрицы C
        MatrixLib^ GetMatrixC()
        {
            return _matrixC;
        }
        
        // установка новой матрицы A
        void SetMatrixA(MatrixLib^ A)
        {
            _matrixA = A;
        }
        
        // установка новой матрицы B
        void SetMatrixB(MatrixLib^ B)
        {
            _matrixB = B;
        }
        
        // установка новой матрицы C
        void SetMatrixC(MatrixLib^ C)
        {
            _matrixC = C;
        }
        
        // установка начального значения
        void SetInitialValue(MatrixLib^ initialValue)
        {
            _initialValue = initialValue;
        }
        
        // решение системы ОДУ методом Эйлера и возврат результата в виде строки
        System::String^ Solve()
        {
            // проверка инициализации параметров
            if (_matrixA == nullptr || _matrixB == nullptr || _matrixC == nullptr || _initialValue == nullptr)
            {
                return "Error: Parameters are not initialized";
            }

            System::Text::StringBuilder^ result = gcnew System::Text::StringBuilder();
            result->AppendLine("Solving ODE using Euler method:");
            result->AppendLine("--------------------------------");

            // текущее время и значение
            double t = _startTime;
            MatrixLib^ y = _initialValue->Clone();

            // добавляем начальное значение в результат
            result->AppendLine(System::String::Format("t = {0}, y = {1}", t, y->ToString()));

            // итеративно применяем метод Эйлера
            while (t < _endTime)
            {
                // вычисляем правую часть уравнения
                MatrixLib^ yPrime = CalculateYFunction(t, y);
                
                // применяем формулу метода Эйлера: y_{n+1} = y_n + h*f(t_n, y_n)
                MatrixLib^ increment = yPrime->MultiplyByScalar(_timeStep);
                y = y->Add(increment);
                
                // увеличиваем время
                t += _timeStep;
                
                // добавляем текущее значение в результат
                result->AppendLine(System::String::Format("t = {0}, y = {1}", t, y->ToString()));
            }

            return result->ToString();
        }
    };
} 