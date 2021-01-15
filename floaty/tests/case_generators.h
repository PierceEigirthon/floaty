// case_generators.h
#ifndef CASE_GENERATORS_H
#define CASE_GENERATORS_H

#include <random>
#include <bitset>
#include <vector>
#include <thread>
#include <stdexcept>


// Abstract BASE GENERATOR class 
class GeneratorBase{
public:
    GeneratorBase(); 
    virtual ~GeneratorBase();
        

    /*
    Main function which generates test cases.

    ARGS:
        case_size: wanted size for case (MAX->ARRAY_SIZE_LIMIT)
        n_cases: number of cases to generate (will get rounded to power of 2,
                 either up or down, depending which is power is closer)
        include_invalid: wether to include incorrect cases 
    */
    virtual std::vector<std::string> generate(int case_size, int n_cases, bool include_invalid); 

protected:
    // CONSTANTS
    static constexpr int ASCII_SIZE = 128;
    static constexpr int ARRAY_SIZE_LIMIT = 50; 
    // protected variables
    std::bitset<128> _valid_events;
    const int _n_workers;


    /* 
    This method calculates the probabilities of events in such way that 
    valid and invalid events both have 50% chance of occurance, with regards 
    to given size N.
    

    ARGS:
        N - size of the character array
        include_invalid - if set to false, all valid events will have
        same probability of occurance, while invalid will have 0% occurance 
    */ 
    virtual std::vector<float> _get_event_probabilities(int N, bool include_invalid);


    /*
    This method generates a singe case given the probability distribution
    provided by _get_event_probabilities method.

    ARGS:
        N - size of character array
        dist - discrete probability distribution
        rng - pseudo-random number generator
    */ 
    virtual std::string _gen_case(
            int N, 
            std::discrete_distribution<int>& dist,
            std::mt19937& rng); 
    

    /*
    This method is used by worker threads to fill up test cases.
    Each threads gets different indexes to populate parts of test cases
    array, so no data race can occur (2 threads don't use same location)
    
    In this implementation each thread(which is mapped to hardware thread)
    uses a different mt engine and distribution due to problems that can arise
    from using single pseudo-random engines.
    For more information regarding this please refer to:
    https://channel9.msdn.com/Events/GoingNative/2013/rand-Considered-Harmful
    

    ARGS:
        case_size - size of character array
        events - disrete probability distribution for events
        start - start index of cases array that needs to be filled
        end - end index of cases array that needs to be filled(exclusive) 
    */
    virtual void _generate(
            int case_size,
            std::vector<std::string>& cases, //TODO: may want to replace this
            std::vector<float>& events,
            int start,
            int end);



};


class GeneratorConvert : public GeneratorBase{
public:
    GeneratorConvert(); 
protected:
    /*
    We can get the required probabilities for event by calculating:
    p = 1/(2^(1/N)) where N is number of events.  
    Assumes events are independent from each other.  
    */
    std::vector<float> _get_event_probabilities(int N, bool include_invalid) override;
    /* Creates a string of specified size and starts generating from given distribution */
    std::string _gen_case(int N, std::discrete_distribution<int>& dist, std::mt19937& rng) override; 
};


class GeneratorReconvert : public GeneratorBase{
public:
    GeneratorReconvert(); 
protected:
    std::vector<float> _get_event_probabilities(int N, bool include_invalid) override;
    std::string _gen_case(int N, std::discrete_distribution<int>& dist, std::mt19937& rng) override; 

};

#endif /* CASE_GENERATORS_H */
