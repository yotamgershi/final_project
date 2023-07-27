/**
 * @file error.h
 * @brief Header file for error handling in the assembler process.
 */

#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/** @def ERROR_MSG_SIZE
 *  @brief The size of the error message buffer.
 */
#define ERROR_MSG_SIZE 256

/** @enum assembler_step
 *  @brief Enum to represent different steps of the assembler process.
 */
typedef enum {
    PRE_ASSEMBLER,  /**< Pre-assembler step */
    FIRST_PASS,     /**< First pass step */
    SECOND_PASS     /**< Second pass step */
} assembler_step;

/**
 * @enum error_code
 * @brief Enumeration to represent different error types.
 */
typedef enum {
    ERROR_ONE,    /**< Error one */
    ERROR_TWO,    /**< Error two */
    ERROR_THREE,  /**< Error three */
    ERROR_FOUR,   /**< Error four */
    ERROR_FIVE,   /**< Error five */
    NUM_ERRORS    /**< Total number of errors in the code */
} error_code;

/**
 * @struct error_item
 * @brief Structure to represent an error item.
 */
typedef struct {
    error_code code;      /**< Error code */
    const char *message;  /**< Error message */
} error_item;

/**
 * @brief Array of error items.
 *
 * This array holds all the error items, each with a corresponding error code and message.
 */
extern error_item errors[NUM_ERRORS];

/** @struct error
 *  @brief Structure to represent a single error encountered during the assembler process.
 */
typedef struct error {
    char message[ERROR_MSG_SIZE];   /**< Error message buffer */
    int line_number;                /**< Line number where the error occurred */
    struct error *next;             /**< Pointer to the next error in the linked list */
} error;

/** @struct error_list
 *  @brief Structure to represent errors for a specific assembler step.
 */
typedef struct error_list {
    assembler_step step;   /**< The assembler step for which errors are stored */
    error *head;           /**< Pointer to the head of the error linked list */
} error_list;

/** @struct error_log
 *  @brief Structure to represent all errors encountered during the assembler process.
 */
typedef struct error_log {
    error_list pre_assembler_errors;    /**< Errors encountered during the pre-assembler step */
    error_list first_pass_errors;       /**< Errors encountered during the first pass step */
    error_list second_pass_errors;      /**< Errors encountered during the second pass step */
} error_log;

/**
 * @brief Initializes the error log.
 *
 * This function initializes the error log by setting all linked lists to empty (head = NULL).
 *
 * @param log Pointer to the error log structure to be initialized.
 */
void init_error_log(error_log *log);

/**
 * @brief Adds an error to the error log.
 *
 * This function adds a new error to the corresponding error list based on the assembler step.
 *
 * @param log Pointer to the error log structure.
 * @param step The assembler step during which the error occurred.
 * @param message The error message to be added.
 * @param line_number The line number where the error occurred.
 */
void add_error(error_log *log, assembler_step step, const char *message, int line_number);

/**
 * @brief Prints the errors for a specific assembler step.
 *
 * This function prints all the errors stored in the error list for a specific assembler step.
 *
 * @param error_list Pointer to the error list for a specific assembler step.
 */
void print_errors(error_list *error_list);

/**
 * @brief Prints all errors in the error log.
 *
 * This function prints all the errors encountered during the assembler process,
 * including errors from pre-assembler, first pass, and second pass steps.
 *
 * @param log Pointer to the error log structure containing all errors.
 */
void print_all_errors(error_log *log);

/**
 * @brief Clears the error log and frees memory.
 *
 * This function clears the error log by freeing memory allocated for error nodes.
 *
 * @param log Pointer to the error log structure to be cleared.
 */
void clear_error_log(error_log *log);

#endif /* ERROR_H */
