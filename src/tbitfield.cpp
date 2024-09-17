// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
    if (len < 1) throw(len < 1);
    this->BitLen = len;
    this->MemLen = (BitLen + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
    //std::cout << this->MemLen << std::endl;
    this->pMem = new TELEM[this->MemLen];
    for (int i = 0; i < this->MemLen; ++i) {
        this->pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    this->BitLen = bf.BitLen;
    this->MemLen = bf.MemLen;
    this->pMem = new TELEM[this->MemLen];
    for (int i = 0; i < this->MemLen; ++i) {
        this->pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] this->pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return (TELEM)1 << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return this->BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n >= this->BitLen || n < 0) {
        throw "wrong n";
    }
    this->pMem[this->GetMemIndex(n)] |= this->GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n > this->BitLen || n < 0) {
        throw "n > BitLen";
    }
    this->pMem[this->GetMemIndex(n)] &= ~(this->GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n > this->BitLen || n < 0) {
        throw "n > BitLen";
    }
    return this->pMem[this->GetMemIndex(n)] & this->GetMemMask(n);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    this->BitLen = bf.BitLen;
    this->MemLen = bf.MemLen;
    delete[] this->pMem;
    this->pMem = new TELEM[this->MemLen];
    for (int i = 0; i < this->MemLen; ++i) {
        this->pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (this->BitLen != bf.BitLen) return 0;
    for (int i = 0; i < this->MemLen; ++i) {
        if (this->pMem[i] != bf.pMem[i]) return 0;
    }
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int maxBitLen = max(this->BitLen, bf.BitLen);
    TBitField result(maxBitLen);

    int minMemLen = min(this->MemLen, bf.MemLen);
    for (int i = 0; i < minMemLen; ++i) {
        result.pMem[i] = this->pMem[i] | bf.pMem[i];
    }

    if (this->BitLen > bf.BitLen) {
        for (int i = minMemLen; i < this->MemLen; ++i) {
            result.pMem[i] = this->pMem[i];
        }
    }
    else if (bf.BitLen > this->BitLen) {
        for (int i = minMemLen; i < bf.MemLen; ++i) {
            result.pMem[i] = bf.pMem[i];
        }
    }

    return result;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int maxBitLen = max(this->BitLen, bf.BitLen);
    TBitField result(maxBitLen);

    int minMemLen = min(this->MemLen, bf.MemLen);
    for (int i = 0; i < minMemLen; ++i) {
        result.pMem[i] = this->pMem[i] & bf.pMem[i];
    }

    if (this->BitLen > bf.BitLen) {
        for (int i = minMemLen; i < this->MemLen; ++i) {
            result.pMem[i] = this->pMem[i];
        }
    }
    else if (bf.BitLen > this->BitLen) {
        for (int i = minMemLen; i < bf.MemLen; ++i) {
            result.pMem[i] = bf.pMem[i];
        }
    }

    return result;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField result(this->BitLen);

    for (int i = 0; i < this->MemLen; ++i) {    
        result.pMem[i] = ~(this->pMem[i]);
    }

    int lastBits = this->BitLen % (sizeof(TELEM) * 8);
    if (lastBits > 0) {
        TELEM mask = (1 << lastBits) - 1;
        result.pMem[this->MemLen - 1] &= mask;
    }

    return result;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    string str;
    istr >> str;
    if (str.length() != bf.BitLen) {
        throw ("len str != len bitfield");
    }
    for (int i = 0; i < bf.BitLen; ++i) {
        if (str[i] == '1') bf.SetBit(i);
        else if (str[i] == '0') bf.ClrBit(i);
        else throw("symbol is not '0' or '1'");
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; ++i) {
        ostr << bf.GetBit(i);
    }
    return ostr;
}
