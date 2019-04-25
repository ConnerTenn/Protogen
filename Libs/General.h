
#ifndef _GENERAL_H_
#define _GENERAL_H_

#include <math.h>
#include <initializer_list>

//#include <unistd.h>
#include <chrono>
#include <stdexcept>

#define MIN(a,b) ((a)<=(b)?(a):(b))
#define MAX(a,b) ((a)>=(b)?(a):(b))
#define ABS(a) ((a)<0?-(a):(a))
#define PI M_PIl
#define TAU (2.0*PI)
#define MOD(a,b) ((a)%(b)+((a)<0?(b):0))
#define CEILDIV(a,b) ( (a)/(b) + ((a)%(b)?1:0) )

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;


i64 GetMilliseconds();
i64 GetMicroseconds();
i64 GetNanoseconds();

extern i64 StartTime;

inline double mmod(double a, double m)
{
	return fmod(a, m) + (a<0?m:0);
}

template<class T>
inline T MinMax(T x, T min, T max)
{
	x=MAX(x,min);
	return MIN(x, max);
}


template<class T, int N>
struct Array
{
	T Values[N];
	
	Array();
	Array(std::initializer_list<T> list);
	Array(Array<T, N> &other);
	inline int size();
	T &operator[](int i);
	void Copy(Array<T, N> &other);
};

template<class T>
class DynamicArray
{
public:
	T *Values;
	int Length;
	
public:
	DynamicArray(int len);
	DynamicArray(std::initializer_list<T> list);
	DynamicArray(const DynamicArray<T> &other);
	~DynamicArray();
	void Resize(int len);
	inline int size();
	T &operator[](int i);
	void operator=(const DynamicArray<T> &other);
	void Copy(DynamicArray<T> &other);
};

template<class T>
class RoundBuffer
{
private:
	int Length = 0;
	T *Buffer = 0;
	int Begin = 0;
	
public:
	//RoundBuffer();
	RoundBuffer(int len);
	RoundBuffer(const RoundBuffer<T> &other);
	~RoundBuffer();
	
private:
	int Transform(int i);
	
public:
	inline int size();
	void InsertBegin(T val);
	void InsertEnd(T val);
	
	T &operator[](int i);
	void operator=(const RoundBuffer<T> &other);
};






template<class T, int N>
Array<T,N>::Array() { }

template<class T, int N>
Array<T,N>::Array(std::initializer_list<T> list)
{
	for (int i = 0; i < (int)list.size(); i++) { Values[i] = list.begin()[i]; }
}

template<class T, int N>
Array<T,N>::Array(Array<T, N> &other)
{
	for (int i = 0; i < N; i++) { Values[i] = other[i]; }
}

template<class T, int N>
int Array<T,N>::size() { return N; }

template<class T, int N>
T &Array<T,N>::operator[](int i) { return Values[i]; }

template<class T, int N>
void Array<T,N>::Copy(Array<T, N> &other)
{
	for (int i = 0; i < N; i++) { Values[i] = other[i]; }
}




template<class T>
DynamicArray<T>::DynamicArray(int len) :
	Length(len)
{
	//std::cout << "DynamicArray:New Len" << len << "\n";
	Values = new T[len];
}

template<class T>
DynamicArray<T>::DynamicArray(std::initializer_list<T> list)
{
	DynamicArray(list.size());
	for (int i = 0; i < (int)list.size(); i++) { Values[i] = list.begin()[i]; }
}

template<class T>
DynamicArray<T>::DynamicArray(const DynamicArray<T> &other)
{
	//std::cout << "DynamicArray:Copy\n";
	DynamicArray(other.size());
	for (int i = 0; i < Length; i++) { Values[i] = other[i]; }
}
template<class T>
DynamicArray<T>::~DynamicArray()
{
	//std::cout<<"DynamicArray:Delete "<<Length<<"\n";
	delete[] Values;
}
template<class T>
void DynamicArray<T>::Resize(int len)
{
	delete[] Values;
	Values = new T[len];
}

template<class T>
int DynamicArray<T>::size() { return Length; }

template<class T>
T &DynamicArray<T>::operator[](int i) { return Values[i]; }

template<class T>
void DynamicArray<T>::operator=(const DynamicArray<T> &other)
{
	Length = other.Length;
	Resize(Length);
	for (int i = 0; i < Length; i++) { Values[i] = other.Values[i]; }
}

template<class T>
void DynamicArray<T>::Copy(DynamicArray<T> &other)
{
	DynamicArray(other.size());
	for (int i = 0; i < Length; i++) { Values[i] = other[i]; }
}



template<class T>
RoundBuffer<T>::RoundBuffer(int len) :
	Length(len)
{
	Buffer = new T[Length];
	Begin=0;
}

template<class T>
RoundBuffer<T>::RoundBuffer(const RoundBuffer<T> &other)
{
	(*this) = other;
}

template<class T>
RoundBuffer<T>::~RoundBuffer()
{
	delete[] Buffer;
}

template<class T>
int RoundBuffer<T>::size() { return Length; }

template<class T>
int RoundBuffer<T>::Transform(int i) 
{
	if (Length==0) { throw std::domain_error("div/0"); }
	int v = i+Begin;
	v = v%Length + (v<0?Length:0);
	return v;  
}

template<class T>
void RoundBuffer<T>::InsertBegin(T val) 
{
	Begin = Transform(-1);
	(*this)[0] = val;
}
template<class T>
void RoundBuffer<T>::InsertEnd(T val) 
{
	Begin = Transform(1);
	(*this)[-1] = val;
}

template<class T>
T &RoundBuffer<T>::operator[](int i)
{
	//std::cout << "operator["<<i<<"]\n";
	return Buffer[Transform(i)];
}

template<class T>
void RoundBuffer<T>::operator=(const RoundBuffer<T> &other)
{
	//std::cout << "operator=\n";
	Length = other.Length;
	if (Buffer) { delete[] Buffer; }
	Buffer = new T[Length];
	Begin=other.Begin;
	for (int i = 0; i < Length; i++)
	{
		Buffer[i] = other.Buffer[i];
	}
}

#endif

