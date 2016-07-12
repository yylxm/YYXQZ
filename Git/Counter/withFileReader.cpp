#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include<windows.h>
#include <thread>
#include "FileReader.h"

using namespace std;

typedef struct{
	int usefultimes;
	int times_15;
	int times_30;
	int times_60;
	int times_120;
	int times_180;
	int times_240;
	int times_300;
	int times_endless;
}bus_intervaltime;

typedef struct{
	string roadId;
	//string downroadId;
	long currenttime;
	string busId;
	double longitude;
	double latitude;
}data_node;

uint32_t totalnum;//有效数据总量
uint32_t total; //总的数据量
map<string,bus_intervaltime> mapBustime;//存储每辆Bus的有效数据
//long time[9]={0};//每个时间段内数据量（<15s,15s~30s,30s~60s,1m~2m,2m~3m,4m~5m,5m~）

data_node SliptData(string line){
	data_node temp;
	int len = line.length();
	int k = 0;
	int  i =0,j = i ;
	for (; i< len; i++)
	{		
		if(line[i]!= 1 && line[i]!='\n')
		{
			continue;
		}
		switch (k)
		{
		case 0:
			temp.roadId = line.substr(j,i);
			break;
		case 1:
			temp.currenttime = atol(line.substr(j,i-j).c_str());
			break;
		case 2:
			temp.busId = line.substr(j,i-j);
			break;
		case 3:
			temp.longitude =  strtod(line.substr(j,i-j).c_str(),NULL);
			break;
		default:
			break;
		}	
		k++;
		j = i+1;
	}
	temp.latitude =  strtod(line.substr(j,i).c_str(),NULL);
	return temp;
}
// 
// void Printdata(data_node temp){
// 	cout<< "RoadId:"<<temp.roadId <<endl;
// 	cout<<"CurrentTime:"<<temp.currenttime<<endl;
// 	cout<<"BusId:"<<temp.busId<<endl;
// 	printf("Longitude:%.6f\n",temp.longitude);
// 	printf("latitude:%.6f\n",temp.latitude);
// }

void PrintTimes(data_node d,bus_intervaltime t){
	DWORD dwStart = GetTickCount();
	cout<<"time :"<<dwStart<<endl;
	cout<<"total:"<<total<<endl;
	cout<<"BusId:"<<d.busId<<endl;
	cout<<"usefultimes:"<<t.usefultimes<<endl;
	cout<<"<15s:"<<t.times_15<<endl;
	cout<<"15s~30s:"<<t.times_30<<endl;
	cout<<"30s~60s:"<<t.times_60<<endl;
	cout<<"1m~2m:"<<t.times_120<<endl;
	cout<<"2m~3m:"<<t.times_180<<endl;
	cout<<"3m~4m:"<<t.times_240<<endl;
	cout<<"4m~5m:"<<t.times_300<<endl;
	cout<<"5m~:"<<t.times_endless<<endl;

}

void PrintMap(map<string,bus_intervaltime> &t){
	for (auto iter = mapBustime.begin();iter!=mapBustime.end();iter++)
	{
		
		cout<<"BusId:"<<iter->first;
		cout<<"usefultimes:"<<iter->second.usefultimes<<endl;
		cout<<"<15s:"<<iter->second.times_15<<endl;
		cout<<"15s~30s:"<<iter->second.times_30<<endl;
		cout<<"30s~60s:"<<iter->second.times_60<<endl;
		cout<<"1m~2m:"<<iter->second.times_120<<endl;
		cout<<"2m~3m:"<<iter->second.times_180<<endl;
		cout<<"3m~4m:"<<iter->second.times_240<<endl;
		cout<<"4m~5m:"<<iter->second.times_300<<endl;
		cout<<"5m~:"<<iter->second.times_endless<<endl;
	}
}

void InitializeTimes(bus_intervaltime &t){
	t.times_15 =0;
	t.times_30 =0;
	t.times_60 =0;
	t.times_120 =0;
	t.times_180 =0;
	t.times_240 =0;
	t.times_300 =0;
	t.times_endless =0;
	t.usefultimes =0;
}


int main(int argc ,char ** argv){
	//ifstream in;
	//DWORD dwStart = GetTickCount();
	//cout<<"time :"<<dwStart<<endl;
	string line="";//读取文件的行
	data_node first;
	data_node second;
	bus_intervaltime times;
	InitializeTimes(times);

	FileReader fileReader(argv[1],1<<30);
	line = fileReader.getline();
	if (line.length()!=0)
	{
		total++;
		first = SliptData(line);
		if (first.latitude != 0.0 && first.longitude !=0.0)
		{
			totalnum++;	
		}
	}
	uint32_t print_gap = 1000000;

	while ((line = fileReader.getline()).length()!= 0)
	{
		
		total++;
		print_gap -- ;
		if (print_gap == 0)
		{
			printf("%d line has been read!\n",total);
			print_gap = 1000000;
		}
		continue;
		second = SliptData(line);
		if (first.currenttime != second.currenttime && second.latitude != 0.0 && second.longitude != 0.0 )
		{
			totalnum++;
			if (strcmp((second.busId).c_str(),(first.busId).c_str())==0)
			{
				times.usefultimes++;
				int timeDiff=second.currenttime-first.currenttime;
				if (timeDiff<15){
					times.times_15++;
				}else if (15<=timeDiff<30){
					times.times_30++;
				}else if (30<=timeDiff<60){
					times.times_60++;
				}else if (60<=timeDiff<120){
					times.times_120++;
				}else if (120<=timeDiff<180){
					times.times_180++;
				}else if (180<=timeDiff<240){
					times.times_240++;
				}else if (240<=timeDiff<300){
					times.times_300++;
				}else{
					times.times_endless++;
				}
			}else{				
				//PrintTimes(first,times);
				mapBustime.insert(pair<string,bus_intervaltime>(first.busId,times));
				InitializeTimes(times);
			}
		}
		first = second;
	}
	
	mapBustime.insert(pair<string,bus_intervaltime>(first.busId,times));
	//PrintTimes(first,times);

	//PrintMap(mapBustime);

	cout<<"totalnum:"<<totalnum<<endl;
	system("pause");
	return 0;
}