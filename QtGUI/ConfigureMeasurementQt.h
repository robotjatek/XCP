#pragma once

#include "ui_ConfigureMeasurement.h"
#include <QtWidgets/QDialog>
#include <XCPMaster.h>
#include "SeriesProperties.h"
#include <QtWidgets/QTreeWidget>
#include <QColorDialog>

class ConfigureMeasurementQt :
	public QDialog
{
	Q_OBJECT

public:
	ConfigureMeasurementQt(const std::map<std::tuple<uint16_t, uint8_t, uint32_t>, SeriesProperties>& ChartSeries, const DAQLayout& MasterDAQLayout, QWidget* parent = nullptr);
	virtual ~ConfigureMeasurementQt();
	public slots:
	void accept();
	void reject();
	const DAQLayout& GetDaqLayout();
	void AddDaqBtnClicked();
	void AddOdtBtnClicked();
	void AddEntryBtnClicked();
	void ItemClicked(QTreeWidgetItem*, int);
	void ItemSelected();
	void SetDAQSettingsClicked();
	void SetEntrySettingsClicked();
	void ColorPickerButtonClicked();

	
private:
	Ui::ConfigureMeasurementUi ui;
	DAQLayout m_DAQLayout;
	std::map<std::tuple<uint16_t, uint8_t, uint32_t>, SeriesProperties> m_ChartSeries; //daq id, odt id, entry id
	QTreeWidgetItem* SelectedDAQ;
	QTreeWidgetItem* SelectedODT;
	QTreeWidgetItem* SelectedEntry;
	int SelectedDAQId;
	int SelectedODTId;
	int SelectedEntryId;
	void AddDAQToTree();
	void AddODTToDAQ(QTreeWidgetItem* parent);
	void AddEntryToODT(QTreeWidgetItem* parent);
	QTreeWidgetItem* AddDAQToList();
	QTreeWidgetItem* AddODTToList(QTreeWidgetItem * parent);
	void AddEntryToList(QTreeWidgetItem * parent);
	QColorDialog* ColorDialog;
public:
	const std::map<std::tuple<uint16_t, uint8_t, uint32_t>, SeriesProperties>& GetChartSeries();
};

