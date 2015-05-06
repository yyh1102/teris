#include <stdio.h>
#include <graphics.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#define BSIZE 32
#define UP 0x4800
#define DOWN 0x5000
#define LEFT 0x4B00
#define RIGHT 0x4D00
#define ESC 0x011B
#define SPACE 0x3920
#define STEP 32
#define dtime 1


typedef struct 
{
	int x,y;
}point;

typedef struct
{
	point p[4];
}SHAPE;

typedef struct
{
	int shape,rotate,color;
	int x,y;
}block;

SHAPE block_shape[7][4]={
	{//É½×ÖÐÍ
		{{{1,0}, {1,1}, {0,1}, {2,1}}},
		{{{0,1}, {1,1}, {1,2}, {1,0}}},
		{{{1,2}, {1,1}, {2,1}, {0,1}}},
		{{{2,1}, {1,1}, {1,0}, {1,2}}}
   },

   {//½×ÌÝ
	   {{{0,0}, {1,0}, {1,1}, {2,1}}},
	   {{{0,2}, {0,1}, {1,1}, {1,0}}},
	   {{{2,2}, {1,2}, {1,1}, {0,1}}},
	   {{{2,0}, {2,1}, {1,1}, {1,2}}}
   },

   {//½×ÌÝ
	   {{{0,1}, {1,1}, {1,0}, {2,0}}},
	   {{{1,0}, {1,1}, {2,1}, {2,2}}},
	   {{{0,2}, {1,2}, {1,1}, {2,1}}},
	   {{{0,0}, {0,1}, {1,1}, {1,2}}}
   },

   {//L
	   {{{0,0}, {0,1}, {0,2}, {1,0}}},
	   {{{0,0}, {1,0}, {2,0}, {2,1}}},
	   {{{2,0}, {2,1}, {2,2}, {1,2}}},
	   {{{0,1}, {0,2}, {1,2}, {2,2}}}
   },

   {//L
	   {{{0,0}, {0,1}, {0,2}, {1,2}}},
	   {{{0,0}, {0,1}, {1,0}, {2,0}}},
	   {{{1,0}, {2,0}, {2,1}, {2,2}}},
	   {{{0,2}, {1,2}, {2,2}, {2,1}}}
   },

   {//Ìï
	   {{{0,0}, {1,0}, {1,1}, {0,1}}},
	   {{{0,0}, {1,0}, {1,1}, {0,1}}},
	   {{{0,0}, {1,0}, {1,1}, {0,1}}},
	   {{{0,0}, {1,0}, {1,1}, {0,1}}}
   },

   {//Ò»×Ö
	   {{{0,0}, {1,0}, {2,0}, {3,0}}},
	   {{{3,0}, {3,1}, {3,2}, {3,3}}},
	   {{{0,3}, {1,3}, {2,3}, {3,3}}},
	   {{{0,0}, {0,1}, {0,2}, {0,3}}}
   }
};

int X0=300,Y0=60,score=0,stop=0;
int all_color[7]={RED,YELLOW,BLUE,GREEN,BROWN,MAGENTA,CYAN};
int map_check[10][20]={0};
char ss[20];



void init_block(block *p)
{
	srand(time(0));
	p->shape=rand()%7;
	p->rotate=0;
	p->color=all_color[p->shape];
	p->x=3;
	p->y=0;
}


void draw_map()
{
	setfillstyle(SOLID_FILL,DARKGRAY);
	bar(X0-1,Y0-1,X0+320,Y0+640);
	setcolor(YELLOW);
	rectangle(X0-1,Y0-1,X0+320,Y0+640);
	setcolor(WHITE);
	outtextxy(X0+400,Y0+320,"score:");
}


void show_block(block t)
{
	int x,y,i,x1,y1;
	for (i=0;i<4;i++)
	{
		x1=t.x+block_shape[t.shape][t.rotate].p[i].x;
		y1=t.y+block_shape[t.shape][t.rotate].p[i].y;
		x=X0+x1*STEP;
		y=Y0+y1*STEP;
		map_check[x1][y1]=1;
		setfillstyle(SOLID_FILL,t.color);
		bar(x,y,x+BSIZE-1,y+BSIZE-1);
		setcolor(BLACK);
		rectangle(x,y,x+BSIZE-1,y+BSIZE-1);
	}
}

void clear_block(block t)
{
	int x,y,i,x1,y1;
	for (i=0;i<4 ;i++ )
	{
		x1=t.x+block_shape[t.shape][t.rotate].p[i].x;
		y1=t.y+block_shape[t.shape][t.rotate].p[i].y;
		x=X0+x1*STEP;
		y=Y0+y1*STEP;
		map_check[x1][y1]=0;
		setfillstyle(SOLID_FILL,DARKGRAY);
		bar(x,y,x+BSIZE-1,y+BSIZE-1);
		setcolor(DARKGRAY);
		rectangle(x,y,x+BSIZE-1,y+BSIZE-1);
	}
}

int check_xy(block t)
{
	int x,y,i,left,right,bottom,n,sbottom;
	left=right=block_shape[t.shape][t.rotate].p[0].x;
	bottom=block_shape[t.shape][t.rotate].p[0].y;
	for (i=1;i<4 ;i++ )
	{
		if(block_shape[t.shape][t.rotate].p[i].x<left)
			left=block_shape[t.shape][t.rotate].p[i].x;
		if(block_shape[t.shape][t.rotate].p[i].x>right)
			right=block_shape[t.shape][t.rotate].p[i].x;
		if(block_shape[t.shape][t.rotate].p[i].y>bottom)
			bottom=block_shape[t.shape][t.rotate].p[i].y;
	}
	left=X0+(t.x+left)*STEP;
	right=X0+(t.x+right)*STEP;
	bottom=Y0+(t.y+bottom)*STEP;
	if(left<X0)
		return 1;
	if(right>=(X0+320))
		return 2;
	if(bottom>=(Y0+640))
		return 3;
	return 0;
}

int check_collision(block t)
{
	int x1,y1,i;
	for (i=0;i<4 ;i++ )
	{
		x1=t.x+block_shape[t.shape][t.rotate].p[i].x;
		y1=t.y+block_shape[t.shape][t.rotate].p[i].y;
		if(map_check[x1][y1]==1)
			return 1;
	}
	return 0;
}

int cover()
{
	int i,j,n=0,time=0;
	for (i=19;i>=0 ;i-- )
	{
		time=0;
		for (j=0;j<10 ;j++ )
		{
			if(map_check[j][i]==1)
				time++;
			if(time==10)
				return i;
		}
	}
	return 0;
}



void delete_last(int bottom)
{
	int i,j,time=0;
	struct picture *p;
	p = (struct picture *)malloc(imagesize(X0,Y0,X0+320-2,Y0+(bottom)*STEP-1));
	if(p)
	{
		getimage(X0,Y0,X0+320-2,Y0+(bottom)*STEP-1,p);
		setfillstyle(SOLID_FILL,DARKGRAY);
		bar(X0,Y0,X0+320-1,Y0+STEP);
		putimage(X0,Y0+STEP,p,COPY_PUT);
	}
	for (i=bottom; i>0 ;i-- )
	{
		for (j=0; j<10 ;j++ )
		{
			map_check[j][i] = map_check[j][i-1];
		}
	}
}

void check_gameover()
{
	int i,j,time=0,flag=0;
	for (i=0;i<10 && flag==0;i++ )
	{
		if(map_check[i][0]==1)
			flag=1;
	}
	if(flag)
	{
		stop=1;
		setfillstyle(SOLID_FILL,BLACK);
		bar(X0+120,Y0+320,X0+220,Y0+340);
		setcolor(WHITE);
		outtextxy(X0+130,Y0+320,"GAME OVER");
		return;
	}
	else
		return;
}



void main()
{
	int driver=0,mode=VESA_1024x768x8bit,paused=0;
	char *p;
	block bl;
	int key,n=1;
	long t0;
	initgraph(&driver,&mode,"");
	randomize();
	memset(map_check,0,sizeof(map_check));
	draw_map();
	t0=time(NULL);
	init_block(&bl);
	itoa(score,ss,10);
	outtextxy(X0+470,Y0+320,ss);
	outtextxy(X0+397,Y0+220,"Press spacebar to pause");
	outtextxy(X0+397,Y0+270,"Press spacebar agian to continue");
	while(!stop)
	{
		show_block(bl);
		if(check_xy(bl)==3)
		{
			bl.y--;
			init_block(&bl);
			check_gameover();
			show_block(bl);
		}
		while(((long)time(NULL)-t0)<=n*dtime)
		{
			if(bioskey(1)!=0)
			{
				key=bioskey(0);
				clear_block(bl);
				switch(key)
				{
					case UP:bl.rotate++;break;
					case LEFT:bl.x--;break;
					case RIGHT:bl.x++;break;
					case DOWN:bl.y++;break;
					case ESC:stop=1;break;
					case SPACE:show_block(bl);paused=1;break;
					default:break;
				}
				while(paused){
					if(bioskey(1)!=0)
					{
						if(bioskey(0)==SPACE)
						{
							clear_block(bl);
							paused=0;
						    break;
						}
					}
				}
				if(bl.rotate>3 || bl.rotate<0)
					bl.rotate=0;
				if(check_xy(bl)==1)
					bl.x++;
				if(check_xy(bl)==2)
					bl.x--;
				if(check_xy(bl)==3)
				{
					bl.y--;
					check_gameover();
					show_block(bl);
					while(cover())
					{
						delete_last(cover());
						score+=100;
						setfillstyle(SOLID_FILL,BLACK);
						bar(X0+460,Y0+300,X0+520,Y0+340);
						itoa(score,ss,10);
						setcolor(WHITE);
						outtextxy(X0+470,Y0+320,ss);
					}
					init_block(&bl);
					check_gameover();
				}
				if(check_collision(bl))
				{
					if(key==UP)
						bl.rotate--;
					if(key==LEFT)
						bl.x++;
					if(key==RIGHT)
						bl.x--;
					if(key==DOWN)
					{
						bl.y--;
						check_gameover();
						show_block(bl);
						while(cover())
						{
							delete_last(cover());
							score+=100;
							setfillstyle(SOLID_FILL,BLACK);
							bar(X0+460,Y0+300,X0+520,Y0+340);
							itoa(score,ss,10);
							setcolor(WHITE);
							outtextxy(X0+470,Y0+320,ss);
						}
						init_block(&bl);
						check_gameover();
					}
				}
				check_gameover();
				show_block(bl);
			}
		}
		clear_block(bl);
		bl.y++;
		if(check_xy(bl)==3 || check_collision(bl))
		{
			bl.y--;
			check_gameover();
			show_block(bl);
			while(cover())
			{
				delete_last(cover());
				score+=100;
				setfillstyle(SOLID_FILL,BLACK);
				bar(X0+460,Y0+300,X0+520,Y0+340);
				itoa(score,ss,10);
				setcolor(WHITE);
				outtextxy(X0+470,Y0+320,ss);
			}
			init_block(&bl);
			check_gameover();
		}
		n++;
	}
	getchar();
	closegraph();
}


//Ð¡è¦´Ã
