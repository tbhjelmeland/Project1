#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <string>
// use namespace for output and input
using namespace std;

// object for output files
ofstream ofile;
// Functions used
double f(double x){
    return 100.0*exp(-10.0*x);
}
double exact(double x) {
    return 1.0-(1-exp(-10))*x-exp(-10*x);
}

//We want our program to take as arument output_filename, one diagonal array and two lower and upper
//diagonal array; d, d_l and d_u respectivly. Want to call them with call by reference sow that
//changes made in the program also is avaliable outside the function


// Begin main program
int main(int argc, char *argv[]){
    int exponent;
    string filename;
    int a;
    int d;
    int b;
    // Argument test
    if( argc <= 4 ){
          cout << "Bad Usage: " << argv[0] <<
              "pass otput filename, max power 10^n, upper diagonal, diagonal, lower diagonal" << endl;
          exit(1);
    }
        else{
        filename = argv[1]; // first command line argument after name of program
        exponent = atoi(argv[2]);
        //cout << "Highest exponent in 10^n, define quadratic matrix sizes: " << argv[2] << endl;
        //cout << "filename: " << argv[1] << endl;
        a = atoi(argv[3]);
        int d = argv[4];
        int b = argv[5];
    }
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
     // Set up arrays
     double *a = new double [n];
     double *d = new double [n+1];
     double *b = new double [n];
     double *solution = new double [n+1];
     double *x = new double [n+1];
     // Quick setup of updated diagonal elements and value of
     d[0] = d[n] = d;
     //cout << "Before we added dialog elements, only endpoints: " << *d << endl;
     solution[0] = solution[n] = 0.0;
     for (int i = 1; i < n; i++) d[i] = (i+1.0)/( (double) i); //ENDRE
     for (int i = 0; i <= n; i++){
         x[i] = i*h;
         b[i] = hh*f(x[i]);  //ENDRE
     }
     // Forward substitution
     for (int i = 2; i < n; i++) b[i] = b[i] + b[i-1]/d[i-1];
     // Backward substitution
     solution[n-1] = b[n-1]/d[n-1];
     for (int i = n-2; i > 0; i--) solution[i] = (b[i]+solution[i+1])/d[i];
     ofile.open(fileout);
     ofile << setiosflags(ios::showpoint | ios::uppercase);
     //      ofile << "       x:             approx:          exact:       relative error" << endl;
     for (int i = 1; i < n;i++) {
      double RelativeError = fabs((exact(x[i])-solution[i])/exact(x[i]));
        ofile << setw(15) << setprecision(8) << x[i];
        ofile << setw(15) << setprecision(8) << solution[i];
        ofile << setw(15) << setprecision(8) << exact(x[i]);
        ofile << setw(15) << setprecision(8) << log10(RelativeError) << endl;
     }
     ofile.close();
     delete [] x; delete [] d; delete [] b; delete [] solution;
   }
   return 0;
}
