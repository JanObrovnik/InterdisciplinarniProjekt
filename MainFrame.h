#pragma once
#include <wx/wx.h>

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);

	void OnPaint(wxPaintEvent& evt); // Za risanje na frame
private:
	void OnSliderChanged(wxCommandEvent& evt); // Funkcija ob zaznani spremembi na slider-ju

	void OnMouseEvent(wxMouseEvent& evt); // Funkcija ob zaznani spremembi na miski
};
