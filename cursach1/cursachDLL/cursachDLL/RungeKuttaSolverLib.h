#pragma once

#include "MatrixLib.h"
using namespace System;
using namespace System::Collections::Generic;

namespace cursachDLL {
    // solver for ODE systems using 3rd-order Runge-Kutta (N=3)
    public ref class RungeKuttaSolverLib
    {
    private:
        MatrixLib^ _A;
        // for simplicity, only homogeneous term x' = A*x
    public:
        // constructor with system matrix A
        RungeKuttaSolverLib(MatrixLib^ A)
        {
            _A = A;
        }

        // solve x' = A*x, x(t0)=x0 over [t0, tEnd] with step h
        List<KeyValuePair<double, MatrixLib^>>^ Solve(MatrixLib^ x0, double t0, double tEnd, double h)
        {
            auto result = gcnew List<KeyValuePair<double, MatrixLib^>>();
            MatrixLib^ x = x0->Clone();
            double t = t0;
            result->Add(KeyValuePair<double, MatrixLib^>(t, x->Clone()));

            while (t < tEnd)
            {
                // k1 = A*x
                MatrixLib^ k1 = _A->Multiply(x);
                // k2 = A*(x + h/2*k1)
                MatrixLib^ xTemp = x->Add(k1->MultiplyByScalar(h/2.0));
                MatrixLib^ k2 = _A->Multiply(xTemp);
                // k3 = A*(x - h*k1 + 2*h*k2)
                xTemp = x->Add(k1->MultiplyByScalar(-h))->Add(k2->MultiplyByScalar(2*h));
                MatrixLib^ k3 = _A->Multiply(xTemp);

                // x_{n+1} = x + h/6*(k1 + 4*k2 + k3)
                MatrixLib^ sum = k1->MultiplyByScalar(1.0)
                    ->Add(k2->MultiplyByScalar(4.0))
                    ->Add(k3->MultiplyByScalar(1.0));
                MatrixLib^ xNext = x->Add(sum->MultiplyByScalar(h/6.0));

                t += h;
                x = xNext;
                result->Add(KeyValuePair<double, MatrixLib^>(t, x->Clone()));
            }

            return result;
        }
    };
} 