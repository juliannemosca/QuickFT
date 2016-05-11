/*
 * server.h
 *
 * $Id: $
 * $HeadURL: $
 * $LastChangedRevision: $
 * $LastChangedDate: $
 * $LastChangedBy: $
 *
 */

#ifndef SERVER_H
#define SERVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mutex.h"
#include "socket.h"
#include "thread.h"

// Declares the user_data structure for threads
typedef struct _user_data_t {
  int is_running;
  int keep_going;
  MUTEX_T* mutex;
} USER_DATA_T;

// Definition for server data structure type
typedef struct _server_t {

  // Connection information structure
  SOCKET_T * connection;
  int initialized;
  
  // Information on the thread's context
  // for the node's listen process
  thread_t * listen_thread;
  USER_DATA_T udata;

} SERVER_T;

/**
 * Initializes server for sending and receiving messages
 *
 * @param port                          port to listen on
 * @param max_connections               max. connections supoerted by the server
 * @param timeout                       timeout for messages, can be 0 for default
 * @param log_writer                    a pointer to a callback logging function
 *                                      that receives two parameters:
 *                                      
 *                                      function: name of the function in which
 *                                                logger_write is being called.
 *                                      message:  body of the message.
 *
 * @return                              pointer to a SERVER_T type
 */
SERVER_T * server_initialize( int port, int max_connections, int timeout, void (*log_writer) (const char* function, char* message) );

/**
 * Ends the connections and finalizes the server
 *
 * @param server                        server data structure
 * @return                              TRUE or FALSE
 */
int server_finalize ( SERVER_T ** server );

/**
 * Starts the thread for the server's listen loop
 *
 * @param server                        server data structure
 *
 */
void server_listen_begin ( SERVER_T * server_context );

/**
 * Finalizes the thread's listen loop
 *
 * @param server                        server data structure
 */
void server_listen_finalize ( SERVER_T * server );

#endif // SERVER_H