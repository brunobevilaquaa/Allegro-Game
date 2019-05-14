#include <vector>
#include <iostream>
#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>

using namespace std;
//Dimensions
//player = 62 x 82
//enemy  = 50 x 89

//Structs for player and fire.
struct Object{
	int bx, by, sx, sy, ow, oh;
};

//struct for enemies.
struct enemy_Object{
	int bx, by, sx, sy, ow, oh, id, mov;
};

//Sort the id of enemies;
void sort();

//Creates enemies;
void create_enemy();

//Test a collision of 2 objects;
int colid();

//Move the enemies;
void move_enemy(BITMAP *enemy, BITMAP *enemy_hit, BITMAP *buffer);

//Controls the player;
void control();

struct Object Player = {220, 260, 9, 0, 62, 82};
struct Object fire   = {900, 800, 0, 0, 24, 12};

vector<enemy_Object> enemies;

//Variables of objects
int move_Sprite_Player = 0;
int move_Sprite_Enemy  = 0;
int player_Direction   = 0;
int id = 0;
int gun_Cooldown = 0;
int gun_Shot     = 0;
int gun_Ammo     = 3;
int enemy_Spawn  = 0;
int spawn_Place_Y  = 0;
int spawn_Place_X  = 0;

//Variables of menu
int play = 0;
int menu = 0;

//Variables of game
int fps  = 90;
int life =  5;

int main(){
	//Inicialization
	allegro_init();
	install_timer();
	install_keyboard();
	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 900, 600, 0, 0);
	set_window_title("Zombie Invader");

	//Load Bitmaps Game
	BITMAP *buffer      = create_bitmap(900, 600);
	BITMAP *player      = load_bitmap("../Sprites/Game/player1.bmp", 			NULL);
	BITMAP *enemy       = load_bitmap("../Sprites/Game/enemy1.bmp",       NULL);
	BITMAP *enemy_hit   = load_bitmap("../Sprites/Game/enemy_hit.bmp",    NULL);
	BITMAP *background  = load_bitmap("../Sprites/Game/background_1.bmp", NULL);
	BITMAP *sprite_Shot = load_bitmap("../Sprites/Game/fire1.bmp", 				NULL);

	//Load Bitmaps Menu;
	BITMAP *menu_play    = load_bitmap("../Sprites/Menu/menu_play.bmp", NULL);
	BITMAP *menu_options = load_bitmap("../Sprites/Menu/menu_options.bmp", NULL);
	BITMAP *menu_quit    = load_bitmap("../Sprites/Menu/menu_quit.bmp", NULL);

	//Game function;
	while(!key[KEY_ESC]){
		if(play == 0){
				 if(key[KEY_UP]    && menu > 0) menu-= 1;
			else if(key[KEY_DOWN]  && menu < 2) menu+= 1;
			else if(key[KEY_ENTER]){
				 if(menu == 0){play = 1; fps = 30; life = 5;}
				 if(menu == 2) break;
			}

					 if(menu == 0) masked_blit(menu_play, buffer,    0, 0, 0, 0, 900, 600);
			else if(menu == 1) masked_blit(menu_options, buffer, 0, 0, 0, 0, 900, 600);
			else if(menu == 2) masked_blit(menu_quit, buffer,    0, 0, 0, 0, 900, 600);

		}

		else{
			control();

			masked_blit(background, buffer, 0, 0, 0, 0, 900, 600);
			move_enemy(enemy, enemy_hit, buffer);
			masked_blit(player, buffer, Player.sx + move_Sprite_Player * 150, Player.sy + player_Direction * 117, Player.bx, Player.by, Player.ow, Player.oh);
			masked_blit(sprite_Shot, buffer, fire.sx, fire.sy, fire.bx, fire.by, fire.ow, fire.oh);

			if(rand() % 20 + 1 == 10){
				create_enemy();
			}

			if(life == 0){
				play = 0;
				enemies.clear();
				fps = 90;
			}

		}

		draw_sprite(screen, buffer, 0, 0);
		rest(fps);
		clear(buffer);
	}

	//Destroy all bitmaps
	destroy_bitmap(buffer);
	destroy_bitmap(player);
	destroy_bitmap(enemy);
	destroy_bitmap(background);

	return 0;
}
END_OF_MAIN();

//Function for player control
void control(){
		   if(key[KEY_RIGHT] && Player.bx < 800) {Player.bx += 10; player_Direction = 3; move_Sprite_Player++;}
	else if(key[KEY_LEFT]  && Player.bx > 220) {Player.bx -= 10; player_Direction = 2; move_Sprite_Player++;}
	else if(key[KEY_DOWN]  && Player.by < 480) {Player.by += 10; player_Direction = 0; move_Sprite_Player++;}
	else if(key[KEY_UP]    && Player.by >  20) {Player.by -= 10; player_Direction = 1; move_Sprite_Player++;}
	else if(key[KEY_SPACE] && gun_Cooldown   <= 0){gun_Shot = 1; fire.bx = Player.bx + 33; fire.by = Player.by + 52; gun_Cooldown = 15;}
	else{move_Sprite_Player = 0; player_Direction = 3;}

	if(move_Sprite_Player > 5) move_Sprite_Player = 0;

	if(gun_Shot == 1){
		fire.bx += 40;

	}

	else{
		fire.by = 700;
	}

	if(gun_Cooldown  >   0) gun_Cooldown -= 1;
	if(gun_Cooldown ==   0) gun_Shot = 0;
}

void sort(){
	for(int i = 0; i < enemies.size(); i++) {
		enemies[i].id = i;
	}
}

void create_enemy(){
		enemy_Object aux;

		int ry = rand() % 5 + 1;
		int rx = rand() % 1;

		aux.id  =   0;
		aux.bx  =   900 + rx * 100;
	  aux.by  =   ry * 90;
		aux.sx  =   0;
		aux.sy  =   0;
		aux.ow  =  50;
		aux.oh  =  89;
		aux.mov = 	0;

		enemies.push_back(aux);

		sort();

		cout << "BOTEEEEIII" << endl;
		cout << "NUMERO DE ZOMBIE: " << enemies.size() << endl;
}

int colid(int Ax, int Ay, int Bx, int By, int Aw, int Ah, int Bw, int Bh){
	if(Ax + Aw > Bx && Ax < Bx + Bw && Ay + Ah > By && Ay < By + Bh) return 1;
	return 0;
}

void move_enemy(BITMAP *enemy, BITMAP *enemy_hit, BITMAP *buffer){
	int i = 0;
	int j = 0;

	for(i = 0; i < enemies.size(); i++){
		if(enemies[i].bx > 220){
			enemies[i].bx -= 4;

			masked_blit(enemy, buffer, enemies[i].sx + enemies[i].mov * 100, enemies[i].sy, enemies[i].bx, enemies[i].by, enemies[i].ow, enemies[i].oh);

			if(enemies[i].mov > 5){enemies[i].mov = 0;}
			else{enemies[i].mov += 1;}

			if(colid(enemies[i].bx, enemies[i].by, fire.bx, fire.by, enemies[i].ow, enemies[i].oh, fire.ow, fire.oh)){
					masked_blit(enemy_hit, buffer, enemies[i].sx + enemies[i].mov * 100, enemies[i].sy, enemies[i].bx, enemies[i].by, enemies[i].ow, enemies[i].oh);
					gun_Shot = 0;
					enemies.erase(enemies.begin() + i);
					sort();
			}

			if(colid(enemies[i].bx, enemies[i].by, Player.bx, Player.by, enemies[i].ow, enemies[i].oh, Player.ow, Player.oh)){
					masked_blit(enemy_hit, buffer, enemies[i].sx + enemies[i].mov * 100, enemies[i].sy, enemies[i].bx, enemies[i].by, enemies[i].ow, enemies[i].oh);
					enemies.erase(enemies.begin() + i);
					sort();
					life -= 1;
			}

		} else{
			enemies.erase(enemies.begin() + i);
			life -= 1;
			cout << life << endl;
			sort();

			cout << "TIREIIII" << endl;
			cout << "NUMERO DE ZOMBIE: " << enemies.size() << endl;
		}
	}
}


//g++ -o Main.bin Main.cpp `allegro-config --libs`
