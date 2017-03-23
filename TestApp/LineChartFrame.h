#pragma once
class MainWindow;


#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/charts/wxcharts.h>
#include <wx/msgdlg.h>

class LineChartFrame :
	public wxFrame
{
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	wxLineChartCtrl* lineChartCtrl;
	wxChartLegendCtrl* legendCtrl;
	wxBoxSizer* panelSizer;
	wxBoxSizer* topSizer;
	wxPanel* panel;
	wxMenu* MenuFile;
	wxMenuBar* MenuBar;
	MainWindow& wnd;
	wxLineChartDataset* dataset;
	wxLineChartData* chartData;
	void CreateChartFromData();
public:
	LineChartFrame(const std::string& title, MainWindow& wnd);
	virtual ~LineChartFrame();

};

