#pragma once

#include "ui_ConfigureMeasurement.h"
#include <QtWidgets/QDialog>
#include <XCPMaster.h>
#include "SeriesProperties.h"

class ConfigureMeasurementQt :
	public QDialog
{
	Q_OBJECT

public:
	ConfigureMeasurementQt(QWidget* parent = nullptr);
	virtual ~ConfigureMeasurementQt();
	public slots:
	void accept();
	void reject();
	const DAQLayout& GetDaqLayout();
	
private:
	Ui::ConfigureMeasurementUi ui;
	DAQLayout daq_layout;
	std::map<std::tuple<uint16_t, uint8_t, uint32_t>, SeriesProperties> ChartSeries; //daq id, odt id, entry id
public:
	const std::map<std::tuple<uint16_t, uint8_t, uint32_t>, SeriesProperties>& GetChartSeries();
};

