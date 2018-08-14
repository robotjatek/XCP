# XCP
An object oriented XCP (Universal Measurement and Calibration Protocol) implementation.
This library is still in "Work In Progress" it is not recommended for everyday use. 
A lots of functionalities are still missing, and the rest is not extensively tested.

[![Build status](https://ci.appveyor.com/api/projects/status/nhffe3hghggmvqqs/branch/master?svg=true)](https://ci.appveyor.com/project/robotjatek/xcp/branch/master)


## Features
- Connect/Disconnect
- Seed&Key (with user provided functions for key computing)
- Static and Dynamic DAQ
- Ethernet and CAN message formats
- Polling via UPLOAD and UPLOAD_SHORT packets
- Getting information from the slave device
- Easy to extend

## Limitations
- STIM direction is not supported
- DOWNLOAD and DOWNLOAD_SHORT packets are not supported at the moment (WIP)
- Block transfer mode is not supported
- Resume mode is not supported

## Implemented (outgoing) packets
- CONNECT
- DISCONNECT
- GET_STATUS
- SYNCH
- GET_SEED
- UNLOCK
- SET_MTA
- UPLOAD
- SHORT_UPLOAD
- SET_DAQ_PTR
- WRITE_DAQ
- SET_DAQ_LIST_MODE
- START_STOP_DAQ_LIST
- START_STOP_SYNCH
- GET_DAQ_PROCESSOR_INFO
- CLEAR_DAQ_LIST
- FREE_DAQ
- ALLOC_DAQ
- ALLOC_ODT
- ALLOC_ODT_ENTRY

## TODO
- Implement the missing mandatory commands from the standard
- DOWNLOAD/SHORT_DOWNLOAD packets
- STIM direction
- Implement the missing transport layers
- **A higher level API for some tasks (eg.: DAQ)**

## Examples

### Initializing the library
The following code will set up the master in Ethernet mode, sets the function pointers for the seed&key algorithms (described by the XCP standard), and the incoming message handler object (discussed later on).
```cpp
XCPMaster* master = new XCPMaster(TransportLayer::ETHERNET);
master->SetSeedAndKeyFunctionPointers(
	GetAvailablePrivileges, //function pointer for the GetAvailablePrivileges 
	ComputeKeyFromSeed); //function pointer for the ComputeKeyFromSeed 
IncomingHandlerExternal* Handler = new IncomingHandlerExternal(thread, master); //It is my implementation of the IIncomingMessageHandler interface. Your implementation can be different.
master->SetExternalMessageHandler(Handler);
```

### Connect to slave, then disconnect
```cpp
XCPMsgPtr connect_message = master->CreateConnectMessage(ConnectPacket::ConnectMode::NORMAL);
Send(s, std::move(connect_message));
XCPMsgPtr disconnect_message = master->CreateDisconnectMessage();
Send(s, std::move(disconnect_message));
```

### Unlock all resources on the slave
The user only needs to send the appropriate packets and provide the function pointer to the key calculator function.
```cpp
XCPMsgPtr GetSeed1 = master->CreateGetSeedMessage(GetSeedPacket::Mode::FIRST_PART, GetSeedPacket::Resource::DAQ);
Send(s, std::move(GetSeed1));
std::vector<XCPMsgPtr> UnlockMessages = master->CreateUnlockMessages();
Send(s, std::move(UnlockMessages[0]));

XCPMsgPtr GetSeed2 = master->CreateGetSeedMessage(GetSeedPacket::Mode::FIRST_PART, GetSeedPacket::Resource::CAL_PG);
Send(s, std::move(GetSeed2));
std::vector<XCPMsgPtr> UnlockMessages1 = master->CreateUnlockMessages();
Send(s, std::move(UnlockMessages1[0]));

XCPMsgPtr GetSeed3 = master->CreateGetSeedMessage(GetSeedPacket::Mode::FIRST_PART, GetSeedPacket::Resource::PGM);
Send(s, std::move(GetSeed3));
std::vector<XCPMsgPtr> UnlockMessages2 = master->CreateUnlockMessages();
Send(s, std::move(UnlockMessages2[0]));

XCPMsgPtr GetSeed4 = master->CreateGetSeedMessage(GetSeedPacket::Mode::FIRST_PART, GetSeedPacket::Resource::STIM);
Send(s, std::move(GetSeed4));
std::vector<XCPMsgPtr> UnlockMessages4 = master->CreateUnlockMessages();
Send(s, std::move(UnlockMessages4[0]));
```

### Get some data from a given memory location (polling)
The following code gets 10 elements of data from the 0x219020 memory address
```cpp
XCPMsgPtr SetMTA = master->CreateSetMTAMessage(0x219020, 0);
Send(s, std::move(SetMTA));
XCPMsgPtr Upload = master->CreateUploadMessage(10);
Send(s, std::move(Upload));
```

### Polling with only 1 message
```cpp
XCPMsgPtr ShortUpload = master->CreateShortUploadMessage(10, 0x21A08D, 0);
Send(s, std::move(ShortUpload));
```

### Setting up the DAQ layout
The following code sets up the DAQ layout to be used. This will be used to create the DAQ lists in the slave. It is always the user's responsibility to maintain the DAQ layout descriptor. The following code sets up 2 DAQ lists with 1 ODT and 1 ODT entry each.
```cpp
DAQLayout daqlayout;
DAQ daq0;
DAQ daq1;
ODT daq0odt0;
ODT daq1odt0;
ODTEntry daq0odt0entry0(0x21A1BD, 0, 1);
ODTEntry daq1odt0entry0(0x21A08D, 0, 1);

daq0odt0.AddEntry(daq0odt0entry0);
daq1odt0.AddEntry(daq1odt0entry0);
daq0.AddODT(daq0odt0);
daq1.AddODT(daq1odt0);
daq0.SetEventChannel(1);
daq0.SetMode(ModeFieldBits::TIMESTAMP);
daq0.SetPrescaler(1);
daq0.SetPriority(1);
daq1.SetEventChannel(2);
daq1.SetMode(ModeFieldBits::TIMESTAMP);
daq1.SetPrescaler(1);
daq1.SetPriority(2);
daqlayout.AddDAQ(daq0);
daqlayout.AddDAQ(daq1);
master->SetDaqLayout(daqlayout);
```

### Setting up the DAQ layout with the fluent API
It's also possible to set up the DAQ layout with a fluent API:

```cppDAQLayout daqlayout;
daqlayout.WithDAQ([](DAQ& d) -> DAQ&
{
	d.SetEventChannel(1);
	d.SetMode(ModeFieldBits::TIMESTAMP);
	d.SetPrescaler(1);
	d.SetPriority(1);

	d.WithODT([](ODT& o) -> ODT& {
		o.WithODTEntry([](ODTEntry& e) -> ODTEntry& {
			e.SetAddress(0x21A1BD);
			e.SetAddressExtension(0);
			e.SetLength(1);
			return e;
		});
		return o;
	});

	return d;
}).WithDAQ([](DAQ& d) ->  DAQ&
{
	d.SetEventChannel(2);
	d.SetMode(ModeFieldBits::TIMESTAMP);
	d.SetPrescaler(1);
	d.SetPriority(2);

	d.WithODT([](ODT& o) -> ODT& {
		o.WithODTEntry([](ODTEntry& e) -> ODTEntry& {
			e.SetAddress(0x21A08D);
			e.SetAddressExtension(0);
			e.SetLength(1);
			return e;
		});

		return o;
	});

	return d;
});
daqlayout.SetInitialized(true);
master->SetDaqLayout(daqlayout);
```

### Getting DAQ processor info
You should get the DAQ processor information by sending out this command.

```cpp
XCPMsgPtr GetDaqProcessorInfo = master->CreateGetDaqProcessorInfoMessage();
Send(s, std::move(GetDaqProcessorInfo));
```

 If it is not supported, then you have to set the properties provided by this command by hand.
 ```cpp
XCPMaster::SlaveProperties properties = m_Master.GetSlaveProperties();
properties.DaqProperies.ConfigType = [...]
properties.DaqProperies.PrescalerSupported = [...]
properties.DaqProperies.ResumeSupported = [...]
properties.DaqProperies.BitStimSupported = [...]
properties.DaqProperies.TimeStampSupported = [...]
properties.DaqProperies.PidOffSupported = [...]

properties.DaqProperies.OverloadIndicationMode = [...]
properties.DaqProperies.OptimisationType = [...]
properties.DaqProperies.AddressExtensionType = [...]
properties.DaqProperies.IdentificationFieldType = [...]

properties.DaqProperies.MaxDaq = [...]
properties.DaqProperies.MinDaq = [...]
properties.DaqProperies.MaxEventChannel = [...]

m_Master.SetSlaveProperties(properties);
```

### Setting up the DAQ layout in the slave device
After the DAQ layout descriptor was set up in the master you can set up the desired layout in the slave with the following commands.
```cpp
DAQLayout daqlayout = master->GetDaqLayout();	
if (daqlayout.IsInitialized())
{
	Send(s, master->CreateAllocDaqMessage(daqlayout.GetNumberOfDAQLists()));
	for (int i = 0; i < daqlayout.GetNumberOfDAQLists(); i++)
	{
		DAQ daq =daqlayout.GetDAQ(i);
		Send(s, master->CreateAllocOdtMessage(i, daq.GetNumberOfODTs()));
		for (int j = 0; j < daqlayout.GetDAQ(i).GetNumberOfODTs(); j++)
		{
			ODT odt = daq.GetOdt(j);
			Send(s, master->CreateAllocOdtEntryMessage(i, j, odt.GetNumberOfEntries()));
		}
	}
    	//The XCP simulator crashes if I merge these two loops...
	for (int i = 0; i < daqlayout.GetNumberOfDAQLists(); i++)
	{
		DAQ daq = daqlayout.GetDAQ(i);
		for (int j = 0; j < daqlayout.GetDAQ(i).GetNumberOfODTs(); j++)
		{
			ODT odt = daq.GetOdt(j);
			for (int k = 0; k < daqlayout.GetDAQ(i).GetOdt(j).GetNumberOfEntries(); k++)
			{
				ODTEntry entry = odt.GetEntry(k);
				Send(s, master->CreateSetDaqPtrMessage(i, j, k));
				Send(s, master->CreateWriteDaqMessage(0xFF, entry.GetLength(), entry.GetAddressExtension(), entry.GetAddress()));
			}
		}
	}
}
```

### Selecting and starting the measurement on the desired DAQ lists
```cpp
for (int id = 0; id < daqlayout.GetNumberOfDAQLists(); id++)
{
	XCPMsgPtr SetDaqListMode = master->CreateSetDaqListModeMessage(daqlayout.GetDAQ(id).GetMode(), id, daqlayout.GetDAQ(id).GetEventChannel(), daqlayout.GetDAQ(id).GetPrescaler(), daqlayout.GetDAQ(id).GetPriority());
	Send(s, std::move(SetDaqListMode));
	XCPMsgPtr StartStopDaqList = master->CreateStartStopDaqListMessage(StartStopDaqListPacket::Mode::SELECT, id);
	Send(s, std::move(StartStopDaqList));
}

XCPMsgPtr StartStopSynch = master->CreateStartStopSynchMessage(StartStopSynchPacket::Mode::START_SELECTED);
Send(s, std::move(StartStopSynch));
```
After that, the slave will send out DAQ packets until you tell it to stop. The following code will get for about 1000 DAQ packets from the slave (using Qt sockets). You have to explicitly call **Master::DeserializeMessage** to decode these packets.
```cpp
int i = 0;
while (i < 1000)
{
	if (s->waitForReadyRead(-1))
	{
		bytes.resize(MaxRecvsize);
		int recv_size = s->read((char*)&bytes[0], MaxRecvsize);
		bytes.resize(recv_size);
		for (int i = 0; i < recv_size; i++)
		{
			std::cout << std::hex << (int)(bytes[i] & 0xff) << " ";
		}
		std::cout << "\n";
		while (bytes.size())
		{
			XCPMsgPtr asd = master->DeserializeMessage(bytes);
		}
		i++;
	}
}

XCPMsgPtr Stop = master->CreateStartStopSynchMessage(StartStopSynchPacket::Mode::STOP_ALL);
Send(s, std::move(Stop));
```

### Handling incoming packets from slave
Some of the incoming responses are handled internally by the library (eg.: ConnectResponse), but there are some responses that you are most definitely want to handle by yourself. (eg.: incoming DAQ packets, UploadResponse, various error messages, etc.)
For that you have to implement the **IIncomingMessageHandler** interface. After you implemented the interface, you'll have to tell the master to use your implementation. It is up to you how to handle the incoming packets. Every incoming packet will be deserialized by the master, then the appropriate handler function will be called.
```cpp
IncomingHandlerExternal* Handler = new IncomingHandlerExternal(thread, master);
master->SetExternalMessageHandler(Handler);
```
(There is not any interface for the constructor of the MessageHandler, so it is up to you to call SetExternalMessageHandler separately or call it in the Handler's constructor. In this example the SetExternalMessageHandler function was called outside of the handler's constructor)

