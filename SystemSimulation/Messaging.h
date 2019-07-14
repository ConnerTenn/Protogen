
#ifndef _MESSAGING_H_
#define _MESSAGING_H_

#include "Global.h"
#include "PseudoInterface.h"

#define STARTBYTE 0x01
#define STARTCONTENTBYTE 0x02
#define ENDBYTE 0x03

//int ParseMessages(void *buffer, u8 *messages);

#define MSG_BUFF_LEN 10

#define MSG_DEST_LEN 16
#define MSG_LABEL_LEN 16
#define MSG_CONTENT_LEN 128
#define MSG_TOTAL_LEN (MSG_DEST_LEN + MSG_LABEL_LEN + MSG_CONTENT_LEN)

struct Message
{
	char Dest[MSG_DEST_LEN];
	char Label[MSG_LABEL_LEN];
	char Content[MSG_CONTENT_LEN];
};

struct MessageHandler 
{
	Serial *Interface;
	Message Messages[MSG_BUFF_LEN];
	char Buffer[MSG_TOTAL_LEN*2];
	u64 MsgHead, MsgTail, BuffHead, BuffTail;
	
};

void InitMessager(MessageHandler *handler, Serial *interface);
bool GetNextMessage(MessageHandler *handler, Message *msg);

#endif
