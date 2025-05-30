// cursachDLL.h

#pragma once

// Включаем созданные классы
#include "CaesarCipherLib.h"
#include "SimpleListLib.h"
#include "PolishNotationLib.h"
#include "MatrixLib.h"
#include "EulerSolverLib.h"
#include "ShiftFoldLib.h"
#include "MultiFunctionLib.h"
#include "RungeKuttaSolverLib.h"

using namespace System;

namespace cursachDLL {
	// Основной класс для экспорта всех функций библиотеки
	public ref class CursachLibrary
	{
	public:
		// Метод для создания шифровальщика Цезаря
		static CaesarCipherLib^ CreateCaesarCipher(int key)
		{
			return gcnew CaesarCipherLib(key);
		}
		
		// Метод для создания простого списка
		static SimpleListLib^ CreateSimpleList()
		{
			return gcnew SimpleListLib();
		}
		
		// Метод для работы с обратной польской записью
		static PolishNotationLib^ CreatePolishNotation()
		{
			return gcnew PolishNotationLib();
		}
		
		// Метод для создания матрицы
		static MatrixLib^ CreateMatrix(int rows, int cols)
		{
			return gcnew MatrixLib(rows, cols);
		}
		
		// Метод для создания решателя ОДУ методом Эйлера
		static EulerSolverLib^ CreateEulerSolver(MatrixLib^ A, MatrixLib^ B, MatrixLib^ C)
		{
			return gcnew EulerSolverLib(A, B, C);
		}
		
		// Метод для создания решателя ОДУ методом Эйлера без параметров
		static EulerSolverLib^ CreateEulerSolver()
		{
			return gcnew EulerSolverLib();
		}
		
		// Метод для синтаксического анализа методом сдвиг-свертка
		static ShiftFoldLib^ CreateShiftFoldParser()
		{
			return gcnew ShiftFoldLib();
		}
		
		// Метод для языка с функциями, возвращающими несколько результатов
		static MultiFunctionLib^ CreateMultiFunction()
		{
			return gcnew MultiFunctionLib();
		}
		
		// Метод для создания решателя ОДУ методом Рунге-Кутта (порядок фиксированный 3)
		static RungeKuttaSolverLib^ CreateRungeKuttaSolver(MatrixLib^ A)
		{
			return gcnew RungeKuttaSolverLib(A);
		}
		
		// Проверка работоспособности библиотеки
		static String^ TestLibrary()
		{
			return "CursachLibrary successfully loaded!";
		}
	};
}
