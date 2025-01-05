#include <stdio.h>
#include <stdint.h>

// r
int read_file_char();
int read_file_line();
int print_line();

// w
int write_file();
int overwrite_file();

// a
int append_file();


/**
 * File I/O
 *
 * FILE is basically a data type that holds information about a file
 */
int main() {

    // r
    // read_file_char();
    read_file_line();
    // print_line();

    // w
    // write_file();
    // overwrite_file();

    // a
    // append_file();

    return 0;
}

/**
 * Read the file using fgets (char by char)
 */
int read_file_char() {
    printf("Opening file...\n");
    FILE *file = fopen("test.txt", "r");  // r = read, w = write, a = append


    // Check if file is opened
    if(file == NULL) {
        printf("Error opening file\n");
        return 1;
    }


    // Write to file
    // fprintf(file, "Hello, World!\n");
    // fprintf(file, "Whats up Mooo!\n");
    while(!feof(file)) {
        char c = fgetc(file);
        printf("%c", c);
    }


    printf("\nClosing file...(read char by char)\n");
    fclose(file);

    return 0;
}

/**
 * Read the file using fgets (line by line)
 */
int read_file_line() {
    FILE *file;

    // Open a file in read mode
    file = fopen("test.txt", "r");

    // Store the content of the file
    char lines[100];

    // Read the content and print it
    while(fgets(lines, 100, file)) {
        printf("%s", lines);
    }

    // Close the file
    printf("\nClosing file...(read line by line)\n");
    fclose(file); 
    return 0;
}

/**
 * Write to the file
 * If the file does not exist, it will be created
 */
int write_file() {
    printf("Opening file...\n");

    FILE *file;
    // fopen(filename, mode);
    file = fopen("test.txt", "w");  // r = read, w = write, a = append


    // Check if file is opened
    if(file == NULL) {
        printf("Error opening file\n");
        return 1;
    }


    // Write to file
    fprintf(file, "Hello, World!\n");
    fprintf(file, "Whats up Mooo!\n");


    printf("Closing file...\n");
    fclose(file);

    return 0;
}

/**
 * Append to the end of the file
 * If the file does not exist, it will be created
 */
int append_file() {
    printf("Opening file...\n");
    FILE *file = fopen("test.txt", "a");  // r = read, w = write, a = append


    // Check if file is opened
    if(file == NULL) {
        printf("Error opening file\n");
        return 1;
    }


    // Write to file
    fprintf(file, "\n");
    fprintf(file, "10\n");
    fprintf(file, "W3SCHOOL\n");


    printf("Closing file...\n");
    fclose(file);

    return 0;
}

int overwrite_file() {
    printf("Opening file...\n");
    FILE *file = fopen("test.txt", "w");  // r = read, w = write, a = append


    // Check if file is opened
    if(file == NULL) {
        printf("Error opening file\n");
        return 1;
    }


    printf("Closing file...\n");
    fclose(file);

    return 0;
}

int print_line() {
    // Store the content of the file
    char line[255];
    FILE *file = fopen("test.txt", "r");

    // Check if file is opened
    if(file == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    // Read the content and store it inside 'line', finally increment pointer
    fgets(line, 255, file);

    // Print the content of the file
    printf("%s", line);

    // Read the second line
    // fgets(line, 255, file);
    // printf("%s", line);

    fclose(file);

    return 0;
}
