#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include "header.h"

int main()
{
int mapa[10][10];
squared_map_creation(&mapa[0][0], 10);
struct position position;
	position.x = 6;
	position.y = 1;


int game_over= 0;

printf("El Camino del Samurai:  Juego desarrollado por Lucas Garcia Mateu (Skyway) \n\n\n\n Bienvenido al camino del samurai. En esta aventura textual con elementos de RPG, deberas escribir por tal de llevar a cabo acciones con tu personaje. Te moveras por un mapa encontrando botines, peleando contra maleantes, descubriendo la historia de tu pasado y escribiendo en cada momento tu presente y futuro.\n Hagamos un pequeño tutorial basico: \n \n ");
char orden[100] = "trololol";

//tutorial(&position, orden, mapa);
struct samurai samurai;
samurai.first_61 = 0;
samurai.first_72 = 0;
samurai.first_82 = 0;
samurai.first_92 = 0;
samurai.first_80 = 0;
samurai.first_81 = 0;
samurai.first_43 = 0;
samurai.sushis = 0;
samurai.sakes = 0;
samurai.llaves = 0;
samurai.bandana = 0;
samurai.kimono = 0;
samurai.katana = 0;
samurai.foto_viejo_loco = 0;
samurai.llave_casa_viejo_loco = 0; 
samurai.bola_pinchos = 0;
samurai.pincel = 0;
samurai.romper_madera_mano = 0;
samurai.huevo_gallina = 0;
samurai.perro = 0;
samurai.hacha = 0;
samurai.cabezas_panda = 0;
samurai.vela = 0;
samurai.mirar_inventario = 0;
samurai.hablar_viejo = 0;  
samurai.katana_kimono = 0;
samurai.enseñar_foto = 0;

printf(" Como te llamas?\n\n Escribe tu orden: ");
scanf_s("%s", samurai.name, 100);

while (game_over == 0)
{
	char orden[100] = "trololol";
	int position_num = mapa[position.x][position.y];
if (samurai.mirar_inventario == 0)
{ 
	
	if (position_num == 61)
	{
		if (samurai.first_61 == 0)
		{ 
			position_61_first();
		    samurai.first_61++;
		}
		else
		{
			bosque();
		}

	}
	if (position_num == 71)
	{
		aldea_destruida();
	}
	if (position_num == 70)
	{
		aldea_destruida();
	}
	if (position_num == 91)
	{
		aldea_destruida();
	}
	if (position_num == 72)
	{
		if (samurai.first_72 == 0)
		{ 
		position_72_first(&samurai);
		samurai.first_72++;
		}
		else
		{
			position_72();
		}
	}
	if (position_num == 82)
	{
		if (samurai.first_82 == 0)
		{
			position_82_first(&samurai);
			samurai.first_82++;
		}
		else
		{
			position_82();
		}
	}
	if (position_num == 92)
	{
		if (samurai.first_92 == 0)
		{
			position_92_first(&samurai);
			samurai.first_92++;
		}
		else
		{
			position_92(&samurai);
		}
	}
	if (position_num == 90)
	{
		position_90(&samurai);
	}
	if (position_num == 80)
	{
		if (samurai.first_80 == 0)
		{ 
		position_80_first(&samurai);
		samurai.first_80 ++;
		}
		else
		{
			position_80(&samurai);
		}
	}
	if (position_num == 81)
	{
		if (samurai.first_81 == 0)
		{
			position_81_first();
			samurai.first_81++;
		}
		else
		{
			position_81();
		}
	}
	if (position_num == 43)
	{
		if (samurai.first_43 == 0)
		{ 
		position_43_first(&samurai);
		samurai.first_43 = 1;
		}
		else
		{
			position_43(&samurai);
		}
	}
	if (position_num == 62)
	{
		position_62();
	}
	
}
	orden_com(orden);
	if (!(string_comparator(orden, "arriba") != 1 && string_comparator(orden, "abajo") != 1 && string_comparator(orden, "derecha") != 1 && string_comparator(orden, "izquierda") != 1))
	{ 
	movement(&position,orden);
	samurai.mirar_inventario = 0;
	}
	printf("\nTu posicion es %i", mapa[position.x][position.y]);

	if (string_comparator(orden, "inventario") == 1)
	{
		inventario(&samurai);
		samurai.mirar_inventario = 1;
	}
	
}

return 0;

}