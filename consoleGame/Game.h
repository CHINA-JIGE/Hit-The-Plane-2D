#pragma once



//---------------------------------------------
#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

#define planeSizeX 3
#define planeSizeY 2
#define bulletSizeX 1
#define bulletSizeY 1
#define string_MadeByJige_SizeX 12
#define string_MadeByJige_SizeY 1
#define string_Go_SizeX 5
#define string_Go_SizeY 1
#define string_GameOver_SizeX 12
#define string_GameOver_SizeY 3
#define string_Ready_SizeX 8
#define string_Ready_SizeY 1
#define planeBulletCount 30
#define backBufferWidth 40
#define backBufferHeight 20
#define enemyPlaneCount 16


enum GAME_STATUS
{
	GAME_STATUS_EXIT = 0,
	GAME_STATUS_START = 1,
	GAME_STATUS_PLAYING = 2,
	GAME_STATUS_GAMEOVER = 3,
};
//-------------VARIABLE-------------------



extern GAME_STATUS gameStatus;
extern int	gameScore;


//------------------FUNCTIONS--------------------]
extern void Init();
extern void Clear();
extern void UpdateGameStartAnimation();
extern void UpdateGameplay();
extern void DrawScore(int x,int y);
extern void RenderToScreen();
extern void DrawImage(char* image, int sizeX, int sizeY,int posX,int posY);

extern bool valInRange(int val, int leftBound, int rightBound);
extern float clamp(float val, float left, float right);

//---------------------------------------------
struct Bullet
{
	Bullet()
	{
		x = backBufferWidth / 2;
		y = backBufferHeight - 2;
		velocityY = 0.5f;
		enabled = false;
	}
	float x;
	float y;
	float velocityY;
	bool enabled;
};

class Plane
{
public:
	Plane(bool shouldBeClampedPos=false);
	void		Initialize(float posX,float posY,float hp);
	void		SetPosition(float x, float y);
	float		GetPosX();
	float		GetPosY();
	float		GetHP();
	Bullet	GetBullet(int index);
	void		UpdateBullets();
	void		SetBulletEnabled(bool enabled, int index);
	void		SetBulletPos(int index,float x, float y);
	bool		CollideWithBullet(Bullet b);
	bool		CollideWithPoint(float x, float y);
	void		Move(float dx, float dy);
	void		Render();
	void		SetImageMemAddr(char* addr);
	void		SetHP(float HP);
	void		ReduceHP();
private:
	bool mShouldBeClampedPos;
	char* imageAddr;
	Bullet mBullet[planeBulletCount];
	float mPosX;
	float mPosY;
	float mHP;
};


//---------------IMAGE ARRAY-----------------

extern char backBuffer[backBufferHeight][backBufferWidth];
extern char ImagePlanePlayer[planeSizeY][planeSizeX];
extern char ImagePlaneEnemy[planeSizeY][planeSizeX];
extern char ImageBullet[bulletSizeY][bulletSizeX];
extern char ImageStringMadeByJige[string_MadeByJige_SizeY][string_MadeByJige_SizeX];
extern char ImageStringReady[string_Ready_SizeY][string_Ready_SizeX];
extern char ImageStringGO[string_Go_SizeY][string_Go_SizeX];
extern char ImageGameOverYourScore[string_GameOver_SizeY][string_GameOver_SizeX];

//---------------------------------------------