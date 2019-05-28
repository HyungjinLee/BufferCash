#include "stdafx.h"
#include "Application.h"

void Application::run()
{
	srand(time(0)); // �õ� ����
					// �� ��ȣ : (0~100)������ ����, �� �ؽ����̺��� �Ҵ簳���� 3~10�� ����

	int myblocknum; // ������ �� ��ȣ
	vector<int> tmp_buffer_count_per_table;

	for (int i = 0; i < MAXSIZE; i++)
	{
		tmp_buffer_count_per_table.push_back(rand() % 8 + 3); // ���� ������ �������� ����
	}

	for (int i = 0; i < MAXSIZE; i++) // �� ���̺� ���� �߰�
	{
		for (int j = 0; j < tmp_buffer_count_per_table[i]; j++) // �� ���̺� �� ���� ������ŭ
		{
			myblocknum = MAXSIZE * (rand() % (100 / MAXSIZE - 1)) + i; // �� ���̺� ���� ���� ������ �߰�

			node = (Buffer*)malloc(sizeof(Buffer)); // ���� ��ŭ�� ��� �Ҵ�
			node->SetBlknum(myblocknum); // ���� ��ȣ �Ҵ�

			int rndforflist = rand() % 4; // 25%�� Ȯ���� freelist�� ����

			int randstate = rand() % 4; // ���� ���� ����

			int delay = rand() % 3; // ������ ���� ����

		
			switch (randstate)
			{
			case LOCK:
				node->SetState(LOCK);
				break;
			case UNLOCK:
				node->SetState(UNLOCK);
				break;
			case DELAY: // FREELIST���� �����ؾ��ϹǷ� �ؿ��� ����ó��

				break;
			case WRITE:
				node->SetState(WRITE);
				break;
			}

			node->SetXY(j , i);

			if (saveidx[myblocknum] == 1) // ���� �������� ��ĥ ��
			{
				if (j == 0) // Ư���� ��� (���̺� ù��° �����ۿ��� �ǵ����� ���)
				{
					i--;
					j = buffer_count_per_table[i] - 1;
				}
				else // �Ϲ����� ���
					j--;

				continue;
			}
			else
			{
				saveidx[myblocknum] = 1; // �� ��ȣ ǥ��
				
				switch (rndforflist) // free����Ʈ�� ����
				{
				case 2:
					freelist.push_back(myblocknum); // free ����Ʈ�� ����
					
					if (delay == 1) // ������ ����
						node->SetState(DELAY); // ������ ���� ����
					
					break;
				default:
					break;
				}

				AddHashData(node); // ���� ��� �߰�

			}
		}
	}

	return;

}

// �ؽ� ���̺��� �����Ѵ�.
void Application::RenewTable()
{
	for (int i = 0; i < MAXSIZE; i++) // �� ���̺� ���� �߰�
	{
		Buffer *node = hash_queue[i];
		for (int j = 0; j < buffer_count_per_table[i]; j++) // �� ���̺� �� ���� ������ŭ
		{
			node->SetXY(j, i);
			node = node->buffernext;
		}

	}

}

// 5���� �ó����� ����
void Application::scenarios(int casenum)
{
	int myblknum; // ���õ� ������ ����ȣ
	int prevblknum; // ���� ������ ����ȣ
	CString blknum_s;
	Buffer* mybuffer; // ����
	list<int>::iterator iter;
	list<int>tmplist = freelist; // ����Ʈ ���



	switch(casenum)
	{
	case 1: // �ó����� 1
		if (freelist.size() == 0)
			break;

		myblknum = freelist.front(); // �ϳ��� ��Ϲ�ȣ ����	
		blknum_s.Format(_T("%02d"), myblknum);
		mychildview->MessageBox(blknum_s + "�˻�");

		RemoveItemInFreeList(myblknum); // FREELIST���� ���� ����
		mybuffer = FindHashData(myblknum); // �ش� ���� ã�Ƽ�

		if (mybuffer != NULL) // �ش� ���۸� ã����
			mybuffer->SetState(LOCK); // �ش� ���۸� LOCK

		mychildview->Invalidate(); // VIEW ����

		break;
	case 2: // �ó����� 2
		if (freelist.size() == 0) // freelist�� ��� �ִ� ���
			break;
		myblknum = freelist.front(); // freelist�� ù��° ������ ����
		
		if (FindHashData(myblknum)->GetState() == DELAY) // freelist�� ù��° �������� DELAY�� ��� 
			break;

		RemoveItemInFreeList(myblknum); // freelist�� ù��° ���۸� ����
		DeleteHashData(myblknum); // Hash Queue������ ����

		prevblknum = myblknum; // ���� �� ��ȣ

		for (int i = 100; i >= 0; i--)
		{
			if (saveidx[i] != 1) // Hash Queue�� �� ���� ���� ���� ����
			{
				myblknum = i;
				break;
			}
		}

		blknum_s.Format(_T("%02d"), myblknum);
		mychildview->MessageBox(blknum_s + "�˻�");

		mybuffer = new Buffer(); // ���� ��ü �Ҵ�
		mybuffer->SetBlknum(myblknum); // �� ��ȣ ����
		mybuffer->SetState(LOCK); // ���� ���� LOCK���� �ʱ�ȭ
		mybuffer->SetXY(buffer_count_per_table[myblknum % MAXSIZE],myblknum % MAXSIZE ); // �ش��ϴ� ���̺��� ���� ��ġ ����
		AddHashData(mybuffer); // ���� �߰�
		saveidx[myblknum] = 1; // ���� ǥ��
		saveidx[prevblknum] = 0; // ���� ǥ��

		mychildview->Invalidate(); // VIEW ����

		break;

	case 3: // �ó����� 3
		if (freelist.size() == 0) // freelist�� ��� �ִ� ���
			break;
		myblknum = freelist.front(); // freelist�� ù��° ������ ����
		
		if (FindHashData(myblknum)->GetState() != DELAY) // freelist�� ù��° �������� DELAY�� �ƴ� ��� 
			break;
		

		for (int i = 100; i >= 0; i--)
		{
			if (saveidx[i] != 1) // Hash Queue�� �� ���� ���� ���� ����
			{
				prevblknum = i;
				break;
			}
		}

		blknum_s.Format(_T("%02d"), prevblknum);
		mychildview->MessageBox(blknum_s + "�˻�");
		
		for (iter = tmplist.begin(); iter != tmplist.end(); ++iter)
		{
			mybuffer = FindHashData(*iter);
			
			if (mybuffer != NULL)
			{
				if (mybuffer->GetState() == DELAY)  // �� ���� DELAY ���¿� �ִٸ�
				{
					blknum_s.Format(_T("%02d"), mybuffer->Getblknum());
					mychildview->MessageBox(blknum_s + "�� ���� WRITE ���·� �ٲ�");

					mybuffer->SetState(WRITE); // WRITE���·� ����
					RemoveItemInFreeList(mybuffer->Getblknum()); // freelist���� ����
					mychildview->Invalidate(); // VIEW ����
				}
				else // DELAY ���°� �ƴ϶��
				{

					blknum_s.Format(_T("%02d"), mybuffer->Getblknum());
					mychildview->MessageBox(blknum_s + "�� ���� �ؽ�ť���� ����");

					RemoveItemInFreeList(mybuffer->Getblknum()); // freelist�� ���۸� ����
					DeleteHashData(mybuffer->Getblknum()); // Hash Queue������ ����
					mychildview->Invalidate(); // VIEW ����

					mybuffer = new Buffer(); // ���� ��ü �Ҵ�
					mybuffer->SetBlknum(prevblknum); // �� ��ȣ ����
					mybuffer->SetState(LOCK); // ���� ���� LOCK���� �ʱ�ȭ
					mybuffer->SetXY(buffer_count_per_table[prevblknum % MAXSIZE], prevblknum % MAXSIZE); // �ش��ϴ� ���̺��� ���� ��ġ ����

					blknum_s.Format(_T("%02d"), prevblknum);
					mychildview->MessageBox(blknum_s + "�� ���� �ؽ�ť�� �߰�");
					AddHashData(mybuffer); // ���� �߰�
					mychildview->Invalidate(); // VIEW ����
					
					saveidx[myblknum] = 0; // ���� ǥ��
					saveidx[prevblknum] = 1; // ���� ǥ��
					
					return; // while loop ��������

				}

			}
		}

		break;
	case 4: // �ó����� 4

		if (freelist.size() != 0) // freelist�� ��� ���� ������
			break;

		for (int i = 100; i >= 0; i--)
		{
			if (saveidx[i] != 1) // Hash Queue�� �� ���� ���� ���� ����
			{
				prevblknum = i;
				break;
			}
		}

		blknum_s.Format(_T("%02d"), prevblknum);
		mychildview->MessageBox(blknum_s + "�˻�");

		mychildview->MessageBox("SLEEP"); // Event Wakeup Time
		mychildview->Invalidate(); // VIEW ����

		return;
		
	case 5: // �ó����� 5

		if (freelist.size() == 0)
			break;

		for (int i = 0; i < MAXSIZE; i++)
		{
			mybuffer = hash_queue[i];

			while (mybuffer != NULL)
			{

				if (!RetrieveItemInFreeList(mybuffer->Getblknum())) // �� �� ��ȣ�� freelist�� �������� ������
				{
					myblknum = mybuffer->Getblknum();
					blknum_s.Format(_T("%02d"), myblknum);
					mychildview->MessageBox(blknum_s + "�˻�");

					mychildview->MessageBox("SLEEP"); // Event Wakeup Time
					mychildview->Invalidate(); // VIEW ����

					return;
				}

				else
					mybuffer = mybuffer->buffernext; // ���� ���� �˻�
			}
		}

		mychildview->Invalidate(); // VIEW ����
		break;
	}


}