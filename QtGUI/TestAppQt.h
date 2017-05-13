#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TestAppQt.h"
#include <QtCharts/QtCharts>


#include <XCPMaster.h>
#include <ResponsePacket.h>
#include "IncomingHandlerExternal.h"
#include <map>
#include "SeriesProperties.h"

using namespace QtCharts;

class TestAppQt : public QMainWindow
{
    Q_OBJECT

public:
    TestAppQt(QWidget *parent = Q_NULLPTR);
	virtual ~TestAppQt();

	public slots:
	///void connected();
	//void disconnected();
	//void readyRead();
	void ConfigMeasurementButtonPressed();
	void TestButtonPressed();
	//void AddPoint(unsigned int series, double point);
	void AddPointToSeries(uint16_t, uint8_t, uint32_t, double, double);
	void MeasurementFinished();
	void FirstMeasurementArrived(uint32_t timestamp);
	void SliderValueChanged(int value);
private:
    Ui::TestAppQtClass ui;
	QChart* chart;
	std::vector<QLineSeries*> SeriesVector;
	std::map<std::tuple<uint16_t, uint8_t, uint32_t>, SeriesProperties> ChartSeries; //daq id, odt id, entry id
	XCPMaster* master;
	XCPWorkerThread* thread;
	unsigned int NumOfPoints = 0;

	int LoadDLL();
	typedef uint32_t(*XCP_GetAvailablePrivilegesPtr_t)(uint8_t* AvailablePrivilege);
	typedef uint32_t(*XCP_ComputeKeyFromSeedPtr_t)(uint8_t RequestedPrivilege, uint8_t ByteLenghtSeed, uint8_t* PointerToSeed, uint8_t* ByteLengthKey, uint8_t* PointerToKey);

	XCP_GetAvailablePrivilegesPtr_t GetAvailablePrivileges = nullptr;
	XCP_ComputeKeyFromSeedPtr_t ComputeKeyFromSeed = nullptr;
	
	int SetupConnection(SOCKET& s);

	IncomingHandlerExternal* Handler;

	uint32_t m_ChartXMin;
	uint32_t m_ChartXMax;

};
