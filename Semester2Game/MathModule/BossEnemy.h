#pragma once
class BossEnemy
{
public:
	BossEnemy();
	bool GetIsLeft();
	void SetIsLeft(bool);
	void SetInactive();
	bool GetActive();
	bool GetJustDead();
	void TakeDamage();
	void AddTime(float);
	int GetGateNum();
	void SetGateNum(int num);

private:
	bool isLeft = true;
	bool isActive = true;
	bool justDead = false;
	int health = 5;
	float timeSinceHit = 0.0;
	int gateNum;

};

