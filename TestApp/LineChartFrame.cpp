#include "LineChartFrame.h"
#include "MainWindow.h"

#pragma comment(lib, "wxchartsd.lib")

void LineChartFrame::OnExit(wxCommandEvent & event)
{
	Close(true);
}

void LineChartFrame::OnAbout(wxCommandEvent & event)
{
	wxMessageBox("Conti Measurement XCP test", "Conti Measurement XCP test", wxOK | wxICON_INFORMATION);	
}

void LineChartFrame::CreateChartFromData()
{
	
}

LineChartFrame::LineChartFrame(const std::string & title, MainWindow& wnd) : wxFrame(NULL, wxID_ANY, title), wnd(wnd)
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
	


	// Add the first dataset
	wxVector<wxDouble> points1;
	for (int i = 0; i < wnd.asd.size(); i++)
	{
		points1.push_back((wnd.asd)[i]);
	}


	wxVector<wxString> labels;
	for (int i = 0; i < points1.size(); i++)
	{
		labels.push_back("");
	}
	wxLineChartData chartData(labels);

	dataset = new wxLineChartDataset(
		"Measurement", wxColor(0, 220, 0),
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
	//panelSizer->Add(legendCtrl, 1, wxEXPAND);
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
