//----------------------------------------------------------------------------
//========================================
// Weiran Zhao, Computer Science Dept
// Indiana University, Bloomington
//========================================
// Started: Thu,Oct 10th 2013 11:15:36 PM EDT
// Last Modified: Thu,Oct 10th 2013 11:45:28 PM EDT
//----------------------------------------------------------------------------
#include"interaction.h"
#include<cstring>
#include<arpa/inet.h>
#include<unistd.h>
#include"misc.h"
interaction::interaction(int fd, struct sockaddr_in *cliaddr) {
    connfd = fd;
    clientAddr = cliaddr;
    memset(inBuff,0, BUFSIZE);
    memset(ipHostFname,0, BUFSIZE);
}
void interaction::takeAction() {
    int status;
    switch((status=parseInput())) {
        case QUIT:
            quit();
            break;
        case HELP:
            help();
            break;
        case ME:
            traceMe();
            break;
        case FNAME:
            dprintf(connfd,"traceroute file with name: %s\n",ipHostFname);
            break;
        case IPHOST:
            traceIpHost(ipHostFname);
            break;
        case INVLD:
            invalid();
            break;
        default:
            break;
    }
}
int interaction::parseInput() {
    char* pch;
    const char* delim=" ";
    // delete trailing \r\n added from telnet
    if(strlen(inBuff)>=2) {
        memset(inBuff+(strlen(inBuff)-2), 0, 2);
    }
    //--------------
    // start parsing
    //--------------
    pch=strtok(inBuff,delim);
    if(pch!=NULL) {
        // input is 'quit'
        if(strcmp(pch,"quit")==0) {
            // make sure there's not any words after 'quit'
            pch=strtok(NULL,delim);
            if(pch!=NULL) {
                return INVLD;
            } else {
                return QUIT;
            }
        } // input is 'quit'
        // input is 'help'
        if(strcmp(pch,"help")==0) {
            // make sure there's not any words after 'help'
            pch=strtok(NULL,delim);
            if(pch!=NULL) {
                return INVLD;
            } else {
                return HELP;
            }
        } // input is 'help'
        // input is 'traceroute'
        if(strcmp(pch,"traceroute")==0) {
            // need to next word
            pch=strtok(NULL,delim);
            // if nothing followed 'traceroute'
            if(pch==NULL) {
                return INVLD;
            }
            // if is 'traceroute me'
            if(strcmp(pch,"me")==0) {
                pch=strtok(NULL,delim);
                // make sure nothing followed 'me'
                if(pch!=NULL) {
                    return INVLD;
                } else {
                    return ME;
                }
            } else { // 'traceroute' followed by 'filename' or 'ip/host'
                memset(ipHostFname,0,BUFSIZE);
                strcpy(ipHostFname, pch);
                // if that is a filename
                if(access(ipHostFname,R_OK)==0) {
                    return FNAME;
                } else { // otherwise, treat as ip/host name
                    return IPHOST;
                }
                //------------------------------------------------
                // the rest following 'traceroute' will be checked
                // by traceroute utility on server when called
                //------------------------------------------------
            } // traceroute 'me' or not
        } // input is 'traceroute'
        return INVLD;
    }
    return INVLD;
}

void interaction::quit() {
    dprintf(connfd, "quit request from client received, quitting...\n");
    Close(connfd);
    exit(0);
}

void interaction::help() {
    // I put necessary help info in a text file to shorten code
    FILE* fhelp = fopen("text/help.txt","r");
    if(fhelp==NULL) {
        cout<<"file: ./text/help.txt open failed"<<endl;
    }
    // read till the end of file
    while(Fgets(ipHostFname, BUFSIZE, fhelp)==1) {
        dprintf(connfd, "%s",ipHostFname);
    }
}

void interaction::traceIpHost(const char* ipHost) {
    pid_t child_pid;
    if((child_pid=fork())<0) {
        sys_err("call to fork() failed");
    } else if(child_pid==0) { // in child
        // Close stuff from parents
        // redirect stderr and stdout to connfd
        Dup2(connfd, STDOUT_FILENO);
        Dup2(connfd, STDERR_FILENO);
        // call execlp 'NULL' must be provided
        execlp("traceroute","traceroute",ipHost, NULL);
    } else {// in parent
        if(VOMIT) {
            cout<<"client interaction process: "<<getpid()
                <<" forked a child with pid: "<<child_pid<<endl;
        }
        // block until child finish
        waitpid(child_pid, NULL, 0);
    }
}

void interaction:: invalid() {
    dprintf(connfd,"INVALID INPUT\n");
    help();
}

void interaction:: traceMe() {
    inet_ntop(AF_INET, &(clientAddr->sin_addr), ipHostFname,BUFSIZE);
    dprintf(connfd,"%s\n",ipHostFname);
    traceIpHost(ipHostFname);
}