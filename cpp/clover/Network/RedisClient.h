// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <signal.h>

// #include <hiredis.h>
// #include <async.h>
// #include <adapters/libevent.h>



// class RedisClient
// {
// public:
//     RedisClient();
//     ~RedisClient();

//     void OnConnect();

//     void OnDisconnected();

//     void OnResult();


// private:

//     std::string m_host;
//     int         m_port;
//     redisAsyncContext * m_asynRedis;
//     struct ev_base* m_base;
// };



// void getCallback(redisAsyncContext *c, void *r, void *privdata) {
//     redisReply *reply = r;
//     if (reply == NULL) return;
//     printf("argv[%s]: %s\n", (char*)privdata, reply->str);

//     /* Disconnect after receiving the reply to GET */
//     redisAsyncDisconnect(c);
// }

// void connectCallback(const redisAsyncContext *c, int status) {
//     if (status != REDIS_OK) {
//         printf("Error: %s\n", c->errstr);
//         return;
//     }
//     printf("Connected...\n");
// }

// void disconnectCallback(const redisAsyncContext *c, int status) {
//     if (status != REDIS_OK) {
//         printf("Error: %s\n", c->errstr);
//         return;
//     }
//     printf("Disconnected...\n");
// }

// int main (int argc, char **argv) {
//     signal(SIGPIPE, SIG_IGN);
//     struct event_base *base = event_base_new();

//     redisAsyncContext *c = redisAsyncConnect("127.0.0.1", 6379);
//     if (c->err) {
//         /* Let *c leak for now... */
//         printf("Error: %s\n", c->errstr);
//         return 1;
//     }

//     redisLibeventAttach(c,base);
//     redisAsyncSetConnectCallback(c,connectCallback);
//     redisAsyncSetDisconnectCallback(c,disconnectCallback);
//     redisAsyncCommand(c, NULL, NULL, "SET key %b", argv[argc-1], strlen(argv[argc-1]));
//     redisAsyncCommand(c, getCallback, (char*)"end-1", "GET key");
//     event_base_dispatch(base);
//     return 0;
// }




// void dequeuedItem(redisAsyncContext *c, void *r, void *privdata) {
 
//     int i;
//     redisReply *reply = r;
//     if (reply == NULL) return;
 
//     switch( reply->type ) {
//       case REDIS_REPLY_ARRAY:
//         for ( i=0; i<reply->elements; ++i ) {
//           printf("Expired: %s\n", reply->element[i]->str );
//           redisAsyncCommand( c, NULL, NULL, "DEL %s", reply->element[i]->str );
//         }
//         if ( i>0 )
//           redisAsyncCommand( c, dequeuedItem, NULL, "EVALSHA %s 1 to_be_expired %ld", singleton.luasha1, time(NULL) );
//         break;
 
//       case REDIS_REPLY_ERROR:
//       case REDIS_REPLY_STATUS:
//         printf("ERror: %s\n",reply->str );
//         break;
 
//       case REDIS_REPLY_NIL:
//         break;
//       default:
//         printf("Error\n");
//         break;
//     }
// }
