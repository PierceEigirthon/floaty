// case_verifiers.h
#ifndef CASE_VERIFIERS_H
#define CASE_VERIFIERS_H


#include <fstream>
#include <sstream>
//#include <iostream> 




#include "case_generators.h" 
#include "floaty/converter32.h"

// Abstract BASE VERIFIER class
template <class I, class R>
class VerifierBase{
public:
    /*
    Main verification method

    ARGS:
        case_size - size of case, say, case_size=5 will generate "12.34"      
        n_cases - how many cases to generate, will get rounded to nearest power of 2
        include_invalid - whether to generate incorrect cases
        verbose - well, speaks for itself :D
        path - path to relevant .txt file in data folder
    */
    void verify(std::string path, int case_size, int n_cases, bool include_invalid, bool verbose);     

    
    /* neither generatorbase or its children owns any memory resources, so this should be ok */
    ~VerifierBase(){ if(this->_gen) delete this->_gen; } 



//TODO: will become protected
//protected: 
//
//
    GeneratorBase* _gen = nullptr;
    std::vector<std::pair<I,R>> _edge_cases; 
    std::vector<I> _generated_cases;
    // function pointer to help loading different functions from src
    R (*_cmp)(const char []); 
    /* Loads cases from text files located in data folder */
    void _load_edge_cases(std::string file_path); 
    /* generates cases given generator provided in Constructor */
    void _generate_cases(int case_size, int n_cases, int include_invalid); 

    // pure virtual methods
    virtual void _print_on_verify_start() = 0;
    virtual bool _compare(I x, R y) = 0;
    virtual bool _compare_with_lib(I x) = 0;
};

//
class VerifierInputConvert : public VerifierBase<std::string, bool>{ 
public:
    VerifierInputConvert(bool(*fn)(const char[]));
protected:    
    void _print_on_verify_start() override; 
    bool _compare(std::string x, bool y) override;
    bool _compare_with_lib(std::string x) override; 
};

//
class VerifierConvert : public VerifierBase<std::string, float>{ 
public:
    VerifierConvert(float(*fn)(const char[]));
protected:
    void _print_on_verify_start() override; 
    bool _compare(std::string x, float y) override;
    bool _compare_with_lib(std::string x) override; 
private:
    // helper methods for comparison of 2 floats
    bool _is_equal(const float a, const float b);
};

//
class VerifierInputReconvert: public VerifierBase<std::string, bool>{
public:
    VerifierInputReconvert(bool(*fn)(const char[]));
protected:
    void _print_on_verify_start() override; 
    bool _compare(std::string x, bool y) override;
    bool _compare_with_lib(std::string x) override; 
};


//
class VerifierReconvert: public VerifierBase<std::string, bool>{
public:
    VerifierReconvert(bool(*fn)(const char[]));
protected:
    void _print_on_verify_start() override; 
    bool _compare(std::string x, bool y) override;
    bool _compare_with_lib(std::string x) override; 
};



#endif /* CASE_VERIFIERS_H */ 
