// Klasse fuer einfache 2d Vektor-Operationen

#pragma once
#include <string>

using namespace std;

class Vector
{

public:
  double x, y;  // public, damit keine extra "getter" und "setter" Funktionen benoetigt werden

  // Konstruktoren
  Vector(); // Default Konstruktor
  Vector(double _x, double _y);

  void print(const string &name); // Ausgabe der Werte

  // Betrag des Vektors ausgeben
  double abs();

  // Drehen des Vektors um phi Grad mit p als Drehpunkt (Default ist (0,0))
  void rotate(double phi, const Vector &p = Vector(0, 0));

  // Ueberlade '+' Operator um zwei Vektoren zu addieren
  Vector operator+(const Vector &);

  // Ueberlade '-' Operator um zwei Vektoren zu subtrahieren
  Vector operator-(const Vector &);

  // Ueberlade '+=' Operator um einen Vektor zu addieren
  void operator+=(const Vector &);

  // Ueberlade '-=' Operator um einen Vektor zu subtrahieren
  void operator-=(const Vector &);
};
