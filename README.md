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

speed comparison[^1]:

see [examples/speed_comparison.cpp](examples/speed_comparison.cpp) for full code.

comparison to [gmp](https://gmplib.org/)

```
fractionalBignum:       0.00000000000000000005421010862427522170331137592055280...
gmp mpf_t:              0.00000000000000000005421010862427522170331137592055280...
running 1073741824 iterations
running fractionalBignum
10.4733s
running gmp mpf_t test
22.4992s
check results:
fractionalBignum:       0.00000000005820766091346740722971794362088404722181797...
gmp mpf_t:              0.00000000005820766091346740722971794362088404722181797...
```
the results do not match exactly, but are accurate to 85 decimal places


TODO:
 - [ ] basic I/O
    - [x] printing in hex
    - [x] printing in base 10[^2]
    - [x] from double constructor
    - [ ] base 2/10/16/64 string constructor
 - [ ] basic arithmetic
    - [x] addition operator
    - [x] subtraction operator
    - [x] multiplication operator
    - [ ] comparision operators
    - [x] shift operator
 - [ ] additional arithmetic
    - [x] div(int64, int64) -> fractionalBignum
    - [ ] karatsuba/fft based multiplication for very large bit counts
    - [ ] `avx2` vecorization for speedup
    - [ ] operations with fractionalBignums of mixed sizes
 - [ ] misc
    - [x] setup proper library packaging
    - [x] speed comparison to GNU GMP
    - [ ] tests

[^1]: test system used: intel i5 4950 @3.5ghz 16GB RAM

[^2]: base 10 conversion relies on the cmath `pow` function which is essentially just a `double`, so it breaks around 2^1024.
