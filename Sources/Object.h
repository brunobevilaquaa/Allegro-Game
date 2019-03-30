#include <allegro.h>
#include <iostream>
using namespace std;

class Object{
protected:
    int ox;
    int oy;
    int sx;
    int sy;
    int ow;
    int oh;
    
public:
    //Sets
    void setObjectX(int ox);
    void setObjectY(int oy);
      
    //Gets
    int getObjectX();
    int getObjectY();
    int getSpriteX();
    int getSpriteY();
    int getObjectW();
    int getObjectH();

    //Others
    void createObject(int sx, int sy);

};
class Gun: public Object{
private:
    int gun_Cooldown;
    bool gun_Shot;

public:
    //Constructor
    Gun();

    //Sets
    void setGunCooldown(int c);
    void setGunShot(bool s);
    void setFireX(int x);
    void setFireY(int y);

    //Gets
    int getGunCooldown();
    bool getGunShot();
};


class Player: public Object{
private:
    int animation;
    int direction;
    int life;
    int point;

public:
    //Constructor;
    Player();
    //Sets
    void setAnimation(int a);
    void setDirection(int d);
    void setLife(int l);
    void setPoint(int p);

    //Gets
    int getAnimation();
    int getDirection();
    int getLife();
    int getPoint();
    
    //others
    void control(Gun *gun);
};

//--------------------Enemy--------------------
class Enemy: public Object{
private:
    int walk;
    int id;
    int life;

public:
    //Sets
    void setWalk(int w);
    void setId(int i);
    void setLife(int l);

    //Gets
    int getWalk();
    int getId();
    int getLife();


    void getEnemy();
    void setEnemyPosition(int, int);
    void setEnemy();

};

