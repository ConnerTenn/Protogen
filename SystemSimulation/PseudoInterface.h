
#ifndef _PSEUDO_INTERFACE_H_
#define _PSEUDO_INTERFACE_H_

#include "Global.h"

#define SERIAL_BUFF_LEN 1024

namespace Serial
{
	struct SerialBuffer
	{
		u16 Port;
		u8 Buffer[SERIAL_BUFF_LEN];
		u32 DataHead, DataTail;
		pthread_mutex_t Lockm;
		u32 Count;

		SerialBuffer();
		~SerialBuffer();

		void Increment();
		void Decrement();

		void Lock();
		void Unlock();
	};

	SerialBuffer *Open(u16 port);
	int Read(SerialBuffer *ser, void *buff, unsigned int len);
	int Write(SerialBuffer *ser, void *buff, unsigned int len);
	int Available(SerialBuffer *ser);
	int Close(SerialBuffer *ser);

	void InitSerial();
	void CloseSerial();
};
typedef Serial::SerialBuffer * Serial_t;

#endif
