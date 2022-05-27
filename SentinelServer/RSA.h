//
// Created by rlego on 30.01.2022.
//
#include <cmath>
#include <string>
#include <vector>
#include "bigint/BigIntegerLibrary.hh"
#ifndef RSA_RSA_H
#define RSA_RSA_H
class RSA{
private:
    static long long gcd(long long a, long long  h);
    double _openExponent;
    double _modulus;
    double _closedExponent;
    double _externalExponent;
    double _externalModulus;
    //BigInteger Pow(BigInteger &value, BigInteger exponent);

public:
    RSA(double p,double q);
    std::string createCyphertext(std::string& plainText);
    std::string createPlaintext(std::string& cypherText);
};
#endif //RSA_RSA_H
