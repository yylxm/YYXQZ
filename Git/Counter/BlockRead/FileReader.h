#include <string>
#include <stdlib.h>

using namespace std;

void  thread_read(char *buffer,int size,int id,FILE *file){
	printf("thread %d read.\n",id);
	fread(buffer+size*id,sizeof(char),size,file);
}

class FileReader
{
public:
	FileReader(string file_path,int size){
		_buffer = (char *)malloc(sizeof(char)*size);
		buffer_size = size;
		if ((stream = fopen(file_path.c_str(),"r+")) == NULL)
		{
			cout<<"File could not be opened"<<endl;
			exit(0);
		}else{
			//fread(_buffer,sizeof(char),buffer_size,stream);
			thread t0(thread_read,_buffer,1<<28,0,stream);
			thread t1(thread_read,_buffer,1<<28,1,stream);
			thread t2(thread_read,_buffer,1<<28,2,stream);
			thread t3(thread_read,_buffer,1<<28,3,stream);
			t0.detach();
			t1.detach();
			t2.detach();
			t3.detach();
		}	
		pos_end = buffer_size;
		pos_cur = 0;
		pos_start =0;
	}
	~FileReader(){
		free(_buffer);
	};
	string getline(){
		int pos_return = pos_cur;
		bool retry = false;
		do 
		{
			retry = false;
			pos_return = pos_cur;
			while (*(_buffer +pos_return) != '\n' && pos_return<pos_end)
			{
				pos_return++;
			}
			if (pos_return == pos_end)
			{
				if (!consume())
				{
					retry = true;
				}else{
					return string();
				}
			}else{
				string ret((char *)(_buffer +pos_cur),pos_return-pos_cur);
				pos_cur = pos_return+1;
				return ret;
			}
		} while (retry);
	}
	int consume(){
		int remind = pos_end-pos_cur;
		if (remind>0)
		{
			memcpy(_buffer,_buffer+pos_cur,remind);//拷贝剩余数据到内存的起始位置
		}
		int read_count = fread(_buffer+remind,1,buffer_size-remind,stream);
		if (read_count == 0)
		{
			return -1;
		}else{
			pos_cur =0;
			pos_end = remind+read_count;
			return 0;
		}
	}
	
private:
	int buffer_size;
	string input_file;
	int pos_cur;
	int pos_start;
	int pos_end;
	char *_buffer;
	FILE *stream;
};


