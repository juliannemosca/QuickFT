/*
 * logger.h
 *
 * $Id: $
 * $HeadURL: $
 * $LastChangedRevision: $
 * $LastChangedDate: $
 * $LastChangedBy: $
 *
 */

#ifndef _LOGGER_H
#define _LOGGER_H

#ifdef __cplusplus
extern "C" {
#endif

#define LOGGER              logger_write
#define LOGGER_DEFAULT_NAME "quickft"

// Global pointer to the log writer function
void (*gl_log_writer) (const char* function, char* message);
  
/**
 * Writes a line to the log file
 *
 * @param function               function where the line is being added
 * @param message                message to write to the log file
 *
 */
void logger_write(const char* function, char* message);

#ifdef __cplusplus
}
#endif

#endif //_LOGGER_H
