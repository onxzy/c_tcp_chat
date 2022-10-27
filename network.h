#define CHECK_ERR(msg, val1, val2)   if (val1==val2) { perror(msg); exit(-1);}

#define SERVER_PORT 5000
#define MAXOCTETS   80

#define CLOSE_MSG       "0"
