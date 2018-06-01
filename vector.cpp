// Klasse fuer einfache 2d Vektor-Operationen

#include "vector.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <string>


using namespace std;

// Default Konstruktor
Vector::Vector()
{
  x = 0;
  y = 0;
};

// Konstruktor
Vector::Vector(double _x, double _y)
{
  x = _x;
  y = _y;
}

void Vector::print(const string &name)
{
  cout << name << "(" << x << "," << y << ")";
}

// Betrag (Laenge) des Vektors
double Vector::abs()
{
  return sqrt(x*x+y*y);
}

// Drehen des Vektors um phi Grad mit p als Drehpunkt (Default ist (0,0))
void Vector::rotate(double phi, const Vector &p)
{
  // http://www.matheboard.de/archive/460078/thread.html
  // x' = px + (x-px)*cos(phi) - (y-py)*sin(phi)
  // y' = py + (x-px)*sin(phi) + (y-py)*cos(phi)
  double phi_rad =  phi * M_PI/180.0;
  double x_new = p.x + (x - p.x) * cos(phi_rad) - (y - p.y) * sin(phi_rad);
  double y_new = p.y + (x - p.x) * sin(phi_rad) + (y - p.y) * cos(phi_rad);
  x = x_new;
  y = y_new;
}

// Ueberlade '+' Operator um zwei Vektoren zu addieren
Vector Vector::operator+(const Vector &v)
{
  Vector vneu(x + v.x, y + v.y);
  return vneu;
}

// Ueberlade '-' Operator um zwei Vektoren zu subtrahieren
Vector Vector::operator-(const Vector &v)
{
  Vector vneu(x - v.x, y - v.y);
  return vneu;
}

// Ueberlade '+=' Operator um einen Vektor zu addieren
void Vector::operator+=(const Vector &v)
{
  x += v.x;
  y += v.y;
}

// Ueberlade '-=' Operator um einen Vektor zu subtrahieren
void Vector::operator-=(const Vector &v)
{
  x -= v.x;
  y -= v.y;
}