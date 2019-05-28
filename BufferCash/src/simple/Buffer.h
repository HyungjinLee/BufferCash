#pragma once
#include "Quad.h"
#define WIDTH 50
#define HEIGHT 50
#define SPACE 5

class Buffer {

private:
	int blknum; // ������ �� ��ȣ
	int state; // ������ ���� ����
	int m_x; // x��
	int m_y; // y��
	Quad* myshape; // �� ����

public:
	Buffer* buffernext; // ���� ���۸� ����Ű�� ������

	Buffer()
	{
		buffernext = NULL;
		blknum = -1; // �� ��ȣ �ʱ�ȭ
		state = UNLOCK; // �⺻ ���´� UNLOCK
	
	}
	
	// X,Y ��ǥ ����
	void SetXY(int x, int y)
	{
		m_x = x;
		m_y = y;
	}

	int Getblknum() const { // �� ��ȣ ����
		return blknum;
	}
	void SetBlknum(int blknum) { // �� ��ȣ ����
		this->blknum = blknum;
	}
	int GetState() // ���� ���� ����
	{
		return state;
	}
	void SetState(int state) // ���� ���� ����
	{
		this->state = state;
	}

	bool operator==(const Buffer& obj) {
		return blknum == obj.Getblknum();
	}

	void Draw(CDC *dc,int trigger)
	{	
		//�귯�� ���� 
		CBrush MyBrush, *pOldBrush;
		// RGB�� ������ �귯�� ���� 
		MyBrush.CreateSolidBrush(RGB(0, 0, 0));
		// ������ �귯���� dc�� ���� 
		pOldBrush = (CBrush *)dc->SelectObject(&MyBrush);

		myshape = new Quad(); // ���� ����
		myshape->AddPoint(20 + SPACE + WIDTH*m_x, 20 + SPACE +HEIGHT*m_y);
		myshape->AddPointCurPos(20 + SPACE + WIDTH*m_x + WIDTH, 20 + SPACE + HEIGHT*m_y + HEIGHT);
	
		myshape->Display(dc, trigger, blknum, state);
		
		dc->SelectObject(pOldBrush);
		
	}
};