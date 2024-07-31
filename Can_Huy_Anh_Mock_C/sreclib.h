/*******************************************************************************
* Include
*******************************************************************************/

#pragma once
#ifndef SCRECLIB_H
#define SCRECLIB_H
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define LENGTH_OF_LINE    514
#define WORTHLESS         -1

/*******************************************************************************
* Enum
*******************************************************************************/

/**
 * @brief: Enum to store S-type of line
 *
 * This enumeration defines the various S-types that can appear in S-records.
 */
typedef enum
{
    INVALID,  /**< Invalid S-record type */
    S0,       /**< S0 record type */
    S1,       /**< S1 record type */
    S2,       /**< S2 record type */
    S3,       /**< S3 record type */   
    S5,       /**< S5 record type */
    S6,       /**< S6 record type */
    S7,       /**< S7 record type */
    S8,       /**< S8 record type */
    S9,        /**< S9 record type */
    S4 = INVALID /**< S4 record type, treated as INVALID */
} SrecTpye;

/**
 * @brief: Enum to store type of data in S-record file
 *
 * This enumeration specifies the types of data records that can be present in an S-record file.
 */
typedef enum
{
    S1_TYPE = 2, /**< S1 record type */
    S2_TYPE = 3, /**< S2 record type */
    S3_TYPE = 4  /**< S3 record type */
} DataRecordType;

/*******************************************************************************
* API
*******************************************************************************/

/**
 * @brief: To check S-type of line in file S-record format
 *
 * This function determines the S-type of a given line from the S-record file.
 *
 * @param[in] line: The line from the S-record file that needs to be checked.
 * @param[out] None
 * @param[inout] None
 * @return: The S-type of the line.
 */
extern SrecTpye CheckSType(const char* line);

/**
 * @brief: To check if the line starts with an S-record identifier
 *
 * This function verifies whether a given line from the S-record file starts with a valid S-record identifier.
 *
 * @param[in] line: The line from the S-record file to be checked.
 * @param[out] None
 * @param[inout] None
 * @return: 1 if the line starts with a valid S-record identifier, 0 otherwise.
 */
extern uint8_t CheckSrecStart(const char* line);

/**
 * @brief: To check the byte count in an S-record line
 *
 * This function checks if the byte count specified in the S-record line is correct.
 *
 * @param[in] line: The line from the S-record file.
 * @param[in] lengthOfLine: The length of the line to be checked.
 * @param[out] None
 * @param[inout] None
 * @return: 1 if the byte count is correct, 0 otherwise.
 */
extern uint8_t CheckByteCountSrec(const char* line, uint32_t lengthOfLine);

/**
 * @brief: To check the checksum of an S-record line
 *
 * This function verifies if the checksum of the S-record line is correct.
 *
 * @param[in] line: The line from the S-record file.
 * @param[out] None
 * @param[inout] None
 * @return: 1 if the checksum is correct, 0 otherwise.
 */
extern uint8_t CheckSumSrec(const char* line);

/**
 * @brief: To check the line count in the S-record file
 *
 * This function checks if the line count in the S-record file is consistent with the S-record type.
 *
 * @param[in] srecfile: The file pointer to the S-record file.
 * @param[out] None
 * @param[inout] None
 * @return: 1 if the line count is correct, 0 otherwise.
 */
extern uint8_t CheckLineCount(FILE* srecfile);

/**
 * @brief: To determine the data record type in the S-record file
 *
 * This function identifies the type of data record present in the S-record file.
 *
 * @param[in] srecfile: The file pointer to the S-record file.
 * @param[out] None
 * @param[inout] None
 * @return: The type of data record.
 */
extern DataRecordType CheckDataRecord(FILE* srecfile);

/**
 * @brief: To check if the S-record line terminates correctly
 *
 * This function verifies if the termination of the S-record line is correct based on the data record type.
 *
 * @param[in] line: The line from the S-record file.
 * @param[in] recordType: The type of data record.
 * @param[out] None
 * @param[inout] None
 * @return: 1 if the termination is correct, 0 otherwise.
 */
extern uint8_t CheckSrecTerminate(const char* line, DataRecordType recordType);

/**
 * @brief: To check if the line contains only hexadecimal characters
 *
 * This function checks if the line contains only valid hexadecimal characters.
 *
 * @param[in] line: The line from the S-record file.
 * @param[in] lengthOfLine: The length of the line to be checked.
 * @param[out] None
 * @param[inout] None
 * @return: 1 if the line contains only hexadecimal characters, 0 otherwise.
 */
extern uint8_t CheckHexaLine(const char* line, uint32_t lengthOfLine);

#endif // !SCRECLIB_H

/*******************************************************************************
* EOF
*******************************************************************************/
