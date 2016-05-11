#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>

#include "../../dist/include/client.h"

// Operation Types:
#define UNKNOWN     0
#define FILE_SND    1
#define FILE_RCV    2
#define FILE_DEL    3

/**
 * Demo logger callback, shows output on screen.
 *
 * @param function               name of the function generating the log entry
 * @param message                log message
 *
 */
void logger(const char* function, char* message) {

  time_t date;
  struct tm * dateinfo;
  
  time(&date);
  dateinfo = localtime(&date);

  printf("%02d/%02d/%04d - %02d:%02d:%02d : [%s] - %s\n",
    dateinfo->tm_mday,
    dateinfo->tm_mon + 1,
    dateinfo->tm_year,
    dateinfo->tm_hour,
    dateinfo->tm_min,
    dateinfo->tm_sec,
    function,
    message
  );

}

/**
 * Client example for libQuickFT
 * 
 * @param argc
 * @param argv
 * @return 
 */
int main(int argc, char *argv[]) {
  
  char opt_buffer[1024];

  char port[5+1];
  char addr[16+1];
  
  char remote_filename[1024];
  char local_filename[1024];

  int timeout     = 60;
  int timeout_ack = 60;
  int op_type     = 0;

  int opt;
  int result;

  // Default values
  strcpy(port, "2332");
  strcpy(addr, "127.0.0.1");

  // Initializes buffers
  memset(remote_filename, 0x00, (1024));
  memset(local_filename,  0x00, (1024));

  //
  // Parameters parsing
  // 
  struct option long_options[] = {
      {"help",        no_argument,        0,  'h' },
      {"operation",   required_argument,  0,  'o' }, // operation type
      {"remote",      required_argument,  0,  'r' }, // remote file name
      {"local",       required_argument,  0,  'l' }, // local file name
      {"port",        required_argument,  0,  'p' }, // server port to connect
      {"addr",        required_argument,  0,  'a' }, // server addr
      {"timeout",     required_argument,  0,  't' }, // timeout for messages, can be 0 for default
      {"timeoutack",  required_argument,  0,  'k' }, // timeout for ack messages, can be 0 for default
      {0,             0,                  0,  0   }
  };
  int option_index = 0;
  int num_value = 0;

  opterr = 0;
  while ((opt = getopt_long(argc, argv, "ho:r:l:p:a:t:k:", long_options, &option_index)) != -1) {

    num_value   = 0;
    memset(opt_buffer, 0x00, 1024);

    if (optarg != 0x00) {
      num_value = atoi(optarg);
      if (num_value == 0) {
        snprintf(opt_buffer, 1024, "%s", optarg);
      }
    }
    

    switch (opt)
    {
      case 'o':
        if (strcmp(opt_buffer, "send") == 0 ) {
          op_type = FILE_SND;
        } else if (strcmp(opt_buffer, "receive") == 0 ) {
          op_type = FILE_RCV;
        } else if (strcmp(opt_buffer, "delete") == 0) {
          op_type = FILE_DEL;
        }

        break;

      case 'r':
        if (opt_buffer[0] != '\0')
          snprintf(remote_filename, 1024, "%s", opt_buffer);   

        break;

      case 'l':
        if (opt_buffer[0] != '\0')
          snprintf(local_filename, 1024, "%s", opt_buffer);   
        
        break;

      case 'p':
        if (opt_buffer[0] != '\0')
          snprintf(port, 6, "%s", opt_buffer);   

        break;

      case 'a':
        if (opt_buffer[0] != '\0')
          snprintf(addr, 16, "%s", opt_buffer);   
      
        break;
      
      case 't':
        if (num_value != 0)
          timeout = num_value;

        break;

      case 'k':
        if (num_value != 0)
          timeout_ack = num_value;

        break;

      case 'h':
        printf("usage: client -o [--operation]: operation type (send, receive, delete).\n");
        printf("              -r [--remote]: remote file name.\n");
        printf("              -l [--local]: local file name.\n");
        printf("              -p [--port] port to connect on server.\n");
        printf("              -a [--addr] server IP address.\n");
        printf("              -t [--timeout] timeout for messages, 0 for default.\n");
        printf("              -k [--timeoutack] timeout for ack messages, 0 for default.\n");
        printf("              -h [--help] displays this instructions.\n");
        printf("\n");
        return 0;

      case '?':
      default:
        printf("Invalid parameters. Type client --help for usage.\n");
        return 1;
        
    }
  }

  //
  // Performs client operation
  //

  switch (op_type)  
  {
    case UNKNOWN:
      printf("Invalid operation type. Type client --help for usage.\n");
      return 1;

    case FILE_SND:
      if (remote_filename[0] == '\0' || local_filename[0] == '\0') {
        printf("Must specify both remote filename and local filename for send operation. Type client --help for usage.\n");
        return 1;        
      } else {
        result = client_file_send( remote_filename, local_filename, addr, port, timeout*1000, timeout_ack*1000, &logger );        
      }

      break;

    case FILE_RCV:
      if (remote_filename[0] == '\0' || local_filename[0] == '\0') {
        printf("Must specify both remote filename and local filename for receive operation. Type client --help for usage.\n");
        return 1;        
      } else {
        result = client_file_receive( remote_filename, local_filename, addr, port, timeout*1000, timeout_ack*1000, &logger );        
      }

      break;

    case FILE_DEL:
      if (remote_filename[0] == '\0') {
        printf("Must specify remote filename for delete operation. Type client --help for usage.\n");
        return 1;        
      } else {
        result = client_file_delete( remote_filename, addr, port, timeout*1000, timeout_ack*1000, &logger );        
      }

      break;

  }

  printf("End.\nOperation result code: %d\n", result);

  return 0;
}
