#pragma once

using namespace System;
using namespace System::Collections::Generic;

namespace cursachDLL {
    
    // структура данных для хранения идентификатора
    public ref class Identifier
    {
    public:
        String^ Name;
        String^ Type;
        Object^ Value;
        
        Identifier(String^ name, String^ type, Object^ value)
        {
            Name = name;
            Type = type;
            Value = value;
        }
        
        // переопределение метода ToString для удобного отображения
        virtual String^ ToString() override
        {
            return String::Format("{0} ({1}): {2}", Name, Type, Value != nullptr ? Value->ToString() : "null");
        }
    };
    
    // класс для реализации простого списка для организации таблиц идентификаторов
    public ref class SimpleListLib
    {
    private:
        // внутренний список для хранения идентификаторов
        List<Identifier^>^ _identifiers;
        
    public:
        // конструктор
        SimpleListLib()
        {
            _identifiers = gcnew List<Identifier^>();
        }
        
        // добавление идентификатора в список
        bool Add(String^ name, String^ type, Object^ value)
        {
            // проверяем, существует ли уже такой идентификатор
            if (FindByName(name) != nullptr)
            {
                return false; // идентификатор с таким именем уже существует
            }
            
            // создаем новый идентификатор и добавляем его в список
            Identifier^ newIdentifier = gcnew Identifier(name, type, value);
            _identifiers->Add(newIdentifier);
            return true;
        }
        
        // удаление идентификатора по имени
        bool Remove(String^ name)
        {
            for (int i = 0; i < _identifiers->Count; i++)
            {
                if (_identifiers[i]->Name->Equals(name, StringComparison::OrdinalIgnoreCase))
                {
                    _identifiers->RemoveAt(i);
                    return true;
                }
            }
            
            return false; // идентификатор не найден
        }
        
        // поиск идентификатора по имени
        Identifier^ FindByName(String^ name)
        {
            for each (Identifier^ identifier in _identifiers)
            {
                if (identifier->Name->Equals(name, StringComparison::OrdinalIgnoreCase))
                {
                    return identifier;
                }
            }
            
            return nullptr; // идентификатор не найден
        }
        
        // обновление значения идентификатора
        bool UpdateValue(String^ name, Object^ newValue)
        {
            Identifier^ identifier = FindByName(name);
            
            if (identifier != nullptr)
            {
                identifier->Value = newValue;
                return true;
            }
            
            return false; // идентификатор не найден
        }
        
        // получение всех идентификаторов
        array<Identifier^>^ GetAll()
        {
            return _identifiers->ToArray();
        }
        
        // получение количества идентификаторов
        int Count()
        {
            return _identifiers->Count;
        }
        
        // очистка списка
        void Clear()
        {
            _identifiers->Clear();
        }
        
        // получение строкового представления всего списка
        virtual String^ ToString() override
        {
            String^ result = "";
            
            for (int i = 0; i < _identifiers->Count; i++)
            {
                result += i + ". " + _identifiers[i]->ToString() + "\r\n";
            }
            
            return result;
        }
    };
} 