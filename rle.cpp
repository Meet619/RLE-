#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<ctype.h>
#include <string>
#include <sstream>
#pragma warning(disable : 4996)
void compress(char* data, int count, FILE* outfile);
void decompress(char* data, int count, FILE* outfile);
char* readFileData(char* filename, int* count_ptr);
void usage();
int main(int num_args, char* arg_values[])
{
    bool var;
    bool outfilecheck=false;
    
   // printf("fourth var ", arg_values[3]);
    if (num_args > 4 || num_args == 1) {
        usage();
    }

    else {
        if (num_args == 2) {
            if (strcmp(arg_values[1], "--help") == 0) {
                usage();
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
            if (!outfilecheck) {
                strcpy(output_filename, input_filename);
            }
            else {
                char* output = arg_values[3];
                strcpy(output_filename, output);
            }
            
            if (var) {
                strcat(output_filename, ".rleb");
                printf("Compressing %s to %s\n", input_filename, output_filename);
                outfile = fopen(output_filename, "wb");
                compress(data, count, outfile);
            }
            else {
                strcat(output_filename, ".rlet");
                printf("Compressing %s to %s\n", input_filename, output_filename);
                outfile = fopen(output_filename, "w");
                compress(data, count, outfile);
            }
            
        
        // Close the output file to ensure data is saved.
        fclose(outfile);
        // Free the array we allocated
        delete data;
        return 0;
    }
}
void compress(char* data, int count, FILE* outfile)
{
    // TODO: compress the data instead of just writing it out to the file
    for (int i = 0; i < count; ++i)
    {
        int x = 1;
        while (i < count - 1 && data[i] == data[i + 1]) {
            x++;
            i++;
        }
        printf("%c", data[i]);
        char t = x + '0';
        printf("%c", t);

        putc(data[i], outfile); // write out a single byte of data
        putc(t, outfile);
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

    printf("rle(c) 2020, Garth Santor\n");
    printf("========================================================== =\n");
    printf("Version 1.0.0\n");

    printf("A run - length encoder utility.\n");

    printf("Usage: rle[-(b | t)][--help][--version] infile[outfile]\n");

    printf("Examples :\n");
    printf("rle myfile.doc\n");
    printf("encodes 'myfile.doc' to 'myfile.doc.rlet' (text mode)\n");

    printf("rle - b ..\myfile.doc\n");
    printf("encodes 'myfile.doc' from the parent folder to 'myfile.doc.rleb' (binary mode) in the current folder\n");


    printf("rle - t myfile.doc ec.text\n");
    printf("encodes 'myfile.doc' from the parent folder to 'ec.text' in the local folder(text mode)\n");

    printf("rle - b myfile.doc ec.bin\n");
    printf("encodes 'myfile.doc' from the parent folder to 'ec.bin' in the local folder(binary mode)\n");

    printf("rle --help\n");
    printf("displays the help\n");

    printf("rle --version\n");
    printf("displays version number in the format d.d.d");
    exit(1);
}