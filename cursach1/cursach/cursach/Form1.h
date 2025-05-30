#pragma once

// �����: ���������, ��� � ���������� ������� Visual Studio (� � ��������� ����� �����)
// �������� ��������� CLR (Common Language Runtime Support, /clr).
// ���� stdafx.h ����� ������ ���� ��������� � C++/CLI, ���� ������������.
#include "stdafx.h" 
#include <locale.h>  // ��� ��������� ������

// ��������� ��� ��������� ��������� C++/CLI. ������ ���� �� ������������� ����� .NET.
#pragma managed

#if defined(_MANAGED)
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <System.Data.dll>
#using <Microsoft.VisualBasic.dll> // ��� InputBox � ������ ������

// ����������������, ���� ����������� ���� DLL � ���� ������ �����.
// ���������, ��� cursachDLL.dll �������������� � �������� �� ����� ����.
#using "..\\..\\cursachDLL\\Debug\\cursachDLL.dll"
#endif

// �������� ���� ����������� #include ��������� ����� #pragma managed � #using <System...>
// ��� ����������� ��������, ����� ������� ��������� ����� C++/CLI.
#include "Authentication.h" 
// #include "LexicalAnalyzer.h" 
// #include "SyntaxAnalyzer.h"  
// #include "TabHandlers.h"     

namespace cursach {

    // ���������� ������������ ���� .NET ����� �� ����������� ����� #using
    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Collections::Generic; // ��� List<T>
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace System::Text;
    
    // ����������������, ���� � ��� ���� ���� � ������������ ���� cursachDLL �� ����� DLL
    using namespace cursachDLL; 

    public ref class Form1 : public System::Windows::Forms::Form
    {
    public:
        Form1(void)
        {
            // ��������� ������� ������ ��� ���������� ������������ � ��������������
            System::Threading::Thread::CurrentThread->CurrentUICulture = gcnew System::Globalization::CultureInfo("ru-RU");
            System::Threading::Thread::CurrentThread->CurrentCulture = gcnew System::Globalization::CultureInfo("ru-RU");
            
            // init auth early so it's available during form load
            try {
                _auth = gcnew Authentication();
            } catch (System::Exception^ ex) {
                MessageBox::Show("������ ������������� ������ ��������������: " + ex->Message, "������", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
            
            InitializeComponent();
            
            _identifiersList = gcnew List<String^>();

            LockTabs(true); // �������������� ����������
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
        // ���� � ������ ��������������
        bool _isAuthenticated;
        Authentication^ _auth;

        // �������� TabControl
        System::Windows::Forms::TabControl^  tabControl1;

        // ������� (������ ����� ��� ���� TabPage)
        System::Windows::Forms::TabPage^  tabLogin;
        System::Windows::Forms::TabPage^  tabEncryption;
        System::Windows::Forms::TabPage^  tabLanguage;
        System::Windows::Forms::TabPage^  tabEuler;
        System::Windows::Forms::TabPage^  tabSimpleList;
        // ������� tabPolish �������, ��� ��� ��� �� ����� �� �������
        System::Windows::Forms::TabPage^  tabMultiFunction;
        System::Windows::Forms::TabPage^  tabUsers;

        // ���������� ��� ������� "�����������" (tabLogin)
        System::Windows::Forms::Label^  lblUsername;
        System::Windows::Forms::TextBox^  txtUsername;
        System::Windows::Forms::Label^  lblPassword;
        System::Windows::Forms::TextBox^  txtPassword;
        System::Windows::Forms::Button^  btnLogin;
        System::Windows::Forms::Button^  btnRegister;
        System::Windows::Forms::Label^  lblLoginStatus;

        // ���������� ��� ������� "����������" (tabEncryption)
        System::Windows::Forms::Label^  lblKey;
        System::Windows::Forms::TextBox^  txtKey;
        System::Windows::Forms::Label^  lblPlainText;
        System::Windows::Forms::TextBox^  txtPlainText;
        System::Windows::Forms::Label^  lblCipherText;
        System::Windows::Forms::TextBox^  txtCipherText;
        System::Windows::Forms::Button^  btnEncrypt;
        System::Windows::Forms::Button^  btnDecrypt;
        
        // ���������� ��� ������� "����" (tabLanguage)
        System::Windows::Forms::Label^ lblSourceCode;
        System::Windows::Forms::TextBox^  txtSourceCode;
        System::Windows::Forms::Button^  btnParse;
        System::Windows::Forms::Label^ lblParseResultDisplay; // �������� ���, ����� �� ������������� � ���������� � ������
        System::Windows::Forms::TextBox^  txtParseOutput; // �������� ���, ����� �� ������ � ����� ������
        // �������������� ���������� ��� ����������, ���� ��� �� ���� �������
        System::Windows::Forms::Label^ lblInput;
        System::Windows::Forms::TextBox^ txtInput;
        System::Windows::Forms::Label^ lblGrammar;
        System::Windows::Forms::TextBox^ txtGrammar;
        System::Windows::Forms::Label^ lblParseStepsDisplay; // �������� ���
        System::Windows::Forms::TextBox^ txtParseSteps;


        // ���������� ��� ������� "Runge-Kutta 3-�� �������" (tabEuler)
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


        // ���������� ��� ������� "������� ������" (tabSimpleList)
        System::Windows::Forms::Label^ lblIdentifierName;
        System::Windows::Forms::TextBox^ txtIdentifierName;
        System::Windows::Forms::Button^ btnAddIdentifier;
        System::Windows::Forms::Button^ btnRemoveIdentifier;
        System::Windows::Forms::Button^ btnFindIdentifier;
        System::Windows::Forms::ListBox^ lstIdentifiers;
        System::Windows::Forms::Label^ lblIdentifiersCount;
        System::Windows::Forms::Label^ lblSearchResult;
        List<String^>^ _identifiersList;

        // ���������� ��� ������� "�������� �������" �������, ��� ��� ��� �� ����� �� �������


        // ���������� ��� ������� "������������� ��������" (tabMultiFunction)
        System::Windows::Forms::Label^ lblFunctionCall;
        System::Windows::Forms::TextBox^ txtFunctionCall;
        System::Windows::Forms::Button^ btnExecuteFunction;
        System::Windows::Forms::Label^ lblResultMulti;
        System::Windows::Forms::TextBox^ txtResultMulti;

        // ���������� ��� ������� "���������� ��������������" (tabUsers)
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
            
            // ������������� ���� TabPage (��� ���� ������)
            this->tabLogin = (gcnew System::Windows::Forms::TabPage());
            this->tabEncryption = (gcnew System::Windows::Forms::TabPage());
            this->tabLanguage = (gcnew System::Windows::Forms::TabPage());
            this->tabEuler = (gcnew System::Windows::Forms::TabPage());
            this->tabSimpleList = (gcnew System::Windows::Forms::TabPage());
            
            this->tabMultiFunction = (gcnew System::Windows::Forms::TabPage());
            this->tabUsers = (gcnew System::Windows::Forms::TabPage());
            
            // ������������� ����������� ��� ������� Users
            this->lvUsers = (gcnew System::Windows::Forms::ListView());
            this->btnAddUser = (gcnew System::Windows::Forms::Button());
            this->btnEditUser = (gcnew System::Windows::Forms::Button());
            this->btnDeleteUser = (gcnew System::Windows::Forms::Button());
            this->btnRefreshUsers = (gcnew System::Windows::Forms::Button());
            this->lblUsersList = (gcnew System::Windows::Forms::Label());

            // ������������� ����������� ��� tabLogin
            this->lblUsername = (gcnew System::Windows::Forms::Label());
            this->txtUsername = (gcnew System::Windows::Forms::TextBox());
            this->lblPassword = (gcnew System::Windows::Forms::Label());
            this->txtPassword = (gcnew System::Windows::Forms::TextBox());
            this->btnLogin = (gcnew System::Windows::Forms::Button());
            this->btnRegister = (gcnew System::Windows::Forms::Button());
            this->lblLoginStatus = (gcnew System::Windows::Forms::Label());
            
            // ������������� ����������� ��� ������� ����������
            this->lblKey = (gcnew System::Windows::Forms::Label());
            this->txtKey = (gcnew System::Windows::Forms::TextBox());
            this->lblPlainText = (gcnew System::Windows::Forms::Label());
            this->txtPlainText = (gcnew System::Windows::Forms::TextBox());
            this->lblCipherText = (gcnew System::Windows::Forms::Label());
            this->txtCipherText = (gcnew System::Windows::Forms::TextBox());
            this->btnEncrypt = (gcnew System::Windows::Forms::Button());
            this->btnDecrypt = (gcnew System::Windows::Forms::Button());
            
            // ������������� ����������� ��� ������� "����"
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
            
            // ������������� ����������� ��� ������� "Runge-Kutta 3-�� �������"
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
            
            // ������������� ����������� ��� ������� "������� ������"
            this->lblIdentifierName = (gcnew System::Windows::Forms::Label());
            this->txtIdentifierName = (gcnew System::Windows::Forms::TextBox());
            this->btnAddIdentifier = (gcnew System::Windows::Forms::Button());
            this->btnRemoveIdentifier = (gcnew System::Windows::Forms::Button());
            this->btnFindIdentifier = (gcnew System::Windows::Forms::Button());
            this->lstIdentifiers = (gcnew System::Windows::Forms::ListBox());
            this->lblIdentifiersCount = (gcnew System::Windows::Forms::Label());
            this->lblSearchResult = (gcnew System::Windows::Forms::Label());
            
            // ������������� ����������� ��� ������� "������������� ��������" (tabMultiFunction)
            this->lblFunctionCall = (gcnew System::Windows::Forms::Label());
            this->txtFunctionCall = (gcnew System::Windows::Forms::TextBox());
            this->btnExecuteFunction = (gcnew System::Windows::Forms::Button());
            this->lblResultMulti = (gcnew System::Windows::Forms::Label());
            this->txtResultMulti = (gcnew System::Windows::Forms::TextBox());
            
            // ������������ ���������� ��� tabControl1 � tabLogin
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
            // �����: ��������� ��� ������� � TabControl
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

            // ��������� tabLogin (��� ����, �������, ��� ��� ���������� ��������� � tabLogin->Controls)
            this->tabLogin->Text = L"�����������";
            this->tabEncryption->Text = L"���������� (������)"; // ������� �2
            this->tabLanguage->Text = L"�������"; // ������� �3 - ��������� ���������� ��������
            this->tabEuler->Text = L"��� (�����-�����)"; // ������� �1
            this->tabSimpleList->Text = L"���-������� (ID)"; // ������� �4 - ���-���������
            // ������� �������� ������� �������
            this->tabMultiFunction->Text = L"�����. �������������"; // ������� �6 - ������
            this->tabUsers->Text = L"������������";
            
            // ��������� ListView �������������
            this->lvUsers->Dock = System::Windows::Forms::DockStyle::None;
            this->lvUsers->Location = System::Drawing::Point(10, 40);
            this->lvUsers->Name = L"lvUsers";
            this->lvUsers->Size = System::Drawing::Size(650, 300);
            this->lvUsers->View = System::Windows::Forms::View::Details;
            this->lvUsers->FullRowSelect = true;
            this->lvUsers->GridLines = true;
            this->tabUsers->Controls->Add(this->lvUsers);
            
            // ��������� ������ ���������� ��������������
            this->btnAddUser->Location = System::Drawing::Point(10, 350);
            this->btnAddUser->Name = L"btnAddUser";
            this->btnAddUser->Size = System::Drawing::Size(120, 30);
            this->btnAddUser->Text = L"��������";
            this->btnAddUser->UseVisualStyleBackColor = true;
            this->btnAddUser->Click += gcnew System::EventHandler(this, &Form1::btnAddUser_Click);
            this->tabUsers->Controls->Add(this->btnAddUser);
            
            this->btnEditUser->Location = System::Drawing::Point(140, 350);
            this->btnEditUser->Name = L"btnEditUser";
            this->btnEditUser->Size = System::Drawing::Size(120, 30);
            this->btnEditUser->Text = L"�������������";
            this->btnEditUser->UseVisualStyleBackColor = true;
            this->btnEditUser->Click += gcnew System::EventHandler(this, &Form1::btnEditUser_Click);
            this->tabUsers->Controls->Add(this->btnEditUser);
            
            this->btnDeleteUser->Location = System::Drawing::Point(270, 350);
            this->btnDeleteUser->Name = L"btnDeleteUser";
            this->btnDeleteUser->Size = System::Drawing::Size(120, 30);
            this->btnDeleteUser->Text = L"�������";
            this->btnDeleteUser->UseVisualStyleBackColor = true;
            this->btnDeleteUser->Click += gcnew System::EventHandler(this, &Form1::btnDeleteUser_Click);
            this->tabUsers->Controls->Add(this->btnDeleteUser);
            
            this->btnRefreshUsers->Location = System::Drawing::Point(400, 350);
            this->btnRefreshUsers->Name = L"btnRefreshUsers";
            this->btnRefreshUsers->Size = System::Drawing::Size(120, 30);
            this->btnRefreshUsers->Text = L"��������";
            this->btnRefreshUsers->UseVisualStyleBackColor = true;
            this->btnRefreshUsers->Click += gcnew System::EventHandler(this, &Form1::btnRefreshUsers_Click);
            this->tabUsers->Controls->Add(this->btnRefreshUsers);
            
            this->lblUsersList->AutoSize = true;
            this->lblUsersList->Location = System::Drawing::Point(10, 15);
            this->lblUsersList->Name = L"lblUsersList";
            this->lblUsersList->Size = System::Drawing::Size(200, 20);
            this->lblUsersList->Text = L"������ �������������:";
            this->tabUsers->Controls->Add(this->lblUsersList);

            // ��������� ��� ������� �����������
            this->lblUsername->AutoSize = true;
            this->lblUsername->Location = System::Drawing::Point(250, 100);
            this->lblUsername->Name = L"lblUsername";
            this->lblUsername->Text = L"�����:";
            this->tabLogin->Controls->Add(this->lblUsername);
            
            this->txtUsername->Location = System::Drawing::Point(320, 97);
            this->txtUsername->Name = L"txtUsername";
            this->txtUsername->Size = System::Drawing::Size(150, 20);
            this->tabLogin->Controls->Add(this->txtUsername);
            
            this->lblPassword->AutoSize = true;
            this->lblPassword->Location = System::Drawing::Point(250, 130);
            this->lblPassword->Name = L"lblPassword";
            this->lblPassword->Text = L"������:";
            this->tabLogin->Controls->Add(this->lblPassword);
            
            this->txtPassword->Location = System::Drawing::Point(320, 127);
            this->txtPassword->Name = L"txtPassword";
            this->txtPassword->PasswordChar = '*';
            this->txtPassword->Size = System::Drawing::Size(150, 20);
            this->tabLogin->Controls->Add(this->txtPassword);
            
            this->btnLogin->Location = System::Drawing::Point(320, 160);
            this->btnLogin->Name = L"btnLogin";
            this->btnLogin->Size = System::Drawing::Size(75, 23);
            this->btnLogin->Text = L"�����";
            this->btnLogin->UseVisualStyleBackColor = true;
            this->btnLogin->Click += gcnew System::EventHandler(this, &Form1::btnLogin_Click);
            this->tabLogin->Controls->Add(this->btnLogin);
            
            this->btnRegister->Location = System::Drawing::Point(400, 160);
            this->btnRegister->Name = L"btnRegister";
            this->btnRegister->Size = System::Drawing::Size(90, 23);
            this->btnRegister->Text = L"�����������";
            this->btnRegister->UseVisualStyleBackColor = true;
            this->btnRegister->Click += gcnew System::EventHandler(this, &Form1::btnRegister_Click);
            this->tabLogin->Controls->Add(this->btnRegister);
            
            this->lblLoginStatus->AutoSize = true;
            this->lblLoginStatus->Location = System::Drawing::Point(320, 200);
            this->lblLoginStatus->Name = L"lblLoginStatus";
            this->lblLoginStatus->Text = L"";
            this->tabLogin->Controls->Add(this->lblLoginStatus);
            
            // ��������� ��� ������� ����������
            this->lblKey->AutoSize = true;
            this->lblKey->Location = System::Drawing::Point(20, 20);
            this->lblKey->Name = L"lblKey";
            this->lblKey->Text = L"����:";
            this->tabEncryption->Controls->Add(this->lblKey);
            
            this->txtKey->Location = System::Drawing::Point(100, 20);
            this->txtKey->Name = L"txtKey";
            this->txtKey->Size = System::Drawing::Size(300, 20);
            this->tabEncryption->Controls->Add(this->txtKey);
            
            this->lblPlainText->AutoSize = true;
            this->lblPlainText->Location = System::Drawing::Point(20, 50);
            this->lblPlainText->Name = L"lblPlainText";
            this->lblPlainText->Text = L"�������� �����:";
            this->tabEncryption->Controls->Add(this->lblPlainText);
            
            this->txtPlainText->Location = System::Drawing::Point(20, 70);
            this->txtPlainText->Multiline = true;
            this->txtPlainText->Name = L"txtPlainText";
            this->txtPlainText->Size = System::Drawing::Size(700, 100);
            this->tabEncryption->Controls->Add(this->txtPlainText);
            
            this->lblCipherText->AutoSize = true;
            this->lblCipherText->Location = System::Drawing::Point(20, 190);
            this->lblCipherText->Name = L"lblCipherText";
            this->lblCipherText->Text = L"������������� �����:";
            this->tabEncryption->Controls->Add(this->lblCipherText);
            
            this->txtCipherText->Location = System::Drawing::Point(20, 210);
            this->txtCipherText->Multiline = true;
            this->txtCipherText->Name = L"txtCipherText";
            this->txtCipherText->Size = System::Drawing::Size(700, 100);
            this->tabEncryption->Controls->Add(this->txtCipherText);
            
            this->btnEncrypt->Location = System::Drawing::Point(20, 320);
            this->btnEncrypt->Name = L"btnEncrypt";
            this->btnEncrypt->Size = System::Drawing::Size(120, 30);
            this->btnEncrypt->Text = L"�����������";
            this->btnEncrypt->UseVisualStyleBackColor = true;
            this->btnEncrypt->Click += gcnew System::EventHandler(this, &Form1::btnEncrypt_Click);
            this->tabEncryption->Controls->Add(this->btnEncrypt);
            
            this->btnDecrypt->Location = System::Drawing::Point(150, 320);
            this->btnDecrypt->Name = L"btnDecrypt";
            this->btnDecrypt->Size = System::Drawing::Size(120, 30);
            this->btnDecrypt->Text = L"������������";
            this->btnDecrypt->UseVisualStyleBackColor = true;
            this->btnDecrypt->Click += gcnew System::EventHandler(this, &Form1::btnDecrypt_Click);
            this->tabEncryption->Controls->Add(this->btnDecrypt);
            
            // ��������� ��� ������� ���� (������)
            this->lblSourceCode->AutoSize = true;
            this->lblSourceCode->Location = System::Drawing::Point(20, 20);
            this->lblSourceCode->Name = L"lblSourceCode";
            this->lblSourceCode->Text = L"�������� ���:";
            this->tabLanguage->Controls->Add(this->lblSourceCode);
            
            this->txtSourceCode->Location = System::Drawing::Point(20, 40);
            this->txtSourceCode->Multiline = true;
            this->txtSourceCode->Name = L"txtSourceCode";
            this->txtSourceCode->Size = System::Drawing::Size(700, 150);
            this->tabLanguage->Controls->Add(this->txtSourceCode);
            
            this->btnParse->Location = System::Drawing::Point(20, 200);
            this->btnParse->Name = L"btnParse";
            this->btnParse->Size = System::Drawing::Size(120, 30);
            this->btnParse->Text = L"�������";
            this->btnParse->UseVisualStyleBackColor = true;
            this->btnParse->Click += gcnew System::EventHandler(this, &Form1::btnParse_Click);
            this->tabLanguage->Controls->Add(this->btnParse);
            
            this->lblParseResultDisplay->AutoSize = true;
            this->lblParseResultDisplay->Location = System::Drawing::Point(20, 240);
            this->lblParseResultDisplay->Name = L"lblParseResultDisplay";
            this->lblParseResultDisplay->Text = L"���������:";
            this->tabLanguage->Controls->Add(this->lblParseResultDisplay);
            
            this->txtParseOutput->Location = System::Drawing::Point(20, 260);
            this->txtParseOutput->Multiline = true;
            this->txtParseOutput->Name = L"txtParseOutput";
            this->txtParseOutput->Size = System::Drawing::Size(700, 150);
            this->tabLanguage->Controls->Add(this->txtParseOutput);
            
            // ��������� �������� �������� ��� ������ �������
            // ������� ���
            this->lblMatrixA_ODE->AutoSize = true;
            this->lblMatrixA_ODE->Location = System::Drawing::Point(20, 20);
            this->lblMatrixA_ODE->Name = L"lblMatrixA_ODE";
            this->lblMatrixA_ODE->Text = L"������� A:";
            this->tabEuler->Controls->Add(this->lblMatrixA_ODE);
            
            this->txtMatrixA_ODE->Location = System::Drawing::Point(100, 20);
            this->txtMatrixA_ODE->Name = L"txtMatrixA_ODE";
            this->txtMatrixA_ODE->Size = System::Drawing::Size(300, 20);
            this->tabEuler->Controls->Add(this->txtMatrixA_ODE);
            
            this->lblInitialValue_ODE->AutoSize = true;
            this->lblInitialValue_ODE->Location = System::Drawing::Point(20, 50);
            this->lblInitialValue_ODE->Name = L"lblInitialValue_ODE";
            this->lblInitialValue_ODE->Text = L"��������� ��������:";
            this->tabEuler->Controls->Add(this->lblInitialValue_ODE);
            
            this->txtInitialValue_ODE->Location = System::Drawing::Point(150, 50);
            this->txtInitialValue_ODE->Name = L"txtInitialValue_ODE";
            this->txtInitialValue_ODE->Size = System::Drawing::Size(100, 20);
            this->tabEuler->Controls->Add(this->txtInitialValue_ODE);
            
            this->lblTimeRange_ODE->AutoSize = true;
            this->lblTimeRange_ODE->Location = System::Drawing::Point(20, 80);
            this->lblTimeRange_ODE->Name = L"lblTimeRange_ODE";
            this->lblTimeRange_ODE->Text = L"��������� ��������:";
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
            this->lblTimeStep_ODE->Text = L"���:";
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
            
            // ������� ������� ������
            this->lblIdentifierName->AutoSize = true;
            this->lblIdentifierName->Location = System::Drawing::Point(20, 20);
            this->lblIdentifierName->Name = L"lblIdentifierName";
            this->lblIdentifierName->Text = L"��� ��������������:";
            this->tabSimpleList->Controls->Add(this->lblIdentifierName);
            
            this->txtIdentifierName->Location = System::Drawing::Point(150, 20);
            this->txtIdentifierName->Name = L"txtIdentifierName";
            this->txtIdentifierName->Size = System::Drawing::Size(200, 20);
            this->tabSimpleList->Controls->Add(this->txtIdentifierName);
            
            this->btnAddIdentifier->Location = System::Drawing::Point(20, 50);
            this->btnAddIdentifier->Name = L"btnAddIdentifier";
            this->btnAddIdentifier->Size = System::Drawing::Size(100, 30);
            this->btnAddIdentifier->Text = L"��������";
            this->btnAddIdentifier->UseVisualStyleBackColor = true;
            this->btnAddIdentifier->Click += gcnew System::EventHandler(this, &Form1::btnAddIdentifier_Click);
            this->tabSimpleList->Controls->Add(this->btnAddIdentifier);
            
            this->btnRemoveIdentifier->Location = System::Drawing::Point(130, 50);
            this->btnRemoveIdentifier->Name = L"btnRemoveIdentifier";
            this->btnRemoveIdentifier->Size = System::Drawing::Size(100, 30);
            this->btnRemoveIdentifier->Text = L"�������";
            this->btnRemoveIdentifier->UseVisualStyleBackColor = true;
            this->btnRemoveIdentifier->Click += gcnew System::EventHandler(this, &Form1::btnRemoveIdentifier_Click);
            this->tabSimpleList->Controls->Add(this->btnRemoveIdentifier);
            
            this->btnFindIdentifier->Location = System::Drawing::Point(240, 50);
            this->btnFindIdentifier->Name = L"btnFindIdentifier";
            this->btnFindIdentifier->Size = System::Drawing::Size(100, 30);
            this->btnFindIdentifier->Text = L"�����";
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
            this->lblIdentifiersCount->Text = L"����� ���������������: 0";
            this->tabSimpleList->Controls->Add(this->lblIdentifiersCount);
            
            this->lblSearchResult->AutoSize = true;
            this->lblSearchResult->Location = System::Drawing::Point(20, 340);
            this->lblSearchResult->Name = L"lblSearchResult";
            this->lblSearchResult->Text = L"";
            this->tabSimpleList->Controls->Add(this->lblSearchResult);
            
            // ��� ��� ������� "�������� �������" ������, ��� ��� �� ��������� �� �������
            
            // ������� ������������� ��������
            this->lblFunctionCall->AutoSize = true;
            this->lblFunctionCall->Location = System::Drawing::Point(20, 20);
            this->lblFunctionCall->Name = L"lblFunctionCall";
            this->lblFunctionCall->Text = L"����� �������:";
            this->tabMultiFunction->Controls->Add(this->lblFunctionCall);
            
            this->txtFunctionCall->Location = System::Drawing::Point(20, 40);
            this->txtFunctionCall->Name = L"txtFunctionCall";
            this->txtFunctionCall->Size = System::Drawing::Size(400, 20);
            this->tabMultiFunction->Controls->Add(this->txtFunctionCall);
            
            this->btnExecuteFunction->Location = System::Drawing::Point(20, 70);
            this->btnExecuteFunction->Name = L"btnExecuteFunction";
            this->btnExecuteFunction->Size = System::Drawing::Size(120, 30);
            this->btnExecuteFunction->Text = L"���������";
            this->btnExecuteFunction->UseVisualStyleBackColor = true;
            this->btnExecuteFunction->Click += gcnew System::EventHandler(this, &Form1::btnExecuteFunction_Click);
            this->tabMultiFunction->Controls->Add(this->btnExecuteFunction);
            
            this->lblResultMulti->AutoSize = true;
            this->lblResultMulti->Location = System::Drawing::Point(20, 110);
            this->lblResultMulti->Name = L"lblResultMulti";
            this->lblResultMulti->Text = L"���������:";
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
            this->Text = L"�������� ������ �� ������������";
            this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
            
            this->tabControl1->ResumeLayout(false);
            this->tabLogin->ResumeLayout(false); this->tabLogin->PerformLayout();
            this->tabEncryption->ResumeLayout(false); this->tabEncryption->PerformLayout();
            this->tabLanguage->ResumeLayout(false); this->tabLanguage->PerformLayout();
            this->tabEuler->ResumeLayout(false); this->tabEuler->PerformLayout();
            this->tabSimpleList->ResumeLayout(false); this->tabSimpleList->PerformLayout();
            // ������� ����� ResumeLayout ��� tabPolish
            this->tabMultiFunction->ResumeLayout(false); this->tabMultiFunction->PerformLayout();
            this->tabUsers->ResumeLayout(false); this->tabUsers->PerformLayout();
            this->ResumeLayout(false);
        }
#pragma endregion

    // --- ���������� ��������������� ������� � ������������ ������� ---
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
        // ����������� ��� �������� ������� �������, ��� ��� ��� �� ����� �� �������
        System::Void btnExecuteFunction_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void btnAddUser_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void btnEditUser_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void btnDeleteUser_Click(System::Object^  sender, System::EventArgs^  e);
        System::Void btnRefreshUsers_Click(System::Object^  sender, System::EventArgs^  e);
        
        // --- ��������������� ����� ��� ����������/������������� ������� --- 
        void LockTabs(bool lockState);
        
    }; // ����� ������ Form1
} // ����� namespace cursach

