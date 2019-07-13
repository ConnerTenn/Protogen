
#ifndef _PSEUDO_INTERFACE_H_
#define _PSEUDO_INTERFACE_H_

#include "Global.h"


class PseudoInterface
{
public:
	u32 InterfaceID;
	std::vector<u32> ConnectionIDs;
	std::vector<PseudoInterface *> Connections;

	//static void Open();
	//static void Close();

	PseudoInterface(u32 id);
};
typedef PseudoInterface * Interface_t;

class GPIO : public PseudoInterface
{
public:
	u8 Direction;
	u8 State;

	GPIO(u32 id);

	//static GPIO *Open(PseudoInterface *interface);
	//static void Close(PseudoInterface *interface);

	static void Set(GPIO *interface, bool val);
	static bool Read(GPIO *interface);
};

#define SERIAL_BUFF_LEN 1024

class Serial : public PseudoInterface
{
public:
	Serial(u32 id);

	//static void Open();
	//static void Close();

	static void Available();
	static void Read();
	static void Write();
};


void InitPseudoInterface();
void DestroyPseudoInterface();


// namespace Serial
// {
// 	struct SerialBuffer
// 	{
// 		u16 Port;
// 		u8 Buffer[SERIAL_BUFF_LEN];
// 		u32 DataHead, DataTail;
// 		pthread_mutex_t Lockm;
// 		u32 Count;

// 		SerialBuffer();
// 		~SerialBuffer();

// 		void Increment();
// 		void Decrement();

// 		void Lock();
// 		void Unlock();
// 	};

// 	SerialBuffer *Open(u16 port);
// 	int Read(SerialBuffer *ser, void *buff, unsigned int len);
// 	int Write(SerialBuffer *ser, void *buff, unsigned int len);
// 	int Available(SerialBuffer *ser);
// 	int Close(SerialBuffer *ser);

// 	void InitSerial();
// 	void CloseSerial();
// };
// typedef Serial::SerialBuffer * Serial_t;

#endif
