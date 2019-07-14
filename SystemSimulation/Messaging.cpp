
#include "Messaging.h"

void InitMessager(MessageHandler *handler, Serial *interface)
{
	handler->Interface = interface;
	handler->Head = 0;
	handler->Tail = 0;
}
bool GetNextMessage(Message)
{
	
}
