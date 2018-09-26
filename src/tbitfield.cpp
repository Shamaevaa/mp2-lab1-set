// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	BitLen = len;
	if (len < 0)
		throw "incorrect data";
	if (len % (sizeof(TELEM) * 8) != 0)
		MemLen = len / (sizeof(TELEM) * 8) + 1;
	else MemLen = len / (sizeof(TELEM) * 8);
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	pMem = new TELEM[bf.MemLen];
	for (int i = 0; i < bf.MemLen; i++)
		pMem[i] = bf.pMem[i];
	MemLen = bf.MemLen;
}

TBitField::~TBitField()
{
	delete[]pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n >= 0 && n < BitLen) 
	{
		return n / (sizeof(TELEM) * 8);
	}
	else
		throw "incorrect data";
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1 << n;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n >= 0) && (n < BitLen))
		pMem[GetMemIndex(n)] |= GetMemMask(n);
		else
		throw "incorrect data";
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n >= 0) && (n < BitLen))
	{
		pMem[GetMemIndex(n)] &= ~GetMemMask(n);
	}
	else
		throw "incorrect data";
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	return pMem[GetMemIndex(n)] >> (n % (sizeof(TELEM) * 8)) & 1;
	TELEM Mask = GetMemMask(n);
	int x = n / BitLen;
	if (Mask & pMem[x] == 0)
		return 0;
	else
		return 1;
}
// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (BitLen != bf.BitLen)
	{
		MemLen = bf.MemLen;
		BitLen = bf.BitLen;
		delete[] pMem;
		pMem = new TELEM[MemLen];
	}

		for (int i = 0; i < MemLen; i++)
			pMem[i] = bf.pMem[i];

	return (*this);
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	int i = 0;
	if (BitLen != bf.BitLen)
		return 0;
	for (i = 0; i < MemLen; i++)
		if (pMem[i] != bf.pMem[i])
			return 0;
	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	int i = 0;
	if (BitLen != bf.BitLen)
		return 1; 
	for (i = 0; i < MemLen; i++)
		if (pMem[i] != bf.pMem[i])
			return 1;
	return 0;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int i;
	int len = BitLen;
	if (bf.BitLen > len)
		len = bf.BitLen;
	TBitField res(len);
	for (i = 0; i < BitLen; i++)
		res.pMem[i] = pMem[i];
	for (i = 0; i < bf.MemLen; i++)
		res.pMem[i] |= bf.pMem[i];
	return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int i;
	int len = BitLen;
	if (bf.BitLen > len)
		len = bf.BitLen;
	TBitField res(len);
	for (i = 0; i < MemLen; i++)
		res.pMem[i] = pMem[i];
	for (i = 0; i < bf.MemLen; i++)
		res.pMem[i] &= bf.pMem[i];
;
	return res;

}

TBitField TBitField::operator~(void) // отрицание
{
	int i;
	int len = BitLen;
	TBitField res(len);
	for (i = 0; i < BitLen; i++)
		if(GetBit(i) == 0)
		res.SetBit(i);
	return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	char s;
	int i = 0;
	/*do
	istr >> s;
	while (s == ' ');*/
	while (1)
	{
		istr >> s;
		if (s == '0')
			bf.ClrBit(i++);
		else
			if (s == '1')
				bf.SetBit(i++);
			else
				break;
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++)
		if (bf.GetBit(i))
			ostr << 1;
		else
			ostr << 0;
	return ostr;
}