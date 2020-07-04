#include "lib/stdio.h"
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <list>
// Aquí incluimos las librerías necesarias
 
using namespace std;
// Aquí usamos los namespaces necesarios
 
#define UP_KEY 72
#define DOWN_KEY 80
#define LEFT_KEY 75
#define RIGHT_KEY 77
#define DEBUG_MODE false
// Aquí definimos las constantes necesarias
 
void gotoxy(int x, int y) {
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;
	SetConsoleCursorPosition(hCon, dwPos);
}
 
void hideCursor() {
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 50;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hCon, &cci);
}
 
class ship {
	int x, y, health, lives;
 
public:
	ship(int _x, int _y, int _health, int _lives) : x(_x), y(_y), health(_health), lives(_lives) {};
	int getX() { return x; };
	int getY() { return y; };
	int getLives() { return lives; };
	void decrementHealth() { health--; };
	void incrementLives() { lives++; };
	void show();
	void clean();
	void move();
	void showHealth();
	void die();
};
 
void ship::show() {
	gotoxy(x, y);     printf("  %c", 65);
	gotoxy(x, y + 1); printf(" %c%c%c", 40, 207, 41);
	gotoxy(x, y + 2); printf("%c%c %c%c", 174, 190, 190, 175);
}
 
void ship::clean() {
	gotoxy(x, y);     printf("      ");
	gotoxy(x, y + 1); printf("      ");
	gotoxy(x, y + 2); printf("      ");
}
 
void ship::move() {
	if (kbhit()) {
		char key = getch();
		clean();
		if ((key == LEFT_KEY) && x > 3) x--;
		if ((key == RIGHT_KEY) && x < 111) x++;
		if ((key == UP_KEY) && y > 4) y--;
		if ((key == DOWN_KEY) && y < 25) y++;
		show();
		showHealth();
	}
}
 
void drawLimits() {
	gotoxy(2, 3);    printf("%c", 201);
	gotoxy(117, 3);  printf("%c", 187);
	gotoxy(2, 28);   printf("%c", 200);
	gotoxy(117, 28); printf("%c", 188);
	for (int i = 3; i < 117; i++) {
		gotoxy(i, 3);  printf("%c", 205);
		gotoxy(i, 28); printf("%c", 205);
	}
 
	for (int i = 4; i < 28; i++) {
		gotoxy(2, i);   printf("%c", 186);
		gotoxy(117, i); printf("%c", 186);
	}
}
 
void ship::showHealth() {
	gotoxy(70, 1); printf("Vidas: %d", lives);
	gotoxy(90, 1); printf("Salud: ");
	gotoxy(97, 1); printf("      ");
	for (int i = 0; i < health; i++) {
		gotoxy(97 + i, 1); printf("X");
	}
}
 
void ship::die() {
	if (health <= 0) {
		clean();
		gotoxy(x, y);     printf("  **  ");
		gotoxy(x, y + 1); printf(" **** ");
		gotoxy(x, y + 2); printf("  **  ");
		Sleep(200);
		// Frame 1 de la animación de muerte
 
		clean();
		gotoxy(x, y);     printf("* ** *");
		gotoxy(x, y + 1); printf(" **** ");
		gotoxy(x, y + 2); printf("* ** *");
		Sleep(300);
		// Frame 2 de la animación de muerte
 
		clean();
		gotoxy(x, y);     printf("  **  ");
		gotoxy(x, y + 1); printf(" **** ");
		gotoxy(x, y + 2); printf("  **  ");
		Sleep(200);
		clean();
		// Frame 3 de la animación de muerte
 
		lives--; health = 3;
		showHealth();
		show();
	}
}
 
class asteroid {
	int x, y;
 
public:
	asteroid(int _x, int _y) : x(_x), y(_y) {};
	int getX() { return x; };
	int getY() { return y; };
	void show();
	void move();
	void clean();
	void colision(class ship& Ship);
};
 
void asteroid::show() {
	gotoxy(x, y); printf("%c", 184);
}
 
void asteroid::clean() {
	gotoxy(x, y); printf(" ");
}
 
void asteroid::move() {
	clean(); y++;
 
	if (y > 27) {
		x = rand() % 113 + 3;
		y = 4;
	}
 
	Sleep(5);
	show();
}
 
void asteroid::colision(ship& Ship) {
	if ((x >= Ship.getX() && x < Ship.getX() + 5) && (y >= Ship.getY() && y < Ship.getY() + 3)) {
		Ship.decrementHealth();
		Ship.clean();
		Ship.show();
		Ship.showHealth();
		x = rand() % 113 + 3;
		y = 4;
	}
}
 
class bullet {
	int x, y;
 
public:
	bullet(int _x, int _y) : x(_x), y(_y) {};
	int getX() { return x; };
	int getY() { return y; };
	void move();
	bool out();
};
 
void bullet::move() {
	gotoxy(x, y); printf(" ");
	y--;
	gotoxy(x, y); printf("%c", 186);
}
 
bool bullet::out() {
	if (y == 4) return true;
	return false;
}
 
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