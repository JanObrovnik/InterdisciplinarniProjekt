#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <math.h>
#include <numeric>



//- IZRACUN ZOBNIKA
static void izracunZobnika(PodatkovnaBazaZobnika* zobnik) {

	short z = zobnik->stZob;
	float m = zobnik->modul;
	float b = zobnik->debelina;

	float dw = m * z;
	float df = dw - 2.5 * m;
	float da = dw + 2 * m;

	zobnik->premerKinematskegaKroga = dw;
	zobnik->premerKorenjskegaKroga = df;
	zobnik->premerTemenskegaKroga = da;
}


//- IZRACUN MEHANIZMA
static void izracun(StanjeZobnika* stanje, std::string konstParameter, std::vector<wxSlider*> seznamSil) {

	/*double dt = 1. / 100; // korak

	double vrtljaji = seznamSil[3]->GetValue();
	double moc = seznamSil[2]->GetValue();

	if (konstParameter == "Konst. moc") {
		vrtljaji = 0;
		vrtljaji -= seznamSil[0]->GetValue();
		vrtljaji += seznamSil[1]->GetValue();
		vrtljaji += moc;
		seznamSil[3]->SetValue(vrtljaji);
	}
	else if (konstParameter == "Konst. vrtljaji") {
		moc = 0;
		moc += vrtljaji;
		moc += seznamSil[0]->GetValue();
		moc -= seznamSil[1]->GetValue();
		seznamSil[2]->SetValue(moc);
	}

	vrtljaji *= dt;
	
	stanje->zasuk += vrtljaji;*/
////////////////////
	double dt = 1. / 100; // korak

	double vrtljaji = stanje->vrtljaji;
	double moc = stanje->moc;

	if (konstParameter == "Konst. moc") {
		vrtljaji = 0;
		vrtljaji -= stanje->delovniTlak;
		vrtljaji += stanje->osnovniTlak;
		vrtljaji += moc;
		stanje->vrtljaji = vrtljaji;
	}
	else if (konstParameter == "Konst. vrtljaji") {
		moc = 0;
		moc += vrtljaji;
		moc += stanje->delovniTlak;
		moc -= stanje->osnovniTlak;
		stanje->moc = moc;
	}

	stanje->zasuk += vrtljaji * dt;
}




PodatkovnaBazaZobnika zobnik;
StanjeZobnika stanje;

bool boolSimulacija = false;
int casSimulacije = 0;

wxSpinCtrl* ctrlStZob;
wxSpinCtrlDouble* ctrlModul;
wxSpinCtrlDouble* ctrlDebelina;

wxRadioBox* konstParameter;

wxSlider* sliderHitrosti; // Ustvarimo spremenljivko za slider
wxSlider* sliderDelovniTlak;
wxSlider* sliderOsnovniTlak;
wxSlider* sliderMoc;
wxSlider* sliderVrtljaji;


MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) { // Ustvarimo okno

	zobnik.stZob = 11;
	zobnik.modul = 4;
	zobnik.debelina = 40;
	izracunZobnika(&zobnik);

	stanje.vrtljaji = 40;
	stanje.moc = 40;
	stanje.delovniTlak = 20;
	stanje.osnovniTlak = 1;


	wxPanel* panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS); // Ustvarimo panel (ozadje na oknu)


	wxButton* sim = new wxButton(panel, wxID_ANY, "Simuliraj", wxPoint(10, 10), wxSize(130, 130)); // Definiramo gumb za simulacijo
	wxButton* reset = new wxButton(panel, wxID_ANY, "Resetiraj Sim", wxPoint(5, 170), wxSize(140, -1)); // Definiramo gumb za resetiranje simulacije
	wxButton* nastavitve = new wxButton(panel, wxID_ANY, "Ponastavi", wxPoint(5, 570), wxSize(100, -1)); // Definiramo gumb za nastavitve


	ctrlStZob = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(75,250), wxSize(70, -1), wxSP_WRAP, 0, 30, zobnik.stZob); // Definiramo spinCtrl
	ctrlModul = new wxSpinCtrlDouble(panel, wxID_ANY, "", wxPoint(75, 280), wxSize(70, -1), wxSP_WRAP, 0, 20, zobnik.modul, .5);
	ctrlDebelina = new wxSpinCtrlDouble(panel, wxID_ANY, "", wxPoint(75, 310), wxSize(70, -1), wxSP_WRAP, 0, 100, zobnik.debelina, .1);


	wxArrayString konstPar;
	konstPar.Add("Konst. moc");
	konstPar.Add("Konst. vrtljaji");
	konstParameter = new wxRadioBox(panel, wxID_ANY, "Konstantna velicina", wxPoint(855, 50), wxDefaultSize, konstPar, 1, wxRA_SPECIFY_COLS);

	sliderHitrosti = new wxSlider(panel, wxID_ANY, 50, 0, 50, wxPoint(250, 0), wxSize(600, -1)); // Definiramo slider
	sliderDelovniTlak = new wxSlider(panel, wxID_ANY, stanje.delovniTlak, 0, 100, wxPoint(850, 270), wxSize(140, -1));
	sliderOsnovniTlak = new wxSlider(panel, wxID_ANY, stanje.osnovniTlak, 0, 100, wxPoint(850, 320), wxSize(140, -1));
	sliderMoc = new wxSlider(panel, wxID_ANY, stanje.moc, 0, 100, wxPoint(850, 370), wxSize(140, -1));
	sliderVrtljaji = new wxSlider(panel, wxID_ANY, stanje.vrtljaji, 0, 100, wxPoint(850, 420), wxSize(140, -1));
	sliderVrtljaji->Disable();


	panel->Bind(wxEVT_LEFT_DOWN, &MainFrame::OnMouseEvent, this); // Zazna levi klik na miski
	sim->Bind(wxEVT_BUTTON, &MainFrame::OnButtonSimClicked, this); // Zazna klik na gumb
	reset->Bind(wxEVT_BUTTON, &MainFrame::OnButtonResetClicked, this);
	nastavitve->Bind(wxEVT_BUTTON, &MainFrame::OnButtonNastavitveClicked, this);
	konstParameter->Bind(wxEVT_RADIOBOX, &MainFrame::OnKonstParameterChanged, this); // Zazna spemembo na RadioBox-u
	sliderHitrosti->Bind(wxEVT_SLIDER, &MainFrame::OnSliderChanged, this); // Zazna spemembo na slider-ju
	sliderDelovniTlak->Bind(wxEVT_SLIDER, &MainFrame::OnSliderChanged, this);
	sliderOsnovniTlak->Bind(wxEVT_SLIDER, &MainFrame::OnSliderChanged, this);
	sliderMoc->Bind(wxEVT_SLIDER, &MainFrame::OnSliderChanged, this);
	sliderVrtljaji->Bind(wxEVT_SLIDER, &MainFrame::OnSliderChanged, this);

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


void MainFrame::OnButtonSimClicked(wxCommandEvent& evt) { // Funkcija ob pritisku na Gumb 'sim'

	if (!boolSimulacija) {
		boolSimulacija = true;
	}
	else if (boolSimulacija) {
		boolSimulacija = false;
	}


	while (boolSimulacija) {
		casSimulacije++;
		Sleep(51 - sliderHitrosti->GetValue()); //////////// Mogoc spremenit hitrost z zmansanjem Vrtljaja, mogoc ne
		Refresh();
		wxYield();

		/**/std::vector<wxSlider*> seznamSil; // Seznam sil
		seznamSil.push_back(sliderDelovniTlak);
		seznamSil.push_back(sliderOsnovniTlak);
		seznamSil.push_back(sliderMoc);
		seznamSil.push_back(sliderVrtljaji);

		izracun(&stanje, static_cast<std::string>(konstParameter->GetStringSelection()), seznamSil); // Izracun
	}
}

void MainFrame::OnButtonResetClicked(wxCommandEvent& evt) { // Funkcija ob pritisku na Gumb 'reset'
	
	boolSimulacija = false;
	casSimulacije = 0;
	stanje.zasuk = 0;

	Refresh();
}

void MainFrame::OnButtonNastavitveClicked(wxCommandEvent& evt) { // Funkcija ob pritisku na Gumb 'nastavitve'

	boolSimulacija = false;
	casSimulacije = 0;
	stanje.zasuk = 0;

	zobnik.stZob = ctrlStZob->GetValue();
	zobnik.modul = ctrlModul->GetValue();
	zobnik.debelina = ctrlDebelina->GetValue();
	izracunZobnika(&zobnik);

	Refresh();
}

void MainFrame::OnKonstParameterChanged(wxCommandEvent& evt) { // Funkcija ob zaznani spremembi na RadioBox-u

	if (konstParameter->GetStringSelection() == "Konst. moc") {
		sliderMoc->Enable();
		sliderVrtljaji->Disable();
	}
	else if (konstParameter->GetStringSelection() == "Konst. vrtljaji") {
		sliderMoc->Disable();
		sliderVrtljaji->Enable();
	}

	if (!boolSimulacija) Refresh();
}


void MainFrame::OnSliderChanged(wxCommandEvent& evt) { // Funkcija ob zaznani spremembi na Slider-ju

	stanje.vrtljaji = sliderVrtljaji->GetValue();
	stanje.moc = sliderMoc->GetValue();
	stanje.delovniTlak = sliderDelovniTlak->GetValue();
	stanje.osnovniTlak = sliderOsnovniTlak->GetValue();

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

	dc.DrawText("St zob:", wxPoint(5, 253));
	dc.DrawText("Modul:", wxPoint(5, 283));
	dc.DrawText("Debelina:", wxPoint(5, 313));

	dc.DrawText(wxString::Format("dw = %g mm", zobnik.premerKinematskegaKroga), wxPoint(5, 353));
	dc.DrawText(wxString::Format("df = %g mm", zobnik.premerKorenjskegaKroga), wxPoint(5, 383));
	dc.DrawText(wxString::Format("da = %g mm", zobnik.premerTemenskegaKroga), wxPoint(5, 413));

	//// Nastavitve simulacije
	dc.DrawText("Nastavitve simulacije:", wxPoint(x_okno + sirina + 5, 225));
	dc.DrawLine(wxPoint(x_okno + sirina - 1, 240), wxPoint(sirina_panel, 240));
	dc.DrawLine(wxPoint(sirina_panel - 1, 240), wxPoint(sirina_panel - 1, 599));
	dc.DrawLine(wxPoint(x_okno + sirina - 1, 599), wxPoint(sirina_panel, 599));

	dc.DrawText(wxString::Format("Delovni tlak %g bar", stanje.delovniTlak), wxPoint(x_okno + sirina + 5, 255));
	dc.DrawText(wxString::Format("Osnovni tlak %g bar", stanje.osnovniTlak), wxPoint(x_okno + sirina + 5, 305));
	dc.DrawText(wxString::Format("Moc crpalke %g kW", stanje.moc), wxPoint(x_okno + sirina + 5, 355));
	dc.DrawText(wxString::Format("Vrljaji crpalke %g min^-1", stanje.vrtljaji), wxPoint(x_okno + sirina + 5, 405));


	// Admin Logs
	if (false) {
		dc.DrawText(wxString::Format("t = %d", rot), wxPoint(x_okno + 12, y_okno + 20));
		//dc.DrawText(wxString::Format("Cos: %g", cos(rot * 2 * M_PI / 360)), wxPoint(x_okno + 12, y_okno + 35));
		dc.DrawText(wxString::Format("z = %d", zobnik.stZob), wxPoint(x_okno + 12, y_okno + 50));
		dc.DrawText(wxString::Format("m = %g", zobnik.modul), wxPoint(x_okno + 12, y_okno + 65));
		dc.DrawText(wxString::Format("b = %g", zobnik.debelina), wxPoint(x_okno + 12, y_okno + 80));
		dc.DrawText(wxString::Format("dw = %g", zobnik.premerKinematskegaKroga), wxPoint(x_okno + 12, y_okno + 95));
		dc.DrawText(wxString::Format("df = %g", zobnik.premerKorenjskegaKroga), wxPoint(x_okno + 12, y_okno + 110));
		dc.DrawText(wxString::Format("da = %g", zobnik.premerTemenskegaKroga), wxPoint(x_okno + 12, y_okno + 125));
		//dc.DrawText(wxString::Format("test: %g", stanje.zasuk), wxPoint(x_okno + 12, y_okno + 155));
	}
	

	// Prikaz crpalke
	//// Prikaz ohisja
	dc.SetPen(wxPen(wxColor(0,0,0), 3, wxPENSTYLE_SOLID)); // Definiramo debelo crto

	dc.DrawLine(wxPoint(x_okno + sirina / 2 - 30, y_okno + visina / 2 - 102 - 60), wxPoint(x_okno + sirina / 2 - 30, y_okno + visina / 2 - 102)); // levo ohisje
	dc.DrawLine(wxPoint(x_okno + sirina / 2 - 30, y_okno + visina / 2 - 102), wxPoint(x_okno + sirina / 2 - 30 - 50, y_okno + visina / 2 - 102));
	dc.DrawArc(wxPoint(x_okno + sirina / 2 - 30 - 50, y_okno + visina / 2 - 102), wxPoint(x_okno + sirina / 2 - 30 - 50, y_okno + visina / 2 + 102), wxPoint(x_okno + sirina / 2 - 30 - 50, y_okno + visina / 2));
	dc.DrawLine(wxPoint(x_okno + sirina / 2 - 30 - 50, y_okno + visina / 2 + 102), wxPoint(x_okno + sirina / 2 - 30, y_okno + visina / 2 + 102));
	dc.DrawLine(wxPoint(x_okno + sirina / 2 - 30, y_okno + visina / 2 + 102), wxPoint(x_okno + sirina / 2 - 30, y_okno + visina / 2 + 102 + 60));

	dc.DrawLine(wxPoint(x_okno + sirina / 2 + 30, y_okno + visina / 2 + 102 + 60), wxPoint(x_okno + sirina / 2 + 30, y_okno + visina / 2 + 102)); // desno ohisje
	dc.DrawLine(wxPoint(x_okno + sirina / 2 + 30, y_okno + visina / 2 + 102), wxPoint(x_okno + sirina / 2 + 30 + 50, y_okno + visina / 2 + 102));
	dc.DrawArc(wxPoint(x_okno + sirina / 2 + 30 + 50, y_okno + visina / 2 + 102), wxPoint(x_okno + sirina / 2 + 30 + 50, y_okno + visina / 2 - 102), wxPoint(x_okno + sirina / 2 + 30 + 50, y_okno + visina / 2));
	dc.DrawLine(wxPoint(x_okno + sirina / 2 + 30 + 50, y_okno + visina / 2 - 102), wxPoint(x_okno + sirina / 2 + 30, y_okno + visina / 2 - 102));
	dc.DrawLine(wxPoint(x_okno + sirina / 2 + 30, y_okno + visina / 2 - 102), wxPoint(x_okno + sirina / 2 + 30, y_okno + visina / 2 - 102 - 60));

	dc.SetPen(wxPen(wxColor(255, 255, 255), 3, wxPENSTYLE_SOLID)); // Odstranjevanje grde crte zaradi 'DrawArc'
	dc.DrawLine(wxPoint(x_okno + sirina / 2 - 30 - 50, y_okno + visina / 2 - 102 + 2), wxPoint(x_okno + sirina / 2 - 30 - 50, y_okno + visina / 2 + 102 - 2));
	dc.DrawLine(wxPoint(x_okno + sirina / 2 + 30 + 50, y_okno + visina / 2 + 102 - 2), wxPoint(x_okno + sirina / 2 + 30 + 50, y_okno + visina / 2 - 102 + 2));
	dc.SetPen(wxPen(wxColor(0, 0, 0), 3, wxPENSTYLE_SOLID));

	//// Vrednosti tlakov
	dc.DrawText(wxString::Format("p = %d bar", sliderDelovniTlak->GetValue()), wxPoint(x_okno + sirina / 2 - 30, y_okno + visina / 2 - 102 - 80));
	dc.DrawText(wxString::Format("p = %d bar", sliderOsnovniTlak->GetValue()), wxPoint(x_okno + sirina / 2 - 30, y_okno + visina / 2 + 102 + 60));
	
	//// Prikaz zobnikov
	int stZob = zobnik.stZob; // Stevilo zob na zobiku
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

			double zasuk = stanje.zasuk; // Lokalna spremenljivka

			wxPoint* tocka;
			tocka = new wxPoint(x_okno + sirina / 2 + cos(smer * zasuk * 2 * M_PI / 360 + M_PI / stZob * i) * dol, y_okno + visina / 2 + sin(smer * zasuk * 2 * M_PI / 360 + M_PI / stZob * i) * dol); // Dolocitev tock zobnikov
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
