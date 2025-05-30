#pragma once

using namespace System;
using namespace System::Collections::Generic;

namespace cursachDLL {
    
    // хеш-таблица с рехешированием
    public ref class HashTableLib
    {
    private:
        int _size;
        int _count;
        double _loadFactor;
        array<String^>^ _keys;
        array<Object^>^ _values;
        array<bool>^ _occupied;
        array<bool>^ _tombstone; // для корректного удаления
        
    public:
        void Initialize(int size, double loadFactor)
        {
            if (size <= 0)
                throw gcnew ArgumentException("Initial size must be positive");
             
            if (loadFactor <= 0 || loadFactor >= 1)
                throw gcnew ArgumentException("Load factor must be between 0 and 1");
                
            _size = size;
            _count = 0;
            _loadFactor = loadFactor;
            
            _keys = gcnew array<String^>(_size);
            _values = gcnew array<Object^>(_size);
            _occupied = gcnew array<bool>(_size);
            _tombstone = gcnew array<bool>(_size);
            
            for (int i = 0; i < _size; i++)
            {
                _occupied[i] = false;
                _tombstone[i] = false;
            }
        }
        
        HashTableLib(int initialSize)
        {
            Initialize(initialSize, 0.75);
        }
        
        HashTableLib()
        {
            Initialize(16, 0.75);
        }
        
        HashTableLib(int initialSize, double loadFactor)
        {
            Initialize(initialSize, loadFactor);
        }
        
        int Hash(String^ key)
        {
            if (key == nullptr)
                throw gcnew ArgumentNullException("key");
            
            int hash = 0;
            
            // хешируем по кодам символов
            for (int i = 0; i < key->Length; i++)
            {
                hash = (hash * 31 + key[i]) % _size;
            }
            
            return hash;
        }
        
        // линейное пробирование
        int Rehash(int hash, int attempt)
        {
            return (hash + attempt) % _size;
        }
        
        void Resize()
        {
            int oldSize = _size;
            array<String^>^ oldKeys = _keys;
            array<Object^>^ oldValues = _values;
            array<bool>^ oldOccupied = _occupied;
            
            // увеличиваем в 2 раза
            _size *= 2;
            
            _keys = gcnew array<String^>(_size);
            _values = gcnew array<Object^>(_size);
            _occupied = gcnew array<bool>(_size);
            _tombstone = gcnew array<bool>(_size);
            
            for (int i = 0; i < _size; i++)
            {
                _occupied[i] = false;
                _tombstone[i] = false;
            }
            
            _count = 0;
            
            // перехешируем элементы
            for (int i = 0; i < oldSize; i++)
            {
                if (oldOccupied[i])
                {
                    Put(oldKeys[i], oldValues[i]);
                }
            }
        }
        
        void Put(String^ key, Object^ value)
        {
            if (key == nullptr)
                throw gcnew ArgumentNullException("key");
            
            // проверяем заполненность
            if ((double)(_count + 1) / _size > _loadFactor)
            {
                Resize();
            }
            
            int hash = Hash(key);
            int i = 0;
            int index;
            
            // ищем свободную ячейку
            do
            {
                index = Rehash(hash, i++);
                
                // обновляем если ключ уже есть
                if (_occupied[index] && _keys[index]->Equals(key))
                {
                    _values[index] = value;
                    return;
                }
                
                // вставляем в свободную ячейку
                if (!_occupied[index])
                {
                    _keys[index] = key;
                    _values[index] = value;
                    _occupied[index] = true;
                    _tombstone[index] = true;
                    _count++;
                    return;
                }
                
            } while (i < _size);
            
            // если не нашли место
            Resize();
            Put(key, value);
        }
        
        Object^ Get(String^ key)
        {
            if (key == nullptr)
                throw gcnew ArgumentNullException("key");
            
            int hash = Hash(key);
            int i = 0;
            int index;
            
            do
            {
                index = Rehash(hash, i++);
                
                if (_occupied[index] && _keys[index]->Equals(key))
                {
                    return _values[index];
                }
                
                // ячейка никогда не была занята
                if (!_tombstone[index])
                {
                    return nullptr;
                }
                
            } while (i < _size);
            
            return nullptr;
        }
        
        bool ContainsKey(String^ key)
        {
            if (key == nullptr)
                throw gcnew ArgumentNullException("key");
            
            int hash = Hash(key);
            int i = 0;
            int index;
            
            do
            {
                index = Rehash(hash, i++);
                
                if (_occupied[index] && _keys[index]->Equals(key))
                {
                    return true;
                }
                
                if (!_tombstone[index])
                {
                    return false;
                }
                
            } while (i < _size);
            
            return false;
        }
        
        bool Remove(String^ key)
        {
            if (key == nullptr)
                throw gcnew ArgumentNullException("key");
            
            int hash = Hash(key);
            int i = 0;
            int index;
            
            do
            {
                index = Rehash(hash, i++);
                
                if (_occupied[index] && _keys[index]->Equals(key))
                {
                    return true;
                }
                
                if (!_tombstone[index])
                {
                    return false;
                }
                
            } while (i < _size);
            
            return false;
        }
        
        // Получение всех ключей таблицы
        List<String^>^ GetKeys()
        {
            List<String^>^ result = gcnew List<String^>();
            
            for (int i = 0; i < _size; i++)
            {
                if (_occupied[i])
                {
                    result->Add(_keys[i]);
                }
            }
            
            return result;
        }
        
        // Получение количества элементов в таблице
        int Count()
        {
            return _count;
        }
        
        // Очистка таблицы
        void Clear()
        {
            for (int i = 0; i < _size; i++)
            {
                _occupied[i] = false;
                _tombstone[i] = false;
            }
            
            _count = 0;
        }
    };
} 