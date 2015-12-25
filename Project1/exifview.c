#include<stdio.h>

//input file
FILE* imgFile = NULL;

//struct for Tiff tag
struct tiffTag
{
        unsigned short id;      //2 byte tag identifier
        unsigned short dtype;   //2 byte data type
        unsigned int itemNum;   //4 byte number of data items
        unsigned int value;     //4 byte value/offset of data items
};

//struct for header
struct jpegHeader
{
	unsigned short fileStart;	//jpeg start of file
	unsigned short app1;		//app1 marker is 0xFF 0xE1
	unsigned short app1Length;	//2 byte length of app1
	char exifString[4];		//4 byte string for "Exif"
	unsigned short null;		//zero byte 0x00 0x00
	char endianness[2];		//Endianness II for little endian
	unsigned short versionNum;	//42
	unsigned int offset;		//4 byte offset
	unsigned short countVal;	//2 byte count value
};

struct newCount
{
	unsigned short val;		//2byte count value
};
	



int main(int argc, char *argv[])
{
	struct jpegHeader jpegHead;
	struct tiffTag ttag;
	struct newCount nCount;

	if(argc == 2)
	{
		//read img file
		printf("img file \n");

		//open file in read binary mode
		imgFile = fopen(argv[1], "rb");
		
		//test for error
		if(imgFile == NULL)
		{
			printf("error");
		}

		//read first 22 bytes into the jpeg header struct
		fread(&jpegHead, sizeof(jpegHead), 1, imgFile);

		//check endianness of jpeg file
		if(strcmp(jpegHead.endianness, "MM*") == 0)
		{
			printf("We do not support the endianness. \n");
			goto exit;
		}

		else
		{
			//loop through file reading a different tiff tag each time
			int i = 0;

			//start at offset 22
			fseek(imgFile, 22, SEEK_SET);
			int tagOffset = 22;                     //keep track of current tag offset

			for(i=0; i < jpegHead.countVal; i++)
			{
				fread(&ttag, sizeof(ttag), 1, imgFile);
				//271 = 0x010F, 272 = 0x0110, 34665 = 0x8769
				if((ttag.id != 271) || (ttag.id != 272) || (ttag.id != 3465))
				{
					//ignore tag and go to next tag	
					fseek(imgFile, 12, SEEK_CUR);
					tagOffset = tagOffset + 12;
					
				}

				//when the 0x010F tag is encountered
				if(ttag.id == 271)
				{
					//seek to 12 + offset from start of file
					int manufacturerInt = 12 + ttag.value;
					char manufacturerStr[ttag.itemNum +1];			//create a char array using itemNum field + 1 as the size
					fseek(imgFile, manufacturerInt, SEEK_SET);
					fread(manufacturerStr, sizeof(manufacturerStr), 1, imgFile);
					printf("Manufacturer:  %s \n", manufacturerStr);
					fseek(imgFile, tagOffset, SEEK_SET);				//go back to tag 
				}

				//when the 0x0110 tag is encountered
                                if(ttag.id == 272)
                                {
                                        //seek to 12 + offset from start of file
                                        int modelInt = 12 + ttag.value;
                                        char modelStr[ttag.itemNum +1];                   //create a char array using itemNum field + 1 as the size
                                        fseek(imgFile, modelInt, SEEK_SET);
                                        fread(modelStr, sizeof(modelStr), 1, imgFile);
                                        printf("Model:  %s \n", modelStr);
                                        fseek(imgFile, tagOffset+12, SEEK_SET);                            //go back to tag

                                }

				//when the 0x8769 tag is encountered
				if(ttag.id == 34665)
				{
					//seek to 12 + offset bytes from start of file
					
					unsigned int exifInt = ttag.value +12;
					fseek(imgFile, exifInt, SEEK_SET);
					//read in new count value
					fread(&nCount, sizeof(nCount), 1, imgFile);
	
					//read sub tiff tags
					int subTagOffset = exifInt;
					int j = 0;
					for(j = 0; j < nCount.val; j++)
					{
						fread(&ttag, sizeof(ttag), 1, imgFile);
						
						//0xA002 = 40962, 0xA003 = 40963, 0x8827 = 34855, 0x829a = 33434, 0x829d = 33437, 0x920A = 37386
						//0x9003 = 36867

						if((ttag.id != 40962) || (ttag.id != 40963) || (ttag.id != 33434) || (ttag.id != 37386) || (ttag.id != 36867) || (ttag.id != 34855) || (ttag.id != 33437))
						{
							//ignore and go to next tag
							fseek(imgFile, subTagOffset, SEEK_CUR);
							subTagOffset = subTagOffset + 12;
						}
						
						if(ttag.id == 40962)
						{
							//no seeking needs to be done
							char widthStr[ttag.itemNum +1];
							fread(widthStr, sizeof(widthStr), 1, imgFile);
							printf("Width in pixels:  %s, \n", widthStr);	
						}
						
						if(ttag.id == 40963)
                                                {
                                                        //no seeking needs to be done
                                                        unsigned int heightInt = 0;
                                                        fread(&heightInt, sizeof(heightInt), 1, imgFile);
                                                        printf("Height in pixels:  %d, \n", heightInt);
                                                }

						if(ttag.id == 34855)
						{
							//no seeking needs to be done
							unsigned short isoSpeed = 0;
							fread(&isoSpeed, sizeof(isoSpeed),1, imgFile);
							printf("ISO:  ISO %d \n", isoSpeed);	
						}

						if(ttag.id == 33434)
						{
							//seek to 12 + offset
							int exposureInt = ttag.value + 12;
							fseek(imgFile, exposureInt, SEEK_SET);

							//read two unsigned integers and display ratio
							unsigned int exp1 = 0;
							unsigned int exp2 = 0;
							fread(&exp1, sizeof(exp1), 1, imgFile);
							//get next int
							fseek(imgFile, exposureInt +4, SEEK_SET);
							fread(&exp2, sizeof(exp2), 1, imgFile);
							printf("Exposure time:  %d/%d second \n", exp1, exp2);
							fseek(imgFile, exposureInt, SEEK_SET);
							fseek(imgFile, subTagOffset+2, SEEK_SET);
							
						}

						if(ttag.id == 33437)
						{

							//seek to 12 + offset
                                                        int fInt = ttag.value + 12;
                                                        fseek(imgFile, fInt, SEEK_SET);

                                                        //read two unsigned integers and display ratio
                                                        float f1 = 0;
                                                        float f2 = 0;
                                                        fread(&f1, sizeof(f1), 1, imgFile);
                                                        //get next int
                                                        fseek(imgFile, fInt +4, SEEK_SET);
                                                        fread(&f2, sizeof(f2), 1, imgFile);
							double ratio = f1 / f2;
                                                        printf("F-stop:  f/%.1f second \n",ratio);
                                                        fseek(imgFile, fInt, SEEK_SET);
                                                        fseek(imgFile, subTagOffset+2, SEEK_SET);

						}

						if(ttag.id == 37386)
						{
							//seek to 12 + offset
							int lengthInt = ttag.value + 12;
							fseek(imgFile, lengthInt, SEEK_SET);

                                                        //read two unsigned integers and display ratio
                                                        unsigned int L1 = 0;
                                                        unsigned int L2 = 0;
                                                        fread(&L1, sizeof(L1), 1, imgFile);
                                                        //get next int
                                                        fseek(imgFile, lengthInt +4, SEEK_SET);
                                                        fread(&L2, sizeof(L2), 1, imgFile);
                                                        double Lratio = L1 / L2;
                                                        printf("Focal Length: %d mm \n",Lratio);
                                                        fseek(imgFile, lengthInt, SEEK_SET);
                                                        fseek(imgFile, subTagOffset+2, SEEK_SET);

						}

						if(ttag.id == 36867)
						{
							//seek to 12 + offset from start of file
                                        		int dateInt = 12 + ttag.value;
                                        		char dateStr[ttag.itemNum +1];                  //create a char array using itemNum field + 1 as the size
                                        		fseek(imgFile, dateInt, SEEK_SET);
                                        		fread(dateStr, sizeof(dateStr), 1, imgFile);
                                        		printf("Date Taken:  %s \n", dateStr);
                                        		fseek(imgFile, subTagOffset+2, SEEK_SET);                            //go back to tag
							
						}


					}
				}
			}
		}
	}

	else
	{
		printf("Type filename when running program \n");
	}

exit:	return 0;
}
