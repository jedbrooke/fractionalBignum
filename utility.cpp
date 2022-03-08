#include "utility.hpp"

namespace util {
    void octoword_slli(u_int64_t v[2], int n) {
        if (n > 0) {
            if(n >= 64) {
                v[0] = v[1];
                v[1] = 0L;
                v[0] <<= (n - 64);
            } else {
                // save n upper bits of fraction
                u_int64_t mask = 0xFFFFFFFFFFFFFFFF >> (64 - n);
                mask <<= (64 - n);
                u_int64_t bits = v[1] & mask;
                bits >>= (64 - n);
                v[1] <<= n;
                v[0] <<= n;

                // add the n upper bits from the fraction to the integer
                v[0] += bits;
            }
        }
        if(n < 0) {
            n = abs(n);
            if(n >= 64) {
                v[1] = v[0];
                v[0] = 0L;
                v[0] >>= (n - 64);
            } else {
                // save n lower bits of integer
                u_int64_t mask = 0xFFFFFFFFFFFFFFFF << (64 - n);
                mask >>= (64 - n);
                u_int64_t bits = v[0] & mask;
                bits <<= (64 - n);
                v[1] >>= n;
                v[0] >>= n;

                // add the n lower bits from the integer to the fraction
                v[1] += bits;
            }
        }
    }

    std::string base10add(const char* a, const char* b, size_t D) {
        
        auto carry = 0;
        char* s = (char*) malloc(D * sizeof(char));

        for(int i = D-1; i >=0; i--) {
            s[i] = (a[i] - '0') + (b[i] - '0') + carry;
            carry = 0;
            if(s[i] > 9) {
                s[i] %= 10;
                carry = 1;
            }
            s[i] += '0';
        } 
        std::string str(s,D);
        return str;
    }

}