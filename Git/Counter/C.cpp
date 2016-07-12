#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <iostream>
#include <map>
#include <windows.h>
#include <string>

using namespace std;

struct Record {
	size_t time;
	char bus_id[8];
	char road_id[50];
	double lat;
	double lon;
};

struct Result{
	int sum;
	int n_0_15;
	int n_15_30;
	int n_30_60;
	int n_60_120;
	int n_120_180;
	int n_180_240;
	int n_240_300;
	int n_300_l;
	Result():sum(0),n_0_15(0),n_15_30(0),n_30_60(0),n_60_120(0),n_120_180(0),n_180_240(0) ,n_240_300(0),n_300_l(0){
	}
	void print(FILE *outfile) {
		fprintf(outfile,"SUM:%d\t0_15:%d\t15_30:%d\t30_60:%d\t60_120:%d\t120_180:%d\t180_240:%d\t20_300:%d\t300_l:%d\n",
			sum, n_0_15, n_15_30, n_30_60, n_60_120, n_120_180, n_180_240,n_240_300,n_300_l);
	}
};

void Initialization(Result *res){
	res->n_0_15=0;
	res->n_120_180=0;
	res->n_15_30 =0;
	res->n_180_240=0;
	res->n_240_300=0;
	res->n_300_l =0;
	res->n_30_60=0;
	res->n_60_120=0;
	res->sum=0;
}


void extract_record(char line[],Record *record) {
// 	if (line[0] != '[') {
// 		//printf("get a bad line, skip\n");
// 		//printf("%s", line);
// 		record->time = -1;
// 		return -1;
// 	}
//	char temp1[500];
	for (int i = 0; i < strlen(line); ++i) {
		if (line[i] == '[' || line[i] == ']' || line[i] == '\001') line[i] = ' ';
	}
	sscanf_s(line, "%499s %u %7s %lf %lf", record->road_id, (unsigned)_countof(record->road_id),
		&(record->time),
		record->bus_id, (unsigned)_countof(record->bus_id),
		&(record->lat), &(record->lon));
}

//  void PrintTimes(Record d,Result &t){
// 	cout<<"BusId:"<<d.bus_id<<endl;
//  	cout<<"sum:"<<t.sum<<endl;
//  	cout<<"<15s:"<<t.n_0_15<<endl;
//  	cout<<"15s~30s:"<<t.n_15_30<<endl;
//  	cout<<"30s~60s:"<<t.n_30_60<<endl;
//  	cout<<"1m~2m:"<<t.n_60_120<<endl;
//  	cout<<"2m~3m:"<<t.n_120_180<<endl;
//  	cout<<"3m~4m:"<<t.n_180_240<<endl;
//  	cout<<"4m~5m:"<<t.n_240_300<<endl;
//  	cout<<"5m~:"<<t.n_300_l<<endl;
//  }

int main(int argc, char *argv[]){
	if (argc < 2) {
		cout << "Please specify a DATA file!" << endl;
		exit(-1);
	}
	FILE *input_file=fopen(argv[1],"r");
	FILE *output_file=fopen(argv[2],"w");
	if (input_file==NULL || output_file==NULL)
	{
		cout<<"Cannot open the file!"<<endl;
		return -1;
	}
	map <string,Result*> ret;
	char line[1024];
	DWORD dwStart = GetTickCount();

	Record record,last_record;
	last_record.time = 0;

	while (1)
	{
		printf("Running!\n");
		char *str=fgets(line,1024,input_file);
		if (str == NULL) break;
		//printf("running!\n");
		//提取每条记录的信息
		extract_record(line,&record);

		//除去无用的记录
		if (record.time == last_record.time || record.lat == 0 || record.lon ==0 || record.road_id[0] =='\\'){
			//last_record = record;
			continue;
		}

		if (strcmp(last_record.bus_id, record.bus_id) == 0) {
			//Result *loc= new Result();
			Result *loc = ret.at(string(record.bus_id));
			int time_gap = record.time - last_record.time;
			loc->sum++;
			if (time_gap < 15) {
				loc->n_0_15++;
			}
			else if (time_gap < 30) {
				loc->n_15_30++;
			}
			else if (time_gap < 60) {
				loc->n_30_60++;
			}
			else if (time_gap < 120) {
				loc->n_60_120++;
			}
			else if (time_gap < 180) {
				loc->n_120_180++;
			}
			else if (time_gap < 240) {
				loc->n_180_240++;
			}else if (time_gap < 300){
				loc ->n_240_300++;
			}else{
				loc->n_300_l++;
			}
		}else {	
			if (record.time != -1)
				ret.insert(pair<string, Result*>(string(record.bus_id), new Result()));
		}
		last_record =record;
	}
	ret.insert(pair<string, Result*>(string(record.bus_id), new Result()));

	DWORD dwEnd = GetTickCount();

	for (auto iter = ret.begin();iter!=ret.end();iter++)
	{
		fprintf(output_file,"Bus_Id: %s\n", (*iter).first.c_str());
		(*iter).second->print(output_file);
	}

	fprintf(output_file,"using time %lfs\n", dwStart - dwEnd);
	fclose(input_file);
	input_file =NULL;
	fclose(output_file);
	output_file = NULL;
	return 0;
}