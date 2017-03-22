#pragma once
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <XCPMaster.h>
#include <ResponsePacket.h>

class MainWindow :
	public wxApp
{
	using XCPMsgPtr = std::unique_ptr<IXCPMessage>;
	XCPMaster* master;
	int MaxRecvsize = 0xff;
	SOCKET s;
	int LoadDLL();
	typedef uint32_t(*XCP_GetAvailablePrivilegesPtr_t)(uint8_t* AvailablePrivilege);
	typedef uint32_t(*XCP_ComputeKeyFromSeedPtr_t)(uint8_t RequestedPrivilege, uint8_t ByteLenghtSeed, uint8_t* PointerToSeed, uint8_t* ByteLengthKey, uint8_t* PointerToKey);

	XCP_GetAvailablePrivilegesPtr_t GetAvailablePrivileges = nullptr;
	XCP_ComputeKeyFromSeedPtr_t ComputeKeyFromSeed = nullptr;
	void Send(SOCKET s, XCPMsgPtr message);
	void Cleanup(SOCKET s);
	int SetupConnection(SOCKET& s);

	void TestXCP();
public:
	MainWindow();
	virtual ~MainWindow();
	virtual bool OnInit() override;
};

