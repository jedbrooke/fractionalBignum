#include <string>
#include <iostream>
#include <iomanip>
#include <string>
#include <memory>

template <size_t K>
class fractionalBignum
{
private:
    u_int64_t v[K] = {0};
public:
    fractionalBignum();
    fractionalBignum(u_int64_t v[K]);
    fractionalBignum(u_int64_t w[2], int offset);
    fractionalBignum(std::string s, int base=10);
    ~fractionalBignum();


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
fractionalBignum<K>::~fractionalBignum() {
}


template <size_t K>
std::ostream& operator<<(std::ostream& os, const fractionalBignum<K>& f) {
    for(auto i = 0; i < K; i++) {
       os << std::setfill('0') << std::setw(16) << std::hex << f.v[i] << " ";
    }
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