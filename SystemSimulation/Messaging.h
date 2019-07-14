
#ifndef _MESSAGING_H_
#define _MESSAGING_H_

#include "Global.h"
#include "PseudoInterface.h"

#define STARTBYTE 0x01
#define STARTCONTENTBYTE 0x02
#define ENDBYTE 0x03

//int ParseMessages(void *buffer, u8 *messages);

#define MESSAGE_BUFF_LEN 10

#define LABEL_LEN 16
#define CONTENT_LEN 128
//#define Message_Len

struct Message
{
	char Label[LABEL_LEN];
	char Content[CONTENT_LEN];
};

struct MessageHandler 
{
	Serial *Interface;
	Message Message[MESSAGE_BUFF_LEN];
	u64 Head, Tail;
};

void InitMessager(MessageHandler *handler, Serial *interface);
bool GetNextMessage(MessageHandler *handler, Message *msg);

#endif
