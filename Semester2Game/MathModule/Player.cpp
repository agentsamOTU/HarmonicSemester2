#include "Player.h"
#include "Scene.h"

Player::Player()
{
}

bool Player::GetGrounded()
{
	if (contactNum > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Player::GetLeftContact()
{
	if(contactLeft > 0)
	{
		return true;
	}
	else
	{
	return false;
	}
}

bool Player::GetRightContact()
{
	if (contactRight > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Player::GetLeft()
{
	return faceLeft;
}

bool Player::GetRight()
{
	return faceRight;
}

bool Player::GetUp()
{
	return faceUp;
}

bool Player::GetRooted()
{
	return rooted;
}

bool Player::GetDown()
{
	return faceDown;
}

bool Player::GetLastRight()
{
	return lastRight;
}

void Player::SetGrounded(bool state)
{
	grounded = state;
}

void Player::Contacted()
{
	contactNum++;
}

void Player::ContactLeft()
{
	contactLeft++;
}

void Player::ContactRight()
{
	contactRight++;
}

void Player::EndLeft()
{
	contactLeft--;
}

void Player::EndRight()
{
	contactRight--;
}

void Player::EndContacted()
{
	contactNum--;
}

void Player::ArrowShot(b2World* curScene)
{
	int offsetX;
	int offsetY = 0;
	int veloDirX;
	int veloDirY=0;
	if (ECS::GetComponent<Player>(EntityIdentifier::MainPlayer()).GetUp())
	{
		veloDirY = 30;
		if (ECS::GetComponent<Player>(EntityIdentifier::MainPlayer()).GetLeft())
		{
			offsetX = -ECS::GetComponent<Sprite>(EntityIdentifier::MainPlayer()).GetWidth()/2 - 5;
			veloDirX = -40;
		}
		else if (ECS::GetComponent<Player>(EntityIdentifier::MainPlayer()).GetRight())
		{
			offsetX = ECS::GetComponent<Sprite>(EntityIdentifier::MainPlayer()).GetWidth()/2 + 5;
			veloDirX = 40;
		}
		else
		{
			veloDirX = 0;
			offsetX = 0;
			offsetY = ECS::GetComponent<Sprite>(EntityIdentifier::MainPlayer()).GetHeight()/2 + 5;
		}
	}
	else if (ECS::GetComponent<Player>(EntityIdentifier::MainPlayer()).GetDown())
	{
		veloDirY = -30;
		if (ECS::GetComponent<Player>(EntityIdentifier::MainPlayer()).GetLeft())
		{
			offsetX = -ECS::GetComponent<Sprite>(EntityIdentifier::MainPlayer()).GetWidth() / 2 - 5;
			veloDirX = -40;
		}
		else if (ECS::GetComponent<Player>(EntityIdentifier::MainPlayer()).GetRight())
		{
			offsetX = ECS::GetComponent<Sprite>(EntityIdentifier::MainPlayer()).GetWidth() / 2 + 5;
			veloDirX = 40;
		}
		else if(!ECS::GetComponent<Player>(EntityIdentifier::MainPlayer()).GetGrounded())
		{
			printf("Air down");
			veloDirX = 0;
			offsetX = 0;
			offsetY = -ECS::GetComponent<Sprite>(EntityIdentifier::MainPlayer()).GetHeight() / 2 - 5;
		}
		else
		{
			if (!ECS::GetComponent<Player>(EntityIdentifier::MainPlayer()).GetLastRight())
			{
				offsetX = -ECS::GetComponent<Sprite>(EntityIdentifier::MainPlayer()).GetWidth() / 2 - 5;
				veloDirX = -40;
			}
			else if (ECS::GetComponent<Player>(EntityIdentifier::MainPlayer()).GetLastRight())
			{
				offsetX = ECS::GetComponent<Sprite>(EntityIdentifier::MainPlayer()).GetWidth() / 2 + 5;
				veloDirX = 40;
			}
		}
	}
	else
	{
		if (!ECS::GetComponent<Player>(EntityIdentifier::MainPlayer()).GetLastRight())
		{
			offsetX = -ECS::GetComponent<Sprite>(EntityIdentifier::MainPlayer()).GetWidth()/2 - 5;
			veloDirX = -40;
		}
		else if (ECS::GetComponent<Player>(EntityIdentifier::MainPlayer()).GetLastRight())
		{
			offsetX = ECS::GetComponent<Sprite>(EntityIdentifier::MainPlayer()).GetWidth()/2 + 5;
			veloDirX = 40;
		}
	}
	
	if (arrNum < 3)
	{
		auto entity = ECS::CreateEntity();
		//add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<Arrow>(entity);
		//sets up components
		std::string fileName = "arrowAction.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 10,1 );
		ECS::GetComponent<Transform>(entity).SetPosition(ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()).GetPosition()+
			vec3(offsetX,offsetY,0));
		//collision settings
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
		auto& tempTrans = ECS::GetComponent<Transform>(entity);
		b2Body* tempBody;
		b2PolygonShape tempShape;
		tempShape.SetAsBox(5.f, 0.5f);
		b2FixtureDef tempFix;
		tempFix.shape = &tempShape;
		tempFix.restitution = 1.f;

		/*b2PolygonShape tempShapeTip;
		tempShapeTip.SetAsBox(1.f, 0.5f,b2Vec2(2.5f,0.f),0);
		b2FixtureDef tempFixTip;
		tempFixTip.shape = &tempShapeTip;
		tempFixTip.restitution = 1.f;
		tempFixTip.density = 1.f;*/

		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(tempTrans.GetPositionX()), float32(tempTrans.GetPositionY()));
		tempBody = curScene->CreateBody(&tempDef);
		tempBody->CreateFixture(&tempFix);
		//tempBody->CreateFixture(&tempFixTip);
		tempBody->SetEntityNumber(entity);
		tempBody->SetEntityType(3);
		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()),
			vec2(0.f, 0.f),
			true);
		tempPhsBody.SetVelocity(vec3(veloDirX, 10+veloDirY, 0));
		b2Vec2 curPos = ECS::GetComponent<PhysicsBody>(entity).GetBody()->GetTransform().p;
		tempPhsBody.GetBody()->SetTransform(curPos, atan2(veloDirY, veloDirX));
		//sets up the identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "arrow");
		arrNum++;
		std::string blankFile = "blank.png";
		ECS::GetComponent<Sprite>(arrNum).LoadSprite(blankFile, 16, 16);
	}
	else
	{

	}
}

void Player::ArrowDestroyed()
{
	std::string fileName = "arrow.png";
	std::string blankFile = "blank.png";
	arrNum--;
	switch (arrNum)
	{
	case 2:
		ECS::GetComponent<Sprite>(3).LoadSprite(fileName, 16, 16);
		ECS::GetComponent<Sprite>(2).LoadSprite(blankFile, 16, 16);
		ECS::GetComponent<Sprite>(1).LoadSprite(blankFile, 16, 16);
		break;
	case 1:
		ECS::GetComponent<Sprite>(3).LoadSprite(fileName, 16, 16);
		ECS::GetComponent<Sprite>(2).LoadSprite(fileName, 16, 16);
		ECS::GetComponent<Sprite>(1).LoadSprite(blankFile, 16, 16);
		break;
	case 0:
		ECS::GetComponent<Sprite>(3).LoadSprite(fileName, 16, 16);
		ECS::GetComponent<Sprite>(2).LoadSprite(fileName, 16, 16);
		ECS::GetComponent<Sprite>(1).LoadSprite(fileName, 16, 16);
		break;
	}
}

void Player::SetLeft(bool left)
{
	faceLeft = left;
}

void Player::SetRight(bool inp)
{
	faceRight = inp;
}

void Player::SetUp(bool inp)
{
	faceUp = inp;
}

void Player::SetRoot(bool inp)
{
	rooted = inp;
}

void Player::SetDown(bool inp)
{
	faceDown = inp;
}

void Player::SetLastRight(bool inp)
{
	lastRight = inp;
}

bool Player::GetKill()
{
	return dead;
}

void Player::SetKill()
{
	dead = true;
}

float Player::GetKillTime()
{
	return killTime;
}

void Player::AddKillTime(float addAm)
{
	killTime += addAm;
}

bool Player::GetGoalContact()
{
	if (contactingGoal > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Player::SetContactingGoal(bool boo)
{
	if (boo)
	{
		contactingGoal++;
	}
	else
	{
		contactingGoal--;
	}
}

void Player::AddJumpTime(float delta)
{
	jumpTime += delta;
}

float Player::GetJumpTime()
{
	return jumpTime;
}

void Player::ResetShoot()
{
	shootTime = 0.f;
}

void Player::ResetJump()
{
	jumpTime = 0.f;
}

void Player::AddShootTime(float delta)
{
	shootTime += delta;
}

float Player::GetShootTime()
{
	return shootTime;
}
