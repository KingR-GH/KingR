#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<iostream>
#include<time.h>
#include<math.h>
#define maxgen 50  // ����������
#define sizepop 15 // ��Ⱥ��Ŀ
#define pcross 0.6 // �������
#define pmutation 0.2 // �������
#define lenchrom 8 // Ⱦɫ�峤��
using namespace std;
typedef int Time;
typedef int CNCSituation;
typedef int RGVSituation;
typedef int PartSituation;
const int NotWork = 0;
const int Working = 1;
const int Broken = -1;
const int Moving = 0;
const int Loading = 1;
const int Cleaning = 2;
const int Waiting = -1;
const int Raw = 0;
const int Process1 = 1;
const int Complete = -1;
const int Process2 = 2;
int BreakDown = -1;																//����ϵ��Ĭ��Ϊ-1
int ProcessMethod = 1;															//������Ĭ��Ϊ1;
int talk = 0;

class Part {
private:
	int No;
	int Position;
	PartSituation partSituation;
	Time UploadTime1,UploadTime2;
	Time DownloadTime1,DownloadTime2;
	int ProcessingCNC1, ProcessingCNC2;
public:
	Part() {
		partSituation = Raw;
	}
	Part(int no) {
		partSituation = Raw;
		No = no;
	}


	void setNo(int no) {
		No = no;
	}
	int getNo() {
		return No;
	}
	void setPosition(int pos) {
		Position = pos;
	}
	int getPosition() {
		return Position;
	}
	//��ȡ���״̬
	PartSituation getPartSituation() {
		return partSituation;
	}
	//�������״̬
	void setPartSituation(PartSituation p) {
		partSituation = p;
	}
	
	//��������ʱ��
	void setUploadTime1(Time uploadtime) {
		UploadTime1 = uploadtime;
	}	
	void setUploadTime2(Time uploadtime) {
		UploadTime2 = uploadtime;
	}
	//��ȡ����ʱ��
	int getUploadTime1() {
		return UploadTime1;
	}
	int getUploadTime2() {
		return UploadTime2;
	}
	//��������ʱ��
	void setDownloadTime1(Time downloadtime) {
		DownloadTime1 = downloadtime;
	}
	void setDownloadTime2(Time downloadtime) {
		DownloadTime2 = downloadtime;
	}
	//��ȡ����ʱ��
	int getDownloadTime1() {
		return DownloadTime1;
	}
	int getDownloadTime2() {
		return DownloadTime2;
	}
	//���ø�������CNC
	void setProcessCNC1(int no) {
		ProcessingCNC1 = no;
	}
	void setProcessCNC2(int no) {
		ProcessingCNC2 = no;
	}
	//��ȡ��������CNC
	int getProcessCNC1() {
		return ProcessingCNC1;
	}
	int getProcessCNC2() {
		return ProcessingCNC2;
	}

	void setInfo(int no,int pos) {
		setNo(no);
		setPosition(pos);
	}

};

class CNC {
private:
	int No;
	Time T1;							//һ������ӹ��������ʱ�䣻
	Time T21, T22;						//��������ӹ�����ֱ�����ʱ��;
	Time StartWorkTime;					//��ʼ������ʱ��;
	Time EndWorkTime;					//����������ʱ��;
	Time StartBrokenTime;				//��ʼ���ϵ�ʱ��;
	Time EndBrokenTime;					//�������ϵ�ʱ��;
	CNCSituation workSituation;			//����״̬,��Ϊ����: NotWork �� Working �� Broken;
public:
	Part * part;
	//CNC���캯��,��ʼ������״̬,��������ʱ��;
	CNC() {
		workSituation = NotWork;
	}
	CNC(int number,Time workTime1,Time workTime21,Time workTime22) {
		No = number;
		T1 = workTime1;
		T21 = workTime21;
		T22 = workTime22;
		workSituation = NotWork;
		part = NULL;
	}
	//��ʼ����,�趨��ʼ����ʱ��ͽ�������ʱ��;
	void StartWork(Time &MainTime) {
		//����ӹ�ʱ�䳤��
		int processingTime;
		if (ProcessMethod == 1) {
			processingTime = T1;
		}
		else {
			if (part->getPartSituation() == Raw) {
				processingTime = T21;
			}
			else {
				processingTime = T22;
			}
		}

		workSituation = Working;
		StartWorkTime = MainTime;
		EndWorkTime = MainTime + processingTime;
	}
	//��������;
	void EndWork() {
		workSituation = NotWork;
		StartWorkTime = -1;
		EndWorkTime = -1;
		StartBrokenTime = -1;
		EndBrokenTime = -1;
	}
	//��ȡCNC״̬;
	CNCSituation getSituation() {
		return workSituation;
	}
	//��ȡ��������ʱ��;
	Time getEndWorkTime() {
		return EndWorkTime;
	}
	//��ȡ�������
	int getNo() {
		return No;
	}
	//��������;
	void CNCBreakDown(Time MainTime) {
		workSituation = Broken;
		//����ӹ�ʱ�䳤��
		int processingTime;
		if (ProcessMethod == 1) {
			processingTime = T1;
		}
		else {
			if (part->getPartSituation() == Raw) {
				processingTime = T21;
			}
			else {
				processingTime = T22;
			}
		}
		StartBrokenTime = rand() % (processingTime+1)+MainTime;
		EndBrokenTime = StartBrokenTime + rand() % (1200 - 600 + 1) + 600;

	}
	//��ȡ���Ͽ�ʼʱ��
	Time getStartBrokenTime() {
		return StartBrokenTime;
	}
	//��ȡ��������ʱ��
	Time getEndBrokenTime() {
		return EndBrokenTime;
	}

	void debug() {
		cout << "CNC��Ϣ:" << endl;
		cout << No << " " << T1 << " " << T21 << " " << T22 << " " << workSituation << endl;;
	}
};

class RGV {
private:
	int Position;					//RGV����λ��
	int Destination;
	Time M0, M1, M2, M3;			//RGV�ƶ�0����λ,1����λ,2����λ,3����λ����ʱ��
	Time Odd;						//RGVΪ��������������ʱ��
	Time Even;						//RGVΪż������������ʱ��
	Time load;						//RGV������ʱ��
	Time C;							//RGV��ϴ��ҵʱ��
	Time situationStart;
	Time situationEnd;
	RGVSituation situation;			//RGV��ǰ״̬


public:
	Part * part;

	RGV() {
		situation = Waiting;
	}
	RGV(Time m0,Time m1,Time m2,Time m3,Time odd,Time even,Time clean) {
		M0 = m0;
		M1 = m1;
		M2 = m2;
		M3 = m3;
		Odd = odd;
		Even = even;
		C = clean;
		situation = Waiting;
	}
	RGVSituation getRGVSituation() {
		return situation;
	}

	void Wait() {
		situation = Waiting;
	}
	void Move(Time MainTime, int PositionStart, int PositionEnd) {
		//debug
		//cout << "Move() started"<<endl;

		int M = calMoveTime(PositionStart,PositionEnd);
		Destination = PositionEnd;
		situation = Moving;
		situationStart = MainTime;
		situationEnd = MainTime + M;

		//debug
		//cout << "Move() ended" << endl;
	}
	void Load(Time &MainTime) {
		if (Position % 2 == 1) {
			load = Odd;
		}
		else {
			load = Even;
		}
		situation = Loading;
		situationStart = MainTime;
		situationEnd = MainTime + load;
	}
	void Clean(Time &MainTime) {
		situation = Cleaning;
		situationStart = MainTime;
		situationEnd = MainTime + C;
	}
	void SetPosition(int pos) {
		Position = pos;
		if (Position % 2 == 1) {
			load = Odd;
		}
		else {
			load = Even;
		}
	}
	int getPosition() {
		return Position;
	}
	int getDestination() {
		return Destination;
	}
	Time getSituationEnd() {
		return situationEnd;
	}

	int getLoad() {
		return load;
	}

	//���㲢����RGV������λ�ü���ƶ�ʱ��
	int calMoveTime(int pos1, int pos2) {
		int res = 0;
		int p1 = (pos1 + 1) / 2;
		int p2 = (pos2 + 1) / 2;
		int ab = p2 - p1;
		if ((p2 - p1) < 0) {
			ab = p1 - p2;
		}
		switch (ab)
		{
		case 0:
			res = M0;
			break;
		case 1:
			res = M1;
			break;
		case 2:
			res = M2;
			break;
		case 3:
			res = M3;
			break;
		}
		return res;
	}

	void debug() {
		cout << "RGV��Ϣ:" << endl;
		cout << Position << " " << M0 << " " << M1 << " " << M2 << " " << M3 << " " << Odd << " " << Even << " " << C << " " << situation << endl;
	}
};


Time MainTime;
Time TotalTime;
CNC cnc[8];
RGV rgv;

double Adaptability(int WorkSeq[8]);				//��Ӧ�Ⱥ���
void InputInfo();//����CNC��RGV������Ϣ

void InputInfo() {
	Time M0, M1, M2, M3;
	Time T1, T21, T22;
	Time Odd, Even;
	Time Clean;

	cout << "����ϵͳ������ʱ��:" << endl;
	cin >> TotalTime;

	cout << "�������һ��������蹤����:"<<endl;
	cin >> ProcessMethod;

	cout << "����Y��N�����Ƿ��ǹ���״̬:"<<endl;
	char flag;
	cin >> flag;
	if (flag == 'Y' ||flag == 'Y'-'A'+'a') {
		BreakDown = 50;
	}
	else {
		BreakDown = -1;
	}

	M0 = 0;
	cout << "����RGV�ƶ�1����λ����ʱ��:" << endl;
	cin >> M1;
	cout << "����RGV�ƶ�2����λ����ʱ��:" << endl;
	cin >> M2;
	cout << "����RGV�ƶ�3����λ����ʱ��:" << endl;
	cin >> M3;
	cout << "����CNC�ӹ����һ��һ���������������ʱ��:" << endl;
	cin >> T1;
	cout << "����CNC�ӹ����һ��������������ϵĵ�һ����������ʱ��:" << endl;
	cin >> T21;
	cout << "����CNC�ӹ����һ��������������ϵĵڶ�����������ʱ��:" << endl;
	cin >> T22;
	cout << "����RGVΪCNC1#��3#��5#��7#һ������������ʱ��:" << endl;
	cin >> Odd;
	cout << "����RGVΪCNC2#��4#��6#��8#һ������������ʱ��:" << endl;
	cin >> Even;
	cout << "����RGV���һ�����ϵ���ϴ��ҵ����ʱ��:" << endl;
	cin >> Clean;
	CNC *cp;
	for (int i = 1; i <= 8; i++) {
		cp = new CNC(i,T1,T21,T22);
		cnc[i - 1] = *cp;
		//cnc[i - 1].debug();
	}
	RGV *rp;
	rp = new RGV(M0,M1,M2,M3,Odd,Even,Clean);
	rgv = *rp;
	//rgv.debug();
}

double Adaptability(int WorkSeq[8]) {
	srand((int)time(0));

	//��ʼ������ϵͳ
	MainTime = 0;
	for (int i = 0; i < 8; i++) {
		cnc[i].EndWork();
		cnc[i].part = NULL;
	}
	rgv.part = NULL;
	rgv.Wait();

	int partNumber = 0;
	int partComplete = 0; //��ɵĹ�����
	Part part[10000];
	rgv.SetPosition(WorkSeq[0]);
	for (MainTime = 0; MainTime <= TotalTime; MainTime++) {
		if (rgv.getRGVSituation() == Waiting) {
			//debug
			//cout << "RGV�ȴ�: " << MainTime << endl;
			for (int i = 0; i < 8; i++) {
				if (cnc[WorkSeq[i] - 1].getSituation() == NotWork) {
					//rgv.Move(MainTime, rgv.getPosition(), cnc[WorkSeq[i - 1] - 1].getNo());
					rgv.Move(MainTime, rgv.getPosition(), WorkSeq[i]);

					//debug
					//cout << "RGV�ƶ�ʱ��: " << MainTime <<" ���: "<<rgv.getPosition()<<" �յ�: "<<cnc[WorkSeq[i]-1].getNo()<< endl;
					MainTime--;//����Ҫע���������!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					break;
				}
			}
		}
		else if (rgv.getRGVSituation() == Moving) {
			//debug
			//cout << "RGV�ƶ�: " << MainTime << endl;
			if (MainTime == rgv.getSituationEnd()) {

				rgv.SetPosition(rgv.getDestination());
				if (rgv.part == NULL) {
					part[partNumber].setInfo(partNumber + 1, rgv.getPosition());
					part[partNumber].setUploadTime1(MainTime);
					part[partNumber].setProcessCNC1(rgv.getPosition());
					rgv.part = &part[partNumber];
					partNumber++;
				}
				else {
					rgv.part->setUploadTime2(MainTime);
					rgv.part->setProcessCNC2(rgv.getPosition());
				}

				rgv.Load(MainTime);

			}
		}
		else if (rgv.getRGVSituation() == Loading) {
			//debug
			//cout << "RGV������: " << MainTime << endl;
			if (MainTime == rgv.getSituationEnd()) {
				if (cnc[rgv.getPosition() - 1].part == NULL) {
					cnc[rgv.getPosition() - 1].part = rgv.part;
					rgv.part = NULL;
					rgv.Wait();
					MainTime--;//����Ҫע���������!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				}//CNC�������
				else {
					Part *temp;
					temp = rgv.part;
					rgv.part = cnc[rgv.getPosition() - 1].part;
					cnc[rgv.getPosition() - 1].part = temp;

					if (rgv.part->getPartSituation() == Process1) {
						rgv.part->setDownloadTime1(MainTime - rgv.getLoad());
						if (ProcessMethod == 1) {
							rgv.Clean(MainTime);
						}
						else {
							rgv.Wait();
							MainTime--;//����Ҫע���������!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
						}
					}
					else {
						rgv.part->setDownloadTime2(MainTime - rgv.getLoad());
						rgv.Clean(MainTime);
					}

				}//CNC�������

				int CNCBreakDownElem = rand() % 100;
				if (CNCBreakDownElem == BreakDown) {
					//cout << rgv.getPosition() <<" �Ż���"<< "�ӹ� " << cnc[rgv.getPosition() - 1].part->getNo() << " �Ź���ʱ���ֹ���,";

					cnc[rgv.getPosition() - 1].CNCBreakDown(MainTime);

					//cout << "����ʱ�俪ʼʱ��Ϊ "<<cnc[rgv.getPosition()-1].getStartBrokenTime() << " ,���Ͻ���ʱ��Ϊ "<<cnc[rgv.getPosition()-1].getEndBrokenTime()<<endl;
				}
				else {
					cnc[rgv.getPosition() - 1].StartWork(MainTime);
				}


			}
		}
		else if (rgv.getRGVSituation() == Cleaning) {
			//debug
			//cout << "RGV��ϴ: " << MainTime << endl;
			if (MainTime == rgv.getSituationEnd()) {
				partComplete++;
				if (talk == 1) {
					if (ProcessMethod == 1) {
						cout << "�� " << MainTime << " ��";
						cout << " ������: " << rgv.part->getNo() << " ������: " << partComplete << " ��CNC " << rgv.part->getProcessCNC1() << " #��ɹ���" << ",���Ͽ�ʼʱ��: " << rgv.part->getUploadTime1() << " ,���Ͽ�ʼʱ��:" << rgv.part->getDownloadTime1() << endl;
					}
					else {
						cout << "�� " << MainTime << " ��";
						cout << " ������: " << rgv.part->getNo() << " ������: " << partComplete << " ��CNC " << rgv.part->getProcessCNC1() << " #��ɵ�һ������" << ",���Ͽ�ʼʱ��: " << rgv.part->getUploadTime1() << " ,���Ͽ�ʼʱ��:" << rgv.part->getDownloadTime1() << " ��CNC " << rgv.part->getProcessCNC2() << " #��ɵڶ�������" << ",���Ͽ�ʼʱ��: " << rgv.part->getUploadTime2() << " ,���Ͽ�ʼʱ��:" << rgv.part->getDownloadTime2() << endl;
					}
				}
				rgv.part = NULL;
				rgv.Wait();
				MainTime--;//����Ҫע���������!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			}
		}

		for (int i = 0; i < 8; i++) {
			if (cnc[i].getSituation() == Broken) {
				if (cnc[i].getStartBrokenTime() == MainTime) {
					if (talk == 1) {
						cout << "�� " << MainTime << " ��";
						cout << ",CNC " << i + 1 << " #�ӹ�" << cnc[i].part->getNo() << " �����ʱ����,";
						cout << "����ʱ�俪ʼʱ��Ϊ " << cnc[i].getStartBrokenTime() << " ,���Ͻ���ʱ��Ϊ " << cnc[i].getEndBrokenTime() << endl;
						cnc[i].part = NULL;
					}
				}

				if (cnc[i].getEndBrokenTime() == MainTime) {
					cnc[i].EndWork();
					MainTime--;//����Ҫע���������!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				}
			}

			if (cnc[i].getSituation() == Working) {
				if (cnc[i].getEndWorkTime() == MainTime) {
					cnc[i].EndWork();
					if (cnc[i].part->getPartSituation() == Raw) {
						cnc[i].part->setPartSituation(Process1);
					}
					else {
						cnc[i].part->setPartSituation(Process2);
					}
					MainTime--;//����Ҫע���������!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				}
			}
		}
	}

	//cout << partComplete << endl;
	for (int i = 0; i < 8; i++) {
		cout << WorkSeq[i] << " ";
	}
	cout << "�����:" << partComplete<<endl;
	cout << "=========" << endl;
	return partComplete*1.;
}




int chrom[sizepop][lenchrom];	// ��Ⱥ
int best_result[lenchrom];		// ��ѱ���
double most_Number;				// ������

	// ��������
void init(void);						// ��Ⱥ��ʼ������

double * max(double *);					// �����������ֵ
void Choice(int[sizepop][lenchrom]);	// ѡ�����
void Cross(int[sizepop][lenchrom]);		// �������
void Mutation(int[sizepop][lenchrom]);	// �������

										// ��Ⱥ��ʼ��
void init(void)
{
	int num = 0;
	while (num < sizepop)
	{
		for (int i = 0; i<sizepop; i++)
			for (int j = 0; j<lenchrom; j++)
				chrom[i][j] = j + 1;
		num++;
		for (int i = 0; i<lenchrom - 1; i++)
		{
			for (int j = i + 1; j<lenchrom; j++)
			{
				int temp = chrom[num][i];
				chrom[num][i] = chrom[num][j];
				chrom[num][j] = temp; // ������num������ĵ�i��Ԫ�غ͵�j��Ԫ��
				num++;
				if (num >= sizepop)
					break;
			}
			if (num >= sizepop)
				break;
		}
		// ������������ѭ�������޷������㹻�ĳ�ʼ���壬������ٲ���һ����
		// ���巽ʽ����ѡ����������λ�ã�Ȼ�󽻻�
		while (num < sizepop)
		{
			double r1 = ((double)rand()) / (RAND_MAX + 1.0);
			double r2 = ((double)rand()) / (RAND_MAX + 1.0);
			int p1 = (int)(lenchrom*r1); // λ��1
			int p2 = (int)(lenchrom*r2); // λ��2
			int temp = chrom[num][p1];
			chrom[num][p1] = chrom[num][p2];
			chrom[num][p2] = temp;    // ��������λ��
			num++;
		}
	}
	chrom[0][0] = 1;chrom[0][1] = 3;chrom[0][2] = 5;chrom[0][3] = 7;chrom[0][4] = 8;chrom[0][5] = 6;chrom[0][6] = 4;chrom[0][7] = 2;
	chrom[1][0] = 1;chrom[1][1] = 2;chrom[1][2] = 3;chrom[1][3] = 4;chrom[1][4] = 5;chrom[1][5] = 6;chrom[1][6] = 7;chrom[1][7] = 8;
	chrom[2][0] = 3;chrom[2][1] = 4;chrom[2][2] = 7;chrom[2][3] = 8;chrom[2][4] = 5;chrom[2][5] = 6;chrom[2][6] = 1;chrom[2][7] = 2;
}
// max()����
double * max(double * arr)
{
	static double best_index[2];
	double max_no = *arr;
	double max_index = 0;
	for (int i = 1; i<sizepop; i++)
	{
		double no = *(arr + i);
		if (no > max_no)
		{
			max_no = no;
			max_index = i;
		}
	}
	best_index[0] = max_index;
	best_index[1] = max_no;
	return best_index;
}

// ѡ�����
void Choice(int chrom[sizepop][lenchrom])
{
	double pick;
	double choice_arr[sizepop][lenchrom];
	double fit_pro[sizepop];
	double sum = 0;
	double fit[sizepop];					// ��Ӧ�Ⱥ�������
	for (int j = 0; j<sizepop; j++)
	{
		double completeNumber = Adaptability(chrom[j]);
		double fitness = completeNumber;
		fit[j] = fitness;
		sum += fitness;
	}
	for (int j = 0; j<sizepop; j++)
	{
		fit_pro[j] = fit[j] / sum; // ��������
	}
	// ��ʼ���̶�
	for (int i = 0; i<sizepop; i++)
	{
		pick = ((double)rand()) / RAND_MAX; // 0��1֮��������  
		for (int j = 0; j<sizepop; j++)
		{
			pick = pick - fit_pro[j];
			if (pick <= 0)
			{
				for (int k = 0; k<lenchrom; k++)
					choice_arr[i][k] = chrom[j][k]; // ѡ��һ������
				break;
			}
		}

	}
	for (int i = 0; i<sizepop; i++)
	{
		for (int j = 0; j<lenchrom; j++)
			chrom[i][j] = choice_arr[i][j];
	}
}

//�������
void Cross(int chrom[sizepop][lenchrom])
{
	double pick;
	double pick1, pick2;
	int choice1, choice2;
	int pos1, pos2;
	int temp;
	int conflict1[lenchrom]; // ��ͻλ��
	int conflict2[lenchrom];
	int num1, num2;
	int index1, index2;
	int move = 0; // ��ǰ�ƶ���λ��
	while (move<lenchrom - 1)
	{
		pick = ((double)rand()) / RAND_MAX; // ���ھ����Ƿ���н������
		if (pick > pcross)
		{
			move += 2;
			continue; // ���β����н���
		}
		// ���ò���ӳ���ӽ�
		choice1 = move; // ����ѡȡ�ӽ�����������
		choice2 = move + 1; // ע������±�Խ��
		pick1 = ((double)rand()) / (RAND_MAX + 1.0);
		pick2 = ((double)rand()) / (RAND_MAX + 1.0);
		pos1 = (int)(pick1*lenchrom); // ����ȷ�������ӽ����λ��
		pos2 = (int)(pick2*lenchrom);
		while (pos1 > lenchrom - 2 || pos1 < 1)
		{
			pick1 = ((double)rand()) / (RAND_MAX + 1.0);
			pos1 = (int)(pick1*lenchrom);
		}
		while (pos2 > lenchrom - 2 || pos2 < 1)
		{
			pick2 = ((double)rand()) / (RAND_MAX + 1.0);
			pos2 = (int)(pick2*lenchrom);
		}
		if (pos1 > pos2)
		{
			temp = pos1;
			pos1 = pos2;
			pos2 = temp; // ����pos1��pos2��λ��
		}
		for (int j = pos1; j <= pos2; j++)
		{
			temp = chrom[choice1][j];
			chrom[choice1][j] = chrom[choice2][j];
			chrom[choice2][j] = temp; // �������˳��
		}
		num1 = 0;
		num2 = 0;
		if (pos1 > 0 && pos2 < lenchrom - 1)
		{
			for (int j = 0; j <= pos1 - 1; j++)
			{
				for (int k = pos1; k <= pos2; k++)
				{
					if (chrom[choice1][j] == chrom[choice1][k])
					{
						conflict1[num1] = j;
						num1++;
					}
					if (chrom[choice2][j] == chrom[choice2][k])
					{
						conflict2[num2] = j;
						num2++;
					}
				}
			}
			for (int j = pos2 + 1; j<lenchrom; j++)
			{
				for (int k = pos1; k <= pos2; k++)
				{
					if (chrom[choice1][j] == chrom[choice1][k])
					{
						conflict1[num1] = j;
						num1++;
					}
					if (chrom[choice2][j] == chrom[choice2][k])
					{
						conflict2[num2] = j;
						num2++;
					}
				}

			}
		}
		if ((num1 == num2) && num1 > 0)
		{
			for (int j = 0; j<num1; j++)
			{
				index1 = conflict1[j];
				index2 = conflict2[j];
				temp = chrom[choice1][index1]; // ������ͻ��λ��
				chrom[choice1][index1] = chrom[choice2][index2];
				chrom[choice2][index2] = temp;
			}
		}
		move += 2;
	}
}

// �������
// ������Բ�ȡ���ѡȡ�����㣬����Ի�λ��
void Mutation(int chrom[sizepop][lenchrom])
{
	double pick, pick1, pick2;
	int pos1, pos2, temp;
	for (int i = 0; i<sizepop; i++)
	{
		pick = ((double)rand()) / RAND_MAX; // �����ж��Ƿ���б������
		if (pick > pmutation)
			continue;
		pick1 = ((double)rand()) / (RAND_MAX + 1.0);
		pick2 = ((double)rand()) / (RAND_MAX + 1.0);
		pos1 = (int)(pick1*lenchrom); // ѡȡ���б����λ��
		pos2 = (int)(pick2*lenchrom);
		while (pos1 > lenchrom - 1)
		{
			pick1 = ((double)rand()) / (RAND_MAX + 1.0);
			pos1 = (int)(pick1*lenchrom);
		}
		while (pos2 > lenchrom - 1)
		{
			pick2 = ((double)rand()) / (RAND_MAX + 1.0);
			pos2 = (int)(pick2*lenchrom);
		}
		temp = chrom[i][pos1];
		chrom[i][pos1] = chrom[i][pos2];
		chrom[i][pos2] = temp;
	}
}

// ������
int main(void)
{
	time_t start, finish;
	start = clock(); // ��ʼ��ʱ
	srand((unsigned)time(NULL)); // ��ʼ�����������
	init(); // ��ʼ����Ⱥ

	InputInfo();

	int best_fit_index = 0;						//���Ž���ִ���
	double no_arr[sizepop];
	double dis;
	for (int j = 0; j<sizepop; j++)
	{
		dis = Adaptability(chrom[j]);
		no_arr[j] = dis;
	}
	double * best_index = max(no_arr); // �������Ž⼰���
	most_Number = *(best_index + 1); // ���Ž��
	int index = (int)(*best_index); // ���Ž�����
	for (int j = 0; j<lenchrom; j++)
		best_result[j] = chrom[index][j]; // ���Ž������

										  // ��ʼ����
	double * new_arr;
	double new_max_arr;
	int new_index;
	for (int i = 0; i<maxgen; i++)
	{
		Choice(chrom); // ѡ��
		Cross(chrom); //����
		Mutation(chrom); //����
		for (int j = 0; j<sizepop; j++)
			no_arr[j] = Adaptability(chrom[j]); // ��������
		new_arr = max(no_arr);
		new_max_arr = *(new_arr + 1);			//�µ����Ž��
		if (new_max_arr > most_Number)
		{
			most_Number = new_max_arr;			// �������Ž��
			new_index = (int)(*new_arr);
			for (int j = 0; j<lenchrom; j++)
				best_result[j] = chrom[new_index][j]; // �������Ž������
			best_fit_index = i + 1;				// ���Ž������
		}
	}
	finish = clock(); // �������
	double duration = ((double)(finish - start)) / CLOCKS_PER_SEC; // �����ʱ

	talk = 1;
	Adaptability(best_result);

	printf("������ʹ���Ŵ��㷨����ģΪ%d������,��Ⱥ��ĿΪ:%d,��������Ϊ:%d\n", lenchrom, sizepop, maxgen);
	printf("�õ����ű���:%d %d %d %d %d %d %d %d\n", best_result[0], best_result[1], best_result[2],
		best_result[3], best_result[4], best_result[5], best_result[6], best_result[7]);
	printf("������Ϊ:%lf,�õ��������ڵ�%d��.\n", most_Number, best_fit_index);
	printf("�����ʱ:%lf��.\n", duration);
	return 0;
}