#pragma once

using namespace System;
using namespace System::Runtime::InteropServices;

namespace cursachDLL {
    
    // класс для шифрования методом Цезаря со сдвигом полученного кода на N бит влево, 
    // где N – последняя цифра номера зачетной книжки
    public ref class CaesarCipherLib
    {
    private:
        int _key; // ключ шифрования (количество битов для сдвига)
        
    public:
        // конструктор с заданием ключа
        CaesarCipherLib(int key)
        {
            _key = key % 8; // ограничиваем ключ до размера байта
            if (_key < 0) _key += 8; // делаем ключ положительным
        }
        
        // получение текущего ключа
        int GetKey()
        {
            return _key;
        }
        
        // установка нового ключа
        void SetKey(int key)
        {
            _key = key % 8;
            if (_key < 0) _key += 8;
        }
        
        // метод для бинарного сдвига символа вправо на n битов
        unsigned char ShiftBitsRight(unsigned char value, int shift)
        {
            // сдвигаем биты вправо с циклическим переносом
            unsigned char result = (value >> shift) | (value << (8 - shift));
            return result;
        }
        
        // метод для бинарного сдвига символа влево на n битов
        unsigned char ShiftBitsLeft(unsigned char value, int shift)
        {
            // сдвигаем биты влево с циклическим переносом
            unsigned char result = (value << shift) | (value >> (8 - shift));
            return result;
        }
        
        // шифрование текста
        String^ Encrypt(String^ plainText)
        {
            String^ result = "";
            array<unsigned char>^ bytes = System::Text::Encoding::UTF8->GetBytes(plainText);
            
            for (int i = 0; i < bytes->Length; i++)
            {
                // применяем битовый сдвиг вправо для каждого байта (encrypt shift right)
                bytes[i] = ShiftBitsRight(bytes[i], _key);
            }
            
            // преобразуем зашифрованные байты обратно в строку
            result = System::Text::Encoding::UTF8->GetString(bytes);
            return result;
        }
        
        // дешифрование текста
        String^ Decrypt(String^ cipherText)
        {
            String^ result = "";
            array<unsigned char>^ bytes = System::Text::Encoding::UTF8->GetBytes(cipherText);
            
            for (int i = 0; i < bytes->Length; i++)
            {
                // для дешифрования используем сдвиг влево (обратное направление)
                bytes[i] = ShiftBitsLeft(bytes[i], _key);
            }
            
            // преобразуем расшифрованные байты обратно в строку
            result = System::Text::Encoding::UTF8->GetString(bytes);
            return result;
        }
        
        // метод для отображения двоичного представления строки
        String^ GetBinaryRepresentation(String^ text)
        {
            String^ result = "";
            array<unsigned char>^ bytes = System::Text::Encoding::UTF8->GetBytes(text);
            
            for (int i = 0; i < bytes->Length; i++)
            {
                String^ binary = Convert::ToString(bytes[i], 2)->PadLeft(8, '0');
                result += binary + " ";
            }
            
            return result->Trim();
        }
    };
} 