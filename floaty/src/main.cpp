// IMPLEMENT ADDITIONAL STUFF FROM: https://www.youtube.com/watch?v=wbxSTxhTmrs


// must be 32 long, must contain either 1 or 0 chars
bool ValidateInputConvert(const char x[]) {
    if (x[32] != '\0') return false;
    for (int i = 0; i < 32; ++i) {
        if ((x[i] != '1') && (x[i] != '0')) return false;
    }
    return true;
}
// TODO: has to contain at least 1 digit, 1 or 0 '.' and optional '-' at the
// start
bool ValidateInputReconvert(const char x[]) { return true; }





// SIGN * (1 +MANTISSA) * 2 ^ (EXPONENT - 127)
float Convert(const char x[]) {
    static constexpr int N = 32;

    int exp_pow = 7;
    int mantissa_pow = 1;

    int sign = (x[0] == '0') ? 1 : -1;
    ++x;  // we got sign skip to next token
    int exponent = -127;
    float mantissa = 1.0;

    const char *exponent_end = x + 8;
    const char *end = x + N;

    for (; x != exponent_end; ++x, --exp_pow) {
        if (*x == '1') exponent += (1 << exp_pow);
    }
    for (; x != end; ++x, ++mantissa_pow) {
        if (*x == '1') mantissa += (1.0 / (1 << mantissa_pow));
    } 
    return sign * (mantissa)*(1 << exponent);
}









void Reconvert(const char x[], char out[], int N) {

    // get sign
    if (*x == '-') {
        out[0] = '1';
        ++x;
    } else {
        out[0] = '0';
    }

    // Convert natural part from chars to an integer
    int natural_part = 0;
    // TODO: figure out what to do for inputs ("10000") with no .
    for (; *x != '.'; ++x) {
        natural_part = natural_part * 10 + (*x - '0');
    }
    ++x;  // move past '.'



    // Convert decimal part from chars to numerator and a denominator
    int numerator = 0, denominator = 1;
    for (; *x != '\0'; ++x){
        numerator = numerator * 10 + (*x - '0');
        denominator = denominator * 10;
    }


    int idx = 0;
    bool started = false;
    for (int i = 31; i >= 0; --i) {
        if (started){
            out[9 + idx++] = (natural_part & (1 << i)) ? '1' : '0';
        }
        else if (!started && (natural_part & (1 << i))){
            started = true;
        }
    }

    // get the exponent part (8 bits)
    // TODO: make this with binary stuff and what not
    int exp = idx + 127;
    for (int i = 8; i >= 1; --i, exp /= 2) {
        out[i] = (exp % 2) ? '1' : '0';
    }
    // convert the decimal part
    // TODO: we can do early cut off here if we want (if like its 0 then it cant
    // get bigger so all esle are 0
    for (; (9 + idx) < 32; ++idx) {
        numerator *= 2;
        if (numerator >= denominator) {
            out[9 + idx] = '1';
            numerator -= denominator;
        } else
            out[9 + idx] = '0';
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
