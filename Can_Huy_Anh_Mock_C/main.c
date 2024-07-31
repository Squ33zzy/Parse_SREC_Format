/*******************************************************************************
* Include
*******************************************************************************/

#include "sreclib.h" 
/* Define a macro for test program mode (set to 0 for normal mode). */
#define TEST_PROGRAM 0
/*******************************************************************************
* Function
*******************************************************************************/
/* Function to print the S-record data to the output file and to the console. */
static void printDataSrec(FILE* output, uint32_t lineNumber, const char* add, const char* data, const char* error)
{
    fprintf(output, "%-10u %-10s %-70s %-20s\n", lineNumber, add, data, error);  
    printf("%-10u %-10s %-70s %-20s\n", lineNumber, add, data, error);  
}

/* Function to parse S-records from an input file and write results to an output file. */
static void parseSrecord(const char* inputFile, const char* outputFile)
{
    char line[LENGTH_OF_LINE];  /* Buffer to hold each line read from the input file. */
    uint32_t numberOfLine;  /* Current line number in the input file. */
    SrecTpye stype;  /* Type of S-record (S1, S2, etc.). */
    char errorMessage[255];  /* Buffer to hold error messages. */
    char address[10];  /* Buffer to hold the address field of the S-record. */
    char data[255];  /* Buffer to hold the data field of the S-record. */
    uint32_t lengthOfAddress;  /* Length of the address field based on S-record type. */
    uint32_t lengthOfData;  /* Length of the data field in the S-record. */
    DataRecordType typeOfData;  /* Type of data record being processed. */
    FILE* srecfile = fopen(inputFile, "r");  /* File pointer for reading S-records. */
    FILE* output = fopen(outputFile, "w");  /* File pointer for writing output. */
    uint16_t index;  /* Loop index for character processing. */
    uint32_t lengthOfLine;  /* Length of the current line being processed. */

    if ((srecfile == NULL) || (output == NULL))
    {
        perror("ERROR: Can't open this file");  /* Print error if file opening fails. */
        return;
    }
    else
    {
        numberOfLine = 0;  /* Initialize line number counter. */
        typeOfData = CheckDataRecord(srecfile);  /* Determine the type of data record. */
        rewind(srecfile);  /* Rewind file to the beginning for processing. */

        /* Print headers for the output file and console. */
        fprintf(output, "%-10s %-10s %-70s %-8s\n", "LINE", "ADDRESS", "DATA", "ERROR MESSAGE");
        printf("%-10s %-10s %-70s %-8s\n", "LINE", "ADDRESS", "DATA", "ERROR MESSAGE");

        /* Process each line in the input file. */
        while (fgets(line, sizeof(line), srecfile) != NULL)
        {
            numberOfLine++;  /* Increment line number. */
            strcpy(errorMessage, "Valid");  /* Initialize error message as "Valid". */
            lengthOfLine = (uint32_t)strlen(line);  /* Get the length of the current line. */

            /* Remove trailing newline character if present. */
            if (line[lengthOfLine - 1] == '\n')
            {
                line[lengthOfLine - 1] = '\0';
                lengthOfLine--;
            }

            stype = CheckSType(line);  /* Determine the type of S-record. */

            /* Determine the length of the address field based on the S-record type. */
            if (typeOfData == S1_TYPE || stype == S5)
            {
                lengthOfAddress = 4;
            }
            else if (typeOfData == S2_TYPE)
            {
                lengthOfAddress = 6;
            }
            else
            {
                lengthOfAddress = 8;
            }

            /* Check for various validation errors. */
            if (stype == INVALID)
            {
                strcpy(errorMessage, "Invalid: Wrong S-type");
            }
            else if ((numberOfLine == 1) && (CheckSrecStart(line) != 1))
            {
                strcpy(errorMessage, "Invalid: First line is not S0 type.");
            }
            else if ((stype != typeOfData))
            {
                if (stype == S0 || stype == S9 || stype == S5 || stype == S8 || stype == S7 || stype == S6)
                {
                    /* No specific action for these S-types in this condition. */
                }
                else
                {
                    strcpy(errorMessage, "Invalid: Asynchronous S-type");
                }
            }
            else if (CheckHexaLine(line, lengthOfLine) != 1)
            {
                strcpy(errorMessage, "Invalid: The line has a non-hexadecimal character");
            }
            else if (CheckByteCountSrec(line, lengthOfLine) != 1)
            {
                strcpy(errorMessage, "Invalid: Wrong byte count");
            }
            else if (CheckSumSrec(line) != 1)
            {
                strcpy(errorMessage, "Invalid: Checksum error");
            }
            else if (((stype == S5) || (stype == S6)) && (CheckLineCount(srecfile) != 1))
            {
                strcpy(errorMessage, "Invalid: Linecount error");
            }
            else if (((stype == S9)||(stype == S8)||(stype==S7)) && (CheckSrecTerminate(line, typeOfData) != 1))
            {
                strcpy(errorMessage, "Invalid: Terminate error");
            }
            

            /* Extract address and data fields from the line. */
            strncpy(address, line + 4, lengthOfAddress);
            address[lengthOfAddress] = '\0';
            lengthOfData = lengthOfLine - lengthOfAddress - 4 - 2;
            strncpy(data, line + 4 + lengthOfAddress, lengthOfData);
            data[lengthOfData] = '\0';

            /* Print the extracted data and error message. */
            printDataSrec(output, numberOfLine, address, data, errorMessage);
        }
    }

    fclose(srecfile);  /* Close the input file. */
    fclose(output);  /* Close the output file. */
}

/*******************************************************************************
* Main
*******************************************************************************/

int main(int argc, char* argv[])
{
#if (TEST_PROGRAM == 1)
    /* Check if the correct number of command-line arguments are provided. */
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        return 1;
    }

    const char* inputFile = argv[1];  /* Get input file name from command-line arguments. */
    const char* outputFile = argv[2];  /* Get output file name from command-line arguments. */
#else
    const char* inputFile = "Srecord_1.txt";  /* Default input file name for testing. */
    const char* outputFile = "Output.txt";  /* Default output file name for testing. */
#endif
    parseSrecord(inputFile, outputFile);  /* Call the parseSrecord function with provided file names. */

    return 0;  /* Return success status. */
}


/*******************************************************************************
* EOF
*******************************************************************************/