#include <vector>
#include <iostream>
#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>

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
void create_Enemy();

//Test a collision of 2 objects;
int colid();

//Move the enemies;
void move_Enemy(BITMAP *enemy, BITMAP *enemy_hit, BITMAP *buffer, SAMPLE *map_Hit, SAMPLE *zombie_DeathA, SAMPLE *zombie_DeathB, SAMPLE *player_Damage);

//Controls the player;
void player_Control(SAMPLE *shot_Sound);

//
void make_Form_A(SAMPLE *zombie_Spawn);

//
void make_Form_B(SAMPLE *zombie_Spawn);

//
void make_Form_C(SAMPLE *zombie_Spawn);

struct Object Player = {220, 260, 9, 0, 62, 82};
struct Object fire   = {900, 800, 0, 0, 24, 12};

//Vectors
vector<enemy_Object> enemies;


//Variables of objects
int move_Sprite_Player = 0;
int move_Sprite_Enemy  = 0;
int player_Direction   = 0;
int id = 0;
int gun_Cooldown = 0;
int gun_Shot     = 0;
int gun_Ammo     = 3;

//Variables of menu
int play = 0;
int menu = 0;
int cred = 0;

//Variables of game
int fps    = 80;
int life   =  5;
int score  =  0;
int hscore = 	0;

int main(){
	//Inicialization
	allegro_init();
	install_timer();
	install_keyboard();
	install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 900, 600, 0, 0);
	set_window_title("Zombie Invader");

	//Fonts
	FONT *font = load_font("../Fonts/font.pcx", NULL, NULL);

	//Load Sample;
	SAMPLE *shot_Sound    = load_sample("../Sounds/shot.wav");
	SAMPLE *map_Hit 	  = load_sample("../Sounds/map_Hit.wav");
	SAMPLE *restore_Life  = load_sample("../Sounds/restore_Life.wav");
	SAMPLE *zombie_DeathA = load_sample("../Sounds/zombie_DeathA.wav");
	SAMPLE *zombie_DeathB = load_sample("../Sounds/zombie_DeathB.wav");
	SAMPLE *zombie_Spawn  = load_sample("../Sounds/zombie_Spawn.wav");
	SAMPLE *player_Damage = load_sample("../Sounds/player_Damage.wav");


	//Load Bitmaps Game
	BITMAP *buffer      = create_bitmap(900, 600);
	BITMAP *player      = load_bitmap("../Sprites/Game/player1.bmp", 	   NULL);
	BITMAP *enemy       = load_bitmap("../Sprites/Game/enemy1.bmp",        NULL);
	BITMAP *enemy_hit   = load_bitmap("../Sprites/Game/enemy_hit1.bmp",    NULL);
	BITMAP *background  = load_bitmap("../Sprites/Game/background_1.bmp",  NULL);
	BITMAP *sprite_Shot = load_bitmap("../Sprites/Game/fire1.bmp", 		   NULL);
	BITMAP *life_heart  = load_bitmap("../Sprites/Game/life_heart.bmp",     NULL);

	//Load Bitmaps Menu;
	BITMAP *menu_play    = load_bitmap("../Sprites/Menu/menu_play.bmp",    NULL);
	BITMAP *menu_credits = load_bitmap("../Sprites/Menu/menu_credits.bmp", NULL);
	BITMAP *menu_quit    = load_bitmap("../Sprites/Menu/menu_quit.bmp",    NULL);
	BITMAP *credits      = load_bitmap("../Sprites/Menu/credits.bmp",      NULL);



	srand (time(NULL));

	//Game function;
	while(!key[KEY_ESC]){
		if(play == 0){
				   if(key[KEY_UP]    && menu > 0) menu-= 1;
			else if(key[KEY_DOWN]  && menu < 2) menu+= 1;
			else if(key[KEY_ENTER]){
				 	    if(menu == 0) {play = 1; fps = 30; life = 5; score = 0;}
		  	 else if(menu == 1) {menu = 3;}
				 else if(menu == 2) break;
				 }

					 if(menu == 0) masked_blit(menu_play, buffer,    0, 0, 0, 0, 900, 600);
			else if(menu == 1) masked_blit(menu_credits, buffer, 0, 0, 0, 0, 900, 600);
			else if(menu == 2) masked_blit(menu_quit, buffer,    0, 0, 0, 0, 900, 600);
			else if(menu == 3 and !key[KEY_ENTER]){
					 masked_blit(credits, buffer, 0, 0, 0, 0, 900, 600);
					 if(key[KEY_ENTER]) menu = 0;
			}
		}

		else{
			masked_blit(background, buffer, 0, 0, 0, 0, 900, 600);
			move_Enemy(enemy, enemy_hit, buffer, map_Hit, zombie_DeathA, zombie_DeathB, player_Damage);
			masked_blit(player, buffer, Player.sx + move_Sprite_Player * 150, Player.sy + player_Direction * 117, Player.bx, Player.by, Player.ow, Player.oh);
			masked_blit(sprite_Shot, buffer, fire.sx, fire.sy, fire.bx, fire.by, fire.ow, fire.oh);

			textprintf_ex(buffer, font, 400, 10, 0xffffff, -1, "SCORE");

			player_Control(shot_Sound);

			if(score == 100){play_sample(restore_Life, 100, 128, 1000, 0); life = 5;}

			for(int x = 0; x < life; x++){
					masked_blit(life_heart, buffer, 0, 0, 20 + x * 50 , 10, 40, 35);
			}

			if(!hscore && !score){
					textprintf_ex(buffer, font, 600, 10, 0xffffff, -1, "000 000");
			} else {
					textprintf_right_ex(buffer, font, 700, 10, 0xffffff, -1, " %i", score);
					if(!hscore) textprintf_ex(buffer, font, 773, 10, 0xffffff, -1, "000");
					else textprintf_right_ex(buffer, font, 873, 10, 0xffffff, -1, " %i", hscore);
			}

			
			if(rand() % 500 + 1 == 69){
				make_Form_A(zombie_Spawn);
			}

			if(rand() % 500 + 1 == 69){
				make_Form_B(zombie_Spawn);
			}

			if(rand() % 500 + 1 == 69){
				make_Form_C(zombie_Spawn);
			}

			if(life == 0){
				play = 0;
				enemies.clear();
				fps = 90;
				hscore = score;
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
	destroy_bitmap(life_heart);
	destroy_bitmap(menu_play);
	destroy_bitmap(menu_quit);
	destroy_bitmap(menu_credits);
	destroy_bitmap(credits);
	destroy_font(font);
	destroy_sample(shot_Sound);
	destroy_sample(zombie_Spawn);
	destroy_sample(zombie_DeathA);
	destroy_sample(zombie_DeathB);
	destroy_sample(player_Damage);
	destroy_sample(map_Hit);
	destroy_sample(restore_Life);



	return 0;
}
END_OF_MAIN();

//Function for player control
void player_Control(SAMPLE *shot_Sound){
		   if(key[KEY_RIGHT] && Player.bx < 800) {Player.bx += 14; player_Direction = 3; move_Sprite_Player++;}
	else if(key[KEY_LEFT]  && Player.bx > 220)   {Player.bx -= 14; player_Direction = 2; move_Sprite_Player++;}
	else if(key[KEY_DOWN]  && Player.by < 480)   {Player.by += 14; player_Direction = 0; move_Sprite_Player++;}
	else if(key[KEY_UP]    && Player.by >  20)   {Player.by -= 14; player_Direction = 1; move_Sprite_Player++;}
	else if(key[KEY_SPACE] && gun_Cooldown <= 0){play_sample(shot_Sound, 100, 128, 1000, 0); gun_Shot = 1; fire.bx = Player.bx + 33; fire.by = Player.by + 52; gun_Cooldown = 13;}
	else{move_Sprite_Player = 0; player_Direction = 3;}

	if(move_Sprite_Player > 5) move_Sprite_Player = 0;

	if(gun_Shot == 1){
		fire.bx += 50;

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

void make_Form_A(SAMPLE *zombie_Spawn){
	int y = rand() % 3 + 1;

	for(int i = 1; i <= 3; i++){
		for(int j = 1; j <= 5; j++){
			if(i == 1 && j == 3 || i == 2 && j == 2 || i == 2 && j == 4 || i == 3 && j == 1 || i == 3 && j == 3 || i == 3 && j == 5){
				
				enemy_Object aux;
		
				aux.id  =   0;
				aux.bx  =   800 + i * 100;
			    aux.by  =   y * 50 + j * 50;
				aux.sx  =   0;
				aux.sy  =   0;
				aux.ow  =  50;
				aux.oh  =  89;
				aux.mov = 	0;
			
				enemies.push_back(aux);

				play_sample(zombie_Spawn, 255, 128, 1000, 0);
			}
		}
	}
}

void make_Form_B(SAMPLE *zombie_Spawn){
	int y = rand() % 3 + 1;

	for(int i = 1; i <= 4; i++){
		for(int j = 1; j <= 4; j++){
			if(i == 1 && j == 2 || i == 1 && j == 3 || i == 2 && j == 1 || i == 2 && j == 4 || i == 3 && j == 1 || i == 3 && j == 4 || i == 4 && j == 2 || i == 4 && j == 3){
				
				enemy_Object aux;
		
				aux.id  =   0;
				aux.bx  =   800 + i * 50;
			    aux.by  =   y * 70 + j * 50;
				aux.sx  =   0;
				aux.sy  =   0;
				aux.ow  =  50;
				aux.oh  =  89;
				aux.mov = 	0;
			
				enemies.push_back(aux);

				play_sample(zombie_Spawn, 255, 128, 1000, 0);
			}
		}
	}
}

void make_Form_C(SAMPLE *zombie_Spawn){
	int y = rand() % 3 + 1;

	for(int i = 1; i <= 4; i++){
		for(int j = 1; j <= 4; j++){
			if(i == j){
				
				enemy_Object aux;
		
				aux.id  =   0;
				aux.bx  =   800 + i * 100;
			    aux.by  =   y * 100 + j * 50;
				aux.sx  =   0;
				aux.sy  =   0;
				aux.ow  =  50;
				aux.oh  =  89;
				aux.mov = 	0;
			
				enemies.push_back(aux);

				play_sample(zombie_Spawn, 255, 128, 1000, 0);
			}
		}
	}
}

int colid(int Ax, int Ay, int Bx, int By, int Aw, int Ah, int Bw, int Bh){
	if(Ax + Aw > Bx && Ax < Bx + Bw && Ay + Ah > By && Ay < By + Bh) return 1;
	return 0;
}

void move_Enemy(BITMAP *enemy, BITMAP *enemy_hit, BITMAP *buffer, SAMPLE *map_Hit, SAMPLE *zombie_DeathA, SAMPLE *zombie_DeathB, SAMPLE *player_Damage){
	int i = 0;
	int j = 0;

	for(i = 0; i < enemies.size(); i++){
		if(enemies[i].bx > 220){
			enemies[i].bx -= 6;

			masked_blit(enemy, buffer, enemies[i].sx + enemies[i].mov * 100, enemies[i].sy, enemies[i].bx, enemies[i].by, enemies[i].ow, enemies[i].oh);

			if(enemies[i].mov > 5){enemies[i].mov = 0;}
			else{enemies[i].mov += 1;}

			if(colid(enemies[i].bx, enemies[i].by, fire.bx, fire.by, enemies[i].ow, enemies[i].oh, fire.ow, fire.oh)){
					masked_blit(enemy_hit, buffer, enemies[i].sx + enemies[i].mov * 100, enemies[i].sy, enemies[i].bx, enemies[i].by, enemies[i].ow, enemies[i].oh);
					gun_Shot 			= 0;
					gun_Cooldown -= 3;
					score  		   += 1;
					enemies.erase(enemies.begin() + i);
					
					int s = rand() % 100;
					if(s % 2 == 0){
						play_sample(zombie_DeathA, 255, 128, 1000, 0);
					} else {
						play_sample(zombie_DeathB, 255, 128, 1000, 0);
					}	
			}

			if(colid(enemies[i].bx, enemies[i].by, Player.bx, Player.by, enemies[i].ow, enemies[i].oh, Player.ow - 10, Player.oh - 10)){
					masked_blit(enemy_hit, buffer, enemies[i].sx + enemies[i].mov * 100, enemies[i].sy, enemies[i].bx, enemies[i].by, enemies[i].ow, enemies[i].oh);
					enemies.erase(enemies.begin() + i);
					sort();
					life  -= 1;
					score += 1;
					play_sample(player_Damage, 255, 128, 1000, 0);
			}

		} else{
			enemies.erase(enemies.begin() + i);
			life -= 1;
			play_sample(map_Hit, 255, 128, 1000, 0);
		}
	}
}


//g++ -o Main.bin Main.cpp `allegro-config --libs`
