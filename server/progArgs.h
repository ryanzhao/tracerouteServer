//----------------------------------------------------------------------------
// Define a class to parse input argument of traceServer and check the validity
// of these parameters, if no parameters specified, use defaults.
//========================================
// Weiran Zhao, Computer Science Dept
// Indiana University, Bloomington
//========================================
// Started: a week or so before Tue,Sep 17th 2013 05:46:20 PM EDT
// Last Modified: Thu,Oct 10th 2013 03:01:04 PM EDT
//----------------------------------------------------------------------------
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
    public:
        static const int32_t listenQ = 10; // listen Queue for listen socket
        static const int32_t timeOut = 20; // automatic timeout if no input from user
        //-------------
        // class member
        //-------------
        int32_t port;       // port number
        rate* reqPerSec;    // number of request per xx seconds
        int32_t maxUsers;   // max concurrent users
        bool strictDest;    // strict destination
        //--------------
        // class methods
        //--------------
        progArgs(int argc, char** argv);
        ~progArgs();
        // print usage of how to invoke this program
        void usage();   
        // check validity of arguments, port, maxUsers, etc
        void checkArgs();   
        // print error message for invalid arguments
        void invalidInput(const char* err_msg);    
        // over load operator <<
        friend ostream& operator << (ostream& output, const progArgs& pa);
};
#endif
