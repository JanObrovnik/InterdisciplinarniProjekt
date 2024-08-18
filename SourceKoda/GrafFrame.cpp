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


std::vector<std::vector<double>> izracunGraf(std::vector<std::string> seznamEnacb, std::string spremenljivka, double korak, double vrednost, std::vector<double>* grafX, std::vector<double>* grafY, PodatkiZobnika* zobnik) {

	std::vector<std::vector<double>> res;
	res.resize(seznamEnacb.size() + 1);




	if (spremenljivka == "u [mm]") { // X os

		for (double j = -zobnik->dolzinaKontakta / 2; j <= zobnik->dolzinaKontakta / 2; j += korak) res[0].push_back(j);
	}

	else {

		for (double j = 0; j <= vrednost; j += korak) res[0].push_back(j);
	}


	for (int i = 0; i < seznamEnacb.size(); i++) { // Y osi

		if (seznamEnacb[i] == "x") for (double j = res[0][0]; j <= res[0][res[0].size() - 1]; j += korak) {
			
			res[i + 1].push_back(j - 5);
		}

		else if (seznamEnacb[i] == "2x") for (double j = res[0][0]; j <= res[0][res[0].size() - 1]; j += korak) {

			res[i + 1].push_back(2 * j);
		}

		else if (seznamEnacb[i] == "x2") for (double j = res[0][0]; j <= res[0][res[0].size() - 1]; j += korak) {

			res[i + 1].push_back(j * j + 5);
		}

		else if (seznamEnacb[i] == "V(u) [l/s]") for (double j = res[0][0]; j <= res[0][res[0].size() - 1]; j += korak) {

			double u = j / 1000;

			float b = zobnik->debelina / 1000;
			float da = zobnik->premerTemenskegaKroga / 1000;
			float dw = zobnik->premerKinematskegaKroga / 1000;
			float n = 2; // vrtljaji

			double resitev = (b * M_PI * n * (pow(da, 2) / 2 - pow(dw, 2) / 2 - 2 * pow(u, 2))) * 1000;

			res[i + 1].push_back(resitev);
		}

		else for (double j = res[0][0]; j <= res[0][res[0].size() - 1]; j += korak) {

			res[i + 1].push_back(0);
		}
	}


	std::vector<std::vector<double>> izr1; // X os
	izr1.resize(2);
	izr1[0] = res[0];
	double maxX = maxVrednost(izr1);
	double minX = minVrednost(izr1);
	for (int i = 0; i < (*grafX).size(); i++) (*grafX)[i] = minX + i * (maxX - minX) / ((*grafX).size() - 1); // X os


	std::vector<std::vector<double>> izr2 = res; // Y os
	izr2.erase(izr2.begin() + 0);
	double maxY = maxVrednost(izr2);
	double minY = minVrednost(izr2);
	for (int i = 0; i < (*grafY).size(); i++) (*grafY)[i] = minY + i * (maxY - minY) / ((*grafY).size() - 1); // Y os

	if ((*grafY)[0] == (*grafY)[(*grafX).size() - 1]) (*grafY).assign({ (*grafY)[0] - .5, (*grafY)[0] - .3, (*grafY)[0] - .1, (*grafY)[0] + .1, (*grafY)[0] + .3, (*grafY)[0] + .5 }); // Popravek za konst. funkcije


	return res;
}



PodatkiZobnika podatkiZobnika;

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
	
	wxButton* risi = new wxButton(panel, wxID_ANY, "Risi", wxPoint(10, 435), wxSize(92, 92));
	wxButton* shraniData = new wxButton(panel, wxID_ANY, "Shrani\nmeritve", wxPoint(10, 535), wxSize(92, 92));

	velicineY.Clear();
	velicineY.Add("x");
	velicineY.Add("2x");
	velicineY.Add("x2");
	velicineY.Add("0");
	velicineY.Add("V(u) [l/s]");
	checkListBoxYOs = new wxCheckListBox(panel, wxID_ANY, wxPoint(10, 25), wxSize(92, -1), velicineY);
	checkListBoxYOs->SetString(4, "/");

	ctrlVrednostMeritveX = new wxSpinCtrlDouble(panel, wxID_ANY, "", wxPoint(878, 586), wxDefaultSize, wxSP_ARROW_KEYS | wxSP_WRAP, 0, 1000, 10, .1);
	ctrlKorakMeritveX = new wxSpinCtrlDouble(panel, wxID_ANY, "", wxPoint(215, 586), wxDefaultSize, wxSP_ARROW_KEYS | wxSP_WRAP, 0, 10, .01, .01);
	
	wxArrayString velicineX;
	velicineX.Add("t [s]");
	velicineX.Add("x [m]");
	velicineX.Add("u [mm]");
	radioVelicinaX = new wxRadioBox(panel, wxID_ANY, "", wxPoint(460, 585), wxDefaultSize, velicineX, 1, wxRA_SPECIFY_ROWS);


	risi->Bind(wxEVT_BUTTON, &GrafFrame::OnButtonRisiClicked, this);
	shraniData->Bind(wxEVT_BUTTON, &GrafFrame::OnButtonShraniClicked, this);
	radioVelicinaX->Bind(wxEVT_RADIOBOX, &GrafFrame::OnRadioVelicinaXChange, this);


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

			shrani << radioVelicinaX->GetStringSelection() << "\t";
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

void GrafFrame::OnRadioVelicinaXChange(wxCommandEvent& evt) {

	for (int i = 0; i < velicineY.size(); i++) checkListBoxYOs->Check(i, false);


	for (int i = 0; i < velicineY.size(); i++) checkListBoxYOs->SetString(i, velicineY[i]);

	if (radioVelicinaX->GetStringSelection() == "t [s]") {
	
		checkListBoxYOs->SetString(4, "/");

		ctrlVrednostMeritveX->Enable();
	}

	else if (radioVelicinaX->GetStringSelection() == "x [m]") {

		checkListBoxYOs->SetString(4, "/");

		ctrlVrednostMeritveX->Enable();
	}

	else if (radioVelicinaX->GetStringSelection() == "u [mm]") {

		checkListBoxYOs->SetString(0, "/");
		checkListBoxYOs->SetString(1, "/");
		checkListBoxYOs->SetString(2, "/");
		checkListBoxYOs->SetString(3, "/");

		ctrlVrednostMeritveX->Disable();
	}
}


void GrafFrame::OnPaint(wxPaintEvent& event) {

	wxPaintDC dc(this);

	wxSize size = this->GetSize();


	// Graf
	seznamEnacb.clear();
	for (int i = 0; i < velicineY.size(); i++) if (checkListBoxYOs->IsChecked(i)) seznamEnacb.push_back(static_cast<std::string>(checkListBoxYOs->GetString(i)));

	std::string spremenljivka = static_cast<std::string>(radioVelicinaX->GetStringSelection());

	if (seznamEnacb.size() > 0) res = izracunGraf(seznamEnacb, spremenljivka, ctrlKorakMeritveX->GetValue(), ctrlVrednostMeritveX->GetValue(), &grafX, &grafY, &podatkiZobnika);
	else res.clear();

	dc.DrawText("Korak =", wxPoint(170, 587));
	dc.DrawText("Obmocje =", wxPoint(813, 587));

	//// Ozadje
	wxPoint pointGraf(150, 20);
	wxSize sizeGraf(size.x - 200, size.y - 100);

	dc.SetPen(wxPen(wxColour(255, 255, 255), 1, wxPENSTYLE_SOLID));
	dc.DrawRectangle(pointGraf, sizeGraf);
	dc.SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_SOLID));

	dc.DrawLine(wxPoint(pointGraf.x, pointGraf.y + sizeGraf.y), wxPoint(size.x, pointGraf.y + sizeGraf.y)); // X os
	dc.DrawLine(wxPoint(size.x, pointGraf.y + sizeGraf.y), wxPoint(size.x - 10, pointGraf.y + sizeGraf.y - 5));
	dc.DrawLine(wxPoint(size.x, pointGraf.y + sizeGraf.y), wxPoint(size.x - 10, pointGraf.y + sizeGraf.y + 5));
	
	dc.DrawLine(wxPoint(pointGraf.x, pointGraf.y + sizeGraf.y), wxPoint(pointGraf.x, 0)); // Y os
	dc.DrawLine(wxPoint(pointGraf.x, 0), wxPoint(pointGraf.x + 5, 10));
	dc.DrawLine(wxPoint(pointGraf.x, 0), wxPoint(pointGraf.x - 5, 10));

	//// Vrednosti
	dc.SetPen(wxPen(wxColour(153, 153, 153), 1, wxPENSTYLE_SOLID));
	for (int i = 0; i < grafX.size(); i++) { // X os
		dc.DrawText(wxString::Format("%g", round(grafX[i] * 10000) / 10000), wxPoint(pointGraf.x + i * sizeGraf.x / 5 - 3,   pointGraf.y + sizeGraf.y + 5));
		if (i > 0) dc.DrawLine(wxPoint(pointGraf.x + i * sizeGraf.x / 5, pointGraf.y + sizeGraf.y + 5), wxPoint(pointGraf.x + i * sizeGraf.x / 5, pointGraf.y));
	}
	for (int i = 0; i < grafY.size(); i++) { // Y os
		dc.DrawText(wxString::Format("%g", round(grafY[i] * 10000) / 10000), wxPoint(pointGraf.x - 36, pointGraf.y + sizeGraf.y - i * sizeGraf.y / 5 - 7));
		if (i > 0) dc.DrawLine(wxPoint(pointGraf.x - 5, pointGraf.y + sizeGraf.y - i * sizeGraf.y / 5), wxPoint(pointGraf.x + sizeGraf.x, pointGraf.y + sizeGraf.y - i * sizeGraf.y / 5));
	}
	dc.SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_SOLID));

	//// Linije
	dc.SetPen(wxPen(wxColour(0, 0, 0), 2, wxPENSTYLE_SOLID));
	for (int i = 1; i < res.size(); i++) {

		int j = i - 1;
		while (j >= seznamBarv.size()) j -= seznamBarv.size();
		dc.SetPen(seznamBarv[j]);

		wxPointList* seznamTock = new wxPointList();

		for (int j = 0; j < res[i].size(); j++) {

			wxPoint* tocka;
			tocka = new wxPoint(pointGraf.x + sizeGraf.x * (res[0][j] - grafX[0]) / (grafX[grafX.size() - 1] - grafX[0]), pointGraf.y + sizeGraf.y - sizeGraf.y * (res[i][j] - grafY[0]) / (grafY[grafY.size() - 1] - grafY[0]));
			seznamTock->Append(tocka);
		}

		dc.DrawLines(seznamTock);
	}
	dc.SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_SOLID));

	//// Legenda
	dc.DrawText(radioVelicinaX->GetString(radioVelicinaX->GetSelection()), wxPoint(size.x - 44, sizeGraf.y)); // X os
	
	dc.DrawText("Izracunaj:", wxPoint(10, 8)); // Y os
	
	if (seznamEnacb.size() > 0) {
		dc.DrawRectangle(wxPoint(pointGraf.x + 10, pointGraf.y + 16), wxSize(80, 15 * seznamEnacb.size()));
		dc.DrawText("Legenda", wxPoint(pointGraf.x + 10, pointGraf.y - 0));
		for (int i = 0; i < seznamEnacb.size(); i++) {

			int j = i;
			while (j >= seznamBarv.size()) j -= seznamBarv.size();
			dc.SetPen(seznamBarv[j]);
			dc.DrawLine(wxPoint(pointGraf.x + 12, pointGraf.y + 23 + i * 15), wxPoint(pointGraf.x + 24, pointGraf.y + 23 + i * 15));

			dc.DrawText(seznamEnacb[i], wxPoint(pointGraf.x + 30, pointGraf.y + 16 + i * 15));
		}
	}



	if (false) { // ADMIN LOGS
		dc.DrawText(wxString::Format("%d", static_cast<int>(res.size())), wxPoint(5,0));

		for (int i = 0; i < res.size(); i++) for (int j = 0; j < res[i].size(); j++) dc.DrawText(wxString::Format("%g", res[i][j]), wxPoint(5 + i * 25, 15 + j * 12));

		dc.DrawText(velicineY[0], wxPoint(5, 165));
	}
}
