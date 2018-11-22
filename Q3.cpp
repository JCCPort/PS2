#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <string>



/**
 * Defining strings to use for pretty printing of arrays.
 */
const std::string sep = "|" ;
const std::string top =    "┌---------┬---------┬---------┐"  ;
const std::string line =   "├---------┼---------┼---------┤"  ;
const std::string bottom = "└---------┴---------┴---------┘"  ;


void swapr(double &firstValue, double &secondValue){
    double tempValue = firstValue;
    firstValue = secondValue;
    secondValue = tempValue;
}


void transpose(double table[3][3]){
    for(unsigned char i = 0; i <= 2; i++){
        for(unsigned char j = 0; j < i; j++){
            if(i!=j){
                swapr(table[i][j], table[j][i]);
            }
        }
    }
}

void printm(double array[3][3]){
    std::cout <<  top << std::endl;
    for(unsigned char i = 0; i <= 2; i++){
        std::cout << sep;
        for(unsigned char j = 0; j <= 2; j++){
            std::cout << std::setw(7) << std::fixed << std::setprecision(2) << array[i][j] << "  " << sep;
        }
        if(i<2){
            std::cout << "\n" << line << "\n";
        }
    }
    std::cout << "\n" << bottom << std::endl;
    std::cout << "\n" << std::endl;
}

int main(){
    double table [3][3] = {{0.36, 0.48, -0.8},
                           {-0.8, 0.6, 0.0},
                           {0.48, 0.64, 0.6}};
    std::cout << "\nARRAY:" << std::endl;
    printm(table);
    transpose(table);
    std::cout << "\nTRANSPOSED ARRAY:" << std::endl;
    printm(table);
    std::cout << "\n\n" << std::endl;
}