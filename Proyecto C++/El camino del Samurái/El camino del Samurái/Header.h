#ifndef __HEADER__
#define __HEADER__

struct samurai
{
	//Stats
	char name[100];
	int vida = 10;
	int ataque = 0;
	int mana = 0;
	int max_vid = 10;
	int max_man = 0;
	//Modificadores de stats
	int katana = 0;
	int kimono = 0;
	//Consumables
	int sushis = 0;
	int sakes = 0;
	int llaves = 0;

	//Objetos de la historia
	bool bandana = false;
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
	bool enseñar_foto = false;

	//Estados del juego
	bool mirar_inventario = false;
};
struct position
{
	int x = 0;
	int y = 0;
};

void squared_map_creation(int*, int );
bool string_comparator(char*, char*);
void movement(position*, char*);
void tutorial();
void orden_com(char*);
void inventario(samurai*);
void position_61_first();
void bosque();
void aldea_destruida();
void position_72_first(samurai*);
void position_72();
void position_82_first(samurai*);
void position_82();
void position_92_first(samurai*);
void position_92(samurai*);
void position_90(samurai*);
void position_80_first(samurai*);
void position_80(samurai*);
void position_81_first();
void position_81();
void position_43_first(samurai*);
void position_43(samurai*);
int input_number(int max_number);
void position_62();


#endif //__HEADER__