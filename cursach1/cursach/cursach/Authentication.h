#pragma once

using namespace System;
using namespace System::IO;
using namespace System::Text;
using namespace System::Security::Cryptography;
using namespace System::Collections::Generic;
using namespace cursachDLL;

// класс для простой аутентификации пользователей
ref class Authentication
{
private:
    Dictionary<String^, String^>^ _users;
    String^ _currentUsername;
    bool _isAuthenticated;
    String^ _usersFilePath;
    
public:
    Authentication()
    {
        _users = gcnew Dictionary<String^, String^>();
        _currentUsername = "";
        _isAuthenticated = false;
        _usersFilePath = "users.dat";
        
        // загружаем пользователей из файла, если он существует
        if (File::Exists(_usersFilePath))
        {
            LoadUsersFromFile();
        }
        else
        {
            // добавляем тестового пользователя
            RegisterUser("admin", "password");
        }
    }
    
    // регистрация нового пользователя
    bool RegisterUser(String^ username, String^ password)
    {
        // проверяем, что пользователь с таким именем не существует
        if (_users->ContainsKey(username))
        {
            return false;
        }
        
        // вычисляем хеш пароля
        String^ passwordHash = ComputeHash(password);
        
        // добавляем пользователя
        _users->Add(username, passwordHash);
        
        // сохраняем изменения
        SaveUsersToFile();
        
        return true;
    }
    
    // аутентификация пользователя
    bool Authenticate(String^ username, String^ password)
    {
        // сбрасываем предыдущую аутентификацию
        _isAuthenticated = false;
        _currentUsername = "";
        
        // проверяем, существует ли пользователь
        if (!_users->ContainsKey(username))
        {
            return false;
        }
        
        // получаем хеш пароля
        String^ storedHash = _users[username];
        
        // вычисляем хеш введенного пароля
        String^ inputHash = ComputeHash(password);
        
        // сравниваем хеши
        if (inputHash == storedHash)
        {
            _isAuthenticated = true;
            _currentUsername = username;
            return true;
        }
        
        return false;
    }
    
    // выход пользователя
    void Logout()
    {
        _isAuthenticated = false;
        _currentUsername = "";
    }
    
    // проверка, аутентифицирован ли пользователь
    bool IsAuthenticated()
    {
        return _isAuthenticated;
    }
    
    // получение имени текущего пользователя
    String^ GetCurrentUsername()
    {
        return _currentUsername;
    }
    
    // получение пароля пользователя
    String^ GetUserPassword(String^ username)
    {
        // проверяем, существует ли пользователь
        if (!_users->ContainsKey(username))
            return "";
        
        // возвращаем маскированный пароль для безопасности
        return "********";
    }
    
    // сохранение списка пользователей в файл
    bool SaveUsersToFile()
    {
        try
        {
            // создаем текстовый файл
            StreamWriter^ writer = gcnew StreamWriter(_usersFilePath);
            
            // записываем каждого пользователя в формате "username:passwordHash"
            for each (KeyValuePair<String^, String^> pair in _users)
            {
                writer->WriteLine(pair.Key + ":" + pair.Value);
            }
            
            writer->Close();
            return true;
        }
        catch (Exception^)
        {
            return false;
        }
    }
    
    // загрузка списка пользователей из файла
    bool LoadUsersFromFile()
    {
        try
        {
            // очищаем текущий список пользователей
            _users->Clear();
            
            // открываем файл для чтения
            StreamReader^ reader = gcnew StreamReader(_usersFilePath);
            
            // читаем каждую строку и разбираем на имя пользователя и хеш пароля
            String^ line;
            while ((line = reader->ReadLine()) != nullptr)
            {
                array<String^>^ parts = line->Split(':');
                
                if (parts->Length == 2)
                {
                    String^ username = parts[0];
                    String^ passwordHash = parts[1];
                    
                    _users->Add(username, passwordHash);
                }
            }
            
            reader->Close();
            return true;
        }
        catch (Exception^)
        {
            return false;
        }
    }
    
    // получение списка имен всех пользователей
    List<String^>^ GetAllUsers()
    {
        List<String^>^ usernames = gcnew List<String^>();
        
        for each (String^ username in _users->Keys)
        {
            usernames->Add(username);
        }
        
        return usernames;
    }
    
    // удаление пользователя
    bool DeleteUser(String^ username)
    {
        // проверяем, что пользователь существует
        if (!_users->ContainsKey(username))
        {
            return false;
        }
        
        // нельзя удалить текущего пользователя
        if (_isAuthenticated && _currentUsername == username)
        {
            return false;
        }
        
        // удаляем пользователя
        _users->Remove(username);
        
        // сохраняем изменения
        SaveUsersToFile();
        
        return true;
    }
    
    // обновление пароля пользователя
    bool UpdateUserPassword(String^ username, String^ newPassword)
    {
        // проверяем, что пользователь существует
        if (!_users->ContainsKey(username))
        {
            return false;
        }
        
        // вычисляем хеш нового пароля
        String^ passwordHash = ComputeHash(newPassword);
        
        // обновляем пароль
        _users[username] = passwordHash;
        
        // сохраняем изменения
        SaveUsersToFile();
        
        return true;
    }
    
    // получение массива имен всех пользователей
    array<String^>^ GetAllUsersArray()
    {
        array<String^>^ usernames = gcnew array<String^>(_users->Count);
        _users->Keys->CopyTo(usernames, 0);
        return usernames;
    }
    
private:
    // вычисление хеша пароля (SHA256)
    String^ ComputeHash(String^ password)
    {
        SHA256^ sha256 = SHA256::Create();
        array<Byte>^ bytes = Encoding::UTF8->GetBytes(password);
        array<Byte>^ hash = sha256->ComputeHash(bytes);
        
        // конвертируем байты хеша в строку шестнадцатеричных символов
        StringBuilder^ builder = gcnew StringBuilder();
        for (int i = 0; i < hash->Length; i++)
        {
            builder->Append(hash[i].ToString("x2"));
        }
        
        return builder->ToString();
    }
};

// сохраняем совместимость с существующим кодом
namespace cursach {
    
    // простой класс для хранения информации о пользователе
    ref class User
    {
    public:
        String^ Username;
        String^ PasswordHash;
        
        User(String^ username, String^ passwordHash)
        {
            Username = username;
            PasswordHash = passwordHash;
        }
    };
    
    // класс для аутентификации пользователей
    ref class AuthenticationManager
    {
    private:
        Dictionary<String^, User^>^ _users;
        User^ _currentUser;
        Authentication^ _auth; // использовать новый класс
        
    public:
        AuthenticationManager()
        {
            _users = gcnew Dictionary<String^, User^>();
            _currentUser = nullptr;
            _auth = gcnew Authentication();
            
            // добавляем тестового пользователя
            RegisterUser("admin", "password");
        }
        
        // регистрация нового пользователя
        bool RegisterUser(String^ username, String^ password)
        {
            return _auth->RegisterUser(username, password);
        }
        
        // аутентификация пользователя
        bool Login(String^ username, String^ password)
        {
            return _auth->Authenticate(username, password);
        }
        
        // выход из системы
        void Logout()
        {
            _auth->Logout();
        }
        
        // проверка статуса аутентификации
        bool IsLoggedIn()
        {
            return _auth->IsAuthenticated();
        }
        
        // получение имени текущего пользователя
        String^ GetCurrentUsername()
        {
            return _auth->GetCurrentUsername();
        }
        
        // сохранение данных пользователей
        bool SaveUsers()
        {
            return _auth->SaveUsersToFile();
        }
        
        // получение списка всех пользователей
        List<String^>^ GetAllUsers()
        {
            return _auth->GetAllUsers();
        }
        
        // удаление пользователя
        bool DeleteUser(String^ username)
        {
            return _auth->DeleteUser(username);
        }
        
    private:
        // вычисление хеша пароля (SHA256)
        String^ ComputeHash(String^ password)
        {
            SHA256^ sha256 = SHA256::Create();
            array<Byte>^ bytes = Encoding::UTF8->GetBytes(password);
            array<Byte>^ hash = sha256->ComputeHash(bytes);
            
            // конвертируем байты хеша в строку шестнадцатеричных символов
            StringBuilder^ builder = gcnew StringBuilder();
            for (int i = 0; i < hash->Length; i++)
            {
                builder->Append(hash[i].ToString("x2"));
            }
            
            return builder->ToString();
        }
    };
} 