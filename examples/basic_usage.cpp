#include "fractionalBignum.hpp"
#include <iostream>
#include <cmath>
#include <limits>

int main(/* int argc, char const *argv[] */)
{
    const size_t k = 4;
    // intialize manually
    u_int64_t v[k] = {1UL << 63, 0, 0, 0};
    fractionalBignum<k> f1(v);
    // initialize from double
    fractionalBignum<k> f2(0.25);
    // initialize from power of 2
    auto pow2 = fractionalBignum<2>::fromPow2(-65);
    std::cout << "from power of 2" << std::endl;
    std::cout << pow2 << std::endl;
    std::cout << pow2.base10_str() << std::endl;


    // print raw hex
    std::cout << "f1 as hex:\t" << f1 << std::endl;
    std::cout << "f2 as hex:\t" << f2 << std::endl;
    // print as decimal
    std::cout << "f1 as decimal:\t" << f1.decimal_str() << std::endl;
    std::cout << "f2 as decimal:\t" << f2.decimal_str() << std::endl;

    std::cout << std::endl;

    // addition
    auto a = f1+f2;
    std::cout << "f1 + f2:" << std::endl;
    std::cout << "as hex:\t\t" << a << std::endl;
    std::cout << "as decimal:\t" << a.decimal_str() << std::endl;

    std::cout << std::endl;

    // multiplication
    auto p = f1 * f2;
    std::cout << "f1 * f2:" << std::endl;
    std::cout << "as hex:\t\t" << p << std::endl;
    std::cout << "as decimal:\t" << p.decimal_str() << std::endl;

    std::cout << std::endl;

    // from division of integers
    auto q = fb_div<k>(1,3);
    std::cout << "1/3:" << std::endl;
    std::cout << "as hex:\t\t" << q << std::endl;
    std::cout << "as decimal:\t" << q.decimal_str() << std::endl;

    std::cout << std::endl;

    std::cout << "let me show that fractionalBignum has more precision than a double" << std::endl;

    // more precision than double
    double d1 = pow(2,-65);
    double d2 = 0.5;
    fractionalBignum<k> fb1(d1);
    fractionalBignum<k> fb2(d2);

    std::cout << "\t" << fb1.decimal_str() << std::endl;
    std::cout << "+\t" << fb2.decimal_str() << std::endl;
    std::cout << "---------------------------------------------" << std::endl;
    std::cout << "as fractionalBignum" << std::endl;
    std::cout << (fb1 + fb2).decimal_str() << std::endl;
    std::cout << "as regular double" << std::endl;

    std::cout.precision(std::numeric_limits<double>::max_digits10);
    std::cout << d1 + d2 << std::endl;






    return 0;
}
