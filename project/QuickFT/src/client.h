/*
 * client.h
 *
 * $Id: $
 * $HeadURL: $
 * $LastChangedRevision: $
 * $LastChangedDate: $
 * $LastChangedBy: $
 *
 */

#ifndef _CLIENT_H
#define _CLIENT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "socket.h"

// Timeout for ACK Messages
unsigned long client_timeout_ack;

// Data structure definition for client nodes
typedef struct _quickft_client_t {

  // Connection data structure
  SOCKET_T * connection;
  
} quickft_client_t;

// Macros
#define QUICKFT_FILE_RECEIVE    client_file_receive
#define QUICKFT_FILE_SEND       client_file_send
#define QUICKFT_FILE_DELETE     client_file_delete

/**
 * Performs a 'File Receive' operation for the client
 *
 * @param remote_filename     filename in location of origin
 * @param local_filename      filename in destination location
 * @param addr                server IP address
 * @param port                server Port, can be 0 for default
 * @param timeout             timeout for messages, can be 0 for default
 * @param timeout_ack         timeout for ack messages, can be 0 for default
 * @param log_writer          a pointer to a callback logging function
 *                            that receives two parameters:
 *                                      
 *                            function: name of the function in which
 *                                      logger_write is being called.
 *                            message:  body of the message.
 *
 * @return                    result code
 */
int client_file_receive( char * remote_filename, 
                         char * local_filename, 
                         char * addr, 
                         char * port, 
                         int timeout, 
                         int timeout_ack, 
                         void (*log_writer) (const char* function, char* message) );

/**
 * Performs a 'File Send' operation for the client
 *
 * @param remote_filename     filename in destination location
 * @param local_filename      filename in location of origin
 * @param addr                server IP
 * @param port                server port
 * @param timeout             timeout for messages, can be 0 for default
 * @param timeout_ack         timeout for ack messages, can be 0 for default
 * @param log_writer          a pointer to a callback logging function
 *                            that receives two parameters:
 *                                      
 *                            function: name of the function in which
 *                                      logger_write is being called.
 *                            message:  body of the message.
 *
 * @return                    result code
 */
int client_file_send( char * remote_filename, 
                      char * local_filename, 
                      char * addr, 
                      char * port, 
                      int timeout, 
                      int timeout_ack, 
                      void (*log_writer) (const char* function, char* message) );

/**
 * Performs a 'File Delete' operation for the client on the server
 *
 * @param remote_filename     filename in destination
 * @param addr                server IP
 * @param port                server port
 * @param timeout             timeout for messages, can be 0 for default
 * @param timeout_ack         timeout for ack messages, can be 0 for default
 * @param log_writer          a pointer to a callback logging function
 *                            that receives two parameters:
 *                                      
 *                            function: name of the function in which
 *                                      logger_write is being called.
 *                            message:  body of the message.
 *
 * @return                    result code
 */
int client_file_delete( char * remote_filename, 
                        char * addr, 
                        char * port, 
                        int timeout, 
                        int timeout_ack,
                        void (*log_writer) (const char* function, char* message) );

#ifdef __cplusplus
}
#endif

#endif  // _CLIENT_H