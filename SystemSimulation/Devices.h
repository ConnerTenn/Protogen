
#ifndef _DEVICES_H_
#define _DEVICES_H_

#include "Global.h"
#include "PseudoInterface.h"

struct Thread
{
	pthread_t ID;
	void *(* Entry)(void *);
};

void InitDevices();
void DestroyDevices();

class Device
{
public:
	std::string Name;
	Thread Entry;
	std::vector<PseudoInterface *> Interfaces;

	Interface_t Open(u32 interfaceID);
	void Close(Interface_t interface);
};




#endif


