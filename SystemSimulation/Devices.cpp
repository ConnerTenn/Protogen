
#include "Devices.h"

void *HeadControllerEntry(void *data);
void *EmotionControllerEntry(void *data);

Device DeviceList[] =
{
	{ 
		.Name="HeadController",
		.Entry={0,HeadControllerEntry},
		.Interfaces={ new GPIO(101), new Serial(201) },
	},
	{ 
		.Name="EmotionController",
		.Entry={0,EmotionControllerEntry},
		.Interfaces={ new GPIO(102), new Serial(202) },
	},
};

std::vector<u32> ConnectionMatrix[] =
{
	{101, 102},
	{201, 201},
};

Interface_t FindInterface(u32 ID)
{
	for (size_t i=0; i<ARRAYLEN(DeviceList); i++)
	{
		for (size_t j=0; j<DeviceList[i].Interfaces.size(); j++)
		{
			if (DeviceList[i].Interfaces[j]->InterfaceID == ID)
			{
				return DeviceList[i].Interfaces[j];
			}
		}
	}
	return 0;
}

void InitDevices()
{
	PRINTENTERFUNC

	for (size_t i=0; i<ARRAYLEN(ConnectionMatrix); i++)
	{
		for (size_t j=0; j<ConnectionMatrix[i].size(); j++)
		{
			Interface_t iface = FindInterface(ConnectionMatrix[i][j]);
			if (!iface) { PRINTF("Interface Not Found: %d\n", ConnectionMatrix[i][j]); }
			else
			{
				for (size_t k=0; k<ConnectionMatrix[i].size(); k++)
				{
					if (k!=j)
					{
						iface->Connections.push_back(FindInterface(ConnectionMatrix[i][k]));
					}
				}
			}
		}
	}

	for (size_t i = 0; i < ARRAYLEN(DeviceList); i++)
	{
		Device *device = &DeviceList[i];
		pthread_create(&(device->Entry.ID), 0, device->Entry.Entry, &Run);
	}

	PRINTRETFUNC
}

void DestroyDevices()
{
	PRINTENTERFUNC

	for (size_t i = 0; i < ARRAYLEN(DeviceList); i++)
	{
		pthread_join(DeviceList[i].Entry.ID, 0);
		for (size_t j=0; j<DeviceList[i].Interfaces.size(); j++)
		{
			delete DeviceList[i].Interfaces[j];
		}
	}

	PRINTRETFUNC
}

Interface_t Device::Open(u32 interfaceID)
{
	for (size_t i=0; i<Interfaces.size(); i++)
	{
		if (Interfaces[i]->InterfaceID==interfaceID) { return Interfaces[i]; }
	}
	return 0;
}

void Device::Close(Interface_t interface)
{
}
