#include <allegro.h>
#include "Object.h"
#include <iostream>

using namespace std;
//##################################OBJECT######################################OBJECT###################################
//Sets
void Object::setObjectX(int ox){
  this->ox += ox;
}

void Object::setObjectY(int oy){
  this->oy += oy;
}

//Gets
int Object::getObjectX(){
	return this->ox;
}
int Object::getObjectY(){
	return this->oy;
}
	
int Object::getSpriteX(){
	return this->sx;
}

int Object::getSpriteY(){
	return this->sy;
}

int Object::getObjectW(){
	return this->ow;
}

int Object::getObjectH(){
	return this->oh;
}

//Others
void Object::createObject(int ox, int oy){
	this->ox = ox;
	this->oy = oy;
}

////##################################GUN######################################GUN###################################
//Constructor
Gun::Gun(){
	this->sx =  0;
    this->sy =  0;
    this->ow = 24; 
    this->oh = 12;
	setGunShot(false);
	setGunCooldown(0);
}

//Sets
void Gun::setGunShot(bool s){
  this->gun_Shot = s;
}

void Gun::setGunCooldown(int c){
  this->gun_Cooldown += c;
}

void Gun::setFireX(int x){
	this->ox = x;
}

void Gun::setFireY(int y){
	this->oy = y;
}


//Gets
int Gun::getGunCooldown(){
	return this->gun_Cooldown;
}

bool Gun::getGunShot(){
	return this->gun_Shot;
}

//##################################PLAYER######################################PLAYER###################################
//Constructor
Player::Player(){
    this->sx =  9;
    this->sy =  0;
    this->ow = 62; 
    this->oh = 82;
    this->direction = 3;
    this->animation = 0;
    this->life = 3;
}

//Sets
void Player::setAnimation(int a){
	this->animation += a;
}

void Player::setDirection(int d){
	this->direction = d;
}

void Player::setLife(int l){
	this->life += l;
}

void Player::setPoint(int p){
	this->point += p;
}

//Gets
int Player::getAnimation(){
  return this->animation;
}

int Player::getDirection(){
  return this->direction;
}

int Player::getLife(){
	return this->life;
}

int Player::getPoint(){
	return this->point;
}

//More Functions

void Player::control(Gun *gun){
  	if(key[KEY_RIGHT]      && this->getObjectX() < 800) {this->setObjectX( 10); this->setDirection(3); this->setAnimation(1);}
  	else if(key[KEY_LEFT]  && this->getObjectX() > 220) {this->setObjectX(-10); this->setDirection(2); this->setAnimation(1);}
  	else if(key[KEY_DOWN]  && this->getObjectY() < 480) {this->setObjectY( 10); this->setDirection(0); this->setAnimation(1);}
  	else if(key[KEY_UP]    && this->getObjectY() >  20) {this->setObjectY(-10); this->setDirection(1); this->setAnimation(1);}
	else if(key[KEY_SPACE] && gun->getGunCooldown() == false){
		gun->setGunShot(true); 
		gun->setFireX(this->getObjectX() + 32);
		gun->setFireY(this->getObjectY() + 52); 
		gun->setGunCooldown(15);
	}
  	else{this->setDirection(3); this->animation = 0;}

  	if(this->getAnimation() > 5) this->animation = 0;

	
	if(gun->getGunShot()){
		gun->setObjectX(40);
	}

	else{
		gun->setFireY(700);
	}

	if(gun->getGunCooldown()  > 0) gun->setGunCooldown(-1);
	if(gun->getGunCooldown() == 0) gun->setGunShot(false);
}

//##################################ENEMY######################################ENEMY###################################
//Sets
    void Enemy::setWalk(int w){
    	this->walk += w;

    	if(this->walk > 6) this->walk = 0;
    }
    
    void Enemy::setId(int i){
    	this->id = i;
    }

    void Enemy::setLife(int l){
    	this->life = l;
    }
    void Enemy::setEnemyPosition(int x, int y){
    	this->ox = x; 
    	this->oy = y;
    }

  
    //Gets
    int Enemy::getWalk(){
    	return this->walk;
    }
    
    int Enemy::getId(){
    	return this->id;
    }
    
    int Enemy::getLife(){
   		return this->life;    
   	}

   	void Enemy::setEnemy(){
   		this->sx =  0;
        this->sy =  0;
        this->ow = 50; 
        this->oh = 89;
        this->walk = 0;
        this->life = 1;
   	}

   	void Enemy::getEnemy(){
   		cout << "sx: "<< this->sx <<endl;
   		cout << "sy: "<< this->sy <<endl;
   		cout << "Ox: "<< this->ox <<endl;
   		cout << "oy: "<< this->oy <<endl;
   		cout << "Ow: "<< this->ow <<endl;
   		cout << "OH: "<< this->oh <<endl;
   	}
 