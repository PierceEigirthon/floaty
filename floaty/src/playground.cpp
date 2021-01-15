// SIMPLE TERMINAL PLAYGROUND WHERE USER CAN INPUT STUFF
#include <iostream> 
#include <string>
#include "floaty/converter32.h"


void play_float(){
    std::cout << "WARNING: adjusting for overflows not implemented yet!\n";
    std::string user_data;
    while(true){
        std::cout << "please input your float: ";
        std::cin >> user_data;
        if(user_data == "exit") return;
        if(!ValidateInputReconvert(user_data.c_str())){
            std::cout << "sorry, your input can't be converted to binary :(\n"; 
        }
        else{
            char bin_buffer[32];
            Reconvert(user_data.c_str(), bin_buffer);
            std::cout << "binary representation: " << bin_buffer << "\n"; //note "\n" is faster than endl, no flush :^) 
        } 
    } 
}


void play_bin(){
    std::string user_data;
    while(true){ 
        std::cout << "please input your binary string: ";
        std::cin >> user_data; 
        if(user_data == "exit") return;
        if(!ValidateInputConvert(user_data.c_str())){
            std::cout << "sorry, your input can't be converted to a float :(\n"; 
        }
        else{ 
            std::cout << "your float is " << Convert(user_data.c_str()) << "\n";
        } 
    }
}


int main(){ 
    std::cout << "Welcome to the playground!\n";
    std::cout << "Select your desired mode:\n"; 
    int user_selection;
    while(true){ 
        std::cout << "0 - convert 32-bit floating numbers to their binary representations\n"; 
        std::cout << "1 - convert binary representations to 32-bit floating numbers\n"; 
        std::cin >> user_selection;
        if(user_selection == 0 || user_selection == 1) break;
        std::cout << "Please select either 0 or 1 :)\n"; 
    }
    std::cout << "input \"exit\" to leave the playground\n";
    if(user_selection == 0) play_float(); 
    else play_bin(); 
    std::cout << "I hope you enjoyed the playgound! :3 \n"; 
}


