#include <string>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <memory.h>
#include <utility.hpp>
#include <sstream>

template <size_t K>
class fractionalBignum
{
private:
    u_int64_t v[K] = {0};
    bool isNan = false;
    bool isInf = false;
public:
    fractionalBignum();
    fractionalBignum(u_int64_t v[K]);
    fractionalBignum(u_int64_t w[2], int offset);
    fractionalBignum(std::string s, int base=10);
    fractionalBignum(double d);
    ~fractionalBignum();

    void insert_octoword(u_int64_t w[2], int offset);

    std::string asBase(util::bases b);

    std::string decimal_str() {return asBase(util::TEN);}
    std::string base10_str();

    std::string binary_str(){return asBase(util::TWO);}
    std::string base2_str();

    std::string hex_str() {return asBase(util::HEX);}
    std::string base16_str();

    std::string base64_str();


    template <size_t L>
    friend std::ostream& operator<<(std::ostream& os, const fractionalBignum<L>& f);

    template <size_t L>
    friend fractionalBignum<L> operator+(const fractionalBignum<L>& a, const fractionalBignum<L>& b);

    fractionalBignum<K>& operator+=(const fractionalBignum<K>& b);

    template <size_t L>
    friend fractionalBignum<L> operator*(const fractionalBignum<L>& a, const fractionalBignum<L>& b);

};

template <size_t K>
fractionalBignum<K>::fractionalBignum() {}


template <size_t K>
fractionalBignum<K>::fractionalBignum(u_int64_t v[K]) {
    std::copy(v, v + K, std::begin(this->v));
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
        if (offset < K) {
            this->v[offset] = w[0];
        }
        if ((offset + 1) < K) {
            this->v[offset+1] = w[1];
        }
    }
}

template <size_t K>
fractionalBignum<K>::fractionalBignum(double d) {
    this->isInf = isinfl(d);
    this->isNan = isnanl(d);
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
    s = "0." + s;
    return s;
}

template <size_t K>
std::string fractionalBignum<K>::base16_str(){}
template <size_t K>
std::string fractionalBignum<K>::base64_str(){}
template <size_t K>
std::string fractionalBignum<K>::base2_str(){}


template <size_t K>
std::string fractionalBignum<K>::asBase(util::bases b) {
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
    std::ios_base::fmtflags flags(os.flags());
    for(auto i = 0; i < K; i++) {
       os << std::setfill('0') << std::setw(16) << std::hex << f.v[i] << " ";
    }
    os.flags(flags);
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
fractionalBignum<K>& fractionalBignum<K>::operator+=(const fractionalBignum<K>& b) {
    auto carry = 0;
    for(int i = (K-1); i >= 0; i--) {
        auto carry1 = __builtin_add_overflow(this->v[i], b.v[i], &this->v[i]);
        auto carry2 = __builtin_add_overflow(this->v[i], carry, &this->v[i]);
        carry = carry1 or carry2;
    }
    return *this;
}

template <size_t K>
fractionalBignum<K> operator*(const fractionalBignum<K>& a, const fractionalBignum<K>& b) {
    fractionalBignum<K> c;
    u_int64_t v[2];
    for(int i = (K-1); i >= 0; i--) {
        for(int j = (K-i); j >= 0; j--) {
            __asm__(
                "mulq %%rbx"
                : "=d" (v[0]), "=a" (v[1])
                : "a" (a.v[i]), "b" (b.v[j])
            );
            fractionalBignum<K> t(v,i+j);
            c += t;
        }
    }
    return c;
}