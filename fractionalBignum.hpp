#pragma once

#include <string>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <memory.h>
#include <sstream>
#include <utility>
#include <algorithm>
#include "utility.hpp"

const u_int64_t ALL_ONES = 0xFFFFFFFFFFFFFFFF;
const u_int64_t HIGHEST_ONE = 0x8000000000000000;
const std::string NAN_STR("nan");
const std::string INF_STR("infinity");

template <size_t K>
class fractionalBignum
{
private:
public:
    u_int64_t v[K] = {0};
    bool isNan = false;
    bool isInf = false;
    fractionalBignum();
    fractionalBignum(const fractionalBignum<K>& f);
    fractionalBignum(u_int64_t v[K]);
    fractionalBignum(u_int64_t v);
    fractionalBignum(u_int64_t w[2], int offset);
    fractionalBignum(std::string s, int base=10);
    fractionalBignum(double d);

    static fractionalBignum<K> fromPow2(int p);

    ~fractionalBignum();

    void insert_octoword(u_int64_t w[2], int offset);

    void addInt(u_int64_t d, size_t offset);

    fractionalBignum<K> twos_complement() const;
    void invert();

    void setZero();

    bool isOne();

    bool isZero();

    std::string asBase(util::bases b);

    std::string decimal_str() {return asBase(util::TEN);}
    std::string base10_str();

    std::string binary_str(){return asBase(util::TWO);}
    std::string base2_str();

    std::string hex_str() {return asBase(util::HEX);}
    std::string base16_str(char sep='\0') const;

    std::string base64_str();


    template <size_t L>
    friend std::ostream& operator<<(std::ostream& os, const fractionalBignum<L>& f);

    template <size_t L>
    friend fractionalBignum<L> operator+(const fractionalBignum<L>& a, const fractionalBignum<L>& b);

    template <size_t L>
    friend fractionalBignum<L> operator-(const fractionalBignum<L>& a, const fractionalBignum<L>& b);

    fractionalBignum<K>& operator+=(const fractionalBignum<K>& b);

    template <size_t L>
    friend fractionalBignum<L> operator*(const fractionalBignum<L>& a, const fractionalBignum<L>& b);

    template <size_t L>
    friend fractionalBignum<L> operator<<(const fractionalBignum<L>& a, const size_t s);
    
    template <size_t L>
    friend fractionalBignum<L> operator>>(const fractionalBignum<L>& a, const size_t s);
    
};


template <size_t K>
class fractionalBignumWithInt {
private:
    u_int64_t integer;
    fractionalBignum<K> fraction;
public:
    fractionalBignumWithInt();
    fractionalBignumWithInt(u_int64_t i, fractionalBignum<K> f){
        integer = i;
        fraction = f;
    }
};

template <size_t K>
fractionalBignum<K>::fractionalBignum() {}

template <size_t K>
fractionalBignum<K>::fractionalBignum(const fractionalBignum<K>& f) {
    this->isInf = f.isInf;
    this->isNan = f.isNan;
    std::copy(f.v, f.v + K, std::begin(this->v));
}


template <size_t K>
fractionalBignum<K>::fractionalBignum(u_int64_t v[K]) {
    std::copy(v, v + K, std::begin(this->v));
}

template <size_t K>
fractionalBignum<K>::fractionalBignum(u_int64_t v) {
    this->v[0] = v;
}

template <size_t K>
fractionalBignum<K>::fractionalBignum(std::string s, int base) {
    std::cout << s << "," << base << "," << K << std::endl;
}

template <size_t K>
fractionalBignum<K>::fractionalBignum(u_int64_t w[2], int offset) {
    insert_octoword(w, offset);
}

template <size_t K>
void fractionalBignum<K>::insert_octoword(u_int64_t w[2], int offset) {
    if (offset < 0) {
        this->v[0] = w[1];
    } else {
        if (offset < (signed) K) {
            this->v[offset] = w[0];
        }
        if ((offset + 1) < (signed) K) {
            this->v[offset+1] = w[1];
        }
    }
}

template <size_t K>
void fractionalBignum<K>::addInt(u_int64_t d, size_t offset) {
    if(offset < K) {
        int carry = 0;
        carry = __builtin_add_overflow(this->v[offset], d, &this->v[offset]);
        for(int i = offset - 1; i >= 0; i--) {
            carry = __builtin_add_overflow(this->v[i], carry, &this->v[i]);
        }
    }
}

template <size_t K>
fractionalBignum<K>::fractionalBignum(double d) {
    this->isInf = std::isinf(d);
    this->isNan = std::isnan(d);
    d -= floor(d);
    
    u_int64_t i;
    memcpy(&i, &d, 8);
    
    u_int64_t f;
    int16_t exponent = ((i & 0x7FF0000000000000) >> 52);
    u_int64_t mantissa = (i & 0xFFFFFFFFFFFFF);
    if(exponent == 0 && mantissa == 0) {
        f = 0;
    } else {
        // sign extend the 11 bits to fill a 16 bit signed int
        if(exponent & (1 << 10)) {
            exponent |= 0xF800;
        }
        // adjust exponent as per IEEE 754 spec
        exponent -= 1023;
        // shift f by 12 to fit 52 bits into 64
        f = mantissa << 12;        
    }
    // add implied leading 1 to mantissa as per IEE 754 spec
    u_int64_t v[2] = {1,f};
    util::octoword_slli(v, exponent % 64);
    insert_octoword(v, abs(exponent / 64) - 1);
}

template <size_t K>
fractionalBignum<K> fractionalBignum<K>::fromPow2(int p) {
    fractionalBignum<K> f;
    if (p > 0) {
        return f;
    }
    size_t block = (-(p+1) / 64);
    size_t offset = (-(p+1) % 64);
    if (block < K) {
        f.v[block] = (HIGHEST_ONE >> offset);
    } 
    return f;
}

template <size_t K>
std::string fractionalBignum<K>::base10_str(){
    const double log2_10 = 3.32192809489;
    const size_t precision = floor((K * 64) / log2_10);
    std::string padding(precision,'0');
    std::string s(precision, '0');
    for(int i = 0; i < K; i++) {
        u_int64_t mask = 1UL << 63;
        for(int j = 1; j <= 64; j++) {
            if (this->v[i] & mask) {
                std::stringstream ss;
                ss.precision(precision);
                ss << std::scientific << pow(2, -((64 * i) + j));
                std::string num;
                std::string exponent_str;
                getline(ss, num, 'e');
                getline(ss, exponent_str, 'e');
                int exponent = std::stoi(exponent_str);

                // remove decimal point from string
                std::string integer;
                std::string fraction;
                std::stringstream split(num);
                getline(split,integer,'.');
                getline(split,fraction,'.');

                // padd with leading zeros
                num = padding.substr(0,(-exponent) - 1) + integer + fraction;
                num = num.substr(0, precision);

                s = util::base10add(s.c_str(), num.c_str(), precision);

            }

            mask >>= 1;
        }
    }
    // remove trailing zeros
    int end = s.size() - 1;
    while(s[end] == '0') {
        end--;
    }
    s =  "0." + s.substr(0,end+1);
    return s;
}

template <size_t K>
std::string fractionalBignum<K>::base16_str(char sep) const{
    std::stringstream ss;
    for(auto i = 0; i < K; i++) {
       ss << std::setfill('0') << std::setw(16) << std::hex << this->v[i];
       if(sep) {
           ss << sep;
       }
    }
    return ss.str();
}
template <size_t K>
std::string fractionalBignum<K>::base64_str(){}
template <size_t K>
std::string fractionalBignum<K>::base2_str() {
    std::stringstream ss;
    for (size_t i = 0; i < K; i++) {
        u_int64_t mask = (1L << 63);
        for(auto b = 0; b < 64; b++) {
            ss << ((this->v[i] & mask) && 1);
            mask >>= 1;
        }
    }
    return ss.str();
}

template <size_t K>
fractionalBignum<K> fractionalBignum<K>::twos_complement() const {
    fractionalBignum<K> f = *this;
    f.invert();
    int carry = __builtin_add_overflow(f.v[K-1], 1, &f.v[K-1]);
    for(int i = K-2; i >= 0; i--) {
        carry = __builtin_add_overflow(f.v[i], carry, &f.v[i]);
    }
    return f;
}

template <size_t K>
void fractionalBignum<K>::invert() {
    static const u_int64_t ALL_ONES = 0xFFFFFFFFFFFFFFFF;
    for(auto i = 0; i < K; i++) {
        this->v[i] ^= ALL_ONES;
    }
}

template <size_t K>
void fractionalBignum<K>::setZero() {
    bzero(this->v, K * sizeof(u_int64_t));
}

template <size_t K>
bool fractionalBignum<K>::isOne() {
    return std::all_of(this->v, this->v + K, [](u_int64_t e) {
            return e == 0xFFFFFFFFFFFFFFFF;
        }
    );
}

template <size_t K>
bool fractionalBignum<K>::isZero() {
    return std::all_of(this->v, this->v + K, [](u_int64_t e) {
            return e == 0;
        }
    );
}


template <size_t K>
std::string fractionalBignum<K>::asBase(util::bases b) {
    
    if(this->isNan) {
        return NAN_STR;
    }
    if (this->isInf) {
        return INF_STR;
    }
    switch (b)
    {
    case util::TWO:
        return this->base2_str();
        break;
    case util::TEN:
        return this->base10_str();
        break;
    case util::HEX:
        return this->base16_str();
        break;
    case util::b64:
        return this->base64_str();
        break;
    }
}

template <size_t K>
fractionalBignum<K>::~fractionalBignum() {

}


template <size_t K>
std::ostream& operator<<(std::ostream& os, const fractionalBignum<K>& f) {
    os << f.base16_str(' ');
    return os;
}

template <size_t K>
fractionalBignum<K> operator+(const fractionalBignum<K>& a, const fractionalBignum<K>& b) {
    fractionalBignum<K> c;
    auto carry = 0;
    for(int i = (K-1); i >= 0; i--) {
        auto carry1 = __builtin_add_overflow(a.v[i], b.v[i], &c.v[i]);
        auto carry2 = __builtin_add_overflow(c.v[i], carry, &c.v[i]);
        carry = carry1 or carry2;
    }
    return c;
}

template <size_t K>
fractionalBignum<K> operator-(const fractionalBignum<K>& a, const fractionalBignum<K>& b) {
    auto c = a + b.twos_complement();
    return c;
}

template <size_t K>
fractionalBignum<K>& fractionalBignum<K>::operator+=(const fractionalBignum<K>& b) {
    auto carry = 0;
    for(int i = (K-1); i >= 0; i--) {
        auto carry1 = __builtin_add_overflow(this->v[i], b.v[i], &this->v[i]);
        auto carry2 = __builtin_add_overflow(this->v[i], carry, &this->v[i]);
        carry = carry1 or carry2;
    }
    return *this;
}

#ifdef __x86_64__
template <size_t K>
fractionalBignum<K> operator*(const fractionalBignum<K>& a, const fractionalBignum<K>& b) {
    fractionalBignum<K> c;
    for(int ai = K-1; ai >= 0; ai--) {
        fractionalBignum<K> carry;
        fractionalBignum<K> term;

        for(int bi = K-1; bi >= 0; bi--) {
            if((ai + bi) >= K) {
                continue;
            }
            __asm__(
                "mulq %%rbx"
                : "=d" (carry.v[ai + bi]), "=a" (term.v[ai + bi + 1])
                : "a" (a.v[ai]), "b" (b.v[bi])
            );
        }
        c += term;
        c += carry;
    }    
    return c;
}
#else
template <size_t K>
fractionalBignum<K> operator*(const fractionalBignum<K>& a, const fractionalBignum<K>& b) {
    fractionalBignum<K> c;
    for(int ai = K-1; ai >= 0; ai--) {
        fractionalBignum<K> carry;
        fractionalBignum<K> term;

        for(int bi = K-1; bi >= 0; bi--) {
            if((ai + bi) >= K) {
                continue;
            }
            __int128 p = (__int128) a.v[ai] * b.v[bi];
            term.v[ai + bi + 1] = (u_int64_t) p;
            carry.v[ai + bi] = p >> 64;
        }
        c += term;
        c += carry;
    }
    return c;
}
#endif

template <size_t K>
fractionalBignum<K> operator<<(const fractionalBignum<K>& a, const size_t s) {
    auto big_shift = s / 64;
    fractionalBignum<K> f;
    for(auto i = 0; i < K - big_shift; i++) {
        f.v[i] = a.v[i + big_shift];
    }   
    auto small_shift = s % 64;
    u_int64_t mask = 0xFFFFFFFFFFFFFFFF >> (64 - small_shift);
    mask <<= (64 - small_shift);

    for(int i = 0; i < K - 1; i++) {
        u_int64_t bits = f.v[i + 1] & mask;
        bits >>= (64 - small_shift);
        f.v[i] <<= small_shift;
        f.v[i] += bits;
    }
    f.v[K-1] <<= small_shift;
    return f;
}

template <size_t K>
fractionalBignum<K> operator>>(const fractionalBignum<K>& a, const size_t s) {
    auto big_shift = s / 64;
    fractionalBignum<K> f;
    for(auto i = big_shift; i < K; i++) {
        f.v[i + big_shift] = a.v[i];
    }   
    auto small_shift = s % 64;
    u_int64_t mask = 0xFFFFFFFFFFFFFFFF << (64 - small_shift);
    mask >>= (64 - small_shift);
    
    for(int i = K - 1; i > 0; i--) {
        u_int64_t bits = f.v[i - 1] & mask;
        bits <<= (64 - small_shift);
        f.v[i] >>= small_shift;
        f.v[i] += bits;
    }
    f.v[0] >>= small_shift;
    return f;
}


template <size_t K>
fractionalBignum<K> div_gs(u_int64_t numerator, u_int64_t denominator) {
    fractionalBignum<K> f;

    if(denominator == 0) {
        f.isNan = true;
        return f;
    }

    if(numerator > denominator) {
        numerator %= denominator;
    }

    if(numerator == 0) {
        return f;
    }
    if (denominator == 1) {
        return f;
    }

    // scale n and d to be [0.5,1]
    int lzcnt = __builtin_clzl(denominator);
    fractionalBignum<K> d_prime(denominator << lzcnt);
    fractionalBignum<K> n_prime(numerator << lzcnt);

    f = d_prime.twos_complement();

    while(not d_prime.isOne()) {
        // n_prime * f
        n_prime = (n_prime * f) + n_prime;
        // d_prime * f
        d_prime = (d_prime * f) + d_prime;

        f = d_prime.twos_complement();
    }

    return n_prime;
}

template <size_t K> 
fractionalBignum<K> fb_div(u_int64_t numerator, u_int64_t denominator) {
    fractionalBignum<K> f;

#ifndef __x86_64__
    // use 32 bit for our base since we don't have access to the upper 64 bit register on non-x86
    u_int32_t f_32[K*2];
#endif

    if(denominator == 0) {
        f.isNan = true;
        return f;
    }

    if(numerator > denominator) {
        numerator %= denominator;
    }

    if(numerator == 0) {
        return f;
    }
    if (denominator == 1) {
        return f;
    }

    u_int64_t r = numerator;

#ifdef __x86_64__
    for(size_t i = 0; i < K; i++) {
        __asm__(
            "divq %%rbx"
            : "=a" (f.v[i]), "=d" (r)
            : "d" (r), "b" (denominator), "a" (0)  
        );
    }
#else
    for(size_t i = 0; i < K*2; i++) {
        f_32[i] = (r << 32) / denominator;
        r = (r << 32) % denominator;
        if (i & 1) {
            f.v[i / 2] = ((u_int64_t) f_32[i - 1] << 32) + (u_int64_t) f_32[i];
        }
    }
#endif

    return f;
}