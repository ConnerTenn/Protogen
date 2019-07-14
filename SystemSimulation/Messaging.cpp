
#include "Messaging.h"

void InitMessager(MessageHandler *handler, Serial *interface)
{
	handler->Interface = interface;
	handler->MsgHead = 0; handler->MsgTail = 0;
	handler->BuffHead = 0; handler->BuffTail = 0;
}
bool GetNextMessage(MessageHandler *handler, Message *msg)
{
	char buff[MSG_TOTAL_LEN*2];
	if (Serial::Available(handler->Interface))
	{
		Serial::Read(handler->Interface, buff, MSG_TOTAL_LEN*2);
		
	}
}
