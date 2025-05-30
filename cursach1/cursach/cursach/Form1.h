#pragma once

// Важно: Убедитесь, что в настройках проекта Visual Studio (и в свойствах этого файла)
// включена поддержка CLR (Common Language Runtime Support, /clr).
// Файл stdafx.h также должен быть совместим с C++/CLI, если используется.
#include "stdafx.h" 
#include <locale.h>  // Для установки локали

// Директива для включения поддержки C++/CLI. Должна быть до использования типов .NET.
#pragma managed

#if defined(_MANAGED)
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <System.Data.dll>
#using <Microsoft.VisualBasic.dll> // Для InputBox и других утилит

// Раскомментируйте, если используете вашу DLL и путь указан верно.
// Убедитесь, что cursachDLL.dll скомпилирована и доступна по этому пути.
#using "..\\..\\cursachDLL\\Debug\\cursachDLL.dll"
#endif

// Помещаем ваши собственные #include директивы ПОСЛЕ #pragma managed и #using <System...>
// Это стандартная практика, чтобы сначала настроить среду C++/CLI.
#include "Authentication.h" 
// #include "LexicalAnalyzer.h" 
// #include "SyntaxAnalyzer.h"  
// #include "TabHandlers.h"     

namespace cursach {

    // Используем пространства имен .NET ПОСЛЕ их подключения через #using
    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Collections::Generic; // Для List<T>
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace System::Text;
    
    // Раскомментируйте, если у вас есть типы в пространстве имен cursachDLL из вашей DLL
    using namespace cursachDLL; 

    public ref class Form1 : public System::Windows::Forms::Form
    {
    public:
        Form1(void)
        {
            // Установка русской локали для интерфейса пользователя и форматирования
            System::Threading::Thread::CurrentThread->CurrentUICulture = gcnew System::Globalization::CultureInfo("ru-RU");
            System::Threading::Thread::CurrentThread->CurrentCulture = gcnew System::Globalization::CultureInfo("ru-RU");
            
            // init auth early so it's available during form load
            try {
                _auth = gcnew Authentication();
            } catch (System::Exception^ ex) {
                MessageBox::Show("Ошибка инициализации модуля аутентификации: " + ex->Message, "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
            
            InitializeComponent();
            
            _identifiersList = gcnew List<String^>();

            LockTabs(true); // Первоначальная блокировка
        }

    protected:
        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        ~Form1()
        {
            if (components)
            {
                delete components;
            }
        }

    private:
        // Флаг и объект аутентификации
        bool _isAuthenticated;
        Authentication^ _auth;

        // Основной TabControl
        System::Windows::Forms::TabControl^  tabControl1;

        // Вкладки (объяви здесь все свои TabPage)
        System::Windows::Forms::TabPage^  tabLogin;
        System::Windows::Forms::TabPage^  tabEncryption;
        System::Windows::Forms::TabPage^  tabLanguage;
        System::Windows::Forms::TabPage^  tabEuler;
        System::Windows::Forms::TabPage^  tabSimpleList;
        // Вкладку tabPolish убираем, так как она не нужна по заданию
        System::Windows::Forms::TabPage^  tabMultiFunction;
        System::Windows::Forms::TabPage^  tabUsers;

        // Компоненты для вкладки "Авторизация" (tabLogin)
        System::Windows::Forms::Label^  lblUsername;
        System::Windows::Forms::TextBox^  txtUsername;
        System::Windows::Forms::Label^  lblPassword;
        System::Windows::Forms::TextBox^  txtPassword;
        System::Windows::Forms::Button^  btnLogin;
        System::Windows::Forms::Button^  btnRegister;
        System::Windows::Forms::Label^  lblLoginStatus;

        // Компоненты для вкладки "Шифрование" (tabEncryption)
        System::Windows::Forms::Label^  lblKey;
        System::Windows::Forms::TextBox^  txtKey;
        System::Windows::Forms::Label^  lblPlainText;
        System::Windows::Forms::TextBox^  txtPlainText;
        System::Windows::Forms::Label^  lblCipherText;
        System::Windows::Forms::TextBox^  txtCipherText;
        System::Windows::Forms::Button^  btnEncrypt;
        System::Windows::Forms::Button^  btnDecrypt;
        
        // Компоненты для вкладки "Язык" (tabLanguage)
        System::Windows::Forms::Label^ lblSourceCode;
        System::Windows::Forms::TextBox^  txtSourceCode;
        System::Windows::Forms::Button^  btnParse;
        System::Windows::Forms::Label^ lblParseResultDisplay; // Изменено имя, чтобы не конфликтовать с переменной в методе
        System::Windows::Forms::TextBox^  txtParseOutput; // Изменено имя, чтобы не путать с полем класса
        // Дополнительные компоненты для грамматики, если они на этой вкладке
        System::Windows::Forms::Label^ lblInput;
        System::Windows::Forms::TextBox^ txtInput;
        System::Windows::Forms::Label^ lblGrammar;
        System::Windows::Forms::TextBox^ txtGrammar;
        System::Windows::Forms::Label^ lblParseStepsDisplay; // Изменено имя
        System::Windows::Forms::TextBox^ txtParseSteps;


        // Компоненты для вкладки "Runge-Kutta 3-го порядка" (tabEuler)
        System::Windows::Forms::Label^ lblMatrixA_ODE;
        System::Windows::Forms::TextBox^ txtMatrixA_ODE;
        System::Windows::Forms::Label^ lblInitialValue_ODE;
        System::Windows::Forms::TextBox^ txtInitialValue_ODE;
        System::Windows::Forms::Label^ lblTimeRange_ODE;
        System::Windows::Forms::TextBox^ txtTimeStart_ODE;
        System::Windows::Forms::TextBox^ txtTimeEnd_ODE;
        System::Windows::Forms::Label^ lblTimeStep_ODE;
        System::Windows::Forms::TextBox^ txtTimeStep_ODE;
        System::Windows::Forms::Button^ btnSolveODE;
        System::Windows::Forms::TextBox^ txtEulerResult;


        // Компоненты для вкладки "Простой список" (tabSimpleList)
        System::Windows::Forms::Label^ lblIdentifierName;
        System::Windows::Forms::TextBox^ txtIdentifierName;
        System::Windows::Forms::Button^ btnAddIdentifier;
        System::Windows::Forms::Button^ btnRemoveIdentifier;
        System::Windows::Forms::Button^ btnFindIdentifier;
        System::Windows::Forms::ListBox^ lstIdentifiers;
        System::Windows::Forms::Label^ lblIdentifiersCount;
        System::Windows::Forms::Label^ lblSearchResult;
        List<String^>^ _identifiersList;

        // Компоненты для вкладки "Польская нотация" удалены, так как они не нужны по заданию


        // Компоненты для вкладки "Множественные значения" (tabMultiFunction)
        System::Windows::Forms::Label^ lblFunctionCall;
        System::Windows::Forms::TextBox^ txtFunctionCall;
        System::Windows::Forms::Button^ btnExecuteFunction;
        System::Windows::Forms::Label^ lblResultMulti;
        System::Windows::Forms::TextBox^ txtResultMulti;

        // Компоненты для вкладки "Управление пользователями" (tabUsers)
        System::Windows::Forms::ListView^ lvUsers;
        System::Windows::Forms::Button^ btnAddUser;
        System::Windows::Forms::Button^ btnEditUser;
        System::Windows::Forms::Button^ btnDeleteUser;
        System::Windows::Forms::Button^ btnRefreshUsers;
        System::Windows::Forms::Label^ lblUsersList;


    private:
        /// <summary>
        /// Required designer variable.
        /// </summary>
        System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        void InitializeComponent(void)
        {
            this->components = (gcnew System::ComponentModel::Container());
            this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
            
            // Инициализация всех TabPage (как было раньше)
            this->tabLogin = (gcnew System::Windows::Forms::TabPage());
            this->tabEncryption = (gcnew System::Windows::Forms::TabPage());
            this->tabLanguage = (gcnew System::Windows::Forms::TabPage());
            this->tabEuler = (gcnew System::Windows::Forms::TabPage());
            this->tabSimpleList = (gcnew System::Windows::Forms::TabPage());
            
            this->tabMultiFunction = (gcnew System::Windows::Forms::TabPage());
            this->tabUsers = (gcnew System::Windows::Forms::TabPage());
            
            // Инициализация компонентов для вкладки Users
            this->lvUsers = (gcnew System::Windows::Forms::ListView());
            this->btnAddUser = (gcnew System::Windows::Forms::Button());
            this->btnEditUser = (gcnew System::Windows::Forms::Button());
            this->btnDeleteUser = (gcnew System::Windows::Forms::Button());
            this->btnRefreshUsers = (gcnew System::Windows::Forms::Button());
            this->lblUsersList = (gcnew System::Windows::Forms::Label());

            // Инициализация компонентов для tabLogin
            this->lblUsername = (gcnew System::Windows::Forms::Label());
            this->txtUsername = (gcnew System::Windows::Forms::TextBox());
            this->lblPassword = (gcnew System::Windows::Forms::Label());
            this->txtPassword = (gcnew System::Windows::Forms::TextBox());
            this->btnLogin = (gcnew System::Windows::Forms::Button());
            this->btnRegister = (gcnew System::Windows::Forms::Button());
            this->lblLoginStatus = (gcnew System::Windows::Forms::Label());
            
            // Инициализация компонентов для вкладки шифрования
            this->lblKey = (gcnew System::Windows::Forms::Label());
            this->txtKey = (gcnew System::Windows::Forms::TextBox());
            this->lblPlainText = (gcnew System::Windows::Forms::Label());
            this->txtPlainText = (gcnew System::Windows::Forms::TextBox());
            this->lblCipherText = (gcnew System::Windows::Forms::Label());
            this->txtCipherText = (gcnew System::Windows::Forms::TextBox());
            this->btnEncrypt = (gcnew System::Windows::Forms::Button());
            this->btnDecrypt = (gcnew System::Windows::Forms::Button());
            
            // Инициализация компонентов для вкладки "Язык"
            this->lblSourceCode = (gcnew System::Windows::Forms::Label());
            this->txtSourceCode = (gcnew System::Windows::Forms::TextBox());
            this->btnParse = (gcnew System::Windows::Forms::Button());
            this->lblParseResultDisplay = (gcnew System::Windows::Forms::Label());
            this->txtParseOutput = (gcnew System::Windows::Forms::TextBox());
            this->lblInput = (gcnew System::Windows::Forms::Label());
            this->txtInput = (gcnew System::Windows::Forms::TextBox());
            this->lblGrammar = (gcnew System::Windows::Forms::Label());
            this->txtGrammar = (gcnew System::Windows::Forms::TextBox());
            this->lblParseStepsDisplay = (gcnew System::Windows::Forms::Label());
            this->txtParseSteps = (gcnew System::Windows::Forms::TextBox());
            
            // Инициализация компонентов для вкладки "Runge-Kutta 3-го порядка"
            this->lblMatrixA_ODE = (gcnew System::Windows::Forms::Label());
            this->txtMatrixA_ODE = (gcnew System::Windows::Forms::TextBox());
            this->lblInitialValue_ODE = (gcnew System::Windows::Forms::Label());
            this->txtInitialValue_ODE = (gcnew System::Windows::Forms::TextBox());
            this->lblTimeRange_ODE = (gcnew System::Windows::Forms::Label());
            this->txtTimeStart_ODE = (gcnew System::Windows::Forms::TextBox());
            this->txtTimeEnd_ODE = (gcnew System::Windows::Forms::TextBox());
            this->lblTimeStep_ODE = (gcnew System::Windows::Forms::Label());
            this->txtTimeStep_ODE = (gcnew System::Windows::Forms::TextBox());
            this->btnSolveODE = (gcnew System::Windows::Forms::Button());
            this->txtEulerResult = (gcnew System::Windows::Forms::TextBox());
            
            // Инициализация компонентов для вкладки "Простой список"
            this->lblIdentifierName = (gcnew System::Windows::Forms::Label());
            this->txtIdentifierName = (gcnew System::Windows::Forms::TextBox());
            this->btnAddIdentifier = (gcnew System::Windows::Forms::Button());
            this->btnRemoveIdentifier = (gcnew System::Windows::Forms::Button());
            this->btnFindIdentifier = (gcnew System::Windows::Forms::Button());
            this->lstIdentifiers = (gcnew System::Windows::Forms::ListBox());
            this->lblIdentifiersCount = (gcnew System::Windows::Forms::Label());
            this->lblSearchResult = (gcnew System::Windows::Forms::Label());
            
            // Инициализация компонентов для вкладки "Множественные значения" (tabMultiFunction)
            this->lblFunctionCall = (gcnew System::Windows::Forms::Label());
            this->txtFunctionCall = (gcnew System::Windows::Forms::TextBox());
            this->btnExecuteFunction = (gcnew System::Windows::Forms::Button());
            this->lblResultMulti = (gcnew System::Windows::Forms::Label());
            this->txtResultMulti = (gcnew System::Windows::Forms::TextBox());
            
            // Приостановка компоновки для tabControl1 и tabLogin
            this->tabControl1->SuspendLayout();
            this->tabLogin->SuspendLayout();
            this->tabEncryption->SuspendLayout();
            this->tabLanguage->SuspendLayout();
            this->tabEuler->SuspendLayout();
            this->tabSimpleList->SuspendLayout();
            
            this->tabMultiFunction->SuspendLayout();
            this->tabUsers->SuspendLayout();
            this->SuspendLayout();
            
            // tabControl1
            // ВАЖНО: Добавляем ВСЕ вкладки в TabControl
            this->tabControl1->Controls->Add(this->tabLogin);
            this->tabControl1->Controls->Add(this->tabEncryption);
            this->tabControl1->Controls->Add(this->tabLanguage);
            this->tabControl1->Controls->Add(this->tabEuler);
            this->tabControl1->Controls->Add(this->tabSimpleList);
            
            this->tabControl1->Controls->Add(this->tabMultiFunction);
            this->tabControl1->Controls->Add(this->tabUsers);
            this->tabControl1->Dock = System::Windows::Forms::DockStyle::Fill;
            this->tabControl1->Location = System::Drawing::Point(0, 0);
            this->tabControl1->Name = L"tabControl1";
            this->tabControl1->SelectedIndex = 0;
            this->tabControl1->Size = System::Drawing::Size(784, 561);
            this->tabControl1->TabIndex = 0;
            this->tabControl1->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::tabControl1_SelectedIndexChanged);

            // Настройка tabLogin (как было, убедись, что все компоненты добавлены в tabLogin->Controls)
            this->tabLogin->Text = L"Авторизация";
            this->tabEncryption->Text = L"Шифрование (Цезарь)"; // Задание №2
            this->tabLanguage->Text = L"Массивы"; // Задание №3 - операторы объявления массивов
            this->tabEuler->Text = L"ОДУ (Рунге-Кутта)"; // Задание №1
            this->tabSimpleList->Text = L"Хеш-таблица (ID)"; // Задание №4 - хеш-адресация
            // Вкладка Польская нотация удалена
            this->tabMultiFunction->Text = L"Внутр. представление"; // Задание №6 - триады
            this->tabUsers->Text = L"Пользователи";
            
            // Настройка ListView пользователей
            this->lvUsers->Dock = System::Windows::Forms::DockStyle::None;
            this->lvUsers->Location = System::Drawing::Point(10, 40);
            this->lvUsers->Name = L"lvUsers";
            this->lvUsers->Size = System::Drawing::Size(650, 300);
            this->lvUsers->View = System::Windows::Forms::View::Details;
            this->lvUsers->FullRowSelect = true;
            this->lvUsers->GridLines = true;
            this->tabUsers->Controls->Add(this->lvUsers);
            
            // Настройка кнопок управления пользователями
            this->btnAddUser->Location = System::Drawing::Point(10, 350);
            this->btnAddUser->Name = L"btnAddUser";
            this->btnAddUser->Size = System::Drawing::Size(120, 30);
            this->btnAddUser->Text = L"Добавить";
            this->btnAddUser->UseVisualStyleBackColor = true;
            this->btnAddUser->Click += gcnew System::EventHandler(this, &Form1::btnAddUser_Click);
            this->tabUsers->Controls->Add(this->btnAddUser);
            
            this->btnEditUser->Location = System::Drawing::Point(140, 350);
            this->btnEditUser->Name = L"btnEditUser";
            this->btnEditUser->Size = System::Drawing::Size(120, 30);
            this->btnEditUser->Text = L"Редактировать";
            this->btnEditUser->UseVisualStyleBackColor = true;
            this->btnEditUser->Click += gcnew System::EventHandler(this, &Form1::btnEditUser_Click);
            this->tabUsers->Controls->Add(this->btnEditUser);
            
            this->btnDeleteUser->Location = System::Drawing::Point(270, 350);
            this->btnDeleteUser->Name = L"btnDeleteUser";
            this->btnDeleteUser->Size = System::Drawing::Size(120, 30);
            this->btnDeleteUser->Text = L"Удалить";
            this->btnDeleteUser->UseVisualStyleBackColor = true;
            this->btnDeleteUser->Click += gcnew System::EventHandler(this, &Form1::btnDeleteUser_Click);
            this->tabUsers->Controls->Add(this->btnDeleteUser);
            
            this->btnRefreshUsers->Location = System::Drawing::Point(400, 350);
            this->btnRefreshUsers->Name = L"btnRefreshUsers";
            this->btnRefreshUsers->Size = System::Drawing::Size(120, 30);
            this->btnRefreshUsers->Text = L"Обновить";
            this->btnRefreshUsers->UseVisualStyleBackColor = true;
            this->btnRefreshUsers->Click += gcnew System::EventHandler(this, &Form1::btnRefreshUsers_Click);
            this->tabUsers->Controls->Add(this->btnRefreshUsers);
            
            this->lblUsersList->AutoSize = true;
            this->lblUsersList->Location = System::Drawing::Point(10, 15);
            this->lblUsersList->Name = L"lblUsersList";
            this->lblUsersList->Size = System::Drawing::Size(200, 20);
            this->lblUsersList->Text = L"Список пользователей:";
            this->tabUsers->Controls->Add(this->lblUsersList);

            // Настройки для вкладки авторизации
            this->lblUsername->AutoSize = true;
            this->lblUsername->Location = System::Drawing::Point(250, 100);
            this->lblUsername->Name = L"lblUsername";
            this->lblUsername->Text = L"Логин:";
            this->tabLogin->Controls->Add(this->lblUsername);
            
            this->txtUsername->Location = System::Drawing::Point(320, 97);
            this->txtUsername->Name = L"txtUsername";
            this->txtUsername->Size = System::Drawing::Size(150, 20);
            this->tabLogin->Controls->Add(this->txtUsername);
            
            this->lblPassword->AutoSize = true;
            this->lblPassword->Location = System::Drawing::Point(250, 130);
            this->lblPassword->Name = L"lblPassword";
            this->lblPassword->Text = L"Пароль:";
            this->tabLogin->Controls->Add(this->lblPassword);
            
            this->txtPassword->Location = System::Drawing::Point(320, 127);
            this->txtPassword->Name = L"txtPassword";
            this->txtPassword->PasswordChar = '*';
            this->txtPassword->Size = System::Drawing::Size(150, 20);
            this->tabLogin->Controls->Add(this->txtPassword);
            
            this->btnLogin->Location = System::Drawing::Point(320, 160);
            this->btnLogin->Name = L"btnLogin";
            this->btnLogin->Size = System::Drawing::Size(75, 23);
            this->btnLogin->Text = L"Войти";
            this->btnLogin->UseVisualStyleBackColor = true;
            this->btnLogin->Click += gcnew System::EventHandler(this, &Form1::btnLogin_Click);
            this->tabLogin->Controls->Add(this->btnLogin);
            
            this->btnRegister->Location = System::Drawing::Point(400, 160);
            this->btnRegister->Name = L"btnRegister";
            this->btnRegister->Size = System::Drawing::Size(90, 23);
            this->btnRegister->Text = L"Регистрация";
            this->btnRegister->UseVisualStyleBackColor = true;
            this->btnRegister->Click += gcnew System::EventHandler(this, &Form1::btnRegister_Click);
            this->tabLogin->Controls->Add(this->btnRegister);
            
            this->lblLoginStatus->AutoSize = true;
            this->lblLoginStatus->Location = System::Drawing::Point(320, 200);
            this->lblLoginStatus->Name = L"lblLoginStatus";
            this->lblLoginStatus->Text = L"";
            this->tabLogin->Controls->Add(this->lblLoginStatus);
            
            // Настройки для вкладки шифрования
            this->lblKey->AutoSize = true;
            this->lblKey->Location = System::Drawing::Point(20, 20);
            this->lblKey->Name = L"lblKey";
            this->lblKey->Text = L"Ключ:";
            this->tabEncryption->Controls->Add(this->lblKey);
            
            this->txtKey->Location = System::Drawing::Point(100, 20);
            this->txtKey->Name = L"txtKey";
            this->txtKey->Size = System::Drawing::Size(300, 20);
            this->tabEncryption->Controls->Add(this->txtKey);
            
            this->lblPlainText->AutoSize = true;
            this->lblPlainText->Location = System::Drawing::Point(20, 50);
            this->lblPlainText->Name = L"lblPlainText";
            this->lblPlainText->Text = L"Исходный текст:";
            this->tabEncryption->Controls->Add(this->lblPlainText);
            
            this->txtPlainText->Location = System::Drawing::Point(20, 70);
            this->txtPlainText->Multiline = true;
            this->txtPlainText->Name = L"txtPlainText";
            this->txtPlainText->Size = System::Drawing::Size(700, 100);
            this->tabEncryption->Controls->Add(this->txtPlainText);
            
            this->lblCipherText->AutoSize = true;
            this->lblCipherText->Location = System::Drawing::Point(20, 190);
            this->lblCipherText->Name = L"lblCipherText";
            this->lblCipherText->Text = L"Зашифрованный текст:";
            this->tabEncryption->Controls->Add(this->lblCipherText);
            
            this->txtCipherText->Location = System::Drawing::Point(20, 210);
            this->txtCipherText->Multiline = true;
            this->txtCipherText->Name = L"txtCipherText";
            this->txtCipherText->Size = System::Drawing::Size(700, 100);
            this->tabEncryption->Controls->Add(this->txtCipherText);
            
            this->btnEncrypt->Location = System::Drawing::Point(20, 320);
            this->btnEncrypt->Name = L"btnEncrypt";
            this->btnEncrypt->Size = System::Drawing::Size(120, 30);
            this->btnEncrypt->Text = L"Зашифровать";
            this->btnEncrypt->UseVisualStyleBackColor = true;
            this->btnEncrypt->Click += gcnew System::EventHandler(this, &Form1::btnEncrypt_Click);
            this->tabEncryption->Controls->Add(this->btnEncrypt);
            
            this->btnDecrypt->Location = System::Drawing::Point(150, 320);
            this->btnDecrypt->Name = L"btnDecrypt";
            this->btnDecrypt->Size = System::Drawing::Size(120, 30);
            this->btnDecrypt->Text = L"Расшифровать";
            this->btnDecrypt->UseVisualStyleBackColor = true;
            this->btnDecrypt->Click += gcnew System::EventHandler(this, &Form1::btnDecrypt_Click);
            this->tabEncryption->Controls->Add(this->btnDecrypt);
            
            // Настройки для вкладки Язык (Парсер)
            this->lblSourceCode->AutoSize = true;
            this->lblSourceCode->Location = System::Drawing::Point(20, 20);
            this->lblSourceCode->Name = L"lblSourceCode";
            this->lblSourceCode->Text = L"Исходный код:";
            this->tabLanguage->Controls->Add(this->lblSourceCode);
            
            this->txtSourceCode->Location = System::Drawing::Point(20, 40);
            this->txtSourceCode->Multiline = true;
            this->txtSourceCode->Name = L"txtSourceCode";
            this->txtSourceCode->Size = System::Drawing::Size(700, 150);
            this->tabLanguage->Controls->Add(this->txtSourceCode);
            
            this->btnParse->Location = System::Drawing::Point(20, 200);
            this->btnParse->Name = L"btnParse";
            this->btnParse->Size = System::Drawing::Size(120, 30);
            this->btnParse->Text = L"Парсить";
            this->btnParse->UseVisualStyleBackColor = true;
            this->btnParse->Click += gcnew System::EventHandler(this, &Form1::btnParse_Click);
            this->tabLanguage->Controls->Add(this->btnParse);
            
            this->lblParseResultDisplay->AutoSize = true;
            this->lblParseResultDisplay->Location = System::Drawing::Point(20, 240);
            this->lblParseResultDisplay->Name = L"lblParseResultDisplay";
            this->lblParseResultDisplay->Text = L"Результат:";
            this->tabLanguage->Controls->Add(this->lblParseResultDisplay);
            
            this->txtParseOutput->Location = System::Drawing::Point(20, 260);
            this->txtParseOutput->Multiline = true;
            this->txtParseOutput->Name = L"txtParseOutput";
            this->txtParseOutput->Size = System::Drawing::Size(700, 150);
            this->tabLanguage->Controls->Add(this->txtParseOutput);
            
            // Добавляем основные элементы для других вкладок
            // Вкладка ОДУ
            this->lblMatrixA_ODE->AutoSize = true;
            this->lblMatrixA_ODE->Location = System::Drawing::Point(20, 20);
            this->lblMatrixA_ODE->Name = L"lblMatrixA_ODE";
            this->lblMatrixA_ODE->Text = L"Матрица A:";
            this->tabEuler->Controls->Add(this->lblMatrixA_ODE);
            
            this->txtMatrixA_ODE->Location = System::Drawing::Point(100, 20);
            this->txtMatrixA_ODE->Name = L"txtMatrixA_ODE";
            this->txtMatrixA_ODE->Size = System::Drawing::Size(300, 20);
            this->tabEuler->Controls->Add(this->txtMatrixA_ODE);
            
            this->lblInitialValue_ODE->AutoSize = true;
            this->lblInitialValue_ODE->Location = System::Drawing::Point(20, 50);
            this->lblInitialValue_ODE->Name = L"lblInitialValue_ODE";
            this->lblInitialValue_ODE->Text = L"Начальное значение:";
            this->tabEuler->Controls->Add(this->lblInitialValue_ODE);
            
            this->txtInitialValue_ODE->Location = System::Drawing::Point(150, 50);
            this->txtInitialValue_ODE->Name = L"txtInitialValue_ODE";
            this->txtInitialValue_ODE->Size = System::Drawing::Size(100, 20);
            this->tabEuler->Controls->Add(this->txtInitialValue_ODE);
            
            this->lblTimeRange_ODE->AutoSize = true;
            this->lblTimeRange_ODE->Location = System::Drawing::Point(20, 80);
            this->lblTimeRange_ODE->Name = L"lblTimeRange_ODE";
            this->lblTimeRange_ODE->Text = L"Временной интервал:";
            this->tabEuler->Controls->Add(this->lblTimeRange_ODE);
            
            this->txtTimeStart_ODE->Location = System::Drawing::Point(150, 80);
            this->txtTimeStart_ODE->Name = L"txtTimeStart_ODE";
            this->txtTimeStart_ODE->Size = System::Drawing::Size(50, 20);
            this->tabEuler->Controls->Add(this->txtTimeStart_ODE);
            
            this->txtTimeEnd_ODE->Location = System::Drawing::Point(210, 80);
            this->txtTimeEnd_ODE->Name = L"txtTimeEnd_ODE";
            this->txtTimeEnd_ODE->Size = System::Drawing::Size(50, 20);
            this->tabEuler->Controls->Add(this->txtTimeEnd_ODE);
            
            this->lblTimeStep_ODE->AutoSize = true;
            this->lblTimeStep_ODE->Location = System::Drawing::Point(20, 110);
            this->lblTimeStep_ODE->Name = L"lblTimeStep_ODE";
            this->lblTimeStep_ODE->Text = L"Шаг:";
            this->tabEuler->Controls->Add(this->lblTimeStep_ODE);
            
            this->txtTimeStep_ODE->Location = System::Drawing::Point(150, 110);
            this->txtTimeStep_ODE->Name = L"txtTimeStep_ODE";
            this->txtTimeStep_ODE->Size = System::Drawing::Size(50, 20);
            this->tabEuler->Controls->Add(this->txtTimeStep_ODE);
            
            this->btnSolveODE->Location = System::Drawing::Point(20, 140);
            this->btnSolveODE->Name = L"btnSolveODE";
            this->btnSolveODE->Size = System::Drawing::Size(120, 30);
            this->btnSolveODE->Text = L"Sgenerirovat' matritsu";
            this->btnSolveODE->UseVisualStyleBackColor = true;
            this->btnSolveODE->Click += gcnew System::EventHandler(this, &Form1::btnSolveODE_Click);
            this->tabEuler->Controls->Add(this->btnSolveODE);
            
            this->txtEulerResult->Location = System::Drawing::Point(20, 180);
            this->txtEulerResult->Multiline = true;
            this->txtEulerResult->Name = L"txtEulerResult";
            this->txtEulerResult->Size = System::Drawing::Size(700, 300);
            this->tabEuler->Controls->Add(this->txtEulerResult);
            
            // Вкладка простой список
            this->lblIdentifierName->AutoSize = true;
            this->lblIdentifierName->Location = System::Drawing::Point(20, 20);
            this->lblIdentifierName->Name = L"lblIdentifierName";
            this->lblIdentifierName->Text = L"Имя идентификатора:";
            this->tabSimpleList->Controls->Add(this->lblIdentifierName);
            
            this->txtIdentifierName->Location = System::Drawing::Point(150, 20);
            this->txtIdentifierName->Name = L"txtIdentifierName";
            this->txtIdentifierName->Size = System::Drawing::Size(200, 20);
            this->tabSimpleList->Controls->Add(this->txtIdentifierName);
            
            this->btnAddIdentifier->Location = System::Drawing::Point(20, 50);
            this->btnAddIdentifier->Name = L"btnAddIdentifier";
            this->btnAddIdentifier->Size = System::Drawing::Size(100, 30);
            this->btnAddIdentifier->Text = L"Добавить";
            this->btnAddIdentifier->UseVisualStyleBackColor = true;
            this->btnAddIdentifier->Click += gcnew System::EventHandler(this, &Form1::btnAddIdentifier_Click);
            this->tabSimpleList->Controls->Add(this->btnAddIdentifier);
            
            this->btnRemoveIdentifier->Location = System::Drawing::Point(130, 50);
            this->btnRemoveIdentifier->Name = L"btnRemoveIdentifier";
            this->btnRemoveIdentifier->Size = System::Drawing::Size(100, 30);
            this->btnRemoveIdentifier->Text = L"Удалить";
            this->btnRemoveIdentifier->UseVisualStyleBackColor = true;
            this->btnRemoveIdentifier->Click += gcnew System::EventHandler(this, &Form1::btnRemoveIdentifier_Click);
            this->tabSimpleList->Controls->Add(this->btnRemoveIdentifier);
            
            this->btnFindIdentifier->Location = System::Drawing::Point(240, 50);
            this->btnFindIdentifier->Name = L"btnFindIdentifier";
            this->btnFindIdentifier->Size = System::Drawing::Size(100, 30);
            this->btnFindIdentifier->Text = L"Найти";
            this->btnFindIdentifier->UseVisualStyleBackColor = true;
            this->btnFindIdentifier->Click += gcnew System::EventHandler(this, &Form1::btnFindIdentifier_Click);
            this->tabSimpleList->Controls->Add(this->btnFindIdentifier);
            
            this->lstIdentifiers->FormattingEnabled = true;
            this->lstIdentifiers->Location = System::Drawing::Point(20, 100);
            this->lstIdentifiers->Name = L"lstIdentifiers";
            this->lstIdentifiers->Size = System::Drawing::Size(300, 200);
            this->lstIdentifiers->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::lstIdentifiers_SelectedIndexChanged);
            this->tabSimpleList->Controls->Add(this->lstIdentifiers);
            
            this->lblIdentifiersCount->AutoSize = true;
            this->lblIdentifiersCount->Location = System::Drawing::Point(20, 310);
            this->lblIdentifiersCount->Name = L"lblIdentifiersCount";
            this->lblIdentifiersCount->Text = L"Всего идентификаторов: 0";
            this->tabSimpleList->Controls->Add(this->lblIdentifiersCount);
            
            this->lblSearchResult->AutoSize = true;
            this->lblSearchResult->Location = System::Drawing::Point(20, 340);
            this->lblSearchResult->Name = L"lblSearchResult";
            this->lblSearchResult->Text = L"";
            this->tabSimpleList->Controls->Add(this->lblSearchResult);
            
            // Код для вкладки "Польская нотация" удален, так как не требуется по заданию
            
            // Вкладка множественные значения
            this->lblFunctionCall->AutoSize = true;
            this->lblFunctionCall->Location = System::Drawing::Point(20, 20);
            this->lblFunctionCall->Name = L"lblFunctionCall";
            this->lblFunctionCall->Text = L"Вызов функции:";
            this->tabMultiFunction->Controls->Add(this->lblFunctionCall);
            
            this->txtFunctionCall->Location = System::Drawing::Point(20, 40);
            this->txtFunctionCall->Name = L"txtFunctionCall";
            this->txtFunctionCall->Size = System::Drawing::Size(400, 20);
            this->tabMultiFunction->Controls->Add(this->txtFunctionCall);
            
            this->btnExecuteFunction->Location = System::Drawing::Point(20, 70);
            this->btnExecuteFunction->Name = L"btnExecuteFunction";
            this->btnExecuteFunction->Size = System::Drawing::Size(120, 30);
            this->btnExecuteFunction->Text = L"Выполнить";
            this->btnExecuteFunction->UseVisualStyleBackColor = true;
            this->btnExecuteFunction->Click += gcnew System::EventHandler(this, &Form1::btnExecuteFunction_Click);
            this->tabMultiFunction->Controls->Add(this->btnExecuteFunction);
            
            this->lblResultMulti->AutoSize = true;
            this->lblResultMulti->Location = System::Drawing::Point(20, 110);
            this->lblResultMulti->Name = L"lblResultMulti";
            this->lblResultMulti->Text = L"Результат:";
            this->tabMultiFunction->Controls->Add(this->lblResultMulti);
            
            this->txtResultMulti->Location = System::Drawing::Point(20, 130);
            this->txtResultMulti->Multiline = true;
            this->txtResultMulti->Name = L"txtResultMulti";
            this->txtResultMulti->Size = System::Drawing::Size(700, 300);
            this->tabMultiFunction->Controls->Add(this->txtResultMulti);

            // Form1
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(784, 561);
            this->Controls->Add(this->tabControl1);
            this->Name = L"Form1";
            this->Text = L"Курсовая работа по Компиляторам";
            this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
            
            this->tabControl1->ResumeLayout(false);
            this->tabLogin->ResumeLayout(false); this->tabLogin->PerformLayout();
            this->tabEncryption->ResumeLayout(false); this->tabEncryption->PerformLayout();
            this->tabLanguage->ResumeLayout(false); this->tabLanguage->PerformLayout();
            this->tabEuler->ResumeLayout(false); this->tabEuler->PerformLayout();
            this->tabSimpleList->ResumeLayout(false); this->tabSimpleList->PerformLayout();
            // Убираем вызов ResumeLayout для tabPolish
            this->tabMultiFunction->ResumeLayout(false); this->tabMultiFunction->PerformLayout();
            this->tabUsers->ResumeLayout(false); this->tabUsers->PerformLayout();
            this->ResumeLayout(false);
        }
#pragma endregion

    // --- Объявления вспомогательных методов и обработчиков событий ---
    private: 
        void RefreshUsersList(); 
        System::Void Form1_Load(System::Object^ sender, System::EventArgs^ e);
        System::Void tabControl1_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);
        System::Void btnLogin_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnRegister_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnEncrypt_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void btnDecrypt_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void btnParse_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnSolveODE_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void btnApplyMatrix_Click(System::Object^  sender, System::EventArgs^  e);
        void SolveODEWithMatrix(array<array<double>^>^ A, int n);
        System::Void btnAddIdentifier_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void btnRemoveIdentifier_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void btnFindIdentifier_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void lstIdentifiers_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
        // Обработчики для польской нотации удалены, так как они не нужны по заданию
        System::Void btnExecuteFunction_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void btnAddUser_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void btnEditUser_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void btnDeleteUser_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void btnRefreshUsers_Click(System::Object^  sender, System::EventArgs^  e);
        
        // --- Вспомогательный метод для блокировки/разблокировки вкладок --- 
        void LockTabs(bool lockState);
        
    }; // конец класса Form1
} // конец namespace cursach

