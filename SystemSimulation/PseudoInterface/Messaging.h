
#ifndef _MESSAGING_H_
#define _MESSAGING_H_

#include "Global.h"
#include "PseudoInterface.h"

#define STARTBYTE 0x01
//#define STARTCONTENTBYTE 0x02
//#define CRCBYTE 0x06
//#define ENDBYTE 0x03

//int ParseMessages(void *buffer, u8 *messages);

#define MSG_BUFF_LEN 10

#define MSG_MAX_DEST_LEN 16
#define MSG_MAX_LABEL_LEN 16
#define MSG_MAX_CONTENT_LEN 128
#define MSG_MAX_LEN (MSG_MAX_DEST_LEN + MSG_MAX_LABEL_LEN + MSG_MAX_CONTENT_LEN)

struct Message
{
	u8 DestLen, LabelLen;
	u16 ContentLen;
	char Dest[MSG_MAX_DEST_LEN];
	char Label[MSG_MAX_LABEL_LEN];
	u8 Content[MSG_MAX_CONTENT_LEN];
};

struct MessageHandler 
{
	Serial *Interface;
	Message Messages[MSG_BUFF_LEN];
	u64 Head, Tail;//BuffHead, BuffTail;

	Message Buffer;
	u8 LenFill, DestFill, LabelFill;
	u64 ContentFill;
	//u8 BufferCRC;
	enum RecvState
	{
		RecvWait,
		RecvLen,
		RecvDest,
		RecvLabel,
		RecvContent,
		RecvFinish,
		//RecvCRC,
	} State;
	
};

//u16 CalcCRC(Message *msg);

void InitMessageHandler(MessageHandler *handler, Serial *interface);
void DestroyMessageHandler(MessageHandler *handler);
bool GetNextMessage(MessageHandler *handler, Message *msg);
void SendMessage(MessageHandler *handler, Message *msg);
void PrintMessage(Message *msg);
void LogMessage(Message *msg);

#endif
