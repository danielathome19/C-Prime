#include <stdio.h>
#include "myclib.h"

int main() {
    printf("========== Start ==========\n");
    
    // Test for division by zero exception
    try {
        int x = 3;
        int y = 0;
        printf("%d\n", x / y);
        // throw(FLOATING_POINT_EXCEPTION);
    } catch (FLOATING_POINT_EXCEPTION) {
        printf("Division by zero exception\n");
    } etry;

    // Test for file not found exception
    try {
        FILE *file = fopen("file.txt", "r");
        if (file == NULL) throw(FILE_NOT_FOUND_EXCEPTION);
        printf("File opened successfully\n");
    } catch (FILE_NOT_FOUND_EXCEPTION) {
        printf("File not found exception\n");
    } etry;

    // Test for memory allocation exception
    try {
        int *arr = (int *)malloc(1000000000000000000 * sizeof(int));
        if (arr == NULL) throw(MEMORY_ALLOCATION_EXCEPTION);
        printf("Memory allocated successfully\n");
    } catch (MEMORY_ALLOCATION_EXCEPTION) {
        printf("Memory allocation exception\n");
    } finally {
        printf("Finally block in memory test\n");  
    } etry;
    
    // Test multiple catch blocks
    try {
        throw(FILE_NOT_FOUND_EXCEPTION);
        throw(FLOATING_POINT_EXCEPTION);
    } catch (FILE_NOT_FOUND_EXCEPTION) {
        printf("File not found exception\n");
    } catch (FLOATING_POINT_EXCEPTION) {
        printf("Division by zero exception\n");
    } etry;

    try {
        FileReader *fileReader = new_filereader("test.txt");
        printf("File opened successfully\n");
        while (fileReader->has_next(fileReader))
            printf("%s\n", fileReader->next_line(fileReader));
        close_filereader(fileReader);
        printf("Finished first read\n");

        FileReader *fileReader2 = new_filereader("test.txt");
        string first = fileReader2->next_line(fileReader2);
        int second = fileReader2->next_int(fileReader2);
        float third = fileReader2->next_float(fileReader2);
        printf("%s %d %f\n", first, second, third);

        string a = fileReader2->next_string(fileReader2);
        string b = fileReader2->next_string(fileReader2);
        string c = fileReader2->next_string(fileReader2);
        string d = fileReader2->next_string(fileReader2);
        string ef = fileReader2->next_line(fileReader2);
        printf("%s %s %s %s\n", a, b, c, d);
        printf("%s\n", ef);

        long l = fileReader2->next_long(fileReader2);
        printf("%ld\n", l);

        double db = fileReader2->next_double(fileReader2);
        printf("%lf\n", db);

        char ch = fileReader2->next_char(fileReader2);
        char ch2 = fileReader2->next_char(fileReader2);
        char ch3 = fileReader2->next_char(fileReader2);
        printf("%c %c%c\n", ch, ch2, ch3);
        close_filereader(fileReader2);

        FileWriter *fileWriter = new_filewriter("test2.txt", true);
        fileWriter->write_int(fileWriter, 123);
        fileWriter->write_float(fileWriter, 3.14);
        fileWriter->write_string(fileWriter, "Hello");
        fileWriter->write_string(fileWriter, "World");
        fileWriter->write_string(fileWriter, "!");
        fileWriter->write_line(fileWriter, "Hello, World!");
        fileWriter->write_long(fileWriter, 1234567890);
        fileWriter->write_double(fileWriter, 3.14159265359);
        fileWriter->write_char(fileWriter, 'a');
        fileWriter->write_char(fileWriter, 'b');
        fileWriter->write_char(fileWriter, 'c');
        close_filewriter_flush(fileWriter);
    } catch (FILE_NOT_FOUND_EXCEPTION) {
        printf("File not found exception in file reader\n");
    } etry;

    // Test substring function with one and two arguments
    string str = "Hello, World!";
    string hello = substr(str, 0, 5);
    string world = substr(str, 7);
    printf("%s %s\n", hello, world);

    int commaindex = strindex(str, ",");
    printf("%d\n", commaindex);

    int commaindex2 = strindex_char(str, ',');
    printf("%d\n", commaindex2);

    printf("========== Done ==========\n");
    return 0;
}