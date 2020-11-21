#include <utility>
#include <cmath>
#include <vector>
#include <iostream> 
#include <string>
#include <cassert>
#include <cstring>



#include "floaty/main.h"




constexpr int N = 32;


bool EqualFloats(const float& a, const float& b) {return std::abs(a - b) < 1e-9;}


void TestConvert(bool VERBOSE = false){ 
    // TEST  "Convert" FUNCTION
    std::vector<std::pair<const char*, float>> cases;
    cases.emplace_back("01000001100010010000000000000000", 17.125);
    cases.emplace_back("11000001100010010000000000000000", -17.125);
    cases.emplace_back("01000011010010001000000000000000", 200.5);
    cases.emplace_back("11000011000010011110000000000000", -137.875); 
    cases.emplace_back("01000001010111111111100000000000", 13.998046875);

    //cases with rounding precision errors 
    cases.emplace_back("01000000100001101011100001010010", 4.21);
    cases.emplace_back("11000011011010010101010101010101", -233.33333);
   
    for (auto& casex: cases) {
        if (VERBOSE) {
            printf("Computer converted: %.20f\n", casex.second);
            printf("My converted: %.20f\n", Convert(casex.first));
        }
        assert(EqualFloats(Convert(casex.first), casex.second));
    }
    printf("CONVERT CASES PASSED!\n"); 
}

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

    for(int i = 0 ; i < N ; ++i) 
    {
        char out[N];
        Reconvert(casesR[i].first, out, std::string(casesR[i].first).size());
        
        if(VERBOSE)
        {
        std::cout << casesR[i].first << std::endl;        
        std::cout << casesR[i].second<< std::endl;        
        std::cout << out << std::endl;        
    }
        assert(strcmp(out, casesR[i].second) == 0); 
    }
    printf("RECONVERT CASES PASSED!\n"); 
    

}

void TestValidateInputConvert(bool VERBOSE = false){ 
    //TEST "ValidateInputConvert" FUNCTION
    assert(ValidateInputConvert("11111111111111111111111111111111"));
    assert(ValidateInputConvert("00000000000000000000000000000000"));
    assert(ValidateInputConvert("01000000100001101011100001010010"));
    assert(ValidateInputConvert("11001100100001101011100001010010"));
    assert(ValidateInputConvert("00000000100001101011100011011010"));
    
    assert(!ValidateInputConvert("weofuihewfi00011011010"));
    assert(!ValidateInputConvert("2i3ub23ifubbnfi22i"));
    assert(!ValidateInputConvert("00000000100001101011100011011020"));
    assert(!ValidateInputConvert("000000001000011010111000110110102200"));
    assert(!ValidateInputConvert("000000001000011010111000110110100"));
    assert(!ValidateInputConvert("0000001011011010"));
    assert(!ValidateInputConvert(""));
    printf("ValidateInputConvert CASES PASSED!\n"); 
}

int main () { 
    TestConvert();
    TestReconvert();
    TestValidateInputConvert();
} 
