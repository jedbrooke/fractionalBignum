# fractionalBignum
bignum library for representing numbers between 0 and 1 with higher than 64 bit depth

in a normal 64 bit int, the MSB is 2<sup>63</sup>, and the LSB is 2<sup>0</sup>.

If instead we want to represent fractions in the range of (0,1), we can take a 64 bit int and let the MSB represent 2<sup>-1</sup>
and the LSB be 2<sup>-64</sup>.

Then, we can chain together mutliple 64bit ints together to extend out LSB to go as far as we want. For example with 4 64bit ints the LSB would represent 2<sup>-256</sup>.

example usage:

see [examples/basic_usage.cpp](examples/basic_usage.cpp) for full code.

addition example: 2<sup>-1</sup> + 2<sup>-65</sup>

```
    0.00000000000000000002710505431213761085
+   0.50000000000000000000000000000000000000
---------------------------------------------
as fractionalBignum
0.50000000000000000002710505431213761085
as regular double
0.5

```

TODO:
 - [ ] basic I/O
    - [x] printing in hex
    - [x] printing in base 10<sup>*</sup>
    - [x] from double constructor
    - [ ] base 2/10/16/64 string constructor
 - [ ] basic arithmetic
    - [x] addition operator
    - [ ] subtraction operator
    - [x] multiplication operator
    - [ ] comparision operators
 - [ ] additional arithmetic
    - [ ] div(int64, int64) -> fractionalBignum
    - [ ] karatsuba/fft based multiplication for very large bit counts
    - [ ] `avx2` vecorization for speedup
    - [ ] operations with fractionalBignums of mixed sizes
 - [ ] misc
    - [ ] setup proper library packaging
    - [ ] speed comparison to GNU GMP

<sup>*</sup> base 10 conversion relies on the cmath `pow` function which is essentially just a `double`, so it breaks around 2^1024.