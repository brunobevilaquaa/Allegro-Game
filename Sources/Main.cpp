#include <allegro.h>
#include <stdio.h>

//Dimensions
//player = 75 x 83
//enemy  = 23 x 49


//Functions
void control();


//Structs
struct Object{
	int bx, by, swx, swy, pw, ph;
};


//Objects
struct Object Player = {220, 260, 0, 0, 75, 83};
struct Object Enemy  = {800, 250, 0, 0, 46, 98};

//Variables
int sprite = 0;
int sprite_enemy = 0; 
int player_Direction = 0;
int velocity = 1; 
int gravity = 4; 
int play = 0;
int menu = 0;
int fps = 90;

int main(){

	//Inicialization
	allegro_init();
	install_timer();
	install_keyboard();
	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 900, 600, 0, 0);
	set_window_title("Zombie Invader");

	//Load Bitmaps Game
	BITMAP *buffer     = create_bitmap(900, 600);
	BITMAP *player     = load_bitmap("../Sprites/player.bmp", NULL);
	BITMAP *enemy      = load_bitmap("../Sprites/enemy.bmp", NULL);
	BITMAP *background = load_bitmap("../Sprites/background_1.bmp", NULL);
	
	

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
				if(menu == 0){play = 1; fps = 40;}
				if(menu == 2) break;
			}
			
			if(menu == 0){
				masked_blit(menu_play, buffer, 0, 0, 0, 0, 900, 600);
			}	
			else if(menu == 1){
				masked_blit(menu_options, buffer, 0, 0, 0, 0, 900, 600);
			}
			else if(menu == 2){
				masked_blit(menu_quit, buffer, 0, 0, 0, 0, 900, 600);
			}
 
		}

		else{
			control();
			masked_blit(background, buffer, 0, 0, 0, 0, 900, 600);
			masked_blit(player, buffer, Player.swx + sprite * 150, Player.swy + player_Direction * 117, Player.bx, Player.by, Player.pw, Player.ph);
			masked_blit(enemy, buffer, Enemy.swx + sprite_enemy * 127, Enemy.swy, Enemy.bx, Enemy.by, Enemy.pw, Enemy.ph);
		}

		draw_sprite(screen, buffer, 0, 0);
		
		rest(fps);
		clear(buffer);
	}
	
	//destroy all bitmaps
	destroy_bitmap(buffer);
	destroy_bitmap(player);
	destroy_bitmap(enemy);
	destroy_bitmap(background);

	return 0;
}


END_OF_MAIN();

//Funciont for player control
void control(){
		 if(key[KEY_RIGHT] && Player.bx < 800) {Player.bx += 10; player_Direction = 3; sprite++;}
	else if(key[KEY_LEFT]  && Player.bx > 220) {Player.bx -= 10; player_Direction = 2; sprite++;}
	else if(key[KEY_DOWN]  && Player.by < 480) {Player.by += 10; player_Direction = 0; sprite++;}
	else if(key[KEY_UP]    && Player.by >  20) {Player.by -= 10; player_Direction = 1; sprite++;}
	else {sprite = 0; player_Direction = 3;}
	
	if(sprite > 4) sprite = 0;

	//if(Enemy.bx > 230){Enemy.bx -= 4; }//sprite_enemy++;}
	//else{sprite_enemy = 0;}

	if(sprite_enemy > 6) sprite_enemy = 0;

}

void control_Menu(){

}

//g++ -o Main Main.cpp `allegro-config --libs`