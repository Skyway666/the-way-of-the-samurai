#ifndef __HEADER__
#define __HEADER__

void squared_map_creation(int*, int );
int string_comparator(char*, char*);
void movement(struct position*, char*);
void tutorial();
void orden_com(char*);
void inventario(struct samurai*);
void position_61_first();
void bosque();
void aldea_destruida();
void position_72_first(struct samurai*);
void position_72();
void position_82_first(struct samurai*);
void position_82();
void position_92_first(struct samurai*);
void position_92(struct samurai*);
void position_90(struct samurai*);
void position_80_first(struct samurai*);
void position_80(struct samurai*);
void position_81_first();
void position_81();
void position_43_first(struct samurai*);
void position_43(struct samurai*);
void comprovador_binario(char*);
void position_62();
struct samurai
{
	//Stats
	char name[100];
	int vida;
	int ataque;
	int mana;
	int max_vid;
	int max_man;
	//modificadores de stats
	int katana; 
	int kimono;
	int bandana;
	//modificadores de stats
	//consumables
	int sushis;
	int sakes;
	int llaves;
	//consumables
	//Objetos de la historia
	int foto_viejo_loco;
	int llave_casa_viejo_loco;
	int saber_usar_mana;
	int bola_pinchos;
	int pincel;
	int romper_madera_mano;
	int huevo_gallina;
	int perro;
	int hacha;
	int cabezas_panda;
	int vela;
	//Objetos de la historia
	//Momentos de la historia
	int first_61;
	int first_72;
	int first_82;
	int first_92;
	int first_80;
	int first_81;
	int first_43;
	int hablar_viejo;
	int katana_kimono;
	int enseñar_foto;
	//Momentos de historia
	//Estados del juegp
	int mirar_inventario;
	


};
struct position
{
	int x;
	int y;
};





























#endif //__HEADER__