// objekte.h
#pragma once
#include "vector.h"
#include <string>

using namespace std;

// Basis-Klasse
class Objekt
{
protected:
  string name; // Objektname
  Vector pos;  // Position
  double size; // Groesse

public:
  Objekt(const string &_name, const Vector &_pos, double _size); // Konstruktor
  virtual void print();   // Ausgabe der Werte (virtual damit die Funktion von einer Unterklasse ueberlagert werden kann)
  Vector getPosition() const;   // Gib die Position aus
  double getSize() const; // Gib die Groesse aus
  bool collidesWith(const Objekt &other);  // prüfe ob eine Kollision mit "other" vorliegt
  virtual void draw() {};  // Dummy für die generische Handhabung im vector-array...
  virtual void move() {};  // Dummy für die generische Handhabung im vector-array...
};

// Unterklasse Planet
class Planet : public Objekt
{
public:
  Planet(const string &_name, const Vector &_pos, double _size) : Objekt(_name, _pos, _size) {}; // Konstruktor
  virtual void print();                                          // Ausgabe der Werte
  virtual void draw();
  virtual void move() {}; // dummy
};

// Unterklasse bewegtes Objekt
class MobilesObjekt : public Objekt
{
protected:
  Vector vmove; // Bewegungsvektor (kein array-vector!)

public:
  MobilesObjekt(const string &name, const Vector &pos, double size, double dir, double speed); // Konstruktor
  virtual void print();   // Ausgabe der Werte
  virtual void move();    // Bewege Objekt
  //virtual void draw() {}; // Dummy wegen vector...
};

// Unterklasse Asteroid
class Asteroid : public MobilesObjekt
{
public:
  Asteroid(const string &_name, const Vector &_pos, double _size, double dir, double speed) : MobilesObjekt(_name, _pos, _size, dir, speed) {}; // Konstruktor
  virtual void print();  // Ausgabe der Werte 
  virtual void draw();
};

// Unterklasse Raumschiff
class Raumschiff : public MobilesObjekt
{
protected:
  int fuel;   // Treibstoff
  int points; // Punkte
  double orientation; // Ausrichtung (wohin zeigt der Bug)
  double spin; // Drehgeschwindigkeit
  int thrusting; // Triebwerk läuft

public:
  Raumschiff(const string &_name, const Vector &_pos, double _orientation, int _fuel, int _points); // Konstruktor
  void print();  // Ausgabe der Werte
  void draw();
  void turnLeft();
  void turnRight();
  void thrust();
  void move();
};