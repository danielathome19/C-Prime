/**
 * @file cprime.h
 * 
 * TBD
 * 
 * @author Daniel J. Szelogowski
 * @date September 2024
 * @license: TBD
 * 
 */
#ifndef _CPRIME_H_
#define _CPRIME_H_

#include <stdio.h>
#include <setjmp.h>
#include <float.h>
#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <iso646.h>


/* Macros for simulating function overloading; based on the max number of parameters */
#define GET_MACRO2(_1, _2, NAME, ...) NAME
#define GET_MACRO3(_1, _2, _3, NAME, ...) NAME
#define GET_MACRO4(_1, _2, _3, _4, NAME, ...) NAME
#define GET_MACRO5(_1, _2, _3, _4, _5, NAME, ...) NAME


/* Exception handling */
static jmp_buf ex_buf__;
#define try do { switch( setjmp(ex_buf__) ) { case 0: while(1) {
#define catch(x) break; case x:
#define finally break; } default: {
#define etry break; } } }while(0)
#define throw(x) longjmp(ex_buf__, x)


/* Exception codes */
#define SUCCESS (0)
#define FAILURE (1)
#define ERROR (2)
#define EXCEPTION (3)
#define MEMORY_ALLOCATION_EXCEPTION (10)
#define FILE_NOT_FOUND_EXCEPTION (11)
#define FLOATING_POINT_EXCEPTION (12)
#define NULL_POINTER_EXCEPTION (13)
#define OUT_OF_BOUNDS_EXCEPTION (14)
#define ILLEGAL_ARGUMENT_EXCEPTION (15)
#define NETWORK_ERROR_EXCEPTION (16)
#define TIMEOUT_EXCEPTION (17)
#define DATABASE_CONNECTION_EXCEPTION (18)
#define INVALID_FILE_FORMAT_EXCEPTION (19)
#define PARSING_ERROR_EXCEPTION (20)
#define INVALID_INPUT_EXCEPTION (21)
#define INVALID_OPERATION_EXCEPTION (22)
#define INVALID_STATE_EXCEPTION (23)
#define INVALID_FORMAT_EXCEPTION (24)
#define INVALID_KEY_EXCEPTION (25)
#define INVALID_VALUE_EXCEPTION (26)
#define INVALID_INDEX_EXCEPTION (27)
#define INVALID_SIZE_EXCEPTION (28)
#define INVALID_LENGTH_EXCEPTION (29)
#define INVALID_RANGE_EXCEPTION (30)
#define INVALID_ARGUMENT_EXCEPTION (31)
#define INVALID_EXPRESSION_EXCEPTION (32)
#define PIPE_ERROR_EXCEPTION (33)
#define BUS_ERROR_EXCEPTION (34)
#define QUIT_EXCEPTION (35)
#define HANGUP_EXCEPTION (36)
#define IO_ERROR_EXCEPTION (33)
#define EOF_EXCEPTION (34)
#define NOT_FOUND_EXCEPTION (35)


/* Exception signals */
// Throw floating-point exception for sigfpe (floating-point exception signal)
static void __handle_sigfpe(int sig) { if (sig == SIGFPE) throw(FLOATING_POINT_EXCEPTION); }

// Throw null pointer exception (segmentation fault)
static void __handle_sigsegv(int sig) { if (sig == SIGSEGV) throw(NULL_POINTER_EXCEPTION); }

// Throw memory allocation failure exception for sigabrt (abort signal)
static void __handle_sigabrt(int sig) { if (sig == SIGABRT) throw(MEMORY_ALLOCATION_EXCEPTION); }

// Throw illegal argument exception for sigill (illegal instruction signal)
static void __handle_sigill(int sig) { if (sig == SIGILL) throw(ILLEGAL_ARGUMENT_EXCEPTION); }

// Throw timeout exception for sigterm (termination signal)
static void __handle_sigterm(int sig) { if (sig == SIGTERM) throw(TIMEOUT_EXCEPTION); }

// Throw timeout exception for sigint (interrupt signal)
static void __handle_sigint(int sig) { if (sig == SIGINT) throw(TIMEOUT_EXCEPTION); }

#ifdef __unix__  // Or __APPLE__ for macOS, __linux__ for Linux
    #ifdef SIGBUS
        // Throw hangup exception for sighup (hangup signal)
        static void __handle_sigbus(int sig) { if (sig == SIGBUS) throw(BUS_ERROR_EXCEPTION); }
    #endif 

    #ifdef SIGPIPE
        // Throw pipe error exception for sigpipe (broken pipe signal)
        static void __handle_sigpipe(int sig) { if (sig == SIGPIPE) throw(PIPE_ERROR_EXCEPTION); }
    #endif

    #ifdef SIGHUP
        // Throw hangup exception for sighup (hangup signal)
        static void __handle_sighup(int sig) { if (sig == SIGHUP) throw(HANGUP_EXCEPTION); }
    #endif

    #ifdef SIGQUIT
        // Throw quit exception for sigquit (quit signal)
        static void __handle_sigquit(int sig) { if (sig == SIGQUIT) throw(QUIT_EXCEPTION); }
    #endif
#endif  // __unix__


/* Register signal handlers */
static void __setup_signal_handlers(void) {
    signal(SIGFPE,  __handle_sigfpe);
    signal(SIGSEGV, __handle_sigsegv);
    signal(SIGABRT, __handle_sigabrt);
    signal(SIGILL,  __handle_sigill);
    signal(SIGTERM, __handle_sigterm);
    signal(SIGINT,  __handle_sigint);
    #ifdef __unix__
        #ifdef SIGBUS
            signal(SIGBUS,  __handle_sigbus);
        #endif
        #ifdef SIGPIPE
            signal(SIGPIPE, __handle_sigpipe);
        #endif
        #ifdef SIGHUP
            signal(SIGHUP,  __handle_sighup);
        #endif
        #ifdef SIGQUIT
            signal(SIGQUIT, __handle_sigquit);
        #endif
    #endif  // __unix__
}


/* Boolean definitions */
#if (__STDC_VERSION__ >= 199901L && __STDC_VERSION__ < 202000L)
    #include <stdbool.h>
#elif __STDC_VERSION__ < 199901L
    #define true 1
    #define false 0
    typedef int _Boolean;
    #define bool _Boolean
#endif


/* Auto-type definitions */
#if defined(__GNUC__) || defined(__GNUG__)
    #define var __auto_type
#elif __STDC_VERSION__ > 201710L || defined(__cplusplus)
    #define var auto
#endif


/* Type definitions */
typedef char* string;
typedef char byte;
typedef char* bytes;
typedef void* any;


/* Input functions */
int get_int(const char* format, ...) __attribute__((format(printf, 1, 2)));
float get_float(const char* format, ...) __attribute__((format(printf, 1, 2)));
double get_double(const char* format, ...) __attribute__((format(printf, 1, 2)));
long get_long(const char* format, ...) __attribute__((format(printf, 1, 2)));
long long get_long_long(const char* format, ...) __attribute__((format(printf, 1, 2)));
char get_char(const char* format, ...) __attribute__((format(printf, 1, 2)));
string get_string(va_list* args, const char* format, ...) __attribute__((format(printf, 2, 3)));
#define get_string(...) get_string(NULL, __VA_ARGS__)


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"

static size_t allocations = 0;
static string* strings = NULL;

#undef get_string
string get_string(va_list* args, const char* format, ...) {
    if (allocations == SIZE_MAX / sizeof (string))
        return NULL;
    string buffer = NULL;
    size_t capacity = 0;
    size_t size = 0;
    int c;
    
    if (format != NULL) {
        va_list ap;
        if (args == NULL)
            va_start(ap, format);
        else
            va_copy(ap, *args);
        vprintf(format, ap);
        va_end(ap);
    }

    while ((c = fgetc(stdin)) != '\r' && c != '\n' && c != EOF) {
        if (size + 1 > capacity) {
            if (capacity < SIZE_MAX) {
                capacity++;
            } else {
                free(buffer);
                return NULL;
            }
            string temp = (string) realloc(buffer, capacity);
            if (temp == NULL) {
                free(buffer);
                return NULL;
            }
            buffer = temp;
        }
        buffer[size++] = c;
    }

    if (size == 0 && c == EOF)
        return NULL;
    
    if (size == SIZE_MAX) {
        free(buffer);
        return NULL;
    }

    if (c == '\r' && (c = fgetc(stdin)) != '\n') {
        if (c != EOF && ungetc(c, stdin) == EOF) {
            free(buffer);
            return NULL;
        }
    }

    string s = (string) realloc(buffer, size + 1);
    if (s == NULL) {
        free(buffer);
        return NULL;
    }
    
    s[size] = '\0';
    
    string* tmp = (string*) realloc(strings, sizeof (string)* (allocations + 1));
    if (tmp == NULL) {
        free(s);
        return NULL;
    }
    strings = tmp;

    strings[allocations] = s;
    allocations++;

    return s;
}

char get_char(const char* format, ...) {
    va_list ap;
    va_start(ap, format);

    while (true) {
        string line = get_string(&ap, format);
        if (line == NULL) {
            va_end(ap);
            return CHAR_MAX;
        }
        
        char c, d;
        if (sscanf(line, "%c%c", &c, &d) == 1) {
            va_end(ap);
            return c;
        }
    }
}

double get_double(const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    
    while (true) {
        string line = get_string(&ap, format);
        if (line == NULL) {
            va_end(ap);
            return DBL_MAX;
        }

        if (strlen(line) > 0 && !isspace((unsigned char) line[0])) {
            char* tail;
            errno = 0;
            double d = strtod(line, &tail);
            if (errno == 0 && *tail == '\0' && isfinite(d) != 0 && d < DBL_MAX) {
                if (strcspn(line, "XxEePp") == strlen(line)) {
                    va_end(ap);
                    return d;
                }
            }
        }
    }
}

float get_float(const char* format, ...) {
    va_list ap;
    va_start(ap, format);

    while (true) {
        string line = get_string(&ap, format);
        if (line == NULL) {
            va_end(ap);
            return FLT_MAX;
        }

        if (strlen(line) > 0 && !isspace((unsigned char) line[0])) {
            char* tail;
            errno = 0;
            float f = strtof(line, &tail);
            if (errno == 0 && *tail == '\0' && isfinite(f) != 0 && f < FLT_MAX) {
                if (strcspn(line, "XxEePp") == strlen(line)) {
                    va_end(ap);
                    return f;
                }
            }
        }
    }
}

int get_int(const char* format, ...) {
    va_list ap;
    va_start(ap, format);

    while (true) {
        string line = get_string(&ap, format);
        if (line == NULL) {
            va_end(ap);
            return INT_MAX;
        }

        if (strlen(line) > 0 && !isspace((unsigned char) line[0])) {
            char* tail;
            errno = 0;
            long n = strtol(line, &tail, 10);
            if (errno == 0 && *tail == '\0' && n >= INT_MIN && n < INT_MAX) {
                va_end(ap);
                return n;
            }
        }
    }
}

long get_long(const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    
    while (true) {
        string line = get_string(&ap, format);
        if (line == NULL) {
            va_end(ap);
            return LONG_MAX;
        }
        
        if (strlen(line) > 0 && !isspace((unsigned char) line[0])) {
            char* tail;
            errno = 0;
            long n = strtol(line, &tail, 10);
            if (errno == 0 && *tail == '\0' && n < LONG_MAX) {
                va_end(ap);
                return n;
            }
        }
    }
}

long long get_long_long(const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    while (true) {
        string line = get_string(&ap, format);
        if (line == NULL) {
            va_end(ap);
            return LLONG_MAX;
        }

        if (strlen(line) > 0 && !isspace((unsigned char) line[0])) {
            char* tail;
            errno = 0;
            long long n = strtoll(line, &tail, 10);
            if (errno == 0 && *tail == '\0' && n < LLONG_MAX) {
                va_end(ap);
                return n;
            }
        }
    }
}


/* Free allocated memory from user-input strings */
static void __teardown(void) {
    if (strings != NULL) {
        for (size_t i = 0; i < allocations; i++)
            free(strings[i]);
        free(strings);
    }
}


/* Initialization function */
#if defined (_MSC_VER) 
    #pragma section(".CRT$XCU",read)
    #define INITIALIZER_(FUNC,PREFIX) \
        static void FUNC(void); \
        __declspec(allocate(".CRT$XCU")) void (*FUNC##_)(void) = FUNC; \
        __pragma(comment(linker,"/include:" PREFIX #FUNC "_")) \
        static void FUNC(void)
    #ifdef _WIN64
        #define INITIALIZER(FUNC) INITIALIZER_(FUNC,"")
    #else
        #define INITIALIZER(FUNC) INITIALIZER_(FUNC,"_")
    #endif
#elif defined (__GNUC__) 
    #define INITIALIZER(FUNC) \
        static void FUNC(void) __attribute__((constructor)); \
        static void FUNC(void)
#else
    #error Some compiler-specific features missing.
#endif

INITIALIZER(setup) {
    setvbuf(stdout, NULL, _IONBF, 0);
    __setup_signal_handlers();
    atexit(__teardown);
}


/* File Reader */
typedef struct FileReader FileReader;
struct FileReader {
    FILE* file;
    string buffer;
    size_t capacity;
    size_t size;
    // string (*nextLine)(FileReader*);
    // string (*nextString)(FileReader*);
    // char (*nextChar)(FileReader*);
    // int (*nextInt)(FileReader*);
    // long (*nextLong)(FileReader*);
    // float (*nextFloat)(FileReader*);
    // double (*nextDouble)(FileReader*);
    // bool (*hasNext)(FileReader*);
};

string FileReader_nextLine(FileReader* filereader) {
    if (filereader == NULL || filereader->file == NULL)
        return NULL;
    if (filereader->buffer != NULL) {
        free(filereader->buffer);
        filereader->buffer = NULL;
    }
    size_t capacity = 0;
    size_t size = 0;
    int c;
    while ((c = fgetc(filereader->file)) != '\r' && c != '\n' && c != EOF) {
        if (size + 1 > capacity) {
            capacity = (capacity == 0) ? 16 : capacity * 2;
            string temp = (string) realloc(filereader->buffer, capacity);
            if (temp == NULL) return NULL;
            filereader->buffer = temp;
        }
        filereader->buffer[size++] = c;
    }
    if (size == 0 && c == EOF)
        return NULL;
    if (c == '\r' && (c = fgetc(filereader->file)) != '\n')
        if (c != EOF && ungetc(c, filereader->file) == EOF)
            return NULL;
    string s = (string) malloc(size + 1);
    if (s == NULL) return NULL;
    strncpy(s, filereader->buffer, size);
    s[size] = '\0';
    return s;
}

string FileReader_nextString(FileReader* filereader) {
    if (filereader == NULL || filereader->file == NULL)
        return NULL;
    if (filereader->buffer != NULL) {
        free(filereader->buffer);
        filereader->buffer = NULL;
    }
    size_t capacity = 16;
    size_t size = 0;
    int c;
    do {
        c = fgetc(filereader->file);
    } while (c == ' ' || c == '\n' || c == '\r');
    if (c == EOF)
        return NULL;
    filereader->buffer = (string)malloc(capacity);
    if (filereader->buffer == NULL)
        return NULL;
    while (c != ' ' && c != '\n' && c != '\r' && c != EOF) {
        if (size + 1 > capacity) {
            capacity *= 2;
            string temp = (string)realloc(filereader->buffer, capacity);
            if (temp == NULL) {
                free(filereader->buffer);
                return NULL;
            }
            filereader->buffer = temp;
        }
        filereader->buffer[size++] = c;
        c = fgetc(filereader->file);
    }
    filereader->buffer[size] = '\0';
    if (c == '\r') {
        c = fgetc(filereader->file);
        if (c != '\n') ungetc(c, filereader->file);
    }
    return filereader->buffer;
}

char FileReader_nextChar(FileReader* filereader) {
    if (filereader == NULL || filereader->file == NULL)
        return CHAR_MAX;
    int c;
    do {
        c = fgetc(filereader->file);
    } while (isspace(c));
    if (c == EOF) return CHAR_MAX;
    return (char)c;
}

int FileReader_nextInt(FileReader* filereader) {
    if (filereader == NULL || filereader->file == NULL)
        return INT_MAX;
    string line = FileReader_nextString(filereader);
    if (line == NULL)
        return INT_MAX;
    if (strlen(line) > 0 && !isspace((unsigned char) line[0])) {
        char* tail;
        errno = 0;
        long n = strtol(line, &tail, 10);
        if (errno == 0 && *tail == '\0' && n >= INT_MIN && n < INT_MAX)
            return n;
    }
    return INT_MAX;
}

long FileReader_nextLong(FileReader* filereader) {
    if (filereader == NULL || filereader->file == NULL)
        return LONG_MAX;
    string line = FileReader_nextString(filereader);
    if (line == NULL)
        return LONG_MAX;
    if (strlen(line) > 0 && !isspace((unsigned char) line[0])) {
        char* tail;
        errno = 0;
        long n = strtol(line, &tail, 10);
        if (errno == 0 && *tail == '\0' && n < LONG_MAX)
            return n;
    }
    return LONG_MAX;
}

float FileReader_nextFloat(FileReader* filereader) {
    if (filereader == NULL || filereader->file == NULL)
        return FLT_MAX;
    string line = FileReader_nextString(filereader);
    if (line == NULL)
        return FLT_MAX;
    if (strlen(line) > 0 && !isspace((unsigned char) line[0])) {
        char* tail;
        errno = 0;
        float f = strtof(line, &tail);
        if (errno == 0 && *tail == '\0' && isfinite(f) != 0 && f < FLT_MAX)
            return f;
    }
    return FLT_MAX;
}

double FileReader_nextDouble(FileReader* filereader) {
    if (filereader == NULL || filereader->file == NULL)
        return DBL_MAX;
    string line = FileReader_nextString(filereader);
    if (line == NULL)
        return DBL_MAX;
    if (strlen(line) > 0 && !isspace((unsigned char) line[0])) {
        char* tail;
        errno = 0;
        double d = strtod(line, &tail);
        if (errno == 0 && *tail == '\0' && isfinite(d) != 0 && d < DBL_MAX)
            return d;
    }
    return DBL_MAX;
}

bool FileReader_hasNext(FileReader* filereader) {
    if (filereader == NULL || filereader->file == NULL)
        return false;
    int c = fgetc(filereader->file);
    if (c != EOF && ungetc(c, filereader->file) == EOF)
        return false;
    return c != EOF;
}

FileReader* new_FileReader(const char* filename) {
    if (filename == NULL) {
        throw(ILLEGAL_ARGUMENT_EXCEPTION);
        return NULL;
    }
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        throw(FILE_NOT_FOUND_EXCEPTION);
        return NULL;
    }
    FileReader* filereader = (FileReader*) malloc(sizeof (FileReader));
    if (filereader == NULL) {
        fclose(file);
        throw(MEMORY_ALLOCATION_EXCEPTION);
        return NULL;
    }
    filereader->file = file;
    filereader->buffer = NULL;
    filereader->capacity = 0;
    filereader->size = 0;
    // filereader->nextLine = FileReader_nextLine;
    // filereader->nextString = FileReader_nextString;
    // filereader->nextChar = FileReader_nextChar;
    // filereader->nextInt = FileReader_nextInt;
    // filereader->nextLong = FileReader_nextLong;
    // filereader->nextFloat = FileReader_nextFloat;
    // filereader->nextDouble = FileReader_nextDouble;
    // filereader->hasNext = FileReader_hasNext;
    return filereader;
}

void close_FileReader(FileReader* filereader) {
    if (filereader != NULL) {
        if (filereader->file != NULL)
            fclose(filereader->file);
        if (filereader->buffer != NULL)
            free(filereader->buffer);
        free(filereader);
    }
}


/* File Writer */
/**
 * @brief File writer structure; writes to a file using various data types.
 * 
 * @note Instantiate using new_FileWriter(filename, appendMode=false)
 * @note Close using close_FileWriter(filewriter, flush=true) where `flush` is a line break
 * 
 * @param file The file to write to
 * 
 * @throw FILE_NOT_FOUND_EXCEPTION if the file is not found
 * @throw MEMORY_ALLOCATION_EXCEPTION if memory allocation fails
 * @throw ILLEGAL_ARGUMENT_EXCEPTION if the filename is NULL
 * 
 * ### Methods
 * 
 * - writeLine(FileWriter*, const char*)
 * 
 * - writeString(FileWriter*, const char*)
 * 
 * - writeChar(FileWriter*, char)
 * 
 * - writeInt(FileWriter*, int)
 * 
 * - writeLong(FileWriter*, long)
 * 
 * - writeFloat(FileWriter*, float)
 * 
 * - writeDouble(FileWriter*, double)
 */
typedef struct FileWriter FileWriter;
struct FileWriter {
    FILE* file;
    // void (*writeLine)(FileWriter*, const char*);
    // void (*writeString)(FileWriter*, const char*);
    // void (*writeChar)(FileWriter*, char);
    // void (*writeInt)(FileWriter*, int);
    // void (*writeLong)(FileWriter*, long);
    // void (*writeFloat)(FileWriter*, float);
    // void (*writeDouble)(FileWriter*, double);
};

/**
 * @brief Write a line to the file
 * 
 * @param filewriter The file writer to write to
 * @param line The line to write (with a line break)
 * 
 * @throw ILLEGAL_ARGUMENT_EXCEPTION if the file writer or line is NULL
 * 
 * @memberof FileWriter
 */
void FileWriter_writeLine(FileWriter* filewriter, const char* line) {
    if (filewriter == NULL || filewriter->file == NULL || line == NULL) return;
    fprintf(filewriter->file, "%s\n", line);
}

void FileWriter_writeString(FileWriter* filewriter, const char* s) {
    if (filewriter == NULL || filewriter->file == NULL || s == NULL) return;
    fprintf(filewriter->file, "%s", s);
}

void FileWriter_writeChar(FileWriter* filewriter, char c) {
    if (filewriter == NULL || filewriter->file == NULL) return;
    fprintf(filewriter->file, "%c", c);
}

void FileWriter_writeInt(FileWriter* filewriter, int n) {
    if (filewriter == NULL || filewriter->file == NULL) return;
    fprintf(filewriter->file, "%d", n);
}

void FileWriter_writeLong(FileWriter* filewriter, long n) {
    if (filewriter == NULL || filewriter->file == NULL) return;
    fprintf(filewriter->file, "%ld", n);
}

void FileWriter_writeFloat(FileWriter* filewriter, float f) {
    if (filewriter == NULL || filewriter->file == NULL) return;
    fprintf(filewriter->file, "%f", f);
}

void FileWriter_writeDouble(FileWriter* filewriter, double d) {
    if (filewriter == NULL || filewriter->file == NULL) return;
    fprintf(filewriter->file, "%lf", d);
}

FileWriter* __new_FileWriter_WA(const char* filename, bool append) {
    if (filename == NULL) {
        throw(ILLEGAL_ARGUMENT_EXCEPTION);
        return NULL;
    }
    FILE* file = fopen(filename, (append) ? "a" : "w");
    if (file == NULL) {
        throw(FILE_NOT_FOUND_EXCEPTION);
        return NULL;
    }
    FileWriter* filewriter = (FileWriter*) malloc(sizeof (FileWriter));
    if (filewriter == NULL) {
        fclose(file);
        throw(MEMORY_ALLOCATION_EXCEPTION);
        return NULL;
    }
    filewriter->file = file;
    // filewriter->writeLine = FileWriter_writeLine;
    // filewriter->writeString = FileWriter_writeString;
    // filewriter->writeChar = FileWriter_writeChar;
    // filewriter->writeInt = FileWriter_writeInt;
    // filewriter->writeLong = FileWriter_writeLong;
    // filewriter->writeFloat = FileWriter_writeFloat;
    // filewriter->writeDouble = FileWriter_writeDouble;
    return filewriter;
}
FileWriter* new_FileWriter_W(const char* filename) { return __new_FileWriter_WA(filename, false); }
FileWriter* new_FileWriter_A(const char* filename) { return __new_FileWriter_WA(filename, true); }
#define new_FileWriter(...) GET_MACRO2(__VA_ARGS__, __new_FileWriter_WA, new_FileWriter_W)(__VA_ARGS__)

void __close_FileWriter(FileWriter* filewriter, bool flush) {
    if (flush) FileWriter_writeChar(filewriter, '\n');
    if (filewriter != NULL) {
        if (filewriter->file != NULL)
            fclose(filewriter->file);
        free(filewriter);
    }
}
void close_FileWriter_flush(FileWriter* filewriter)   { __close_FileWriter(filewriter, true); }
void close_FileWriter_noflush(FileWriter* filewriter) { __close_FileWriter(filewriter, false); }
#define close_FileWriter(...) GET_MACRO2(__VA_ARGS__, __close_FileWriter, close_FileWriter_flush)(__VA_ARGS__)


/* String functions */
/**
 * @brief Get a substring of a string from the starting index to the ending index
 * 
 * @param str The string to get the substring from
 * @param start The starting index of the substring
 * @param end The ending index of the substring
 * 
 * @return The substring of the string
 * @throw OUT_OF_BOUNDS_EXCEPTION if the starting index is out of bounds
 */
string substr_end(string str, int start, int end) {
    if (str == NULL) return NULL;
    int len = strlen(str);
    if (start < 0 || start >= len || end < 0 || end > len) throw(OUT_OF_BOUNDS_EXCEPTION);
    if (start >= end) return NULL;
    string sub = (string) malloc(end - start + 1);
    if (sub == NULL) return NULL;
    strncpy(sub, str + start, end - start);
    sub[end - start] = '\0';
    return sub;
}

/**
 * @brief Get a substring of a string from the starting index to the end of the string
 * 
 * @param str The string to get the substring from
 * @param start The starting index of the substring
 * 
 * @return The substring of the string
 * @throw OUT_OF_BOUNDS_EXCEPTION if the starting index is out of bounds
 */
string substr_len(string str, int start) {
    if (str == NULL) return NULL;
    int len = strlen(str);
    return substr_end(str, start, len);
}

/**
 * @brief Get a substring of a string
 * 
 * @param str The string to get the substring from
 * @param start The starting index of the substring
 * @param end The ending index of the substring (optional; default is end of string)
 * 
 * @return The substring of the string
 */
#define substr(...) GET_MACRO3(__VA_ARGS__, substr_end, substr_len)(__VA_ARGS__)

/**
 * @brief Find the index of a substring in a string
 * 
 * @param str The string to search
 * @param substr The substring to find
 * 
 * @return The index of the substring in the string, or -1 if not found
 */
int strindex(string str, string substr) {
    if (str == NULL || substr == NULL) return -1;
    string found = strstr(str, substr);
    if (found == NULL) return -1;
    return found - str;
}

/**
 * @brief Find the index of a character in a string
 * 
 * @param str The string to search
 * @param c The character to find
 * 
 * @return The index of the character in the string, or -1 if not found
 */
int strindex_char(string str, char c) {
    if (str == NULL) return -1;
    string found = strchr(str, c);
    if (found == NULL) return -1;
    return found - str;
}

/**
 * @brief Convert a string to uppercase
 * 
 * @param str The string to convert to uppercase
 * 
 * @return The uppercase string
 */
string strtoupper(string str) {
    if (str == NULL) return NULL;
    int len = strlen(str);
    string upper = (string) malloc(len + 1);
    if (upper == NULL) return NULL;
    for (int i = 0; i < len; i++)
        upper[i] = toupper(str[i]);
    upper[len] = '\0';
    return upper;
}

/**
 * @brief Convert a string to lowercase
 * 
 * @param str The string to convert to lowercase
 * 
 * @return The lowercase string
 */
string strtolower(string str) {
    if (str == NULL) return NULL;
    int len = strlen(str);
    string lower = (string) malloc(len + 1);
    if (lower == NULL) return NULL;
    for (int i = 0; i < len; i++)
        lower[i] = tolower(str[i]);
    lower[len] = '\0';
    return lower;
}


/* For macros */
#define arrlen(array) (sizeof(array) / sizeof(array[0]))
#define __foreach_3(type, var, arr) \
    for (type* var = (arr); var < (arr) + arrlen(arr); ++var)
#define __foreach_4(type, var, arr, len) \
    for (type* var = (arr); var < (arr) + (len); ++var)

/**
 * @brief Foreach loop macro with implicit or explicit length
 * 
 * @param ... Arguments for the foreach loop (type, variable, array, length [optional; required for manually allocated arrays])
 */
#define foreach(...) GET_MACRO4(__VA_ARGS__, __foreach_4, __foreach_3)(__VA_ARGS__)

#define __fori_2(var, stop) \
    for (int var = 0; var < (stop); ++var)
#define __fori_3(var, start, stop) \
    for (int var = (start); var < (stop); ++var)
#define __fori_4(var, start, stop, step) \
    for (int var = (start); ((step) > 0) ? (var < (stop)) : (var > (stop)); var += (step))

/**
 * @brief For loop macro with implicit or explicit start, stop, and step
 * 
 * @param ... Arguments for the for loop (loop variable, start=0, stop, step=1)
 *
 * @code 
 * fori (i, 0, 10, 2) { printf("%d ", i); }  // 0 2 4 6 8
 * @endcode
 */
#define fori(...) GET_MACRO4(__VA_ARGS__, __fori_4, __fori_3, __fori_2)(__VA_ARGS__)


#pragma GCC diagnostic pop

#endif  // _CPRIME_H_