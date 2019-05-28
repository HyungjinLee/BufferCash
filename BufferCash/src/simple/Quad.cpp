#include "stdafx.h"
#include "Quad.h"
#include <cmath>

Quad::Quad()
{
	m_shapeID = QUAD;
	m_vertexLength = 4;
	m_vertex = new CPoint[m_vertexLength];
	

}

void Quad::Draw(MouseEvent ev, int x, int y, UINT nFlags)
{
	m_nFlags = nFlags;
	
		CString tmp_str;
	
	switch(ev)
	{
	case LBUTTON_DOWN:

		AddPoint(x,y);
		AddPointCurPos(x,y);
		SetStart();

		break;
	case LBUTTON_UP:

	//	AddPoint(x,y);

		

		NormalEnd();
		break;

	case MOVE:
		if(IsStarted())
			AddPointCurPos(x,y);
		break;
	}

}


void Quad::MoveShape(UINT nChar)
{
	switch(nChar)
	{
	case VK_LEFT:
		m_vertex[0].x -= 1;
		m_vertex[1].x -= 1;
		m_vertex[2].x -= 1;
		m_vertex[3].x -= 1;

		m_shapeBoundary.left -=1;
		m_shapeBoundary.right -=1;

		break;

	case VK_RIGHT:
		m_vertex[0].x += 1;
		m_vertex[1].x += 1;
		m_vertex[2].x += 1;
		m_vertex[3].x += 1;

		m_shapeBoundary.left +=1;
		m_shapeBoundary.right +=1;

		break;
		
	case VK_DOWN:
		m_vertex[0].y += 1;
		m_vertex[1].y += 1;
		m_vertex[2].y += 1;
		m_vertex[3].y += 1;

		m_shapeBoundary.top +=1;
		m_shapeBoundary.bottom +=1;

		break;

	
	case VK_UP:
		m_vertex[0].y -= 1;
		m_vertex[1].y -= 1;
		m_vertex[2].y -= 1;
		m_vertex[3].y -= 1;

		m_shapeBoundary.top -=1;
		m_shapeBoundary.bottom -=1;
		break;

	case 'A':
		seta -= 1.0;
		

		break;

	case 'D':
		seta += 1.0;
		
		m_shapeBoundary.left = m_vertex[0].x;
		m_shapeBoundary.bottom = m_vertex[2].y; 


		break;

	default :
		break;
	}

}


void Quad::Display(CDC* pDC,int trigger, int blknum, int state)
{
		
	double rad = 3.141592/180*seta;
	
	center.x = (m_vertex[0].x + m_vertex[2].x)/2;
	center.y = (m_vertex[0].y + m_vertex[2].y)/2;

	m_vertex[0].x = (m_vertex[0].x-center.x)*cos(rad) + (m_vertex[0].y-center.y)*sin(rad) + center.x;
	m_vertex[0].y = (m_vertex[0].y-center.y)*cos(rad) - (m_vertex[0].x-center.x)*sin(rad) + center.y; 

	m_vertex[1].x = (m_vertex[1].x-center.x)*cos(rad) + (m_vertex[1].y-center.y)*sin(rad) + center.x;
	m_vertex[1].y = (m_vertex[1].y-center.y)*cos(rad) - (m_vertex[1].x-center.x)*sin(rad) + center.y; 

	m_vertex[2].x = (m_vertex[2].x-center.x)*cos(rad) + (m_vertex[2].y-center.y)*sin(rad) + center.x;
	m_vertex[2].y = (m_vertex[2].y-center.y)*cos(rad) - (m_vertex[2].x-center.x)*sin(rad) + center.y; 

	m_vertex[3].x = (m_vertex[3].x-center.x)*cos(rad) + (m_vertex[3].y-center.y)*sin(rad) + center.x;
	m_vertex[3].y = (m_vertex[3].y-center.y)*cos(rad) - (m_vertex[3].x-center.x)*sin(rad) + center.y; 

	CString blknum_s;
	blknum_s.Format(_T("%02d"), blknum);
	CString statestring;

	pDC->SetBkMode(OPAQUE);


	if (trigger == 1)
	{
		pDC->SetBkColor(RGB(0, 255, 0));
	}


	pDC->TextOut((m_vertex[0].x + m_vertex[3].x) / 2, (m_vertex[0].y + m_vertex[3].y) / 2, blknum_s);
		
	pDC->MoveTo(m_vertex[0].x, m_vertex[0].y);
	pDC->LineTo(m_vertex[1].x, m_vertex[1].y);
	pDC->MoveTo(m_vertex[1].x, m_vertex[1].y);
	pDC->LineTo(m_vertex[2].x, m_vertex[2].y);
	pDC->MoveTo(m_vertex[2].x, m_vertex[2].y);
	pDC->LineTo(m_vertex[3].x, m_vertex[3].y);
	pDC->MoveTo(m_vertex[3].x, m_vertex[3].y);
	pDC->LineTo(m_vertex[0].x, m_vertex[0].y);


	pDC->SetBkColor(RGB(255,255,255));


	if (state == LOCK) // 버퍼가 LOCK 상태이면
	{
		statestring = "LOCK";
	}
	else if (state == UNLOCK) // 버퍼가 UNLOCK 상태이면
	{
		statestring = "";
	}
	else if (state == DELAY) // 버퍼가 DELAY 상태이면
	{
		statestring = "DELAY";
	}
	else if (state == WRITE) // 버퍼가 WRITE 상태이면
	{
		statestring = "WRITE";
	}


	pDC->TextOut((m_vertex[0].x + m_vertex[3].x) / 2 - 10 , (m_vertex[0].y + m_vertex[3].y) / 2 + 20, statestring);





	
}

void Quad::AddPoint(int x, int y)
{
	m_vertex[m_curNum].x = x;
	m_vertex[m_curNum].y = y;
	m_curNum+=3;
}

void Quad::AddPointCurPos(int x, int y)
{
	CalcShapeBoundary(x,y);
	
	m_vertex[m_curNum-2].x = m_vertex[m_curNum-3].x;
	m_vertex[m_curNum-2].y = y;

	m_vertex[m_curNum-1].x = x;
	m_vertex[m_curNum-1].y = y;
	
	m_vertex[m_curNum].x = x;
	m_vertex[m_curNum].y = m_vertex[m_curNum-3].y;

}

void Quad::CalcShapeBoundary(int x, int y)
{
	if(!IsStarted())
	{
		m_shapeBoundary.top = y;
		m_shapeBoundary.bottom = y;
		m_shapeBoundary.left = x;
		m_shapeBoundary.right = x;
	}	
	else
	{
		m_shapeBoundary.top = min(m_vertex[0].y,y);
		m_shapeBoundary.bottom = max(m_vertex[0].y,y);
		m_shapeBoundary.left = min(m_vertex[0].x,x);
		m_shapeBoundary.right = max(m_vertex[0].x,x);
	}
}
