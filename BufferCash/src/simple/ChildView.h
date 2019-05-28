
// ChildView.h : interface of the CChildView class
//

#pragma once
#include "Quad.h"
#include "Application.h"
#include "TestDlg.h"

class CTestDlg;
class Application;

// CChildView window
class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:
	
// Operations
public:
	Shape* AllocShape();
	Shape* GetCurShape();
	void CancleSelection();
	
// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

	Application* app; // ���� ��ü

	// Generated message map functions
protected:
	COLORREF m_color;
	afx_msg void OnPaint();
		
	DECLARE_MESSAGE_MAP()
	CList<Shape*, Shape*> m_shapeList;
	
	Shape* m_drawShape;
	Shape* cur_Shape;

	ShapeID m_drawShapeID;
	CPoint m_curMousePoint;

	bool start;
	bool select_mode; //  ���� ���
	CTestDlg* TestDlg; // �׽�Ʈ ���̾�α�

public:
	afx_msg void OnColorRed();
	afx_msg void OnColorBlue();
	afx_msg void OnColorGreen();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnShapeElipse();
	afx_msg void OnShapeLine();
	afx_msg void OnShapeRectangle();
	afx_msg void OnShapeTriangle();
	afx_msg void OnShapeSelect();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
};

