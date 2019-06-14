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

printf("El Camino del Samurai:  Juego desarrollado por Lucas Garcia Mateu (Skyway) \n\n\n\n Bienvenido al camino del samurai. En esta aventura textual con elementos de RPG, deberas escribir por tal de llevar a cabo acciones con tu personaje. Te moveras por un mapa encontrando botines, peleando contra maleantes, descubriendo la historia de tu pasado y escribiendo en cada momento tu presente y futuro.\n Hagamos un pequeño tutorial basico: \n \n ");
char orden[100] = "trololol";

tutorial();

struct samurai samurai;

printf(" Como te llamas?\n\n Escribe tu orden: ");
scanf_s("%s", samurai.name, 100);

while (true)
{
	char orden[100] = "trololol";
	int position_num = mapa[position.x][position.y];
	if (!samurai.mirar_inventario)
	{ 
	
		if (position_num == 61)
		{
			if (!samurai.first_61)
			{ 
				position_61_first();
				samurai.first_61 = true;
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
			if (!samurai.first_72)
			{ 
			position_72_first(&samurai);
			samurai.first_72 = true;
			}
			else
			{
				position_72();
			}
		}
		if (position_num == 82)
		{
			if (!samurai.first_82)
			{
				position_82_first(&samurai);
				samurai.first_82 = true;
			}
			else
			{
				position_82();
			}
		}
		if (position_num == 92)
		{
			if (!samurai.first_92)
			{
				position_92_first(&samurai);
				samurai.first_92 = true;
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
			if (!samurai.first_80)
			{ 
				position_80_first(&samurai);
				samurai.first_80 = true;
			}
			else
			{
				position_80(&samurai);
			}
		}
		if (position_num == 81)
		{
			if (!samurai.first_81)
			{
				position_81_first();
				samurai.first_81 = true;
			}
			else
			{
				position_81();
			}
		}
		if (position_num == 43)
		{
			if (!samurai.first_43)
			{ 
				position_43_first(&samurai);
				samurai.first_43 = true;
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