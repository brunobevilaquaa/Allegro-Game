#include <iostream>
#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>

//Dimensions
//player = 62 x 82 | sx = 150 sy = 117
//enemy  = 60 x 76 | sx = 127

//Structs
struct Object{
	int bx, by, swx, swy, pw, ph;
};

struct Enemy_Object{
	int bx, by, swx, swy, pw, ph, id;
	struct Enemy_Object *previous, *next;
};

Enemy_Object *create_enemy(Enemy_Object *R, int id, int bx, int by){
    Enemy_Object *build, *aux;
    
    build = (Enemy_Object *)malloc(sizeof(Enemy_Object));  
    build->id  = id;
	build->bx  = 900 - bx * 100;
	build->by  = by * 90;
	build->swx =   0;
	build->swy =   0;
	build->pw  =  60;
	build->ph  =  76;
    build->next = NULL;
    build->previous = NULL;
    
    if (R == NULL){
		return build;    
	}

    aux = R;

    while (aux->next != NULL)
		aux = aux->next;
    
    build->previous = aux;
    aux->next = build;
	return build;
}

void clear(Enemy_Object *p){
	Enemy_Object *aux;
	while (p!=NULL)
	{
	   aux=p->next;
	   free(p);
	   p=aux;
    }
}

int colid(int Ax, int Ay, int Bx, int By, int Aw, int Ah, int Bw, int Bh){
	if(Ax + Aw > Bx && Ax < Bx + Bw && Ay + Ah > By && Ay < By + Bh) return 1;		
		return 0;
}

Enemy_Object *kill_Enemy(Enemy_Object *h, int vl)
{
	Enemy_Object *current = h;
	if (h == NULL) return NULL; // lista vazia
	
	while (current->next != NULL){//se tiver 1 elemento na lista não entra no while{
		if(current->id == vl){
			
			if(current == h){
				h = current->next;//o H que estava apntando para o primeiro, aponta pro proximo.
                h->previous = NULL;//como o primeiro elementro sera excluido o proximo deverá apontar para NULL, para se tornar 1 elemento;
				free(current);
				return h;
			}
            current->next->previous = current->previous;
            current->previous->next = current->next;
            free(current);
            return h;
	    }
        current = current->next;
    }
    if (current == h)//vem para ca caso so tenha 1 elemento, ou o elemento a ser deletado e o ultimo;
    {
		free(current);
		return NULL;
	}
    current->previous->next = NULL;
	free(current);
	return h;
}

void move_enemy(Enemy_Object *R, BITMAP *enemy ,BITMAP *buffer, int move_Sprite_Enemy, int *life){
	Enemy_Object *Aux = R;

	while(Aux->next != NULL){
		if(Aux->bx > 220){
			Aux->bx -= 7;
			masked_blit(enemy, buffer, Aux->swx + move_Sprite_Enemy * 127, Aux->swy, Aux->bx, Aux->by, Aux->pw, Aux->ph);
		}

		Aux = Aux->next;
	}
}

void perc(Enemy_Object *R){
	Enemy_Object *Aux = R;

		while(Aux != NULL){
			printf("%d\n\n", Aux->id);
			Aux = Aux->next;
		}
}

void control();

//Objectsove_spove_sp
struct Object Player = {220, 260, 9, 0, 62, 82};
struct Object fire   = {900, 800, 0, 0, 24, 12};

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

Enemy_Object *Root = NULL;
Enemy_Object *Aux  = NULL;

//Variables of menu
int play = 0;
int menu = 0;

//Variables of game
int fps  = 90;
int life =  9;

int x = 0;


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
	BITMAP *player      = load_bitmap("../Sprites/Game/player1.bmp", NULL);
	BITMAP *enemy       = load_bitmap("../Sprites/Game/enemy1.bmp", NULL);
	BITMAP *background  = load_bitmap("../Sprites/Game/background_1.bmp", NULL);
	BITMAP *sprite_Shot = load_bitmap("../Sprites/Game/fire1.bmp", NULL);
	
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
				 if(menu == 0){play = 1; fps = 35;}
				 if(menu == 2) break;
			}
			
				 if(menu == 0) masked_blit(menu_play, buffer,    0, 0, 0, 0, 900, 600);
			else if(menu == 1) masked_blit(menu_options, buffer, 0, 0, 0, 0, 900, 600);
			else if(menu == 2) masked_blit(menu_quit, buffer,    0, 0, 0, 0, 900, 600);
			
		}

		else{
			control();
			masked_blit(background, buffer, 0, 0, 0, 0, 900, 600);
			masked_blit(player, buffer, Player.swx + move_Sprite_Player * 150, Player.swy + player_Direction * 117, Player.bx, Player.by, Player.pw, Player.ph);
			masked_blit(sprite_Shot, buffer, fire.swx, fire.swy, fire.bx, fire.by, fire.pw, fire.ph);
			masked_blit(sprite_Shot, buffer, fire.swx, fire.swy, fire.bx, fire.by, fire.pw, fire.ph);
			
			enemy_Spawn = (rand()%40);
			
			if(enemy_Spawn == 0){
				spawn_Place_Y = 1 + (rand()%5);
				spawn_Place_X = 1 + (rand()%3);
				
				if(Root == NULL){
					Root = create_enemy(Root, id, spawn_Place_X, spawn_Place_Y);
				}
				else{
					create_enemy(Root, id, spawn_Place_X, spawn_Place_Y);	
				}
			}
			
			if(Root != NULL){
				Aux = Root;
				while(Aux->next != NULL){
					if(colid(Aux->bx, Aux->by, fire.bx, fire.by, Aux->next->pw, Aux->next->ph, fire.pw, fire.ph)){
						Root = kill_Enemy(Root, Aux->id);
						gun_Shot = 0;
						gun_Cooldown -= 7;
					}
					
					Aux = Aux->next;
				}
			
				move_enemy(Root, enemy, buffer, move_Sprite_Enemy, &life);
				if(life == 0){play = 0; fps = 90; clear(Root);}

				if(Root == NULL){
					printf("NULL\n");
				}

				printf("%d\n", life);

			}
			
			move_Sprite_Enemy++;
			if(move_Sprite_Enemy > 6) move_Sprite_Enemy = 0;

		}

		draw_sprite(screen, buffer, 0, 0);
		rest(fps);
		clear(buffer);
	}
	
	//Destroy all bitmaps
	clear(Root);
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
	else if(key[KEY_SPACE] && gun_Cooldown   == 0){gun_Shot = 1; fire.bx = Player.bx + 33; fire.by = Player.by + 52; gun_Cooldown = 15;}
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

//g++ -o Main.bin Main.cpp `allegro-config --libs`
