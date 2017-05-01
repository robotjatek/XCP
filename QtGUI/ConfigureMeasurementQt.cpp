#include "ConfigureMeasurementQt.h"
#include <XCPMaster.h>


ConfigureMeasurementQt::ConfigureMeasurementQt(QWidget* parent) : QDialog(parent)
{
	ui.setupUi(this);
	using ModeFieldBits = SetDaqListModePacket::ModeFieldBits;
	//some default values for testing
	DAQ daq0;
	ODT daq0odt0;
	ODTEntry daq0odt0entry0(0x21A1BD, 0, 1);
	ODTEntry daq0odt0entry1(0x21A08D, 0, 1);
	daq0odt0.AddEntry(daq0odt0entry0);
	daq0odt0.AddEntry(daq0odt0entry1);
	daq0.AddODT(daq0odt0);
	daq0.SetEventChannel(0);
	daq0.SetMode(ModeFieldBits::TIMESTAMP);
	daq0.SetPrescaler(1);
	daq0.SetPriority(1);
	this->daq_layout.AddDAQ(daq0);
	this->daq_layout.SetInitialized(true);
	SeriesProperties p;
	p.b = 0;
	p.g = 0;
	p.r = 255;
	p.SeriesIndex = 0;
	ChartSeries[std::make_tuple<uint16_t, uint8_t, uint32_t>(0, 0, 0)] = p;
	p.r = 0;
	p.g = 255;
	p.b = 0;
	p.SeriesIndex = 1;
	ChartSeries[std::make_tuple<uint16_t, uint8_t, uint32_t>(0, 0, 1)] = p; //daq id, odt id, entry id
}


ConfigureMeasurementQt::~ConfigureMeasurementQt()
{
}

const DAQLayout & ConfigureMeasurementQt::GetDaqLayout()
{
	return daq_layout;
}

const std::map<std::tuple<uint16_t, uint8_t, uint32_t>, SeriesProperties>& ConfigureMeasurementQt::GetChartSeries()
{
	return this->ChartSeries;
}

void ConfigureMeasurementQt::reject()
{
	QDialog::reject();
}

void ConfigureMeasurementQt::accept()
{
	QDialog::accept();
}
