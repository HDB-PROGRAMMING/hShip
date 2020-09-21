#include "lib/lib.h"
 
int main() {
	bool game_over = false;
	int points = 0;
	ship Ship(57, 25, 3, 4);
	list<asteroid*> asteroids;
	list<asteroid*>::iterator itA;
	for (int i = 0; i < 5; i++) {
		asteroids.push_back(new asteroid(rand() % 115 + 2, rand() % 3 + 4));
	}
	list<bullet*> bullets;
	list<bullet*>::iterator it;
	// Aquí creamos las variables, objetos y listas
 
	hideCursor();
	system("title hShip, un juego de naves en caracteres ASCII");
	// Aquí cambiamos valores de la consola
 
	drawLimits();
	gotoxy(58, 14); printf("HSHIP");
	gotoxy(41, 15); printf("Pulsa cualquier tecla para iniciar el juego");
	system("pause> NUL");
	gotoxy(58, 14); printf("     ");
	gotoxy(41, 15); printf("                                           ");
 
	Ship.show();
	Ship.showHealth();
	drawLimits();
 
	while (!game_over) {
		gotoxy(7, 1); printf("Puntos: %d", points);
 
		if ((points % 2000 == 0) && points != 0) {
			if ((points % 4000 == 0) && points != 0) {
				asteroids.push_back(new asteroid(rand() % 115 + 2, rand() % 3 + 4));
			}
			Ship.incrementLives();
			points += 100;
		}
 
		if (kbhit()) {
			char key = getch();
			if (key == 'z' || key == ' ') {
				bullets.push_back(new bullet(Ship.getX() + 2, Ship.getY() - 1));
			}
			if (key == 'p') {
				gotoxy(58, 14); printf("HSHIP");
				gotoxy(38, 15); printf("Pulsa cualquier tecla para continuar con el juego");
				system("pause> NUL");
				gotoxy(58, 14); printf("     ");
				gotoxy(38, 15); printf("                                                 ");
			}
			if (key == 'l' && DEBUG_MODE) {
				points += 100;
			}
			if (key == 'e' && DEBUG_MODE) {
				Ship.decrementHealth();
			}
		}
 
		for (it = bullets.begin(); it != bullets.end(); it++) {
			(*it)->move();
			if ((*it)->out()) {
				gotoxy((*it)->getX(), (*it)->getY()); printf(" ");
				delete(*it);
				it = bullets.erase(it);
			}
		}
 
		Ship.move();
		Ship.showHealth();
		Ship.die();
		for (itA = asteroids.begin(); itA != asteroids.end(); itA++) {
			(*itA)->move();
			(*itA)->colision(Ship);
		}
 
		for (itA = asteroids.begin(); itA != asteroids.end(); itA++) {
			for (it = bullets.begin(); it != bullets.end(); it++) {
				if (((*itA)->getX() == (*it)->getX()) && ((*itA)->getY() == (*it)->getY() || (*itA)->getY() + 1 == (*it)->getY())) {
					gotoxy((*it)->getX(), (*it)->getY()); printf(" ");
					delete(*it);
					it = bullets.erase(it);
 
					asteroids.push_back(new asteroid(rand() % 115 + 2, rand() % 3 + 4));
					gotoxy((*itA)->getX(), (*itA)->getY()); printf(" ");
					delete(*itA);
					itA = asteroids.erase(itA);
 
					points += 100;
				}
			}
		}
		// Aquí el bucle para mantener activos a la nave y los asteroides
 
		if (Ship.getLives() <= 0) {
			game_over = true;
		}
 
		Sleep(30);
	}
 
	while (game_over) {
		gotoxy(55, 14); printf("GAME OVER");
		gotoxy(43, 15); printf("Pulsa ESPACIO para salir del juego");
 
		if (kbhit()) {
			char key = getch();
			if (key == ' ') {
				exit(0);
			}
		}
	}
 
	return 0;
}