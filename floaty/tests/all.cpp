#include <utility>
#include <cmath>
#include <vector>
#include <iostream> 
#include <string>
#include <cassert>
#include <cstring>


#include "floaty/converter32.h"


constexpr int N = 32;


void TestReconvert(bool VERBOSE = false){ 
    // TEST "Reconvert" FUNCTION
    std::vector<std::pair<const char*, const char*>> casesR;
    casesR.emplace_back("17.125","01000001100010010000000000000000");
    casesR.emplace_back("-17.125", "11000001100010010000000000000000");
    casesR.emplace_back("200.5", "01000011010010001000000000000000");
    casesR.emplace_back("-137.875","11000011000010011110000000000000");
    casesR.emplace_back("13.998046875","01000001010111111111100000000000");
    // cases with rounding precision errors 
    casesR.emplace_back("4.21","01000000100001101011100001010010");
    casesR.emplace_back("-233.33333","11000011011010010101010101010101");
    int N = casesR.size(); 

    for(int i = 0 ; i < N ; ++i) {
        char out[N];
        Reconvert(casesR[i].first, out);
        //TODO: WHAT THE ACTUAL FUCK WHY IS VERBOSE 42 HERE ??
        if(false) {
            std::cout << casesR[i].first << std::endl;        
            std::cout << casesR[i].second<< std::endl;        
            std::cout << out << std::endl;        
        }

        assert(strcmp(out, casesR[i].second) == 0); 
    }
    printf("RECONVERT CASES PASSED!\n"); 
    

}



int main () { 
    //TestReconvert();


} 
