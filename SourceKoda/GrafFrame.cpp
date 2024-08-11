#include "GrafFrame.h"
#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <math.h>
#include <numeric>
#include <fstream>



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

			res[i + 1].push_back(j - 5);
		}

		else if (seznamEnacb[i] == "2x") for (double j = 0; j <= vrednost; j += korak) {

			res[i + 1].push_back(2 * j);
		}

		else if (seznamEnacb[i] == "x2") for (double j = 0; j <= vrednost; j += korak) {

			res[i + 1].push_back(j * j + 5);
		}
	}

	(*grafX).assign({ 0., .2, .4, .6, .8, 1. }); // X os

	std::vector<std::vector<double>> izr = res; // Y osi
	izr.erase(izr.begin() + 0);
	double max = maxVrednost(izr);
	double min = minVrednost(izr);
	
	for (int i = 0; i < (*grafX).size(); i++) (*grafX)[i] *= vrednost; // X os
	for (int i = 0; i < (*grafY).size(); i++) (*grafY)[i] = min + i * (max - min) / ((*grafY).size() - 1); // Y osi

	return res;
}



std::vector<std::vector<double>> res; // Resitve

std::vector<std::string> seznamEnacb; // Seznam enacb za izmerit

std::vector<wxPen> seznamBarv; // Seznam razlicnih barv za risanje grafa

std::vector<double> grafX{ 0., .2, .4, .6, .8, 1. };
std::vector<double> grafY{ 0., .2, .4, .6, .8, 1. };

wxArrayString velicineY;
wxCheckListBox* checkListBoxYOs;

wxSpinCtrlDouble* ctrlVrednostMeritveX;
wxSpinCtrlDouble* ctrlKorakMeritveX;

wxRadioBox* radioVelicinaX;


GrafFrame::GrafFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {

	seznamBarv.clear();
	seznamBarv.push_back(wxPen(wxColour(0, 0, 204), 2, wxPENSTYLE_SOLID));
	seznamBarv.push_back(wxPen(wxColour(0, 204, 0), 2, wxPENSTYLE_SOLID));
	seznamBarv.push_back(wxPen(wxColour(204, 0, 0), 2, wxPENSTYLE_SOLID));
	seznamBarv.push_back(wxPen(wxColour(0, 204, 204), 2, wxPENSTYLE_SOLID));
	seznamBarv.push_back(wxPen(wxColour(204, 204, 0), 2, wxPENSTYLE_SOLID));
	seznamBarv.push_back(wxPen(wxColour(204, 0, 204), 2, wxPENSTYLE_SOLID));
	seznamBarv.push_back(wxPen(wxColour(102, 102, 102), 2, wxPENSTYLE_SOLID));
	seznamBarv.push_back(wxPen(wxColour(204, 204, 204), 2, wxPENSTYLE_SOLID));

	wxPanel* panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS);
	
	wxButton* risi = new wxButton(panel, wxID_ANY, "Risi", wxPoint(10, 530), wxSize(80, 60));
	wxButton* shraniData = new wxButton(panel, wxID_ANY, "Shrani meritve", wxPoint(640, 560), wxDefaultSize);

	velicineY.Clear();
	velicineY.Add("x");
	velicineY.Add("2x");
	velicineY.Add("x2");
	checkListBoxYOs = new wxCheckListBox(panel, wxID_ANY, wxPoint(5, 200), wxDefaultSize, velicineY);

	ctrlVrednostMeritveX = new wxSpinCtrlDouble(panel, wxID_ANY, "", wxPoint(900, 550), wxDefaultSize, wxSP_ARROW_KEYS | wxSP_WRAP, 0, 1000, 10, .1);
	ctrlKorakMeritveX = new wxSpinCtrlDouble(panel, wxID_ANY, "", wxPoint(160, 550), wxDefaultSize, wxSP_ARROW_KEYS | wxSP_WRAP, 0, 10, 1, .01);
	
	wxArrayString velicineX;
	velicineX.Add("t [s]");
	velicineX.Add("x [m]");
	radioVelicinaX = new wxRadioBox(panel, wxID_ANY, "", wxPoint(500, 540), wxDefaultSize, velicineX, 1, wxRA_SPECIFY_ROWS);


	risi->Bind(wxEVT_BUTTON, &GrafFrame::OnButtonRisiClicked, this);
	shraniData->Bind(wxEVT_BUTTON, &GrafFrame::OnButtonShraniClicked, this);


	panel->Bind(wxEVT_SIZE, &GrafFrame::OnSizeChanged, this); // Zazna spremembo velikosti okna
	panel->Connect(wxEVT_PAINT, wxPaintEventHandler(GrafFrame::OnPaint));
	panel->SetDoubleBuffered(true);
}


void GrafFrame::OnSizeChanged(wxSizeEvent& evt) {

	Refresh();
}

void GrafFrame::OnButtonRisiClicked(wxCommandEvent& evt) {

	Refresh();
}

void GrafFrame::OnButtonShraniClicked(wxCommandEvent& evt) {

	std::string ime = __DATE__;
	wxFileDialog* fileDialog = new wxFileDialog(this, "Shrani", wxEmptyString, "Meritev " + ime, "Text files (*.txt)|*.txt", wxFD_SAVE);

	if (fileDialog->ShowModal() == wxID_OK) {

		std::string pot = static_cast<std::string>(fileDialog->GetPath());

		std::ofstream shrani;
		shrani.open(pot, std::ios::out);

		if (shrani.is_open()) {

			shrani << "Zapis meritev simulacije:" << std::endl;
			shrani << __DATE__ << ", " << __TIME__ << ":" << std::endl << std::endl << std::endl;

			shrani << radioVelicinaX->GetString(radioVelicinaX->GetSelection()) << "\t";
			for (int i = 0; i < seznamEnacb.size(); i++) shrani << seznamEnacb[i] << "\t";
			shrani << std::endl << std::endl;

			for (int i = 0; i < res[0].size(); i++) {

				for (int j = 0; j < res.size(); j++) {

					shrani << res[j][i] << "\t";
				}
				shrani << std::endl;
			}


			shrani.close();
		}
	}
}


void GrafFrame::OnPaint(wxPaintEvent& event) {

	wxPaintDC dc(this);

	wxSize size = this->GetSize();


	// Graf
	seznamEnacb.clear();
	for (int i = 0; i < velicineY.size(); i++) if (checkListBoxYOs->IsChecked(i)) seznamEnacb.push_back(static_cast<std::string>(velicineY[i]));

	if (seznamEnacb.size() > 0) res = izracunGraf(seznamEnacb, ctrlKorakMeritveX->GetValue(), ctrlVrednostMeritveX->GetValue(), &grafX, &grafY);
	else res.clear();

	//// Ozadje
	wxPoint pointGraf(120, 20);
	wxSize sizeGraf(size.x - 160, size.y - 100);

	dc.SetPen(wxPen(wxColour(255, 255, 255), 1, wxPENSTYLE_SOLID));
	dc.DrawRectangle(pointGraf, sizeGraf);
	dc.SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_SOLID));

	dc.DrawLine(wxPoint(pointGraf.x, pointGraf.y + sizeGraf.y), wxPoint(size.x, pointGraf.y + sizeGraf.y)); // X os
	dc.DrawLine(wxPoint(size.x, pointGraf.y + sizeGraf.y), wxPoint(size.x - 10, pointGraf.y + sizeGraf.y - 5));
	dc.DrawLine(wxPoint(size.x, pointGraf.y + sizeGraf.y), wxPoint(size.x - 10, pointGraf.y + sizeGraf.y + 5));
	
	dc.DrawLine(wxPoint(pointGraf.x, pointGraf.y + sizeGraf.y), wxPoint(pointGraf.x, 0)); // Y os
	dc.DrawLine(wxPoint(pointGraf.x, 0), wxPoint(pointGraf.x + 5, 10));
	dc.DrawLine(wxPoint(pointGraf.x, 0), wxPoint(pointGraf.x - 5, 10));

	//// Velicine
	dc.DrawText(radioVelicinaX->GetString(radioVelicinaX->GetSelection()), wxPoint(size.x - 32, sizeGraf.y)); // X os

	dc.DrawRectangle(wxPoint(10, 10), wxSize(80, 15 * seznamEnacb.size()));// Y os
	for (int i = 0; i < seznamEnacb.size(); i++) {
		
		if (i == seznamBarv.size()) i -= seznamBarv.size();
		dc.SetPen(seznamBarv[i]);
		dc.DrawLine(wxPoint(12, 16 + i * 15), wxPoint(24, 16 + i * 15));
		
		dc.DrawText(seznamEnacb[i], wxPoint(30, 8 + i * 15));
	}

	//// Vrednosti
	dc.SetPen(wxPen(wxColour(153, 153, 153), 1, wxPENSTYLE_SOLID));
	for (int i = 0; i < grafX.size(); i++) { // X os
		dc.DrawText(wxString::Format("%g", grafX[i]), wxPoint(120 + i * sizeGraf.x / 5 - 3, size.y - 75)); //////////////////// preureditev 'size.x' in 'size.y' z 'pointGraf' in 'sizeGraf'
		if (i > 0) dc.DrawLine(wxPoint(120 + i * sizeGraf.x / 5, size.y - 75), wxPoint(120 + i * sizeGraf.x / 5, 20));
	}
	for (int i = 0; i < grafY.size(); i++) { // Y os
		dc.DrawText(wxString::Format("%g", grafY[i]), wxPoint(120 - 20, size.y - 80 - i * (size.y - 100) / 5 - 7));
		if (i > 0) dc.DrawLine(wxPoint(120 - 5, size.y - 80 - i * (size.y - 100) / 5), wxPoint(pointGraf.x + sizeGraf.x, size.y - 80 - i * (size.y - 100) / 5));
	}
	dc.SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_SOLID));

	//// Linije
	dc.SetPen(wxPen(wxColour(0, 0, 0), 2, wxPENSTYLE_SOLID));
	for (int i = 1; i < res.size(); i++) {

		if (i - 1 == seznamBarv.size()) i -= seznamBarv.size();
		dc.SetPen(seznamBarv[i - 1]);
		wxPointList* seznamTock = new wxPointList();

		for (int j = 0; j < res[i].size(); j++) {

			wxPoint* tocka;
			tocka = new wxPoint(120 + sizeGraf.x * res[0][j] / grafX[grafX.size() - 1], size.y - 80 - (size.y - 100) * (res[i][j] - grafY[0]) / (grafY[grafY.size() - 1] - grafY[0]));
			seznamTock->Append(tocka);
		}

		dc.DrawLines(seznamTock);
	}
	dc.SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_SOLID));




	if (false) { // ADMIN LOGS
		dc.DrawText(wxString::Format("%d", static_cast<int>(res.size())), wxPoint(5,0));

		for (int i = 0; i < res.size(); i++) for (int j = 0; j < res[i].size(); j++) dc.DrawText(wxString::Format("%g", res[i][j]), wxPoint(5 + i * 25, 15 + j * 12));

		dc.DrawText(velicineY[0], wxPoint(5, 165));
	}
}
