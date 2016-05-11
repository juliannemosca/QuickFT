/*
 * logger.c
 *
 * $Id: $
 * $HeadURL: $
 * $LastChangedRevision: $
 * $LastChangedDate: $
 * $LastChangedBy: $
 *
 */

#include <stdlib.h>
#include "logger.h"

/**
 * Writes a line to the log file
 *
 * @param function               function where the line is being added
 * @param message                message to write to the log file
 *
 */
void logger_write(const char* function, char* message) {

  if (gl_log_writer != NULL) {  
    gl_log_writer(function, message);
  }
  
}
