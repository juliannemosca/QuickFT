/*
 * server.c
 *
 * $Id: $
 * $HeadURL: $
 * $LastChangedRevision: $
 * $LastChangedDate: $
 * $LastChangedBy: $
 *
 */

#include <unistd.h>
#define Sleep(ms) usleep(ms*1000)

#include "macros.h"

#include "server.h"
#include "message.h"
#include "process.h"
#include "quickft.h"

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
SERVER_T * server_initialize( int port, int max_connections, int timeout, void (*log_writer) (const char* function, char* message) ) {
  
  SERVER_T * new_server;
  
  gl_timeout = DEFAULT_TIMEOUT;

  if (timeout != 0) {
    gl_timeout = timeout;
  }
  printf("DEBUG: gl_timeout: %lu\n", gl_timeout);
  
  // Stores the log writer function
  gl_log_writer = log_writer;
  
  // Initializes the library's socket functionalities
  if ( ! SOCKET_INIT() ) {
    return NULL;
  }

  new_server = malloc( sizeof(SERVER_T) );
  memset(new_server, 0x00, sizeof(SERVER_T));

  // Creates a connection
  new_server->connection = SOCKET_NEW_SRVR(NULL, port, max_connections);
  if ( new_server->connection != NULL ) {

    // Sets initialization variable
    new_server->initialized = TRUE;
    
    // Begins thread for listening
    server_listen_begin( new_server );

  }
  
  return new_server;
}

/**
 * Ends the connections and finalizes the server
 *
 * @param server                        server data structure
 * @return                              TRUE or FALSE
 */
int server_finalize ( SERVER_T ** server ) {

  if ( (*server) != NULL ) {

    // Sets initialized variable to false
    (*server)->initialized = FALSE;

    // Finalizes listen thread
    server_listen_finalize( (*server) );
    
    // Ends connection and closes socket
    SOCKET_CLOSE( &(*server)->connection );
    
    // Frees memory previously allocated for server structure
    free(*server);
    (*server) = NULL;

    // Finalizes library's socket functionalities
    SOCKET_DEINIT();
    
    return TRUE;
  }

  return FALSE;
}

/**
 * Performs the server listen loop
 *
 */
void * server_listen_function ( void * server_l ) {
  
  SOCKET_T * accepted_socket;
  SERVER_T * server = (SERVER_T * ) server_l;

  while ( server->udata.keep_going == TRUE ) {

    // Locks mutex on the thread
    MUTEX_LOCK(server->udata.mutex);

    if ( TRUE == SOCKET_ACCEPT(server->connection, &accepted_socket) ) 
    {
      process_incoming_request(&accepted_socket);
    }
    
    // Removes lock from mutex
    MUTEX_UNLOCK(server->udata.mutex);

    Sleep(100);

  }
  
  // Sets the thread state
  server->udata.is_running=FALSE;
  LOGGER(__FUNCTION__, "stops listen");
  return 0;
}

/**
 * Starts the thread for the server's listen loop
 *
 * @param server                        server data structure
 *
 */
void server_listen_begin (  SERVER_T * server  ) {

  // Initializes processes structures
  process_init();

  // Creates a thread for the server_listen
  server->udata.keep_going = TRUE;
  server->udata.is_running = TRUE;
  
  // Allocates memory for the mutex
  server->udata.mutex = (MUTEX_T*)malloc(sizeof(MUTEX_T));
  memset(server->udata.mutex, 0x00, sizeof(MUTEX_T));
  
  // Creates the mutex for the new thread
  MUTEX_CREATE (&server->udata.mutex);

  // Creates the thread
  server->listen_thread = (thread_t*) malloc(sizeof(thread_t));
  THREAD_CREATE(&server->listen_thread, &server_listen_function, (void *)server);

}

/**
 * Finalizes the thread's listen loop
 *
 * @param server                        server data structure
 */
void server_listen_finalize (  SERVER_T * server ) {

  // Sets flag to stop the thread
  server->udata.keep_going = FALSE;
  
  //  Waits for the thread to end and destroys it
  THREAD_JOIN(server->listen_thread, TRUE);
  free(server->listen_thread);

  // Destroys the thread's mutex
  MUTEX_DESTROY(&server->udata.mutex);
  free(server->udata.mutex);

  // Finalizes the processes structures
  process_deinit();
}
