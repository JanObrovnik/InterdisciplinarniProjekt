#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <math.h>
//#include <algorithm>



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
wxChoice* choice_dod;
wxChoice* choice_izb;
wxSlider* slider;

std::vector<std::vector<int>> seznam_valjev;

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {

	panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS);

	wxButton* button_dod = new wxButton(panel, wxID_ANY, "Dodaj element", wxPoint(5, 40), wxSize(120, -1));
	wxButton* button_izb = new wxButton(panel, wxID_ANY, "Izbrisi element", wxPoint(5, 160), wxSize(120, -1));
	wxButton* button_izb_vse = new wxButton(panel, wxID_ANY, "Izbrisi vse", wxPoint(5, 280), wxSize(120, -1));

	wxArrayString choices;
	choices.Add("Element 0");
	choices.Add("Element 1");
	choices.Add("Element 2");
	choices.Add("Element 3");

	choice_dod = new wxChoice(panel, wxID_ANY, wxPoint(5, 20), wxSize(120, -1), choices, wxCB_SORT);
	choice_dod->SetSelection(0);
	choice_izb = new wxChoice(panel, wxID_ANY, wxPoint(5, 140), wxSize(120, -1), choices, wxCB_SORT);
	choice_izb->SetSelection(0);

	slider = new wxSlider(panel, wxID_ANY, 0, 0, 100, wxPoint(200, 0), wxSize(600, -1));

	panel->Bind(wxEVT_LEFT_DOWN, &MainFrame::OnMouseEvent, this);
	button_dod->Bind(wxEVT_BUTTON, &MainFrame::OnButtonDodClicked, this);
	button_izb->Bind(wxEVT_BUTTON, &MainFrame::OnButtonIzbClicked, this);
	button_izb_vse->Bind(wxEVT_BUTTON, &MainFrame::OnButtonIzbVseClicked, this);
	choice_dod->Bind(wxEVT_CHOICE, &MainFrame::OnChoicesClicked, this);
	slider->Bind(wxEVT_SLIDER, &MainFrame::OnSliderChanged, this);

	panel->Connect(wxEVT_PAINT, wxPaintEventHandler(MainFrame::OnPaint));

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

void MainFrame::OnButtonDodClicked(wxCommandEvent& evt) {

	if (dx != x || dy != y) {

		wxString naziv_gumb = wxString::Format("Gumb za element %d", choice_dod->GetSelection());


		x = dx;
		y = dy;
		seznam_valjev.push_back({ x, y });

	}
	else wxLogStatus("Izberi lokacijo z levik klikom");

	Refresh();
}

void MainFrame::OnButtonIzbClicked(wxCommandEvent& evt) {

	int n = choice_izb->GetSelection();

	if (n < seznam_valjev.size()) {

		seznam_valjev.erase(seznam_valjev.begin() + n);

		Refresh();
	}
	else wxLogStatus("Error");
}

void MainFrame::OnButtonIzbVseClicked(wxCommandEvent& evt) {

	seznam_valjev.clear();

	Refresh();
}

void MainFrame::OnChoicesClicked(wxCommandEvent& evt) {

	wxString naziv_element = wxString::Format("Item %d", choice_dod->GetSelection());

	wxLogStatus(naziv_element);

	Refresh();
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



	dc.DrawText(wxString::Format("%d", rot), wxPoint(x_okno + sirina / 2, y_okno + 20));
	dc.DrawText(wxString::Format("%g", cos(rot * 2 * M_PI / 12)), wxPoint(x_okno + sirina / 2, y_okno + 40));

	int x = cos(rot * 2 * M_PI / 12) * 120;
	int y = sin(rot * 2 * M_PI / 12) * 120;

	dc.SetPen(wxPen(wxColor(0,0,0), 3, wxPENSTYLE_SOLID));
	dc.DrawLine(wxPoint(x_okno + sirina / 2, y_okno + visina / 2), wxPoint(x_okno + sirina / 2 + x, y_okno + visina / 2 + y));
	//dc.SetPen(wxPen(wxColor(0, 0, 0), 1, wxPENSTYLE_SOLID));

	wxPoint ha (x_okno + sirina / 4, y_okno + visina / 4);
	wxPoint ha2 = ha + wxPoint(0, 20);
	dc.DrawLine(ha, ha2);
}



/*
povezava za dokumentacijo kontrol
https://docs.wxwidgets.org/3.0/group__group__class__ctrl.html
*/
