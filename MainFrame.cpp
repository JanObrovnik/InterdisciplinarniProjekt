#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <math.h>



//- IZRACUN
std::vector<double> izracun(int n) {
	std::vector<double> res;


	
	return res;
}




bool boolSimulacija = false;
int casSimulacije = 0;

wxSlider* sliderHitrosti; // Ustvarimo spremenljivko za slider
wxSlider* sliderDelovniTlak;
wxSlider* sliderOsnovniTlak;
wxSlider* sliderMoc;

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) { // Ustvarimo okno

	wxPanel* panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS); // Ustvarimo panel (ozadje na oknu)


	wxButton* sim = new wxButton(panel, wxID_ANY, "Simuliraj", wxPoint(10, 10), wxSize(130, 130)); // Definiramo gumb za simulacijo
	wxButton* nastavitve = new wxButton(panel, wxID_ANY, "Ponastavi", wxPoint(5, 570), wxSize(100, -1)); // Definiramo gumb za nastavitve


	sliderHitrosti = new wxSlider(panel, wxID_ANY, 50, 0, 50, wxPoint(250, 0), wxSize(600, -1)); // Definiramo slider
	sliderDelovniTlak = new wxSlider(panel, wxID_ANY, 0, 0, 100, wxPoint(850, 270), wxSize(140, -1));
	sliderOsnovniTlak = new wxSlider(panel, wxID_ANY, 0, 0, 100, wxPoint(850, 320), wxSize(140, -1));
	sliderMoc = new wxSlider(panel, wxID_ANY, 0, 0, 100, wxPoint(850, 370), wxSize(140, -1));


	panel->Bind(wxEVT_LEFT_DOWN, &MainFrame::OnMouseEvent, this); // Zazna levi klik na miski
	sim->Bind(wxEVT_BUTTON, &MainFrame::OnButtonSimClicked, this); // Zazna klik na gumb 'sim'
	nastavitve->Bind(wxEVT_BUTTON, &MainFrame::OnButtonNastavitveClicked, this); // Zazna klik na gumb 'nastavitve'
	sliderHitrosti->Bind(wxEVT_SLIDER, &MainFrame::OnSliderHitrostiChanged, this); // Zazna spemembo na slider-ju
	sliderDelovniTlak->Bind(wxEVT_SLIDER, &MainFrame::OnSliderHitrostiChanged, this);
	sliderOsnovniTlak->Bind(wxEVT_SLIDER, &MainFrame::OnSliderHitrostiChanged, this);
	sliderMoc->Bind(wxEVT_SLIDER, &MainFrame::OnSliderHitrostiChanged, this);
	panel->Connect(wxEVT_PAINT, wxPaintEventHandler(MainFrame::OnPaint)); // Uvozimo risanje na framu
	panel->SetDoubleBuffered(true); // Da prikaz na framu ne utripa

	wxStatusBar* statusBar = CreateStatusBar(); // Ustvarimo status bar
	statusBar->SetDoubleBuffered(true);
}



void MainFrame::OnMouseEvent(wxMouseEvent& evt) { // Funkcija ob zaznani spremembi na miski

	wxPoint mousePos = wxGetMousePosition(); // Najdemo pozicijo miske
	mousePos = this->ScreenToClient(mousePos); // Spremenimo pozicijo miske na relativni na okno



	wxString message = wxString::Format("Mouse Event Detected! (x=%d y=%d)", mousePos.x, mousePos.y); // Ustvarimo sporocilo
	wxLogStatus(message); // Izpise sporocilo v status bar
}


void MainFrame::OnButtonSimClicked(wxCommandEvent& evt) { // Funkcija ob pritisku na gumb 'sim'

	if (!boolSimulacija) {
		boolSimulacija = true;
	}
	else if (boolSimulacija) {
		casSimulacije = 0;
		boolSimulacija = false;
	}


	while (boolSimulacija) {
		casSimulacije++;
		Sleep(51 - sliderHitrosti->GetValue()); // Mogoc spremenit hitrost z zmansanjem Vrtljaja, mogoc ne
		Refresh();
		wxYield();
	}
}

void MainFrame::OnButtonNastavitveClicked(wxCommandEvent& evt) { // Funkcija ob pritisku na gumb 'nastavitve'


}


void MainFrame::OnSliderHitrostiChanged(wxCommandEvent& evt) { // Funkcija ob zaznani spremembi na slider-ju

	wxLogStatus("Slider change event");

	if (!boolSimulacija) Refresh(); // 'Refresh()' pozene 'MainFrame::OnPaint(wxPaintEvent& event)'
}



void MainFrame::OnPaint(wxPaintEvent& event) { // Funkcija, ki rise

	wxPaintDC dc(this); // spremenljivka za risanje


	// Spremenljivke
	int x_okno = 150;
	int y_okno = 30;
	wxSize size = this->GetSize(); // Velikost okna
	int sirina_panel = size.x;
	int visina_panel = size.y;
	int sirina = sirina_panel - 2 * x_okno;
	int visina = visina_panel - y_okno;

	int rot = casSimulacije; // Dobimo vrednost na sliderju


	// Prikaz okna simulacije
	dc.SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_SOLID)); // Definiramo crto
	dc.DrawRectangle(x_okno, y_okno, sirina, visina); // Izrisemo kvadrat


	// Kontrole
	//// Prikaz barve gumba
	if (boolSimulacija) { dc.SetPen(wxPen(wxColour(0, 255, 0), 1, wxPENSTYLE_SOLID)); dc.SetBrush(wxBrush(wxColour(153, 255, 153),wxBRUSHSTYLE_SOLID)); } // Ce simulira - zelena barva
	else { dc.SetPen(wxPen(wxColour(255, 0, 0), 1, wxPENSTYLE_SOLID)); dc.SetBrush(wxBrush(wxColour(255, 153, 153), wxBRUSHSTYLE_SOLID)); } // Ce ne simulira - rdeca barva

	dc.DrawRoundedRectangle(wxPoint(5, 5), wxSize(140, 140), 6); // Izrisemo barvo za prikaz stanja gumba
	dc.DrawText(wxString::Format("Cas sim: %d", casSimulacije), wxPoint(10, 150));

	dc.SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_SOLID)); // Ponastavimo posalo na osnovno
	dc.SetBrush(wxBrush(wxColour(255, 255, 255), wxBRUSHSTYLE_SOLID)); // Ponastavimo copic na osnovni

	//// Hitrost
	dc.DrawText("Hitrost simulacije:", wxPoint(155, 3));

	//// Nastavitve crpalke
	dc.DrawText("Nastavitve crpalke:", wxPoint(5, 225));
	dc.DrawLine(wxPoint(0, 240), wxPoint(x_okno, 240));
	dc.DrawLine(wxPoint(0, 240), wxPoint(0, 599));
	dc.DrawLine(wxPoint(0, 599), wxPoint(x_okno, 599));

	//// Nastavitve simulacije
	dc.DrawText("Nastavitve simulacije:", wxPoint(x_okno + sirina + 5, 225));
	dc.DrawLine(wxPoint(x_okno + sirina - 1, 240), wxPoint(sirina_panel, 240));
	dc.DrawLine(wxPoint(sirina_panel - 1, 240), wxPoint(sirina_panel - 1, 599));
	dc.DrawLine(wxPoint(x_okno + sirina - 1, 599), wxPoint(sirina_panel, 599));

	dc.DrawText(wxString::Format("Delovni tlak %d bar", sliderDelovniTlak->GetValue()), wxPoint(x_okno + sirina + 5, 255));
	dc.DrawText(wxString::Format("Osnovni tlak %d bar", sliderOsnovniTlak->GetValue()), wxPoint(x_okno + sirina + 5, 305));
	dc.DrawText(wxString::Format("Moc crpalke %d kW", sliderMoc->GetValue()), wxPoint(x_okno + sirina + 5, 355));


	// Admin Logs
	if (true) {
		dc.DrawText(wxString::Format("Cas: %d", rot), wxPoint(x_okno + sirina / 2, y_okno + 20));
		dc.DrawText(wxString::Format("Cos: %g", cos(rot * 2 * M_PI / 360)), wxPoint(x_okno + sirina / 2, y_okno + 40));
	}


	// Prikaz zobnikov
	dc.SetPen(wxPen(wxColor(0,0,0), 3, wxPENSTYLE_SOLID)); // Definiramo debelo crto

	int stZob = 11; // Stevilo zob na zobiku
	sirina -= 160;

	for (int j = 0; j < 2; j++) { // Izrisemo 2 zobnika

		if (j > 0) sirina += 320; // Spremenimo pozicijo zobnika

		int smer = 1; // Smer vrtenja
		if (j % 2 != 0) smer = -1; // Smer vrtenja

		wxPointList* seznamTock = new wxPointList();
		wxPoint* tocka0 = new wxPoint();

		for (int i = 0; i < stZob * 2; i++) {

			int dol = 120;
			if (i % 2 == 0) dol /= 3;

			wxPoint* tocka;
			tocka = new wxPoint(x_okno + sirina / 2 + cos(smer * rot * 2 * M_PI / 360 + M_PI / stZob * i) * dol, y_okno + visina / 2 + sin(smer * rot * 2 * M_PI / 360 + M_PI / stZob * i) * dol); // Dolocimo tocke zobnikov
			seznamTock->Append(tocka);

			if (i == 0) tocka0 = tocka;
			if (i == stZob * 2 - 1) seznamTock->Append(tocka0);
		}

		dc.DrawSpline(seznamTock); // Povezemo tocke zobnikov
	}

	dc.SetPen(wxPen(wxColor(0, 0, 0), 1, wxPENSTYLE_SOLID)); // Definiramo navadno crto
}



/*
povezava za dokumentacijo kontrol
https://docs.wxwidgets.org/3.0/group__group__class__ctrl.html
*/
