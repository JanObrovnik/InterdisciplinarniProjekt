#include "GrafFrame.h"
#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <math.h>
#include <numeric>



double maxVrednost(std::vector<std::vector<double>> res) {

	double max = res[0][0];

	for (int i = 0; i < res.size(); i++) for (int j = 0; j < res[i].size(); j++) if (res[i][j] > max) max = res[i][j];

	return max;
}

double minVrednost(std::vector<std::vector<double>> res) {

	double min = res[0][0];

	for (int i = 0; i < res.size(); i++) for (int j = 0; j < res[i].size(); j++) if (res[i][j] < min) min = res[i][j];

	return min;
}


std::vector<std::vector<double>> izracunGraf(std::vector<std::string> seznamEnacb, double korak, double vrednost, std::vector<double>* grafX, std::vector<double>* grafY) {

	std::vector<std::vector<double>> res;
	res.resize(seznamEnacb.size() + 1);

	for (double j = 0; j <= vrednost; j += korak) res[0].push_back(j); // X os

	for (int i = 0; i < seznamEnacb.size(); i++) { // Y osi

		if (seznamEnacb[i] == "x") for (double j = 0; j <= vrednost; j += korak) {

			res[i + 1].push_back(j);
		}

		else if (seznamEnacb[i] == "2x") for (double j = 0; j <= vrednost; j += korak) {

			res[i + 1].push_back(2 * j);
		}

		else if (seznamEnacb[i] == "x2") for (double j = 0; j <= vrednost; j += korak) {

			res[i + 1].push_back(j * j);
		}
	}

	std::vector<std::vector<double>> izr = res;
	izr.erase(izr.begin() + 0);
	double max = maxVrednost(izr);
	double min = minVrednost(izr);
	
	for (int i = 0; i < (*grafX).size(); i++) (*grafX)[i] *= vrednost;
	for (int i = 0; i < (*grafY).size(); i++) (*grafY)[i] *= max; ////////////// Spremenit da lahko zacetek razlicen od 0

	return res;
}



std::vector<double> grafX{ 0., .2, .4, .6, .8, 1. };
std::vector<double> grafY{ 0., .2, .4, .6, .8, 1. };


GrafFrame::GrafFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {

	wxPanel* panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS);



	panel->Bind(wxEVT_SIZE, &GrafFrame::OnSizeChanged, this);
	panel->Connect(wxEVT_PAINT, wxPaintEventHandler(GrafFrame::OnPaint)); // Uvozimo risanje na framu
	panel->SetDoubleBuffered(true); // Da prikaz na framu ne utripa
}


void GrafFrame::OnSizeChanged(wxSizeEvent& evt) {

	Refresh();
}


void GrafFrame::OnPaint(wxPaintEvent& event) {

	wxPaintDC dc(this);

	wxSize size = this->GetSize();


	// Graf
	std::vector<std::vector<double>> res = izracunGraf({ "x", "2x", "x2" }, 1, 10, &grafX, &grafY);

	//// Ozadje
	dc.SetPen(wxPen(wxColour(255, 255, 255), 1, wxPENSTYLE_SOLID));
	dc.DrawRectangle(wxPoint(120, 20), wxSize(size.x - 140, size.y - 100));
	dc.SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_SOLID));

	dc.DrawLine(wxPoint(120, size.y - 80), wxPoint(size.x, size.y - 80));
	dc.DrawLine(wxPoint(size.x, size.y - 80), wxPoint(size.x - 10, size.y - 85));
	dc.DrawLine(wxPoint(size.x, size.y - 80), wxPoint(size.x - 10, size.y - 75));
	dc.DrawLine(wxPoint(120, size.y - 80), wxPoint(120, 0));
	dc.DrawLine(wxPoint(120, 0), wxPoint(125, 10));
	dc.DrawLine(wxPoint(120, 0), wxPoint(115, 10));

	//// Vrednosti
	dc.SetPen(wxPen(wxColour(153, 153, 153), 1, wxPENSTYLE_SOLID));
	for (int i = 0; i < grafX.size(); i++) {
		dc.DrawText(wxString::Format("%g", grafX[i]), wxPoint(120 + i * (size.x - 140) / 5 - 10, size.y - 75));
		if (i > 0) dc.DrawLine(wxPoint(120 + i * (size.x - 140) / 5, size.y - 75), wxPoint(120 + i * (size.x - 140) / 5, 20));
	}
	for (int i = 0; i < grafY.size(); i++) {
		dc.DrawText(wxString::Format("%g", grafY[i]), wxPoint(120 - 20, size.y - 80 - i * (size.y - 100) / 5));
		if (i > 0) dc.DrawLine(wxPoint(120 - 20, size.y - 80 - i * (size.y - 100) / 5), wxPoint(size.x - 20, size.y - 80 - i * (size.y - 100) / 5));
	}
	dc.SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_SOLID));

	//// Linije
	dc.SetPen(wxPen(wxColour(0, 0, 0), 2, wxPENSTYLE_SOLID));
	for (int i = 0; i < res.size(); i++) {

		wxPointList* seznamTock = new wxPointList();

		for (int j = 0; j < res[i].size(); j++) {

			wxPoint* tocka;
			tocka = new wxPoint(120 + (size.x - 140) * res[0][j] / grafX[grafX.size() - 1], size.y - 80 - (size.y - 100) * res[i][j] / grafY[grafY.size() - 1]);
			seznamTock->Append(tocka);
		}

		dc.DrawLines(seznamTock);
	}
	dc.SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_SOLID));




	if (false) { // ADMIN LOGS
		dc.DrawText(wxString::Format("%d", static_cast<int>(res.size())), wxPoint(5,0));

		for (int i = 0; i < res.size(); i++) for (int j = 0; j < res[i].size(); j++) dc.DrawText(wxString::Format("%g", res[i][j]), wxPoint(5 + i * 25, 15 + j * 12));

		std::vector<double> vec = *max_element(res.begin(), res.end());
		for (int i = 0; i < vec.size(); i++) dc.DrawText(wxString::Format("%g", vec[i]), wxPoint(5, 165 + i * 12));
	}
}
