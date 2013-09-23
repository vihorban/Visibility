#pragma warning(push)
#pragma warning(disable : 4482)
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "gl\freeglut.h"

using namespace std;

#define Esc 27
#define Tab 9
#define Del 127
#define Enter 13
#define Backspace 8
#define PI 3.14159265358979323846264338332795
#define INF 1000000000
#define POSBETA 32
#define POSALPHA -20
#define STEP 0.1
#define EPS 0.0000001

enum StatusEnum {NotVisible, Visible, OnLightOfSight};

class Point3D
{
public:
	double x;
	double y;
	double z;
	Point3D();
	Point3D(double _x, double _y, double _z);
	string ToString();
};

class Observer
{
public:
	Point3D position;
	Observer();
	Observer(Point3D _position);
	void draw();
};

class Line
{
public:
	int indexFirstPoint;
	int indexSecondPoint;
	Line();
	Line(int indexFirst, int indexSecond);
};

class Tope
{
public:
	vector<int> indexes;
	int status;
	Tope();
	Tope(int indexFirst, int indexSecond, int indexThird);
	Tope(int indexFirst, int indexSecond, int indexThird, int indexFourth);
};

class Vector
{
public:
	double x;
	double y;
	double z;
	Vector();
	Vector(double _x, double _y, double _z);
	double operator *(Vector mult);
	double makeNormalToSurface(Point3D a, Point3D b, Point3D c);
	void makeOposite();
};

class Polytope
{
public:
	vector<Point3D> points;
	vector<Line> lines;
	vector<Tope> topes;
	void draw();
	void drawLines();
	void drawTopes();
	void standart();
	void findVisible(Point3D observer);
	void generateSome(int numOfLayers);
	void ReadFromFile(string fileName);
};

void write_text_big(double R, double G, double B, double x, double y, const char str[255]);
void write_text_small(double R, double G, double B, double x, double y, const char str[255]);
void glWrite(double x, double y, int *font,const char text[256], int kls);