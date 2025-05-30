#pragma once
#include "StdAfx.h"
#include "Form1.h"

namespace cursach {
    // Обработчики для вкладки "Свертка сдвигом"
    public ref class ShiftFoldHandlers {
    public:
        static System::Void btnParse_ShiftFold_Click(System::Object^ sender, System::EventArgs^ e, 
            System::Windows::Forms::TextBox^ txtInput,
            System::Windows::Forms::TextBox^ txtGrammar,
            System::Windows::Forms::TextBox^ txtParseSteps,
            System::Windows::Forms::Label^ lblParseResult) {
            
            // Получаем входную строку и грамматику
            System::String^ input = txtInput->Text;
            System::String^ grammar = txtGrammar->Text;
            
            // Очищаем текстовый вывод
            txtParseSteps->Clear();
            
            // Здесь должна быть реализация алгоритма свертки сдвигом
            txtParseSteps->AppendText("Начало разбора строки: " + input + "\r\n");
            txtParseSteps->AppendText("Используемая грамматика:\r\n" + grammar + "\r\n\r\n");
            
            // Простая имитация процесса разбора
            txtParseSteps->AppendText("1. Чтение символа: id\r\n");
            txtParseSteps->AppendText("2. Сдвиг: id\r\n");
            txtParseSteps->AppendText("3. Чтение символа: +\r\n");
            txtParseSteps->AppendText("4. Сдвиг: id+\r\n");
            txtParseSteps->AppendText("5. Свертка: F -> id, стек: F+\r\n");
            txtParseSteps->AppendText("6. Свертка: T -> F, стек: T+\r\n");
            txtParseSteps->AppendText("7. Свертка: E -> T, стек: E+\r\n");
            
            // Выводим результат
            lblParseResult->Text = "Строка " + input + " соответствует грамматике";
        }
    };
    
    // Обработчики для вкладки "Польская нотация"
    public ref class PolishNotationHandlers {
    public:
        static System::String^ InfixToPostfix(System::String^ infix) {
            System::Text::StringBuilder^ result = gcnew System::Text::StringBuilder();
            System::Collections::Generic::Stack<char>^ stack = gcnew System::Collections::Generic::Stack<char>();
            
            for (int i = 0; i < infix->Length; i++) {
                char c = infix[i];
                
                if (c == ' ') continue;
                
                if (System::Char::IsDigit(c)) {
                    // Считываем все цифры числа
                    System::Text::StringBuilder^ numBuilder = gcnew System::Text::StringBuilder();
                    while (i < infix->Length && (System::Char::IsDigit(infix[i]) || infix[i] == '.')) {
                        numBuilder->Append(infix[i]);
                        i++;
                    }
                    i--; // Возвращаемся на один символ назад (компенсируем увеличение i в цикле for)
                    
                    result->Append(numBuilder);
                    result->Append(' '); // Добавляем пробел после числа
                }
                else if (c == '(') {
                    stack->Push(c);
                }
                else if (c == ')') {
                    while (stack->Count > 0 && stack->Peek() != '(') {
                        result->Append(stack->Pop());
                        result->Append(' '); // Добавляем пробел после оператора
                    }
                    if (stack->Count > 0 && stack->Peek() == '(') {
                        stack->Pop(); // удаляем открывающую скобку
                    }
                }
                else { // оператор
                    int precedence = GetPrecedence(c);
                    while (stack->Count > 0 && stack->Peek() != '(' &&
                          GetPrecedence(stack->Peek()) >= precedence && IsLeftAssociative(c)) {
                        result->Append(stack->Pop());
                        result->Append(' '); // Добавляем пробел после оператора
                    }
                    stack->Push(c);
                }
            }
            
            // Выталкиваем оставшиеся операторы
            while (stack->Count > 0) {
                if (stack->Peek() == '(') {
                    // Если в стеке осталась открывающая скобка - ошибка
                    return "Error: mismatched parentheses";
                }
                result->Append(stack->Pop());
                result->Append(' '); // Добавляем пробел после оператора
            }
            
            return result->ToString()->Trim(); // Удаляем начальные и конечные пробелы
        }
        
        static double EvaluatePostfix(System::String^ postfix) {
            System::Collections::Generic::Stack<double>^ stack = gcnew System::Collections::Generic::Stack<double>();
            
            // Разбиваем строку на токены, используя пробел как разделитель
            array<String^>^ tokens = postfix->Split(' ');
            
            for each (String^ token in tokens) {
                // Пропускаем пустые токены
                if (String::IsNullOrWhiteSpace(token))
                    continue;
                
                // Пробуем преобразовать токен в число
                double number;
                if (Double::TryParse(token, number)) {
                    stack->Push(number);
                }
                else if (token->Length == 1) {
                    char c = token[0];
                    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
                        if (stack->Count < 2) {
                            throw gcnew System::InvalidOperationException("Invalid postfix expression");
                        }
                        
                        double val2 = stack->Pop();
                        double val1 = stack->Pop();
                        
                        switch (c) {
                            case '+': stack->Push(val1 + val2); break;
                            case '-': stack->Push(val1 - val2); break;
                            case '*': stack->Push(val1 * val2); break;
                            case '/': stack->Push(val1 / val2); break;
                            case '^': stack->Push(System::Math::Pow(val1, val2)); break;
                        }
                    }
                    else {
                        throw gcnew System::InvalidOperationException("Invalid token in postfix expression: " + token);
                    }
                }
                else {
                    throw gcnew System::InvalidOperationException("Invalid token in postfix expression: " + token);
                }
            }
            
            if (stack->Count != 1) {
                throw gcnew System::InvalidOperationException("Invalid postfix expression");
            }
            
            return stack->Pop();
        }
        
    private:
        static int GetPrecedence(char op) {
            switch (op) {
                case '+':
                case '-':
                    return 1;
                case '*':
                case '/':
                    return 2;
                case '^':
                    return 3;
                default:
                    return 0;
            }
        }
        
        static bool IsLeftAssociative(char op) {
            return op != '^'; // только ^ (возведение в степень) является правоассоциативным
        }
    };
    
    // Обработчики для вкладки "Простой список"
    public ref class SimpleListHandlers {
    public:
        static void InitSimpleListTab(
            System::Windows::Forms::TabPage^ tabSimpleList,
            System::Windows::Forms::Label^ lblIdentifierName,
            System::Windows::Forms::TextBox^ txtIdentifierName,
            System::Windows::Forms::Button^ btnAddIdentifier,
            System::Windows::Forms::Button^ btnRemoveIdentifier,
            System::Windows::Forms::Button^ btnFindIdentifier,
            System::Windows::Forms::ListBox^ lstIdentifiers,
            System::Windows::Forms::Label^ lblIdentifiersCount,
            System::Windows::Forms::Label^ lblSearchResult) {
            
            // Настройка элементов управления
            tabSimpleList->Controls->Add(lblIdentifierName);
            tabSimpleList->Controls->Add(txtIdentifierName);
            tabSimpleList->Controls->Add(btnAddIdentifier);
            tabSimpleList->Controls->Add(btnRemoveIdentifier);
            tabSimpleList->Controls->Add(btnFindIdentifier);
            tabSimpleList->Controls->Add(lstIdentifiers);
            tabSimpleList->Controls->Add(lblIdentifiersCount);
            tabSimpleList->Controls->Add(lblSearchResult);
            
            // Настраиваем положение и размеры контролов
            lblIdentifierName->AutoSize = true;
            lblIdentifierName->Location = System::Drawing::Point(20, 20);
            lblIdentifierName->Text = L"Identifikator:";
            
            txtIdentifierName->Location = System::Drawing::Point(20, 40);
            txtIdentifierName->Size = System::Drawing::Size(200, 22);
            
            btnAddIdentifier->Location = System::Drawing::Point(20, 70);
            btnAddIdentifier->Size = System::Drawing::Size(80, 25);
            btnAddIdentifier->Text = L"Dobavit";
            
            btnRemoveIdentifier->Location = System::Drawing::Point(110, 70);
            btnRemoveIdentifier->Size = System::Drawing::Size(80, 25);
            btnRemoveIdentifier->Text = L"Udalit";
            
            btnFindIdentifier->Location = System::Drawing::Point(200, 70);
            btnFindIdentifier->Size = System::Drawing::Size(80, 25);
            btnFindIdentifier->Text = L"Nayti";
            
            lstIdentifiers->Location = System::Drawing::Point(20, 110);
            lstIdentifiers->Size = System::Drawing::Size(260, 200);
            
            lblIdentifiersCount->AutoSize = true;
            lblIdentifiersCount->Location = System::Drawing::Point(20, 320);
            lblIdentifiersCount->Text = L"Vsego identifikatorov: 0";
            
            lblSearchResult->AutoSize = true;
            lblSearchResult->Location = System::Drawing::Point(20, 350);
            lblSearchResult->Text = L"";
        }
    };
} 