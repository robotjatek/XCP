#pragma once
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

	wxLineChartDataset* dataset;
public:
	LineChartFrame(const std::string& title);
	virtual ~LineChartFrame();
};

