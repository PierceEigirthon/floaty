#include "case_generators.h"

// ===============================================================
// DEFINITIONS FOR BASE GENERATOR METHODS
// =============================================================== 
std::vector<float> GeneratorBase::_get_event_probabilities(int N, bool include_invalid){ 
    throw std::logic_error("_get_event_probabilities not implemented for base class, please use of children classes");
    return {};
} 

std::string GeneratorBase::_gen_case( int N, std::discrete_distribution<int>& dist, std::mt19937& rng){ 
    throw std::logic_error("_gen_case not implemented for base class, please use of children classes");
    return ""; 
}


GeneratorBase::GeneratorBase() :
    _valid_events(),
    _n_workers(std::thread::hardware_concurrency()) {} 

GeneratorBase::~GeneratorBase() {} // to please the compiler


std::vector<std::string> GeneratorBase::generate(int case_size, int n_cases, bool include_invalid){
    
    if(this->_n_workers > n_cases) throw std::runtime_error("number of cases can't be less than available hardware threads");

    // increase/decrease to nearest power of 2 
    // this will break if n_cases >= 2^31, but who is gonna generate so many cases lel?  
    if(!((n_cases & (n_cases -1)) == 0)){ 
        int upper_bound = 1 << (32 - __builtin_clz(n_cases)); 
        int lower_bound = upper_bound >> 1;
        if (upper_bound - n_cases < n_cases - lower_bound) n_cases = upper_bound;
        else n_cases = lower_bound;
    } 

    // get the discrete distribution for given events 
    std::vector<float> events = this->_get_event_probabilities(case_size, include_invalid); 
    
    // create a container for generated cases
    //TODO: will need to use a heap array here, since we can't control when vector will decide to reallocate
    std::vector<std::string> cases(n_cases); 
    int step_size = n_cases / this->_n_workers; 
    std::thread workers[this->_n_workers];
    for(int i = 0; i < n_cases; i+= step_size){ 
        workers[i/step_size] = std::thread(
                &GeneratorBase::_generate, this,
                case_size, std::ref(cases), std::ref(events),  
                i, i + step_size);
    } 
    for(int i = 0; i < this->_n_workers; ++i) workers[i].join(); 
    return cases; // TODO: check if you need to move so no copy 
}


void GeneratorBase::_generate(int case_size, std::vector<std::string>& cases, std::vector<float>& events, int start, int end){ 
    static thread_local std::random_device rd;
    static thread_local std::mt19937 rng(rd());
    static thread_local std::discrete_distribution<int> dist(events.begin(), events.end()); 
    for(int i = start; i < end; ++i) cases[i] = this->_gen_case(case_size, dist, rng);
} 


// ===============================================================
// DEFINITIONS FOR CONVERT GENERATOR METHODS
// =============================================================== 
GeneratorConvert::GeneratorConvert() : GeneratorBase(){ 
    // set '1' and '0' to be valid events
    this->_valid_events[48] = 1;
    this->_valid_events[49] = 1;
} 


std::vector<float> GeneratorConvert::_get_event_probabilities(int N, bool include_invalid) { 

    if(N > this->ARRAY_SIZE_LIMIT) throw std::range_error("Big N may cause invalid events to underflow!\n"); 

    std::vector<float> events(this->ASCII_SIZE); 
    int n_valid = static_cast<int>(this->_valid_events.count()); 

    float v_e_p;
    float inv_e_p;
    if(include_invalid){ 
        v_e_p = 1.0/pow(2.0, 1.0/N);
        inv_e_p = (1.0 - v_e_p) / (this->ASCII_SIZE - n_valid);
        v_e_p/= n_valid; 
    }else{ 
        v_e_p = 1.0 / n_valid;  
        inv_e_p = 0.0;
    }

    for(int i = 0; i < this->ASCII_SIZE; ++i){
        events[i] = this->_valid_events[i] ? v_e_p : inv_e_p;
    } 
    return events; 
} 


std::string GeneratorConvert::_gen_case(int N, std::discrete_distribution<int>& dist, std::mt19937& rng) { 
    std::string res(N, 42); 
    for(int i = 0; i < N; ++i) res[i] = dist(rng); 
    return res; 
} 


// ===============================================================
// DEFINITIONS FOR RECONVERT GENERATOR METHODS
// =============================================================== 
// TODO: FOR NOW CONTAINS SIMILAR IMPLEMENTATINO TO GeneratorConver
// SINCE  I WAS NOT ABLE TO GET NEEDED PROBABILITY DISTRIBUTION :(
GeneratorReconvert::GeneratorReconvert() : GeneratorBase() { 
    for(int i = 48; i < 58; ++i) this->_valid_events[i] = 1;
} 


std::vector<float> GeneratorReconvert::_get_event_probabilities(int N, bool include_invalid) { 
    if(N > this->ARRAY_SIZE_LIMIT) throw std::range_error("Big N may cause invalid events to underflow!\n"); 

    std::vector<float> events(this->ASCII_SIZE); 
    int n_valid = static_cast<int>(this->_valid_events.count()); 

    float v_e_p;
    float inv_e_p;
    if(include_invalid){ 
        v_e_p = 1.0/pow(2.0, 1.0/N);
        inv_e_p = (1.0 - v_e_p) / (this->ASCII_SIZE - n_valid);
        v_e_p/= n_valid; 
    }else{ 
        v_e_p = 1.0 / n_valid;  
        inv_e_p = 0.0;
    }

    for(int i = 0; i < this->ASCII_SIZE; ++i){
        events[i] = this->_valid_events[i] ? v_e_p : inv_e_p;
    } 
    return events; 
}


std::string GeneratorReconvert::_gen_case(int N, std::discrete_distribution<int>& dist, std::mt19937& rng) { 
    //TODO: for now need since didn't finish deriving single distribution
     
    static thread_local std::random_device rd_dot;
    static thread_local std::mt19937 rng_dot(rd_dot());
    thread_local float dot_e_p = 1.0 / pow(N, 2); 
    static thread_local std::discrete_distribution<int> dot_dist({1.0 - dot_e_p, dot_e_p}); 
    
    static thread_local std::random_device rd_min;
    static thread_local std::mt19937 rng_min(rd_min()); 
    static thread_local std::uniform_int_distribution<int> min_dist(0,1); 

    std::string res(N, 42); 
    for(int i = 0; i < N; ++i) res[i] = dist(rng); 
    for(int i = 0; i < N; ++i){
        if(dot_dist(rng_dot)) res[i] = '.';
    }
    if (min_dist(rng_min)) res[0] = '-';

    return res; 
}
