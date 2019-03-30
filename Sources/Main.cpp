#include <allegro.h>
#include "Object.h"
#include <vector>

#include <iostream>

using namespace std;

//Functions
Player *P = new Player();
Gun    *G = new Gun();
vector<Enemy> vEnemy;

void createEnemy(vector<Enemy> &v, int id, int x, int y){
	Enemy build;

	build.setEnemy();
	build.setEnemyPosition(x, y);

	build.setId(id);

	v.push_back(build);

}

int colid(int Ax, int Ay, int Aw, int Ah, int Bx, int By, int Bw, int Bh){
	if(Ax + Aw > Bx && Ax < Bx + Bw && Ay + Ah > By && Ay < By + Bh) return 1;		
		return 0;
}

void controlEnemy(vector<Enemy> &v, BITMAP *buffer, BITMAP *enemy, Gun *G, Player *P){
	for(int i = 0; i < v.size(); i++){
		if(v[i].getObjectX() > 200){
			masked_blit(enemy, buffer, v[i].getSpriteX() + v[i].getWalk() * 100, v[i].getSpriteY(), v[i].getObjectX(), v[i].getObjectY(), v[i].getObjectW(), v[i].getObjectH());
			
			if(P->getPoint() > 15){
				v[i].setObjectX(-7);
			}
			
			if(P->getPoint() > 30){
				v[i].setObjectX(-7);
			}
			
			else{
				v[i].setObjectX(-4);
			}

			v[i].setWalk(1);
		
			if(colid(v[i].getObjectX(), v[i].getObjectY(), v[i].getObjectW(), v[i].getObjectH(), G->getObjectX(), G->getObjectY(), G->getObjectW(), G->getObjectH())){
				G->setFireY(800);
				G->setGunCooldown(0);
				G->setGunShot(false);
				v.erase(v.begin() + i);
				P->setPoint(1);
				cout << P->getPoint() << endl;

			
			}
		}
		else{
			v.erase(v.begin() + i);
			P->setLife(-1);
			cout << P->getLife() << endl; 
		}
	}
}



int main(){
	allegro_init();
	install_timer();
	install_keyboard();
	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 900, 600, 0, 0);
	set_window_title("Zombie Invader");

	//Load Bitmaps Game
	BITMAP *buffer      = create_bitmap(900, 600);
	BITMAP *player      = load_bitmap("../Sprites/Game/player1.bmp", NULL);
	BITMAP *enemy       = load_bitmap("../Sprites/Game/enemy1.bmp", NULL);
	BITMAP *background  = load_bitmap("../Sprites/Game/background_1.bmp", NULL);
	BITMAP *gun_Fire    = load_bitmap("../Sprites/Game/fire1.bmp", NULL);
	
	//Load Bitmaps Menu;
	BITMAP *menu_play    = load_bitmap("../Sprites/Menu/menu_play.bmp", NULL);
	BITMAP *menu_options = load_bitmap("../Sprites/Menu/menu_options.bmp", NULL);
	BITMAP *menu_quit    = load_bitmap("../Sprites/Menu/menu_quit.bmp", NULL);

	
	P->createObject(220, 260);
	G->createObject(  0, 800);

	int id = 0;
	int i = 0;
	bool play = true;
		
	for(int j = 0; j < vEnemy.size(); j++){
		cout << "Id: " << vEnemy[j].getId() << endl;
		cout << "" << endl;
	}

	while(!key[KEY_ESC] && play){
		P->control(G);
		
		if(rand() % 70 == 35){
			createEnemy(vEnemy, id, 800 ,(rand() % 5 + 1) * 80);
			id += 1;
		}
		

		masked_blit(background, buffer, 0, 0, 0, 0, 900, 600);
	    masked_blit(player, buffer, P->getSpriteX() + P->getAnimation() * 150, P->getSpriteY() + P->getDirection() * 117, P->getObjectX(), P->getObjectY(), P->getObjectW(), P->getObjectH());
	    masked_blit(gun_Fire, buffer, G->getSpriteX(), G->getSpriteY(), G->getObjectX(), G->getObjectY(), G->getObjectW(), G->getObjectH());
	   	
	   	controlEnemy(vEnemy, buffer, enemy, G, P);

	   	draw_sprite(screen, buffer, 0, 0);
		rest(30);
		clear(buffer);
		
		if(P->getLife() == 0){
			play = false;
		}

	}

	for(int i = 0; i < vEnemy.size(); i++){
		vEnemy.erase(vEnemy.begin() + i);
	}

	//Destroy all bitmaps
	destroy_bitmap(buffer);
	destroy_bitmap(gun_Fire);
	destroy_bitmap(player);
	destroy_bitmap(enemy);
	destroy_bitmap(menu_play);
	destroy_bitmap(menu_options);
	destroy_bitmap(menu_quit);
	destroy_bitmap(background);

	return 0;
}
END_OF_MAIN();





//g++ -o Main.bin *.cpp `allegro-config --libs` 
