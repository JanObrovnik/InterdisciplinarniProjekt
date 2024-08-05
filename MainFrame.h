#pragma once
#include <wx/wx.h>

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);

	void OnPaint(wxPaintEvent& evt); // Za risanje na frame
private:
	void OnButtonSimClicked(wxCommandEvent& evt); // Funkcija ob pritisku na Gumb 'sim'
	void OnButtonResetClicked(wxCommandEvent& evt); // Funkcija ob pritisku na Gumb 'reset'
	void OnButtonNastavitveClicked(wxCommandEvent& evt); // Funkcija ob pritisku na Gumb 'nastavitve'
	void OnKonstParameterChanged(wxCommandEvent& evt); // Funkcija ob zaznani spremembi na SpinCtrl-ju
	void OnSliderHitrostiChanged(wxCommandEvent& evt); // Funkcija ob zaznani spremembi na Slider-ju

	void OnMouseEvent(wxMouseEvent& evt); // Funkcija ob zaznani spremembi na miski
};


struct PodatkovnaBazaZobnika { // Baza podatkov

	short stZob;
	float modul;
	float debelina;
	float premerKinematskegaKroga;
	float premerKorenjskegaKroga;
	float premerTemenskegaKroga;
};

struct StanjeZobnika {

	double zasuk = 0; // [deg]
};