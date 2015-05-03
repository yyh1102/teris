#include <stdio.h>
#include <graphics.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define delta 1


/* 改变了游戏规则：允许出现相同首字母，优先消除最低位置的含有该首字母的单词*/

struct WORD
{
	char word[20];
};

struct WORD *point;
struct WORD rec[25];
struct WORD words[25]={{"adobe"},{"boxheadler"},{"camel"},{"document"},{"ear"},{"football"},{"groin"},{"handcase"},{"interest"},
{"jacket"},{"knee"},{"lugage"},{"month"},{"nose"},{"optimistic"},{"palm"},{"quit"},{"random"},{"sky"},
	{"uncle"},{"vedio"},{"whereabouts"},{"xalloy"},{"young"},{"zero"}};
int i,j,k=0,record;
int screen_words=0,score=0;
int x,y,wordx=0,wordy,wordX[25],index[25];
int queue[24];
int f=0,ran;




int check(int n,int *index){
	int t;
	if (index[n]!=0)
	{
		return 1;
	}
	else
		return 0;
}

void readWords(){
	char *p;
	srand(time(0));
	i=rand()%25;
	ran=rand()%3;
	if(ran==1)
	{
		return;
	}
	while(check(i,wordX)){
		i=rand()%25;
	}
	x=rand()%(_width-strlen(words[i].word));
	while(x==wordx){
		x=rand()%(_width-strlen(words[i].word));
	}
	p=_vp+x*2;
	for (j=0;j<strlen(words[i].word) ;j++ )
	{
		*p=words[i].word[j];
		*(p+1)=(BLACK<<4) | WHITE;
		p+=2;
	}
	if(screen_words<24)
	   screen_words++;
	wordx=x;
	wordX[i]=x;
	k++;
	if(k>24)
	{
		memset(wordX,0,sizeof(wordX));
		k=0;
	}
}

void move_words_down(){
	char *p;
	int xx,yy;
	for (xx=0;xx<_width ;xx++ )
	{
		p=_vp+(23*_width+xx)*2;
		*p=0;
		*(p+1)=0;
	}
	for (yy=23;yy>=0;yy-- )
	{
		for (xx=0;xx<_width ;xx++ )
		{
			p=_vp+(yy*_width+xx)*2;
			if(*p!=0)
			{
				*(p+_width*2)=*p;
			    *(p+_width*2+1)=*(p+1);
				*p=0;
				*(p+1)=0;
			}
		}
	}
}

char *getPointer(char key){
	int wx,wy;
	char *pb;
	for (wy=screen_words+1 ;wy>=0 ;wy-- )
	{
	
	   for (wx=0;wx<_width ;wx++ )
	   {
		pb=_vp+(wy*_width+wx)*2;
		if(*pb==key && (wx==0 || *(pb-2)==0))
			return pb;
		else
			continue;
	   }
	}
	return 0;
}

void clearColor(char *po)
{
	while(*po!=0)
	{
		po-=2;
		if(*po==0)
			break;
		*(po+1)=(BLACK<<4) | WHITE;
	}
}

void clear(char *po)
{
	while(*po!=0)
	{
		*po=0;
		po-=2;
	}
}

void setscore(){
	char *scorep=_vp+(24*_width+1)*2;
	char sco[10];
	int length,i;
	itoa(score,sco,10);
	length=strlen(sco);
	*scorep='s';
	*(scorep+2)='c';
	*(scorep+4)='o';
	*(scorep+6)='r';
	*(scorep+8)='e';
	*(scorep+10)=':';
	scorep+=12;
	for(i=0;i<length;i++)
	{
		*scorep=sco[i];
		scorep+=2;
	}
}
	


int main()
{
	int key;
	long t0,n=0;
	int m,flag=0,stop=0;
	char *record;
	randomize();
	t0=time(NULL);
	clrscr();
	setscore();
	while(!stop)
	{
		move_words_down();
		readWords();
		while((long)time(NULL)-t0<=delta*n)
		{
			if(bioskey(1)!=0)
			{
				key=bioskey(0) & 0xFF;
			    if(key==0x1B)//Esc
			   {
				  stop=1;
				  break;
		     	}
			    if(flag==1)
		     	{
				    if(key==*record)
				   {
				    		*(record+1)=(BLACK<<4) | RED;
							record+=2;				
						 if(*record==0 && *(record-2)!=0){
							clear(record-2);
							flag=0;
							record=0;
							score++;
							setscore();
							continue;
							}
					}
					else
					{
						
						clearColor(record);
						flag=0;
						record=0;
						continue;
					}
				}
				if(flag==0 && (record=getPointer(key))!=0)
				{
					*(record+1)=(BLACK<<4) | RED;
					flag=1;
					record+=2;
				}
			}	
		}
		n++;
		record+=_width*2;
		setscore();
	}
	clrscr();
	return 1;
}