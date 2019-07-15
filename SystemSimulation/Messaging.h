
#ifndef _MESSAGING_H_
#define _MESSAGING_H_

#include "Global.h"
#include "PseudoInterface.h"

#define STARTBYTE 0x01
#define STARTCONTENTBYTE 0x02
#define CRCBYTE 0x06
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
	u8 Content[MSG_CONTENT_LEN];
};

struct MessageHandler 
{
	Serial *Interface;
	Message Messages[MSG_BUFF_LEN];
	u64 Head, Tail;//BuffHead, BuffTail;
	Message Buffer;
	u64 DestFill, LabelFill, ContentFill;
	u8 BufferCRC;
	enum RecvState
	{
		RecvDest,
		RecvLabel, 
		RecvContent,
		RecvCRC,
	} State;
	
};

u16 CalcCRC(Message *msg);

void InitMessageHandler(MessageHandler *handler, Serial *interface);
void DestroyMessageHandler(MessageHandler *handler);
bool GetNextMessage(MessageHandler *handler, Message *msg);
void SendMessage(MessageHandler *handler, Message *msg);
void PrintMessage(Message *msg);

#endif
