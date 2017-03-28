#pragma once

#include <memory>
#include <XCPMaster.h>
#include <qtcpsocket.h>
#include <qthread.h>

using XCPMsgPtr = std::unique_ptr<IXCPMessage>;


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
	void AddPoint(double point);

	signals:
	void NotifyUI(double point);
	void Finished();
};
