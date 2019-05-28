
// ChildView.cpp : implementation of the CChildView class
//
#include "stdafx.h"
#include "simple.h"
#include "ChildView.h"
#include "Application.h"
#include "TestDlg.h"
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView
CChildView::CChildView()
{
	app = new Application(this);
	app->run();
	start = false;
	select_mode = false;
	m_drawShapeID = QUAD;
	m_color = RGB(255,0,0);

	TestDlg = new CTestDlg(this);
	TestDlg->Create(IDD_DIALOG1);
	TestDlg->ShowWindow(SW_SHOW);
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_COMMAND(ID_COLOR_RED, &CChildView::OnColorRed)
	ON_COMMAND(ID_COLOR_BLUE, &CChildView::OnColorBlue)
	ON_COMMAND(ID_COLOR_GREEN, &CChildView::OnColorGreen)
	ON_COMMAND(ID_SHAPE_ELIPSE, &CChildView::OnShapeElipse)
	ON_COMMAND(ID_SHAPE_LINE, &CChildView::OnShapeLine)
	ON_COMMAND(ID_SHAPE_RECTANGLE, &CChildView::OnShapeRectangle)
	ON_COMMAND(ID_SHAPE_TRIANGLE, &CChildView::OnShapeTriangle)
	ON_COMMAND(ID_SHAPE_SELECT, &CChildView::OnShapeSelect)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_FILE_OPEN, &CChildView::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CChildView::OnFileSave)
END_MESSAGE_MAP()

// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
		
	// TODO: Add your message handler code here

	CFont font;
	font.CreatePointFont(120, "궁서");
	dc.SelectObject(&font);
	
	app->RenewTable();
	app->DrawBuffers(&dc);


	// Do not call CWnd::OnPaint() for painting messages

}

Shape* CChildView::GetCurShape()
{
	POSITION pos = m_shapeList.GetHeadPosition();

	for(int i=0;i<m_shapeList.GetSize();i++)
	{
		Shape* tmp = m_shapeList.GetNext(pos);

		if(tmp->get_selected()) // 선택된 도형을 발견한다면
		{
			return tmp; // 그 도형을 반환한다.
		}
	}

	return NULL;
}

Shape* CChildView::AllocShape()
{
	Shape* tempShape;

	switch(m_drawShapeID)
	{
	case QUAD:
		tempShape = new Quad();
		break;
	}

	return tempShape;
}

void CChildView::OnColorRed()
{
	// TODO: Add your command handler code here
	m_color = RGB(255,0, 0);
	Invalidate();
}


void CChildView::OnColorBlue()
{
	// TODO: Add your command handler code here
	m_color = RGB(0,0,255);
	Invalidate();
}


void CChildView::OnColorGreen()
{
	// TODO: Add your command handler code here
	m_color = RGB(0,255,0);
	Invalidate();
}


void CChildView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: Add your message handler code here
	CMenu menu;
	menu.LoadMenu(IDR_MAINFRAME);
	CMenu *pMenu = menu.GetSubMenu(5);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());

}

void CChildView::CancleSelection()
{
	POSITION pos = m_shapeList.GetHeadPosition();

	for(int i=0;i<m_shapeList.GetSize();i++)
	{
		Shape* tmp = m_shapeList.GetNext(pos);
		tmp->Cancle();				
	}
}

void CChildView::OnShapeElipse()
{
	// TODO: Add your command handler code here
	m_drawShapeID = CIRCLE;
	select_mode = false;

}


void CChildView::OnShapeLine()
{
	// TODO: Add your command handler code here
	m_drawShapeID = LINE;
	select_mode = false;
}


void CChildView::OnShapeRectangle()
{
	// TODO: Add your command handler code here
	m_drawShapeID = QUAD;
	select_mode = false;
}


void CChildView::OnShapeTriangle()
{
	// TODO: Add your command handler code here
	m_drawShapeID = TRI;
	select_mode = false;
}


void CChildView::OnShapeSelect()
{
	// TODO: Add your command handler code here
	select_mode = true;
}


void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	
	cur_Shape = GetCurShape();

	if(cur_Shape != NULL) // 선택 된 도형이 있을 때
	{
		cur_Shape->MoveShape(nChar);
		Invalidate();
		UpdateWindow();
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CChildView::OnFileOpen()
{
	// TODO: Add your command handler code here
	CFileDialog dlg(TRUE);

	if(dlg.DoModal() == IDOK)
	{
		CFile file;
		CFileException e;

		if(!file.Open(dlg.GetPathName(), CFile::modeReadWrite, &e))
			e.ReportError();
			
		m_shapeList.RemoveAll();

		int count; // 리스트 갯수
		file.Read(&count,sizeof(count));
	
		for(int i=0;i<count;i++)
		{
			ShapeID tmpID;
			Shape* tmpShape;
			file.Read(&tmpID,sizeof(tmpID));
			
		
			if(tmpID == QUAD)
			{
				tmpShape = new Quad();
			}
		
			tmpShape->LoadFromFile(file);
			m_shapeList.AddTail(tmpShape);
		}

		Invalidate();
		UpdateWindow();
		file.Close();		

	}

}


void CChildView::OnFileSave()
{
	// TODO: Add your command handler code here
	CFileDialog dlg(FALSE);


	CFile file;
	CFileException e;
	CString m_strPath;

	if(dlg.DoModal() == IDOK)
	{		
		if(!file.Open(dlg.GetPathName(),CFile::modeCreate | CFile::modeReadWrite, &e))
			e.ReportError();
		
		int count = m_shapeList.GetSize();
		POSITION pos = m_shapeList.GetHeadPosition();

		file.Write(&count, sizeof(count));

		for(int i=0;i<count;i++)
		{
			Shape* tmp = m_shapeList.GetNext(pos);
			tmp->SaveTOFile(file);				
		}
		file.Close();		
	}
}