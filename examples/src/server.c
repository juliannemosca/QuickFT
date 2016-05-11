#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>

#include "../../dist/include/server.h"

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
 * Server example for libQuickFT
 * 
 * @param argc
 * @param argv
 * @return 
 */
int main(int argc, char *argv[]) {

  int port            = 2332;
  int max_connections = 256;
  int timeout         = 60;

  int opt;

  //
  // Parameters parsing
  // 
  struct option long_options[] = {
      {"help",        no_argument,        0,  'h' },
      {"port",        required_argument,  0,  'p' }, // port to listen on
      {"maxconn",     required_argument,  0,  'm' }, // max. connections supoerted by the server
      {"timeout",     required_argument,  0,  't' }, // timeout for messages, can be 0 for default
      {0,             0,                  0,  0   }
  };
  int option_index = 0;
  int value;

  opterr = 0;
  while ((opt = getopt_long(argc, argv, "hp:m:t:", long_options, &option_index)) != -1) {

    value = 0;
    if (optarg != 0x00)
      value = atoi(optarg);

    switch (opt)
    {
      case 'p':
        if (value != 0) port = value;
        break;

      case 'm':
        if (value != 0) max_connections = value;
        break;
      
      case 't':
        if (value != 0) timeout = value;
        break;

      case 'h':
        printf("usage: server -p [--port]: specifies port to listen on.\n");
        printf("              -m [--maxconn]: max. simultaneous connections.\n");
        printf("              -t [--timeout] timeout for messages, 0 for default.\n");
        printf("              -h [--help] displays this instructions.\n");
        printf("\n");
        return 0;

      case '?':
      default:
        printf("Invalid parameters. Type server --help for usage.\n");
        return 1;
    }
  }

  //
  // Initializes server
  //

  SERVER_T * server = NULL;

  printf("Initializing server on port %d...\n", port);
  server = server_initialize(port, max_connections, timeout*1000, &logger);

  if (server != NULL) {

    puts("[OK] ...server listening. Press Enter key to end.");

  } else {
    puts("[FAIL] ...could not initialize server.");
    return 1;
  }
  getchar();

  server_finalize(&server);
  puts("Server finalized.");

  return 0;
}
