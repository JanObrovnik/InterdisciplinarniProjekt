#pragma once
#include <wx/wx.h>

class GrafFrame : public wxFrame
{
public:
	GrafFrame(const wxString& title);

	void OnPaint(wxPaintEvent& evt);
private:
	void OnSizeChanged(wxSizeEvent& evt);

	void OnButtonNastaviClicked(wxCommandEvent& evt);
	void OnButtonRisiClicked(wxCommandEvent& evt);
	void OnButtonShraniClicked(wxCommandEvent& evt);

	void OnRadioVelicinaXChange(wxCommandEvent& evt);
};

class NastavitveZobnika : public wxFrame
{
public:
	NastavitveZobnika();

	void OnPaint(wxPaintEvent& evt);
private:
	void OnButtonUporabiClicked(wxCommandEvent& evt);
	void OnButtonZapriClicked(wxCommandEvent& evt);
};


struct PodatkiZobnika {

	float stZob = 11;
	float modul = 4;
	float debelina = 40; // [mm]
	float premerKinematskegaKroga = modul * stZob; // [mm]
	float premerKorenjskegaKroga = premerKinematskegaKroga - 2.5 * modul; // [mm]
	float premerTemenskegaKroga = premerKinematskegaKroga + 2 * modul; // [mm]
	float kotPhi = 20. / 180. * M_PI; // [rad]
	float dolzinaKontakta = sqrt(pow(premerTemenskegaKroga, 2) - pow(premerKinematskegaKroga, 2) * pow(cos(kotPhi), 2)) - premerKinematskegaKroga * sin(kotPhi); // [mm]

	float vrtljaji = 2; // [s-1]
	float tlak = 20; // [bar]
	float iztisnina = 20; // [m3/vrt]
	float izkoristek = .9; // [/]
};