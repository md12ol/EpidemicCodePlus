/*
 *   Interface for a class implimenting sets of natural numbers
 *
 */
#ifndef    _SETU_H
#define    _SETU_H

using namespace std;

#define SETINCR 10

//fitness proportional selector used in simulations
int rselect(double* v, double ttl, int N);

class set {

public:

    set();                 //default constructor
    set(int* m, int z);     //construct with a list of elements
    set(const set&);       //copy constructor
    ~set();                //destructor

    //utilities
    void create(int* m, int z);   //create a set with a given membership and size
    void destroy();              //destroy a set
    void setempty();             //mark as empty for mass allocation
    void copy(set&);            //copy another set
    void copyO(set&, int q);     //copy another set with an offset
    void enlarge();              //increment max
    int add(int z);              //add a member, returns true if a not ALREADY
    int remo(int z);             //remove a member, returns true if in there
    void clear();                //make the set empty

    int infected(int n, double alpha);//SIR utility routine

    //information
    int size();                //what is the size of the set
    int memb(int z);           //is z a member?  0=no 1=yes
    int memz(int z);           //zth member

    //operations
    void unyun(set& A, set& B);   //union
    void inter(set& A, set& B);   //intersection
    void setdf(set& A, set& B);   //set difference
    void symdf(set& A, set& B);   //symmetric difference
    double sumAt(double* ft);    //sum indecies of ft in the set
    int FPS(double* ft);         //fitness proportional selection

    //input-output
    void write(ostream& aus);     //write set
    void writememb(ostream& aus); //write members on one line
    void read(istream& inp);      //read set
    void readmemb(istream& inp);  //read members on one line

private:

    int max;   //maximum number of objects in the set
    int n;     //number of objects in the set
    int* mem;  //members, sorted into increasing order


};

/***********************SET BASED GRAPH UNIT*************************/

class graph {

public:

    //constructors and destructors
    graph();           //initialize an empty structure
    graph(int max);    //initialize to maximum of M vertices
    ~graph();          //delete s structure

    //utilities
    void create(int max);     //create with max maximum vertices
    void destroy();           //deallocate everything
    void Enlarge(int newmax); //increase maximum number of vertices to newmax
    void clearE();            //change graph to empty

    int infected(int n, double alpha); //SIR utility routine

    //initializers
    void empty(int n);            //empty graph
    void Kn(int n);               //complete
    void Knm(int n, int m);        //complete bipartite graph
    void Cn(int n);               //cycle
    void Pn(int n, int m);         //Petersen n,m
    void Hn(int dim);             //Hypercube
    void RNGnm(int n, int m);      //Ring with +/-m neighbors
    void UTAM(int* ed);           //initialize from an upper tri. adj. matrix
    void WalkO(int* wk, int wl);   //overlaying walk representation
    void WalkT(int* wk, int wl);   //toggling walk representation
    void EdgeLst(int* el, int ne); //Edge list
    void ADTS(int** el, int L);    //implement an add delete toggle swap
    void HADTS(int** el, int L);   //implement a hop add delete toggle swap
    void copy(graph& other);      //copy another graph

    //modifiers
    void add(int a, int b);             //force an edge to add
    void del(int a, int b);             //force an edge to be gone
    void toggle(int a, int b);          //toggle an edge
    void simplexify(int a);            //simplexify at a
    void hop(int v, int n1, int n2);     //perform a hoperation
    void ltog(int v, int n1, int n2);    //perform a local toggle
    void ladd(int v, int n1, int n2);  //perform a local add
    void ldel(int v, int n1, int n2);  //perform a local del
    void edgeswap(int a, int b, int k);  //decode and perform an edge swap
    //with degree bound k
    void Absorb(graph& other);         //add a copy of other to yourself
    void Prism();                      //create the prism of a graph

    //information
    int size();                 //number of vertices
    int edges();                //number of edges
    int edgeP(int a, int b);     //is a-b an edge?
    void dfrom(int z, int* ds);  //distances from z
    void dfrom0(int z, int* ds); //distances from z in color zero
    int ecc(int z);             //eccentricity of a vertex
    int diameter();             //eccentricity of a vertex
    int radius();               //eccentricity of a vertex
    int connectedP();           //is the graph connected?
    void eccSeq(int* ecs);      //compute the eccentricity sequence
    int nbrmod(int v, int n);    //compute the n%degreeth neighbor of v
    int degree(int v);          //report the degree of v
    double meandegree();        //report the mean degree of the graph
    int Nbrs(int v, int* nb);    //report the neighboors of v
    int MaxCol();               //report the maximal color

    //Simulation methods
    /* Run and SIR epidemic with a given patient zero; return maximum number
     * of people infected, length of epidemic, total number infected.
     * The parameter alpha is the probability of passing an infection
     */
    void SIR(int p0, int& max, int& len, int& ttl, double alpha); //SIR method
    void SIRProfile(int p0, int& max, int& len, int& ttl, double alpha,
            double* prof); //Sir Method, with profile

    /*SIRr is the same as SIR except that patient zero is assigned at random*/
    void SIRr(int& max, int& len, int& ttl, double alpha); //SIR method

    /*
     *   Run an attack until disconnection simulation on the graph.  The
     *   vector vulnerability must be a probability distribution on the
     *   vertices.  Vertices are chosen at random until the graph
     *   becomes disconnected and the number of vertices until
     *   disconnection is returned.  Getting pr right is the users
     *   responsibility.  The routine uses the color array to mark dead
     *   vertices.
     */
    int attack(double* pr);  //probabalistic attack method

    //color methods
    void setC2(int vl);         //set colors to vl
    void GDC(int* ord);         //run the greedy coloring algorithm/order ord
    int AUC(double* gn, int tg); //run Austrailian coloring with target tg

    //genetics
    int FPN(int v, double* ft); //perform fitness perprotional neighbor selection

    //I-O
    void write(ostream& aus);   //write the graph
    void read(istream& inp);    //read the graph
    void writeC(ostream& aus);  //write the colors

private:

    int M;    //maximum number of vertices
    int V;    //number of vertices
    int E;    //number of edges
    set* nbr; //neighbor lists are stored as sets
    int* clr; //colors

};

#endif /* _SETU_H */
