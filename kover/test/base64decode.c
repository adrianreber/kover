/* Global Headers */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Function Declaractions */
int get_next_char(FILE * aFile);
int lookup(char aChar);

/* Global Variables */
/* This is the standard translation table that RFC 1521 prescribes */
    char gArray[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int main (int argc, char* argv[])
{
        FILE* lFD;
        FILE* lFD2;
        char* lTemp;
        unsigned char lIn[3];
		int	lTempHold;
        char lHold;
        int lDone = 0;
        if (argc != 2)
        {
                printf("%s: Only one argument: file name to base 64 decode\n", argv[0]);
                exit(1);
        }
        if ((lFD = fopen(argv[1], "r")) < 0)
        {
                printf("%s: Could not open file (%s)\n", argv[0], argv[1]);
                exit(2);
        }
        if ((lTemp = (char*)malloc(strlen(argv[1])+3)) == 0)
        {
                fclose(lFD);
                printf("%s: Could not allocate memory for output file\n", argv[0]);
                exit(3);
        }
        strcpy(lTemp, argv[1]);
		if (strcmp(argv[1]+strlen(argv[1])-2, ".e") == 0)
			strcpy(lTemp+strlen(argv[1])-2, ".d");
		else
        	strcat(lTemp, ".d");
        if ((lFD2 = fopen(lTemp, "w")) < 0)
        {
                free(lTemp);
                fclose(lFD);
                printf("%s: Could not open output file (%s)\n", argv[0], lTemp);
                exit(4);
        }
        while(lDone == 0)
        {
			int lRead = 0;
			if ((lTempHold = get_next_char(lFD)) != EOF)
			{
				lIn[0] = lTempHold;
				if ((lTempHold = get_next_char(lFD)) != EOF)
				{
					lIn[1] = lTempHold;
					if ((lTempHold = get_next_char(lFD)) != EOF)
					{
						lIn[2] = lTempHold;
						if ((lTempHold = get_next_char(lFD)) != EOF)
						{
							lIn[3] = lTempHold;
							lRead++;
						}
						lRead++;
					}
					lRead++;
				}
				lRead++;
			}
			
        /*
         * lRead refers to the number of characters that have been read in.
         * This will always be four until the end of the file.  At the end
         * of a file that isn't a multiple of four in length, lRead will
         * equal 1, 2, or 3.  These special cases are taken care of below.
         */
            switch(lRead)
            {
			case 0:
				lDone = 1;
				break;
            case 1:
                fclose(lFD);
				fclose(lFD2);
				free(lTemp);
				printf("%s: This input file %s has a bad ending, can not decode\n", argv[0], argv[1]);
                exit(6);
            case 2:
				if (fputc( (lookup(lIn[0]) << 2) + (lookup(lIn[1]) >> 4) , lFD2) == EOF)
				{
					fclose(lFD);
					fclose(lFD2);
					printf("%s: Error writing to output file (%s)", argv[0], lTemp);
					free(lTemp);
					exit(6);
				}
                break;
            case 3:
				if (fputc( (lookup(lIn[0]) << 2) + (lookup(lIn[1]) >> 4) , lFD2) == EOF)
                {
                    fclose(lFD);
                    fclose(lFD2);
                    printf("%s: Error writing to output file (%s)", argv[0], lTemp);
                    free(lTemp);
                    exit(6);
                }
				if (fputc( (lookup(lIn[1]) << 4) + (lookup(lIn[2]) >> 2) , lFD2) == EOF)
                {
                    fclose(lFD);
                    fclose(lFD2);
                    printf("%s: Error writing to output file (%s)", argv[0], lTemp);
                    free(lTemp);
                    exit(6);
                }
                break;
            case 4:
				if (fputc( (lookup(lIn[0]) << 2) + (lookup(lIn[1]) >> 4) , lFD2)== EOF)
                {
                    fclose(lFD);
                    fclose(lFD2);
                    printf("%s: 1Error writing to output file (%s)", argv[0], lTemp);
                    free(lTemp);
                    exit(6);
                }
				if (fputc( (lookup(lIn[1]) << 4) + (lookup(lIn[2]) >> 2), lFD2)== EOF)
                {
                    fclose(lFD);
                    fclose(lFD2);
                    printf("%s: 2Error writing to output file (%s)", argv[0], lTemp);
                    free(lTemp);
                    exit(6);
                }
				if (fputc( (lookup(lIn[2]) << 6) + lookup(lIn[3]), lFD2)== EOF)
                {
                    fclose(lFD);
                    fclose(lFD2);
                    printf("%s: 3Error writing to output file (%s)", argv[0], lTemp);
                    free(lTemp);
                    exit(6);
                }
				break;
			default:
                fclose(lFD);
                fclose(lFD2);
                free(lTemp);
                printf("%s: Error reading the input file (%s)\n", argv[0], argv[1]);
                exit(6);
            }
        }
	fclose(lFD);
	fclose(lFD2);
	printf("%s: Output file (%s) written\n", argv[0], lTemp);
	free(lTemp);
}

int get_next_char(FILE * aFile)
{
    static int gPad_Flag = 0;
    char lChar;

    if (gPad_Flag) {
        printf("Error: pad flags found in the middle of the file instead of at the end.\n");
    }
    while ((lChar = fgetc(aFile)) != EOF) {
        if (isdigit(lChar)) break;

        /*
         * isupper and islower are used instead of isalpha
         * because, in some locales (C header file implementations),
         * isalpha includes non-"abc" characters.
         */
        if (isupper(lChar)) break;
        if (islower(lChar)) break;

        if (lChar == '+' || lChar == '/') break;

        /*
         * If it is not one of the above characters, print an error,
         * and grab another character.
         */
        if (lChar == '=') {
            gPad_Flag++;
            if (gPad_Flag > 2) {
                printf("Found an invalid sequence of pad characters.\n");
            }
        }
        if (!isspace(lChar) && lChar != '=') {
            printf("Found an invalid character val=(%d) hex=(%x) char=(%c)\n", lChar, lChar, lChar);
        }
    }

    return(lChar);
}

int lookup(char aChar)
{
    int i;

    for (i = 0; i < 64; i++) {
        if (gArray[i] == aChar) break;
    }
    return(i);
}
