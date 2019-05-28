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
			buffer_count_per_table.push_back(0); // 테이블 당 버퍼 갯수 초기화
		}

		mychildview = child;
	}

	int AddHashData(Buffer* node)  // 해당 블럭을 해쉬 큐에 추가
	{
		int hash_key = node->Getblknum() % MAXSIZE; // hash_key 생성 - modular

		if (hash_queue[hash_key] == NULL)  // 해당 key의 아이템이 비어 있는 경우
		{
			hash_queue[hash_key] = node; // 노드를 추가
			hash_queue[hash_key]->buffernext = NULL; 	
	
		}

		else // 비어 있지 않는 경우
		{

			node->buffernext = hash_queue[hash_key]; // 다음 아이템을 가리키게 한다.
			hash_queue[hash_key] = node; // 첫번째 주소를 추가한 아이템으로 갱신한다.
			
		}
		buffer_count_per_table[hash_key]++; // 버퍼의 갯수 하나 증가
		return 1; // 추가 성공
	}
	
	void DeleteHashData(int blknum) // 해당 블럭을 해시 테이블에서 삭제
	{
		int hash_key = blknum%MAXSIZE;  // hash_key 생성 - modular
		if (hash_queue[hash_key] == NULL) // 해당 key의 아이템이 비어 있는 경우
			return; // 삭제 실패

		Buffer* delNode = NULL; // 지우기 위한 노드 포인터

		if (hash_queue[hash_key]->Getblknum() == blknum) // 블럭 번호가 일치하면
		{
			delNode = hash_queue[hash_key]; // 현재 아이템을 delNode가 가르키게 함
			hash_queue[hash_key] = hash_queue[hash_key]->buffernext; // 첫번 째 주소를 다음 아이템의 주소로 갱신한다.
		}
		else
		{
			Buffer* node = hash_queue[hash_key]; // 첫번째 아이템을 가리킨다.
			Buffer* next = node->buffernext; // 다음 아이템을 가리킨다. 
			while (next) // 다음 노드가 null이 아닐 동안
			{
				if (next->Getblknum() == blknum) // 다음 아이템의 블럭 번호가 일치하면
				{
					node->buffernext = next->buffernext; // 다음 노드의 주소를 첫번째 주소로 설정
					delNode = next; // 그 노드를 delNode로 설정
					break;
				}
				// 블럭 번호가 일치하지 않는 경우
				node = next;  // 현재 노드를 저장
				next = node->buffernext; // 다음 노드로 이동
			}
		}
		free(delNode); // 해당 버퍼를 삭제한다.
		buffer_count_per_table[hash_key] -= 1; // 버퍼의 갯수 하나 삭제
	}

	Buffer* FindHashData(int blknum)
	{
		int hash_key = blknum%MAXSIZE;  // hash_key 생성 - modular
		if (hash_queue[hash_key] == NULL) // 해당 key의 아이템이 비어 있는 경우
			return NULL; // 비어있음

		if (hash_queue[hash_key]->Getblknum() == blknum) // 블럭 번호가 일치하는 경우
		{
			return hash_queue[hash_key]; // 찾음
		}

		else // 블럭 번호가 일치하지 않는 경우
		{
			Buffer* tmpnode = hash_queue[hash_key]; // 첫번째 아이템을 가리킴
			while ((tmpnode->buffernext) != NULL) // 다음 노드로 이동 (null이 아닐 동안)
			{
				if (tmpnode->buffernext->Getblknum() == blknum) // 블럭 번호가 일치하는 경우
				{
					return tmpnode->buffernext; // 찾음
				}
				tmpnode = tmpnode->buffernext; // 다음 노드를 가리킴
			}
		}
		return  NULL; // 찾지 못한 경우
	}

	int RemoveItemInFreeList(int blknum) // 해당 블럭 번호 아이템 freelist에서 삭제
	{
		list<int>::iterator iter;

		for (iter = freelist.begin(); iter != freelist.end(); ++iter)
		{
			if (*iter == blknum) // 일치하는 블럭 번호를 발견 시
			{
				freelist.remove(*iter); // 해당 아이템 삭제한다.
				return 1; // 삭제 성공
			}
		}

		return 0; // 삭제 실패
	}

	int RetrieveItemInFreeList(int blknum) // 해당 블럭 번호 아이템 freelist에서 찾음
	{
		list<int>::iterator iter;

		for (iter = freelist.begin(); iter != freelist.end(); ++iter)
		{
			if (*iter == blknum) // 일치하는 블럭 번호를 발견 시
			{
				return 1; // 성공
			}
		}

		return 0; // 실패
	}

	void run(); // 테스트 세팅
	void scenarios(int casenum); // 5가지 시나리오 실행 
	
	void DrawBuffers(CDC* dc) // 버퍼를 시각화한다
	{
		for (int i = 0; i < MAXSIZE; i++) // 각 테이블 별로 추가
		{
			Buffer* tmpBuffer = hash_queue[i];

			while (tmpBuffer != NULL)
			{
				if (RetrieveItemInFreeList(tmpBuffer->Getblknum())) // freeList에서 찾으면
					tmpBuffer->Draw(dc, 1);
				else
					tmpBuffer->Draw(dc, 0);

				tmpBuffer = tmpBuffer->buffernext;
			}

		}

	}
	
	// 테이블 갱신
	void RenewTable();

private:

	Buffer* hash_queue[MAXSIZE];// 블럭을 포함하는 hash queue headers
	list <int> freelist; // 블럭 관리를 위한 freelist header
	vector<int> buffer_count_per_table; // 테이블 당 버퍼의 갯수 3~10개
	Buffer* node; // 버퍼
	CChildView* mychildview;
	int saveidx[101] = { 0, }; // 버퍼 번호를 저장

};