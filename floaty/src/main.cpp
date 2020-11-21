// IMPLEMENT ADDITIONAL STUFF FROM: https://www.youtube.com/watch?v=wbxSTxhTmrs 

int POWERBASEX(const int exp, int base, int res = 1) {
    for (int i = 0; i < exp; ++i) res *= base;
    return res;
}
int POWBASE2(const int exp) { return POWERBASEX(exp, 2); }
int POWBASE10(const int exp) { return POWERBASEX(exp, 10); }

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
        if (*x == '1') exponent += POWBASE2(exp_pow);
    }
    for (; x != end; ++x, ++mantissa_pow) {
        if (*x == '1') mantissa += (1.0 / POWBASE2(mantissa_pow));
    }

    return sign * (mantissa)*POWBASE2(exponent);
}

void Reconvert(const char x[], char out[], int N) {
    // get sign
    if (*x == '-') {
        out[0] = '1';
        ++x;
    } else
        out[0] = '0';

    // Convert natural part from chars to an integer
    int natural_part = 0;
    // TODO: figure out what to do for inputs ("10000") with no .
    for (; *x != '.'; ++x) {
        natural_part = natural_part * 10 + (*x - '0');
    }
    ++x;  // move past '.'

    // Convert decimal part from chars to numerator and a denominator
    int numerator = 0, denominator = 0;
    for (; *x != '\0'; ++x, ++denominator)
        numerator = numerator * 10 + (*x - '0');
    denominator = POWBASE10(denominator);

    // convert to fixed representation
    constexpr int M = 24;  // 23 mantissa for 32 float, we discard last 1 and
                           // add when back to float "(1 + mantissa) ....
    char bitstore[M] = {'0', '0', '0', '0', '0', '0', '0', '0',
                        '0', '0', '0', '0', '0', '0', '0', '0',
                        '0', '0', '0', '0', '0', '0', '0', '0'};
    char tmp_whole[M] = {'0', '0', '0', '0', '0', '0', '0', '0',
                         '0', '0', '0', '0', '0', '0', '0', '0',
                         '0', '0', '0', '0', '0', '0', '0', '0'};

    int idx = 0;
    while (natural_part >= 1 && idx < M) {
        if (natural_part % 2) tmp_whole[idx] = '1';
        natural_part /= 2;
        ++idx;
    }

    if (!idx) ++idx;  // no natural part? so it's 0
    // move from temp to bitstore
    for (int i = idx - 1; i >= 0; --i) {
        bitstore[(idx - 1) - i] = tmp_whole[i];
    }

    // get the exponent part (8 bits)
    int exp = (idx - 1) + 127;
    for (int i = 8; i >= 1; --i, exp /= 2) {
        out[i] = (exp % 2) ? '1' : '0';
    }

    // convert the decimal part
    for (; idx < M; ++idx) {
        numerator *= 2;
        if (numerator >= denominator) {
            bitstore[idx] = '1';
            numerator -= denominator;
        }
    }

    // ROUNDING: https://www.youtube.com/watch?v=wbxSTxhTmrs
    if ((numerator * 2) > denominator) {
        --idx; 

        while (bitstore[idx] != '0') {
            bitstore[idx] = '0';
            --idx;
        }
        bitstore[idx] = '1';
    }

    for (int i = 9; i < 32; ++i) {
        out[i] = bitstore[i - 8];
    }
    out[32] = '\0';
}
