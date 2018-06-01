#define _USE_MATH_DEFINES
#include <cmath>

#include "graphics.h"

#include "vector.h"
#include "objekte.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Funktion zum Erzeugen des optischen Knalls und zum Ausgeben der erreichten Zeit
void booom(unsigned int time)
{
	char text[100];
	int key;

	// optischer Knall
	for (int i = 1; i < 5; i++)
	{
		cout << "BOOOOOM" << endl;
		g_clear(255, 255, 255, 255);
		g_show();
		g_delay(40);
		g_clear(0, 0, 0, 255);
		g_show();
		g_delay(40);
	}

	// Erreichte Zeit anzeigen
	g_clear(0, 0, 0, 255);
	snprintf(text, sizeof(text), "Deine Zeit: %d", time);
	g_render_text(250, 300, text, g_rgb(0xff, 0xff, 0xff));
	g_show();

	// Warten auf Tastendruck 'q'
	while (!g_has_quit())
	{
		if (g_key(&key)) {
			if (key == 'q') return;
		}
	}
}

/*
* Standardkonformes Hauptprogramm
* argv ist ein Array der Laenge argc von nullterminierten Strings
* Beim Aufruf des Programmes koennen dem Programm Argumente uebergeben werden
* Diese Argumente koennen im Programm durch Auswertung von argv verwendet werden  */
int main(int argc, char **argv)
{
	int width = 800;
	int height = 800;
	char text[100];
	unsigned int time = 0;
	int key;

	// Weltraum erzeugen und mit Planeten/Asteroiden fuellen
	vector<Objekt*> weltraum;

	weltraum.push_back(new Asteroid("A-1", Vector(100, 100), 30, 30, 3));
	weltraum.push_back(new Asteroid("A-2", Vector(300, 700), 60, 280, 2));
	weltraum.push_back(new Asteroid("A-3", Vector(-50, 300), 30, 180, 3));
	weltraum.push_back(new Asteroid("A-4", Vector(0, -200), 60, 300, 5));
	weltraum.push_back(new Planet("Erde", Vector(400, 400), 200));
	
	// Raumschiff erzeugen
	Raumschiff eagle("Eagle", Vector(600, 600), 0, 1000, 100);

	// Alle erzeugten Objekte auf der Konsole ausgeben

	for (int i = 0; i < weltraum.size(); i++)
	{
		weltraum[i]->print();
	}

	eagle.print();

	/* Initialisierung der Graphikumgebung */
	g_create(width, height, "Spiel");

	/* Laden einer Schriftart des "ttf-Formats" aus der Datei
	 * "VeraMono.ttf" im Verzeichnis "ttf".
	 * Die Schriftgroesse ist 32
	 */
	g_load_font("ttf/VeraMono.ttf", 32);

	/* Hauptschleife */
	while (!g_has_quit())
	{
		/* Bild loeschen, immer am Anfang der Schleife */
		g_clear(0, 0, 0, 255);


		// Tastatur einlesen und entsprechen darauf reagieren

		if (g_key(&key))
		{
			switch (key)
			{
			case 'a':
				eagle.turnLeft();
				break;
			case 'd':
				eagle.turnRight();
				break;
			case 'm':
				eagle.thrust();
				break;
			case 'q':
				g_destroy();
				return 0;
			}
		}

		// Objekte bewegen und darstellen

		eagle.move();
		eagle.draw();
		//eagle.print();

		for (int i = 0; i < weltraum.size(); i++)
		{
			weltraum[i]->move();
			weltraum[i]->draw();
		}

		// Kollisionspruefungen

		for (int i = 0; i < weltraum.size(); i++)
		{
			if (weltraum[i]->collidesWith(eagle))
			{
				booom(time);
				g_destroy();
				return 0;
			}
		}

		// Treibstoff und Zeit anzeigen

  	snprintf(text, sizeof(text), "Zeit: %d", time++);
  	g_render_text(600, 10, text, g_rgb(0xff, 0xff, 0xff));

		/* Bild anzeigen, immer am Ende der Schleife */
		g_show();
		/* Optional kurze Zeit warten (CPU-Last, Flimmern) */
		g_delay(40);
	}

	/* Freigeben der Graphikumgebung */
	g_destroy();
	return 0;
}
