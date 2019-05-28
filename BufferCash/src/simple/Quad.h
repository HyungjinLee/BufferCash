#pragma once
#include "Shape.h"

enum BUFFERSTATE { LOCK, UNLOCK, DELAY, WRITE }; // ������ ���� - �������� �Ҵ� (delay ���� ���۴� freelist�ȿ���)

class Quad : public Shape
{
public:
	Quad();
	~Quad(){};

	void AddPoint(int x, int y);
	void AddPointCurPos(int x, int y);

	virtual void Draw(MouseEvent ev, int x, int y, UINT nFlags);

	virtual void MoveShape(UINT nChar);

	virtual void Display(CDC* pDC, int trigger, int blknum, int state);

	virtual void CalcShapeBoundary(int x, int y);
};