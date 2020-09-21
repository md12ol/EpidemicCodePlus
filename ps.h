/****************************************************
 *                                                  *
 *                                                  *
 *  Interface for postscript generation code        *
 *                                                  *
 *                                                  *
 ****************************************************/

using namespace std;

class psDoc {

public:

    psDoc();                                             //
    psDoc(const char* name);                             //(filename)
    psDoc(const char* name, int aa, int bb, int cc, int dd); //(filename, BB)
    ~psDoc();                                            //closes output file

    //starts a path
    void startpath();
    //close the gap at the end of the path
    void closepath();

    //move the focus or draws a line to it
    void moveto(int x, int y);
    void moveto(float x, float y);
    void moveto(double x, double y);
    void lineto(int x, int y);
    void lineto(float x, float y);
    void lineto(double x, double y);

    //draws an arrow with a head at x,y; a tail at u,v
    void arrow(int u, int v,  //arrow tail
            int x, int y,  //arrow head
            int hz,       //arrow head line length
            double aa     //arrow head angle in degrees
    );

    //the following routines return an integer position in the picture from
    //a value in [0,1] that is the fraction of the way across the picture
    //ASSUMING a margin of margin pixels on all sides.
    int vpos(double y, int margin);
    int hpos(double x, int margin);

    //make a glyph at position x,y
    void glyph(int x, int y, int tp, int z);
    //Glyph Types: 0 vertical cross  1 box  2 triangle pointing up
    // 3 X's  4 -circles  5 - five stars  6 solid boxes 7 solid triangles
    // 8 solid circles

    //save and restore graphics paths
    void gsave();
    void grestore();

    //display a graphic path
    void stroke();

    //set grayscale 1-100
    void setgray(int b);

    //set color
    void setcol(int r, int g, int b);

    //fill a path...
    void fill();

    //set the width of lines
    void setlinewidth(int w);
    //make lines dashed
    void dashed(int w);
    //return to solid lines
    void undashed();

    //create box
    void box(int aa, int bb, int cc, int dd, int rim, int core);
    void boxI(int aa, int bb, int cc, int dd);
    void fillpolygon(int* XY, int n);
    void polygon(int* XY, int n);

    //create a bar
    void bar(int h, int k,    //center of bar
            int r,          //radius
            int d,          //radius of diamond
            int horizontal, //predicate, vertical if false
            int wings       //wing size
    );

    //Primitive text I/O
    void initfont();                //loads times roman font size 12
    void initfont(int size);        //loads TR font size "size"
    void initSfont(int size);       //loads symbol font font size "size"
    void sayAT(int x, int y, const char* s);//put text at a position
    void sayATvert(int x, int y, const char* s);//put text at a position, write vert.
    void LeftLabel(int y, const char* s);  //make a label at the left, at y position
    void LeftLabel(const char* s);        //make a label at the left, centered
    void RightLabel(int y, const char* s); //make a label at the right, at y position
    void RightLabel(const char* s);       //make a label at the right, centered
    void BottomLabel(int x, const char* s); //make a bottom label, at x position
    void BottomLabel(const char* s);       //make a bottom label, centered
    void TopLabel(int x, const char* s); //make a bottom label, at x position
    void TopLabel(const char* s);       //make a bottom label, centered

    //data plotting functions
    void plotR(double* d, int n, int aa, int bb, int cc, int dd);
    void plotR(double* d, int n, int aa, int bb, int cc, int dd,
            double min, double max);
    /* d is a vector of at least n doubles.  They are plotted as a curve
       sized to fit the reigon given by aa,bb,cc,dd.  Min and max are
       hard limits.  If not supplied they are derived from the data.
       */
    void frame(double x0, double y0, double x1, double y1,
            int aa, int bb, int cc, int dd);

    //histogrammer
    void histPS(double* data, int z, double min, double max, double width, int tall);

    //make a dozer   //ll corner and heading; size
    void MakeDozer(float a, float b, int d, float z);
    void MakeStar(float h, float k, float r, int n, int m);
    void FillStar(float h, float k, float r, int n, int m);

    //make a polygon with center h,k radisu r, n points, the phase
    void polygon(int n, float h, float k, float r, float th);
    //make a filld polygon with center h,k radisu r, n points, the phase
    void fillpoly(int n, float h, float k, float r, float th);

    //make a symbot at position h,k going in direction the with radius r
    void MakeSymbot(float h, float k, float r, float th);

    //make a boxplot
    void BoxPlot(double* data, int n,    //dataset
            double dlo, double dhi, //data range
            int plo, int phi,       //pixel range
            int psn,               //middle of boxplot
            int width,             //width of boxplot
            int vert               //1=vertical 0=horizontal
    );

    //make an inflected boxplot
    void InfBoxPlot(double* data, int n,    //dataset
            double dlo, double dhi, //data range
            int plo, int phi,       //pixel range
            int psn,               //middle of boxplot
            int width,             //width of boxplot
            int vert               //1=vertical 0=horizontal
    );

private:

    fstream tx;    //output stream to place document on disk
    char* fn;      //name of .ps document
    int a, b, c, d;   //bounding Box
    int hasfont;   //boolean, checks for the presence of a font
    int fontscale; //int scalar, stores font size
};

void QuartileAnalysis(double* data, int n, //data and number of data items
        double& Q1,         //first quartile
        double& MD,         //third median
        double& Q3,         //third quartile
        double& min,        //data minimum
        double& max         //data maximum
);

void PRNps(double dlo,  //data minimum
        double dhi,  //data maximum
        int plo,     //position minimum
        int phi,     //position maximum
        double val,  //value to be pixelated
        int& pval    //pixel value
);

double CI95ps(double* data, int n);  //compute a 95% CI on the mean
