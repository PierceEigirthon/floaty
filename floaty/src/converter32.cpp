// TODO: remove these later
#include <bits/stdc++.h>
using namespace std;

// IMPLEMENT ADDITIONAL STUFF FROM: https://www.youtube.com/watch?v=wbxSTxhTmrs

bool ValidateInputConvert(const char x[]) {
    // If termination token is not at correct position, it's not valid 
    if (x[32] != '\0') return false;
    // we check if all characters are either '1' or '0'
    for (int i = 0; i < 32; ++i) {
        if ((x[i] != '1') && (x[i] != '0')) return false;
    }
    return true;
}

bool ValidateInputReconvert(const char x[]){
    int dot_cnt = 0;

    if(x[0] == '\0') return false;
    else if(x[0] == '-'){
        if(x[1] == '\0') return false;
        else if(x[1] == '.' && x[2] == '\0') return false;
    }
    else if(x[0] == '.'){
        if(x[1] == '\0') return false;
        ++dot_cnt;
    }
    else if(47 > x[0] || x[0] > 58) return false; 


    for(int idx = 1; x[idx] != '\0'; ++idx){
        if(47 < x[idx] && x[idx] < 58) continue;
        else if(x[idx] == '.') {
            ++dot_cnt;
            if(dot_cnt != 0 && dot_cnt != 1) return false;
        }
        else return false;
    } 
    return true;
} 

// SIGN * (1 + MANTISSA) * 2 ^ (EXPONENT - 127)
float Convert(const char x[]) {
    // get the addresses where exponent and mantissa part ends
    const char* exponent_end = x + 9;
    const char* end = x + 32;

    int sign = (x[0] == '0') ? 1 : -1; ++x; 
    // calculate the exponent
    int exponent = -127;
    for (int exp_pow = 7; x != exponent_end; ++x, --exp_pow) {
        if (*x == '1') exponent += (1 << exp_pow);
    } 
    // get the numerator for mantissa part
    // we only perform addition and bit shifts where, no divisions 
    int mant_num = 1;
    static constexpr int mant_den = 0x800000; // denominator value of last mantissa bit
    for (; x != end; ++x) {
        mant_num <<= 1;
        if (*x == '1') ++mant_num;
    } 

    if (exponent > 0) return sign * static_cast<float>(mant_num) / mant_den * (1 << exponent);
    return sign * static_cast<float>(mant_num) / mant_den / (1 << -exponent);
}

void Reconvert(const char x[], char out[]) { 
    // convert the sign
    if (*x == '-') { out[0] = '1'; ++x;}
    else out[0] = '0'; 
    // convert natural part from chars to an integer
    int natural_part = 0;
    for (; *x != '.'; ++x) {
        natural_part = natural_part * 10 + (*x - '0');
    }
    ++x;  // move past '.' 

    // Convert decimal part from chars to numerator and a denominator
    int numerator = 0, denominator = 1;
    for (; *x != '\0'; ++x) {
        numerator = numerator * 10 + (*x - '0');
        denominator = denominator * 10;
    } 
    /* 
    First, we look for the first bit that is 1 in natural_part.
    When we find it, we skip it (since we add 1 in Convert formula, we can ignore it).
    Then, we start adding natural part bits to our output.  
    */
    int idx = 0;
    bool started = false;
    for (int i = 31; i >= 0; --i) {
        if (started) {
            out[9 + idx++] = (natural_part & (1 << i)) ? '1' : '0';
        } else if (!started && (natural_part & (1 << i))) {
            started = true;
        }
    } 
    // get the exponent part (8 bits)
    // idx denotes how many shifts we need to get the natural part back
    int exp = idx + 127; 
    for (int i = 8; i >= 1; --i, exp /= 2) { 
        out[i] = (exp % 2) ? '1' : '0';
    } 
    // convert the decimal part 
    for (; (9 + idx) < 32; ++idx) {
        numerator *= 2;
        if (numerator >= denominator) {
            out[9 + idx] = '1';
            numerator -= denominator;
        } else out[9 + idx] = '0';
    } 
    // ROUNDING: https://www.youtube.com/watch?v=wbxSTxhTmrs
    if ((numerator * 2) > denominator) {
        --idx;
        while (out[9 + idx] != '0') {
            out[9 + idx] = '0';
            --idx;
        }
        out[9 + idx] = '1';
    }
    out[32] = '\0';
}
