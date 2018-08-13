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

	LoadDLL(); //Loads the user provided dll with the Seed&Key algorithms. Sets up the GetAvailablePrivileges and ComputeKeyFromSeed function pointers.
	master = new XCPMaster(TransportLayer::ETHERNET);
	master->SetSeedAndKeyFunctionPointers(
		GetAvailablePrivileges, //function pointer for the GetAvailablePrivileges 
		ComputeKeyFromSeed); //function pointer for the ComputeKeyFromSeed 
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
