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
	//Modificadores de stats
	int katana; 
	int kimono;
	int bandana;
	//Consumables
	int sushis = 0;
	int sakes = 0;
	int llaves = 0;

	//Objetos de la historia
	bool foto_viejo_loco = false;
	bool llave_casa_viejo_loco = false;
	bool saber_usar_mana = false;
	bool bola_pinchos = false;
	bool pincel = false;
	bool romper_madera_mano = false;
	bool huevo_gallina = false;
	bool perro = false;
	bool hacha = false;
	int cabezas_panda = 0;
	bool vela = false;
	//Momentos de la historia
	bool first_61 = false;
	bool first_72 = false;
	bool first_82 = false;
	bool first_92 = false;
	bool first_80 = false;
	bool first_81 = false;
	bool first_43 = false;
	bool hablar_viejo = false;
	bool katana_kimono = false;
	bool ense�ar_foto = false;

	//Estados del juego
	bool mirar_inventario = false;
	


};
struct position
{
	int x;
	int y;
};





























#endif //__HEADER__