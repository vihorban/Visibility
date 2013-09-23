#include "methods.h"

void glWrite(double x, double y, int *font,const char text[256], int kls)
{
    int i;
    glRasterPos2d(x, y);
    for (i=0; i<kls; i++)
            glutBitmapCharacter(font, (int)text[i]);
}

void write_text_big(double R, double G, double B, double x, double y,const char str[255])
{
    glColor3d(R,G,B);
    glWrite(x, y, (int*)GLUT_BITMAP_TIMES_ROMAN_24, str, strlen(str));
}

void write_text_small(double R, double G, double B, double x, double y,const char str[255])
{
    glColor3d(R,G,B);
    glWrite(x, y, (int*)GLUT_BITMAP_TIMES_ROMAN_10, str, strlen(str));
}

Point3D::Point3D()
{
	x=0;
	y=0;
	z=0;
}

Vector::Vector()
{
	x=0;
	y=0;
	z=0;
}

Point3D::Point3D(double _x, double _y, double _z)
{
	x=_x;
	y=_y;
	z=_z;
}

Vector::Vector(double _x, double _y, double _z)
{
	x=_x;
	y=_y;
	z=_z;
}

Observer::Observer()
{
	position=Point3D();
}

Observer::Observer(Point3D _position)
{
	position=_position;
}

void Observer::draw()
{
	glPushMatrix();
	glTranslated(position.x,position.y,position.z);
    glColor3f(0, 1, 1);                        
	glutSolidSphere(0.05, 15, 15);
	glColor3f(0, 0, 0);  
	glutWireSphere(0.05, 15, 15);
    glPopMatrix();  
}

Line::Line()
{
	indexFirstPoint=0;
	indexSecondPoint=0;
}

Tope::Tope()
{
	status=0;
}

Line::Line(int indexFirst, int indexSecond)
{
	indexFirstPoint=indexFirst;
	indexSecondPoint=indexSecond;
}

Tope::Tope(int indexFirst, int indexSecond, int indexThird)
{
	indexes.push_back(indexFirst);
	indexes.push_back(indexSecond);
	indexes.push_back(indexThird);
	status=0;
}

Tope::Tope(int indexFirst, int indexSecond, int indexThird, int indexFourth)
{
	indexes.push_back(indexFirst);
	indexes.push_back(indexSecond);
	indexes.push_back(indexThird);
	indexes.push_back(indexFourth);
	status=0;
}

void Polytope::standart()
{
	points.push_back(Point3D(3,3,3));
	points.push_back(Point3D(3,3,1));
	points.push_back(Point3D(2,3,2));
	points.push_back(Point3D(3.1,2,3));
	points.push_back(Point3D(3,2,1.2));
	points.push_back(Point3D(2.1,2,2.2));
	points.push_back(Point3D(2.7,1,2.5));
	lines.push_back(Line(0,1));
	lines.push_back(Line(1,2));
	lines.push_back(Line(2,0));

	lines.push_back(Line(0,3));
	lines.push_back(Line(1,4));
	lines.push_back(Line(2,5));
	lines.push_back(Line(3,6));

	lines.push_back(Line(0,4));
	lines.push_back(Line(1,5));
	lines.push_back(Line(2,3));

	lines.push_back(Line(3,4));
	lines.push_back(Line(3,5));
	lines.push_back(Line(4,5));
	lines.push_back(Line(4,6));
	lines.push_back(Line(5,6));

	topes.push_back(Tope(0,1,2));

	topes.push_back(Tope(0,1,4));
	topes.push_back(Tope(0,3,4));

	topes.push_back(Tope(1,2,5));
	topes.push_back(Tope(1,4,5));

	topes.push_back(Tope(2,0,3));
	topes.push_back(Tope(2,5,3));

	topes.push_back(Tope(3,4,6));
	topes.push_back(Tope(4,5,6));
	topes.push_back(Tope(5,3,6));
}

void Polytope::drawLines()
{
	glPushMatrix();
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	for(unsigned int i=0; i< lines.size(); i++)
	{
		glVertex3d(points[lines[i].indexFirstPoint].x,points[lines[i].indexFirstPoint].y,points[lines[i].indexFirstPoint].z);
		glVertex3d(points[lines[i].indexSecondPoint].x,points[lines[i].indexSecondPoint].y,points[lines[i].indexSecondPoint].z);
	}
	glEnd();
	glPopMatrix(); 
}

void Polytope::drawTopes()
{
	glPushMatrix();
	for(unsigned int i=0; i< topes.size(); i++)
	{
		glBegin(GL_POLYGON);
		glColor3f(1, 0, 0);
		if(topes[i].status==StatusEnum::Visible)
			glColor3f(0, 1, 0);
		else if(topes[i].status==StatusEnum::OnLightOfSight)
			glColor3f(1, 1, 0);
		for(unsigned int k=0; k< topes[i].indexes.size(); k++)
			glVertex3d(points[topes[i].indexes[k]].x,points[topes[i].indexes[k]].y,points[topes[i].indexes[k]].z);
		glEnd();
	}
	glPopMatrix(); 
}

void Polytope::draw()
{
	drawTopes();
	drawLines();
}

void Polytope::findVisible(Point3D observer)
{
	for(unsigned int i=0; i< topes.size(); i++)
		if(topes[i].indexes.size()>=3)
		{
			double middleX=0;
			double middleY=0;
			double middleZ=0;
			int n=topes[i].indexes.size();
			for(int k=0; k<n; k++)
			{
				middleX+=points[topes[i].indexes[k]].x;
				middleY+=points[topes[i].indexes[k]].y;
				middleZ+=points[topes[i].indexes[k]].z;
			}
			middleX/=n;
			middleY/=n;
			middleZ/=n;

			Vector fromObserverToSurface(middleX-observer.x,middleY-observer.y,middleZ-observer.z);
			Vector normalToSurface;
			double d=normalToSurface.makeNormalToSurface(points[topes[i].indexes[0]],points[topes[i].indexes[1]],points[topes[i].indexes[2]]);

			middleX=0;
			middleY=0;
			middleZ=0;
			for(unsigned int k=0; k<points.size();k++)
			{
				middleX+=points[k].x;
				middleY+=points[k].y;
				middleZ+=points[k].z;
			}
			middleX/=points.size();
			middleY/=points.size();
			middleZ/=points.size();

			double sum=normalToSurface.x*middleX+normalToSurface.y*middleY+normalToSurface.z*middleZ+d;
			if(sum>EPS)
			{
				normalToSurface.makeOposite();
			}

			double multiplication=fromObserverToSurface*normalToSurface;
			if(multiplication>EPS)
				topes[i].status=StatusEnum::NotVisible;
			else if(multiplication<-EPS)
				topes[i].status=StatusEnum::Visible;
			else
				topes[i].status=StatusEnum::OnLightOfSight;
		}
}

double Vector::operator*(Vector mult)
{
	return x*mult.x+y*mult.y+z*mult.z;
}

double Vector::makeNormalToSurface(Point3D a, Point3D b, Point3D c)
{
	double A=a.y*(b.z-c.z)+b.y*(c.z-a.z)+c.y*(a.z-b.z);
	double B=a.z*(b.x-c.x)+b.z*(c.x-a.x)+c.z*(a.x-b.x);
	double C=a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y);
	double D=-(A*a.x+B*a.y+C*a.z);
	x=A;
	y=B;
	z=C;
	return D;
}

void Vector::makeOposite()
{
	x=-x;
	y=-y;
	z=-z;
}

void Polytope::generateSome(int numOfLayers)
{
	double alpha=double(rand()%10943)/10000;
	double beta=double(rand()%10943+20943)/10000;
	double gama=double(rand()%10943+41886)/10000;
	points.push_back(Point3D(cos(alpha),3,sin(alpha)));
	points.push_back(Point3D(cos(beta),3,sin(beta)));
	points.push_back(Point3D(cos(gama),3,sin(gama)));
	lines.push_back(Line(0,1));
	lines.push_back(Line(0,2));
	lines.push_back(Line(1,2));
	topes.push_back(Tope(0,1,2));
	double distBetweenLayers=6.0/(numOfLayers+1);
	int i1=3,i2=4,i3=5;
	for(int i=0; i<numOfLayers+1; i++,i1+=3,i2+=3,i3+=3)
	{
		double level=3-distBetweenLayers*(i+1);
		double raduis=4-abs(level);
		//int Dist10000=int(distBetweenLayers*10000);
		//double eps1=double(rand()%Dist10000)/10000-distBetweenLayers/2;
		//double eps2=double(rand()%Dist10000)/10000-distBetweenLayers/2;
		//double eps3=double(rand()%Dist10000)/10000-distBetweenLayers/2;
		double alpha=double(rand()%10943)/10000;
		double beta=double(rand()%10943+20943)/10000;
		double gama=double(rand()%10943+41886)/10000;
		points.push_back(Point3D(raduis*cos(alpha),level,raduis*sin(alpha)));
		points.push_back(Point3D(raduis*cos(beta),level,raduis*sin(beta)));
		points.push_back(Point3D(raduis*cos(gama),level,raduis*sin(gama)));
		lines.push_back(Line(i1,i2));
		lines.push_back(Line(i1,i3));
		lines.push_back(Line(i2,i3));
		lines.push_back(Line(i1-3,i1));
		lines.push_back(Line(i1-3,i2));
		lines.push_back(Line(i2-3,i2));
		lines.push_back(Line(i2-3,i3));
		lines.push_back(Line(i3-3,i3));
		lines.push_back(Line(i3-3,i1));
		topes.push_back(Tope(i1-3,i1,i2));
		topes.push_back(Tope(i2-3,i2,i3));
		topes.push_back(Tope(i3-3,i3,i1));
		topes.push_back(Tope(i1-3,i2-3,i2));
		topes.push_back(Tope(i2-3,i3-3,i3));
		topes.push_back(Tope(i3-3,i1-3,i1));
	}
	topes.push_back(Tope(i1-3,i2-3,i3-3));
}

string Point3D::ToString()
{
	ostringstream posx;
	posx << x;
	string posX = posx.str();
	ostringstream posy;
	posy << y;
	string posY = posy.str();
	ostringstream posz;
	posz << z;
	string posZ = posz.str();
	return "x: "+posX+"       y: "+posY+"       z: "+posZ;
}

void Polytope::ReadFromFile(string fileName)
{
	ifstream inputStream;
	inputStream.open(fileName);
	int numberPoints;
	inputStream>>numberPoints;
	char temp='0';
	while(temp!=':')
	{
		inputStream>>temp;
	}
	for(int i=0; i<numberPoints; i++)
	{
		double posX, posY, posZ;
		inputStream>>posX;
		inputStream>>posY;
		inputStream>>posZ;
		points.push_back(Point3D(posX, posY, posZ));
	}
	int numberOfLines;
	inputStream>>numberOfLines;
	temp='0';
	while(temp!=':')
	{
		inputStream>>temp;
	}
	for(int i=0; i<numberOfLines; i++)
	{
		int first, second;
		inputStream>>first;
		inputStream>>second;
		lines.push_back(Line(first, second));
	}
	int numberOfTopes;
	inputStream>>numberOfTopes;
	temp='0';
	while(temp!=':')
	{
		inputStream>>temp;
	}
	for(int i=0; i<numberOfTopes; i++)
	{
		int first, second, third;
		inputStream>>first;
		inputStream>>second;
		inputStream>>third;
		topes.push_back(Tope(first, second, third));
	}
	inputStream.close();
}
