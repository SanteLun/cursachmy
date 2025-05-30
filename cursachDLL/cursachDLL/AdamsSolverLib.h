#pragma once

#include "MatrixLib.h"
using namespace System;
using namespace System::Collections::Generic;

namespace cursachDLL {
    
    // делегат для функций в методе Адамса
    public delegate MatrixLib^ MatrixFunction(MatrixLib^ x, double t);
    
    // решение ОДУ методом Адамса
    public ref class AdamsSolverLib
    {
    private:
        MatrixLib^ _A;       // матрица A
        MatrixLib^ _B;       // матрица B
        MatrixLib^ _C;       // матрица C
        MatrixFunction^ _u;  // функция u(x,t)
        MatrixFunction^ _f;  // функция f(t)
        
        // вычисление правой части
        MatrixLib^ CalculateRightSide(MatrixLib^ x, double t)
        {
            MatrixLib^ ax = _A->Multiply(x);
            
            MatrixLib^ ux = _u(x, t);
            MatrixLib^ bux = _B->Multiply(ux);
            
            // f зависит только от t
            MatrixLib^ ft = _f(gcnew MatrixLib(1, 1), t);
            MatrixLib^ cft = _C->Multiply(ft);
            
            MatrixLib^ result = ax->Add(bux);
            result = result->Add(cft);
            
            return result;
        }
        
        // метод Рунге-Кутта для начальных точек
        array<MatrixLib^>^ RungeKutta4Steps(MatrixLib^ x0, double t0, double h, int steps)
        {
            array<MatrixLib^>^ result = gcnew array<MatrixLib^>(steps + 1);
            result[0] = x0;
            double t = t0;
            
            for (int i = 0; i < steps; i++)
            {
                MatrixLib^ x = result[i];
                
                // k1, k2, k3, k4
                MatrixLib^ k1 = CalculateRightSide(x, t);
                
                MatrixLib^ xTemp = x->Add(k1->MultiplyByScalar(h / 2.0));
                MatrixLib^ k2 = CalculateRightSide(xTemp, t + h / 2.0);
                
                xTemp = x->Add(k2->MultiplyByScalar(h / 2.0));
                MatrixLib^ k3 = CalculateRightSide(xTemp, t + h / 2.0);
                
                xTemp = x->Add(k3->MultiplyByScalar(h));
                MatrixLib^ k4 = CalculateRightSide(xTemp, t + h);
                
                // новое значение x
                MatrixLib^ sum = k1->Add(k2->MultiplyByScalar(2.0));
                sum = sum->Add(k3->MultiplyByScalar(2.0));
                sum = sum->Add(k4);
                
                MatrixLib^ nextX = x->Add(sum->MultiplyByScalar(h / 6.0));
                result[i + 1] = nextX;
                
                t += h;
            }
            
            return result;
        }
        
    public:
        AdamsSolverLib(MatrixLib^ A, MatrixLib^ B, MatrixLib^ C)
        {
            _A = A;
            _B = B;
            _C = C;
            
            // проверка размерностей
            if (A->Rows != A->Cols)
                throw gcnew ArgumentException("матрица A должна быть квадратной");
                
            if (B->Rows != A->Rows)
                throw gcnew ArgumentException("число строк матрицы B должно совпадать с размером матрицы A");
                
            if (C->Rows != A->Rows)
                throw gcnew ArgumentException("число строк матрицы C должно совпадать с размером матрицы A");
        }
        
        void SetFunctions(MatrixFunction^ u, MatrixFunction^ f)
        {
            _u = u;
            _f = f;
        }
        
        // решение методом Адамса-Башфорта
        List<KeyValuePair<double, MatrixLib^>>^ Solve(MatrixLib^ x0, double t0, double tEnd, double h)
        {
            // проверка начального вектора
            if (x0->Rows != _A->Rows || x0->Cols != 1)
                throw gcnew ArgumentException("размерность начального вектора должна соответствовать системе");
                
            List<KeyValuePair<double, MatrixLib^>>^ result = gcnew List<KeyValuePair<double, MatrixLib^>>();
            
            result->Add(KeyValuePair<double, MatrixLib^>(t0, x0));
            
            // начальные точки методом Рунге-Кутта
            array<MatrixLib^>^ initialPoints = RungeKutta4Steps(x0, t0, h, 3);
            
            for (int i = 1; i < initialPoints->Length; i++)
            {
                result->Add(KeyValuePair<double, MatrixLib^>(t0 + i * h, initialPoints[i]));
            }
            
            // метод Адамса-Башфорта
            double t = t0 + 3 * h;
            
            while (t < tEnd)
            {
                // последние 4 точки
                MatrixLib^ x_n = result[result->Count - 1].Value;
                MatrixLib^ x_n1 = result[result->Count - 2].Value;
                MatrixLib^ x_n2 = result[result->Count - 3].Value;
                MatrixLib^ x_n3 = result[result->Count - 4].Value;
                
                // значения правой части
                MatrixLib^ f_n = CalculateRightSide(x_n, t);
                MatrixLib^ f_n1 = CalculateRightSide(x_n1, t - h);
                MatrixLib^ f_n2 = CalculateRightSide(x_n2, t - 2 * h);
                MatrixLib^ f_n3 = CalculateRightSide(x_n3, t - 3 * h);
                
                // коэффициенты метода
                double c1 = 55.0 / 24.0;
                double c2 = -59.0 / 24.0;
                double c3 = 37.0 / 24.0;
                double c4 = -9.0 / 24.0;
                
                // новое значение x
                MatrixLib^ term1 = f_n->MultiplyByScalar(c1);
                MatrixLib^ term2 = f_n1->MultiplyByScalar(c2);
                MatrixLib^ term3 = f_n2->MultiplyByScalar(c3);
                MatrixLib^ term4 = f_n3->MultiplyByScalar(c4);
                
                MatrixLib^ sum = term1->Add(term2);
                sum = sum->Add(term3);
                sum = sum->Add(term4);
                
                MatrixLib^ x_next = x_n->Add(sum->MultiplyByScalar(h));
                
                t += h;
                result->Add(KeyValuePair<double, MatrixLib^>(t, x_next));
            }
            
            return result;
        }
    };
} 