/****************************************************
 *                                                  *
 *                                                  *
 *           Postscript generation code             *
 *                                                  *
 *                                                  *
 ****************************************************/

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cmath>
#include <cstring>

#include "ps.h"

#define pi 3.14159265

psDoc::psDoc()
{

    fn = new char[12];
    strcpy(fn, "default.eps");
    tx.open(fn, ios::out);
    tx << "%!PS-Adobe-2.0 EPSF-2.0" << endl;
    tx << "%This file produce by C++ routines" << endl;
    tx << "%produced by Dan Ashlock, 5/12/98" << endl;
    tx << "%danwell@iastate.edu" << endl;
    a = 0;
    b = 0;
    c = 99;
    d = 99;
    tx << "%%BoundingBox: " << a << ' ' << b
       << ' ' << c << ' ' << d << endl;
    hasfont = 0;
}

psDoc::psDoc(const char* name)
{

    fn = new char[strlen(name)+1];
    strcpy(fn, name);
    tx.open(fn, ios::out);
    tx << "%!PS-Adobe-2.0 EPSF-2.0" << endl;
    tx << "%This file produce by C++ routines" << endl;
    tx << "%produced by Dan Ashlock, 5/12/98" << endl;
    tx << "%danwell@iastate.edu" << endl;
    a = 0;
    b = 0;
    c = 99;
    d = 99;
    tx << "%%BoundingBox: " << a << ' ' << b
       << ' ' << c << ' ' << d << endl;
    hasfont = 0;

}

psDoc::psDoc(const char* name, int aa, int bb, int cc, int dd)
{

    fn = new char[strlen(name)+1];
    strcpy(fn, name);
    tx.open(fn, ios::out);
    tx << "%!PS-Adobe-2.0 EPSF-2.0" << endl;
    tx << "%This file produce by C++ routines" << endl;
    tx << "%produced by Dan Ashlock, 5/12/98" << endl;
    tx << "%danwell@iastate.edu" << endl;
    a = aa;
    b = bb;
    c = cc;
    d = dd;
    tx << "%%BoundingBox: " << a << ' ' << b
       << ' ' << c << ' ' << d << endl;
    hasfont = 0;
}

psDoc::~psDoc()
{

    tx << "showpage" << endl;
    tx << "%eof";
    tx.close();

    delete[] fn;

}

void psDoc::startpath()
{

    tx << "newpath" << endl;

}

void psDoc::closepath()
{

    tx << "closepath" << endl;

}

void psDoc::moveto(int x, int y)
{

    tx << x << ' ' << y << " moveto" << endl;

}

void psDoc::moveto(float x, float y)
{

    tx << x << ' ' << y << " moveto" << endl;

}

void psDoc::moveto(double x, double y)
{

    tx << x << ' ' << y << " moveto" << endl;

}

void psDoc::lineto(int x, int y)
{

    tx << x << ' ' << y << " lineto" << endl;

}

void psDoc::lineto(float x, float y)
{

    tx << x << ' ' << y << " lineto" << endl;

}

void psDoc::lineto(double x, double y)
{

    tx << x << ' ' << y << " lineto" << endl;

}

void psDoc::arrow(int u, int v,  //arrow tail
        int x, int y,  //arrow head
        int hz,       //arrow head line length
        double aa     //arrow head angle ni degrees
)
{

    double th;    //arrow body angle
    double a1, a2; //arrow head angles
    int a, b, c, d;

    th = M_PI/2-atan2(x-u, y-v);
    a1 = th+M_PI-aa/180*M_PI;
    a2 = th-M_PI+aa/180*M_PI;
    //cout << th << " " << a1 << " " << a2 << endl;

    moveto(u, v);
    lineto(x, y);
    stroke();

    a = x+((int) (0.5+((double) hz)*cos(a1)));
    b = y+((int) (0.5+((double) hz)*sin(a1)));
    c = x+((int) (0.5+((double) hz)*cos(a2)));
    d = y+((int) (0.5+((double) hz)*sin(a2)));
    moveto(a, b);
    lineto(x, y);
    lineto(c, d);
    stroke();

}

//the following routines return an integer position in the picture from
//a value in [0,1] that is the fraction of the way across the picture 
//ASSUMING a margin of margin pixels on all sides.
int psDoc::vpos(double y, int margin)
{

    int dt;
    double dd;

    dt = d-b-2*margin;
    dd = (double) dt;
    dd *= y;
    dt = (int) dd;
    dt += margin;
    return (dt);
}

int psDoc::hpos(double x, int margin)
{

    int dt;
    double dd;

    dt = c-a-2*margin;
    dd = (double) dt;
    dd *= x;
    dt = (int) dd;
    dt += margin;
    return (dt);

}

//make a glyph at position x,y
//Glyph types:
//0 cross
void psDoc::glyph(int x, int y, int tp, int z)
{

    if (tp<0)tp = -tp;

    switch (tp) {
    case 0: //vertical cross
        moveto(x-z, y);
        lineto(x+z, y);
        stroke();
        moveto(x, y-z);
        lineto(x, y+z);
        stroke();
        break;
    case 1: //open box
        moveto(x-z, y-z);
        lineto(x+z, y-z);
        lineto(x+z, y+z);
        lineto(x-z, y+z);
        lineto(x-z, y-z);
        stroke();
        break;
    case 2: //open triangle
        moveto(x, y+z);
        lineto(x+3*z/3, y-2*z/3);
        lineto(x-3*z/3, y-2*z/3);
        lineto(x, y+z);
        stroke();
        break;
    case 3: //diagonal cross (X)
        moveto(x-z, y-z);
        lineto(x+z, y+z);
        stroke();
        moveto(x+z, y-z);
        lineto(x-z, y+z);
        stroke();
        break;
    case 4: //open circle
        MakeStar(x, y, z, 12, 1);
        break;
    case 5:  //five pointed star
        MakeStar(x, y, z, 5, 2);
        break;
    case 6: //solid box
        startpath();
        moveto(x-z, y-z);
        lineto(x+z, y-z);
        lineto(x+z, y+z);
        lineto(x-z, y+z);
        lineto(x-z, y-z);
        closepath();
        fill();
        break;
    case 7: //solid triangle
        z += 1;
        startpath();
        moveto(x, y+z);
        lineto(x+3*z/3, y-2*z/3);
        lineto(x-3*z/3, y-2*z/3);
        closepath();
        fill();
        break;
    case 8: //solid circle
        FillStar(x, y, z+1, 12, 1);
        break;
    }

}

void psDoc::gsave()
{

    tx << "gsave" << endl;

}

void psDoc::grestore()
{

    tx << "grestore" << endl;

}

void psDoc::stroke()
{

    tx << "stroke" << endl;
}

void psDoc::setgray(int b)
{

    if (b>100)b = 100;
    if (b<1)b = 1;

    if (b==100)tx << "0 setgray" << endl;
    else if (b<10)tx << ".0" << b << " setgray" << endl;
    else
        tx << '.' << b << " setgray" << endl;

}

//set color
void psDoc::setcol(int r, int g, int b)
{

    double rr, gg, bb;

    if (r>255)r = 255;
    if (r<0)r = 0;
    if (g>255)g = 255;
    if (g<0)g = 0;
    if (b>255)b = 255;
    if (b<0)b = 0;

    rr = ((double) r)/255.0;
    gg = ((double) g)/255.0;
    bb = ((double) b)/255.0;

    tx << rr << " " << gg << " " << bb << " setrgbcolor" << endl;

}

void psDoc::fill()
{

    tx << "fill" << endl;

}

void psDoc::setlinewidth(int w)
{

    tx << w << " setlinewidth" << endl;

}

//make lines dashed
void psDoc::dashed(int w)
{

    tx << "[" << w << " " << w << "] " << 0 << " setdash" << endl;

}

//make lines dashed
void psDoc::undashed()
{

    tx << "[] " << 0 << " setdash" << endl;

}

void psDoc::box(int aa, int bb, int cc, int dd, int rim, int core)
{

    setgray(core);
    startpath();
    moveto(aa, bb);
    lineto(aa, dd);
    lineto(cc, dd);
    lineto(cc, bb);
    closepath();
    gsave();
    fill();
    setgray(rim);
    grestore();
    stroke();

}

void psDoc::boxI(int aa, int bb, int cc, int dd)
{

    startpath();
    moveto(aa, bb);
    lineto(aa, dd);
    lineto(cc, dd);
    lineto(cc, bb);
    closepath();
    fill();

}

void psDoc::fillpolygon(int* XY, int n)
{

    int i;

    startpath();
    moveto(XY[0], XY[1]);
    for (i = 1; i<n; i++)lineto(XY[2*i], XY[2*i+1]);
    closepath();
    fill();

}

void psDoc::polygon(int* XY, int n)
{

    int i;

    startpath();
    moveto(XY[0], XY[1]);
    for (i = 1; i<n; i++)lineto(XY[2*i], XY[2*i+1]);
    closepath();
    stroke();

}

//create a bar
void psDoc::bar(int h, int k,    //center of bar
        int r,          //radius
        int d,          //radius of diamond
        int horizontal, //predicate, vertical if false
        int wings       //wing size
)
{

    if (horizontal) {
        startpath();
        moveto(h-r, k);
        lineto(h+r, k);
        stroke();
        if (wings) {
            startpath();
            moveto(h-r, k-wings);
            lineto(h-r, k+wings);
            moveto(h+r, k-wings);
            lineto(h+r, k+wings);
            stroke();
        }
    }
    else {
        startpath();
        moveto(h, k-r);
        lineto(h, k+r);
        stroke();
        if (wings) {
            startpath();
            moveto(h-wings, k-r);
            lineto(h+wings, k-r);
            moveto(h-wings, k+r);
            lineto(h+wings, k+r);
            stroke();
        }
    }
    if (d) {
        startpath();
        moveto(h-d, k);
        lineto(h, k-d);
        lineto(h+d, k);
        lineto(h, k+d);
        closepath();
        stroke();
    }
}

void psDoc::initfont()
{          //loads times roman font size 12

    initfont(12);

}

void psDoc::initfont(int size)
{  //loads TR font size "size"

    tx << "/Times-Roman findfont" << endl;
    tx << size << " scalefont" << endl;
    tx << "setfont" << endl;
    fontscale = size;
    hasfont = 1;

}

void psDoc::initSfont(int size)
{  //loads TR font size "size"

    tx << "/Symbol findfont" << endl;
    tx << size << " scalefont" << endl;
    tx << "setfont" << endl;
    fontscale = size;
    hasfont = 1;

}

void psDoc::sayAT(int x, int y, const char* s)
{

    if (!hasfont)initfont();
    moveto(x, y);
    tx << "(" << s << ") " << "show" << endl;

}

void psDoc::sayATvert(int x, int y, const char* s)
{

    if (!hasfont)initfont();
    gsave();
    moveto(x, y);
    tx << "90 rotate" << endl;
    tx << "(" << s << ") " << "show" << endl;
    grestore();

}

void psDoc::LeftLabel(int y, const char* s)
{

    if (!hasfont)initfont();
    gsave();
    moveto(fontscale+2, y);
    tx << "90 rotate" << endl;
    tx << "(" << s << ") show" << endl;
    grestore();

}

void psDoc::LeftLabel(const char* s)
{//make a label at the left, centered

    int y;

    if (!hasfont)initfont();
    y = (d-b)/2-(strlen(s)*fontscale/4)+b;
    gsave();
    moveto(fontscale+2, y);
    tx << "90 rotate" << endl;
    tx << "(" << s << ") show" << endl;
    grestore();

}

void psDoc::RightLabel(int y, const char* s)
{

    if (!hasfont)initfont();
    gsave();
    moveto(c-fontscale-2, y);
    tx << "270 rotate" << endl;
    tx << "(" << s << ") show" << endl;
    grestore();

}

void psDoc::RightLabel(const char* s)
{//make a label at the left, centered

    int y;

    if (!hasfont)initfont();
    y = (d-b)/2+strlen(s)*fontscale/4+b;
    gsave();
    moveto(c-fontscale-2, y);
    tx << "270 rotate" << endl;
    tx << "(" << s << ") show" << endl;
    grestore();

}

void psDoc::BottomLabel(int x, const char* s)
{//make a bottom label, at x position

    if (!hasfont)initfont();
    gsave();
    moveto(x, 2+fontscale/3);
    tx << "(" << s << ") show" << endl;
    grestore();

}

void psDoc::BottomLabel(const char* s)
{//make a bottom label, centered

    int x;

    if (!hasfont)initfont();
    x = (c-a)/2-strlen(s)*fontscale/4+a;
    gsave();
    moveto(x, 2+fontscale/3);
    tx << "(" << s << ") show" << endl;
    grestore();
}

void psDoc::TopLabel(int x, const char* s)
{//make a bottom label, at x position

    if (!hasfont)initfont();
    gsave();
    moveto(x, d-fontscale);
    tx << "(" << s << ") show" << endl;
    grestore();

}

void psDoc::TopLabel(const char* s)
{//make a bottom label, centered

    int x;

    if (!hasfont)initfont();
    x = (c-a)/2-strlen(s)*fontscale/4+a;
    gsave();
    moveto(x, d-fontscale);
    tx << "(" << s << ") show" << endl;
    grestore();
}

void psDoc::plotR(double* d, int n, int aa, int bb, int cc, int dd)
{

    double cmp, max, min, ar, br, cr, dr, scale, scayle;
    int i;

    if (d==0)return;
    ar = ((double) aa);
    br = ((double) bb);
    cr = ((double) cc);
    dr = ((double) dd);
    max = d[0];
    min = d[0];
    for (i = 1; i<n; i++) {
        if (d[i]>max)max = d[i];
        if (d[i]<min)min = d[i];
    }
    scale = max-min;
    if (scale==0) {
        moveto(ar, (br+dr)/2);
        lineto(cr, (br+dr)/2);
        stroke();
    }
    else {
        scale = scale/(dr-br);
        scayle = ((double) n)/(cr-ar);
        moveto(ar, (d[0]-min)/scale+br);
        for (i = 1; i<n; i++) {
            lineto(ar+i/scayle, (d[i]-min)/scale+br);
        }
        stroke();
    }
}

void psDoc::plotR(double* d, int n, int aa, int bb, int cc, int dd,
        double min, double max)
{

    double cmp, ar, br, cr, dr, scale, scayle;
    int i;

    if (d==0)return;
    ar = ((double) aa);
    br = ((double) bb);
    cr = ((double) cc);
    dr = ((double) dd);
    scale = max-min;
    if (scale==0) {
        moveto(ar, (br+dr)/2);
        lineto(cr, (br+dr)/2);
        stroke();
    }
    else {
        scale = scale/(dr-br);
        scayle = ((double) n)/(cr-ar);
        moveto(ar, (d[0]-min)/scale+br);
        for (i = 1; i<n; i++) {
            lineto(ar+i/scayle, (d[i]-min)/scale+br);
        }
        stroke();
    }
}

void psDoc::frame(double x0, double y0, double x1, double y1,
        int aa, int bb, int cc, int dd)
{

    char s[20];

    sprintf(s, "%f", x0);
    sayAT(aa+10, bb, s);
    sprintf(s, "%f", y0);
    sayAT(aa, bb+10, s);
    sprintf(s, "%f", x1);
    sayAT(cc-30, bb, s);
    sprintf(s, "%f", y1);
    sayAT(aa, dd-10, s);
}

//histogrammer
void psDoc::histPS(double* data,  //vector or reals to be histogrammed
        int z,         //number of data items
        double min,    //minimum independant value
        double max,    //maximum independant value
        double width,  //width of a bar
        int tall)
{     //max height - overridden if too small

    int* q;
    int h, w, i, j, ymax;
    int left, right, top;
    char quix[10];

    w = (int) ((max-min)/width+0.999999);
    q = new int[w];
    for (i = 0; i<w; i++)q[i] = 0;
    for (i = 0; i<z; i++) {
        if (data[i]<min)cout << "Too small in postscript histogram" << endl;
        if (data[i]>max)cout << "Too large in postscript gistogram" << endl;
        h = (int) ((data[i]-min)/width);
        q[h]++;
    }
    ymax = 0;
    for (i = 0; i<w; i++)if (q[i]>ymax)ymax = q[i];
    if (tall>ymax)ymax = tall;
    left = 13;
    for (i = 0; i<w; i++) {
        right = (c-a-10)*(i+1)/(w)+7;
        top = (d-b-10)*(q[i])/ymax+10;
        box(left, 10, right, top, 0, 0);
        left = right+3;
    }
    sprintf(quix, "%g", min);
    sayAT(a+13, b, quix);
    sprintf(quix, "%g", (max+min)/2);
    sayAT((a+10+c-strlen(quix))/2, b, quix);
    sprintf(quix, "%g", max);
    sayAT(c-strlen(quix)*6-3, b, quix);

    sprintf(quix, "%d", 0);
    sayAT(a, b+10, quix);
    sprintf(quix, "%g", ((double) ymax)/2);
    sayAT(a, (b+10+d)/2, quix);
    sprintf(quix, "%d", ymax);
    sayAT(a, d-10, quix);
    delete[] q;
}

//make a dozer   //ll corner and heading
void psDoc::MakeDozer(float a, float b, int d, float z)
{

    switch (d) {
    case 0:moveto(a+z, b);
        lineto(a+5*z/6, b);
        lineto(a+5*z/6, b+z/6);
        lineto(a+z/2, b+z/6);
        lineto(a+z/2, b);
        lineto(a+z/3, b);
        lineto(a+z/3, b+z);
        lineto(a+z/2, b+z);
        lineto(a+z/2, b+5*z/6);
        lineto(a+5*z/6, b+5*z/6);
        lineto(a+5*z/6, b+z);
        lineto(a+z, b+z);
        closepath();
        moveto(a+z/3, b+z/2);
        lineto(a+z/6, b+z/2);
        moveto(a, b);
        lineto(a+z/6, b+z/6);
        lineto(a+z/6, b+5*z/6);
        lineto(a, b+z);
        break;
    case 1:moveto(a, b+z);
        lineto(a, b+5*z/6);
        lineto(a+z/6, b+5*z/6);
        lineto(a+z/6, b+z/2);
        lineto(a, b+z/2);
        lineto(a, b+z/3);
        lineto(a+z, b+z/3);
        lineto(a+z, b+z/2);
        lineto(a+5*z/6, b+z/2);
        lineto(a+5*z/6, b+5*z/6);
        lineto(a+z, b+5*z/6);
        lineto(a+z, z+b);
        closepath();
        moveto(a+z/2, b+z/3);
        lineto(a+z/2, b+z/6);
        moveto(a, b);
        lineto(a+z/6, b+z/6);
        lineto(a+5*z/6, b+z/6);
        lineto(a+z, b);
        break;
    case 2:moveto(a, b);
        lineto(a+z/6, b);
        lineto(a+z/6, b+z/6);
        lineto(a+z/2, b+z/6);
        lineto(a+z/2, b);
        lineto(a+2*z/3, b);
        lineto(a+2*z/3, b+z);
        lineto(a+z/2, b+z);
        lineto(a+z/2, b+5*z/6);
        lineto(a+z/6, b+5*z/6);
        lineto(a+z/6, b+z);
        lineto(a, b+z);
        closepath();
        moveto(a+2*z/3, b+z/2);
        lineto(a+5*z/6, b+z/2);
        moveto(a+z, b);
        lineto(a+5*z/6, b+z/6);
        lineto(a+5*z/6, b+5*z/6);
        lineto(a+z, b+z);
        break;
    case 3:moveto(a, b);
        lineto(a, b+z/6);
        lineto(a+z/6, b+z/6);
        lineto(a+z/6, b+z/2);
        lineto(a, b+z/2);
        lineto(a, b+2*z/3);
        lineto(a+z, b+2*z/3);
        lineto(a+z, b+z/2);
        lineto(a+5*z/6, b+z/2);
        lineto(a+5*z/6, b+z/6);
        lineto(a+z, b+z/6);
        lineto(a+z, b);
        closepath();
        moveto(a+z/2, b+2*z/3);
        lineto(a+z/2, b+5*z/6);
        moveto(a, b+z);
        lineto(a+z/6, b+5*z/6);
        lineto(a+5*z/6, b+5*z/6);
        lineto(a+z, b+z);
        break;
    }
    stroke();

}

void psDoc::MakeStar(float h, float k, float r, int n, int m)
{

    float x[n], y[n];
    int i;

    for (i = 0; i<n; i++) {
        x[i] = h+r*cos(2*pi*i/n);
        y[i] = k+r*sin(2*pi*i/n);
    }
    startpath();
    moveto(x[0], y[0]);
    for (i = 1; i<n; i++)lineto(x[m*i%n], y[m*i%n]);
    closepath();
    stroke();
}

void psDoc::FillStar(float h, float k, float r, int n, int m)
{

    float x[n], y[n];
    int i;

    for (i = 0; i<n; i++) {
        x[i] = h+r*cos(2*pi*i/n);
        y[i] = k+r*sin(2*pi*i/n);
    }
    startpath();
    moveto(x[0], y[0]);
    for (i = 1; i<n; i++)lineto(x[m*i%n], y[m*i%n]);
    closepath();
    fill();
}

//make a polygon with center h,k radisu r, n points, the phase
void psDoc::polygon(int n, float h, float k, float r, float th)
{

    float x[n], y[n];
    int i;

    for (i = 0; i<n; i++) {
        x[i] = h+r*cos(2*pi*i/n+th);
        y[i] = k+r*sin(2*pi*i/n+th);
    }
    moveto(x[0], y[0]);
    for (i = 1; i<n; i++)lineto(x[i%n], y[i%n]);
    closepath();
    stroke();

}

//make a filled polygon with center h,k radisu r, n points, the phase
void psDoc::fillpoly(int n, float h, float k, float r, float th)
{

    float x[n], y[n];
    int i;

    for (i = 0; i<n; i++) {
        x[i] = h+r*cos(2*pi*i/n+th);
        y[i] = k+r*sin(2*pi*i/n+th);
    }
    moveto(x[0], y[0]);
    for (i = 1; i<n; i++)lineto(x[i%n], y[i%n]);
    closepath();
    fill();

}

//make a symbot at position h,k going in direction the with radius r
void psDoc::MakeSymbot(float h, float k, float r, float th)
{

    int ns;       //number of sides for the symbots's body
    float xe, ye;  //eye positions
    float er;     //eye radius

    //compute number of sides for body
    ns = 2*((int) (r/(pi)))+6;
    //draw body
    polygon(ns, h, k, r, th);
    //compute eye radius
    er = r/10.0;
    if (er<3.0)er = 3.0;
    //first eye
    xe = h+(r-er/2)*cos(th+pi/4);
    ye = k+(r-er/2)*sin(th+pi/4);
    fillpoly(8, xe, ye, er, th);
    //second eye
    xe = h+(r-er/2)*cos(th-pi/4);
    ye = k+(r-er/2)*sin(th-pi/4);
    fillpoly(8, xe, ye, er, th);
    //lateral line
    xe = h+r/3*cos(th);
    ye = k+r/3*sin(th);
    moveto(xe, ye);
    //lateral line
    xe = h-r/3*cos(th);
    ye = k-r/3*sin(th);
    lineto(xe, ye);
    stroke();
}

//make a boxplot
void psDoc::BoxPlot(double* data, int n,     //dataset
        double dlo, double dhi,  //data range
        int plo, int phi,        //pixel range
        int psn,                //middle of boxplot
        int width,              //width of boxplot
        int vert                //1=vertical 0=horizontal
)
{

    double Q1, MD, Q3, min, max, IQR;
    int lo, hi, md, llo, hhi, val;
    double lov, hiv;
    int i;
    int* flavor;

    flavor = new int[n];

    //go get the information on quartiles
    QuartileAnalysis(data, n, Q1, MD, Q3, min, max);
    IQR = Q3-Q1;  //find the IQR

    //cout << IQR << endl;

    lov = hiv = MD;
    for (i = 0; i<n; i++) {
        if ((data[i]-MD<1.5*IQR) && (data[i]>hiv))hiv = data[i];
        if ((MD-data[i]<1.5*IQR) && (data[i]<lov))lov = data[i];
        if (abs(data[i]-MD)<1.5*IQR)flavor[i] = 0;
        else if (abs(data[i]-MD)>3.0*IQR)flavor[i] = 2;
        else
            flavor[i] = 1;
    }

    //cout << lov << " " << Q1 << " " << MD << " " << Q3 << " " << hiv << endl;

    PRNps(dlo, dhi, plo, phi, lov, llo);
    PRNps(dlo, dhi, plo, phi, Q1, lo);
    PRNps(dlo, dhi, plo, phi, MD, md);
    PRNps(dlo, dhi, plo, phi, Q3, hi);
    PRNps(dlo, dhi, plo, phi, hiv, hhi);

    if (vert) {
        startpath();

        //main box
        moveto(psn-width/2, lo);
        lineto(psn-width/2, hi);
        lineto(psn+width/2, hi);
        lineto(psn+width/2, lo);
        closepath();
        stroke();

        //median position
        moveto(psn-width/2, md);
        lineto(psn+width/2, md);
        stroke();

        //limbs
        moveto(psn, hi);
        lineto(psn, hhi);
        stroke();
        moveto(psn-width/2, hhi);
        lineto(psn+width/2, hhi);
        stroke();
        moveto(psn, lo);
        lineto(psn, llo);
        stroke();
        moveto(psn-width/2, llo);
        lineto(psn+width/2, llo);
        stroke();

        //plot the outliers
        for (i = 0; i<n; i++)
            if (flavor[i]>0) {
                PRNps(dlo, dhi, plo, phi, data[i], val);
                if (flavor[i]==1)glyph(psn, val, 0, 3); else glyph(psn, val, 1, 3);
            }

        //glyph(psn,hhi,2,5);

    }
}

//make a boxplot
void psDoc::InfBoxPlot(double* data, int n,     //dataset
        double dlo, double dhi,  //data range
        int plo, int phi,        //pixel range
        int psn,                //middle of boxplot
        int width,              //width of boxplot
        int vert                //1=vertical 0=horizontal
)
{

    double Q1, MD, Q3, min, max, IQR;
    int lo, hi, md, llo, hhi, val;
    double lov, hiv, civ;
    int i;
    int* flavor;
    int ilo, ihi;

    flavor = new int[n];

    //go get the information on quartiles
    QuartileAnalysis(data, n, Q1, MD, Q3, min, max);
    IQR = Q3-Q1;  //find the IQR



    lov = hiv = MD;
    for (i = 0; i<n; i++) {
        if ((data[i]-MD<1.5*IQR) && (data[i]>hiv))hiv = data[i];
        if ((MD-data[i]<1.5*IQR) && (data[i]<lov))lov = data[i];
        if (abs(data[i]-MD)<1.5*IQR)flavor[i] = 0;
        else if (abs(data[i]-MD)>3.0*IQR)flavor[i] = 2;
        else
            flavor[i] = 1;
    }
    civ = CI95ps(data, n);

    PRNps(dlo, dhi, plo, phi, lov, llo);
    PRNps(dlo, dhi, plo, phi, Q1, lo);
    PRNps(dlo, dhi, plo, phi, MD, md);
    PRNps(dlo, dhi, plo, phi, Q3, hi);
    PRNps(dlo, dhi, plo, phi, hiv, hhi);
    PRNps(dlo, dhi, plo, phi, MD-civ, ilo);
    PRNps(dlo, dhi, plo, phi, MD+civ, ihi);

    if (vert) {

        //main box
        startpath();
        moveto(psn-width/2, lo);
        lineto(psn-width/2, ilo);
        lineto(psn-width/4, md);
        lineto(psn-width/2, ihi);
        lineto(psn-width/2, hi);
        lineto(psn+width/2, hi);
        lineto(psn+width/2, ihi);
        lineto(psn+width/4, md);
        lineto(psn+width/2, ilo);
        lineto(psn+width/2, lo);
        closepath();
        stroke();

        //median bar
        moveto(psn-width/4, md);
        lineto(psn+width/4, md);
        stroke();

        //arms
        moveto(psn, hi);
        lineto(psn, hhi);
        stroke();
        moveto(psn-width/2, hhi);
        lineto(psn+width/2, hhi);
        stroke();
        moveto(psn, lo);
        lineto(psn, llo);
        stroke();
        moveto(psn-width/2, llo);
        lineto(psn+width/2, llo);
        stroke();

        //plot the outliers
        for (i = 0; i<n; i++)
            if (flavor[i]>0) {
                PRNps(dlo, dhi, plo, phi, data[i], val);
                if (flavor[i]==1)glyph(psn, val, 0, 3); else glyph(psn, val, 1, 3);
            }

        //glyph(psn,hhi,2,5);

    }

    delete[] flavor;
}

/*This section contains local helpful functions
 */

void QuartileAnalysis(double* data, int n, //data and number of data items
        double& Q1,         //first quartile
        double& MD,         //median
        double& Q3,         //third quartile
        double& min,        //data minimum
        double& max         //data maximum
)
{

    int* dx;
    int sw, fl;
    int i;

    dx = new int[n];
    for (i = 0; i<n; i++)dx[i] = i;
    do {
        fl = 0;
        for (i = 0; i<n-1; i++) {
            if (data[dx[i]]>data[dx[i+1]]) {
                sw = dx[i];
                dx[i] = dx[i+1];
                dx[i+1] = sw;
                fl = 1;
            }
        }
    }
    while (fl==1);
    Q1 = data[dx[n/4]];
    MD = data[dx[n/2]];
    Q3 = data[dx[(3*n)/4]];
    min = data[dx[0]];
    max = data[dx[n-1]];

    //for(i=0;i<n;i++)cout << data[dx[i]] << " ";
    //cout << endl;

    delete[] dx;

}

void PRNps(double dlo,  //data minimum
        double dhi,  //data maximum
        int plo,     //position minimum
        int phi,     //position maximum
        double val,  //value to be pixelated
        int& pval    //pixel value
)
{

    double delta;

    delta = dhi-dlo;
    val -= dlo;
    val /= delta;
    if (val>1.0)val = 1.0; else if (val<0.0)val = 0.0;
    pval = plo+((int) ((phi-plo)*val));

}

double CI95ps(double* data, int n)
{

    double mu, sg;
    int i;

    mu = sg = 0.0;
    for (i = 0; i<n; i++) {
        mu += data[i];
        sg += data[i]*data[i];
    }
    mu /= ((double) n);
    sg /= ((double) n);
    sg -= (mu*mu);
    sg = sqrt(sg);
    return (1.96*sg/sqrt((double) (n-1)));

}
