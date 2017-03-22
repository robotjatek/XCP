#include "LineChartFrame.h"

#pragma comment(lib, "wxchartsd.lib")

void LineChartFrame::OnExit(wxCommandEvent & event)
{
	Close(true);
}

void LineChartFrame::OnAbout(wxCommandEvent & event)
{
	wxMessageBox("Conti Measurement XCP test", "Conti Measurement XCP test", wxOK | wxICON_INFORMATION);
}

LineChartFrame::LineChartFrame(const std::string & title) : wxFrame(NULL, wxID_ANY, title)
{
	MenuFile = new wxMenu;
	MenuFile->Append(wxID_EXIT);
	wxMenu* MenuHelp = new wxMenu;
	MenuHelp->Append(wxID_ABOUT);
	MenuBar = new wxMenuBar;
	MenuBar->Append(MenuFile, "&File");
	MenuBar->Append(MenuHelp, "&Help");

	SetMenuBar(MenuBar);
	CreateStatusBar();
	SetStatusText("asd");

	Bind(wxEVT_MENU, &LineChartFrame::OnAbout, this, wxID_ABOUT);
	Bind(wxEVT_MENU, &LineChartFrame::OnExit, this, wxID_EXIT);

	// Create a top-level panel to hold all the contents of the frame
	panel = new wxPanel(this, wxID_ANY);

	// Create the data for the line chart widget
	wxVector<wxString> labels;
	labels.push_back("January");
	labels.push_back("February");
	labels.push_back("March");
	labels.push_back("April");
	labels.push_back("May");
	labels.push_back("June");
	labels.push_back("July");
	wxLineChartData chartData(labels);

	// Add the first dataset
	wxVector<wxDouble> points1;
	points1.push_back(3);
	points1.push_back(-2.5);
	points1.push_back(-1.2);
	points1.push_back(3);
	points1.push_back(6);
	points1.push_back(5);
	points1.push_back(1);
	dataset = new wxLineChartDataset(
		"My First Dataset", wxColor(220, 220, 220),
		wxColor(255, 255, 255), wxColor(220, 220, 220, 0x33),
		points1);
	wxLineChartDataset::ptr dataset1(dataset);
	chartData.AddDataset(dataset1);

	// Create the line chart widget from the constructed data
	lineChartCtrl = new wxLineChartCtrl(panel, wxID_ANY, chartData,
		wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);

	// Create the legend widget
	wxChartLegendData legendData(chartData.GetDatasets());
	legendCtrl = new wxChartLegendCtrl(panel, wxID_ANY, legendData,
		wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);

	// Set up the sizer for the panel
	panelSizer = new wxBoxSizer(wxHORIZONTAL);
	panelSizer->Add(lineChartCtrl, 1, wxEXPAND);
	panelSizer->Add(legendCtrl, 1, wxEXPAND);
	panelSizer->SetMinSize(800, 600);
	panel->SetSizer(panelSizer);

	// Set up the sizer for the frame
	topSizer = new wxBoxSizer(wxHORIZONTAL);
	topSizer->Add(panel, 1, wxEXPAND);
	SetSizerAndFit(topSizer);
	
}

LineChartFrame::~LineChartFrame()
{
//	delete topSizer;
//	delete panelSizer;
	delete legendCtrl;
	delete lineChartCtrl;
	delete panel;
//	delete dataset;
}
