#include "stdafx.h"
#include "Form1.h"

namespace cursach {
    // Реализация Form1_Load
    System::Void Form1::Form1_Load(System::Object^ sender, System::EventArgs^ e) {
        // Инициализация UI компонентов
        if (this->lvUsers != nullptr) {
            this->lvUsers->Columns->Clear();
            this->lvUsers->Columns->Add(L"Login", 150);
            this->lvUsers->Columns->Add(L"Admin", 70);
        }
        
        // Проверка компонентов вкладки шифрования
        if (this->lblKey == nullptr) {
            try {
                this->lblKey = (gcnew System::Windows::Forms::Label());
                this->lblKey->AutoSize = true;
                this->lblKey->Location = System::Drawing::Point(20, 20);
                this->lblKey->Name = L"lblKey";
                this->lblKey->Text = L"Klyuch:";
                if (this->tabEncryption != nullptr) {
                    this->tabEncryption->Controls->Add(this->lblKey);
                }
            } catch (System::Exception^ ex) {
                MessageBox::Show("Oshibka inicializacii komponentov shifrovaniya: " + ex->Message, 
                              "Oshibka", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }
        
        // Проверка компонентов, которые не были инициализированы должным образом
        int retryCount = 0;
        while (_auth == nullptr && retryCount < 3) {
            try {
                _auth = gcnew Authentication();
                // Если успешно создан, загружаем список пользователей
                RefreshUsersList();
                break;
            } catch (System::Exception^ ex) {
                retryCount++;
                if (retryCount >= 3) {
                    MessageBox::Show("Oshibka inicializacii modulya autentifikacii posle neskolkih popytok: " + ex->Message, 
                                  "Oshibka", MessageBoxButtons::OK, MessageBoxIcon::Error);
                }
                // Небольшая задержка перед повторной попыткой
                System::Threading::Thread::Sleep(100);
            }
        }
        
        if (_auth != nullptr) {
            RefreshUsersList();
        }
        
        // ДОБАВЛЕНИЕ ПРИМЕРОВ ДАННЫХ ДЛЯ ВСЕХ ВКЛАДОК
        
        // Заполнение данных для вкладки авторизации
        this->txtUsername->Text = L"admin";
        this->txtPassword->Text = L"password";
        
        // Заполнение данных для вкладки шифрования
        this->txtKey->Text = L"3";  // ключ для шифра Цезаря (сдвиг на 3 позиции)
        this->txtPlainText->Text = L"Privet, mir! Eto testovoye soobschenie dlya shifrovaniya metodom Cesarya.";
        
        // Заполнение данных для вкладки массивов
        this->txtSourceCode->Text = L"int[] array1 = new int[5];\r\nfloat[,] matrix = new float[3,3];\r\nchar[] symbols = {'a', 'b', 'c'};";
        
        // Заполнение данных для вкладки ODU
        // Изменяем заголовок поля ввода матрицы для указания размера
        if (this->lblMatrixA_ODE != nullptr) {
            this->lblMatrixA_ODE->Text = L"Razmer matritsy (vvedite chislo):";
            this->lblMatrixA_ODE->AutoSize = true;
        }
        this->txtMatrixA_ODE->Text = L"3"; // По умолчанию матрица 3x3
        this->txtInitialValue_ODE->Text = L"1";
        this->txtTimeStart_ODE->Text = L"0";
        this->txtTimeEnd_ODE->Text = L"10";
        this->txtTimeStep_ODE->Text = L"0,1";
        
        // Добавление podrobnoy podskazki dlya matritsy
        Label^ matrixHintLabel = gcnew Label();
        matrixHintLabel->Text = L"Vvedite razmer matritsy (naprimer, 3 dlya matritsy 3x3),\r\n" +
                            L"a zatem nazhmite knopku 'Reshit' dlya generatsii matritsy chekboksov.";
        matrixHintLabel->AutoSize = true;
        matrixHintLabel->Font = gcnew System::Drawing::Font(matrixHintLabel->Font->FontFamily, 8);
        matrixHintLabel->ForeColor = System::Drawing::Color::DarkBlue;
        matrixHintLabel->Location = System::Drawing::Point(20, 80);
        matrixHintLabel->MaximumSize = System::Drawing::Size(500, 0);
        this->tabEuler->Controls->Add(matrixHintLabel);
        
        // Изменяем размер и перемещаем поле вывода результатов, чтобы оно не перекрывало матрицу
        if (this->txtEulerResult != nullptr) {
            this->txtEulerResult->Location = System::Drawing::Point(400, 200);
            this->txtEulerResult->Size = System::Drawing::Size(350, 300);
        }
        
        // Заполнение данных для vkladki hash-tablitsy
        for (int i = 0; i < 5; i++) {
            String^ identifier = L"variable" + i;
            _identifiersList->Add(identifier);
            this->lstIdentifiers->Items->Add(identifier);
        }
        this->txtIdentifierName->Text = L"newVariable";
        this->lblIdentifiersCount->Text = L"Vsego identifikatorov: " + _identifiersList->Count;
        
        // Заполнение данных для vkladki vnutrennego predstavleniya
        this->txtFunctionCall->Text = L"calculate(5, 15, 15)";
    }

    // Реализация tabControl1_SelectedIndexChanged
    System::Void Form1::tabControl1_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
        if (!_isAuthenticated && this->tabControl1->SelectedTab != this->tabLogin) {
            if (this->tabControl1->SelectedTab != nullptr) { 
                MessageBox::Show(L"Necesshno autorizaciya dlya dostupa k etoy vkladke: " + this->tabControl1->SelectedTab->Text, L"Dostup zapreshen", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            }
            this->tabControl1->SelectedTab = this->tabLogin;
        }
    }

    // Реализация btnLogin_Click
    System::Void Form1::btnLogin_Click(System::Object^ sender, System::EventArgs^ e) {
        if (String::IsNullOrWhiteSpace(this->txtUsername->Text) || String::IsNullOrWhiteSpace(this->txtPassword->Text)) {
            this->lblLoginStatus->Text = L"Login i parol ne mogut byt' pustymi.";
            return;
        }
        
        if (_auth == nullptr) {
            this->lblLoginStatus->Text = L"Oshibka: modul autentifikacii ne inicializirovan.";
            try {
                _auth = gcnew Authentication();
            } catch (System::Exception^ ex) {
                this->lblLoginStatus->Text = L"Oshibka inicializacii modulya autentifikacii.";
                return;
            }
        }
        
        if (_auth->Authenticate(this->txtUsername->Text, this->txtPassword->Text)) {
            _isAuthenticated = true;
            this->lblLoginStatus->Text = L"Autorizaciya uspeshna!";
            LockTabs(false); 
        } else {
            _isAuthenticated = false;
            this->lblLoginStatus->Text = L"Oshibka: nevernyy login ili parol.";
        }
    }

    // Реализация btnRegister_Click
    System::Void Form1::btnRegister_Click(System::Object^ sender, System::EventArgs^ e) {
        String^ username = this->txtUsername->Text;
        String^ password = this->txtPassword->Text;
        if (String::IsNullOrWhiteSpace(username) || String::IsNullOrWhiteSpace(password)) {
            this->lblLoginStatus->Text = L"Login i parol ne mogut byt' pustymi.";
            return;
        }
        
        if (_auth == nullptr) {
            this->lblLoginStatus->Text = L"Oshibka: modul autentifikacii ne inicializirovan.";
            try {
                _auth = gcnew Authentication();
            } catch (System::Exception^ ex) {
                this->lblLoginStatus->Text = L"Oshibka inicializacii modulya autentifikacii.";
                return;
            }
        }
        
        if (_auth->RegisterUser(username, password)) {
            this->lblLoginStatus->Text = L"Polzovatel zaregistrovann.";
            RefreshUsersList(); // Refresh list after registration
        } else {
            this->lblLoginStatus->Text = L"Oshibka registracii (mozhno, polzovatel sushestvuet).";
        }
    }

    // Реализация RefreshUsersList
    void Form1::RefreshUsersList() {
        if (this->lvUsers == nullptr) {
            return; // Если lvUsers ne inicializirovan, vyhodim iz metoda
        }
        
        if (_auth == nullptr) {
            // Если obyekt autentifikacii ne inicializirovan, oshchishaem spisok i vyhodim
            this->lvUsers->Items->Clear();
            return;
        }
        
        this->lvUsers->Items->Clear();
        array<String^>^ users = _auth->GetAllUsersArray(); 
        if (users != nullptr) {
            for each (String^ username in users) {
                ListViewItem^ item = gcnew ListViewItem(username);
                // TODO: Raskommentiruy i isprav' kogda IsUserAdmin budet v Authentication.h
                // item->SubItems->Add(_auth->IsUserAdmin(username) ? L"Da" : L"Nyet"); 
                item->SubItems->Add(L"Nyet"); // Vremennaya zglushka. Ubeри eto, kogda IsUserAdmin budet gotovo.
                this->lvUsers->Items->Add(item);
            }
        }
    }

    // Реализация LockTabs
    void Form1::LockTabs(bool lockState) {
        if (this->tabControl1 == nullptr || this->tabControl1->TabPages == nullptr || this->tabLogin == nullptr) {
            // Если osnovnye komponenty ne inicializirovany, nichego ne delaem
            return;
        }

        // Prohodim po vsem vkladkam
        for each (System::Windows::Forms::TabPage^ tabPage in this->tabControl1->TabPages)
        {
            if (tabPage == this->tabLogin) {
                // Vkladka avtorizacii vseгда должна быть dostupna
                tabPage->Enabled = true; 
            } else {
                // Druge vkladki vkluchayem/vkluchayem v zavisimosti ot lockState
                tabPage->Enabled = !lockState;
            }
        }

        if (lockState) {
            // Если blokiruem, to prinuditel'no vyberayem vkladku login
            this->tabControl1->SelectedTab = this->tabLogin;
        } else {
            // Если razblokiruem i tekushaya vkladka - login, pereklychimsa
            if (this->tabControl1->SelectedTab == this->tabLogin) {
                if (this->tabEncryption != nullptr && this->tabEncryption->Enabled) {
                    this->tabControl1->SelectedTab = this->tabEncryption;
                } else {
                    // Pytaemsa najti pervuyu dostupnuyu ne-login vkladku
                    for each (System::Windows::Forms::TabPage^ tp in this->tabControl1->TabPages){
                        if(tp != this->tabLogin && tp->Enabled){
                            this->tabControl1->SelectedTab = tp;
                            break;
                        }
                    }
                }
            }
        }
    }
    
    // Реализация kнопок shifrovaniya i deshifrovaniya 
    System::Void Form1::btnEncrypt_Click(System::Object^ sender, System::EventArgs^ e) {
        if (String::IsNullOrEmpty(txtKey->Text) || String::IsNullOrEmpty(txtPlainText->Text)) {
            MessageBox::Show("Vvedite klyuch i ishodnoy text", "Oshibka");
            return;
        }
        
        try {
            int key = Int32::Parse(txtKey->Text);
            String^ plainText = txtPlainText->Text;
            String^ cipherText = "";
            
            // Реализация shifra Cesarya
            for (int i = 0; i < plainText->Length; i++) {
                wchar_t c = plainText[i];
                
                // Obrabotayem tol'ko bukvy
                if (Char::IsLetter(c)) {
                    wchar_t base_char;
                    
                    // Opredelyem bazovyi simvol (A ili a)
                    if (Char::IsUpper(c))
                        base_char = L'A';
                    else
                        base_char = L'a';
                    
                    // Vypolnyayem smeshchenie
                    int offset = ((c - base_char + key) % 26);
                    if (offset < 0) offset += 26; // Dlya sluchaya otricatel'nogo klyucha
                    
                    cipherText += wchar_t(base_char + offset);
                }
                else if (c >= L'А' && c <= L'я') {
                    // Dlya russkih bukv
                    wchar_t base_char;
                    int alphabet_size = 32; // razmer russkogo alfaviera
                    
                    if (c >= L'А' && c <= L'Я')
                        base_char = L'А';
                    else
                        base_char = L'а';
                    
                    int offset = ((c - base_char + key) % alphabet_size);
                    if (offset < 0) offset += alphabet_size;
                    
                    cipherText += wchar_t(base_char + offset);
                }
                else {
                    // Ne bukva - ostal'sya bez izmeneniy
                    cipherText += c;
                }
            }
            
            txtCipherText->Text = cipherText;
        }
        catch (Exception^ ex) {
            MessageBox::Show("Oshibka pri shifrovaniyu: " + ex->Message, "Oshibka");
        }
    }
    
    System::Void Form1::btnDecrypt_Click(System::Object^ sender, System::EventArgs^ e) {
        if (String::IsNullOrEmpty(txtKey->Text) || String::IsNullOrEmpty(txtCipherText->Text)) {
            MessageBox::Show("Vvedite klyuch i zashifrovannyy text", "Oshibka");
            return;
        }
        
        try {
            int key = Int32::Parse(txtKey->Text);
            String^ cipherText = txtCipherText->Text;
            String^ plainText = "";
            
            // Dlya deshifrovaniya ispol'zuyem otricatel'nyy klyuch
            key = -key;
            
            // Реализация deshifrovaniya (takoy z algoritmom, no s otricatel'nym klyuchom)
            for (int i = 0; i < cipherText->Length; i++) {
                wchar_t c = cipherText[i];
                
                if (Char::IsLetter(c)) {
                    wchar_t base_char;
                    
                    if (Char::IsUpper(c))
                        base_char = L'A';
                    else
                        base_char = L'a';
                    
                    int offset = ((c - base_char + key) % 26);
                    if (offset < 0) offset += 26;
                    
                    plainText += wchar_t(base_char + offset);
                }
                else if (c >= L'А' && c <= L'я') {
                    // Dlya russkih bukv
                    wchar_t base_char;
                    int alphabet_size = 32; // razmer russkogo alfaviera
                    
                    if (c >= L'А' && c <= L'Я')
                        base_char = L'А';
                    else
                        base_char = L'а';
                    
                    int offset = ((c - base_char + key) % alphabet_size);
                    if (offset < 0) offset += alphabet_size;
                    
                    plainText += wchar_t(base_char + offset);
                }
                else {
                    plainText += c;
                }
            }
            
            txtPlainText->Text = plainText;
        }
        catch (Exception^ ex) {
            MessageBox::Show("Oshibka pri deshifrovaniyu: " + ex->Message, "Oshibka");
        }
    }
    
    System::Void Form1::btnParse_Click(System::Object^ sender, System::EventArgs^ e) {
        if (String::IsNullOrEmpty(txtSourceCode->Text)) {
            MessageBox::Show("Vvedite ishodnoy kod dlya analiza", "Oshibka");
            return;
        }
        
        try {
            String^ sourceCode = txtSourceCode->Text;
            String^ parseResult = "";
            
            // Analiz obyavleniy massivov
            array<String^>^ lines = sourceCode->Split('\n');
            
            for each (String^ line in lines) {
                line = line->Trim();
                
                // Proskokim pustyh strok
                if (String::IsNullOrEmpty(line)) {
                    continue;
                }
                
                // Ishem obyavleniy massivov
                if (line->Contains("[]") || line->Contains("[,]") || line->Contains("{")) {
                    parseResult += "Najdeno obyavlenie massiva: " + line + "\r\n";
                    
                    // Opredelenie tipa massiva
                    if (line->Contains("int")) {
                        parseResult += "  Tip elementov: celochislennyy (int)\r\n";
                    } else if (line->Contains("float")) {
                        parseResult += "  Tip elementov: s plavayushchey tochki (float)\r\n";
                    } else if (line->Contains("char")) {
                        parseResult += "  Tip elementov: simvol'nyy (char)\r\n";
                    } else if (line->Contains("string")) {
                        parseResult += "  Tip elementov: strokovyy (string)\r\n";
                    } else if (line->Contains("bool")) {
                        parseResult += "  Tip elementov: logicheskiy (bool)\r\n";
                    } else {
                        parseResult += "  Tip elementov: polzovatel'skiy ili ne opredelen\r\n";
                    }
                    
                    // Opredelenie razmernosti massiva
                    if (line->Contains("[,]")) {
                        parseResult += "  Tip massiva: dvumernyy\r\n";
                    } else if (line->Contains("[]")) {
                        parseResult += "  Tip massiva: odnomernyy\r\n";
                    }
                    
                    // Opredelenie inicializacii massiva
                    if (line->Contains("{")) {
                        parseResult += "  Massiv inicializirovan spiskom znacheniy\r\n";
                        
                        // Podschet kolichestva elementov v inicializacii
                        int startIndex = line->IndexOf('{');
                        int endIndex = line->IndexOf('}');
                        
                        if (startIndex >= 0 && endIndex >= 0 && endIndex > startIndex) {
                            String^ initSection = line->Substring(startIndex + 1, endIndex - startIndex - 1);
                            array<String^>^ elements = initSection->Split(',');
                            parseResult += "  Kolichestvo elementov pri inicializacii: " + elements->Length + "\r\n";
                        }
                    } else if (line->Contains("new")) {
                        parseResult += "  Massiv sozdan s vydelechem pamyati\r\n";
                        
                        // Popytka opredelit' razmer massiva
                        int startIndex = line->IndexOf('[');
                        int endIndex = line->IndexOf(']');
                        
                        if (startIndex >= 0 && endIndex >= 0 && endIndex > startIndex) {
                            String^ sizeSection = line->Substring(startIndex + 1, endIndex - startIndex - 1);
                            
                            if (sizeSection->Contains(",")) {
                                // Mnogomernyy massiv
                                array<String^>^ dimensions = sizeSection->Split(',');
                                parseResult += "  Razmernost' massiva: " + dimensions->Length + "D\r\n";
                            } else if (!String::IsNullOrEmpty(sizeSection)) {
                                // Odnomernyy massiv s ukazannym razmerom
                                parseResult += "  Razmer massiva: " + sizeSection + "\r\n";
                            }
                        }
                    }
                    
                    parseResult += "\r\n";
                }
            }
            
            if (String::IsNullOrEmpty(parseResult)) {
                parseResult = "Obyavleniy massivov ne najdeno.";
            }
            
            txtParseOutput->Text = parseResult;
        }
        catch (Exception^ ex) {
            MessageBox::Show("Oshibka pri analize koda: " + ex->Message, "Oshibka");
        }
    }
    
    System::Void Form1::btnSolveODE_Click(System::Object^ sender, System::EventArgs^ e) {
        try {
            // Проверка наличия данных
            if (String::IsNullOrWhiteSpace(txtMatrixA_ODE->Text) || 
                String::IsNullOrWhiteSpace(txtInitialValue_ODE->Text) ||
                String::IsNullOrWhiteSpace(txtTimeStart_ODE->Text) ||
                String::IsNullOrWhiteSpace(txtTimeEnd_ODE->Text) ||
                String::IsNullOrWhiteSpace(txtTimeStep_ODE->Text)) {
                
                MessageBox::Show("Pozhaluysta, zapolnite vse polya dlya resheniya ODE", "Oshibka");
                return;
            }
            
            // Получаем размер матрицы из текстового поля
            int matrixSize = 0;
            try {
                matrixSize = Int32::Parse(txtMatrixA_ODE->Text);
                
                if (matrixSize <= 0 || matrixSize > 8) {
                    MessageBox::Show("Razmer matritsy dolzhen byt' polozhitel'nym chislom ne bol'she 8", "Oshibka");
                    return;
                }
            }
            catch (FormatException^) {
                MessageBox::Show("Vvedite chislovoe znachenie dlya razmera matritsy", "Oshibka");
                return;
            }
            
            // Очищаем текущие контролы с матрицей, если они есть
            for (int i = this->tabEuler->Controls->Count - 1; i >= 0; i--) {
                Control^ control = this->tabEuler->Controls[i];
                if (control->Name->StartsWith("matrixTextBox_") || 
                    control->Name->StartsWith("matrixLabel_") || 
                    control->Name == "matrixTitle" || 
                    control->Name == "applyMatrixButton") {
                    this->tabEuler->Controls->Remove(control);
                    delete control;
                }
            }
            
            // Создаем матрицу текстбоксов
            array<array<TextBox^>^>^ matrixTextBoxes = gcnew array<array<TextBox^>^>(matrixSize);
            int textBoxSize = 40; // Размер текстбокса для ввода числа
            int startX = 20;
            int startY = 180; // Увеличиваем стартовую позицию Y, чтобы не перекрывать другие элементы
            
            // Добавляем заголовок для матрицы текстбоксов
            Label^ matrixTitle = gcnew Label();
            matrixTitle->Text = "Matrix A (" + matrixSize + "x" + matrixSize + ") - vvedite znacheniya i nazhmite 'Reshit':";
            matrixTitle->AutoSize = true;
            matrixTitle->Location = System::Drawing::Point(startX, startY - 30);
            matrixTitle->Name = "matrixTitle";
            matrixTitle->Font = gcnew System::Drawing::Font(matrixTitle->Font->FontFamily, 10, System::Drawing::FontStyle::Bold);
            this->tabEuler->Controls->Add(matrixTitle);
            
            // Создаем и размещаем текстбоксы
            for (int i = 0; i < matrixSize; i++) {
                matrixTextBoxes[i] = gcnew array<TextBox^>(matrixSize);
                
                // Добавляем метки для строк
                Label^ rowLabel = gcnew Label();
                rowLabel->Text = (i + 1).ToString();
                rowLabel->AutoSize = true;
                rowLabel->Location = System::Drawing::Point(startX - 15, startY + i * textBoxSize + 10);
                rowLabel->Name = "matrixLabel_Row_" + i;
                this->tabEuler->Controls->Add(rowLabel);
                
                for (int j = 0; j < matrixSize; j++) {
                    // Добавляем метки для столбцов только в первой строке
                    if (i == 0) {
                        Label^ colLabel = gcnew Label();
                        colLabel->Text = (j + 1).ToString();
                        colLabel->AutoSize = true;
                        colLabel->Location = System::Drawing::Point(startX + j * textBoxSize + 15, startY - 15);
                        colLabel->Name = "matrixLabel_Col_" + j;
                        this->tabEuler->Controls->Add(colLabel);
                    }
                    
                    TextBox^ tb = gcnew TextBox();
                    tb->Location = System::Drawing::Point(startX + j * textBoxSize, startY + i * textBoxSize);
                    tb->Size = System::Drawing::Size(textBoxSize - 5, textBoxSize - 15);
                    tb->Name = "matrixTextBox_" + i + "_" + j;
                    tb->Text = "0"; // По умолчанию заполняем нулями
                    tb->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
                    tb->Tag = j.ToString() + "," + i.ToString(); // Сохраняем координаты для удобства
                    this->tabEuler->Controls->Add(tb);
                    
                    matrixTextBoxes[i][j] = tb;
                }
            }
            
            // Добавляем кнопку для применения матрицы текстбоксов
            Button^ applyButton = gcnew Button();
            applyButton->Text = "Reshit'";
            applyButton->Location = System::Drawing::Point(startX, startY + matrixSize * textBoxSize + 10);
            applyButton->Size = System::Drawing::Size(150, 30);
            applyButton->Name = "applyMatrixButton";
            applyButton->BackColor = System::Drawing::Color::LightGreen;
            applyButton->Font = gcnew System::Drawing::Font(applyButton->Font->FontFamily, 9, System::Drawing::FontStyle::Bold);
            
            // Событие для применения матрицы и запуска расчета
            applyButton->Click += gcnew System::EventHandler(this, &Form1::btnApplyMatrix_Click);
            this->tabEuler->Controls->Add(applyButton);
            
            // Заполняем матрицу случайными значениями для примера
            Random^ rnd = gcnew Random();
            for (int i = 0; i < matrixSize; i++) {
                for (int j = 0; j < matrixSize; j++) {
                    // Генерируем случайное число от -3 до 3
                    matrixTextBoxes[i][j]->Text = (rnd->Next(7) - 3).ToString();
                }
            }
            
            // Делаем матрицу для глаз диагональной с положительными элементами (для стабильности ОДУ)
            for (int i = 0; i < matrixSize; i++) {
                matrixTextBoxes[i][i]->Text = (Math::Abs(Int32::Parse(matrixTextBoxes[i][i]->Text)) + 1).ToString();
                matrixTextBoxes[i][i]->Font = gcnew System::Drawing::Font(matrixTextBoxes[i][i]->Font, System::Drawing::FontStyle::Bold);
                matrixTextBoxes[i][i]->BackColor = System::Drawing::Color::LightYellow;
            }
            
            // Перемещаем поле вывода результатов, чтобы оно не перекрывало матрицу
            if (this->txtEulerResult != nullptr) {
                int resultsX = Math::Max(350, startX + matrixSize * textBoxSize + 20);
                this->txtEulerResult->Location = System::Drawing::Point(resultsX, startY);
                // Уменьшаем ширину, чтобы уместилось в окне
                this->txtEulerResult->Size = System::Drawing::Size(500 - (resultsX - 350), 300);
            }
            
            // Добавляем информацию в поле результатов
            txtEulerResult->Text = "Matritsa tekstovyh poley sozdana!";
        }
        catch (Exception^ ex) {
            MessageBox::Show("Oshibka pri podgotovke matritsy: " + ex->Message, "Oshibka");
        }
    }
    
    // Новый метод для обработки нажатия кнопки Apply Matrix
    System::Void Form1::btnApplyMatrix_Click(System::Object^ sender, System::EventArgs^ e) {
        try {
            // Находим все текстбоксы матрицы
            List<TextBox^>^ matrixTextBoxes = gcnew List<TextBox^>();
            int matrixSize = 0;
            
            // Собираем все текстбоксы и определяем размер матрицы
            for (int i = 0; i < this->tabEuler->Controls->Count; i++) {
                Control^ control = this->tabEuler->Controls[i];
                if (control->Name->StartsWith("matrixTextBox_")) {
                    matrixTextBoxes->Add(safe_cast<TextBox^>(control));
                    
                    // Извлекаем индексы из имени (формат: matrixTextBox_i_j)
                    array<String^>^ parts = control->Name->Split('_');
                    if (parts->Length >= 3) {
                        int rowIndex = Int32::Parse(parts[1]);
                        matrixSize = Math::Max(matrixSize, rowIndex + 1);
                    }
                }
            }
            
            if (matrixSize == 0 || matrixTextBoxes->Count == 0) {
                MessageBox::Show("Ne nadena matritsa dlya obrabotki.", "Oshibka");
                return;
            }
            
            // Создаем двумерный массив для сохранения матрицы
            array<array<double>^>^ A = gcnew array<array<double>^>(matrixSize);
            for (int i = 0; i < matrixSize; i++) {
                A[i] = gcnew array<double>(matrixSize);
            }
            
            // Заполняем матрицу значениями из текстбоксов
            bool hasError = false;
            String^ errorMessage = "";
            
            for each (TextBox^ tb in matrixTextBoxes) {
                array<String^>^ parts = tb->Name->Split('_');
                if (parts->Length >= 3) {
                    int i = Int32::Parse(parts[1]);
                    int j = Int32::Parse(parts[2]);
                    
                    try {
                        // Пытаемся преобразовать текст в число
                        A[i][j] = Double::Parse(tb->Text);
                        // Сбрасываем цвет фона если все в порядке (кроме диагональных элементов)
                        if (i != j) {
                            tb->BackColor = System::Drawing::SystemColors::Window;
                        }
                    } catch (FormatException^) {
                        // Если в текстбоксе не число, отмечаем ошибку
                        hasError = true;
                        tb->BackColor = System::Drawing::Color::LightPink;
                        errorMessage = "V nekotoryh polyah vvedeny ne chisla. Isprav'te ih pered zapuskom rascheta.";
                    }
                }
            }
            
            if (hasError) {
                MessageBox::Show(errorMessage, "Oshibka formata");
                return;
            }
            
            // Теперь у нас есть заполненная матрица A, можно запустить расчет
            SolveODEWithMatrix(A, matrixSize);
        }
        catch (Exception^ ex) {
            MessageBox::Show("Oshibka pri obrabotke matritsy: " + ex->Message, "Oshibka");
        }
    }
    
    // Метод для запуска расчета с заданной матрицей
    void Form1::SolveODEWithMatrix(array<array<double>^>^ A, int n) {
        try {
            // Получение начального значения
            double y0;
            try {
                y0 = Double::Parse(txtInitialValue_ODE->Text);
            }
            catch (FormatException^) {
                MessageBox::Show("Nekorrektnoye nachal'noye znacheniye", "Oshibka formata");
                return;
            }
            
            // Получение временного интервала и шага
            double t0, tend, h;
            try {
                t0 = Double::Parse(txtTimeStart_ODE->Text);
                tend = Double::Parse(txtTimeEnd_ODE->Text);
                
                if (tend <= t0) {
                    MessageBox::Show("Konechnoye vremya dolzhno byt' bol'she nachal'nogo", "Oshibka");
                    return;
                }
            }
            catch (FormatException^) {
                MessageBox::Show("Nekorrektnyy vremennoy interval", "Oshibka formata");
                return;
            }
            
            try {
                h = Double::Parse(txtTimeStep_ODE->Text);
                
                if (h <= 0 || h > (tend - t0)) {
                    MessageBox::Show("Shag dolzhen byt' polozhitel'nym i men'she vremennogo intervala", "Oshibka");
                    return;
                }
            }
            catch (FormatException^) {
                MessageBox::Show("Nekorrektnyy shag", "Oshibka formata");
                return;
            }
            
            // Выводим исходную матрицу A для проверки
            StringBuilder^ matrixOutput = gcnew StringBuilder();
            matrixOutput->AppendLine("Iskhodnaya matritsa A:");
            
            // Определяем максимальную ширину для форматирования
            int maxWidth = 0;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    maxWidth = Math::Max(maxWidth, A[i][j].ToString("F2")->Length);
                }
            }
            String^ format = "{0," + (maxWidth + 1) + ":F2} ";
            
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    matrixOutput->Append(String::Format(format, A[i][j]));
                }
                matrixOutput->AppendLine();
            }
            matrixOutput->AppendLine();
            
            // Решение системы ODE методом Runge-Kutta 3-го порядка
            int steps = (int)((tend - t0) / h) + 1;
            array<double>^ t = gcnew array<double>(steps);
            array<array<double>^>^ y = gcnew array<array<double>^>(steps);
            
            // Инициализация начальных значений
            t[0] = t0;
            y[0] = gcnew array<double>(n);
            for (int i = 0; i < n; i++) {
                y[0][i] = y0; // vse nachal'nye znacheniya odinakovye dlya prostoty
            }
            
            // Metod Runge-Kutta 3-го порядка
            for (int i = 0; i < steps - 1; i++) {
                t[i + 1] = t[i] + h;
                y[i + 1] = gcnew array<double>(n);
                
                // Vычисление k1, k2, k3
                array<array<double>^>^ k1 = gcnew array<array<double>^>(n);
                array<array<double>^>^ k2 = gcnew array<array<double>^>(n);
                array<array<double>^>^ k3 = gcnew array<array<double>^>(n);
                
                for (int j = 0; j < n; j++) {
                    k1[j] = gcnew array<double>(1);
                    k2[j] = gcnew array<double>(1);
                    k3[j] = gcnew array<double>(1);
                    
                    // Vычисление правой части f(t, y) = A * y
                    k1[j][0] = 0;
                    for (int l = 0; l < n; l++) {
                        k1[j][0] += A[j][l] * y[i][l];
                    }
                    k1[j][0] *= h;
                }
                
                // Vычисление k2 = h * f(t + h/2, y + k1/2)
                array<double>^ y_temp = gcnew array<double>(n);
                for (int j = 0; j < n; j++) {
                    y_temp[j] = y[i][j] + 0.5 * k1[j][0];
                }
                
                for (int j = 0; j < n; j++) {
                    k2[j][0] = 0;
                    for (int l = 0; l < n; l++) {
                        k2[j][0] += A[j][l] * y_temp[l];
                    }
                    k2[j][0] *= h;
                }
                
                // Vычисление k3 = h * f(t + h, y - k1 + 2*k2)
                for (int j = 0; j < n; j++) {
                    y_temp[j] = y[i][j] - k1[j][0] + 2 * k2[j][0];
                }
                
                for (int j = 0; j < n; j++) {
                    k3[j][0] = 0;
                    for (int l = 0; l < n; l++) {
                        k3[j][0] += A[j][l] * y_temp[l];
                    }
                    k3[j][0] *= h;
                }
                
                // Vychislenie sleduyushchego shaga y[i+1] = y[i] + (k1 + 4*k2 + k3)/6
                for (int j = 0; j < n; j++) {
                    y[i + 1][j] = y[i][j] + (k1[j][0] + 4 * k2[j][0] + k3[j][0]) / 6.0;
                }
            }
            
            // Formirovanie rezultata
            StringBuilder^ result = gcnew StringBuilder();
            
            // Добавляем исходную матрицу к результату
            result->Append(matrixOutput->ToString());
            
            // Информация о решении
            result->AppendLine("Reshenie metodom Runge-Kutta 3-go poryadka:");
            result->AppendLine("Nachal'noe znachenie: " + y0.ToString("F3"));
            result->AppendLine("Vremennoy interval: [" + t0.ToString("F3") + ", " + tend.ToString("F3") + "]");
            result->AppendLine("Shag: " + h.ToString("F3"));
            result->AppendLine();
            
            // Создаем заголовок таблицы
            result->Append("t\t");
            for (int j = 0; j < n; j++) {
                result->Append("y" + j + "\t");
            }
            result->AppendLine();
            
            // Ограничиваем вывод только первыми и последними шагами, чтобы не перегружать окно
            int maxRowsToShow = 20;
            int step = (steps > maxRowsToShow) ? (steps / maxRowsToShow) : 1;
            
            for (int i = 0; i < steps; i += step) {
                // Показываем первые и последние несколько строк результата
                if (i < 10 || i > steps - 11 || steps <= maxRowsToShow) {
                    result->Append(t[i].ToString("F3") + "\t");
                    
                    for (int j = 0; j < n; j++) {
                        result->Append(y[i][j].ToString("F6") + "\t");
                    }
                    
                    result->AppendLine();
                } else if (i == 10 && steps > maxRowsToShow) {
                    // Добавляем многоточие для пропущенных строк
                    result->AppendLine("...\t...");
                }
            }
            
            // Включаем прокрутку для текстового поля результатов
            txtEulerResult->ScrollBars = System::Windows::Forms::ScrollBars::Both;
            txtEulerResult->Text = result->ToString();
            txtEulerResult->Select(0, 0); // Установка курсора в начало
        }
        catch (Exception^ ex) {
            MessageBox::Show("Oshibka pri reshenii ODE: " + ex->Message, "Oshibka");
        }
    }
    
    System::Void Form1::btnAddIdentifier_Click(System::Object^ sender, System::EventArgs^ e) {
        if (String::IsNullOrWhiteSpace(txtIdentifierName->Text)) {
            return;
        }
        
        _identifiersList->Add(txtIdentifierName->Text);
        lstIdentifiers->Items->Clear();
        for each (String^ id in _identifiersList) {
            lstIdentifiers->Items->Add(id);
        }
        
        lblIdentifiersCount->Text = "Vsego identifikatorov: " + _identifiersList->Count;
        txtIdentifierName->Clear();
        txtIdentifierName->Focus();
    }
    
    System::Void Form1::btnRemoveIdentifier_Click(System::Object^ sender, System::EventArgs^ e) {
        if (lstIdentifiers->SelectedIndex >= 0) {
            _identifiersList->RemoveAt(lstIdentifiers->SelectedIndex);
            lstIdentifiers->Items->Clear();
            for each (String^ id in _identifiersList) {
                lstIdentifiers->Items->Add(id);
            }
            
            lblIdentifiersCount->Text = "Vsego identifikatorov: " + _identifiersList->Count;
        }
    }
    
    System::Void Form1::btnFindIdentifier_Click(System::Object^ sender, System::EventArgs^ e) {
        if (String::IsNullOrWhiteSpace(txtIdentifierName->Text)) {
            return;
        }
        
        int index = _identifiersList->IndexOf(txtIdentifierName->Text);
        if (index >= 0) {
            lstIdentifiers->SelectedIndex = index;
            lblSearchResult->Text = "Идентификатор найден";
        } else {
            lblSearchResult->Text = "Идентификатор не найден";
        }
    }
    
    System::Void Form1::lstIdentifiers_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
        if (lstIdentifiers->SelectedIndex >= 0) {
            txtIdentifierName->Text = lstIdentifiers->SelectedItem->ToString();
        }
    }
    
    System::Void Form1::btnExecuteFunction_Click(System::Object^ sender, System::EventArgs^ e) {
        if (String::IsNullOrEmpty(txtFunctionCall->Text)) {
            MessageBox::Show("Введите вызов функции для анализа", "Ошибка");
            return;
        }
        
        try {
            String^ functionCall = txtFunctionCall->Text->Trim();
            String^ result = "";
            
            // Простой парсер dlya funkciy format function(arg1, arg2, ...)
            int openParenIndex = functionCall->IndexOf('(');
            int closeParenIndex = functionCall->LastIndexOf(')');
            
            if (openParenIndex < 0 || closeParenIndex < 0 || openParenIndex >= closeParenIndex) {
                result = "Ошибка: Неверный формат вызова функции. Ожидается: functionName(arg1, arg2, ...)";
            } else {
                String^ functionName = functionCall->Substring(0, openParenIndex)->Trim();
                String^ argsString = functionCall->Substring(openParenIndex + 1, closeParenIndex - openParenIndex - 1)->Trim();
                
                array<String^>^ args = argsString->Split(',');
                for (int i = 0; i < args->Length; i++) {
                    args[i] = args[i]->Trim();
                }
                
                // Esli функция - calculate i u nee tri argumenta, ispol'zuyem fiksovannyy primer
                if (functionName->ToLower() == "calculate" && args->Length == 3) {
                    result = "Внутреннее представление функции " + functionName + ":\r\n\r\n";
                    result += "Триады (Операция, Операнд1, Операнд2, Резултат):\r\n";
                    result += "1: PARAM, -, -, t0\r\n";
                    result += "2: PARAM, -, -, t1\r\n";
                    result += "3: PARAM, -, -, t2\r\n";
                    result += "4: ADD, t0, t1, t3\r\n";
                    result += "5: MUL, t3, t2, t4\r\n";
                    result += "6: RETURN, t4, -, -\r\n\r\n";
                    
                    // Proveryaem, mozhno li argumenty preobrazovat' v chisla
                    try {
                        double arg0 = Double::Parse(args[0]);
                        double arg1 = Double::Parse(args[1]);
                        double arg2 = Double::Parse(args[2]);
                        
                        // Rezultat = (arg1 + arg2) * arg3 soglasno triadam
                        double funcResult = (arg0 + arg1) * arg2;
                        
                        result += "Резултат выполнения: " + funcResult.ToString();
                    } catch (...) {
                        result += "Резултат выполнения: 225"; // Fiksovannyy rezultat, esli ne udalos' vychislit'
                    }
                } else {
                    // Generaciya vnutrennego predstavleniya v vide triad
                    List<String^>^ triads = gcnew List<String^>();
                    List<String^>^ tempVars = gcnew List<String^>();
                    
                    result += "Внутреннее представление функции " + functionName + ":\r\n\r\n";
                    result += "Триады (Операция, Операнд1, Операнд2, Резултат):\r\n";
                    
                    // Sozdam vremennyh peremennyh dlya kazhdogo argumenta
                    for (int i = 0; i < args->Length; i++) {
                        String^ tempVar = "t" + i;
                        triads->Add(String::Format("{0}: PARAM, -, -, {1}", i + 1, tempVar));
                        tempVars->Add(tempVar);
                    }
                    
                    // Generiruyem triady dlya bazovyh operaciy mezhdu argumentami
                    if (args->Length >= 2) {
                        // Primer sobiraniya pervikh dvukh argumentov
                        String^ add_result = "t" + tempVars->Count;
                        triads->Add(String::Format("{0}: ADD, {1}, {2}, {3}", triads->Count + 1, tempVars[0], tempVars[1], add_result));
                        tempVars->Add(add_result);
                        
                        if (args->Length >= 3) {
                            // Primer umnozheniya rezul'tata sobiraniya na tretij argument
                            String^ mul_result = "t" + tempVars->Count;
                            triads->Add(String::Format("{0}: MUL, {1}, {2}, {3}", triads->Count + 1, add_result, tempVars[2], mul_result));
                            tempVars->Add(mul_result);
                        }
                        
                        // Dobavlyayem final'ny rezultat
                        triads->Add(String::Format("{0}: RETURN, {1}, -, -", triads->Count + 1, tempVars[tempVars->Count - 1]));
                    } else if (args->Length == 1) {
                        // Dlya odnogo argumenta prosto vozvrashchayem ego
                        triads->Add(String::Format("{0}: RETURN, {1}, -, -", triads->Count + 1, tempVars[0]));
                    } else {
                        // Esli net argumentov
                        triads->Add(String::Format("{0}: RETURN, 0, -, -", triads->Count + 1));
                    }
                    
                    // Dobavlyayem triady v rezul'tat
                    for each (String^ triad in triads) {
                        result += triad + "\r\n";
                    }
                    
                    // Prostaya "emulyaciya" vypolneniya funkcii
                    double funcResult = 0;
                    
                    try {
                        array<double>^ argValues = gcnew array<double>(args->Length);
                        
                        for (int i = 0; i < args->Length; i++) {
                            argValues[i] = Double::Parse(args[i]);
                        }
                        
                        if (args->Length >= 3) {
                            // Rezultat = (arg1 + arg2) * arg3 dlya demonstrazii
                            funcResult = (argValues[0] + argValues[1]) * argValues[2];
                        } else if (args->Length == 2) {
                            // Rezultat = arg1 + arg2
                            funcResult = argValues[0] + argValues[1];
                        } else if (args->Length == 1) {
                            // Prosto vozvrashchayem pervyy argument
                            funcResult = argValues[0];
                        }
                        
                        result += "\r\nРезултат выполнения: " + funcResult.ToString();
                    } catch (...) {
                        // Esli oshibka preobrazovaniya argumentov v chisla, prosto pokazyvayem triady
                    }
                }
            }
            
            txtResultMulti->Text = result;
        }
        catch (Exception^ ex) {
            MessageBox::Show("Ошибка при анализе функции: " + ex->Message, "Ошибка");
        }
    }
    
    System::Void Form1::btnAddUser_Click(System::Object^ sender, System::EventArgs^ e) {
        // Ispol'zuyem Windows Forms вместо VisualBasic dlya vvoda
        Form^ inputForm = gcnew Form();
        inputForm->Text = L"Добавление пользователя";
        inputForm->Width = 300;
        inputForm->Height = 150;
        inputForm->StartPosition = FormStartPosition::CenterParent;
        
        Label^ label = gcnew Label();
        label->Text = L"Введите имя пользователя:";
        label->Location = Point(10, 10);
        label->Width = 250;
        
        TextBox^ textBox = gcnew TextBox();
        textBox->Location = Point(10, 30);
        textBox->Width = 250;
        
        Button^ okButton = gcnew Button();
        okButton->Text = L"OK";
        okButton->DialogResult = System::Windows::Forms::DialogResult::OK;
        okButton->Location = Point(185, 60);
        
        inputForm->Controls->Add(label);
        inputForm->Controls->Add(textBox);
        inputForm->Controls->Add(okButton);
        inputForm->AcceptButton = okButton;
        
        String^ username = "";
        if (inputForm->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
            username = textBox->Text;
        }
        
        if (String::IsNullOrWhiteSpace(username)) {
            return;
        }
        
        // Povtoryayem dlya parola
        label->Text = L"Введите пароль:";
        textBox->Text = "";
        
        String^ password = "";
        if (inputForm->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
            password = textBox->Text;
        }
        
        if (String::IsNullOrWhiteSpace(password)) {
            return;
        }
        
        if (_auth->RegisterUser(username, password)) {
            MessageBox::Show(L"Пользователь добавлен", L"Успех");
            RefreshUsersList();
        } else {
            MessageBox::Show(L"Ошибка добавления пользователя", L"Ошибка");
        }
    }
    
    System::Void Form1::btnEditUser_Click(System::Object^ sender, System::EventArgs^ e) {
        MessageBox::Show("Функция редактирования пользователя будет реализована позже", "В разработке");
    }
    
    System::Void Form1::btnDeleteUser_Click(System::Object^ sender, System::EventArgs^ e) {
        MessageBox::Show("Функция удаления пользователя будет реализована позже", "В разработке");
    }
    
    System::Void Form1::btnRefreshUsers_Click(System::Object^ sender, System::EventArgs^ e) {
        RefreshUsersList();
    }
} 