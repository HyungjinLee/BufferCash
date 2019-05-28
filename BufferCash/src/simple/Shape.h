#pragma once
#include "stdafx.h"
enum ShapeID {QUAD,TRI, CIRCLE, LINE};
enum MouseEvent { MOVE, DRAG, LBUTTON_DOWN, LBUTTON_UP, RBUTTON_UP };
enum KeyEvent { LEFT, RIGHT, UP, DOWN, A, D, R};

class Shape : public CObject
{
	//DECLARE_SERIAL(Shape);
public:
	Shape() : m_start(0), m_curNum(0), m_shapeBoundary(0,0,0,0){ 
		
		m_selected = false;
		seta = 0.0;
	}
	virtual ~Shape() { delete[] m_vertex; }

	int IsCompleted() { if(m_start == -1) return 1; else return 0; }
	void SetStart(){ m_start = 1; }
	int IsStarted() { if(m_start >0) return 1; else return 0; }
	void NormalEnd() { m_start = -1; }
	void Select(){ m_selected = true; }
	void Cancle(){ m_selected = false; }

	ShapeID get_shapeID() { return m_shapeID;}

	CRect GetShapeBoundary(){ return m_shapeBoundary;}
	
	virtual void DisplaySelected(CDC* pDC)
	{
		pDC->Rectangle(m_shapeBoundary);
		pDC->FillSolidRect(m_shapeBoundary, RGB(0, 255, 255));
		
	};

	virtual void Draw(MouseEvent ev, int x, int y, UINT nFlags)=0;
	virtual void Display(CDC* pDC, int trigger, int blknum, int state) = 0;
	virtual bool get_selected() { return m_selected; };
	virtual void MoveShape(UINT nChar) = 0;

	void LoadFromFile(CFile &fin)
	{
		fin.Read(&m_shapeBoundary.left, sizeof(m_shapeBoundary.left));
		fin.Read(&m_shapeBoundary.top, sizeof(m_shapeBoundary.top));
		fin.Read(&m_shapeBoundary.right, sizeof(m_shapeBoundary.right));
		fin.Read(&m_shapeBoundary.bottom, sizeof(m_shapeBoundary.bottom));
		fin.Read(&m_start, sizeof(m_start));
		fin.Read(&m_selected, sizeof(m_selected));
		fin.Read(&m_nFlags, sizeof(m_nFlags));
		
		if(m_shapeID == CIRCLE)
		{
			fin.Read(&m_c1, sizeof(m_c1));
			fin.Read(&m_c2, sizeof(m_c2));
			fin.Read(&seta, sizeof(seta));
		}

		else
		{
			m_vertex = new CPoint[4];

			fin.Read(&m_vertex[0], sizeof(m_vertex[0]));
			fin.Read(&m_vertex[1], sizeof(m_vertex[1]));
			fin.Read(&m_vertex[2], sizeof(m_vertex[2]));
			fin.Read(&m_vertex[3], sizeof(m_vertex[3]));
		}
		
	}

	void SaveTOFile(CFile &fout){ 

		fout.Write(&m_shapeID, sizeof(m_shapeID));
		fout.Write(&m_shapeBoundary.left, sizeof(m_shapeBoundary.left));
		fout.Write(&m_shapeBoundary.top, sizeof(m_shapeBoundary.top));
		fout.Write(&m_shapeBoundary.right, sizeof(m_shapeBoundary.right));
		fout.Write(&m_shapeBoundary.bottom, sizeof(m_shapeBoundary.bottom));
		fout.Write(&m_start, sizeof(m_start));
		fout.Write(&m_selected, sizeof(m_selected));
		fout.Write(&m_nFlags, sizeof(m_nFlags));

		if(m_shapeID == CIRCLE)
		{
			fout.Write(&m_c1, sizeof(m_c1));
			fout.Write(&m_c2, sizeof(m_c2));
			fout.Write(&seta, sizeof(seta));
		}
		
		else
		{
		
			fout.Write(&m_vertex[0], sizeof(m_vertex[0]));
			fout.Write(&m_vertex[1], sizeof(m_vertex[1]));
			fout.Write(&m_vertex[2], sizeof(m_vertex[2]));
			fout.Write(&m_vertex[3], sizeof(m_vertex[3]));

		}

	};
		

protected:

	ShapeID m_shapeID; // 도형의 type
	
	CPoint m_c1;
	CPoint m_c2;
	double seta; // 회전을 위한 각도
	
	CPoint center; // 중심
	int m_start; // 도형이 다 그려졌는지 그리고 있는지 상태
	CPoint* m_vertex; // 그려야 할 도형의 좌표들을 저장할 포인터
	int m_vertexLength; // 저장할 좌표들의 최대 갯수
	int m_curNum; // 저장된 좌표들의 갯수
	CRect m_shapeBoundary; // 도형이 그려질 영역
	bool m_selected; // 선택되었는지 여부
	
	UINT m_nFlags; // View 클래스에서 전달받은 마우스 이벤트의 flag값
	UINT m_Char; // View 클래스에서 전달받은 키 이벤트의 flag값
};


