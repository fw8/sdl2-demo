#include "objekte.h"
#include "vector.h"
#include "graphics.h"
#include <iostream>
#include <string>
#define _USE_MATH_DEFINES
#include <cmath>

using namespace std;


/********************************************************************
* Objekt
********************************************************************/

Objekt::Objekt(const string &_name, const Vector &_pos, double _size)
{
  name = _name;
  pos = _pos;
  size = _size;
}

void Objekt::print()
{
  cout << name << ",";
  pos.print(" Position: ");
  cout << ", Groesse: " << size;
}

Vector Objekt::getPosition() const   // const => funktion verändert das objekt nicht!
{
  return pos;
}

double Objekt::getSize() const  // const => funktion verändert das objekt nicht!
{
  return size;
}

// Kollisionspruefung...
// Jedes Objekt wird als Kreis mit Durchmesser "size" an der Stelle "pos" gesehen.
// Ueberschneiden sich zwei Kreise, haben wir eine Kollision
bool Objekt::collidesWith(const Objekt &other)
{
  double distance = (pos - other.getPosition()).abs(); // entfernung = |eigene_position - postition_des_anderen| 
  return (distance < size/2 + other.getSize()/2); // ist die Entfernung der Positionen von einander < eigener_radius + radius_des_anderen ?
}


/********************************************************************
* Planet (Unterklasse von Objekt)
********************************************************************/

void Planet::print()
{
  cout << "Planet: ";
  Objekt::print();
  cout << endl;
}

void Planet::draw()
{
  g_fill_circle(pos.x, pos.y, size/2, g_rgb(0x0, 0x0, 0xff));
}


/********************************************************************
* MobilesObjekt (Unterklasse von Objekt)
********************************************************************/

MobilesObjekt::MobilesObjekt(const string &_name, const Vector &_pos, double _size, double dir, double speed) : Objekt(_name, _pos, _size)
{
  vmove = Vector(0, speed);
  vmove.rotate(dir, Vector(0, 0));
}

void MobilesObjekt::print()
{
  Objekt::print();
  vmove.print(", Richtung: ");
}

void MobilesObjekt::move()
{
  pos += vmove;
  if (pos.x > 800)
  {
    pos.x = 0;
  }
  else if (pos.x < 0)
  {
    pos.x = 800;
  }
  if (pos.y > 800)
  {
    pos.y = 0;
  }
  else if (pos.y < 0)
  {
    pos.y = 800;
  }
}


/********************************************************************
* Asteroid (Unterklasse von MobilesObjekt)
********************************************************************/

void Asteroid::print()
{
  cout << "Asteroid: ";
  MobilesObjekt::print();
  cout << endl;
}

void Asteroid::draw()
{
  g_fill_rounded_rectangle(pos.x - size / 2, pos.y - size / 2, pos.x + size / 2, pos.y + size / 2, 10, g_rgba(0x0, 0xff, 0x0, 0x50));
}


/********************************************************************
* Raumschiff (Unterklasse von MobilesObjekt)
********************************************************************/

Raumschiff::Raumschiff(const string &_name, const Vector &_pos, double _orientation, int _fuel, int _points) : MobilesObjekt(_name, _pos, 30, 0, 0)
{
  fuel = _fuel;
  points = _points;
  orientation = _orientation;
  spin = 0;
  thrusting = 0;
}

void Raumschiff::print()
{
  cout << "Raumschiff: ";
  MobilesObjekt::print();
  cout << ", Treibstoff: " << fuel;
  cout << ", Punkte: " << points;
  cout << endl;
}

void Raumschiff::draw()
{
  Vector p1,p2,p3;

  // Verbleibenden Treibstoff anzeigen
  char text[100];
  snprintf(text, sizeof(text), "Treibstoff: %d", fuel);
  g_render_text(10, 10, text, g_rgb(0xff, 0xff, 0xff));

  // Darstellung der Triebwerksflamme
  if (thrusting) {
    p1 = pos;
    p1 -= Vector(0, size / 2.0); // Darstellung etwas hinter dem Raumschiff
    p1.rotate(orientation, pos); // in die richtige Postition drehen...
    g_fill_circle(p1.x, p1.y, 3*thrusting, g_rgb(0xff, 0xff, 0x00)); // und Feuer
    ++thrusting %= 5; // Flamme steigert sich von 0 - 4 und springt dann wieder auf 0
  }

  // Darstellung des Raumschiffs
  // p1 soll Spitze des Raumschiffs sein (also vorne)
  // p1 liegt genau size/2 über der Mitte (pos)
  // also pos + v(0,size/2)
  p1 = pos;
  p1 += Vector(0, size / 2.0);

  // Jetzt wird die Spitze (p1) in die richtige Richtung gedreht
  // Drehpunkt ist natürlich die Mitte (pos)
  p1.rotate(orientation, pos);

  // Die anderen 2 Ecken des Raumschiffs liegen immer um 120 Grad weiter gedreht
  p2 = p1;
  p2.rotate(120, pos);

  p3 = p2;
  p3.rotate(120, pos);

  g_fill_triangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, g_rgb(0xff, 0x0, 0x0));
  g_fill_circle(p1.x, p1.y, 5, g_rgb(0xff, 0xff, 0xff)); // Bug als weisse Kugel

}

void Raumschiff::turnLeft()
{
  spin--; // Manoeverduesen feuern
}

void Raumschiff::turnRight()
{
  spin++; // Manoeverduesen feuern
}

void Raumschiff::thrust()
{
  if (fuel < 1)
    return; // kein Sprit mehr...

  // Beschleunigungsvektor mit "0.1" initialisieren
  Vector accelerate(0, 0.1);
  // Und in die Richtung des Raumschiffs drehen
  accelerate.rotate(orientation);

  // Zum aktuellen Bewegungsvektor addieren
  vmove += accelerate;

  // Sprit verbrauchen
  fuel--;

  // Triebwerk zuenden
  if (!thrusting) thrusting=1;
}

// Ueberlade die move() Methode, weil hier der Spin noch berücksichtigt werden muss
void Raumschiff::move()
{
  orientation += spin;   // Aktueller Drehimpuls wirkt sich auf die Ausrichtung aus..
  MobilesObjekt::move(); // Methode aus Superklasse erledigt den Rest
}