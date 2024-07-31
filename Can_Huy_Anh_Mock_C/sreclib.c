/*******************************************************************************
* Include
*******************************************************************************/

#include "sreclib.h"
/* Define max function to find the maximum of 2 numbers */
#define MAX(x, y) (x > y ? x : y)

/*******************************************************************************
* Prototype
*******************************************************************************/

/**
 * @brief: Convert a single hexadecimal character to its integer value
 *
 * This function converts a single hexadecimal character (0-9, A-F, a-f) to its integer equivalent.
 *
 * @param[in] hexadecimal: The hexadecimal character to convert.
 * @return: The integer value of the hexadecimal character, or WORTHLESS if invalid.
 */
static uint8_t ConvertHexToIntDigit(char hexadecimal)
{
    uint8_t newvalue;

    /* Check if the character is between '0' and '9' */
    if ((hexadecimal >= '0') && (hexadecimal <= '9'))
    {
        newvalue = hexadecimal - '0';  /* Convert '0'-'9' to 0-9 */
    }
    /* Check if the character is between 'A' and 'F' */
    else if ((hexadecimal >= 'A') && (hexadecimal <= 'F'))
    {
        newvalue = hexadecimal - 'A' + 10;  /* Convert 'A'-'F' to 10-15 */
    }
    /* Check if the character is between 'a' and 'f' */
    else if ((hexadecimal >= 'a') && (hexadecimal <= 'f'))
    {
        newvalue = hexadecimal - 'a' + 10;  /* Convert 'a'-'f' to 10-15 */
    }
    else
    {
        newvalue = WORTHLESS;  /* Return WORTHLESS if character is not valid */
    }

    return newvalue;
}

/**
 * @brief: Convert a pair of hexadecimal characters to their integer value
 *
 * This function converts a pair of hexadecimal characters to a single integer value.
 *
 * @param[in] hexadecimal: The pair of hexadecimal characters to convert.
 * @return: The integer value of the hexadecimal pair, or WORTHLESS if invalid.
 */
static uint8_t ConvertHextoInt(const char* hexadecimal)
{
    uint8_t HighDigit;
    uint8_t LowDigit;
    uint8_t newValue;

    /* Convert the high and low hexadecimal digits to integer values */
    HighDigit = ConvertHexToIntDigit(hexadecimal[0]);
    LowDigit = ConvertHexToIntDigit(hexadecimal[1]);

    /* Combine the high and low digits to form the integer value */
    if ((HighDigit != WORTHLESS) && (LowDigit != WORTHLESS))
    {
        newValue = (HighDigit << 4) + LowDigit;  /* Shift high digit and add low digit */
    }
    else
    {
        newValue = WORTHLESS;  /* Return WORTHLESS if any digit is invalid */
    }

    return newValue;
}

/**
 * @brief: Check if a character is a valid hexadecimal character
 *
 * This function checks if the given character is a valid hexadecimal character (0-9, A-F, a-f).
 *
 * @param[in] character: The character to check.
 * @return: 1 if valid hexadecimal character, 0 otherwise.
 */
static uint8_t CheckHexCharacter(char character)
{
    uint8_t Flag;

    /* Initialize flag to true */
    Flag = 1;

    /* Check if the character is within the valid hexadecimal range */
    if ((character >= '0') && (character <= '9') ||
        (character >= 'A') && (character <= 'F') ||
        (character >= 'a') && (character <= 'f'))
    {
        Flag = 1;  /* Valid hexadecimal character */
    }
    else
    {
        Flag = 0;  /* Invalid character */
    }

    return Flag;
}

/*******************************************************************************
* Function
*******************************************************************************/

/**
 * @brief: Determine the S-type of a line in the S-record file
 *
 * This function checks the S-type of a given line from the S-record file.
 *
 * @param[in] line: The line from the S-record file.
 * @return: The S-type of the line.
 */
extern SrecTpye CheckSType(const char* line)
{
    SrecTpye Flag;

    /* Check if the line is NULL or does not start with 'S' */
    if ((line == NULL) || (line[0] != 'S') || (strlen(line) < 2))
    {
        Flag = INVALID;  /* Invalid S-record type */
    }
    else
    {
        /* Determine the S-type based on the second character of the line */
        switch (line[1])
        {
        case '0':
            Flag = S0;
            break;
        case '1':
            Flag = S1;
            break;
        case '2':
            Flag = S2;
            break;
        case '3':
            Flag = S3;
            break;
        case '4':
            Flag = S4;
            break;
        case '5':
            Flag = S5;
            break;
        case '6':
            Flag = S6;
            break;
        case '7':
            Flag = S7;
            break;
        case '8':
            Flag = S8;
            break;
        case '9':
            Flag = S9;
            break;
        default:
            Flag = INVALID;  /* Invalid S-type */
            break;
        }
    }
    return Flag;
}

/**
 * @brief: Check if the line starts with a valid S-record identifier (S0)
 *
 * This function verifies whether a given line from the S-record file starts with an S0 identifier.
 *
 * @param[in] line: The line from the S-record file.
 * @return: 1 if the line starts with S0, 0 otherwise.
 */
extern uint8_t CheckSrecStart(const char* line)
{
    SrecTpye SType;
    uint8_t Flag;

    /* Initialize flag to true */
    Flag = 1;

    /* Determine the S-type of the line */
    SType = CheckSType(line);

    /* Check if the S-type is S0 */
    if (SType != S0)
    {
        Flag = 0;  /* Invalid start line */
    }
    else
    {
        Flag = 1;  /* Valid start line */
    }

    return Flag;
}

/**
 * @brief: Check the byte count in an S-record line
 *
 * This function checks if the byte count specified in the S-record line is correct.
 *
 * @param[in] line: The line from the S-record file.
 * @param[in] lengthOfLine: The length of the line to be checked.
 * @return: 1 if the byte count is correct, 0 otherwise.
 */
extern uint8_t CheckByteCountSrec(const char* line, uint32_t lengthOfLine)
{
    uint8_t Flag;
    uint8_t ByteCount;

    /* Initialize flag to true */
    Flag = 1;

    /* Check if the line is NULL or too short */
    if ((line == NULL) || (strlen(line) < 4))
    {
        Flag = 0;  /* Invalid line length */
    }
    else
    {
        /* Convert byte count from hexadecimal to integer */
        ByteCount = ConvertHextoInt(&line[2]);

        /* Check if the byte count is valid */
        if (ByteCount == WORTHLESS)
        {
            Flag = 0;  /* Invalid byte count */
        }
        else
        {
            /* Verify the byte count range and length */
            if ((ByteCount < 3) || (ByteCount > 255) || ((ByteCount * 2) != (lengthOfLine - 4)))
            {
                Flag = 0;  /* Byte count error */
            }
            else
            {
                Flag = 1;  /* Byte count is correct */
            }
        }
    }

    return Flag;
}

/**
 * @brief: Check the checksum of an S-record line
 *
 * This function verifies if the checksum of the S-record line is correct.
 *
 * @param[in] line: The line from the S-record file.
 * @return: 1 if the checksum is correct, 0 otherwise.
 */
extern uint8_t CheckSumSrec(const char* line)
{
    uint8_t Flag;
    uint8_t sum;
    uint32_t lengthOfLine;
    uint8_t index;
    uint8_t convertedValue;
    uint8_t LineCheckSum;
    uint8_t ComputedCheckSum;

    sum = 0;
    Flag = 1;
    lengthOfLine = (uint32_t)strlen(line);

    /* Check if all characters in the line are hexadecimal */
    for (index = 2; index < lengthOfLine - 2; index++)
    {
        if (CheckHexCharacter(line[index]) == 0)
        {
            Flag = 0;  /* Invalid character */
        }
    }

    /* Calculate the checksum */
    for (index = 2; index < lengthOfLine - 2; index += 2)
    {
        convertedValue = ConvertHextoInt(&line[index]);
        if (convertedValue == WORTHLESS)
        {
            Flag = 0;  /* Conversion error */
        }
        else
        {
            sum += convertedValue;  /* Accumulate sum */
        }
    }

    /* Get the checksum value from the line */
    LineCheckSum = ConvertHextoInt(&line[lengthOfLine - 2]);

    /* Check if the checksum value is valid */
    if (LineCheckSum == WORTHLESS)
    {
        Flag = 0;  /* Conversion error */
    }
    else
    {
        /* Compute the expected checksum */
        ComputedCheckSum = 0xFF - (sum & 0xFF);

        /* Verify if the computed checksum matches the line's checksum */
        if (ComputedCheckSum != LineCheckSum)
        {
            Flag = 0;  /* Checksum mismatch */
        }
        else
        {
            Flag = 1;  /* Checksum is correct */
        }
    }

    return Flag;
}

/**
 * @brief: Check the line count in the S-record file
 *
 * This function verifies if the line count specified in the S-record file is correct.
 *
 * @param[in] srecfile: The file pointer to the S-record file.
 * @return: 1 if the line count is correct, 0 otherwise.
 */
extern uint8_t CheckLineCount(FILE* srecfile)
{
    char line[LENGTH_OF_LINE];
    SrecTpye typeOfLine;
    uint8_t Flag;
    uint32_t numberOfLineData;
    uint32_t firstDigit;
    uint32_t secondDigit;
    uint32_t thirdDigit;
    uint32_t fouthDigit;
    uint32_t fifthDigit;
    uint32_t sixthDigit;
    uint32_t lineCount;

    numberOfLineData = 0;

    /* Read lines from the file */
    while (fgets(line, sizeof(line), srecfile))
    {
        /* Determine the S-type of the line */
        typeOfLine = CheckSType(line);
        if ((typeOfLine == S1) || (typeOfLine == S2) || (typeOfLine == S3))
        {
            numberOfLineData++;  /* Increment data line count */
        }
        else if (typeOfLine == S5)
        {
            /* Extract and compute the line count from the S5 record */
            firstDigit = ConvertHexToIntDigit(line[4]);
            secondDigit = ConvertHexToIntDigit(line[5]);
            thirdDigit = ConvertHexToIntDigit(line[6]);
            fouthDigit = ConvertHexToIntDigit(line[7]);

            lineCount = ((firstDigit << 12) + (secondDigit << 8) + (thirdDigit << 4) + fouthDigit);
            if (lineCount == numberOfLineData)
            {
                Flag = 1;  /* Line count matches */
            }
            else
            {
                Flag = 0;  /* Line count mismatch */
            }
        }
        else if (typeOfLine == S6)
        {
            /* Extract and compute the line count from the S6 record */
            firstDigit = ConvertHexToIntDigit(line[4]);
            secondDigit = ConvertHexToIntDigit(line[5]);
            thirdDigit = ConvertHexToIntDigit(line[6]);
            fouthDigit = ConvertHexToIntDigit(line[7]);
            fifthDigit = ConvertHexToIntDigit(line[8]);
            sixthDigit = ConvertHexToIntDigit(line[9]);

            lineCount = ((firstDigit << 20) + (secondDigit << 16) + (thirdDigit << 12)
                + (fouthDigit << 8) + (fifthDigit << 4) + sixthDigit);
            if (lineCount == numberOfLineData)
            {
                Flag = 1;  /* Line count matches */
            }
            else
            {
                Flag = 0;  /* Line count mismatch */
            }
        }
        else
        {
            /* Do nothing if the line type is not relevant */
        }
    }

    return Flag;
}

/**
 * @brief: Determine the predominant data record type in the S-record file
 *
 * This function identifies the most common data record type (S1, S2, or S3) in the S-record file.
 *
 * @param[in] srecfile: The file pointer to the S-record file.
 * @return: The most frequent data record type.
 */
extern DataRecordType CheckDataRecord(FILE* srecfile)
{
    DataRecordType recordType;
    uint32_t numberOfS1;
    uint32_t numberOfS2;
    uint32_t numberOfS3;
    SrecTpye typeOfLine;
    char line[LENGTH_OF_LINE];
    uint32_t maxOfThree;

    numberOfS1 = 0;
    numberOfS2 = 0;
    numberOfS3 = 0;

    /* Read lines from the file */
    while (fgets(line, sizeof(line), srecfile) != NULL)
    {
        /* Determine the S-type of the line */
        typeOfLine = CheckSType(line);
        if (typeOfLine == S1)
        {
            numberOfS1++;  /* Count S1 records */
        }
        else if (typeOfLine == S2)
        {
            numberOfS2++;  /* Count S2 records */
        }
        else if (typeOfLine == S3)
        {
            numberOfS3++;  /* Count S3 records */
        }
        else
        {
            /* Do nothing for other types */
        }
    }

    /* Determine the maximum count among S1, S2, and S3 records */
    maxOfThree = MAX(numberOfS1, MAX(numberOfS2, numberOfS3));
    if (maxOfThree == numberOfS1)
    {
        recordType = S1_TYPE;  /* S1 is the most frequent type */
    }
    else if (maxOfThree == numberOfS2)
    {
        recordType = S2_TYPE;  /* S2 is the most frequent type */
    }
    else
    {
        recordType = S3_TYPE;  /* S3 is the most frequent type */
    }

    return recordType;
}

/**
 * @brief: Check if the termination line of the S-record file is correct
 *
 * This function verifies if the termination line in the S-record file matches the expected record type (S7, S8, or S9).
 *
 * @param[in] line: The line from the S-record file.
 * @param[in] recordType: The expected data record type (S1_TYPE, S2_TYPE, or S3_TYPE).
 * @return: 1 if the termination line matches, 0 otherwise.
 */
extern uint8_t CheckSrecTerminate(const char* line, DataRecordType recordType)
{
    SrecTpye typeOfLine;
    uint8_t Flag;

    /* Check the expected termination line based on recordType */
    if (recordType == S1_TYPE)
    {
        typeOfLine = CheckSType(line);
        if (typeOfLine == S9)
        {
            Flag = 1;  /* Valid termination line for S1_TYPE */
        }
        else
        {
            Flag = 0;  /* Invalid termination line */
        }
    }
    else if (recordType == S2_TYPE)
    {
        typeOfLine = CheckSType(line);
        if (typeOfLine == S8)
        {
            Flag = 1;  /* Valid termination line for S2_TYPE */
        }
        else
        {
            Flag = 0;  /* Invalid termination line */
        }
    }
    else
    {
        typeOfLine = CheckSType(line);
        if (typeOfLine == S7)
        {
            Flag = 1;  /* Valid termination line for S3_TYPE */
        }
        else
        {
            Flag = 0;  /* Invalid termination line */
        }
    }

    return Flag;
}

/**
 * @brief: Check if all characters in a line are valid hexadecimal characters
 *
 * This function verifies if all characters in the given line are valid hexadecimal characters.
 *
 * @param[in] line: The line to check.
 * @param[in] lengthOfLine: The length of the line.
 * @return: 1 if all characters are valid hexadecimal, 0 otherwise.
 */
extern uint8_t CheckHexaLine(const char* line, uint32_t lengthOfLine)
{
    uint32_t index = 1;
    uint8_t Flag;

    /* Initialize flag to true */
    Flag = 1;

    /* Check each character in the line */
    for (index = 1; index < lengthOfLine; index++)
    {
        if (CheckHexCharacter(line[index]) != 1)
        {
            Flag = 0;  /* Invalid hexadecimal character */
        }
    }
    return Flag;
}

/*******************************************************************************
* EOF
*******************************************************************************/
