#pragma once

#include <memory>
#include <XCPMaster.h>
#include <qtcpsocket.h>
#include <qthread.h>




class XCPWorkerThread : public QThread
{
	Q_OBJECT
private:
	XCPMaster* master;
	QTcpSocket* s;
	void TestXCP();
	std::vector<uint8_t> bytes;
	int MaxRecvsize = 0xff;
	void Send(QTcpSocket* s, XCPMsgPtr message);
public:
	XCPWorkerThread(XCPMaster* master);
	virtual ~XCPWorkerThread();
	void run();
	void AddPoint(unsigned int series, double point);
	void AddPointToSeries(uint16_t DAQId, uint8_t ODTId, uint32_t EntryId, double x, double y);
	void FirstMeasurementArrived(uint32_t timestamp);
	signals:
	//void NotifyUI(unsigned int series, double point);
	void NotifyUI(uint16_t, uint8_t, uint32_t, double, double);
	void Finished();
	void SetChartXAxisStart(uint32_t);
};
