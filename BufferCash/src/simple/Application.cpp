#include "stdafx.h"
#include "Application.h"

void Application::run()
{
	srand(time(0)); // 시드 설정
					// 블럭 번호 : (0~100)까지의 숫자, 각 해쉬테이블의 할당개수는 3~10개 사이

	int myblocknum; // 생성된 블럭 번호
	vector<int> tmp_buffer_count_per_table;

	for (int i = 0; i < MAXSIZE; i++)
	{
		tmp_buffer_count_per_table.push_back(rand() % 8 + 3); // 버퍼 갯수를 랜덤으로 생성
	}

	for (int i = 0; i < MAXSIZE; i++) // 각 테이블 별로 추가
	{
		for (int j = 0; j < tmp_buffer_count_per_table[i]; j++) // 각 테이블 당 버퍼 갯수만큼
		{
			myblocknum = MAXSIZE * (rand() % (100 / MAXSIZE - 1)) + i; // 각 테이블 별로 버퍼 아이템 추가

			node = (Buffer*)malloc(sizeof(Buffer)); // 버퍼 만큼의 노드 할당
			node->SetBlknum(myblocknum); // 버퍼 번호 할당

			int rndforflist = rand() % 4; // 25%의 확률로 freelist에 연결

			int randstate = rand() % 4; // 랜덤 상태 생성

			int delay = rand() % 3; // 딜레이 상태 생성

		
			switch (randstate)
			{
			case LOCK:
				node->SetState(LOCK);
				break;
			case UNLOCK:
				node->SetState(UNLOCK);
				break;
			case DELAY: // FREELIST에만 존재해야하므로 밑에서 예외처리

				break;
			case WRITE:
				node->SetState(WRITE);
				break;
			}

			node->SetXY(j , i);

			if (saveidx[myblocknum] == 1) // 버퍼 아이템이 겹칠 때
			{
				if (j == 0) // 특수한 경우 (테이블 첫번째 아이템에서 되돌리는 경우)
				{
					i--;
					j = buffer_count_per_table[i] - 1;
				}
				else // 일반적인 경우
					j--;

				continue;
			}
			else
			{
				saveidx[myblocknum] = 1; // 블럭 번호 표시
				
				switch (rndforflist) // free리스트에 연결
				{
				case 2:
					freelist.push_back(myblocknum); // free 리스트에 연결
					
					if (delay == 1) // 딜레이 상태
						node->SetState(DELAY); // 딜레이 상태 설정
					
					break;
				default:
					break;
				}

				AddHashData(node); // 버퍼 노드 추가

			}
		}
	}

	return;

}

// 해시 테이블을 갱신한다.
void Application::RenewTable()
{
	for (int i = 0; i < MAXSIZE; i++) // 각 테이블 별로 추가
	{
		Buffer *node = hash_queue[i];
		for (int j = 0; j < buffer_count_per_table[i]; j++) // 각 테이블 당 버퍼 갯수만큼
		{
			node->SetXY(j, i);
			node = node->buffernext;
		}

	}

}

// 5가지 시나리오 실행
void Application::scenarios(int casenum)
{
	int myblknum; // 선택된 버퍼의 블럭번호
	int prevblknum; // 이전 버퍼의 블럭번호
	CString blknum_s;
	Buffer* mybuffer; // 버퍼
	list<int>::iterator iter;
	list<int>tmplist = freelist; // 리스트 백업



	switch(casenum)
	{
	case 1: // 시나리오 1
		if (freelist.size() == 0)
			break;

		myblknum = freelist.front(); // 하나의 블록번호 선택	
		blknum_s.Format(_T("%02d"), myblknum);
		mychildview->MessageBox(blknum_s + "검색");

		RemoveItemInFreeList(myblknum); // FREELIST에서 버퍼 제거
		mybuffer = FindHashData(myblknum); // 해당 버퍼 찾아서

		if (mybuffer != NULL) // 해당 버퍼를 찾으면
			mybuffer->SetState(LOCK); // 해당 버퍼를 LOCK

		mychildview->Invalidate(); // VIEW 갱신

		break;
	case 2: // 시나리오 2
		if (freelist.size() == 0) // freelist가 비어 있는 경우
			break;
		myblknum = freelist.front(); // freelist의 첫번째 아이템 선택
		
		if (FindHashData(myblknum)->GetState() == DELAY) // freelist의 첫번째 아이템이 DELAY인 경우 
			break;

		RemoveItemInFreeList(myblknum); // freelist의 첫번째 버퍼를 제거
		DeleteHashData(myblknum); // Hash Queue에서도 제거

		prevblknum = myblknum; // 이전 블럭 번호

		for (int i = 100; i >= 0; i--)
		{
			if (saveidx[i] != 1) // Hash Queue에 들어가 있지 않은 블럭을 선택
			{
				myblknum = i;
				break;
			}
		}

		blknum_s.Format(_T("%02d"), myblknum);
		mychildview->MessageBox(blknum_s + "검색");

		mybuffer = new Buffer(); // 버퍼 객체 할당
		mybuffer->SetBlknum(myblknum); // 블럭 번호 지정
		mybuffer->SetState(LOCK); // 버퍼 상태 LOCK으로 초기화
		mybuffer->SetXY(buffer_count_per_table[myblknum % MAXSIZE],myblknum % MAXSIZE ); // 해당하는 테이블의 버퍼 위치 설정
		AddHashData(mybuffer); // 버퍼 추가
		saveidx[myblknum] = 1; // 버퍼 표시
		saveidx[prevblknum] = 0; // 버퍼 표시

		mychildview->Invalidate(); // VIEW 갱신

		break;

	case 3: // 시나리오 3
		if (freelist.size() == 0) // freelist가 비어 있는 경우
			break;
		myblknum = freelist.front(); // freelist의 첫번째 아이템 선택
		
		if (FindHashData(myblknum)->GetState() != DELAY) // freelist의 첫번째 아이템이 DELAY가 아닌 경우 
			break;
		

		for (int i = 100; i >= 0; i--)
		{
			if (saveidx[i] != 1) // Hash Queue에 들어가 있지 않은 블럭을 선택
			{
				prevblknum = i;
				break;
			}
		}

		blknum_s.Format(_T("%02d"), prevblknum);
		mychildview->MessageBox(blknum_s + "검색");
		
		for (iter = tmplist.begin(); iter != tmplist.end(); ++iter)
		{
			mybuffer = FindHashData(*iter);
			
			if (mybuffer != NULL)
			{
				if (mybuffer->GetState() == DELAY)  // 이 블럭이 DELAY 상태에 있다면
				{
					blknum_s.Format(_T("%02d"), mybuffer->Getblknum());
					mychildview->MessageBox(blknum_s + "번 버퍼 WRITE 상태로 바꿈");

					mybuffer->SetState(WRITE); // WRITE상태로 갱신
					RemoveItemInFreeList(mybuffer->Getblknum()); // freelist에서 삭제
					mychildview->Invalidate(); // VIEW 갱신
				}
				else // DELAY 상태가 아니라면
				{

					blknum_s.Format(_T("%02d"), mybuffer->Getblknum());
					mychildview->MessageBox(blknum_s + "번 버퍼 해시큐에서 삭제");

					RemoveItemInFreeList(mybuffer->Getblknum()); // freelist의 버퍼를 제거
					DeleteHashData(mybuffer->Getblknum()); // Hash Queue에서도 제거
					mychildview->Invalidate(); // VIEW 갱신

					mybuffer = new Buffer(); // 버퍼 객체 할당
					mybuffer->SetBlknum(prevblknum); // 블럭 번호 지정
					mybuffer->SetState(LOCK); // 버퍼 상태 LOCK으로 초기화
					mybuffer->SetXY(buffer_count_per_table[prevblknum % MAXSIZE], prevblknum % MAXSIZE); // 해당하는 테이블의 버퍼 위치 설정

					blknum_s.Format(_T("%02d"), prevblknum);
					mychildview->MessageBox(blknum_s + "번 버퍼 해시큐에 추가");
					AddHashData(mybuffer); // 버퍼 추가
					mychildview->Invalidate(); // VIEW 갱신
					
					saveidx[myblknum] = 0; // 버퍼 표시
					saveidx[prevblknum] = 1; // 버퍼 표시
					
					return; // while loop 빠져나감

				}

			}
		}

		break;
	case 4: // 시나리오 4

		if (freelist.size() != 0) // freelist가 비어 있지 않으면
			break;

		for (int i = 100; i >= 0; i--)
		{
			if (saveidx[i] != 1) // Hash Queue에 들어가 있지 않은 블럭을 선택
			{
				prevblknum = i;
				break;
			}
		}

		blknum_s.Format(_T("%02d"), prevblknum);
		mychildview->MessageBox(blknum_s + "검색");

		mychildview->MessageBox("SLEEP"); // Event Wakeup Time
		mychildview->Invalidate(); // VIEW 갱신

		return;
		
	case 5: // 시나리오 5

		if (freelist.size() == 0)
			break;

		for (int i = 0; i < MAXSIZE; i++)
		{
			mybuffer = hash_queue[i];

			while (mybuffer != NULL)
			{

				if (!RetrieveItemInFreeList(mybuffer->Getblknum())) // 이 블럭 번호가 freelist에 존재하지 않으면
				{
					myblknum = mybuffer->Getblknum();
					blknum_s.Format(_T("%02d"), myblknum);
					mychildview->MessageBox(blknum_s + "검색");

					mychildview->MessageBox("SLEEP"); // Event Wakeup Time
					mychildview->Invalidate(); // VIEW 갱신

					return;
				}

				else
					mybuffer = mybuffer->buffernext; // 다음 버퍼 검사
			}
		}

		mychildview->Invalidate(); // VIEW 갱신
		break;
	}


}