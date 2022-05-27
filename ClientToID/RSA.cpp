#include "RSA.h"
using namespace std;

long long RSA::gcd(long long int a, long long int h)     {
    long long  temp;
    while(true)
    {
        temp = a%h;
        if(temp==0)
            return h;
        a = h;
        h = temp;
    }
}

RSA::RSA(double  p, double q) {
        double  count=0;
        _modulus = (double) (p * q);
        double euler  =  ((p-1)*(q-1));
        double openExponent = 2;
        while(openExponent<euler){
            count = gcd(openExponent,euler);
            if(count==1)
                break;
            else
                openExponent++;
        }

        double k=2;
        _closedExponent = (double)(1 + (k*euler))/openExponent;
        _openExponent = (double) openExponent;
}

string RSA::createCyphertext(std::string &plainText) {
    vector<int> resultVec{};

    for (int i = 0 ; i< plainText.size();i++)
    {
        double temp = pow(plainText[i],_externalExponent);

        resultVec.push_back(fmod(temp , _externalModulus));

    }
    string str;
    for (auto item : resultVec)
    {
        str+=to_string(item) + ";";

    }
    return str;
}

std::string RSA::createPlaintext(string &plainText) {
    vector<int> resultVec{};
    vector<int> plainTextVec{};
    string temp;
    for (auto str : plainText)
    {
        if (str !=';') {
            temp += str;
        }
        else
        {
            plainTextVec.push_back(atoi(temp.c_str()));
            temp="";
        }

    }
    for (int dig : plainTextVec)
    {
        BigUnsigned tempd = dig;
        BigUnsigned closedExponent = (unsigned long) _closedExponent;
        BigUnsigned modulus = (unsigned long) _modulus;
        resultVec.push_back(modexp(tempd,closedExponent,modulus).toInt());

    }
    string str;
    for (auto item : resultVec)
    {
        str+=item ;

    }
    return  str;
}

