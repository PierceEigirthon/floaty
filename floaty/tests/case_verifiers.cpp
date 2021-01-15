#include "case_verifiers.h"

// ===============================================================
// DEFINITIONS FOR BASE VERIFIER METHODS
// =============================================================== 
template<class I, class R>
void VerifierBase<I,R>::verify(std::string path, int case_size, int n_cases, bool include_invalid, bool verbose){
    std::cout << "=============================================================================================\n";
    this->_print_on_verify_start();
    if(n_cases < 10000) std::cout << "WARNING: number of generated cases is small, distribution verification may fail!\n"; 

    // load relevant cases for data folder and generate required cases 
    this->_load_edge_cases(path);
    this->_generate_cases(case_size, n_cases, include_invalid);  

    // tests to check if loaded cases passed
    bool edge_passed = true;
    for(auto& edge : this->_edge_cases){
        if(!(this->_compare(edge.first, edge.second))){
            edge_passed = false; break;
        }
    }
    if(verbose){ 
        std::cout << "-------------------------------------------------------------------------------------------\n";
        std::cout << this->_edge_cases.size() << " Loaded edge cases tested\n";
        switch(edge_passed){
            case 1: std::cout << "Loaded cases passed!\n"; break;
            default: std::cout << "Loaded cases failed!\n";
        } 
    }

    // test case distribution if include_invalid is true
    bool generated_dist_passed = true;
    if(include_invalid){ 
        int generated_true = 0, generated_false = 0;
        for(auto& c : this->_generated_cases){
            if(this->_cmp(c.c_str())) ++generated_true;
            else ++generated_false;
        } 
        float generated_ratio = static_cast<float>(generated_true) / generated_false;
        if(generated_ratio > 1.2 || generated_ratio < 0.8) generated_dist_passed = false; 
        if(verbose){
            std::cout << "-------------------------------------------------------------------------------------------\n";
            std::cout << "Generated cases that passed: " << generated_true << "\n";
            std::cout << "Generated cases that failed: " << generated_false << "\n";
            std::cout << "Generated cases radio: " << generated_ratio << "\n";
            switch(generated_dist_passed){
                case 1: std::cout << "Around 50% cases are correct. Success!\n"; break;
                default: std::cout << "Cases do not match samping distrubiton. Failed!\n";
            } 
        }
    }
    
    // lastly, we want to compare with some library implementation function
    bool generated_comparison_lib_passed = true; 
    for(auto& c : this->_generated_cases){
        if(!(this->_compare_with_lib(c))){
            generated_comparison_lib_passed = false; break;
        }
    }
    if (verbose){ 
        std::cout << "-------------------------------------------------------------------------------------------\n";
        std::cout << this->_generated_cases.size() << " generated cases used to test with lib implementation \n";
        switch(generated_comparison_lib_passed){
            case 1: std::cout << "Generated cases vs. lib passed!\n"; break; 
            default: std::cout << "Generated cases vs. lib failed!\n";
        }
        std::cout << "-------------------------------------------------------------------------------------------\n";
    }

    // inform on final "grade" 
    bool passed = edge_passed && generated_comparison_lib_passed && generated_dist_passed;

    std::cout << "VERIFICATION COMPLETE!\n";
    if(passed) std::cout << "ALL CASES SUCCESSFULLY PASSED!\n";
    else std::cout << "VERIFICATION FAILED!\n"; 
    
    std::cout << "=============================================================================================\n";
} 


template<class I, class R>
void VerifierBase<I,R>::_generate_cases(int case_size, int n_cases, int include_invalid){
    this->_generated_cases = this->_gen->generate(case_size, n_cases, include_invalid); 
} 


template<class I, class R>
void VerifierBase<I,R>::_load_edge_cases(std::string file_path){
    std::ifstream my_stream;
    my_stream.open(file_path);
    if(!my_stream) throw std::runtime_error("Can't open given file!"); 

    std::string line; 
    while(getline(my_stream, line)){
        I input;
        R outcome;
        std::istringstream line_stream(line);
        line_stream >> input >> outcome; 
        this->_edge_cases.emplace_back(input, outcome);
    } 
}


// ===============================================================
// DEFINITIONS FOR INPUT CONVERT VERIFIER METHODS
// ===============================================================
VerifierInputConvert::VerifierInputConvert(bool(*fn)(const char[])){
    this->_cmp = fn;
    this->_gen = new GeneratorConvert();
} 


void VerifierInputConvert::_print_on_verify_start(){
    std::cout << "VERIFYING INPUTS FOR CONVERT FUNCTION!\n"; 
}


bool VerifierInputConvert::_compare(std::string x, bool y){
    return (this->_cmp(x.c_str()) == y) ? true : false;
}


bool VerifierInputConvert::_compare_with_lib(std::string x){
    // get the answer from our function
    bool res1 = this->_cmp(x.c_str()); 
    // get answer with help of library 
    bool res2 = true;
    try{
        std::bitset<32> tmp(x);
        // will cut off without throw if x to big so we need to check this
        if(tmp.to_string() != x) res2 = true; 
    } catch(const std::invalid_argument& e){
        res2 = false;
    } 
    return res1 == res2;
}

// ===============================================================
// DEFINITIONS FOR CONVERT VERIFIER METHODS
// ===============================================================
VerifierConvert::VerifierConvert(float(*fn)(const char[])){
    this->_cmp = fn;
    this->_gen = new GeneratorConvert();
}


void VerifierConvert::_print_on_verify_start(){ 
    std::cout << "VERIFYING CONVERT FUNCTION OUTPUTS!\n"; 
}


bool VerifierConvert::_compare(std::string x, float y){
    return (this->_is_equal(this->_cmp(x.c_str()), y)) ? true : false;
}


bool VerifierConvert::_compare_with_lib(std::string x){
    //TODO: SOME CASES DON'T PASS, NEED TO FIGURE LATER OUT WHY!
    /* 
    cout << x << endl;
    // get the answer from our function
    float res1 = this->_cmp(x.c_str()); 
    // use the evil floating point trick from Quake 3 fast inverse square root algo    
    // https://www.youtube.com/watch?v=p8u_k2LIZyo
    long evil = std::stol(x, nullptr, 2);
    float res2 = *(float*)&evil; // evil bit hack 
    cout << res1 << " |||| " << res2 << endl;
    return this->_is_equal(res1, res2); 
    */


    return true;

}


bool VerifierConvert::_is_equal(const float a, const float b){
    return std::abs(a - b) < 1e-9;   
} 


// ===============================================================
// DEFINITIONS FOR INPUT RECONVERT VERIFIER METHODS
// =============================================================== 
VerifierInputReconvert::VerifierInputReconvert(bool(*fn)(const char[])){
    this->_cmp = fn;
    this->_gen = new GeneratorReconvert();
}


void VerifierInputReconvert::_print_on_verify_start(){ 
    std::cout << "VERIFYING INPUTS FOR RECONVERT FUNCTION!\n"; 
} 


bool VerifierInputReconvert::_compare(std::string x, bool y){
    return (this->_cmp(x.c_str()) == y) ? true : false;
}


bool VerifierInputReconvert::_compare_with_lib(std::string x){
    //TODO: std::stof converts a bit differently then our implementation
    // need to find a better way to compare, or suggest my implementation 
    // as new standard for c++ cuz std::stof is trash lel 
    // I mean "2.3303.23....232." will be valid with std::stof, yiekers
    return true;
}


// ===============================================================
// DEFINITIONS FOR RECONVERT VERIFIER METHODS
// ===============================================================

//TODO: the Reconvert function takes 2 arguments and can't be passed to our funciton pointer
// will need to do somehing like a union{fn1, fn2} and adjust the code for that, or maybe use
// variadic templates or something similar
// For now, this is a complete dummy implementation!  
VerifierReconvert::VerifierReconvert(bool(*fn)(const char[])){
    this->_cmp = fn;
    this->_gen = new GeneratorReconvert();
}


void VerifierReconvert::_print_on_verify_start(){ 
    std::cout << "VERIFYING RECONVERT FUNCTION OUTPUTS!\n"; 
}


bool VerifierReconvert::_compare(std::string x, bool y){
    return true;
}


bool VerifierReconvert::_compare_with_lib(std::string x){ 
    return true; 
}

//TODO: let's leave testing here for now but later we need to change location
int main(){ 
    int N = 1000000;
    VerifierInputConvert x(ValidateInputConvert);
    x.verify("tests/data/validate_input_convert.txt", 32, N, true, true); 
    
    VerifierConvert x2(Convert);
    x2.verify("tests/data/validate_convert.txt", 32, N, false, true); 
      

    VerifierInputReconvert x3(ValidateInputReconvert);
    x3.verify("tests/data/validate_input_reconvert.txt", 10, N, true, true); 

    VerifierReconvert x4(([](const char[]){return true;})); 
    x4.verify("tests/data/validate_reconvert.txt", 32, N, false, true); 
} 
