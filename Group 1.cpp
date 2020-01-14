/*
����������������������
|�����������ѹ���ļ�|
|�����ܳ��ȣ�543��   |
|���Ӻ�����272��,50% |
|�����ã���241��,44% |
����������������������
*/

/*
*ͷ�ļ����ṹ�嶨��
*���Ӻ�������
*�����ã�15-62����
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
	unsigned char byte;//�ֽ�Դ��
	long long value;//�ֽ�Ƶ��
}weight;
weight weights[256];//��¼256���ַ���Ƶ��

struct HaffNode//���������Ľ��ṹ
{
	unsigned char byte;//�������
	long long weight;//Ȩֵ
	int parent;//˫�׽���±�
	int leftChild;//�����±�
	int rightChild;//�Һ����±�
	char code[256];//����������
	int code_len;//���볤��
};
HaffNode HaffTree[511];//��������

struct heads//ͷ����Ϣ
{
	int suffixlength;//��׺����
	char suffixname[128];//��׺��
	int bytesnum;//�������ֽ�����
	int codenum;//��������
	char byte[256];//�ֽ�
	int weight[256];//�ֽ�Ƶ��
};
heads head;

int bytes_count=0;//����������Ҷ�ӽ����
long long file_length=0;//�����ļ����ֽ�������ʼΪ0
const int FILENAMEMAX=127;//�ļ���������·���������Ȳ�����127
const int SUFFIXNAMEMAX=9;//�ļ���׺�������Ȳ�����9���ַ�
char source_filename[FILENAMEMAX];//Դ�ļ�����
char compress_filename[FILENAMEMAX];//ѹ���ļ�����
char decompress_filename[FILENAMEMAX];//��ѹ�ļ�����

const int BUFF_MAXSIZE=1024*1024;//�ļ����������뻺������С
char buff[BUFF_MAXSIZE]="";//�ļ����������뻺����������ѹ���ļ�ʱ�����ֽڵı����ȴ���buff�У�Ȼ��8����ѹ����һ���ֽڴ浽buff_string�������У�

/*
*ͳ���ļ����ֽڵ�Ƶ��
*���Ӻ�������
*�����ã�69-88����
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
*��������Ԫ�صıȽϷ�ʽ
*���Ӻ�������
*�����ã�95-98����
*/
bool compare(HaffNode a,HaffNode b)
{
	return a.weight>b.weight;  //����
}

/*
*�����ֽ�Ƶ������
*���Ӻ�������
*�����ã�105-108����
*/
void Sortweights(HaffNode HaffTree[],int bytescount=255)
{
    sort(HaffTree,HaffTree+bytescount,compare);
}

/*
*��ʼ�����������洢�ṹ
*���Ӻ�������
*�����ã�115-126����
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
*ͨ���ϲ����������������
*���Ӻ�������
*�����ã�133-162����
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
*�������������
*���Ӻ�������
*�����ã�169-181����
*/
void creatHaffCode()
{
	int root=bytes_count*2-2;
	HaffTree[root].code[0]='\0';
	for(int i=root-1;i>=0;i--)
	{
		strcpy(HaffTree[i].code, HaffTree[HaffTree[i].parent].code);
		if(i==HaffTree[HaffTree[i].parent].rightChild)//�������ϱ�������
			strcat(HaffTree[i].code,"1");
		else
			strcat(HaffTree[i].code,"0");
	}
}

/*
*���������
*���Ӻ�������
*�����ã�188-204����
*/
void PrintSort(int count)
{
	printf("���������Ĵ洢�ṹ���£�\n");
	printf("  No\tByte\tWeight\tLchild\tRchild\tParent\tCode\n");
	for(int i=0;i<count;i++)
	{
		printf("  %d\t%x\t %d\t %d\t %d\t %d\t %s\n",i,HaffTree[i].byte,
		HaffTree[i].weight,HaffTree[i].leftChild,HaffTree[i].rightChild,
		HaffTree[i].parent,HaffTree[i].code);//ʮ���������Ϊ%x
	}
	for(int i=count;i<2*count-1;i++)//Ҷ�ӽ����count����˫�׽ڵ���count-1��
	{
		printf("  %d\t0\t %d\t %d\t %d\t %d\t %s\n",i,
		HaffTree[i].weight,HaffTree[i].leftChild,HaffTree[i].rightChild,
		HaffTree[i].parent,HaffTree[i].code);
	}
}

/*
*����ѹ���ļ�
*���Ӻ���265-385���룬����
*�����ã�211-263���룬����
*/
void writeCompressFile()
{
	string s(source_filename);
	string filehead,filetail;
	int index=s.find('.');//�洢'.'��λ��
	filehead=s.substr(0,index);
	filetail=s.substr(index+1);
	FILE *ifp=fopen(source_filename,"r");
	FILE *ofp=fopen(compress_filename,"w");
	head.suffixlength=filetail.length();//��չ������
	for(int i=0;i<filetail.length();i++)
		head.suffixname[i]=filetail[i];//��չ��
	head.bytesnum=file_length;//�ֽ�����
	head.codenum=bytes_count;//�������ֽ���
	for(int i=0;i<bytes_count;i++)
	{
		head.byte[i]=HaffTree[i].byte;//�ֽ�
		head.weight[i]=HaffTree[i].weight;//�ֽ�Ƶ��
	}
	fputc(head.suffixlength+48,ofp);//��'0'��ASCII�뿪ʼ�����ַ�
	fputc(',',ofp);
	fprintf(ofp,head.suffixname);//��ʽ��������ļ���
	fputc(',',ofp);
//	//�Ӹ�λ����ʼ��int���ַ�����תΪchar�ʹ����ļ���
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
//	//�Ӹ�λ����ʼ��int�ʹ������ַ�����תΪchar�ʹ����ļ���
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

	//д���ֽ���
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
	//�������
	temp_val.clear();
	//�ͷ��ڴ�
	vector<int>().swap(temp_val);
	//д���ַ���
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
	//д���Ӧ�ַ��������ַ�
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
	//���buff������
	for(int i=0;i<BUFF_MAXSIZE;i++)
		buff[i]='\0';
   //��Դ�ļ�ifp�ж�ȡһ���ֽڵ�c
    char c;
    fread(&c,1,1,ifp);
    while(!feof(ifp))
    {
    	int pos;
    	//�ҵ������������ַ���Ӧ���±�
    	for(int i=0;i<bytes_count;i++)
		{
			if(HaffTree[i].byte==c)
				pos=i;
		}
    	int len=strlen(buff);
    	//��256Ϊ��ֵ��һ��������Ƚ���д�����
    	if(len+strlen(HaffTree[pos].code)<=256)
    	{
    		 //���ַ���Ӧ�������������buff
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
	//�жϲ�������
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
	//buff��һ������8�����³�����ʣ��buff
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
*���ɽ�ѹ�ļ�
*���Ӻ���392-491���룬����
*�����ã�����
*/
void deCompressFile()
{
	ifstream ifp;
	ofstream ofp;
	ifp.open(compress_filename);
	ofp.open(decompress_filename);
	string all,temp;
	vector<string> h;
	//��ȡѹ���ļ�ȫ������
    ostringstream os;
	os<<ifp.rdbuf();
    all=os.str();
	//���ļ����ݰ�','�ֿ��洢
	stringstream data(all);
	while(getline(data,temp,','))
		h.push_back(temp);
	//��ȡ��չ������
	char t0[1];
	t0[0]=h[0][0];
	head.suffixlength=atoi(t0);
	//��ȡ��չ��
	for(int i=0;i<head.suffixlength;i++)
		head.suffixname[i]=h[1][i];
	//��ȡ�ֽ���
	char t[64];
	for(int i=0;i<h[2].length();i++)
		t[i]=h[2][i];
	head.bytesnum=atoi(t);
	memset(t,0,sizeof(t)/sizeof(char));
	//��ȡ�ַ���
	for(int i=0;i<h[3].length();i++)
		t[i]=h[3][i];
	head.codenum=atoi(t);
	memset(t,0,sizeof(t)/sizeof(char));
	//���¸���ǰ��õ����ַ�������ȡ�ַ������ַ���������������
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
	//��ȡ����
	vector<int> val;
	for(int i=0;i<h[begin].length();i++)
		val.push_back(((int)h[begin][i]>=0)?(int)h[begin][i]:(int)h[begin][i]+256);
	creatHaffTree();
	creatHaffCode();
	//���������ݻ�ԭΪ01�ַ���
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
	//����01�ַ�������������������õ���ӳ�䣬�����ļ���ԭ
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
	//��ԭ�ļ�д��
	ofp<<end;
	ifp.close();
	ofp.close();
}

/*
*������
*���Ӻ���498-551���룬����
*�����ã�498-551���룬����
*/
int main()
{
	string str,str2;
	bool flag=true;
	do
	{
		if(!flag) cout<<"�ļ�·�����ļ���׺�����Ϲ淶������������!"<<endl, system("pause"), system("cls"), flag=true;
		cout<<"�������ļ�·����";
		cin>>str;
		cout<<"�������ļ���׺����";
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
	 
	//��ȡ�ļ������������ 
	initHaffTree();
	
	Statistics(source_filename);
	Sortweights(HaffTree,255);
	creatHaffTree();
	creatHaffCode();
	
	//ѹ�� 
	cout<<"ѹ����"<<endl;
	writeCompressFile();
	cout<<"ѹ����ɣ�ѹ���ļ�Ϊ"<<compress_filename<<endl;
	
	FILE *ifp=fopen(source_filename,"r");
	fseek(ifp,0L,SEEK_END);
	long size1=ftell(ifp);
	fclose(ifp);
	
	FILE *ifp2=fopen(compress_filename,"r");
	fseek(ifp2,0L,SEEK_END);
	long size2=ftell(ifp2);
	fclose(ifp2);
	
	cout<<"ѹ����Ϊ��"<<((float)size2/size1)*100<<"%"<<endl;
	//��ѹ 
	cout<<"��ѹ��..."<<endl; 
	deCompressFile();
	cout<<"��ѹ��ɣ���ѹ�ļ�Ϊ"<<decompress_filename<<endl;
	return 0;
}

