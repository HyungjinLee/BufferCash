#pragma once
#include "Quad.h"
#define WIDTH 50
#define HEIGHT 50
#define SPACE 5

class Buffer {

private:
	int blknum; // 버퍼의 블럭 번호
	int state; // 버퍼의 현재 상태
	int m_x; // x축
	int m_y; // y축
	Quad* myshape; // 내 도형

public:
	Buffer* buffernext; // 다음 버퍼를 가리키는 포인터

	Buffer()
	{
		buffernext = NULL;
		blknum = -1; // 블럭 번호 초기화
		state = UNLOCK; // 기본 상태는 UNLOCK
	
	}
	
	// X,Y 좌표 설정
	void SetXY(int x, int y)
	{
		m_x = x;
		m_y = y;
	}

	int Getblknum() const { // 블럭 번호 리턴
		return blknum;
	}
	void SetBlknum(int blknum) { // 블럭 번호 설정
		this->blknum = blknum;
	}
	int GetState() // 버퍼 상태 리턴
	{
		return state;
	}
	void SetState(int state) // 버퍼 상태 설정
	{
		this->state = state;
	}

	bool operator==(const Buffer& obj) {
		return blknum == obj.Getblknum();
	}

	void Draw(CDC *dc,int trigger)
	{	
		//브러쉬 생성 
		CBrush MyBrush, *pOldBrush;
		// RGB로 검은색 브러쉬 생성 
		MyBrush.CreateSolidBrush(RGB(0, 0, 0));
		// 생성한 브러쉬를 dc에 선택 
		pOldBrush = (CBrush *)dc->SelectObject(&MyBrush);

		myshape = new Quad(); // 도형 생성
		myshape->AddPoint(20 + SPACE + WIDTH*m_x, 20 + SPACE +HEIGHT*m_y);
		myshape->AddPointCurPos(20 + SPACE + WIDTH*m_x + WIDTH, 20 + SPACE + HEIGHT*m_y + HEIGHT);
	
		myshape->Display(dc, trigger, blknum, state);
		
		dc->SelectObject(pOldBrush);
		
	}
};