
#include "Messaging.h"

u16 CalcCRC(Message *msg)
{
	u8 crc = 0;
	for (u64 i=0; i<strlen(msg->Dest); i++)
	{
		crc = crc ^ msg->Dest[i];
	}
	for (u64 i=0; i<strlen(msg->Label); i++)
	{
		crc = crc ^ msg->Label[i];
	}
	for (u64 i=0; i<MSG_CONTENT_LEN; i++)
	{
		crc = crc ^ msg->Content[i];
	}
	return crc;
}

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
	memset(&handler->Buffer, 1, sizeof(Message));
	//handler->BuffHead = 0; handler->BuffTail = 0;
}
void DestroyMessageHandler(MessageHandler *handler)
{

}


bool GetNextMessage(MessageHandler *handler, Message *msg)
{
	u8 buff[MSG_TOTAL_LEN*2];
	if (Serial::Available(handler->Interface))
	{
		u64 len = Serial::Read(handler->Interface, buff, MSG_TOTAL_LEN*2);
		
		for (u64 i=0; i<len; i++)
		{
			if (buff[i]==STARTBYTE) 
			{ 
				memset(&handler->Buffer, 1, sizeof(Message));
				handler->DestFill=0;
				handler->LabelFill=0;
				handler->ContentFill=0;
				handler->BufferCRC=0;
				handler->State=MessageHandler::RecvDest; 
			}
			else if (buff[i] == 0 && handler->State==MessageHandler::RecvDest) 
			{
				handler->State=MessageHandler::RecvLabel; 
			}
			else if (buff[i] == STARTCONTENTBYTE) 
			{
				handler->State=MessageHandler::RecvContent; 
			}
			else if (buff[i] == CRCBYTE) 
			{ 
				handler->State=MessageHandler::RecvCRC; 
			}
			else if (buff[i] == ENDBYTE)
			{
				handler->Buffer.Dest[MIN(handler->DestFill, MSG_DEST_LEN-1)] = 0;
				handler->Buffer.Label[MIN(handler->LabelFill, MSG_DEST_LEN-1)] = 0;
				memset(handler->Buffer.Content+handler->ContentFill, MAX(MSG_DEST_LEN-handler->ContentFill, 0),1);

				if (handler->BufferCRC == CalcCRC(&handler->Buffer))
				{
					PostMessage(handler);
				}
				else { LOGERROR("Msg Corruption\n"); }
			}
			else
			{
				if (handler->State==MessageHandler::RecvDest)
				{
					handler->Buffer.Dest[handler->DestFill]=buff[i];
					handler->DestFill++;
				}
				else if (handler->State==MessageHandler::RecvLabel)
				{
					handler->Buffer.Label[handler->LabelFill]=buff[i];
					handler->LabelFill++;
				}
				else if (handler->State==MessageHandler::RecvContent)
				{
					handler->Buffer.Content[handler->ContentFill]=buff[i];
					handler->ContentFill++;
				}
				else if (handler->State==MessageHandler::RecvCRC)
				{
					handler->BufferCRC |= buff[i];
				}
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
	Serial::Write(handler->Interface, msg->Dest, strlen(msg->Dest));
	ctl = 0x0;
	Serial::Write(handler->Interface, &ctl, 1);
	Serial::Write(handler->Interface, msg->Label, strlen(msg->Label));
	ctl = STARTCONTENTBYTE;
	Serial::Write(handler->Interface, &ctl, 1);
	Serial::Write(handler->Interface, msg->Content, MSG_CONTENT_LEN);
	ctl = CRCBYTE;
	Serial::Write(handler->Interface, &ctl, 1);
	ctl = CalcCRC(msg);
	Serial::Write(handler->Interface, &ctl, 1);
	ctl = ENDBYTE;
	Serial::Write(handler->Interface, &ctl, 1);
}

void PrintMessage(Message *msg)
{
	PRINT("%s->%s \"%s\"\n", msg->Label, msg->Dest, msg->Content);
}
void LogMessage(Message *msg)
{
	LOG("%s->%s \"%s\"\n", msg->Label, msg->Dest, msg->Content);
}

