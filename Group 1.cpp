/*
———————————
|哈夫曼编码解压缩文件|
|代码总长度：543行   |
|邱子豪：共272行,50% |
|陈淑婷：共241行,44% |
———————————
*/

/*
*头文件及结构体定义
*邱子豪：测试
*陈淑婷：15-62编码
*/
//#include <iostream>
//#include <algorithm>
//#include <cstring>
//#include <math.h>
#include<bits/stdc++.h>
#include<stdlib.h>
using namespace std;

typedef struct weight
{
	unsigned char byte;//字节源码
	long long value;//字节频度
}weight;
weight weights[256];//记录256个字符的频度

struct HaffNode//哈夫曼树的结点结构
{
	unsigned char byte;//结点数据
	long long weight;//权值
	int parent;//双亲结点下标
	int leftChild;//左孩子下标
	int rightChild;//右孩子下标
	char code[256];//哈夫曼编码
	int code_len;//编码长度
};
HaffNode HaffTree[511];//哈夫曼树

struct heads//头部信息
{
	int suffixlength;//后缀长度
	char suffixname[128];//后缀名
	int bytesnum;//电文中字节总数
	int codenum;//编码数量
	char byte[256];//字节
	int weight[256];//字节频度
};
heads head;

int bytes_count=0;//哈夫曼树的叶子结点树
long long file_length=0;//保存文件的字节数，初始为0
const int FILENAMEMAX=127;//文件名（包括路径），长度不超过127
const int SUFFIXNAMEMAX=9;//文件后缀名，长度不超过9个字符
char source_filename[FILENAMEMAX];//源文件名字
char compress_filename[FILENAMEMAX];//压缩文件名字
char decompress_filename[FILENAMEMAX];//解压文件名字

const int BUFF_MAXSIZE=1024*1024;//文件哈夫曼编码缓冲区大小
char buff[BUFF_MAXSIZE]="";//文件哈夫曼编码缓冲区（生成压缩文件时，各字节的编码先存在buff中，然后8个码压缩成一个字节存到buff_string缓冲区中）

/*
*统计文件各字节的频度
*邱子豪：测试
*陈淑婷：69-88编码
*/
void Statistics(char fileName[])
{
	char c;
	file_length=0;
	FILE *fp=fopen(fileName,"rb");
	fread(&c,1,1,fp);
	while(!feof(fp))
	{
		HaffTree[c].weight++;
		file_length++;
		fread(&c,1,1,fp);
	}
	fclose(fp);
	bytes_count=0;
	for(int i=0;i<256;i++)
	{
		if(HaffTree[i].weight>0)
			bytes_count++;
	}
}

/*
*定义两个元素的比较方式
*邱子豪：测试
*陈淑婷：95-98编码
*/
bool compare(HaffNode a,HaffNode b)
{
	return a.weight>b.weight;  //降序
}

/*
*根据字节频度排序
*邱子豪：测试
*陈淑婷：105-108编码
*/
void Sortweights(HaffNode HaffTree[],int bytescount=255)
{
    sort(HaffTree,HaffTree+bytescount,compare);
}

/*
*初始化哈夫曼树存储结构
*邱子豪：测试
*陈淑婷：115-126编码
*/
void initHaffTree()
{
	for(int i=0;i<511;i++)//2*256-1;
	{
		HaffTree[i].byte=i;//=weights[i].byte
		HaffTree[i].code[0]='\0';
		HaffTree[i].leftChild=-1;
		HaffTree[i].rightChild=-1;
		HaffTree[i].parent=-1;
		HaffTree[i].weight=0;//=weights[i].value
	}
}

/*
*通过合并树，构造哈夫曼树
*邱子豪：测试
*陈淑婷：133-162编码
*/
void creatHaffTree()
{
	for(int current=bytes_count;current<bytes_count*2-1;current++)
	{
		int min1=0,min2=-1;
		for(int i=0;i<current;i++)
		{
			if((HaffTree[i].parent==-1)&&(HaffTree[i].weight<HaffTree[min1].weight))
				min1=i;
  		}
		for(int j=0;j<current;j++)
		{
			if(min2==-1)
			{
				if((j!=min1)&&(HaffTree[j].parent==-1))
					min2=j;
			}
			else
			{
				if((j!=min1)&&(HaffTree[j].parent==-1)&&(HaffTree[j].weight<HaffTree[min2].weight))
					min2=j;
			}
		}
		HaffTree[min1].parent=current;
		HaffTree[min2].parent=current;
		HaffTree[current].weight=HaffTree[min1].weight+HaffTree[min2].weight;
		HaffTree[current].leftChild=min1;
		HaffTree[current].rightChild=min2;
	}
}

/*
*构造哈夫曼编码
*邱子豪：测试
*陈淑婷：169-181编码
*/
void creatHaffCode()
{
	int root=bytes_count*2-2;
	HaffTree[root].code[0]='\0';
	for(int i=root-1;i>=0;i--)
	{
		strcpy(HaffTree[i].code, HaffTree[HaffTree[i].parent].code);
		if(i==HaffTree[HaffTree[i].parent].rightChild)//从下往上遍历数组
			strcat(HaffTree[i].code,"1");
		else
			strcat(HaffTree[i].code,"0");
	}
}

/*
*输出排序结果
*邱子豪：测试
*陈淑婷：188-204编码
*/
void PrintSort(int count)
{
	printf("哈夫曼树的存储结构如下：\n");
	printf("  No\tByte\tWeight\tLchild\tRchild\tParent\tCode\n");
	for(int i=0;i<count;i++)
	{
		printf("  %d\t%x\t %d\t %d\t %d\t %d\t %s\n",i,HaffTree[i].byte,
		HaffTree[i].weight,HaffTree[i].leftChild,HaffTree[i].rightChild,
		HaffTree[i].parent,HaffTree[i].code);//十六进制输出为%x
	}
	for(int i=count;i<2*count-1;i++)//叶子结点有count个，双亲节点有count-1个
	{
		printf("  %d\t0\t %d\t %d\t %d\t %d\t %s\n",i,
		HaffTree[i].weight,HaffTree[i].leftChild,HaffTree[i].rightChild,
		HaffTree[i].parent,HaffTree[i].code);
	}
}

/*
*生成压缩文件
*邱子豪：265-385编码，测试
*陈淑婷：211-263编码，测试
*/
void writeCompressFile()
{
	string s(source_filename);
	string filehead,filetail;
	int index=s.find('.');//存储'.'的位置
	filehead=s.substr(0,index);
	filetail=s.substr(index+1);
	FILE *ifp=fopen(source_filename,"r");
	FILE *ofp=fopen(compress_filename,"w");
	head.suffixlength=filetail.length();//扩展名长度
	for(int i=0;i<filetail.length();i++)
		head.suffixname[i]=filetail[i];//扩展名
	head.bytesnum=file_length;//字节总数
	head.codenum=bytes_count;//需编码的字节数
	for(int i=0;i<bytes_count;i++)
	{
		head.byte[i]=HaffTree[i].byte;//字节
		head.weight[i]=HaffTree[i].weight;//字节频度
	}
	fputc(head.suffixlength+48,ofp);//从'0'的ASCII码开始读出字符
	fputc(',',ofp);
	fprintf(ofp,head.suffixname);//格式化输出到文件中
	fputc(',',ofp);
//	//从高位数开始将int型字符总数转为char型存入文件中
//	int num=head.bytesnum;
//	if(num>99)
//	{
//		fputc(num/100+48,ofp);
//		num%=100;
//	}
//	if(num>9)
//		fputc(num/10+48,ofp);
//	fputc(head.bytesnum%10+48,ofp);
//	fputc(',',ofp);
//	//从高位数开始将int型待编码字符总数转为char型存入文件中
//	if(head.codenum<10)
//	{
//		fputc(head.codenum+48,ofp);
//		fputc(',',ofp);
//	}
//	else
//	{
//		fputc(head.codenum/10+48,ofp);
//		fputc(head.codenum%10+48,ofp);
//		fputc(',',ofp);
//	}
//	for(int i=0;i<head.codenum;i++)
//	{
//		fputc(head.byte[i],ofp);
//		fputc(',',ofp);
//		fputc(head.weight[i]+48,ofp);
//		fputc(',',ofp);
//	}

	//写入字节数
	int temp=head.bytesnum;
	vector<int> temp_val;
	while(temp>0)
	{
		temp_val.push_back(temp%10);
		temp/=10;
	}
	vector<int>::iterator it=temp_val.end()-1;
	for(;it>=temp_val.begin();it--)
		fputc(*it+48,ofp);
	fputc(',',ofp);
	//清空数据
	temp_val.clear();
	//释放内存
	vector<int>().swap(temp_val);
	//写入字符数
	temp=head.codenum;
	while(temp>0)
	{
		temp_val.push_back(temp%10);
		temp/=10;
	}
	it=temp_val.end()-1;
	for(;it>=temp_val.begin();it--)
		fputc(*it+48,ofp);
	fputc(',',ofp);
	temp_val.clear();
	vector<int>().swap(temp_val);
	//写入对应字符数量及字符
	for(int i=0;i<head.codenum;i++)
	{
		if(head.byte[i]==',')
		{
			fprintf(ofp,"//");
			fputc(',',ofp);
		}
		else
		{
			fputc(head.byte[i],ofp);
			fputc(',',ofp);
		}
		temp=head.weight[i];
		while(temp>0)
		{
			temp_val.push_back(temp%10);
			temp/=10;
		}
		it=temp_val.end()-1;
		for(;it>=temp_val.begin();it--)
			fputc(*it+48,ofp);
		fputc(',',ofp);
		temp_val.clear();
		vector<int>().swap(temp_val);
	}
	//清空buff缓冲区
	for(int i=0;i<BUFF_MAXSIZE;i++)
		buff[i]='\0';
   //从源文件ifp中读取一个字节到c
    char c;
    fread(&c,1,1,ifp);
    while(!feof(ifp))
    {
    	int pos;
    	//找到哈夫曼树中字符对应的下标
    	for(int i=0;i<bytes_count;i++)
		{
			if(HaffTree[i].byte==c)
				pos=i;
		}
    	int len=strlen(buff);
    	//以256为阀值，一旦溢出优先进行写入操作
    	if(len+strlen(HaffTree[pos].code)<=256)
    	{
    		 //将字符对应哈夫曼编码加入buff
    		for(int k=0;k<strlen(HaffTree[pos].code);k++)
				buff[len++]=HaffTree[pos].code[k];
		}
    	else
    	{
			int x=strlen(buff)/8;
			int p=0,ch=0;
			for(int i=1;i<=x;i++)
			{
				ch=0;
				p=8*(i-1);
				for(int j=7;p<8*i;p++,--j)
				{
					if(buff[p]=='1')
						ch+=pow(2,j);
				}
				fputc(ch,ofp);
			}
		}
		fread(&c,1,1,ifp);
	}
	//判断操作次数
	int x=strlen(buff)/8;
	int p=0,ch=0;
	for(int i=1;i<=x;i++)
	{
		ch=0;
		p=8*(i-1);
		for(int j=7;p<8*i;p++,--j)
		{
			if(buff[p]=='1')
				ch+=pow(2,j);
		}
		fputc(ch,ofp);
	}
	//buff不一定整除8，以下程序处理剩余buff
	ch=0;
	for(int j=7;p<strlen(buff);j--)
	{
		if(buff[p++]=='1')
			ch+=pow(2,j);
	}
	fputc(ch,ofp);
	fclose(ofp);
	fclose(ifp);
}

/*
*生成解压文件
*邱子豪：392-491编码，测试
*陈淑婷：测试
*/
void deCompressFile()
{
	ifstream ifp;
	ofstream ofp;
	ifp.open(compress_filename);
	ofp.open(decompress_filename);
	string all,temp;
	vector<string> h;
	//读取压缩文件全部内容
    ostringstream os;
	os<<ifp.rdbuf();
    all=os.str();
	//将文件内容按','分开存储
	stringstream data(all);
	while(getline(data,temp,','))
		h.push_back(temp);
	//获取扩展名长度
	char t0[1];
	t0[0]=h[0][0];
	head.suffixlength=atoi(t0);
	//获取扩展名
	for(int i=0;i<head.suffixlength;i++)
		head.suffixname[i]=h[1][i];
	//获取字节数
	char t[64];
	for(int i=0;i<h[2].length();i++)
		t[i]=h[2][i];
	head.bytesnum=atoi(t);
	memset(t,0,sizeof(t)/sizeof(char));
	//获取字符数
	for(int i=0;i<h[3].length();i++)
		t[i]=h[3][i];
	head.codenum=atoi(t);
	memset(t,0,sizeof(t)/sizeof(char));
	//以下根据前面得到的字符数，读取字符及其字符数，构建哈夫曼
	int begin=4;
	initHaffTree();
	for(int i=0;i<head.codenum;i++)
	{
		if(h[begin].length()==2)
			head.byte[i]=',';
		else
			head.byte[i]=h[begin][0];
		for(int j=0;j<h[begin+1].length();j++)
			t[j]=h[begin+1][j];
		head.weight[i]=atoi(t);
		memset(t,0,sizeof(t)/sizeof(char));
		HaffTree[i].byte=head.byte[i];
		HaffTree[i].weight=head.weight[i];
		begin+=2;
	}
	//读取正文
	vector<int> val;
	for(int i=0;i<h[begin].length();i++)
		val.push_back(((int)h[begin][i]>=0)?(int)h[begin][i]:(int)h[begin][i]+256);
	creatHaffTree();
	creatHaffCode();
	//将正文内容还原为01字符串
	string mother="",end="";
	for(int i=0;i<val.size();i++)
	{
		int x=val[i],tem=0,j=7;
		while(j>=0)
		{
			tem=x/pow(2,j);
			if(tem!=0)
				mother+='1';
			else
				mother+='0';
			x-=tem*pow(2,j);
			j--;
		}
	}
	//根据01字符串及所构造哈夫曼树得到的映射，进行文件还原
	map<string,char> p;
	for(int i=0;i<head.codenum;i++)
	{
		string p2(HaffTree[i].code);
		p[p2]=HaffTree[i].byte;
	}
	int q1=0,q2=1;
	int num=0;
	while(num<head.bytesnum)
	{
		string m=mother.substr(q1,q2);
		if(p.find(m)==p.end())
			q2++;
		else
		{
			end+=p[m];
			num++;
			q1+=q2;
			q2=1;
		}
	}
	//还原文件写入
	ofp<<end;
	ifp.close();
	ofp.close();
}

/*
*主函数
*邱子豪：498-551编码，测试
*陈淑婷：498-551编码，测试
*/
int main()
{
	string str,str2;
	bool flag=true;
	do
	{
		if(!flag) cout<<"文件路径或文件后缀不符合规范！请重新输入!"<<endl, system("pause"), system("cls"), flag=true;
		cout<<"请输入文件路径：";
		cin>>str;
		cout<<"请输入文件后缀名：";
		cin>>str2; 
		flag=false;
	}while(str.length()>(FILENAMEMAX-SUFFIXNAMEMAX)||str2.length()>SUFFIXNAMEMAX);
	int j=0;
	decompress_filename[0]='d', decompress_filename[1]='e'; 
	for(int i=0;i<str.length();i++) source_filename[j]=str[i], decompress_filename[j+2]=str[i], compress_filename[j++]=str[i] ;
	int k;
	compress_filename[k=j]='.';
	compress_filename[++k]='b';
	compress_filename[++k]='u';
	compress_filename[++k]='f';
	source_filename[j++]='.';
	for(int i=0;i<str2.length();i++) source_filename[j++]=str2[i];
	 
	//读取文件，构造哈夫曼 
	initHaffTree();
	
	Statistics(source_filename);
	Sortweights(HaffTree,255);
	creatHaffTree();
	creatHaffCode();
	
	//压缩 
	cout<<"压缩中"<<endl;
	writeCompressFile();
	cout<<"压缩完成，压缩文件为"<<compress_filename<<endl;
	
	FILE *ifp=fopen(source_filename,"r");
	fseek(ifp,0L,SEEK_END);
	long size1=ftell(ifp);
	fclose(ifp);
	
	FILE *ifp2=fopen(compress_filename,"r");
	fseek(ifp2,0L,SEEK_END);
	long size2=ftell(ifp2);
	fclose(ifp2);
	
	cout<<"压缩率为："<<((float)size2/size1)*100<<"%"<<endl;
	//解压 
	cout<<"解压中..."<<endl; 
	deCompressFile();
	cout<<"解压完成，解压文件为"<<decompress_filename<<endl;
	return 0;
}

