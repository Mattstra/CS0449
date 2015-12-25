#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//input file
FILE* inFile = NULL;

//struct to read character
struct readChar
{
	unsigned char charByte;		//char is 1 byte
};

int main(int argc, char *argv[])
{
	struct readChar inByte;
	//number of consectutive printable characters
	int i = 0;

	//keeps track of max elements
	int full = 3;

	//Initial allocation of 4 bytes for string array
	char *strArray = malloc(4 * sizeof(char));

	if(argc == 2)
	{
		//read file
		inFile = fopen(argv[1], "rb");
		
		//test for error when opening file
		if(inFile == NULL)
		{
			printf("error opening file \n");
		}

		//loop to read each byte in file and see if there are printable
		//characters
		while(1)
		{
			fread(&inByte, sizeof(inByte),1 , inFile);

			//check to see if current byte is a printable character
			if(((inByte.charByte >= 32) && (inByte.charByte <= 126)) ||(inByte.charByte == 9))
			{
				//add to string
				strArray[i] = inByte.charByte;
				++i;

				//array is full so increase size
				if(i == full)
				{
					char *newStr = realloc(strArray, i+2);

					//check to see if reallocation succeeded
					if(newStr == NULL)
					{
						printf("Error reallocating array \n");
						break;
					}

					else
					{
						strArray = newStr;
						full = i+2;
					}					
				}
			}
	
			//non printable character encountered
			else
			{
				if(i >= 4)
				{
					strArray[i+1] = '\0';
					printf("%s \n", strArray);
					i = 0;
					memset(strArray, 0, full);
				}
				else
				{
					i = 0;
					memset(strArray, 0, full);
				}
			}

			
			//check for end of file
			if(feof(inFile))
				break;
		}

		//free memory
		free(strArray);
	}

	if(argc < 2)
		printf("Add filename to run program \n");

	return 0;
}
