#pragma once
#define MAXSIZE 8
#include <list>
#include <ctime>
#include "Buffer.h"
#include <vector>
#include "stdafx.h"
#include "ChildView.h"

using namespace std;

class CChildView;

class Application
{

public:
	Application(CChildView* child)
	{
		for (int i = 0; i<MAXSIZE; i++)
			hash_queue[i] = NULL;	

		for (int i = 0; i < MAXSIZE; i++)
		{
			buffer_count_per_table.push_back(0); // ���̺� �� ���� ���� �ʱ�ȭ
		}

		mychildview = child;
	}

	int AddHashData(Buffer* node)  // �ش� ���� �ؽ� ť�� �߰�
	{
		int hash_key = node->Getblknum() % MAXSIZE; // hash_key ���� - modular

		if (hash_queue[hash_key] == NULL)  // �ش� key�� �������� ��� �ִ� ���
		{
			hash_queue[hash_key] = node; // ��带 �߰�
			hash_queue[hash_key]->buffernext = NULL; 	
	
		}

		else // ��� ���� �ʴ� ���
		{

			node->buffernext = hash_queue[hash_key]; // ���� �������� ����Ű�� �Ѵ�.
			hash_queue[hash_key] = node; // ù��° �ּҸ� �߰��� ���������� �����Ѵ�.
			
		}
		buffer_count_per_table[hash_key]++; // ������ ���� �ϳ� ����
		return 1; // �߰� ����
	}
	
	void DeleteHashData(int blknum) // �ش� ���� �ؽ� ���̺��� ����
	{
		int hash_key = blknum%MAXSIZE;  // hash_key ���� - modular
		if (hash_queue[hash_key] == NULL) // �ش� key�� �������� ��� �ִ� ���
			return; // ���� ����

		Buffer* delNode = NULL; // ����� ���� ��� ������

		if (hash_queue[hash_key]->Getblknum() == blknum) // �� ��ȣ�� ��ġ�ϸ�
		{
			delNode = hash_queue[hash_key]; // ���� �������� delNode�� ����Ű�� ��
			hash_queue[hash_key] = hash_queue[hash_key]->buffernext; // ù�� ° �ּҸ� ���� �������� �ּҷ� �����Ѵ�.
		}
		else
		{
			Buffer* node = hash_queue[hash_key]; // ù��° �������� ����Ų��.
			Buffer* next = node->buffernext; // ���� �������� ����Ų��. 
			while (next) // ���� ��尡 null�� �ƴ� ����
			{
				if (next->Getblknum() == blknum) // ���� �������� �� ��ȣ�� ��ġ�ϸ�
				{
					node->buffernext = next->buffernext; // ���� ����� �ּҸ� ù��° �ּҷ� ����
					delNode = next; // �� ��带 delNode�� ����
					break;
				}
				// �� ��ȣ�� ��ġ���� �ʴ� ���
				node = next;  // ���� ��带 ����
				next = node->buffernext; // ���� ���� �̵�
			}
		}
		free(delNode); // �ش� ���۸� �����Ѵ�.
		buffer_count_per_table[hash_key] -= 1; // ������ ���� �ϳ� ����
	}

	Buffer* FindHashData(int blknum)
	{
		int hash_key = blknum%MAXSIZE;  // hash_key ���� - modular
		if (hash_queue[hash_key] == NULL) // �ش� key�� �������� ��� �ִ� ���
			return NULL; // �������

		if (hash_queue[hash_key]->Getblknum() == blknum) // �� ��ȣ�� ��ġ�ϴ� ���
		{
			return hash_queue[hash_key]; // ã��
		}

		else // �� ��ȣ�� ��ġ���� �ʴ� ���
		{
			Buffer* tmpnode = hash_queue[hash_key]; // ù��° �������� ����Ŵ
			while ((tmpnode->buffernext) != NULL) // ���� ���� �̵� (null�� �ƴ� ����)
			{
				if (tmpnode->buffernext->Getblknum() == blknum) // �� ��ȣ�� ��ġ�ϴ� ���
				{
					return tmpnode->buffernext; // ã��
				}
				tmpnode = tmpnode->buffernext; // ���� ��带 ����Ŵ
			}
		}
		return  NULL; // ã�� ���� ���
	}

	int RemoveItemInFreeList(int blknum) // �ش� �� ��ȣ ������ freelist���� ����
	{
		list<int>::iterator iter;

		for (iter = freelist.begin(); iter != freelist.end(); ++iter)
		{
			if (*iter == blknum) // ��ġ�ϴ� �� ��ȣ�� �߰� ��
			{
				freelist.remove(*iter); // �ش� ������ �����Ѵ�.
				return 1; // ���� ����
			}
		}

		return 0; // ���� ����
	}

	int RetrieveItemInFreeList(int blknum) // �ش� �� ��ȣ ������ freelist���� ã��
	{
		list<int>::iterator iter;

		for (iter = freelist.begin(); iter != freelist.end(); ++iter)
		{
			if (*iter == blknum) // ��ġ�ϴ� �� ��ȣ�� �߰� ��
			{
				return 1; // ����
			}
		}

		return 0; // ����
	}

	void run(); // �׽�Ʈ ����
	void scenarios(int casenum); // 5���� �ó����� ���� 
	
	void DrawBuffers(CDC* dc) // ���۸� �ð�ȭ�Ѵ�
	{
		for (int i = 0; i < MAXSIZE; i++) // �� ���̺� ���� �߰�
		{
			Buffer* tmpBuffer = hash_queue[i];

			while (tmpBuffer != NULL)
			{
				if (RetrieveItemInFreeList(tmpBuffer->Getblknum())) // freeList���� ã����
					tmpBuffer->Draw(dc, 1);
				else
					tmpBuffer->Draw(dc, 0);

				tmpBuffer = tmpBuffer->buffernext;
			}

		}

	}
	
	// ���̺� ����
	void RenewTable();

private:

	Buffer* hash_queue[MAXSIZE];// ���� �����ϴ� hash queue headers
	list <int> freelist; // �� ������ ���� freelist header
	vector<int> buffer_count_per_table; // ���̺� �� ������ ���� 3~10��
	Buffer* node; // ����
	CChildView* mychildview;
	int saveidx[101] = { 0, }; // ���� ��ȣ�� ����

};