#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<ctype.h>
#include <string>
#include <sstream>
#pragma warning(disable : 4996)
void decompress(char* data, int count, FILE* outfile);
char* readFileData(char* filename, int* count_ptr);
void usage();
int main(int num_args, char* arg_values[])
{
    bool var;
    bool outfilecheck = false;
 
    if (num_args > 4 || num_args == 1) {
        usage();
    }

    else {
        if (num_args == 2) {
            if (strcmp(arg_values[1], "--help") == 0) {
                printf("HELP");
            }
            else if (strcmp(arg_values[1], "--version") == 0) {
                printf("1.0.0");
                exit(1);
            }
            else {
                usage();
            }
            exit(1);
        }
        if (num_args == 3) {
            if (strcmp(arg_values[1], "-b") == 0 || strcmp(arg_values[1], "-t") == 0) {
                if (arg_values[1][1] == 'b') {
                    var = true;
                    printf("Mode: Binary\n");
                    printf("Origin File Name: %s\n", arg_values[2]);
                }
                else if (arg_values[1][1] == 't') {
                    var = false;
                    printf("Mode: Text\n");
                    printf("Origin File Name: %s\n", arg_values[2]);
                }
                else {
                    usage();
                }
            }
            else {
                usage();
            }

        }
        if (num_args == 4) {
            
            outfilecheck = true;
            if (strcmp(arg_values[1], "-b") == 0 || strcmp(arg_values[1], "-t") == 0) {
                if (arg_values[1][1] == 'b') {
                    var = true;
                    printf("Mode: Binary\n");
                    printf("Origin File Name: %s\n", arg_values[2]);
                    printf("Output File Name: %s\n", arg_values[3]);
                }
                else if (arg_values[1][1] == 't') {
                    var = false;
                    printf("Mode: Text\n");
                    printf("Origin File Name: %s\n", arg_values[2]);
                    printf("Output File Name: %s\n", arg_values[3]);
                }
                else {
                    usage();
                }
            }
            else {
                usage();
            }
            //  exit(1);
        }




        char* input_filename = arg_values[2];

        // read the file data into an array
        int count;
        char* data = readFileData(input_filename, &count);
        // Call compress() or decompress().
        FILE* outfile;
        int len = strlen(input_filename);
       
            char output_filename[80];
            strncpy(output_filename, input_filename, len - 4);
            output_filename[len - 4] = 0;
            if (!outfilecheck) {
                strcpy(output_filename, input_filename);
            }
            else {
                char* output = arg_values[3];
                strcpy(output_filename, output);
            }

           if (var) {
               strcat(output_filename, ".txt");
                printf("Decompressing %s to %s\n", input_filename, output_filename);
                outfile = fopen(output_filename, "wb");
                decompress(data, count, outfile);
           }
           else {
               strcat(output_filename, ".txt");
               printf("Decompressing %s to %s\n", input_filename, output_filename);
               outfile = fopen(output_filename, "w");
               decompress(data, count, outfile);

           }
            
        
        // Close the output file to ensure data is saved.
        fclose(outfile);
        // Free the array we allocated
        delete data;
        return 0;
    }
   
}
void decompress(char* x, int count, FILE* outfile)
{
    char decoded[80];
    // TODO: decompress the data instead of just writing it out to the file
    for (int i = 0; i < count; ++i) {
        if ((x[i] >= 65 && x[i] <= 90) || (x[i] >= 97 && x[i] <= 122)) {
            if (x[i + 1] >= 48 && x[i + 1] <= 57) {
                int num = x[i + 1] - 48;
                for (int j = 0; j < num; j++) {
                    printf("%c", x[i]);
                    putc(x[i], outfile);
                }
            }
            else {
                printf("%c", x[i]);
                putc(x[i], outfile);
            }
        }
        // write out a single byte of data
    }

}



char* readFileData(char* filename, int* count_ptr)
{
    // Returns a pointer to an array storing the file data.
    // Sets the variable pointed to by 'count' to contain the file size.
    // Exits the program if the filename doesn't exist.
    FILE* infile = fopen(filename, "rb");
    if (!infile)
    {
        printf("No such file \"%s\"!\n", filename);
        exit(1);
    }
    // Get file size by going to the end of the file, getting the
    // position, and then going back to the start of the file.
    fseek(infile, 0, SEEK_END);
    int count = ftell(infile);
    fseek(infile, 0, SEEK_SET);
    // read the data from the file
    char* data = new char[count];
    fread(data, 1, count, infile);
    fclose(infile);
    *count_ptr = count;
    return data;
}
void usage() {
    printf("Error: no commandline arguments detected - showing help.\n");

    printf("rld(c) 2020, Garth Santor\n");
    printf("========================================================== =\n");
    printf("Version 1.0.0\n");

    printf("A run - length deocder utility.\n");

    printf("Usage: rld[-(b | t)][--help][--version] infile[outfile]\n");

    printf("Examples :\n");
    printf("rld myfile.doc.rlet\n");
    printf("decodes 'myfile.doc.rlet' to 'myfile.doc' (text mode)\n");

    printf(" rld myfile.doc.rleb\n");
    printf("decodes 'myfile.doc.rleb' to 'myfile.doc' (binary mode)\n");

    printf("rld - t myfile.rle myfile.doc\n");
    printf("decodes 'myfile.rle' to 'myfile.doc' (text mode)\n");

    printf(" rld - b myfile.rle ec.exe\n");
    printf("decodes 'myfile.rle' to 'ec.exe' (binary mode)\n");

    printf("rld --help\n");
    printf("displays the help\n");

    printf("rld --version\n");
    printf("displays version number in the format d.d.d");
    exit(1);
}