#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>

using namespace std;

#include "setu.h"
#include "stat.h"

/*************************algorithm controls******************************/
/*
EDIT 1:
The following code was altered to allow the name of the profile to be
passed as a parameter.  The origional code is commented out to provide
an easy way to revert the code.  Note, this should be moved to a better
location at some point.  FIX THIS.
*/
//P2use - The epidemic profile used for infection comparison
//#define pname "Profile1.dat"
//This is the replacement
char* pname;

/*
EDIT 5:
A global variable was needed to pass the output location to the ourput graph.
Therefore, this was implemented here.  A better location should be found.
FIXXX THIIISSSS.
*/
char* outLoc;

//Profile length - for reading in profiles
#define PL 16
//number of sampled epidemics
#define NSE 50

//probability of epidemic spread
#define alpha 0.5
//minimum length of meaningful epidemic
#define mepl 3
//trials for rejecting short epidemics
#define rse 5
//length of final test
#define ftl 50

//Verbosity - print running output, yes=1, no=0
#define verbose 1

//number of runs, number of mating events, reporting interval
#define runs 30
#define mevs 40000
#define RI 400

//number of commands Toggle Hop Add Delete Swap LToggle Null
/*
EDIT 13:
I changed the number of commands to 9 to include LAdd and LDel
*/
#define NmC 9

//minimum degree for swap
#define EDGB 2

/*
EDIT 12:
I replaced the following line in order to facilitate passing the paramaters
via command line.
*/
//Command densities for T.H.A.D.S.L.N.
//double CmD[NmC]={0.14,0.14,0.14,0.14,0.14,0.14,0.16};
// New line
double CmD[NmC];

//population size, vertices per graph, gene length (number of commands)
#define popsize 1000
#define verts 128
#define GL 256

//random number seed
#define RNS 91207819

//size of integer (vertices cubed)
#define MAXL 2097152

//maximum number of mutation for new structure; tournament size
#define MNM 3
#define tsize 7

/**************************Variable dictionary************************/
int pop[popsize][GL];            //population of command strings
double fit[popsize];             //fitness array
int dx[popsize];                 //sorting index
double PD[verts];                //profile dictionary

/****************************Procedures********************************/

void initalg();                        //initialize the algorithm
int validloci();                       //generate an acceptable large integer
void express(graph& G, int* cmd);       //express a command string
void initpop();                        //initialize population
void matingevent();                    //run a mating event
void report(ostream& aus);             //make a statistical report
void reportbest(ostream& aus, int run); //report the best graph

/****************************Main Routine*******************************/
/*
EDIT 2:
I added the parameters to the main method as I am using command line params
to get the name of the profile.
*/
int main(int argc, char* argv[])
{
    /*
    EDIT 4:
    This block of code will be to keep track of what command line parameters
    will be passed to the function:
    1. Profile Name - ./Profiles/Profile#.dat
    2. Output Location For Paramater Settings and Profile - ./Output/PS#/P#/
      The program output will be placed in ./Output/PS#/P#/
      Ensure that the scripts make these folders as the program does not make them
    */

    int run, mev;        //loop index variales for replicate and mating events
    fstream stat, best;  //statistics, best structures
    char fn[60];        //file name construction buffer

    /*
    EDIT 3:
    The following line of code must be added to get the profile name from the
    command line parameters
    */
    pname = argv[1];

    /*
    EDIT 7:
    This line of code takes the command line param and passes it to a global variable
    */
    outLoc = argv[2];

    initalg();  //seed the random number generator, read the profile
    /*
    EDIT 8:
    This line was added in order to create a proper file name and location
    for the best.lint file as one was needed per Parameter Setting and
    Profile combination.
    */
    sprintf(fn, "%sbest.lint", outLoc);
    /*
    EDIT 9:
    This line needed to be changed in order to properly create the file and
    output location.
    */
    //best.open("best.lint",ios::out); // What does this do?
    //New line
    best.open(fn, ios::out);

    /*
    EDIT 11:
    The following lines were added in order to get the values for the parameters
    from command line arguments as compared to hard coding.  I also added the offset
    variable because the parameters are the 3rd through 9th command line parameter.
    The offset represents the difference between the parameter's location in the CmD
    array and it's location within the command line parameters.
    */
    int offset = 3;
    for (int commandParam = 0; commandParam<NmC; commandParam++) {
        CmD[commandParam] = strtod(argv[commandParam+offset], NULL);
    }
    //End addition
    for (run = 0; run<runs; run++) {//loop over runs
        /*
        EDIT 5:
        This edit was completed to file the output of different runs appropriatly.
        outLoc is the command line variable which is the output location desired
        for these runs.
        */
        //Old line commented out
        //sprintf(fn,"run%02d.dat",run);
        //New line
        sprintf(fn, "%srun%02d.dat", outLoc, run); // File name
        stat.open(fn, ios::out);
        initpop(); //initalize a popualtion
        report(stat); //report the statistics of the initial population
        for (mev = 0; mev<mevs; mev++) {//do mating events
            matingevent();  //run a mating event
            if ((mev+1)%RI==0) {//Time for a report?
                if (verbose)cout << run << " " << (mev+1)/RI << " ";
                report(stat); //report statistics
            }
        }
        stat.close();
        reportbest(best, run);
    }
    best.close();

    return (0);  //keep the system happy

}

void initalg()
{//initialize the algorithm

    fstream inp;    //input file
    int i;          //loop index
    char buf[20];   //input buffer

    srand48(RNS);                 //read the random number seed
    inp.open(pname, ios::in);      //open input file
    for (i = 0; i<verts; i++)PD[i] = 0;  //pre-fill missing values
    PD[0] = 1;                      //put in patient zero
    for (i = 0; i<PL; i++) {      //loop over input values
        inp.getline(buf, 19);  //read in the number
        PD[i+1] = atof(buf);    //translate the number
    }

    cout << "Profile: "; //Prints out the profile being tested
    for (i = 0; i<=PL; i++)cout << PD[i] << " ";
    cout << endl;

}

//This routine generates valid loci for the expression routine
int validloci()
{//generate an acceptable large integer

    int cmd;       //command type generated
    double dart;   //Random command

    dart = drand48()-CmD[0];//throw the dart
    cmd = 0;                  //initialize the position on the dartboard
    while ((dart>0) && (cmd<NmC-1))dart -= CmD[++cmd]; //walk the board
    cmd += NmC*(lrand48()%MAXL); //add in the large integer part
    return (cmd);  //return the generated command

}

//This is expression of a large integer represenation
void express(graph& G, int* cmd)
{//express a command string

    int i;      //loop index
    int a, b, c;  //decoded values
    int cdv;    //command value
    int block;  //integer carving block

    G.RNGnm(verts, 2);
    for (i = 0; i<GL; i++) {//loop over the commands (genetic loci)
        block = cmd[i];  //get integer
        cdv = block%NmC; //slice of the command
        block /= NmC;    //clear command information from block
        switch (cdv) {//What command is it?
        case 0://Toggle
            a = block%verts;
            b = (block/verts)%verts; //get vertex numbers
            G.toggle(a, b);    //toggle edge {a,b}
            break;
        case 1://Hop
            //get vertex numbers
            a = block%verts;
            b = (block/verts)%verts;
            c = (block/verts/verts)%verts;
            G.hop(a, b, c);
            break;
        case 2://Add
            a = block%verts;
            b = (block/verts)%verts; //get vertex numbers
            G.add(a, b);       //add edge {a,b}
            break;
        case 3://Delete
            a = block%verts;
            b = (block/verts)%verts; //get vertex numbers
            G.del(a, b);       //delete edge a,b
            break;
        case 4://Swap
            a = block%(verts*10);
            b = (block/(verts*10))%(verts*10); //get vertex numbers
            G.edgeswap(a, b, EDGB);
            break;
        case 5://Local Toggle
            //get vertex numbers
            a = block%verts;
            b = (block/verts)%verts;
            c = (block/verts/verts)%verts;
            G.ltog(a, b, c);
            break;
            /*
            EDIT 14: Added in the code necessary for Local Add and Local Del
            */
        case 6: // Local Add
            //get vertex numbers
            a = block%verts;
            b = (block/verts)%verts;
            c = (block/verts/verts)%verts;
            G.ladd(a, b, c);
            break;
        case 7: // Local Del
            //get vertex numbers
            a = block%verts;
            b = (block/verts)%verts;
            c = (block/verts/verts)%verts;
            G.ldel(a, b, c);
            break;
        case 8: // Null - Do nothing
            break;
        }
    }
}

double fitness(int* cmd)
{//compute the epidemic length fitness

    graph G(verts);      //scratch graph
    int max, len, ttl;     //maximum, length, and total removed
    int cnt;             //counter for tries
    double prof[verts];  //profile variable
    double trials[NSE];  //stores squared error for each trial
    int en;              //epidemic number
    double delta;        //difference between profile and trial
    int i;               //loop index
    int dx[NSE];         //sorting index
    double accu;         //accumulator

    cnt = 0;                        //zero the try counter
    express(G, cmd);               //create the graph
    for (en = 0; en<NSE; en++) {//loop over epidemics
        G.SIRProfile(0, max, len, ttl, alpha, prof); //Acquire profile
        trials[en] = 0;  //zero the current squared error
        if (len<PL+1)len = PL+1;  //find length of epi/prof (longer)
        for (i = 0; i<len; i++) {//loop over time periods
            delta = prof[i]-PD[i];
            trials[en] += delta*delta;
        }
        trials[en] = sqrt(trials[en]/len); //convert to RMS error
    }

    for (i = 0; i<NSE; i++)dx[i] = i;   //initialize the sorting index
    tselect(trials, dx, NSE, NSE);  //sort
    accu = 0.0;                    //zero the accumulator
    for (i = 0; i<NSE; i++) {//loop over trials
        //cout << i+1 << " " << dx[i] << " " << trials[dx[i]] << endl;
        accu += trials[dx[i]]/(i+1); //weighted, sorted error
    }

    return (accu);  //return the fitness value

}

void initpop()
{//initialize population

    int i, j;  //loop index variables

    for (i = 0; i<popsize; i++) {    //loop overthe popualtion

        /***INITIALIZATION CODE***/
        for (j = 0; j<GL; j++)pop[i][j] = validloci(); //fill in the loci
        fit[i] = fitness(pop[i]);  //compute its fitness
        dx[i] = i;                 //refresh the sorting index
    }
}

void matingevent()
{//run a mating event

    int i, rp, sw;   //loop index, random position, swap variable
    int cp1, cp2;   //crossover points

    //perform tournament selection, highest fitness first
    Tselect(fit, dx, tsize, popsize);

    //selection and crossover
    cp1 = lrand48()%GL;
    cp2 = lrand48()%GL;
    if (cp1>cp2) {
        sw = cp1;
        cp1 = cp2;
        cp2 = sw;
    }
    for (i = 0; i<cp1; i++) {
        pop[dx[0]][i] = pop[dx[tsize-2]][i];
        pop[dx[1]][i] = pop[dx[tsize-1]][i];
    }

    for (i = cp1; i<cp2; i++) {
        pop[dx[0]][i] = pop[dx[tsize-1]][i];
        pop[dx[1]][i] = pop[dx[tsize-2]][i];
    }
    for (i = cp2; i<GL; i++) {
        pop[dx[0]][i] = pop[dx[tsize-2]][i];
        pop[dx[1]][i] = pop[dx[tsize-1]][i];
    }

    //muation
    rp = lrand48()%MNM+1;
    for (i = 0; i<rp; i++)pop[dx[0]][lrand48()%GL] = validloci();
    rp = lrand48()%MNM+1;
    for (i = 0; i<rp; i++)pop[dx[1]][lrand48()%GL] = validloci();

    //update fitness
    fit[dx[0]] = fitness(pop[dx[0]]);
    fit[dx[1]] = fitness(pop[dx[1]]);

    //HACKED - skeptical tournament selection not needed
    //since fitness is stochastic, update parental fitness
    //fit[dx[tsize-1]]=fitness(pop[dx[tsize-1]]);
    //fit[dx[tsize-2]]=fitness(pop[dx[tsize-2]]);

}

void report(ostream& aus)
{//make a statistical report
    dset D;

    D.add(fit, popsize);  //load fitness

    //print report
    aus << D.Rmu() << " " << D.RCI95() << " " << D.Rsg()
        << " " << D.Rmin() << endl;
    if (verbose)
        cout << D.Rmu() << " " << D.RCI95() << " " << D.Rsg()
             << " " << D.Rmin() << endl;
}

void reportbest(ostream& aus, int run)
{//report the best graph

    int i, j, b;       //loop indices and best pointer
    double q;        //final test value
    double cf;       //current fitness mark
    double tf;       //tentative fitness for comparison
    graph G(verts);  //scratch graph
    fstream gout;    //graph output file
    char fn[60];     //file name construction buffer

    b = 0;
    for (i = 1; i<popsize; i++)if (fit[i]<fit[b])b = i; //find best fitness

    //output the fitness and the gene
    aus << fit[b] << " -fitness" << endl;
    aus << pop[b][0];
    for (i = 1; i<GL; i++)aus << " " << pop[b][i];
    aus << endl;

    //recover the graph and output it
    express(G, pop[b]);
    /*
    EDIT 10:
    Needed to change this so that the graph would go to the proper output location
    */
    //Old line
    //sprintf(fn,"Graph%d.grf",run);
    //New line
    sprintf(fn, "%sGraph%d.dat", outLoc, run);
    gout.open(fn, ios::out);
    G.write(gout);
    gout.close();
}