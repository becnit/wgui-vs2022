#include <iostream>
#include <thread>
#include "WGUI.hpp"
#include <cstdio>

using namespace std;
using namespace WGUI;

void console()
{
	std::wstring str;

	while (std::wcin >> str)
	{
		
	}
}


int main()
{
	setlocale(LC_ALL, "chs");
	
	//std::thread thread_console(console);
	//thread_console.detach();


	WGUI::Winform wf;
	WGUI::DesktopWindow Desktop;
	
	wf.Rect = { 0,0,1584,866 };
	wf.Create();

	WGUI::SetAlign(wf, Desktop, WGUI::AlignMode::Center, WGUI::AlignMode::Center);

	PushButton pb;
	pb.Rect = { 50,50,150,30 };
	pb.Create(wf);
	pb.Caption = L"PushButon";

	SplitButton sb;
	sb.Rect = { 50,100,150,30 };
	sb.Caption = L"SplitButton";
	sb.Create(wf);
	
	ThreeStateButton tse;
	tse.Rect = { 50,150,150,30 };
	tse.Caption = L"ThreeStateButton";
	tse.Create(wf);

	CheckBox cb;
	cb.Rect = { 50,200,150,30 };
	cb.Caption = L"CheckBox";
	cb.Create(wf);

	RadioButton rb;
	rb.Rect = { 50,250,150,30 };
	rb.Caption = L"RadioButton";
	rb.Create(wf);

	GroupBox gb;
	gb.Rect = { 250,50,300,600 };
	gb.Caption = L"GroupBox";
	gb.Create(wf);

	CommandLink cl;
	cl.Rect = { 50,300,150,80 };
	cl.Caption = L"CommandLink";
	cl.Create(wf);
	cl.NoteText = L"NoteText";
	
	ComboBox cb2;
	cb2.Rect = { 50,400,150,30 };
	cb2.Create(wf);
	cb2.AddItems({ L"Item1", L"Item2", L"Item3", L"Item4", L"Item5" });

	EditBox eb;
	eb.Rect = { 50,450,150,30 };
	eb.Create(wf);
	eb.SetText(L"EditBox");
	eb.Border = EditBoxBorder::Border;
	eb.ScrollBar = CtrlScrollBar::Horz;

	ListBox lb;
	lb.Rect = { 50,500,150,300 };
	lb.Create(wf);
	lb.AddItems({ L"Item1", L"Item2", L"Item3", L"Item4", L"Item5" });
	lb.Border = ListBoxBorder::Border;
	
	if (wf.IsCreate())
		return WGUI::MessageLoop();
	return 0;
}
