
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
//typedef PseudoInterface * Interface_t;

class GPIO : public PseudoInterface
{
public:
	enum Direction_e
	{
		Input,
		Output
	};
	enum Logic_e
	{
		Low = 0,
		High = 1,
		HiZ = 2,
		Err = 3,
	};
	struct State_t
	{
		Direction_e Direction;
		Logic_e Logic, Push;
	} State;

	GPIO(u32 id);

	//static GPIO *Open(PseudoInterface *interface);
	//static void Close(PseudoInterface *interface);

	static GPIO *Open(PseudoInterface *interface);
	static void Close(GPIO *interface);

	void Trans();
	void Recv();
	State_t Get();

	static void SetDirection(GPIO *interface, Direction_e direction);
	static void Set(GPIO *interface, Logic_e val);
	static Logic_e Read(GPIO *interface);
};

#define SERIAL_BUFF_LEN 1024

class Serial : public PseudoInterface
{
public:
	u8 Buffer[SERIAL_BUFF_LEN];
	u64 Head, Tail;
	pthread_mutex_t TransLock, ReadLock;

	Serial(u32 id);

	static Serial *Open(PseudoInterface *interface);
	static void Close(Serial *interface);

	void Trans(u8 *buff, u64 len);
	void Recv(u8 *buff, u64 len);
	//void Get();

	static bool Available(Serial *interface);
	static u64 Read(Serial *interface, void *buff, u64 len);
	static void Write(Serial *interface, void *buff, u64 len);
};


void InitPseudoInterface();
void DestroyPseudoInterface();


#endif
