#include "TestAppQt.h"
#include "XCPWorkerThread.h"
#include "ConfigureMeasurementQt.h"
#include "vxlapi.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifdef XCPLIB_EXPORTS
#define XCP_API    __declspec(dllexport)
#else
#define XCP_API    __declspec(dllimport)
#endif

#include <tchar.h>
#include <windows.h>

#include <vector>
//#include <vld.h>

#define RECEIVE_EVENT_SIZE 1                // DO NOT EDIT! Currently 1 is supported only
#define RX_QUEUE_SIZE      4096             // internal driver queue size in CAN events
#define UNUSED_PARAM(a) { a=a; }

XLstatus          xlStatus;
unsigned int      i;
XLaccess          xlChannelMaskFd = 0;
XLdriverConfig  g_xlDrvConfig;
XLaccess        g_xlChannelMask = 0;
XLaccess      xlChanMaskTx = 0;
XLaccess* pxlChannelMaskTx = &xlChanMaskTx;
unsigned int  xlChanIndex = 0;
unsigned int* pxlChannelIndex = &xlChanIndex;
unsigned int    g_canFdSupport = 0;
XLaccess        g_xlPermissionMask = 0;
XLportHandle    g_xlPortHandle = XL_INVALID_PORTHANDLE;
char            g_AppName[XL_MAX_LENGTH + 1] = "QTCAN";
#define RX_QUEUE_SIZE      4096
unsigned int    g_BaudRate = 500000;

XLhandle        g_hMsgEvent;                                          //!< notification handle for the receive queue
HANDLE          g_hRXThread;                                          //!< thread handle (RX)
HANDLE          g_hTXThread;                                          //!< thread handle (TX)
int             g_RXThreadRun;                                        //!< flag to start/stop the RX thread
int             g_TXThreadRun;                                        //!< flag to start/stop the TX thread (for the transmission burst)
int             g_RXCANThreadRun;                                     //!< flag to start/stop the RX thread
DWORD     WINAPI RxCanFdThread(PVOID par);
DWORD     WINAPI RxThread(PVOID par);
DWORD     WINAPI TxThread(LPVOID par);
int             g_silent = 0;

DWORD WINAPI RxThread(LPVOID par)
{
	XLstatus        xlStatus;

	unsigned int    msgsrx = RECEIVE_EVENT_SIZE;
	XLevent         xlEvent;

	UNUSED_PARAM(par);

	g_RXThreadRun = 1;

	while (g_RXThreadRun) {

		WaitForSingleObject(g_hMsgEvent, 10);

		xlStatus = XL_SUCCESS;

		while (!xlStatus) {

			msgsrx = RECEIVE_EVENT_SIZE;

			xlStatus = xlReceive(g_xlPortHandle, &msgsrx, &xlEvent);
			if (xlStatus != XL_ERR_QUEUE_IS_EMPTY) {

				if (!g_silent) {
					printf("%s\n", xlGetEventString(&xlEvent));
				}

			}
		}

	}
	return NO_ERROR;
}


DWORD WINAPI RxCanFdThread(LPVOID par)
{
	XLstatus        xlStatus = XL_SUCCESS;
	DWORD           rc;
	XLcanRxEvent    xlCanRxEvt;

	UNUSED_PARAM(par);

	g_RXCANThreadRun = 1;

	while (g_RXCANThreadRun) {
		rc = WaitForSingleObject(g_hMsgEvent, 10);
		if (rc != WAIT_OBJECT_0) continue;

		do {
			xlStatus = xlCanReceive(g_xlPortHandle, &xlCanRxEvt);
			if (xlStatus == XL_ERR_QUEUE_IS_EMPTY) {
				break;
			}
			if (!g_silent) {
				printf("%s\n", xlCanGetEventString(&xlCanRxEvt));
			}

		} while (XL_SUCCESS == xlStatus);
	}

	return(NO_ERROR);
} // RxCanFdThread


TestAppQt::TestAppQt(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.TestSend, SIGNAL(clicked()), this, SLOT(TestButtonPressed()));
	connect(ui.MeasurementBtn, SIGNAL(clicked()), this, SLOT(ConfigMeasurementButtonPressed()));

	LoadDLL();
	master = new XCPMaster(TransportLayer::ETHERNET);
	master->SetSeedAndKeyFunctionPointers(GetAvailablePrivileges, ComputeKeyFromSeed);
	thread = new XCPWorkerThread(master);

	connect(thread, SIGNAL(NotifyUI(uint16_t, uint8_t, uint32_t, double, double)), this, SLOT(AddPointToSeries(uint16_t, uint8_t, uint32_t, double, double)));
	connect(thread, SIGNAL(Finished()), this, SLOT(MeasurementFinished()));
	connect(thread, SIGNAL(SetChartXAxisStart(uint32_t)), this, SLOT(FirstMeasurementArrived(uint32_t)));
	connect(ui.verticalSlider, SIGNAL(valueChanged(int)), this, SLOT(SliderValueChanged(int)));
	Handler = new IncomingHandlerExternal(thread, master);
	master->SetExternalMessageHandler(Handler);

	chart = new QChart();
	chart->legend()->hide();
	chart->setTitle("Measurement");
	chart->createDefaultAxes();
	QChartView* chartView = new QChartView(chart);
	chartView->setRenderHint(QPainter::Antialiasing);

	QGridLayout* layout = new QGridLayout();
	layout->addWidget(chartView);
	ui.ChartWidget->setLayout(layout);
	m_ChartXMin = 0;
	m_ChartXMax = 0;

	

	// ------------------------------------
	// open the driver
	// ------------------------------------
	xlStatus = xlOpenDriver();

	// ------------------------------------
	// get/print the hardware configuration
	// ------------------------------------
	if (XL_SUCCESS == xlStatus) {
		xlStatus = xlGetDriverConfig(&g_xlDrvConfig);
	}

	if (XL_SUCCESS == xlStatus) {
	

		// ------------------------------------
		// select the wanted channels
		// ------------------------------------
		g_xlChannelMask = 0;
		for (i = 0; i < g_xlDrvConfig.channelCount; i++) {

			// we take all hardware we found and supports CAN
			if (g_xlDrvConfig.channel[i].channelBusCapabilities & XL_BUS_ACTIVE_CAP_CAN) {

				if (!*pxlChannelMaskTx) {
					*pxlChannelMaskTx = g_xlDrvConfig.channel[i].channelMask;
					*pxlChannelIndex = g_xlDrvConfig.channel[i].channelIndex;
				}

				// check if we can use CAN FD - the virtual CAN driver supports CAN-FD, but we don't use it
				if (g_xlDrvConfig.channel[i].channelCapabilities & XL_CHANNEL_FLAG_CANFD_ISO_SUPPORT) {
					xlChannelMaskFd |= g_xlDrvConfig.channel[i].channelMask;
				}
				else {
					g_xlChannelMask |= g_xlDrvConfig.channel[i].channelMask;
				}

			}
		}

		// if we found a CAN FD supported channel - we use it.
		if (xlChannelMaskFd) {
			g_xlChannelMask = xlChannelMaskFd;
			printf("- Use CAN-FD for   : CM=0x%I64x\n", g_xlChannelMask);
			g_canFdSupport = 1;
		}

		if (!g_xlChannelMask) {
			printf("ERROR: no available channels found! (e.g. no CANcabs...)\n\n");
			xlStatus = XL_ERROR;
		}
	}

	g_xlPermissionMask = g_xlChannelMask;

	// ------------------------------------
	// open ONE port including all channels
	// ------------------------------------
	if (XL_SUCCESS == xlStatus) {

		// check if we can use CAN FD
		if (g_canFdSupport) {
			xlStatus = xlOpenPort(&g_xlPortHandle, g_AppName, g_xlChannelMask, &g_xlPermissionMask, 16000, XL_INTERFACE_VERSION_V4, XL_BUS_TYPE_CAN);
		}
		// if not, we make 'normal' CAN
		else {
			xlStatus = xlOpenPort(&g_xlPortHandle, g_AppName, g_xlChannelMask, &g_xlPermissionMask, RX_QUEUE_SIZE, XL_INTERFACE_VERSION, XL_BUS_TYPE_CAN);

		}
		printf("- OpenPort         : CM=0x%I64x, PH=0x%02X, PM=0x%I64x, %s\n",
			g_xlChannelMask, g_xlPortHandle, g_xlPermissionMask, xlGetErrorString(xlStatus));

	}

	if ((XL_SUCCESS == xlStatus) && (XL_INVALID_PORTHANDLE != g_xlPortHandle)) {

		// ------------------------------------
		// if we have permission we set the
		// bus parameters (baudrate)
		// ------------------------------------
		if (g_xlChannelMask == g_xlPermissionMask) {

			if (g_canFdSupport) {
				XLcanFdConf fdParams;

				memset(&fdParams, 0, sizeof(fdParams));

				// arbitration bitrate
				fdParams.arbitrationBitRate = 1000000;
				fdParams.tseg1Abr = 6;
				fdParams.tseg2Abr = 3;
				fdParams.sjwAbr = 2;

				// data bitrate
				fdParams.dataBitRate = fdParams.arbitrationBitRate * 2;
				fdParams.tseg1Dbr = 6;
				fdParams.tseg2Dbr = 3;
				fdParams.sjwDbr = 2;

				xlStatus = xlCanFdSetConfiguration(g_xlPortHandle, g_xlChannelMask, &fdParams);
				printf("- SetFdConfig.     : ABaudr.=%u, DBaudr.=%u, %s\n", fdParams.arbitrationBitRate, fdParams.dataBitRate, xlGetErrorString(xlStatus));

			}
			else {
				xlStatus = xlCanSetChannelBitrate(g_xlPortHandle, g_xlChannelMask, g_BaudRate);
				printf("- SetChannelBitrate: baudr.=%u, %s\n", g_BaudRate, xlGetErrorString(xlStatus));
			}
		}
		else {
			printf("-                  : we have NO init access!\n");
		}
	}
	else {

		xlClosePort(g_xlPortHandle);
		g_xlPortHandle = XL_INVALID_PORTHANDLE;
		xlStatus = XL_ERROR;
	}

	xlStatus = XL_ERROR;
	DWORD         ThreadId = 0;
	int           activated = 0;

	if (g_xlPortHandle != XL_INVALID_PORTHANDLE) {

		// Send a event for each Msg!!!
		xlStatus = xlSetNotification(g_xlPortHandle, &g_hMsgEvent, 1);

		if (g_canFdSupport) {
			g_hRXThread = CreateThread(0, 0x1000, RxCanFdThread, (LPVOID)0, 0, &ThreadId);
		}
		else {
			g_hRXThread = CreateThread(0, 0x1000, RxThread, (LPVOID)0, 0, &ThreadId);
		}

	}

	xlStatus = xlActivateChannel(g_xlPortHandle, g_xlChannelMask, XL_BUS_TYPE_CAN, XL_ACTIVATE_RESET_CLOCK);
	printf("- ActivateChannel  : CM=0x%I64x, %s\n", g_xlChannelMask, xlGetErrorString(xlStatus));
	if (xlStatus == XL_SUCCESS) activated = 1;


	unsigned int         messageCount = 1;
	static int           cnt = 0;
	unsigned int  txID = 0x01;

	if (g_canFdSupport) {
		unsigned int  fl[7] = {

			0 , // CAN (no FD)
			XL_CAN_TXMSG_FLAG_EDL,
			XL_CAN_TXMSG_FLAG_EDL | XL_CAN_TXMSG_FLAG_BRS,
			XL_CAN_TXMSG_FLAG_EDL,
			XL_CAN_TXMSG_FLAG_EDL,
			XL_CAN_TXMSG_FLAG_EDL | XL_CAN_TXMSG_FLAG_BRS,
			XL_CAN_TXMSG_FLAG_EDL | XL_CAN_TXMSG_FLAG_BRS

		};

		XLcanTxEvent canTxEvt;
		unsigned int cntSent;
		unsigned int i;

		memset(&canTxEvt, 0, sizeof(canTxEvt));
		canTxEvt.tag = XL_CAN_EV_TAG_TX_MSG;
		canTxEvt.transId = 0xffff;

		canTxEvt.tagData.canMsg.canId = txID;
		canTxEvt.tagData.canMsg.msgFlags = fl[cnt % (sizeof(fl) / sizeof(fl[0]))];
		canTxEvt.tagData.canMsg.dlc = 8;//0x0f;

		++cnt;

		for (i = 1; i<XL_CAN_MAX_DATA_LEN; ++i) {
			canTxEvt.tagData.canMsg.data[i] = (unsigned char)i - 1;
		}
		canTxEvt.tagData.canMsg.data[0] = (unsigned char)cnt;
		xlStatus = xlCanTransmitEx(g_xlPortHandle, xlChanMaskTx, messageCount, &cntSent, &canTxEvt);
	}
	else {
		static XLevent       xlEvent;
		printf("sadasdasd");
		memset(&xlEvent, 0, sizeof(xlEvent));

		xlEvent.tag = XL_TRANSMIT_MSG;
		xlEvent.tagData.msg.id = txID;
		xlEvent.tagData.msg.dlc = 8;
		xlEvent.tagData.msg.flags = 0;
		++xlEvent.tagData.msg.data[0];
		xlEvent.tagData.msg.data[1] = 2;
		xlEvent.tagData.msg.data[2] = 3;
		xlEvent.tagData.msg.data[3] = 4;
		xlEvent.tagData.msg.data[4] = 5;
		xlEvent.tagData.msg.data[5] = 6;
		xlEvent.tagData.msg.data[6] = 7;
		xlEvent.tagData.msg.data[7] = 8;

		xlStatus = xlCanTransmit(g_xlPortHandle, xlChanMaskTx, &messageCount, &xlEvent);

	}

	printf("- Transmit         : CM(0x%I64x), %s\n", xlChanMaskTx, xlGetErrorString(xlStatus));

	xlCloseDriver();
}

TestAppQt::~TestAppQt()
{
	for (auto a : SeriesVector)
	{
		delete a;
	}

	delete master;
	delete Handler;
	delete thread;
}

void TestAppQt::MeasurementFinished()
{
	ui.TestSend->setDisabled(false);
	ui.MeasurementBtn->setDisabled(false);
	std::cout << "Measurement complete!\nResizing chart axis...";
	if (this->SeriesVector.size())
	{
		chart->axisX()->setRange(m_ChartXMin, m_ChartXMax);
		chart->update();
	}
}

void TestAppQt::FirstMeasurementArrived(uint32_t timestamp)
{
	m_ChartXMin = timestamp;
	chart->axisX()->setRange(timestamp, timestamp + 110000);
}

void TestAppQt::SliderValueChanged(int value)
{
	chart->axisY()->setRange((int32_t)0-value, (int32_t)0+value);
	chart->update();
}

int TestAppQt::LoadDLL()
{
	QString str("SeedNKeyXcp.dll");
	LPCWSTR str1 = (const wchar_t*)str.utf16();
	HINSTANCE hGetProcIDDLL = LoadLibrary(str1);
	if (!hGetProcIDDLL) {
		std::cout << "could not load the dynamic library" << std::endl;
		return -1;
	}

	GetAvailablePrivileges = (XCP_GetAvailablePrivilegesPtr_t)GetProcAddress(hGetProcIDDLL, "XCP_GetAvailablePrivileges");
	if (!GetAvailablePrivileges) {
		std::cout << "could not locate the function" << std::endl;
		return -1;
	}

	ComputeKeyFromSeed = (XCP_ComputeKeyFromSeedPtr_t)GetProcAddress(hGetProcIDDLL, "XCP_ComputeKeyFromSeed");
	if (!GetAvailablePrivileges) {
		std::cout << "could not locate the function" << std::endl;
		return -1;
	}

	return 0;
}

void TestAppQt::ConfigMeasurementButtonPressed()
{
	ConfigureMeasurementQt* d = new ConfigureMeasurementQt(ChartSeries, master->GetDaqLayout(),this);
	if (d->exec() == QDialog::Accepted)
	{
		this->master->SetDaqLayout(d->GetDaqLayout());
		this->ChartSeries = d->GetChartSeries();

		if (SeriesVector.size())
		{
			for (int i = 0; i < SeriesVector.size(); i++)
			{
				chart->removeSeries(SeriesVector[i]);
				delete SeriesVector[i];
			}
			SeriesVector.clear();
		}

		for (int i = 0; i < ChartSeries.size(); i++)
		{
			SeriesVector.push_back(new QLineSeries());
			chart->addSeries(SeriesVector[i]);
		}
		for (auto it = ChartSeries.begin(); it != ChartSeries.end(); it++)
		{
			SeriesVector[it->second.SeriesIndex]->setColor(QColor(it->second.r, it->second.g, it->second.b));
			/*QPen p;
			p.setWidth(2);
			p.setColor(QColor(it->second.r, it->second.g, it->second.b));
			SeriesVector[it->second.SeriesIndex]->setPen(p);*/
		}

		if (SeriesVector.size())
		{
			chart->createDefaultAxes();
			chart->setTitle("Measurement");
			chart->axisX()->setRange(0, 1000);
			chart->axisY()->setRange(-150, 150);
		}

	}
	else
	{

	}
	delete d;
}

void TestAppQt::TestButtonPressed()
{
	ui.TestSend->setDisabled(true);
	ui.MeasurementBtn->setDisabled(true);
	NumOfPoints = 0;
	Handler->Reset();
	for (auto a : SeriesVector)
	{
		a->clear();
	}
	thread->start();
}

void TestAppQt::AddPointToSeries(uint16_t DAQId, uint8_t ODTId, uint32_t EntryId, double x, double y)
{
	m_ChartXMax = x;
	SeriesVector[ChartSeries[{DAQId, ODTId, EntryId}].SeriesIndex]->append(x, y);
}
