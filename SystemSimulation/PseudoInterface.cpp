
#include "PseudoInterface.h"

PseudoInterface::PseudoInterface(u32 id) :
	InterfaceID(id)
{
}

GPIO::GPIO(u32 id) :
	PseudoInterface(id)
{
}

void GPIO::Set(GPIO * interface, bool val)
{
	interface->Connections.size();
}

Serial::Serial(u32 id) :
	PseudoInterface(id)
{
}




// using namespace Serial;

// std::vector<Serial_t> SerialList;

// pthread_mutex_t SerialLock;


// SerialBuffer::SerialBuffer()
// {
// 	pthread_mutex_init(&Lockm,0);
// 	Count=0;
// 	Port=-1;
// }
// SerialBuffer::~SerialBuffer()
// {
// 	pthread_mutex_destroy(&Lockm);
// }
// void SerialBuffer::Increment()
// {
// 	Lock();
// 	Count++;
// 	Unlock();
// }
// void SerialBuffer::Decrement()
// {
// 	Lock();
// 	Count--;
// 	Unlock();
// }
// void SerialBuffer::Lock()
// {
// 	pthread_mutex_lock(&Lockm);
// }
// void SerialBuffer::Unlock()
// {
// 	pthread_mutex_unlock(&Lockm);
// }

// Serial_t Serial::Open(u16 port)
// {
// 	Serial_t serial=0;
// 	for (u64 i=0; i<SerialList.size(); i++)
// 	{
// 		if (SerialList[i]->Port==port) { serial = SerialList[i]; }
// 	}
// 	if (!serial)
// 	{
// 		serial = new SerialBuffer();
// 		pthread_mutex_lock(&SerialLock);
// 		SerialList.push_back(serial);
// 		pthread_mutex_unlock(&SerialLock);
// 	}

// 	serial->Increment();

// 	return serial;
// }

// int Serial::Read(Serial_t ser, void *buff, unsigned int len)
// {
// 	if (!buff) { return 0; }
// 	int count=0;
// 	ser->Lock();
// 	for (unsigned int i=0;i<len;i++)
// 	{
// 		if (ser->DataHead != ser->DataTail)
// 		{
// 			((char *)buff)[i]=ser->Buffer[ser->DataHead];
// 			ser->DataHead=(ser->DataHead+1)%SERIAL_BUFF_LEN;
// 			count++;
// 		}
// 	}
// 	ser->Unlock();
// 	return count;
// }

// int Serial::Write(Serial_t ser, void *buff, unsigned int len)
// {
// 	ser->Lock();
// 	for (unsigned int i=0;i<len;i++)
// 	{
// 		ser->Buffer[ser->DataTail]=((char *)buff)[i];
// 		if (ser->DataTail==ser->DataHead) { ser->DataHead=(ser->DataHead+1)%SERIAL_BUFF_LEN; }
// 		ser->DataTail=(ser->DataTail+1)%SERIAL_BUFF_LEN;
// 	}
// 	ser->Unlock();
// 	return 0;
// }

// int Available(SerialBuffer *ser)
// {
// 	if (ser->DataTail>=ser->DataHead) { return ser->DataTail-ser->DataHead; } 
// 	return ser->DataHead+SERIAL_BUFF_LEN-ser->DataTail-1;
// }

// int Serial::Close(Serial_t ser)
// {
// 	Serial_t serial=0;
// 	int index=-1;
// 	for (u64 i=0; i<SerialList.size(); i++)
// 	{
// 		if (SerialList[i]->Port==ser->Port) { serial = SerialList[i]; index=i; }
// 	}
// 	if (!serial) { return -1; }
// 	serial->Decrement();
// 	if (!serial->Count)
// 	{
// 		pthread_mutex_lock(&SerialLock);
// 		delete serial;
// 		SerialList.erase(SerialList.begin()+index);
// 		pthread_mutex_unlock(&SerialLock);
// 	}
// 	return 0;
// }


// void Serial::InitSerial()
// {
// 	pthread_mutex_init(&SerialLock, 0);
// }
// void Serial::CloseSerial()
// {
// 	for (u64 i=0; i<SerialList.size(); i++)
// 	{
// 		delete SerialList[i];
// 	}
// 	pthread_mutex_destroy(&SerialLock);
// }

