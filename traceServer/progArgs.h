#ifndef PROG_ARGS_H_
#define PROG_ARGS_H_
#include<stdint.h>
#include<iostream>
using namespace std;

class rate {
    private:
        int32_t req;       // number of requests
        int32_t sec;       // number of seconds;
    public:
        rate(int32_t r, int32_t s);
        int32_t getReq() { return req;}
        int32_t getSec() { return sec;}
        void setReq(int32_t r) { req = r;}
        void setSec(int32_t s) { sec = s;}
        friend ostream & operator << (ostream & output, const rate & r);
};
class progArgs {
    private:
        int16_t port;
        rate* reqPerSec;
        int32_t maxUsers;
        bool strictDest;
    public:
        progArgs(int argc, char** argv);
        ~progArgs();
        void usage();   // print usage of how to invoke this program
        void checkArgs();   // check validity of arguments, port, maxUsers, etc
        void invalidInput();    // print error message for invalid arguments
};
#endif