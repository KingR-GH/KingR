#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<iostream>
#include<time.h>
#include<math.h>
#define maxgen 50  // 最大进化代数
#define sizepop 15 // 种群数目
#define pcross 0.6 // 交叉概率
#define pmutation 0.2 // 变异概率
#define lenchrom 8 // 染色体长度
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
int BreakDown = -1;																//故障系数默认为-1
int ProcessMethod = 1;															//工序数默认为1;
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
	//获取物件状态
	PartSituation getPartSituation() {
		return partSituation;
	}
	//设置物件状态
	void setPartSituation(PartSituation p) {
		partSituation = p;
	}
	
	//设置上料时间
	void setUploadTime1(Time uploadtime) {
		UploadTime1 = uploadtime;
	}	
	void setUploadTime2(Time uploadtime) {
		UploadTime2 = uploadtime;
	}
	//获取上料时间
	int getUploadTime1() {
		return UploadTime1;
	}
	int getUploadTime2() {
		return UploadTime2;
	}
	//设置下料时间
	void setDownloadTime1(Time downloadtime) {
		DownloadTime1 = downloadtime;
	}
	void setDownloadTime2(Time downloadtime) {
		DownloadTime2 = downloadtime;
	}
	//获取下料时间
	int getDownloadTime1() {
		return DownloadTime1;
	}
	int getDownloadTime2() {
		return DownloadTime2;
	}
	//设置各道工序CNC
	void setProcessCNC1(int no) {
		ProcessingCNC1 = no;
	}
	void setProcessCNC2(int no) {
		ProcessingCNC2 = no;
	}
	//获取各道工序CNC
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
	Time T1;							//一道工序加工零件所需时间；
	Time T21, T22;						//两道工序加工零件分别所需时间;
	Time StartWorkTime;					//开始工作的时间;
	Time EndWorkTime;					//结束工作的时间;
	Time StartBrokenTime;				//开始故障的时间;
	Time EndBrokenTime;					//结束故障的时间;
	CNCSituation workSituation;			//工作状态,分为三种: NotWork 和 Working 和 Broken;
public:
	Part * part;
	//CNC构造函数,初始化工作状态,工作所需时间;
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
	//开始工作,设定开始工作时间和结束工作时间;
	void StartWork(Time &MainTime) {
		//零件加工时间长度
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
	//结束工作;
	void EndWork() {
		workSituation = NotWork;
		StartWorkTime = -1;
		EndWorkTime = -1;
		StartBrokenTime = -1;
		EndBrokenTime = -1;
	}
	//获取CNC状态;
	CNCSituation getSituation() {
		return workSituation;
	}
	//获取结束工作时间;
	Time getEndWorkTime() {
		return EndWorkTime;
	}
	//获取机器编号
	int getNo() {
		return No;
	}
	//机器故障;
	void CNCBreakDown(Time MainTime) {
		workSituation = Broken;
		//零件加工时间长度
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
	//获取故障开始时间
	Time getStartBrokenTime() {
		return StartBrokenTime;
	}
	//获取结束故障时间
	Time getEndBrokenTime() {
		return EndBrokenTime;
	}

	void debug() {
		cout << "CNC信息:" << endl;
		cout << No << " " << T1 << " " << T21 << " " << T22 << " " << workSituation << endl;;
	}
};

class RGV {
private:
	int Position;					//RGV所在位置
	int Destination;
	Time M0, M1, M2, M3;			//RGV移动0个单位,1个单位,2个单位,3个单位所需时间
	Time Odd;						//RGV为奇数机器上下料时间
	Time Even;						//RGV为偶数机器上下料时间
	Time load;						//RGV上下料时间
	Time C;							//RGV清洗作业时间
	Time situationStart;
	Time situationEnd;
	RGVSituation situation;			//RGV当前状态


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

	//计算并返回RGV在两个位置间的移动时间
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
		cout << "RGV信息:" << endl;
		cout << Position << " " << M0 << " " << M1 << " " << M2 << " " << M3 << " " << Odd << " " << Even << " " << C << " " << situation << endl;
	}
};


Time MainTime;
Time TotalTime;
CNC cnc[8];
RGV rgv;

double Adaptability(int WorkSeq[8]);				//适应度函数
void InputInfo();//输入CNC和RGV基本信息

void InputInfo() {
	Time M0, M1, M2, M3;
	Time T1, T21, T22;
	Time Odd, Even;
	Time Clean;

	cout << "输入系统运行总时间:" << endl;
	cin >> TotalTime;

	cout << "输入完成一个物件所需工序数:"<<endl;
	cin >> ProcessMethod;

	cout << "输入Y或N决定是否考虑故障状态:"<<endl;
	char flag;
	cin >> flag;
	if (flag == 'Y' ||flag == 'Y'-'A'+'a') {
		BreakDown = 50;
	}
	else {
		BreakDown = -1;
	}

	M0 = 0;
	cout << "输入RGV移动1个单位所需时间:" << endl;
	cin >> M1;
	cout << "输入RGV移动2个单位所需时间:" << endl;
	cin >> M2;
	cout << "输入RGV移动3个单位所需时间:" << endl;
	cin >> M3;
	cout << "输入CNC加工完成一个一道工序的物料所需时间:" << endl;
	cin >> T1;
	cout << "输入CNC加工完成一个两道工序的物料的第一道工序所需时间:" << endl;
	cin >> T21;
	cout << "输入CNC加工完成一个两道工序的物料的第二道工序所需时间:" << endl;
	cin >> T22;
	cout << "输入RGV为CNC1#，3#，5#，7#一次上下料所需时间:" << endl;
	cin >> Odd;
	cout << "输入RGV为CNC2#，4#，6#，8#一次上下料所需时间:" << endl;
	cin >> Even;
	cout << "输入RGV完成一个物料的清洗作业所需时间:" << endl;
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

	//初始化车间系统
	MainTime = 0;
	for (int i = 0; i < 8; i++) {
		cnc[i].EndWork();
		cnc[i].part = NULL;
	}
	rgv.part = NULL;
	rgv.Wait();

	int partNumber = 0;
	int partComplete = 0; //完成的工件数
	Part part[10000];
	rgv.SetPosition(WorkSeq[0]);
	for (MainTime = 0; MainTime <= TotalTime; MainTime++) {
		if (rgv.getRGVSituation() == Waiting) {
			//debug
			//cout << "RGV等待: " << MainTime << endl;
			for (int i = 0; i < 8; i++) {
				if (cnc[WorkSeq[i] - 1].getSituation() == NotWork) {
					//rgv.Move(MainTime, rgv.getPosition(), cnc[WorkSeq[i - 1] - 1].getNo());
					rgv.Move(MainTime, rgv.getPosition(), WorkSeq[i]);

					//debug
					//cout << "RGV移动时间: " << MainTime <<" 起点: "<<rgv.getPosition()<<" 终点: "<<cnc[WorkSeq[i]-1].getNo()<< endl;
					MainTime--;//后面要注意这条语句!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					break;
				}
			}
		}
		else if (rgv.getRGVSituation() == Moving) {
			//debug
			//cout << "RGV移动: " << MainTime << endl;
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
			//cout << "RGV上下料: " << MainTime << endl;
			if (MainTime == rgv.getSituationEnd()) {
				if (cnc[rgv.getPosition() - 1].part == NULL) {
					cnc[rgv.getPosition() - 1].part = rgv.part;
					rgv.part = NULL;
					rgv.Wait();
					MainTime--;//后面要注意这条语句!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				}//CNC上无物件
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
							MainTime--;//后面要注意这条语句!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
						}
					}
					else {
						rgv.part->setDownloadTime2(MainTime - rgv.getLoad());
						rgv.Clean(MainTime);
					}

				}//CNC上有物件

				int CNCBreakDownElem = rand() % 100;
				if (CNCBreakDownElem == BreakDown) {
					//cout << rgv.getPosition() <<" 号机器"<< "加工 " << cnc[rgv.getPosition() - 1].part->getNo() << " 号工件时出现故障,";

					cnc[rgv.getPosition() - 1].CNCBreakDown(MainTime);

					//cout << "故障时间开始时间为 "<<cnc[rgv.getPosition()-1].getStartBrokenTime() << " ,故障结束时间为 "<<cnc[rgv.getPosition()-1].getEndBrokenTime()<<endl;
				}
				else {
					cnc[rgv.getPosition() - 1].StartWork(MainTime);
				}


			}
		}
		else if (rgv.getRGVSituation() == Cleaning) {
			//debug
			//cout << "RGV清洗: " << MainTime << endl;
			if (MainTime == rgv.getSituationEnd()) {
				partComplete++;
				if (talk == 1) {
					if (ProcessMethod == 1) {
						cout << "第 " << MainTime << " 秒";
						cout << " 物件序号: " << rgv.part->getNo() << " 完成序号: " << partComplete << " 由CNC " << rgv.part->getProcessCNC1() << " #完成工序" << ",上料开始时间: " << rgv.part->getUploadTime1() << " ,下料开始时间:" << rgv.part->getDownloadTime1() << endl;
					}
					else {
						cout << "第 " << MainTime << " 秒";
						cout << " 物件序号: " << rgv.part->getNo() << " 完成序号: " << partComplete << " 由CNC " << rgv.part->getProcessCNC1() << " #完成第一道工序" << ",上料开始时间: " << rgv.part->getUploadTime1() << " ,下料开始时间:" << rgv.part->getDownloadTime1() << " 由CNC " << rgv.part->getProcessCNC2() << " #完成第二道工序" << ",上料开始时间: " << rgv.part->getUploadTime2() << " ,下料开始时间:" << rgv.part->getDownloadTime2() << endl;
					}
				}
				rgv.part = NULL;
				rgv.Wait();
				MainTime--;//后面要注意这条语句!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			}
		}

		for (int i = 0; i < 8; i++) {
			if (cnc[i].getSituation() == Broken) {
				if (cnc[i].getStartBrokenTime() == MainTime) {
					if (talk == 1) {
						cout << "第 " << MainTime << " 秒";
						cout << ",CNC " << i + 1 << " #加工" << cnc[i].part->getNo() << " 号零件时故障,";
						cout << "故障时间开始时间为 " << cnc[i].getStartBrokenTime() << " ,故障结束时间为 " << cnc[i].getEndBrokenTime() << endl;
						cnc[i].part = NULL;
					}
				}

				if (cnc[i].getEndBrokenTime() == MainTime) {
					cnc[i].EndWork();
					MainTime--;//后面要注意这条语句!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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
					MainTime--;//后面要注意这条语句!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				}
			}
		}
	}

	//cout << partComplete << endl;
	for (int i = 0; i < 8; i++) {
		cout << WorkSeq[i] << " ";
	}
	cout << "完成数:" << partComplete<<endl;
	cout << "=========" << endl;
	return partComplete*1.;
}




int chrom[sizepop][lenchrom];	// 种群
int best_result[lenchrom];		// 最佳编码
double most_Number;				// 最多产量

	// 函数声明
void init(void);						// 种群初始化函数

double * max(double *);					// 计算数组的最值
void Choice(int[sizepop][lenchrom]);	// 选择操作
void Cross(int[sizepop][lenchrom]);		// 交叉操作
void Mutation(int[sizepop][lenchrom]);	// 变异操作

										// 种群初始化
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
				chrom[num][j] = temp; // 交换第num个个体的第i个元素和第j个元素
				num++;
				if (num >= sizepop)
					break;
			}
			if (num >= sizepop)
				break;
		}
		// 如果经过上面的循环还是无法产生足够的初始个体，则随机再补充一部分
		// 具体方式就是选择两个基因位置，然后交换
		while (num < sizepop)
		{
			double r1 = ((double)rand()) / (RAND_MAX + 1.0);
			double r2 = ((double)rand()) / (RAND_MAX + 1.0);
			int p1 = (int)(lenchrom*r1); // 位置1
			int p2 = (int)(lenchrom*r2); // 位置2
			int temp = chrom[num][p1];
			chrom[num][p1] = chrom[num][p2];
			chrom[num][p2] = temp;    // 交换基因位置
			num++;
		}
	}
	chrom[0][0] = 1;chrom[0][1] = 3;chrom[0][2] = 5;chrom[0][3] = 7;chrom[0][4] = 8;chrom[0][5] = 6;chrom[0][6] = 4;chrom[0][7] = 2;
	chrom[1][0] = 1;chrom[1][1] = 2;chrom[1][2] = 3;chrom[1][3] = 4;chrom[1][4] = 5;chrom[1][5] = 6;chrom[1][6] = 7;chrom[1][7] = 8;
	chrom[2][0] = 3;chrom[2][1] = 4;chrom[2][2] = 7;chrom[2][3] = 8;chrom[2][4] = 5;chrom[2][5] = 6;chrom[2][6] = 1;chrom[2][7] = 2;
}
// max()函数
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

// 选择操作
void Choice(int chrom[sizepop][lenchrom])
{
	double pick;
	double choice_arr[sizepop][lenchrom];
	double fit_pro[sizepop];
	double sum = 0;
	double fit[sizepop];					// 适应度函数数组
	for (int j = 0; j<sizepop; j++)
	{
		double completeNumber = Adaptability(chrom[j]);
		double fitness = completeNumber;
		fit[j] = fitness;
		sum += fitness;
	}
	for (int j = 0; j<sizepop; j++)
	{
		fit_pro[j] = fit[j] / sum; // 概率数组
	}
	// 开始轮盘赌
	for (int i = 0; i<sizepop; i++)
	{
		pick = ((double)rand()) / RAND_MAX; // 0到1之间的随机数  
		for (int j = 0; j<sizepop; j++)
		{
			pick = pick - fit_pro[j];
			if (pick <= 0)
			{
				for (int k = 0; k<lenchrom; k++)
					choice_arr[i][k] = chrom[j][k]; // 选中一个个体
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

//交叉操作
void Cross(int chrom[sizepop][lenchrom])
{
	double pick;
	double pick1, pick2;
	int choice1, choice2;
	int pos1, pos2;
	int temp;
	int conflict1[lenchrom]; // 冲突位置
	int conflict2[lenchrom];
	int num1, num2;
	int index1, index2;
	int move = 0; // 当前移动的位置
	while (move<lenchrom - 1)
	{
		pick = ((double)rand()) / RAND_MAX; // 用于决定是否进行交叉操作
		if (pick > pcross)
		{
			move += 2;
			continue; // 本次不进行交叉
		}
		// 采用部分映射杂交
		choice1 = move; // 用于选取杂交的两个父代
		choice2 = move + 1; // 注意避免下标越界
		pick1 = ((double)rand()) / (RAND_MAX + 1.0);
		pick2 = ((double)rand()) / (RAND_MAX + 1.0);
		pos1 = (int)(pick1*lenchrom); // 用于确定两个杂交点的位置
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
			pos2 = temp; // 交换pos1和pos2的位置
		}
		for (int j = pos1; j <= pos2; j++)
		{
			temp = chrom[choice1][j];
			chrom[choice1][j] = chrom[choice2][j];
			chrom[choice2][j] = temp; // 逐个交换顺序
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
				temp = chrom[choice1][index1]; // 交换冲突的位置
				chrom[choice1][index1] = chrom[choice2][index2];
				chrom[choice2][index2] = temp;
			}
		}
		move += 2;
	}
}

// 变异操作
// 变异策略采取随机选取两个点，将其对换位置
void Mutation(int chrom[sizepop][lenchrom])
{
	double pick, pick1, pick2;
	int pos1, pos2, temp;
	for (int i = 0; i<sizepop; i++)
	{
		pick = ((double)rand()) / RAND_MAX; // 用于判断是否进行变异操作
		if (pick > pmutation)
			continue;
		pick1 = ((double)rand()) / (RAND_MAX + 1.0);
		pick2 = ((double)rand()) / (RAND_MAX + 1.0);
		pos1 = (int)(pick1*lenchrom); // 选取进行变异的位置
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

// 主函数
int main(void)
{
	time_t start, finish;
	start = clock(); // 开始计时
	srand((unsigned)time(NULL)); // 初始化随机数种子
	init(); // 初始化种群

	InputInfo();

	int best_fit_index = 0;						//最优解出现代数
	double no_arr[sizepop];
	double dis;
	for (int j = 0; j<sizepop; j++)
	{
		dis = Adaptability(chrom[j]);
		no_arr[j] = dis;
	}
	double * best_index = max(no_arr); // 计算最优解及序号
	most_Number = *(best_index + 1); // 最优结果
	int index = (int)(*best_index); // 最优结果序号
	for (int j = 0; j<lenchrom; j++)
		best_result[j] = chrom[index][j]; // 最优结果序列

										  // 开始进化
	double * new_arr;
	double new_max_arr;
	int new_index;
	for (int i = 0; i<maxgen; i++)
	{
		Choice(chrom); // 选择
		Cross(chrom); //交叉
		Mutation(chrom); //变异
		for (int j = 0; j<sizepop; j++)
			no_arr[j] = Adaptability(chrom[j]); // 个数数组
		new_arr = max(no_arr);
		new_max_arr = *(new_arr + 1);			//新的最优结果
		if (new_max_arr > most_Number)
		{
			most_Number = new_max_arr;			// 更新最优结果
			new_index = (int)(*new_arr);
			for (int j = 0; j<lenchrom; j++)
				best_result[j] = chrom[new_index][j]; // 更新最优结果序列
			best_fit_index = i + 1;				// 最优结果代数
		}
	}
	finish = clock(); // 计算结束
	double duration = ((double)(finish - start)) / CLOCKS_PER_SEC; // 计算耗时

	talk = 1;
	Adaptability(best_result);

	printf("本程序使用遗传算法求解规模为%d的问题,种群数目为:%d,进化代数为:%d\n", lenchrom, sizepop, maxgen);
	printf("得到最优编码:%d %d %d %d %d %d %d %d\n", best_result[0], best_result[1], best_result[2],
		best_result[3], best_result[4], best_result[5], best_result[6], best_result[7]);
	printf("最多产量为:%lf,得到最多产量在第%d代.\n", most_Number, best_fit_index);
	printf("程序耗时:%lf秒.\n", duration);
	return 0;
}