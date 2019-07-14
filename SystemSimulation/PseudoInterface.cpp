
#include "PseudoInterface.h"

PseudoInterface::PseudoInterface(u32 id) :
	InterfaceID(id)
{
}



GPIO::GPIO(u32 id) :
	PseudoInterface(id)
{
}


GPIO *GPIO::Open(PseudoInterface *interface)
{
	GPIO *gpio = (GPIO *)interface;
	SetDirection(gpio, Input);
	return gpio;
}
void GPIO::Close(GPIO *interface)
{
}

void GPIO::Trans()
{
	for (size_t i=0; i<Connections.size(); i++)
	{
		((GPIO *)Connections[i])->Recv();
	}
}

/*
In *, In * = HiZ

Out HiZ, In * = HiZ
Out Low, In * = Low
Out High, In * = High

Out HiZ, Out HiZ = HiZ
Out Low, Out HiZ = Low
Out High, Out HiZ = High
Out HiZ, Out HiZ = HiZ

if HiZ : = Push
*/
void GPIO::Recv()
{
	//PRINTENTERFUNC
	Logic_e newLogic = HiZ;

	if (State.Direction==Output) { newLogic=State.Push; }

	for (size_t i=0; i<Connections.size(); i++)
	{
		State_t state = ((GPIO *)Connections[i])->Get();

		if (state.Direction==Output)
		{
			if (newLogic==Err || state.Push==Err) { newLogic=Err; }
			else if (newLogic==HiZ) { newLogic=state.Push; }
			else if (state.Push==HiZ) { }
			else if (newLogic!=state.Push) { newLogic=Err; }
		}
	}

	if (State.Direction==Input && newLogic==HiZ) { newLogic=State.Push; }

	State.Logic=newLogic;
	//PRINT("newLogic %d\n", (int)newLogic);


	// if (state==HiZ) { newState = Push; }
	// else if (Direction == Output)
	// {
	// 	if (state!=Push) { newState = Err; }
	// 	else { newState = state; }
	// }
	// else 
	// {
	// 	newState = state;
	// }
}
GPIO::State_t GPIO::Get()
{
	return State;
}

void GPIO::SetDirection(GPIO *interface, Direction_e direction)
{
	GPIO *gpio = (GPIO *)interface;
	gpio->State.Direction=direction;
	gpio->Trans();
}

void GPIO::Set(GPIO *interface, Logic_e val)
{
	GPIO *gpio = (GPIO *)interface;
	gpio->State.Push = val;
	gpio->Trans();
}

GPIO::Logic_e GPIO::Read(GPIO *interface)
{
	if (!interface) { return Err; }
	GPIO *gpio = (GPIO *)interface;
	return gpio->State.Logic;
}




Serial::Serial(u32 id) :
	PseudoInterface(id)
{
}

Serial *Serial::Open(PseudoInterface *interface)
{
	Serial *serial = (Serial *)interface;

	serial->Head=0;
	serial->Tail=0;

	pthread_mutex_init(&serial->TransLock, 0);
	pthread_mutex_init(&serial->ReadLock, 0);

	return serial;
}
void Serial::Close(Serial *interface)
{
	pthread_mutex_destroy(&interface->TransLock);
	pthread_mutex_destroy(&interface->ReadLock);
}

void Serial::Trans(u8 *buff, u64 len)
{
	pthread_mutex_lock(&TransLock);
	for (size_t i=0; i<Connections.size(); i++)
	{
		((Serial *)Connections[i])->Recv(buff, len);
	}
	pthread_mutex_unlock(&TransLock);
}
void Serial::Recv(u8 *buff, u64 len)
{
	pthread_mutex_lock(&ReadLock);
	for (u64 i=0; i<len; i++)
	{
		Buffer[Tail] = buff[i];
		Tail=(Tail+1)%SERIAL_BUFF_LEN;

		if (Tail==Head) { Head=(Head+1)%SERIAL_BUFF_LEN; }
	}
	pthread_mutex_unlock(&ReadLock);
}

bool Serial::Available(Serial *interface)
{
	u64 len = 0;
	pthread_mutex_lock(&interface->ReadLock);
	if (interface->Tail>=interface->Head) { len = interface->Tail-interface->Head; }
	else { len = (interface->Tail+SERIAL_BUFF_LEN)-interface->Head; }
	pthread_mutex_unlock(&interface->ReadLock);
	return len;
}
u64 Serial::Read(Serial *interface, void *buff, u64 len)
{
	pthread_mutex_lock(&interface->ReadLock);
	u64 i=0;
	for (; i<len && interface->Head!=interface->Tail; i++)
	{
		(((u8 *)buff)[i]) = interface->Buffer[interface->Head];
		interface->Head=(interface->Head+1)%SERIAL_BUFF_LEN;
	}
	pthread_mutex_unlock(&interface->ReadLock);
	return i;
}
void Serial::Write(Serial *interface, void *buff, u64 len)
{
	interface->Trans((u8 *)buff, len);
}

