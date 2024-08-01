#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <math.h>



std::vector<double> izracun(int n) {
	std::vector<double> res;


	
	return res;
}



int x = 300;
int y = 250;
int dx = x;
int dy = y;
int velikost_x = 120;
int velikost_y = 60;

wxPanel* panel;
wxSlider* slider;

std::vector<std::vector<int>> seznam_valjev;

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {

	panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS);


	slider = new wxSlider(panel, wxID_ANY, 0, 0, 1000, wxPoint(200, 0), wxSize(600, -1));


	panel->Bind(wxEVT_LEFT_DOWN, &MainFrame::OnMouseEvent, this);
	slider->Bind(wxEVT_SLIDER, &MainFrame::OnSliderChanged, this);

	panel->Connect(wxEVT_PAINT, wxPaintEventHandler(MainFrame::OnPaint));
	panel->SetDoubleBuffered(true);

	wxStatusBar* statusBar = CreateStatusBar();
	statusBar->SetDoubleBuffered(true);
}



void MainFrame::OnMouseEvent(wxMouseEvent& evt) {

	wxPoint mousePos = wxGetMousePosition();
	mousePos = this->ScreenToClient(mousePos);

	dx = round(static_cast<float>(mousePos.x) / 20) * 20;
	dy = round(static_cast<float>(mousePos.y) / 20) * 20;

	wxString message = wxString::Format("Mouse Event Detected! (x=%d y=%d)", dx, dy);
	wxLogStatus(message);
}



void MainFrame::OnSliderChanged(wxCommandEvent& evt) {

	wxLogStatus("Slider change event");

	Refresh();
}

void MainFrame::OnPaint(wxPaintEvent& event) {

	wxPaintDC dc(this);


	int x_okno = 150;
	int y_okno = 30;
	wxSize size = this->GetSize();
	int sirina_panel = size.x;
	int visina_panel = size.y;
	int sirina = sirina_panel - 2 * x_okno;
	int visina = visina_panel - y_okno;

	int rot = slider->GetValue();


	dc.SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_SOLID));
	dc.DrawRectangle(x_okno, y_okno, sirina, visina);


	if (true) { // Admin Logs
		dc.DrawText(wxString::Format("Slider: %d", rot), wxPoint(x_okno + sirina / 2, y_okno + 20));
		dc.DrawText(wxString::Format("Cos: %g", cos(rot * 2 * M_PI / 360)), wxPoint(x_okno + sirina / 2, y_okno + 40));
	}


	dc.SetPen(wxPen(wxColor(0,0,0), 3, wxPENSTYLE_SOLID));

	int stZob = 11;
	sirina -= 160;

	for (int j = 0; j < 2; j++) {

		if (j == 1) sirina += 320;
		int smer = 1;
		if (j == 1) smer = -1;

		wxPointList* seznamTock = new wxPointList();
		wxPoint* tocka0 = new wxPoint();

		for (int i = 0; i < stZob * 2; i++) {

			int dol = 120;
			if (i % 2 == 0) dol /= 3;

			wxPoint* tocka;
			tocka = new wxPoint(x_okno + sirina / 2 + cos(smer * rot * 2 * M_PI / 360 + M_PI / stZob * i) * dol, y_okno + visina / 2 + sin(smer * rot * 2 * M_PI / 360 + M_PI / stZob * i) * dol);
			seznamTock->Append(tocka);

			if (i == 0) tocka0 = tocka;
			if (i == stZob * 2 - 1) seznamTock->Append(tocka0);
		}

		dc.DrawSpline(seznamTock);
	}

	dc.SetPen(wxPen(wxColor(0, 0, 0), 1, wxPENSTYLE_SOLID));
}



/*
povezava za dokumentacijo kontrol
https://docs.wxwidgets.org/3.0/group__group__class__ctrl.html
*/
