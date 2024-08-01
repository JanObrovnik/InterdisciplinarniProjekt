#pragma once
#include <wx/wx.h>

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);

	void OnPaint(wxPaintEvent& evt);
private:
	void OnSliderChanged(wxCommandEvent& evt);

	void OnMouseEvent(wxMouseEvent& evt);
};
