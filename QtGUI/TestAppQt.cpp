#include "TestAppQt.h"
#include "XCPWorkerThread.h"
#include "ConfigureMeasurementQt.h"

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
	connect(ui.MeasurementBtn, SIGNAL(clicked()), this, SLOT(ConfigMeasurementButtonPressed()));

	LoadDLL();
	master = new XCPMaster(TransportLayer::ETHERNET);
	master->SetSeedAndKeyFunctionPointers(GetAvailablePrivileges, ComputeKeyFromSeed);
	thread = new XCPWorkerThread(master);
	/*connect(thread, SIGNAL(NotifyUI(unsigned int, double)), this,
		SLOT(AddPoint(unsigned int, double)));*/
	connect(thread, SIGNAL(NotifyUI(uint16_t, uint8_t, uint32_t, double, double)), this, SLOT(AddPointToSeries(uint16_t, uint8_t, uint32_t, double, double)));
	connect(thread, SIGNAL(Finished()), this, SLOT(MeasurementFinished()));
	Handler = new IncomingHandlerExternal(thread);
	master->SetExternalMessageHandler(Handler);

	chart = new QChart();
	chart->legend()->hide();
	chart->setTitle("Measurement");
	QChartView* chartView = new QChartView(chart);
	chartView->setRenderHint(QPainter::Antialiasing);

	QGridLayout* layout = new QGridLayout();
	layout->addWidget(chartView);
	ui.ChartWidget->setLayout(layout);
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
/*
void TestAppQt::AddPoint(unsigned int series, double point)
{
	SeriesVector[series]->append((NumOfPoints++) / 2, point);
}
*/
void TestAppQt::MeasurementFinished()
{
	ui.TestSend->setDisabled(false);
	ui.MeasurementBtn->setDisabled(false);
	std::cout << "Measurement complete!\n";
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
	ConfigureMeasurementQt* d = new ConfigureMeasurementQt(this);
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

		chart->createDefaultAxes();
		chart->setTitle("Measurement");
		chart->axisX()->setRange(0, 1000);
		chart->axisY()->setRange(-150, 150);

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
	SeriesVector[ChartSeries[{DAQId, ODTId, EntryId}].SeriesIndex]->append(x, y);
}
