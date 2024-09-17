// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp)
{
    if (mp < 0) {
        throw "len < 0";
    }
    MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet& s) : MaxPower(s.MaxPower),  BitField(s.BitField)
{
}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : MaxPower(bf.GetLength()),  BitField(bf)
{
}

TSet::operator TBitField()
{
    return this->BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return this->BitField.GetLength();
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if (Elem >= 0 && Elem < MaxPower) {
        return BitField.GetBit(Elem);
    }
    else {
        return 0;
    }
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem >= this->MaxPower || Elem < 0) {
        throw "wrong Elem";
    }
    this->BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem >= this->MaxPower || Elem < 0) {
        throw "wrong Elem";
    }
    this->BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    if (this == &s) {
        return *this;
    }
    this->MaxPower = s.MaxPower;
    this->BitField = s.BitField;
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    if (this->MaxPower == s.MaxPower && this->BitField == s.BitField) {
        return 1;
    }
    return 0;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return !(*this == s);
}

TSet TSet::operator+(const TSet& s) // объединение
{
    int maxPower = std::max(this->MaxPower, s.MaxPower);
    TSet p(maxPower);

    for (int i = 0; i < this->MaxPower; ++i) {
        if (BitField.GetBit(i)) {
            p.BitField.SetBit(i);
        }
    }

    for (int i = 0; i < s.MaxPower; ++i) {
        if (s.BitField.GetBit(i)) {
            p.BitField.SetBit(i);
        }
    }

    return p;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    if (Elem < 0 || Elem > this->MaxPower) {
        throw "wrong Elem";
    }
    TSet p(this->MaxPower);
    p.InsElem(Elem);
    return p;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    if (Elem < 0 || Elem > this->MaxPower) {
        throw "wrong Elem";
    }
    TSet p(this->MaxPower);
    p.DelElem(Elem);
    return p;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
    int maxPower = std::max(this->MaxPower, s.MaxPower);
    int minPower = std::min(this->MaxPower, s.MaxPower);
    TSet p(maxPower);
    for (int i = 0; i < minPower; ++i) {
        if (this->BitField.GetBit(i) && s.BitField.GetBit(i)) {
            p.BitField.SetBit(i);
        }
    }
    return p;
}

TSet TSet::operator~(void) // дополнение
{
    TSet p(this->MaxPower);
    p.BitField = ~(this->BitField);
    return p;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    string str;
    istr >> str;
    if (str.length() != s.MaxPower) {
        throw ("len str != len bitset");
    }
    for (int i = 0; i < s.MaxPower; ++i) {
        if (str[i] == '1') s.InsElem(i);
        else if (str[i] == '0') s.DelElem(i);
        else throw("symbol is not '0' or '1'");
    }
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    for (int i = 0; i < s.MaxPower; ++i) {
        ostr << s.IsMember(i);
    }
    return ostr;
}
