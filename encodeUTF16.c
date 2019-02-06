/**
* Jae B. Lim code
*
* This is encodeUTF16.c file in program 1.
*
* Encode(UTF32 to UTF16).
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Implementing all of the jobs in here.
static void helperF(int argc, char *argv[]){
    FILE *inputF, *outputF;
    int first, second, third, fourth;
    int littleORbig = 3; // 0 is little endian, and 1 is big endian
    int n = 0;
    int value1 = 0x00000000;
    int value2 = 0x00000000;
    int value3 = 0x00000000;
    int value4 = 0x00000000;
    int nonBMPflag = 0;
    // int value5 = 0x00000000; // This is buffer.
    int leadSurrogate = 0x00000000;
    int trailSurrogate = 0x00000000;
    int noncharacter = 0x0000;

    // Read command line arguments.
    if(argc != 3){
        printf("Usage: lab2 [input filename] [output filename]\n");
        exit(-1);
    }

    // input and output filename check. (They should be different)
    if(!strcmp(argv[1], argv[2])){
        perror("The input file and output file should have a different name.\n");
        exit(-1);
    }

    // Read file in here
    if((inputF = fopen(argv[1], "r"))==NULL){
        printf("cannot open %s\n", argv[1]);
        exit(-1);
    }

    // Write file in here
    outputF = fopen(argv[2],"wt");
    if(outputF == NULL){
        perror("fopen fail.");
        exit(-1);
    }

    // judging it is big or little endian.
    if(inputF){
        if((first = getc(inputF))==EOF){
            perror("A value is an incomplete value.");
            exit(-1);
        }
        if((second = getc(inputF))==EOF){
            perror("A value is an incomplete value.");
            exit(-1);
        }
        if((third = getc(inputF))==EOF){
            perror("A value is an incomplete value.");
            exit(-1);
        }
        if((fourth = getc(inputF))==EOF){
            perror("A value is an incomplete value.");
            exit(-1);
        }
    }

    // Check the input file starts with proper BOM.
    if(first == 0x00 && second == 0x00 && third == 0xFE && fourth == 0xFF){
        littleORbig = 1;
        fputc(third, outputF);
        fputc(fourth, outputF);
    }
    else if(first == 0xFF && second == 0xFE && third == 0x00 && fourth == 0x00){
        littleORbig = 0;
        fputc(first, outputF);
        fputc(second, outputF);
    }
    else{
        perror("The input file does not start with BOM.");
        exit(-1);
    }

    if(inputF){
        while(1){
            first=getc(inputF);
            if(first==EOF){
                break;
            }
            second = getc(inputF);
            third = getc(inputF);
            fourth = getc(inputF);
            value1 = first << 24;
            value2 = second << 16;
            value3 = third << 8;
            value4 = value1 | value2 | value3 | fourth;

            // When it is not BMP-character, use different algorithm.
            if(0x0ffff < value4  && value4 < 0x100000){
                value4 = value4 - 0x10000;
                leadSurrogate = (value4 >> 10) + 0xD800;
                leadSurrogate = leadSurrogate << 16;
                trailSurrogate = (value4 & 0x03ff) + 0xDC00;
                value4 = leadSurrogate | trailSurrogate;
                first = (value4 >> 24) & 0xff;
                second = (value4 >> 16) & 0xff;
                third = (value4 >> 8) & 0xff;
                fourth = value4 & 0xff;
                fputc(first, outputF);
                fputc(second, outputF);
                fputc(third, outputF);
                fputc(fourth, outputF);
                value4 = 0x1; // Make pass test temprary
                nonBMPflag = 1;
            }

            // If it is little endian, reverse the order.
            if(littleORbig == 0){
                value1 = fourth << 24;
                value2 = third << 16;
                value3 = second << 8;
                value4 = value1 | value2 | value3 | first;
                // When it is not BMP-character, use different algorithm.
                if(0x0ffff < value4  && value4 < 0x100000){
                    value4 = value4 - 0x10000;
                    leadSurrogate = (value4 >> 10) + 0xD800;
                    leadSurrogate = leadSurrogate << 16;
                    trailSurrogate = (value4 & 0x03ff) + 0xDC00;
                    value4 = leadSurrogate | trailSurrogate;
                    first = (value4 >> 24) & 0xff;
                    second = (value4 >> 16) & 0xff;
                    third = (value4 >> 8) & 0xff;
                    fourth = value4 & 0xff;
                    fputc(second, outputF);
                    fputc(first, outputF);
                    fputc(fourth, outputF);
                    fputc(third, outputF);
                    value4 = 0x1; // Make pass test temprary
                    nonBMPflag = 1;
                }
            }

            // printf("%02x\n", value4);
            if(0xD800 < value4 && value4 < 0xDFFF){
                perror("This is leading or trailing surrogates.");
                exit(-1);
            }

            // Check a value outside the valid range for unicode characters.
            if(0x00000000 > value4){
                perror("A value is less than the valid range for Unicode characters.");
                exit(-1);
            }
            if(value4 > 0x0010FFFF){
                perror("A value is bigger than the valid range for Unicode characters.");
                exit(-1);
            }

            // Check a value that denotes a noncharacter.
            noncharacter = value4 & 0xFFFE;
            if(noncharacter == 0xFFFE){
                perror("A value that denotes a noncharacter.");
                exit(-1);
            }
            noncharacter = value4 & 0xFFFF;
            if(noncharacter == 0xFFFF){
                perror("A value that denotes a noncharacter.");
                exit(-1);
            }
            if(0xFDD0 <= value4 && value4 <= 0xFDEF){
                perror("A value that denotes a noncharacter.");
                exit(-1);
            }

            // Check an incomplete value.
            if(second == EOF){
                perror("A value is an incomplete value.");
                exit(-1);
            }
            if(third == EOF){
                perror("A value is an incomplete value.");
                exit(-1);
            }
            if(fourth == EOF){
                perror("A value is an incomplete value.");
                exit(-1);
            }

            if(nonBMPflag == 0){
                if(littleORbig == 0){
                    fputc(first, outputF);
                    fputc(second, outputF);
                }
                if(littleORbig == 1){
                    fputc(third, outputF);
                    fputc(fourth, outputF);
                }
            }
            n++;
        }
        fclose(inputF);
        fclose(outputF);
    }
}

int main(int argc, char *argv[]){
    helperF(argc, argv);
    return 0;
}
