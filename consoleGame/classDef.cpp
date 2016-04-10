#include "Game.h"

Plane::Plane(bool shouldBeClampedPos)
{
	mShouldBeClampedPos = shouldBeClampedPos;
}

void Plane::Initialize(float posX, float posY, float hp)
{
	mPosX = posX;
	mPosY = posY;
	mHP = hp;
}

void Plane::SetPosition(float x, float y)
{
	if (mShouldBeClampedPos)
	{
		mPosX = clamp(x,0,backBufferWidth-planeSizeX);
		mPosY = clamp(y,0,backBufferHeight -planeSizeY);
	}
	else
	{
		mPosX = x;
		mPosY = y;
	}
};

float Plane::GetPosX()
{
	return mPosX;
};


float Plane::GetPosY()
{
	return mPosY;
};

float Plane::GetHP()
{
	return mHP;
};

Bullet Plane::GetBullet(int index)
{
	if (index >= 0 && index < planeBulletCount)
	{
		return mBullet[index];
	}
	return mBullet[0];
};

void Plane::UpdateBullets()
{
	for (int i = 0;i < planeBulletCount;i++)
	{
		if(mBullet[i].enabled)mBullet[i].y -= mBullet[i].velocityY;

		//bullets will only up/down side
		if (mBullet[i].y > backBufferHeight || mBullet[i].y <0)mBullet[i].enabled = false;

	}
}

void Plane::SetBulletEnabled(bool enabled, int index)
{
	if (index >= 0 && index < planeBulletCount)
	{
		mBullet[index].enabled = enabled;
	}
}

void Plane::SetBulletPos(int index, float x, float y)
{
	if (index >= 0 && index < planeBulletCount)
	{
		mBullet[index].x = x;
		mBullet[index].y = y;
	}
}

bool Plane::CollideWithBullet(Bullet b)
{
	if ((b.enabled == true) &&
		(b.x >= mPosX) &&
		(b.x <= mPosX + planeSizeX) &&
		(b.y >= mPosY) &&
		(b.y <= mPosY + planeSizeY))
	{
		return true;
	}

	return false;
}

bool Plane::CollideWithPoint(float x, float y)
{
	if ( (x >= mPosX) &&
		(x <= mPosX + planeSizeX) &&
		(y >= mPosY) &&
		(y <= mPosY + planeSizeY))
	{
		return true;
	}


	return false;
};


void Plane::Move(float dx, float dy)
{
	SetPosition(mPosX + dx, mPosY + dy);
}

void Plane::Render()
{
	DrawImage(imageAddr, planeSizeX, planeSizeY, int(mPosX), int(mPosY));

	//update bullets of player / enemies
	for (int i = 0;i < planeBulletCount;i++)
	{
		if (mBullet[i].enabled)
		{
			DrawImage(imageAddr, bulletSizeX, bulletSizeY, int(mBullet[i].x), int(mBullet[i].y));
		}
	}
}

void Plane::SetImageMemAddr(char * addr)
{
	imageAddr = addr;
}

void Plane::SetHP(float HP)
{
	mHP = HP;
}

void Plane::ReduceHP()
{
	mHP--;
}
