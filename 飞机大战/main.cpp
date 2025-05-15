#include<stdio.h>
#include<easyx.h>//须额外安装
#include<conio.h>
#include<time.h>
#define Background_Width 650
#define Background_Height 750
#define Bullet_Num 30
#define My_Plane_Speed 8
#define Bullet_Speed 10
#define Enermy_Type_Big 1
#define Enermy_Type_Small 2
#define Enermy_Number 10
#define Enermy_Speed 2
const int MAX = 10;
int score=0;
struct plane
{	
	int x, y;
	bool islive;
	int hp;
	int type;
	int width, height;
};
plane myplane, enemy[Enermy_Number], bullet[Bullet_Num];
IMAGE background, myplane_img[2], enermy_img[2][2], bullet_img[2];
void load_image() {
	loadimage(&background, "游戏背景图.png", Background_Width, Background_Height);
	loadimage(&myplane_img[0], "me2.png", 80, 80);
	loadimage(&enermy_img[0][0], "enemy2.png", 80, 80);
	loadimage(&enermy_img[1][0], "enemy1.png", 50, 50);
	loadimage(&bullet_img[0], "bullet1.png", 15, 15);

}
bool timer(int ms,int id)
{// 计时器
	static unsigned long t[MAX];
	if (clock() - t[id] > ms)
	{
		t[id] = clock();
		return true;
	}
	else
	{
		return false;
	}
}
void init_game()
{
	load_image();
	myplane.x = Background_Width / 2 - 40;
	myplane.y = Background_Height - 100;
	myplane.islive = true;
	
	for (int i = 0; i < Bullet_Num; i++)
	{
		bullet[i].islive = false;
		bullet[i].x = 0;
		bullet[i].y = 0;
	}

}
void draw_game()
{
	putimage(0, 0, &background);
	putimage(myplane.x, myplane.y, &myplane_img[0]);
	for (int i = 0; i < Bullet_Num; i++)
	{
		if (bullet[i].islive)
		{
			putimage(bullet[i].x, bullet[i].y, &bullet_img[0]);
		}
	}
	for(int i = 0; i < Enermy_Number; i++)
	{
		if (enemy[i].islive)
		{
			if (enemy[i].type == Enermy_Type_Big)
				putimage(enemy[i].x, enemy[i].y, &enermy_img[0][0]);
			else if (enemy[i].type == Enermy_Type_Small)
				putimage(enemy[i].x, enemy[i].y, &enermy_img[1][0]);
		}
	}
}
void create_bullet()
{
	for (int i = 0; i < Bullet_Num; i++)
	{
		if (bullet[i].islive == false)
		{
			bullet[i].islive = true;
			bullet[i].x = myplane.x + 33;
			bullet[i].y = myplane.y;
			break;
		}
	}
}
void plane_move()
{
	if (_kbhit()) {
		if (myplane.islive == false) return ;
		switch (_getch())
		{
		case 'w':myplane.y = myplane.y - My_Plane_Speed; break;
		case 's':myplane.y = myplane.y + My_Plane_Speed; break;
		case 'a':myplane.x = myplane.x - My_Plane_Speed; break;
		case 'd':myplane.x = myplane.x + My_Plane_Speed; break;
		case 'j':create_bullet(); break;
		}
	}
}
void bullet_move()
{
	for (int i = 0; i < Bullet_Num; i++)
	{
		if (bullet[i].islive == true)
		{
			bullet[i].y = bullet[i].y - Bullet_Speed;
			if (bullet[i].y < 0)
			{
				bullet[i].islive = false;
			}
		}
	}
}
void create_enermy()
{
    for(int i = 0; i < Enermy_Number; i++)
    {
        if (enemy[i].islive == false)
        {
            enemy[i].islive = true;
            enemy[i].x = rand() % (Background_Width - 80);
            enemy[i].y = 0;
            // 随机分配敌机类型
			if (rand() % 2 == 0) {
				enemy[i].type = Enermy_Type_Big;
				enemy[i].width = 80;
				enemy[i].height = 80;
				enemy[i].hp = 3;
			}
             else {
                enemy[i].type = Enermy_Type_Small;
                enemy[i].width = 50;
                enemy[i].height = 50;
				enemy[i].hp = 1;
            }
            break;
        }
    }
}
void enermy_move()
{
	for (int i = 0; i < Enermy_Number; i++)
	{
		if (enemy[i].islive == true)
		{
			enemy[i].y = enemy[i].y + Enermy_Speed;
			if (enemy[i].y > Background_Height)
			{
				enemy[i].islive = false;
			}
		}
	}
}
void enemy_plane_attacked()
{
	for (int i = 0; i < Enermy_Number; i++)
	{
		if (!enemy[i].islive) continue;
		for (int j = 0; j < Bullet_Num; j++)
		{
			if (bullet[j].islive == false) continue;
			if (bullet[j].x > enemy[i].x && bullet[j].x < enemy[i].x + enemy[i].width &&
				bullet[j].y > enemy[i].y && bullet[j].y < enemy[i].y + enemy[i].height)
			{
				bullet[j].islive = false;
				enemy[i].hp--;
				if (enemy[i].hp <= 0)
				{
					enemy[i].islive = false;
					score +=1 ;
				}
			}
		}
	}
}
void my_plane_attacked()
{
	for (int i = 0; i < Enermy_Number; i++)
	{
		if (!enemy[i].islive) continue;
		if (
				(myplane.x > enemy[i].x && myplane.x < enemy[i].x + enemy[i].width &&
			myplane.y > enemy[i].y && myplane.y < enemy[i].y + enemy[i].height)
			||
				(myplane.x+80 > enemy[i].x && myplane.x+80< enemy[i].x + enemy[i].width &&
			myplane.y+80 > enemy[i].y && myplane.y+80< enemy[i].y + enemy[i].height)
			)
		{
			myplane.islive = false;
			break;
		}
	}
}
int main()
{
	initgraph(Background_Width, Background_Height);
	init_game();
	
	printf("游戏背景图为AI生成\n规则：请在终端内输入指令(按\"w\"\"a\"\"s\"\"d\"进行移动，按\"j\"发射子弹)\n");
	while (myplane.islive)
	{
		draw_game();
		FlushBatchDraw();
		plane_move();
		bullet_move();
		if (timer(500, 0)) create_enermy();
		if (timer(30, 2)) enermy_move();
		enemy_plane_attacked();
		my_plane_attacked();
	}
	EndBatchDraw();
	printf("本局得分：%d\n", score);
	system("pause");
	
	return 0;
}

