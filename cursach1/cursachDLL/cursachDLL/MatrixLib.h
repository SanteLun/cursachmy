#pragma once

using namespace System;
using namespace System::Globalization;

namespace cursachDLL {

    // матрицы
    public ref class MatrixLib
    {
    private:
        array<double, 2>^ _data;
        int _rows, _cols;

    public:
        MatrixLib(int rows, int cols)
        {
            if (rows <= 0 || cols <= 0)
                throw gcnew ArgumentException("размеры матрицы должны быть положительными");
                
            _rows = rows;
            _cols = cols;
            _data = gcnew array<double, 2>(rows, cols);
            
            // заполняем нулями
            for (int i = 0; i < rows; i++)
                for (int j = 0; j < cols; j++)
                    _data[i, j] = 0.0;
        }
        
        MatrixLib(MatrixLib^ source)
        {
            _rows = source->Rows;
            _cols = source->Cols;
            _data = gcnew array<double, 2>(_rows, _cols);
            
            // копируем данные
            for (int i = 0; i < _rows; i++)
                for (int j = 0; j < _cols; j++)
                    _data[i, j] = source->Get(i, j);
        }
        
        property int Rows { int get() { return _rows; } }
        property int Cols { int get() { return _cols; } }
        
        double Get(int row, int col)
        {
            if (row < 0 || row >= _rows || col < 0 || col >= _cols)
                throw gcnew IndexOutOfRangeException("индекс вне диапазона");
                
            return _data[row, col];
        }
        
        void Set(int row, int col, double value)
        {
            if (row < 0 || row >= _rows || col < 0 || col >= _cols)
                throw gcnew IndexOutOfRangeException("индекс вне диапазона");
                
            _data[row, col] = value;
        }
        
        // сложение матриц
        MatrixLib^ Add(MatrixLib^ other)
        {
            if (_rows != other->Rows || _cols != other->Cols)
                throw gcnew ArgumentException("размеры матриц должны совпадать");
                
            MatrixLib^ result = gcnew MatrixLib(_rows, _cols);
            
            for (int i = 0; i < _rows; i++)
                for (int j = 0; j < _cols; j++)
                    result->Set(i, j, _data[i, j] + other->Get(i, j));
                    
            return result;
        }
        
        // умножение матриц
        MatrixLib^ Multiply(MatrixLib^ other)
        {
            if (_cols != other->Rows)
                throw gcnew ArgumentException("число столбцов первой матрицы должно равняться числу строк второй");
                
            MatrixLib^ result = gcnew MatrixLib(_rows, other->Cols);
            
            for (int i = 0; i < _rows; i++)
                for (int j = 0; j < other->Cols; j++)
                {
                    double sum = 0.0;
                    for (int k = 0; k < _cols; k++)
                        sum += _data[i, k] * other->Get(k, j);
                        
                    result->Set(i, j, sum);
                }
                
            return result;
        }
        
        // умножение на число
        MatrixLib^ MultiplyByScalar(double scalar)
        {
            MatrixLib^ result = gcnew MatrixLib(_rows, _cols);
            
            for (int i = 0; i < _rows; i++)
                for (int j = 0; j < _cols; j++)
                    result->Set(i, j, _data[i, j] * scalar);
                    
            return result;
        }
        
        // единичная матрица
        static MatrixLib^ Identity(int size)
        {
            MatrixLib^ result = gcnew MatrixLib(size, size);
            
            for (int i = 0; i < size; i++)
                result->Set(i, i, 1.0);
                
            return result;
        }
        
        virtual String^ ToString() override
        {
            System::Text::StringBuilder^ sb = gcnew System::Text::StringBuilder();
            
            for (int i = 0; i < _rows; i++)
            {
                for (int j = 0; j < _cols; j++)
                {
                    sb->Append(_data[i, j].ToString("F4"));
                    if (j < _cols - 1)
                        sb->Append("\t");
                }
                if (i < _rows - 1)
                    sb->Append("\r\n");
            }
            
            return sb->ToString();
        }
        
        // создание из строки
        static MatrixLib^ FromString(String^ str)
        {
            array<String^>^ rows = str->Split('\n');
            int rowCount = rows->Length;
            
            if (rowCount == 0)
                throw gcnew ArgumentException("неверный формат матрицы");
                
            array<String^>^ firstRowCols = rows[0]->Split('\t');
            int colCount = firstRowCols->Length;
            
            MatrixLib^ result = gcnew MatrixLib(rowCount, colCount);
            
            for (int i = 0; i < rowCount; i++)
            {
                array<String^>^ cols = rows[i]->Split('\t');
                
                if (cols->Length != colCount)
                    throw gcnew ArgumentException("неравномерное количество столбцов");
                    
                for (int j = 0; j < colCount; j++)
                {
                    double value;
                    // заменяем запятые и парсим
                    if (!Double::TryParse(cols[j]->Replace(",", "."), 
                                          System::Globalization::NumberStyles::Float, 
                                          System::Globalization::CultureInfo::InvariantCulture, 
                                          value))
                        throw gcnew ArgumentException("неверный формат числа");
                        
                    result->Set(i, j, value);
                }
            }
            
            return result;
        }
        
        // создание копии (клона) матрицы
        MatrixLib^ Clone()
        {
            MatrixLib^ result = gcnew MatrixLib(_rows, _cols);
            
            for (int i = 0; i < _rows; i++)
                for (int j = 0; j < _cols; j++)
                    result->Set(i, j, _data[i, j]);
                    
            return result;
        }
    };
} 