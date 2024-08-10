#pragma once
#include <wx/wx.h>

class GrafFrame : public wxFrame
{
public:
	GrafFrame(const wxString& title);

	void OnPaint(wxPaintEvent& evt);
private:
	void OnSizeChanged(wxSizeEvent& evt);

	void OnButtonRisiClicked(wxCommandEvent& evt);
	void OnButtonShraniClicked(wxCommandEvent& evt);
};
