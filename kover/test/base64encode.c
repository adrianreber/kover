/* Global Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
	char lArray[65];
	FILE* lFD;
	FILE* lFD2;
	char* lTemp;
	unsigned char lIn[3];
	int lTempHold;
	char lHold;
	int lDone = 0, lCount = 0;
	char pw  [] = "adreit00:leicht23a\n";

	char base64[1000];
	char base65[1000];
	int i,bla;

	bla=strlen(pw);

	i=0;
	strcpy(lArray, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");


	while(lDone == 0)
	{
		int lRead = 0;
		if (i < bla)
		{
			lIn[0] = pw[i++];
			if (i < bla)
			{
				lIn[1] = pw[i++];
				if (i < bla)
				{
					lIn[2] = pw[i++];
					lRead++;
				}
				lRead++;
			}
			lRead++;
		}

        /*
         * Every 19 blocks of three bytes needs to have a line break
         */
        if (lCount == 19 * 3) {
			 sprintf(base64,"%c",'\n');
			 //strcat(base64,'\n');
		
			 lCount = 0;
        }
        lCount += lRead;

        /*
         * lRead refers to the number of characters that have been read in.
         * This will always be three until the end of the file.  At the end
         * of a file that isn't a multiple of three in length, lRead will
         * equal 1 or 2.  These special cases are taken care of below.
         */
		switch(lRead)
		{
		case 0:
			lDone = 1;
			break;
		case 1:
			lHold = lIn[0] >> 2;
			sprintf(base64,"%s%c",base64,lArray[lHold]);
			//strcat(base64,lArray[lHold]);
		
			lHold = (lIn[0] << 4) & 63;
			//strcat(base64,lArray[lHold]);
          sprintf(base64,"%s%c",base64,lArray[lHold]);
            /* Add padding */
			strcat(base64,"==");
          
      
			lDone = 1;
			break;
		case 2:
            lHold = lIn[0] >> 2;
				//strcat(base64,lArray[lHold]);
            sprintf(base64,"%s%c",base64,lArray[lHold]);
            lHold = ((lIn[0] << 4) & 63) + (lIn[1] >> 4);
            //strcat(base64,lArray[lHold]);
				sprintf(base64,"%s%c",base64,lArray[lHold]);
			lHold = (lIn[1] << 2) & 63;
			//strcat(base64,lArray[lHold]);
			sprintf(base64,"%s%c",base64,lArray[lHold]);
			  /* Add padding */
           strcat(base64,"=");
            lDone = 1;
            break;
		case 3:
            lHold = lIn[0] >> 2;
				//strcat(base64,lArray[lHold]);
				sprintf(base64,"%s%c",base64,lArray[lHold]);
            lHold = (((lIn[0] << 4) & 63) + (lIn[1] >> 4));
				//strcat(base64,lArray[lHold]);
          sprintf(base64,"%s%c",base64,lArray[lHold]);
            lHold = (((lIn[1] << 2) & 63) + (lIn[2] >> 6));
				//strcat(base64,lArray[lHold]);
				sprintf(base64,"%s%c",base64,lArray[lHold]);
			lHold = lIn[2] & 63;
			  //strcat(base64,lArray[lHold]);
			  sprintf(base64,"%s%c",base64,lArray[lHold]);
			break;
		default:
		
			exit(5);
		}
	}

	printf("%s is base64 encoded string\n", base64);

	return(0);
}
