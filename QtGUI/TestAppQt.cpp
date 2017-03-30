#include "TestAppQt.h"
#include "XCPWorkerThread.h"

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


TestAppQt::TestAppQt(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.TestSend, SIGNAL(clicked()), this, SLOT(TestButtonPressed()));


	LoadDLL();
	master = new XCPMaster(TransportLayer::ETHERNET);
	master->SetSeedAndKeyFunctionPointers(GetAvailablePrivileges, ComputeKeyFromSeed);
	thread = new XCPWorkerThread(master);
	connect(thread, SIGNAL(NotifyUI(unsigned int,double)),this,
		SLOT(AddPoint(unsigned int,double)));
	connect(thread, SIGNAL(Finished()), this, SLOT(MeasurementFinished()));

	//connect(thread, SIGNAL(NotifyUI(double)), this, SLOT(AddPoint(double)));
	Handler = new IncomingHandlerExternal(thread);
	master->SetExternalMessageHandler(Handler);


	SeriesArray[0] = new QLineSeries();
	SeriesArray[0]->setColor(QColor(0, 255, 0));

	SeriesArray[1] = new QLineSeries();
	SeriesArray[1]->setColor(QColor(255, 0, 0));

	QChart* chart = new QChart();
	chart->legend()->hide();
	chart->addSeries(SeriesArray[0]);
	chart->addSeries(SeriesArray[1]);

	chart->createDefaultAxes();
	chart->setTitle("Measurement");
	chart->axisX()->setRange(0, 1000);
	chart->axisY()->setRange(-150, 150);
	
	QChartView* chartView = new QChartView(chart);
	chartView->setRenderHint(QPainter::Antialiasing);


	QGridLayout* layout = new QGridLayout();
	layout->addWidget(chartView);
	ui.ChartWidget->setLayout(layout);
	//setCentralWidget(chartView);
}

TestAppQt::~TestAppQt()
{
	delete SeriesArray[0];
	delete SeriesArray[1];

	delete master;
	delete Handler;
	delete thread;
	//Cleanup(s);
}

void TestAppQt::AddPoint(unsigned int series, double point)
{
	SeriesArray[series]->append((NumOfPoints++)/2, point);
}

void TestAppQt::MeasurementFinished()
{
	ui.TestSend->setDisabled(false);
	std::cout << "num of points: " << std::dec<<NumOfPoints << "\n";
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

void TestAppQt::TestButtonPressed()
{
	ui.TestSend->setDisabled(true);
	NumOfPoints = 0;
	SeriesArray[0]->clear();
	SeriesArray[1]->clear();
	thread->start();
}
