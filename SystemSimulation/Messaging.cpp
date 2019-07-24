
#include "Messaging.h"

// u16 CalcCRC(Message *msg)
// {
// 	u8 crc = 0;
// 	for (u64 i=0; i<strlen(msg->Dest); i++)
// 	{
// 		crc = crc ^ msg->Dest[i];
// 	}
// 	for (u64 i=0; i<strlen(msg->Label); i++)
// 	{
// 		crc = crc ^ msg->Label[i];
// 	}
// 	for (u64 i=0; i<MSG_CONTENT_LEN; i++)
// 	{
// 		crc = crc ^ msg->Content[i];
// 	}
// 	return crc;
// }

void PostMessage(MessageHandler *handler)
{
	handler->Messages[handler->Tail] = handler->Buffer;
	handler->Tail=(handler->Tail+1)%MSG_BUFF_LEN;
	if (handler->Tail==handler->Head) { handler->Head=(handler->Head+1)%MSG_BUFF_LEN; }
}

void InitMessageHandler(MessageHandler *handler, Serial *interface)
{
	handler->Interface = interface;
	handler->Head = 0; handler->Tail = 0;
	memset(&handler->Buffer, 0, sizeof(Message));
	// //handler->BuffHead = 0; handler->BuffTail = 0;
}
void DestroyMessageHandler(MessageHandler *handler)
{
}


bool GetNextMessage(MessageHandler *handler, Message *msg)
{
	u8 buff[MSG_MAX_LEN*2];
	if (Serial::Available(handler->Interface))
	{
		u64 len = Serial::Read(handler->Interface, buff, MSG_MAX_LEN*2);
		
		for (u64 i=0; i<len; i++)
		{
			if (buff[i]==STARTBYTE && handler->State==MessageHandler::RecvWait) 
			{ 
				memset(&handler->Buffer, 0, sizeof(Message));
				handler->LenFill=0;
				handler->DestFill=0;
				handler->LabelFill=0;
				handler->ContentFill=0;
				// handler->HeaderFill=0;
				// handler->ContentFill=0;
				// //handler->BufferCRC=0;
				handler->State=MessageHandler::RecvLen; 
			}
			else if (handler->State==MessageHandler::RecvLen)
			{
				if (handler->LenFill==0) { handler->Buffer.DestLen=buff[i]; }
				if (handler->LenFill==1) { handler->Buffer.LabelLen=buff[i]; }
				if (handler->LenFill==2) { handler->Buffer.ContentLen |= buff[i]; }
				if (handler->LenFill==3) { handler->Buffer.ContentLen |= buff[i]<<8; handler->State=MessageHandler::RecvDest; }
				handler->LenFill++;
			}
			else if (handler->State==MessageHandler::RecvDest)
			{
				handler->Buffer.Dest[handler->DestFill]=buff[i];
				handler->DestFill++;
				if (handler->DestFill>=handler->Buffer.DestLen) { handler->State=MessageHandler::RecvLabel; }
			}
			else if (handler->State==MessageHandler::RecvLabel)
			{
				handler->Buffer.Label[handler->LabelFill]=buff[i];
				handler->LabelFill++;
				if (handler->LabelFill>=handler->Buffer.LabelLen) { handler->State=MessageHandler::RecvContent; }
			}
			else if (handler->State==MessageHandler::RecvContent)
			{
				handler->Buffer.Content[handler->ContentFill]=buff[i];
				handler->ContentFill++;
				if (handler->ContentFill>=handler->Buffer.ContentLen) { handler->State=MessageHandler::RecvFinish; }
			}
			
			if (handler->State==MessageHandler::RecvFinish)
			{
				handler->Buffer.Dest[MIN(handler->Buffer.DestLen, MSG_MAX_DEST_LEN-1)] = 0;
				handler->Buffer.Label[MIN(handler->Buffer.LabelLen, MSG_MAX_LABEL_LEN-1)] = 0;
				memset(handler->Buffer.Content+handler->ContentFill, 0, MAX(MSG_MAX_CONTENT_LEN-handler->ContentFill, 0));

				// if (handler->BufferCRC == CalcCRC(&handler->Buffer))
				// {
				PostMessage(handler);
				// }
				// else { LOGERROR("Msg Corruption\n"); }
				handler->State=MessageHandler::RecvWait;
			}
		}
	}

	if (handler->Head != handler->Tail)
	{
		*msg=handler->Messages[handler->Head];
		handler->Head=(handler->Head+1)%MSG_BUFF_LEN;

		return true;
	}
	return false;
}

void SendMessage(MessageHandler *handler, Message *msg)
{
	u8 ctl;
	ctl = STARTBYTE;
	Serial::Write(handler->Interface, &ctl, 1);
	
	u8 destLen = MIN(strlen(msg->Dest), MSG_MAX_DEST_LEN);
	Serial::Write(handler->Interface, &destLen, 1);
	u8 labelLen = MIN(strlen(msg->Label), MSG_MAX_LABEL_LEN);
	Serial::Write(handler->Interface, &labelLen, 1);
	u16 contentLen = MIN(msg->ContentLen, MSG_MAX_CONTENT_LEN);
	Serial::Write(handler->Interface, &contentLen, 2);

	Serial::Write(handler->Interface, msg->Dest, destLen);
	Serial::Write(handler->Interface, msg->Label, labelLen);
	Serial::Write(handler->Interface, msg->Content, contentLen);
	
}

void PrintMessage(Message *msg)
{
	PRINT("%s->%s \"%s\"\n", msg->Label, msg->Dest, msg->Content);
}
void LogMessage(Message *msg)
{
	LOG("%s->%s \"%s\"\n", msg->Label, msg->Dest, msg->Content);
}

