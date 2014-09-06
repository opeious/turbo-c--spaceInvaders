#include<time.h>
#include<fstream.h>
#include<graphics.h>
#include<conio.h>
#include<ctype.h>
#include<dos.h>
#include<stdlib.h>
#include<stdio.h>
#include<io.h>
#include<string.h>




//global variables

int dropPotential=0;
char newScore='f';
int quit=1337;
char btn='*';
int mm=1;
char dswarm='r';




//function declarations

void init();
void end();
void append(char s[80],char c);
void loadingBar();
int mainMenu();
void instructions();
void start();
void keyboard();
void pause();
void defaultScores();
void readScores();
void drawScores();
void newHighscore();
void writeHighscores();




//structures

struct highscore
{
 char name[80];
 int score;
}h[10];









//classes

class actor
{
 public:
 int x,y;
 char alive;
};

class user_interface
{
 public:
 int XL,XR,YU,YD,lives,score,ammo,level,ui,cd;
 char cooling;
 user_interface()
 {
  XL=40,XR=632,YU=20,YD=460,lives=3,score=0,ammo=0,level=1,cd=0,cooling='f';
 }
 void render()
 {
  char score_render[15],lives_render[15],ammo_render[15],level_render[15];
  settextstyle(0,0,0);
  char heart[3]= " ";
  heart[0]=3;
  strcpy(lives_render,"Lives: ");
  for(int i=0;i<lives;i++)
   strcat(lives_render,heart);
  setcolor(CYAN);
  sprintf(score_render,"Score: %i",score);
  outtextxy(550,8,score_render);
  setcolor(RED);
  outtextxy(5,8,lives_render);
  setcolor(YELLOW);
  sprintf(ammo_render,"Ammo (missiles): %i",ammo);
  outtextxy(5,465,ammo_render);
  setcolor(10);
  sprintf(level_render,"Level: %i",level);
  outtextxy(300,8,level_render);
  setcolor(13);
  outtextxy(430,465,"C++ Project: Tapan Ranjan");
  setcolor(WHITE);
  rectangle(XL,YU,XR,YD);
  outtextxy(550,8,"Score: ");
  outtextxy(5,8,"Lives: ");
  outtextxy(5,465,"Ammo (missiles): ");
  outtextxy(300,8,"Level: ");
  outtextxy(430,465,"C++ Project: ");
  setcolor(YELLOW);
  rectangle(5,YU,30,YD);
  if(cd>YD-20)
   cooling='t';
  if(cd<6)
   cooling='f';
  if(cooling=='f')
   setcolor(GREEN);
  if(cooling=='t')
   setcolor(RED);
  if(cd>0)
   {
    for(i=0;i<cd;i++)
    {
     line(6,YD-1-i,29,YD-1-i);
    }
    cd-=5;
   }
  setcolor(WHITE);
  settextstyle(0,0,1);
  outtextxy(15,YU+15,"C");
  outtextxy(15,YU+75,"O");
  outtextxy(15,YU+135,"O");
  outtextxy(15,YU+195,"L");
  outtextxy(15,YU+255,"D");
  outtextxy(15,YU+315,"O");
  outtextxy(15,YU+375,"W");
  outtextxy(15,YU+430,"N");
 }
}ui;

class user:public actor
{
 public:
 user()
 {
  y=440;
  x=321;
  alive='t';
 }
 void render()
 {
  if(alive=='t')
  {
   int i;
   setcolor(GREEN);
   rectangle(x-8,y+4,x-7,y+12);
   rectangle(x+8,y+4,x+7,y+12);
   setcolor(RED);
   putpixel(x,y,GREEN);
   for(i=0;i<3;i++)
    line(x-10,y+10+i,x+10,y+10+i);
   for(i=0;i<3;i++)
    line(x-5,y+9-i,x+5,y+9-i);
   for(i=0;i<3;i++)
    line(x-2,y+4+i,x+2,y+4+i);
   setcolor(CYAN);
   for(i=1;i<4;i++)
    line(x-1,y+i,x+1,y+i);
  }
 }
}player;

class powerUp:public actor
{
 public:
 void move(int speed)
 {
  if(alive=='t')
   y+=speed;
 }
};

class life:public powerUp
{
 public:
 life()
 {
  x=0;
  y=0;
  alive='f';
 }
 void render()
 {
  if(alive=='t')
  {
   char heart[3]=" ";
   sprintf(heart,"%c",3);
   settextstyle(0,0,0);
   setcolor(RED);
   setfillstyle(1,YELLOW);
   fillellipse(x,y,7,7);
   outtextxy(x-3,y-3,heart);
   if(x>player.x-15&&x<player.x+15)
    if(y==player.y)
     {
      ui.lives++;
      alive='f';
     }
  }
 }
}lifeup;

class returnFire:public powerUp
{
 public:
 returnFire()
 {
  alive='f';
  x=0;
  y=0;
 }
 void render()
 {
  if(alive=='t')
   {
    setcolor(11);
    line(x,y,x,y-8);
   }
 }
}laser[5];

class bullet:public actor
{
 public:
 void move(int speed)
 {
  if(alive=='t')
   y-=speed;
 }
};

class user_laser:public bullet
{
 public:
 user_laser()
 {
  x=0,y=0,alive='f';
 }
 void render()
 {
  if(alive=='t')
  {
   setcolor(BROWN);
   rectangle(x,y,x,y+5);
  }
 }
 void shoot()
 {
  if(ui.cooling=='f')
  {
   x=player.x;
   y=player.y;
   alive='t';
   ui.cd+=30;
  }
 }
}single[20];

class missile:public bullet
{
 public:
 int x2,y2;
 char alive2;
 missile()
 {
  x=0,y=0,alive='f',x2=0,y2=0,alive2='f';
 }
 void render()
 {
  if(alive=='t')
   {
    setcolor(RED);
    rectangle(x,y,x+2,y+5);
    line(x+1,y+1,x+1,y+4);
    setcolor(YELLOW);
    line(x+1,y+6,x+1,y+8);
   }
  if(alive2=='t')
   {
    setcolor(RED);
    rectangle(x2,y2,x2+2,y2+5);
    line(x2+1,y2+1,x2+1,y2+4);
    setcolor(YELLOW);
    line(x2+1,y2+6,x2+1,y2+8);
   }
 }
 void shoot()
 {
  x=player.x-8;
  x2=player.x+8;
  y=player.y-3;
  y2=player.y-3;
  alive='t';
  alive2='t';
  ui.cd+=40;
 }
 void move()
 {
  if(alive=='t')
   y-=15;
  if(alive2=='t')
   y2-=15;
 }
}dual[10];

class enemy:public actor
{
 public:
 int frame,type;
 enemy()
 {
  x=ui.XL+10,y=ui.YU+10,alive='t',frame=1,type=1;
 }
 void declare(int i,int j)
 {
  x=x+(i*20);
  y=y+(j*20);
 }
 void move()
 {
  if(alive=='t')
  {
   if(dswarm=='r')
    x+=ui.level+3-1;
   if(dswarm=='l')
    x-=ui.level+3-1;
   int randomNumber=rand()%100+1,ex,ey;
   if(randomNumber==5&&alive=='t')
   for(int i=0;i<2+ui.level;i++)
    if(laser[i].alive=='f')
     {
      laser[i].x=x;
      laser[i].y=y;
      laser[i].alive='t';
      break;
     }

  }
 }
 void kill()
 {
  alive='f';
  ui.score++;
  ui.ammo++;
  int randomNumber;
  randomNumber=(rand()%15+1)-(dropPotential/10);
  if(randomNumber==2&&lifeup.alive=='f'&&ui.lives<8)
  {
   lifeup.alive='t';
   lifeup.x=x;
   lifeup.y=y;
   if(dropPotential>0)
    dropPotential=0;
   dropPotential=-10;
  }
  dropPotential++;
 }
 void render(int colour)
 {
  if(alive=='t')
  {
   setcolor(colour);
   if(frame<10)
   {
    if(type==1)
    {
     for(int i=1;i<3;i++)
     {
      putpixel(x-i,y,colour);
      putpixel(x+i,y,colour);
     }
     putpixel(x-3,y-1,colour);
     putpixel(x+3,y-1,colour);
     for(i=1;i<3;i++)
     {
      putpixel(x-5,y-i,colour);
      putpixel(x+5,y-i,colour);
     }
     for(i=0;i<4;i++)
     {
      putpixel(x-i,y-2,colour);
      putpixel(x+i,y-2,colour);
     }
     line(x-5,y-3,x+5,y-3);
     line(x-4,y-4,x+4,y-4);
     line(x-3,y-5,x+3,y-5);
     putpixel(x-3,y-7,colour);
     putpixel(x+3,y-7,colour);
     putpixel(x-2,y-6,colour);
     putpixel(x+2,y-6,colour);
     putpixel(x-2,y-4,BLACK);
     putpixel(x+2,y-4,BLACK);
    }
    if(type==2)
    {
     putpixel(x-3,y,colour);
     putpixel(x+3,y,colour);
     putpixel(x-4,y-1,colour);
     putpixel(x+4,y-1,colour);
     putpixel(x-3,y-2,colour);
     putpixel(x+3,y-2,colour);
     for(int i=2;i<8;i++)
      line(x-1,y-i,x+1,y-i);
     for(i=6;i>3;i--)
     {
      line(x-2,y-3,x-2,y-i);
      line(x+2,y-3,x+2,y-i);
      line(x-3,y-3,x-3,y-i);
      line(x+3,y-3,x+3,y-i);
     }
     putpixel(x-2,y-4,BLACK);
     putpixel(x+2,y-4,BLACK);
    }
    if(type==3)
    {
     putpixel(x-3,y,colour);
     putpixel(x+3,y,colour);
     putpixel(x-4,y,colour);
     putpixel(x+4,y,colour);
     putpixel(x-1,y-1,colour);
     putpixel(x+1,y-1,colour);
     putpixel(x-5,y-1,colour);
     putpixel(x+5,y-1,colour);
     putpixel(x+4,y-1,colour);
     putpixel(x-4,y-1,colour);
     for(int i=1;i<5;i++)
     {
      putpixel(x-i,y-2,colour);
      putpixel(x+i,y-2,colour);
     }
     putpixel(x,y-2,colour);
     for(i=3;i<6;i++)
      line(x-6,y-i,x+6,y-i);
     line(x-5,y-6,x+5,y-6);
     line(x-2,y-7,x+2,y-7);
     for(i=2;i<4;i++)
     {
      putpixel(x-i,y-4,BLACK);
      putpixel(x+i,y-4,BLACK);
     }
    }
   frame++;
   return;
  }
  else
  {
   if(type==1)
   {
    putpixel(x-4,y,colour);
    putpixel(x+4,y,colour);
    putpixel(x-3,y,colour);
    putpixel(x+3,y,colour);
    for(int i=0;i<3;i++)
     line(x-4,y-2-i,x+4,y-2-i);
    line(x-5,y-6,x-5,y-3);
    line(x+5,y-6,x+5,y-3);
    line(x-3,y-5,x+3,y-5);
    putpixel(x-3,y-7,colour);
    putpixel(x+3,y-7,colour);
    putpixel(x-2,y-6,colour);
    putpixel(x+2,y-6,colour);
    putpixel(x-2,y-4,BLACK);
    putpixel(x+2,y-4,BLACK);
   }
   if(type==2)
   {
    for(int i=0;i<5;i+=2)
    {
     putpixel(x-i,y,colour);
     putpixel(x+i,y,colour);
    }
    putpixel(x-1,y-1,colour);
    putpixel(x+1,y-1,colour);
    putpixel(x-3,y-1,colour);
    putpixel(x+3,y-1,colour);
    putpixel(x,y-2,colour);
    putpixel(x-2,y-2,colour);
    putpixel(x+2,y-2,colour);
    for(i=2;i<8;i++)
     line(x-1,y-i,x+1,y-i);
    for(i=6;i>3;i--)
    {
     line(x-2,y-3,x-2,y-i);
     line(x+2,y-3,x+2,y-i);
     line(x-3,y-3,x-3,y-i);
     line(x+3,y-3,x+3,y-i);
    }
    putpixel(x-2,y-4,BLACK);
    putpixel(x+2,y-4,BLACK);
   }
   if(type==3)
   {
    putpixel(x-5,y,colour);
    putpixel(x-6,y,colour);
    putpixel(x+5,y,colour);
    putpixel(x+6,y,colour);
    putpixel(x-1,y-1,colour);
    putpixel(x+1,y-1,colour);
    putpixel(x-3,y-1,colour);
    putpixel(x-4,y-1,colour);
    putpixel(x+3,y-1,colour);
    putpixel(x+4,y-1,colour);
    putpixel(x-2,y-2,colour);
    putpixel(x-3,y-2,colour);
    putpixel(x+2,y-2,colour);
    putpixel(x+3,y-2,colour);
    putpixel(x,y-2,colour);
    for(int i=3;i<6;i++)
     line(x-6,y-i,x+6,y-i);
    line(x-5,y-6,x+5,y-6);
    line(x-2,y-7,x+2,y-7);
    for(i=2;i<4;i++)
    {
     putpixel(x-i,y-4,BLACK);
     putpixel(x+i,y-4,BLACK);
    }
   }
   frame++;
   if(frame==20)
    frame=1;
   return;
  }
 }
 }
};





























//main function

int main()
{
 init();
 defaultScores();
 readScores();
 loadingBar();
 int choice;
 while(quit!=0)
 {
  choice=mainMenu();
  if(choice==3)
   {
    quit=0;
   }
  if(choice==2)
   {
    instructions();
    mainMenu();
   }
  if(choice==1)
   {
    mm=1;
    dropPotential=0;
    start();
   }
 }
 end();
 return 0 ;
}





//function definitions

void init()
{
 srand(time(0));
 int a=DETECT,b;
 initgraph(&a,&b,"C:\\TC\\BGI");
 cleardevice();
}

void end()
{
 cleardevice();
 closegraph();
}

void append(char s[80], char c)
{
 int len=strlen(s);
 s[len]=c;
 s[len+1]='\0';
}

void loadingBar()
{
 cleardevice();
 getch();
 setcolor(YELLOW);
 rectangle(200,150,400,350);
 int line_buffer=201;
 int color_dot,dot_x,dot_y=400;
 while(line_buffer<400)
	{
	 setcolor(GREEN);
	 line(line_buffer,151,line_buffer,349);
	 line_buffer++;
	 delay(40);
	 do
	 {
	  color_dot=rand() % 16;
	  dot_x=rand()%640+1;
	  dot_y=rand()%480+1;
	 }while((dot_x>190&&dot_x<410)&&(dot_y>140)&&dot_y<360);
	 setcolor(color_dot);
	 circle(dot_x,dot_y,1);
	 setcolor(color_dot+1);
	 circle(dot_x+5,dot_y+5,1);
	 setcolor(color_dot+2);
	 circle(dot_x-5,dot_y-5,1);
	 setcolor(BLACK);
	 settextstyle(4,0,5);
	 outtextxy(215,160,"Space");
	 outtextxy(245,220,"Invaders");
	 settextstyle(0,0,0);
	 outtextxy(220,300,"Made by: Tapan Ranjan");
	 outtextxy(220,310,"Class  : XII - F");
	}
 setcolor(WHITE);
 getch();
 cleardevice();
}

int mainMenu()
{
 cleardevice();
 char buttonPress='*';
 char grafic[5];
 sprintf(grafic,"%c",16);
 drawScores();
 settextstyle(0,0,2);
 setcolor(WHITE);
 outtextxy(350,100,"Start Game");
 outtextxy(350,250,"Instructions");
 outtextxy(350,400,"Exit");
 setcolor(BLUE);
 outtextxy(300,100,grafic);
 int selected=1,xs,ys,cs,rs,selector=1;
 while(selected!=0)
 {
  do
  {
   xs=rand()%640+1;
   ys=rand()%480+1;
   cs=rand()%16;
   rs=rand()%10+1;
  }while((xs<600&&xs>40)&&(ys<460)&&(ys>20));
  setcolor(cs);
  circle(xs,ys,rs);
  delay(5);
  if(kbhit())
  {
   setcolor(BLACK);
   outtextxy(300,100,grafic);
   outtextxy(300,250,grafic);
   outtextxy(300,400,grafic);
   buttonPress=getch();
   if(buttonPress==0)
    buttonPress=getch();
   if(buttonPress==27)
    {selected=0;quit=0;}
   if(buttonPress==80&&selector<3)
    selector++;
   if(buttonPress==72&&selector>1)
    selector--;
   setcolor(BLUE);
   if(selector==1)
    outtextxy(300,100,grafic);
   if(selector==2)
    outtextxy(300,250,grafic);
   if(selector==3)
    outtextxy(300,400,grafic);
   if(buttonPress=='\r')
    selected=0;
  }
 }
 return selector;
}

void instructions()
{
 setcolor(WHITE);
 settextstyle(0,0,0);
 cleardevice();
 outtextxy(75,40,"Press ESC anytime during the game to PAUSE the game");
 outtextxy(75,65,"Scores of Highest Scorers will be Saved");
 outtextxy(75,90,"Kill Aliens during the game to gain Ammo for the missiles");
 outtextxy(315,150,"Player");
 outtextxy(75,150,"Controls");
 gotoxy(10,27);
 line(63,165,545,165);
 line(63,167,545,167);
 outtextxy(90,200,"LEFT");
 outtextxy(310,200,"LEFT_ARROW");
 outtextxy(90,245,"RIGHT");
 outtextxy(310,245,"RIGHT_ARROW");
 outtextxy(90,290,"SHOOT");
 outtextxy(310,290,"SPACE BAR");
 outtextxy(90,335,"DOUBLE GUN");
 outtextxy(310,335,"Z");
 int text_color,dot_x,dot_y;
 rectangle(61,30,547,350);
 while(!kbhit())
 {
  settextstyle(1,0,0);
  text_color=rand()%16;
  do
  {
   dot_x=rand()%640+1;
   dot_y=rand()%480+1;
  }while(((dot_x>56)&&(dot_x<552))&&((dot_y>25)&&(dot_y<355)));
  setcolor(text_color);
  outtextxy(120,375,"Thank You for Playing");
  circle(dot_x,dot_y,rand()%4);
 }
 cleardevice();
 setcolor(WHITE);
}

void start()
{
 dswarm='r';
 enemy alien[10][8];
 lifeup.alive='f';
 for(int ax=0;ax<10;ax++)
  for(int ay=0;ay<8;ay++)
   {
    alien[ax][ay].declare(ax,ay);
    alien[ax][ay].type=ay%3+1;
   }
 for(int i=0;i<20;i++)
 {
  dual[i].alive='f';
  dual[i].alive2='f';
  if(i<10)
   single[i].alive='f';
  laser[i].alive='f';
 }
 while(mm!=0)
  {
   cleardevice();
   ui.render();
   player.render();
   lifeup.render();
   lifeup.move(5);
   if(lifeup.y>ui.YD)
    lifeup.alive='f';
   for(int ay=0;ay<8;ay++)
    for(int ax=0;ax<10;ax++)
     {
      alien[ax][ay].move();
      alien[ax][ay].render(ay+1);
      for(int i=0;i<20;i++)
      {
       if(single[i].alive=='t'&&alien[ax][ay].alive=='t')
       if(alien[ax][ay].x>single[i].x-5&&alien[ax][ay].x<single[i].x+5)
       if(alien[ax][ay].y>single[i].y)
	{
	 single[i].alive='f';
	 single[i].x=1;
	 single[i].y=1;
	 alien[ax][ay].kill();
	}
       if(i<10)
	{
	 if(dual[i].alive=='t'&&alien[ax][ay].alive=='t')
	 if(alien[ax][ay].x>dual[i].x-5&&alien[ax][ay].x<dual[i].x+5)
	 if(alien[ax][ay].y>dual[i].y)
	   {
	    alien[ax][ay].kill();
	    dual[i].alive='f';
	   }
	 if(dual[i].alive2=='t'&&alien[ax][ay].alive=='t')
	  if(alien[ax][ay].x>dual[i].x2-5&&alien[ax][ay].x<dual[i].x2+5)
	   if(alien[ax][ay].y>dual[i].y2)
	   {
	    alien[ax][ay].kill();
	    dual[i].alive2='f';
	   }
      }
     }
  }
  if(kbhit())
   keyboard();
  for(int i=0;i<20;i++)
  {
   single[i].render();
   single[i].move(10);
   if(single[i].y<ui.YU)
    single[i].alive='f';
   if(i<10)
   {
    dual[i].render();
    dual[i].move();
    if(dual[i].y<ui.YU)
     {dual[i].alive='f';dual[i].alive2='f';}
   }
  }
  int aliensKilled=0;
  for(ax=0;ax<10;ax++)
   for(ay=0;ay<8;ay++)
    {
     if(alien[ax][ay].alive=='f')
      aliensKilled++;
     if(alien[ax][ay].y>player.y-1&&alien[ax][ay].alive=='t')
      {
       ui.lives--;
       start();
      }
     if(alien[ax][ay].alive=='t'&&alien[ax][ay].x+5>ui.XR-2&&dswarm=='r')
      {
       dswarm='l';
       for(int tx=0;tx<10;tx++)
	for(int ty=0;ty<8;ty++)
	 alien[tx][ty].y+=30;
       break;
      }
     if(alien[ax][ay].alive=='t'&&alien[ax][ay].x-5<ui.XL+2&&dswarm=='l')
      {
       dswarm='r';
       for(int tx=0;tx<10;tx++)
	for(int ty=0;ty<8;ty++)
	 alien[tx][ty].y+=30;
       break;
      }
     if(aliensKilled==80)
      {
       ui.level++;
       aliensKilled=0;
       for(int i=0;i<20;i++)
	{
	 single[i].alive='f';
	 if(i<10)
	  dual[i].alive='f';
	}
       player.x=321;
       player.y=440;
       cleardevice();
       ui.render();
       player.render();
       char levelup[80];
       sprintf(levelup,"Congratulations you have completed level: %i",(ui.level-1));
       setcolor(WHITE);
       rectangle(140,230,500,250);
       setcolor(ui.level);
       outtextxy(150,240,levelup);
       ui.cd=0;
       ui.cooling='f';
       getch();
       start();
      }
    }
  if(ui.lives<0)
   {
    cleardevice();
    ui.render();
    for(int x=0;x<10;x++)
     for(int y=0;y<8;y++)
      if(alien[x][y].alive=='t')
       alien[x][y].render(y);
    settextstyle(4,0,0);
    setcolor(WHITE);
    outtextxy(280,240,"Game Over");
    rectangle(270,230,445,300);
    getch();
    for(i=0;i<10;i++)
    {
     if(h[i].score<ui.score)
     newScore++;
    }
    if(newScore!=0)
    {
     newScore=0;
     newHighscore();
    }
    mm=0;
    ui.ammo=10;
    ui.lives=3;
    ui.score=0;
    ui.level=1;
    player.x=321;
    player.y=440;
    ui.cd=0;
    ui.cooling='f';
    for(int i=0;i<20;i++)
     {
      single[i].alive='f';
      if(i<10)
       dual[i].alive='f';
     }
   }
 for(i=0;i<5;i++)
   {
    laser[i].render();
    laser[i].move(10);
    if(laser[i].y>ui.YD)
     laser[i].alive='f';
    if(laser[i].x>player.x-10&&laser[i].x<player.x+10)
     if(laser[i].y>player.y-1&&laser[i].y<player.y+10)
      if(laser[i].alive=='t')
       {ui.lives--;laser[i].alive='f';}
   }
  if(ui.level<10)
   {
    delay(55);
   }
  else
   {
    ui.level=1;
   }
 }
}

void keyboard()
{
 btn=getch();
 if(btn==0)
  btn=getch();
 if(btn==27)
  pause();
 if(btn==75&&player.x-11>ui.XL)
  player.x-=15;
 if(btn==77&&player.x+11<ui.XR)
  player.x+=15;
 if(btn==' ')
  {
   for(int i=0;i<20;i++)
    {
     if(single[i].alive=='f'&&ui.cooling=='f')
      {
       single[i].shoot();
       break;
      }
    }
  }
 if((btn=='z'||btn=='Z')&&ui.ammo>1)
  {
   for(int i=0;i<10;i++)
    if((dual[i].alive=='f'&&dual[i].alive2=='f')&&ui.cooling=='f')
     {
      dual[i].shoot();
      ui.ammo-=2;
      break;
     }
  }
}

void pause()
{
 setcolor(WHITE);
 rectangle(220,140,420,340);
 for(int y=141;y<340;y++)
  for(int x=221;x<420;x++)
   putpixel(x,y,BLACK);
 settextstyle(4,0,2);
 outtextxy(260,150,"Game Paused");
 settextstyle(0,0,0);
 outtextxy(300,210,"Resume");
 outtextxy(300,240,"Instructions");
 outtextxy(300,270,"Main Menu");
 outtextxy(300,300,"Exit");
 int select=0;
 char done='f';
 char grafic[10]=" ";
 grafic[0] = 16;
 while(done=='f')
 {
  char button='*';
  setcolor(BLACK);
  for(int i=0;i<4;i++)
   outtextxy(280,210+(i*30),grafic);
  setcolor(LIGHTBLUE);
  outtextxy(280,210+(select*30),grafic);
  button=getch();
  if(button==0)
   button=getch();
  if(button==72&&select>0)
   select--;
  if(button==80&&select<3)
   select++;
  if(button=='l')
   ui.lives--;
  if(button=='\r'&&select==3)
   {done='t';mm=0;quit=0;}
  if(button=='\r'&&select==2)
   {
    dropPotential=0;
    done='t';
    mm=0;
    ui.ammo=10;
    ui.lives=3;
    ui.score=0;
    ui.level=1;
    player.x=321;
    player.y=440;
    ui.cd=0;
    ui.cooling='f';
    lifeup.alive='f';
    for(int i=0;i<20;i++)
     {
      single[i].alive='f';
      if(i<10)
       dual[i].alive='f';
      if(i<5)
       laser[i].alive='f';
     }
   }
  if(button=='\r'&&select==1)
   {
    instructions();
    ui.render();
    player.render();
    for(int i=0;i<20;i++)
     {
      single[i].render();
      if(i<10)
       dual[i].render();
     }
    pause();
   }
  if(button=='\r'&&select==0)
   return;
  if(button==27)
   return;
 }
}

void defaultScores()
{
 ifstream dScores;
 dScores.open("highscor.dat",ios::nocreate);
 if(!dScores)
 {
  for(int i=6;i<10;i++)
   h[i].score=99;
  for(i=1;i<6;i++)
   h[i].score=999;
  h[0].score=1561;
  for(i=0;i<10;i++)
   strcpy(h[i].name,"Default");
  strcpy(h[0].name,"Tapan");
  dScores.close();
  ofstream dWrite;
  dWrite.open("highscor.dat");
  for(i=0;i<10;i++)
   dWrite.write((char *)&h[i],sizeof(highscore));
  dWrite.close();
 }
 dScores.close();
}

void readScores()
{
 highscore temp;
 ifstream file;
 file.open("highscor.dat");
 for(int i=0;i<10;i++)
 {
  file.read((char *)&temp,sizeof(highscore));
  strcpy(h[i].name,temp.name);
  h[i].score=temp.score;
 }
 file.close();
}

void drawScores()
{
 settextstyle(0,0,1);
 setcolor(RED);
 outtextxy(149,99,"HIGHSCORES");
 setcolor(YELLOW);
 outtextxy(150,100,"HIGHSCORES");
 setcolor(WHITE);
 char tempHS[80];
 char tempScore[10];
 for(int i=0;i<10;i++)
 {
  strcpy(tempHS,h[i].name);
  for(int j=15;j>strlen(h[i].name);j--)
   strcat(tempHS," ");
  sprintf(tempScore,"%i",h[i].score);
  strcat(tempHS,tempScore);
  outtextxy(105,(120+(i*29)),tempHS);
 }
 setcolor(GREEN);
 rectangle(85,95,290,400);
}

void newHighscore()
{
 cleardevice();
 setcolor(WHITE);
 settextstyle(4,0,2);
 outtextxy(75,200,"Congratulations! A you made a New Highscore!");
 char newName[80]="Enter your name: ";
 char finalName[80]=" ";
 outtextxy(75,240,newName);
 char ch='*';
 while(ch!='\r')
 {
  ch=getch();
  if(isalpha(ch)!=0||ch==' ')
   {
    append(newName,ch);
    append(finalName,ch);
   }
  outtextxy(75,240,newName);
 }
 int rBroken=9;
 for(int i=9;i>-1;i--)
 {
  if(ui.score<h[i].score)
   break;
  rBroken=i;
 }
 for(i=rBroken;i<10;i++)
 {
  if(i<9)
  {
   strcpy(h[i+1].name,h[i].name);
   h[i+1].score=h[i].score;
   break;
  }
 }
 for(i=0;i<strlen(finalName)-1;i++)
  finalName[i]=finalName[i+1];
 finalName[strlen(finalName)-1]='\0';
 strcpy(h[rBroken].name,finalName);
 h[rBroken].score=ui.score;
 writeHighscores();
}

void writeHighscores()
{
 ofstream writing;
 writing.open("highscor.dat");
 for(int i=0;i<10;i++)
 {
  writing.write((char *)&h[i],sizeof(highscore));
 }
 writing.close();
}
