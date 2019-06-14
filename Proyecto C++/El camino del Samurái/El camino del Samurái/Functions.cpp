#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include "header.h"

void squared_map_creation(int* mapa, int length)
{
	int counter = 0;
	while (counter < length*length)
	{
		mapa[counter] = counter;
		counter++;
	}
}

int string_comparator(char* str, char*str2)
{
	int counter = 0;
	int counter2 = 0;
	while (counter != strlen(str2))
	{
		if (str[counter] == str2[counter])
		{
			counter2++;
		}
		counter++;
	}
	if (counter == counter2)
		return 1;
	else
		return 0;
}
void comprovador_binario(char* orden)
{
	while (string_comparator(orden, "1") == 0 && string_comparator(orden, "2") == 0)
	{
		scanf_s("%s", orden, 100);
		if (string_comparator(orden, "1") == 0 && string_comparator(orden, "2") == 0)
		{
			printf("\n\nEse no es un comando valido aqui\n");
		}
	}
}
void movement(struct position * position, char* orden)
{
	if (string_comparator(orden, "arriba") == 1)
	{
		position->x--;
	}
	if (string_comparator(orden, "abajo") == 1)
	{
		position->x++;
	}
	if (string_comparator(orden, "derecha") == 1)
	{
		position->y++;
	}
	if (string_comparator(orden, "izquierda") == 1)
	{
		position->y--;
	}

	if (position->x < 0)
	{
		position->x = 0;
		printf("\nNo intentes salir del mapa o te perderas!");
	}
	if (position->x > 9)
	{
		position->x = 9;
		printf("\nNo intentes salir del mapa o te perderas!");
	}
	if (position->y < 0)
	{
		position->y = 0;
		printf("\nNo intentes salir del mapa o te perderas!");
	}
	if (position->y > 9)
	{
		position->y = 9;
		printf("\nNo intentes salir del mapa o te perderas!");
	}
	
}
void tutorial()
{
	int tutorial_step = 0;
	while (true)
	{
		char* movement_objective = "undefined";
		char orden[100] = "none";
		if (tutorial_step == 0) {
			printf("\nMuevete hacia arriba (escribe 'arriba' y presiona 'enter'. No apretes 'espacio' en ningun momento, pasaran cosas raras)\n\n");
			movement_objective = "arriba";
		}
		if (tutorial_step == 1) {
			printf("\nMuy bien! Ahora abajo.");
			movement_objective = "abajo";
		}
		if (tutorial_step == 2) {
			printf("\nMuy bien! Ahora izquierda.");
			movement_objective = "izquierda";
		}
		if (tutorial_step == 3) {
			printf("\nMuy bien! Ahora derecha.");
			movement_objective = "derecha";
		}
		if (tutorial_step == 4) {
			printf("\nFelicidades! Has completado con exito el tutorial y puedes empezar tu aventura. Moverse no es la única mecanica del juego, pero ya se te explicaran conforme vayas avanzando. Ahora que vas a empezar, te recomiendo que hagas caso de los consejos del juego, ya que de no hacerlo, muy probablemente moriras, y la pena a pagar es volver a empezar el juego desde el principio.\n\n\n\n");
			return;
		}
		
		printf("\nEscribe tu orden:  ");
		orden_com(orden);

		if (string_comparator(orden, movement_objective) == 1)
			tutorial_step++;
		else{
			printf("Parece que has cometido un error! Vuelve a empezar el tutorial.");
			tutorial_step = 0;
		}
	}
}
void position_61_first()
{
	printf("\n\n\n Despiertas en un bosque, escondido tras unos matorrales. La noche anterior tu pueblo fue atacado, tu fuiste capturado de los primeros. El soldado que te llevaba atado, encontro tan pateticas tus suplicas y llantos que decidio matarte de un golpe en la cabeza y esconderte tras unos matorrales. Parece que estas de suerte, una cabeza dura siempre es una bendición. Con la cabeza dolorida, te levantas del matorral y miras a tu alrededor. Detras se encuentra la aldea, y desde la lejania se puede ver que nada bueno ha pasado la noche anterior. Deberias ir a explorarla muchacho\n\n\n Escribe tu orden:");
}
void bosque()
{
	printf("\n\n\n Estas en un bosque\n\n\n Escribe tu orden:");
}
void aldea_destruida()
{
	printf("\n\n\n La aldea esta arrasada. Alla donde miras hay casas quemadas y cadaveres.\n\n\n Escribe tu orden:");
}
void position_72_first(struct samurai* samurai)
{
	printf("\n\n\n Te encuentras uno de los soldados encargados de defender la aldea. Se encuentra con una lanza que le atraviesa de lado a lado, ensartado en un arbol. Te acercas para ver si lleva algo de valor encima. Metes la mano en su bolsillo y de repente notas como se mueve. El soldado tose sangre en tu cara y tu retrocedes un paso asustado. Acto siguiente intentas sacarle la lanza del pecho, y el soldado pronuncia las siguientes palabras: \n\n 'Ni lo intentes joven %s. Yo ya estoy muerto, lo unico que conseguiras es prologar mi sufrimiento. Ademas, dudo que puedas sujetar esta lanza con esos bracitos de princesa, jajaj *cof**cof*... Maldita sea chico, eran como 100 mercenarios sedientos de sangre y mujeres contra 30 civiles y dos soldados. Malditos cobardes... Han matado a los hombres y niños, y se han llevado a todas las mujeres y niñas, excepto a las que han opuesto resistencia, a esas las han violado y matado... No habia visto una masacre asi en todos mis años de servicio, es terrible. Ese perro de Iruka Donov pagara algun dia por todos sus viles actos. Es un enfermo que se dedica a saquear aldeas indefensas, se lleva las riquezas y mujeres, y dios sabe que hace con ellas *cof**cof*. No sé como has sobrevivido chico, pero sera mejor que te largues y reagas tu vida, por donde pasa Iruka no vuelve a crecer la vida, y todo el norte ya esta reclamado como su imperio. Se que no dormiras tranquilo despues de lo que has visto, pero la vida es injusta y no tienes otra opcion... A mi ya me ha llegado la hora, nos vemos en el otro lado' \n\n Con un ultimo suspiro, el soldado muere silenciosamente. Donde estaran tus padres?\n\n\n Escribe una orden:", samurai->name);
}
void position_72()
{
	printf("\n\n\n Hay un soldado ensartado en un arbol por una lanza. Esta muerto\n\n\n Escribe una orden:");
}
void position_82_first(struct samurai* samurai)
{
	printf("\n\n\n Ahi esta el bar de la aldea. El unico amigo que tenias en la aldea murio hace tiempo por una pelea en ese bar, desde entonces nunca mas habias vuelto a esa zona. El bar esta completamente destrozado, pero te rugen las tripas, y quieres entrar a ver si queda algo que valga la pena. Entras y encuentras en la despensa del local una botella de sake y un sushi. \n\n\n Apunte para el jugador: Puedes acceder al inventario una vez hayan terminado los eventos del lugar en el que estas escribiendo 'inventario'. Puedes probarlo ahora mismo! \n\n\n Escribe una orden:");
	samurai->sushis++;
	samurai->sakes++;
}
void position_82()
{
	printf("\n\n\n Es un bar desolado sin nada de interes dentro. \n\n\n Escribe tu orden: ");
}
void position_92_first(struct samurai* samurai)
{
	printf("\n\nA lo lejos ves una mujer tirada en el suelo. Esta completamente desnuda encima de un charco de sangre. Te acercas poco a poco y ves que es tu madre. Tiene marcas de golpes por todos lados, y el cuerpo cubierto de esperma. Ambos pechos han sido extirpados y tiene una katana metida en la vagina. Es duro, pero yo de tí cogeria esa katana, si vas a ir por ahí solo la vas a necesitar.\n\n\n Escribe '1' para coger la katana\n\n Escribe '2' para dejarla en el sitio\n\n\n Escribe tu orden:");
	char orden[100]= "trololo";
	comprovador_binario(orden);
    
	if (string_comparator(orden, "1") == 1)
	{
		printf("\n\nHas adquirido la 'katana oxidada'");
		samurai->katana = 1;
	}
	else
	{
		printf("\n\nEntiendo que sea duro, pero deberias haberla cogido... ");
	}
	if (samurai->katana >= 1 && samurai->kimono >= 1 && samurai->katana_kimono == 0)
	{
		printf("\n\n\nApunte para el jugador : Que hayas adquirido el kimono significa que tienes vida maxima.Si ademas tienes la katana, ya puedes combatir.Coger mejores kimonos aunmentara tu vida maxima y coger mejores katanas aumentara tu daño de combate.Puedes ver tu vida accediendo a 'inventario' puedes curarte en cualquier momento escribiendo 'sushi'.Si tu vida esta al maximo, el sushi se gastara pero no tendra ningun efecto.");
		samurai->katana_kimono = 1;
	}
	printf("\n\n\n Escribe tu orden:");
}
void position_92(struct samurai* samurai)
{
	if (samurai->katana == 1)
	{
		printf("\n\nAhi esta el cadaver de tu madre con los senos extirpados\n\n\n Escribe tu orden:");
	}
	else
	{
		printf("\n\nAhi esta el cadaver de tu madre con los senos extirpados y una katana metida en la vagina.\n\n\n Escribe '1' para coger la katana\n\n Escribe '2' para dejarla en el sitio\n\n\n Escribe tu orden: ");
		char orden[100] = "trololo";
		comprovador_binario(orden);
		if (string_comparator(orden, "1") == 1)
		{
			printf("\n\nHas adquirido la 'katana oxidada'");
			samurai->katana = 1;
		}
		else
		{
			printf("\n\nEntiendo que sea duro, pero deberias haberla cogido... ");
		}
		if (samurai->katana >= 1 && samurai->kimono >= 1 && samurai->katana_kimono == 0)
		{
			printf("\n\n\nApunte para el jugador : Que hayas adquirido el kimono significa que tienes vida maxima.Si ademas tienes la katana, ya puedes combatir.Coger mejores kimonos aunmentara tu vida maxima y coger mejores katanas aumentara tu daño de combate.Puedes ver tu vida accediendo a 'inventario' puedes curarte en cualquier momento escribiendo 'sushi'.Si tu vida esta al maximo, el sushi se gastara pero no tendra ningun efecto.");
			samurai->katana_kimono = 1;
		}
		printf("\n\n\n Escribe tu orden:");
	}
}
void position_90(struct samurai* samurai)
{
	printf("\n\nTe encuentras frente a la casa del viejo loco de la aldea. En la puerta hay una nota:\n\n 'Si encuentras esto ven a verme. Para encontrarme debes dar 5 pasos arriba y 3 a la derecha'");
	if (samurai->llave_casa_viejo_loco == 0)
	{
		printf("\n\nNecesitas la llave de la casa del viejo loco para entrar. \n\n\n Escribe tu orden:");
	}
	else
	{
		printf("\n\nTienes la llave de la casa del viejo loco. Deseas entrar? \n\n Escribe '1' para entrar \n Escribe '2' para seguir tu camino\n\n\n Escribe tu orden:");
		char orden[100] = "trololo";
		comprovador_binario(orden); 
		if (string_comparator(orden, "1") == 1)
		{
			printf("\n\nEntras a la casa del viejo loco. Todo esta desordenado y el hedor a excrementos y comida podrida es insoportable. Te adentras en la casa, abriendote paso entre ratas muertas y manchas pegajosas de a saber que substancia encuentras la habitacion de la que el viejo loco te ha hablado. Delante de la cama, en un pedestal, se encuentra el retrato de su familia junto a una bandana. Un rayo de luz ilumina los dos objetos, que dentro del desorden y el mal olor, parecen puros y casi divinos. El viejo loco solo te pidio la foto, pero no puedes dejar esa preciosa bandana blanca allí, y decides cogerla. Sales de la casa, y te colocas la bandana en la frente.\n\n\n Escribe tu orden:");
			samurai->foto_viejo_loco = 1;
			samurai->bandana = 1;
		}
		if (string_comparator(orden, "2") == 1)
		{
			printf("\n\n Llamamos loco a quien es diferente. Quien dice que los cuerdos seamos nosotros?\n\n\n Escribe tu orden:");
		}
		
	}
}
void position_80_first(struct samurai* samurai)
{
	printf("\n\n Caminas hacia lo que parece ser una persona ensartada en una pared por varias katanas. Cuando te acercas lo suficiente te das cuenta que es (o era) tu padre. Ambas extremidades superiores le han sido mutiladas y el cuerpo esta lleno de estocadas de katanas. La cara es casi irreconocible, con varios cortes y una espada que atraviesa el craneo de lado a lado y se clava en la pared. LLevaba su kimono de samurai... Parece que aun lo guardaba despues de tantos años de haberse jubilado. Murio luchando con honor. El kimono que lleva es de gran calidad. Aunque ensangrentado y con algunos ajugeros, te servira si has de pelear. \n\n Escribe '1' para coger el kimono \n\n Escribe '2' para irte \n\n Escribe tu orden:");
	char orden[100] = "trololo";
	comprovador_binario(orden);
	if (string_comparator(orden, "1") == 1)
	{
		printf("\n\nEmpiezas a sacar las espadas del cuerpo de tu padre. Debes sujetar su cadaver mientras sacas las espadas, y al tocar su cara notas lo fria que esta y te das cuenta de que nunca volveras a verle sonreir. Sigues sacando las espadas, y cuando solo queda una, su cuerpo cae encima tuyo. Lo tienes que apartar y dejarlo caer al suelo. Debes tumbarlo boca a bajo si quieres sacarle el kimono. El hecho de que no tenga brazos facilita la tarea, aunque preferirias que la última imagen de tu padre fuese la de una muerte natural. Finalmente consigues quitarle el kimono y te lo pones.");
		samurai->kimono = 1;
	}
	else
	{
		printf("\n\nSe que es duro, pero deberias haberlo cogido...");
	}
	if (samurai->katana >= 1 && samurai->kimono >= 1 && samurai->katana_kimono == 0)
	{
		printf("\n\n\nApunte para el jugador : Que hayas adquirido el kimono significa que tienes vida maxima.Si ademas tienes la katana, ya puedes combatir.Coger mejores kimonos aunmentara tu vida maxima y coger mejores katanas aumentara tu daño de combate.Puedes ver tu vida accediendo a 'inventario' puedes curarte en cualquier momento escribiendo 'sushi'.Si tu vida esta al maximo, el sushi se gastara pero no tendra ningun efecto.");
		samurai->katana_kimono = 1;
	}
	printf("\n\n\n Escribe tu orden: ");
}
void position_80(struct samurai* samurai)
{
	if (samurai->kimono == 1)
	{ 
		printf("Tirado en el suelo esta el cadaver de tu padre \n\n\n Escribe tu orden:");
	}
	else
	{
		printf("\n\nTu padre se encuentra ensartado en una pared por varias katanas. Deseas tomar su kimono?\n\n Escribe '1' para coger el kimono \n\n Escribe '2' para irte \n\n Escribe tu orden:");
		char orden[100] = "trololo";
		comprovador_binario(orden);
		if (string_comparator(orden, "1") == 1)
		{
			printf("\n\nEmpiezas a sacar las espadas del cuerpo de tu padre. Debes sujetar su cadaver mientras sacas las espadas, y al tocar su cara notas lo fria que esta y te das cuenta de que nunca volveras a verle sonreir. Sigues sacando las espadas, y cuando solo queda una, su cuerpo cae encima tuyo. Lo tienes que apartar y dejarlo caer al suelo. Debes tumbarlo boca a bajo si quieres sacarle el kimono. El hecho de que no tenga brazos facilita la tarea, aunque preferirias que la última imagen de tu padre fuese la de una muerte natural. Finalmente consigues quitarle el kimono y te lo pones.");
			samurai->kimono = 1;
		}
		else
		{
			printf("Se que es duro, pero deberias haberlo cogido...");
		}
		if (samurai->katana >= 1 && samurai->kimono >= 1 && samurai->katana_kimono == 0)
		{
			printf("\n\n\nApunte para el jugador : Que hayas adquirido el kimono significa que tienes vida maxima.Si ademas tienes la katana, ya puedes combatir.Coger mejores kimonos aunmentara tu vida maxima y coger mejores katanas aumentara tu daño de combate.Puedes ver tu vida accediendo a 'inventario' puedes curarte en cualquier momento escribiendo 'sushi'.Si tu vida esta al maximo, el sushi se gastara pero no tendra ningun efecto.");
			samurai->katana_kimono = 1;
		}
		printf("\n\n\n Escribe tu orden: ");
	}
}
void position_81_first()
{
	printf("\n\nTe encuentras en la plaza principal del pueblo. La gente solia reunirse allí los fines de semana. Los niños jugaban, los abuelos contaban sus historias... Todo eso habia desaparecido para siempre. En medio de la plaza habia una montaña de cadaveres. La paza entera estaba teñida de rojo. No habian tenido piedad con nadie, nisiquiera con los mas pequeños. La mayoria de cadaveres tenian partes del cuerpo mutiladas, y a todos los niños los habian cortado por la mitad. Cortar a un enemigo por la mitad es un enemigo por la mitad denota fuerza en un guerrero, pero nunca pensaste que alguien podia ser tan cobarde como para llevar a cabo esa 'azaña' con un niño. Habia gente atada en arboles que habia sido quemada viva. Las pocas mujeres que habian estaban completamente desnudas y sus cuerpos estaban destrozados. A una mujer embarazada la habian colgado de un arbol. Tiene el vientre abierto y en el suelo estan sus tripas y el feto, aparentemente pisoteados. Todo tu pasado esta destruido, y reemplazado con esa imagen infernal. \n\n\n Escribe tu orden:");
}
void position_81()
{
	printf("\n\nEstas en la plaza, donde una imagen infernal se hiergue ante tus ojos\n\n\n Escribe tu orden:");
}
void orden_com(char *orden)
{
	bool valid_command = false;
	while (!valid_command){
		scanf_s("%s", orden, 100);

		if (string_comparator(orden, "arriba") == 0 && string_comparator(orden, "abajo") == 0 && string_comparator(orden, "derecha") == 0 && string_comparator(orden, "izquierda") == 0 && string_comparator(orden, "inventario") == 0)
			printf("\nEse no es un comando valido\n");
		else
			valid_command = true;
	}
}
void position_43_first(struct samurai *samurai)
{
	printf("\n\n A lo lejos ves al viejo loco del pueblo. Está sentado encima de una roca meditando. Te hacercas a hablar con él:\n\n");

	
		printf("'Hola %s. Parece que has visto el mensaje en mi puerta o me has encontrado por casualidad. Si el caso es el segundo, deberias explorar la aldea, algo terrible ha pasado. Todo el mundo siempre ha pensado que estoy loco, pero es que simplemente no me gusta la gente, ademas ahora están todos muertos y yo estoy aqui... QUIEN ES EL LOCO AHORA EH!!! QUIEN ES EL LOCO AQUI??!\n ...\n...\n Perdón %s, quizás la soledad si queme ha trastocado un poco. Yo solía tener una família, una mujer y un hijo preciosos. Desgraciadamente, mis ansias de poder hicieron que ambos me abandonasen en uno de mis viajes. Ahora soy poseedor de un gran poder, pero me siento miserable sin mi familia... Me gustaría haber podido ver a mi hijo usar el mana un dia de estos, pero ya es tarde para eso, hace demasiado que los perdi. Aqui tengo una foto... Mierda. MIERDA LA PUTA FOTO, DONDE ESTA LA JODIDA FOTO!!! AH MALDITA SEA ME LA HE DEJADO, SIEMPRE CON PRISA, SIEMPRE CON PRISA. Y SI HAN QUEMADO LA CASA?? HIJOS DE PUTA, TENIAN QUE VENIR A MI MALDITA ALDEA... \n...\n... Me he vuelto a alterar verdad? Perdoname. No te vayas aun! Hagamos un trato: Yo quiero esa foto, y tu quieres ser fuerte. Traemela, y te instruire en el arte del uso del mana. Toma las llaves de mi casa. Para el entrenamieno necesitarás tambien una katana y un kimono.' \n\n\n Escribe tu orden:",samurai->name);
		samurai->llave_casa_viejo_loco = 1;

		
}
void position_43(struct samurai *samurai)
{
	printf("\n\n A lo lejos ves al viejo loco del pueblo. Esta sentado encima de una roca meditando. Te hacercas a hablar con él:\n\n");

	if (!(samurai->bandana == 1 && samurai->foto_viejo_loco == 1))
	{
		printf("'Ve a por la foto o dejame meditar en paz'. \n\n\n Escribe tu orden:");
	}
	
	if (samurai->bandana == 1 && samurai->foto_viejo_loco == 1)
    {
		if ( samurai->enseñar_foto == 0)
		{ 
		printf("\n\n'Vaya %s, parece que has recogido la foto... Es una mujer preciosa, verdad? Y ese chico... Bueno, supongo que nu hubiese podido ser un buen padre de todas formas.", samurai->name);
		samurai->enseñar_foto = 1;
		} 
		else
		{
			printf("\n\n'Has vuelto! Aun no me canso de mirar esa foto'");
		}
		if (!(samurai->katana == 1 && samurai->kimono == 1))
		{
			printf("\n\nParece que te faltan la katana o el kimono, vuelve cuando los tengas. \n\n\n Escribe tu orden:");
		}
		if (samurai->katana == 1 && samurai->kimono == 1)
		  { 
		printf("\n\nEstá bien, quieres que te ensenye a usar el mana ? \n\n Escribe '1' para aprender a usar mana \n Escribe '2' para rechazar la oferta.\n\n\n Escribe tu orden : '");
			char orden[100] = "trololo";
			comprovador_binario(orden);
			if (string_comparator(orden, "1") == 1)
			{
				printf("\n\n'Bien chaval, empecemos el entrenamiento. El mana es la energia vital de las personas, y se restaura bebiendo sake. Gracias al mana pudes realizar hechizos que te ayudarán en el combate. El primero hechizo que podrás usar será el de fuego. Hace daño a tu enemigo y le quemará.' \n\n Después de unos duros dias de entrenamiento, aprendes a usar el maná \n\n 'Bien chico, parece que ya sabes usar el mana. Repasemos lo que has aprendido.'");
				// Insertar tutorial aquí
				printf("\n\n\n Escribe tu orden:");
				
				samurai->hablar_viejo = 1;
			}
			else
			{
				printf("\n\n 'Eres un necio. Crees que el mana no existe verdad? Bueno chico, tu mismo, sin lo que te ofrezco no podras sobrevivir alla afuera. Solo espero que cuando mueras me de tiempo a llegar a tu cadaver caliente para poder aliviar las necesidades de un viejo solitario. Vuelve si cambias de opinión.'\n\n\n Escribe tu orden:");
			}
			
		  }
		
     }
}
void position_62()
{
	printf("\n\n Clavado en el suelo, hay un cartel con un mapa. \n\n Leyenda: \n\n 'B'= Bosses \n 'P'= Puertas \n '/' = Murallas \n '@'= Posicion del jugador \n\n");
	char map[10][10] = {
		{'/', '/', '/', '/', '/', '/', '/', 'P', '/', '/'},
		{'B', '-', '-', '-', '/', '-', '-', '-', '-', '-'},
		{'-', '-', '-', 'P', '-', '-', '-', '-', '-', '-'},
		{'/', '/', '/', '-', '-', '-', '-', '-', '-', '-'},
		{'-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
		{'-', '-', '-', '-', '-', '-', '-', '-', '-', '/'},
		{ '-', '-', '@', '-', '-', '-', '-', '-', 'P','-'},
		{ '-', '-', '-', '-', '-', '-', '-', '/','-', '-'},
		{ '-', '-', '-', '-', '-', '-', '/','-', '-', '-'},
		{ '-', '-', '-', '-', '-', '/','B', '-', '-', '-'}


	};
	int counter = 0;
	int counter2 = 0;
	while (counter != 10 || counter2 != 9)
	{
		if (counter % 10 == 0 && counter != 0)
		{
			printf("\n");
			++counter2;
			counter = 0;
		}
		printf("%c ", map[counter2][counter]);

		++counter;
	}
	printf("\n\n\n Escribe tu orden: ");
}


void inventario(struct samurai* samurai)
{
	printf("\n\n\n Estos son tus objetos: \n\n\n");
	
	if (samurai->kimono != 0 && samurai->katana != 0)
    {
		  printf("Tienes %i hp de tus %i hp maximos\n", samurai->vida, samurai->max_vid);
	}
	if (samurai->katana == 1)
	{
		printf("Tienes la katana oxidada\n");
	}
	if (samurai->kimono == 1)
	{
		printf("Tienes el kimono ensangrentado de tu padre");
	}

	if (samurai->bandana != 0 && samurai->hablar_viejo != 0)
	{
		  printf("Tienes %i mana de tu %i maná maximo\n", samurai->mana, samurai->max_man);
    }

	if (samurai->sushis != 0)
	{
		printf("Tienes %i sushis\n", samurai->sushis);
	}

	if (samurai->sakes != 0)
	{
		  printf("Tienes %i sakes\n", samurai->sakes);
    }

	if (samurai->llaves != 0)
	{
		printf("Tienes %i llaves\n");
	}
	if (samurai->foto_viejo_loco != 0)
	{
		printf("Tienes la foto del viejo loco\n");
	}
	if (samurai->bola_pinchos != 0)
	{
		printf("Tienes la bola de pinchos\n");
	}
	if (samurai->pincel != 0)
	{
		printf("Tienes el pincel\n");
	}
	if (samurai->romper_madera_mano != 0)
	{
		printf("Puedes romper madera con la mano\n");
	}
	if (samurai->huevo_gallina != 0)
	{
		printf("Tienes el huevo de gallina\n");
	}
	if (samurai->perro != 0)
	{
		printf("Un perro te acompaña\n");
	}
	if (samurai->hacha != 0)
	{
		printf("Tienes un hacha\n");
	}
	if (samurai->cabezas_panda)
	{
		printf("Tienes %i cabezas de panda\n", samurai->cabezas_panda);
	}
	if (samurai->vela != 0)
	{
		printf("Tienes una vela", samurai->vela);
	}
	printf("\n\n\nEscribe una orden:");
}
