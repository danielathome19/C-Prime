#include <stdio.h>
#include "cprime.h"

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
        FileReader *fr = new_FileReader("test.txt");
        printf("File opened successfully\n");
        while (FileReader_hasNext(fr))
            printf("%s\n", FileReader_nextLine(fr));
        close_FileReader(fr);
        printf("Finished first read\n");

        FileReader *fr2 = new_FileReader("test.txt");
        string first = FileReader_nextLine(fr2);
        int second   = FileReader_nextInt(fr2);
        float third  = FileReader_nextFloat(fr2);
        printf("%s %d %f\n", first, second, third);

        string a  = FileReader_nextString(fr2);
        string b  = FileReader_nextString(fr2);
        string c  = FileReader_nextString(fr2);
        string d  = FileReader_nextString(fr2);
        string ef = FileReader_nextLine(fr2);
        printf("%s %s %s %s\n", a, b, c, d);
        printf("%s\n", ef);

        long l = FileReader_nextLong(fr2);
        printf("%ld\n", l);

        double db = FileReader_nextDouble(fr2);
        printf("%lf\n", db);

        char ch  = FileReader_nextChar(fr2);
        char ch2 = FileReader_nextChar(fr2);
        char ch3 = FileReader_nextChar(fr2);
        printf("%c %c%c\n", ch, ch2, ch3);
        close_FileReader(fr2);
    } catch (FILE_NOT_FOUND_EXCEPTION) {
        printf("File not found exception in file reader\n");
    } etry;

    try {
        FileWriter *fw = new_FileWriter("test2.txt", true);
        FileWriter_writeInt(fw, 123);
        FileWriter_writeFloat(fw, 3.14);
        FileWriter_writeString(fw, "Hello");
        FileWriter_writeString(fw, "World");
        FileWriter_writeString(fw, "!");
        FileWriter_writeLine(fw, "Hello, World!");
        FileWriter_writeLong(fw, 1234567890);
        FileWriter_writeDouble(fw, 3.14159265359);
        FileWriter_writeChar(fw, 'a');
        FileWriter_writeChar(fw, 'b');
        FileWriter_writeChar(fw, 'c');
        close_FileWriter(fw);
    } catch (FILE_NOT_FOUND_EXCEPTION) {
        printf("File not found exception in file writer\n");
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

    // Test user input and type casting
    int n = get_int("Enter an integer: ");
    float f = get_float("Enter a float: ");
    printf("Integer: %d, Float: %f\n", n, f);

    var n2 = input(int, "Enter an int: ");
    printf("Integer 2: %d\n", n2);

    // Test verbal logical operators (from iso646.h)
    bool testbool = 1 and 3;
    while (testbool or false) {
        printf("Test\n");
        testbool = false;
    }
    
    // Test foreach macro with implicit and explicit length
    int arr[] = {1, 2, 3, 4, 5};
    foreach (int, x, arr) {
        printf("%d ", *x);  // Dereference the pointer
    }
    printf("\n");
    printf("Length of arr: %d\n", arrlen(arr));

    char* words = "Hello, World!";
    foreach (char, c, words, 5) {
        printf("%c ", *c);
    }
    printf("\n");

    fori (i, 0, 10, 2) {
        printf("%d ", i);
    }
    printf("\n");

    fori (i, 1, 5) {
        printf("%d ", i);
    }
    printfn("");

    fori (i, 10) printf("%d ", i);
    printfn("");

    // Test autofree attribute (freed at the end of scope)
    {
    autofree string str2 = (string)malloc(10);
    strcpy(str2, "Hello");
    printfn("%s", str2);
    }

    // Test until, unless, and repeat
    int i = 0;
    until (i == 5) {
        printf("%d ", i);
        i++;
    }

    do {
        printf("%d ", i);
        i++;
    } until (i == 10);

    unless (i == 10) {
        printf("Not 10\n");
    } else {
        printf("10\n");
    }

    repeat (5) {
        printf("%d ", _i);
    }
    printfn("");

    printf("========== Done ==========\n");
    return 0;
}