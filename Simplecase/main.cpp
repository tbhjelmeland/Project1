#include <iostream>     //function for input and output to my function/main
#include <fstream>
#include <iomanip>      //manipulator function for in-/output streams
#include <cmath>
#include <string>
#include <ctime>        //to time the program

// use namespace for output and input
using namespace std;

// object for output files
ofstream ofile;
ofstream myfile;

// Functions used
double f(double x){return 100.0*exp(-10.0*x);}
double exact(double x) {return 1.0-(1-exp(-10))*x-exp(-10*x);}

// Begin main program
int main(int argc, char *argv[]){
    int exponent;
    string filename;
    // We need two user input; first name for the output file and than n which defines the different matrix sizes
    if( argc <= 1 ){
          cout << "Bad Usage: " << argv[0] <<
              " We need two user input; first the basic name for the output file and than the highest power of 10^n we want on matrix size" << endl;
          exit(1);
    }
        else{
        filename = argv[1];
        exponent = atoi(argv[2]);        
    }
    myfile.open(filename);
    myfile << setiosflags(ios::showpoint | ios::uppercase);
    myfile << "    n:       CPU (sec):       nr FLO:          nr FLOPS:" << endl;

    // Loop over powers of 10
    for (int i = 1; i <= exponent; i++){
     int  n = (int) pow(10.0,i);
     // Declare new file name
     string fileout = filename;
     // Convert the power 10^i to a string
     string argument = to_string(i);
     // Final filename as filename-i-
     fileout.append(argument);

     double h = 1.0/(n);
     double hh = h*h;
     // Set up arrays for the simple case
     double *d = new double [n+1];
     double *b = new double [n+1];
     double *solution = new double [n+1];
     double *x = new double [n+1];

     // Quick setup of updated diagonal elements and value of
     d[0] = d[n] = 2;
     solution[0] = solution[n] = 0.0;

     clock_t begin = clock();
     for (int i = 1; i < n; i++){
         d[i] = (i+1.0)/( (double) i);         
     }
     for (int i = 0; i <= n; i++){
         x[i] = i*h;
         b[i] = hh*f(x[i]);         
     }
     // Forward substitution
     for (int i = 2; i < n; i++){
         b[i] = b[i] + b[i-1]/d[i-1];         
     }
     // Backward substitution
     solution[n-1] = b[n-1]/d[n-1];
     for (int i = n-2; i > 0; i--){
         solution[i] = (b[i]+solution[i+1])/d[i];         
     }
     ofile.open(fileout);
     ofile << setiosflags(ios::showpoint | ios::uppercase);
     ofile << " x:             approx:          exact:       relative error:" << endl;
     for (int i = 1; i < n;i++) {
        double RelativeError = fabs((exact(x[i])-solution[i])/exact(x[i]));
        ofile << setw(0) << setprecision(8) << x[i];
        ofile << setw(15) << setprecision(8) << solution[i];
        ofile << setw(15) << setprecision(8) << exact(x[i]);
        ofile << setw(15) << setprecision(8) << log10(RelativeError) << endl;
     }
     ofile.close();
     delete [] x; delete [] d; delete [] b; delete [] solution;

     clock_t end = clock();
     double elapsed_secs = double(end-begin)/ CLOCKS_PER_SEC;
     cout << "Elapsed time during computations in seconds = " << elapsed_secs << endl;

     //Calculate flops,
     double FLO = double ((n-1)*2 + (n-2)*2 + (n-2)*3 + (n-1)*5); //incl. fabs
     //double FLO = double ((n-1)*2 + (n-2)*2 + (n-2)*3);
     double FLOPS = double (FLO/elapsed_secs);
     cout << "Number of FLO = " << FLO << endl;
     cout << "Number of FLOPS = " << FLOPS << endl;

     myfile << setw(7) << setprecision(8) << n;
     myfile << setw(15) << setprecision(8) << elapsed_secs;
     myfile << setw(15) << setprecision(8) << FLO;
     myfile << setw(15) << setprecision(8) << FLOPS << endl;

   }
    myfile.close();
   return 0;
    //
}

