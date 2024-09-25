#ifndef _MYCLIB_H_
#define _MYCLIB_H_

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
static void handle_sigfpe(int sig) { if (sig == SIGFPE) throw(FLOATING_POINT_EXCEPTION); }

// Throw null pointer exception (segmentation fault)
static void handle_sigsegv(int sig) { if (sig == SIGSEGV) throw(NULL_POINTER_EXCEPTION); }

// Throw memory allocation failure exception for sigabrt (abort signal)
static void handle_sigabrt(int sig) { if (sig == SIGABRT) throw(MEMORY_ALLOCATION_EXCEPTION); }

// Throw illegal argument exception for sigill (illegal instruction signal)
static void handle_sigill(int sig) { if (sig == SIGILL) throw(ILLEGAL_ARGUMENT_EXCEPTION); }

// Throw timeout exception for sigterm (termination signal)
static void handle_sigterm(int sig) { if (sig == SIGTERM) throw(TIMEOUT_EXCEPTION); }

// Throw timeout exception for sigint (interrupt signal)
static void handle_sigint(int sig) { if (sig == SIGINT) throw(TIMEOUT_EXCEPTION); }

#ifdef __unix__  // Or __APPLE__ for macOS, __linux__ for Linux
    #ifdef SIGBUS
        // Throw hangup exception for sighup (hangup signal)
        static void handle_sigbus(int sig) { if (sig == SIGBUS) throw(BUS_ERROR_EXCEPTION); }
    #endif 

    #ifdef SIGPIPE
        // Throw pipe error exception for sigpipe (broken pipe signal)
        static void handle_sigpipe(int sig) { if (sig == SIGPIPE) throw(PIPE_ERROR_EXCEPTION); }
    #endif

    #ifdef SIGHUP
        // Throw hangup exception for sighup (hangup signal)
        static void handle_sighup(int sig) { if (sig == SIGHUP) throw(HANGUP_EXCEPTION); }
    #endif

    #ifdef SIGQUIT
        // Throw quit exception for sigquit (quit signal)
        static void handle_sigquit(int sig) { if (sig == SIGQUIT) throw(QUIT_EXCEPTION); }
    #endif
#endif  // __unix__


/* Register signal handlers */
static void setup_signal_handlers(void) {
    signal(SIGFPE, handle_sigfpe);
    signal(SIGSEGV, handle_sigsegv);
    signal(SIGABRT, handle_sigabrt);
    signal(SIGILL, handle_sigill);
    signal(SIGTERM, handle_sigterm);
    signal(SIGINT, handle_sigint);
    #ifdef __unix__
        #ifdef SIGBUS
            signal(SIGBUS, handle_sigbus);
        #endif
        #ifdef SIGPIPE
            signal(SIGPIPE, handle_sigpipe);
        #endif
        #ifdef SIGHUP
            signal(SIGHUP, handle_sighup);
        #endif
        #ifdef SIGQUIT
            signal(SIGQUIT, handle_sigquit);
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
static void teardown(void) {
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
    setup_signal_handlers();
    atexit(teardown);
}


/* File Scanner */
typedef struct FileReader FileReader;
struct FileReader {
    FILE* file;
    string buffer;
    size_t capacity;
    size_t size;
    string (*next_line)(FileReader*);
    string (*next_string)(FileReader*);
    char (*next_char)(FileReader*);
    int (*next_int)(FileReader*);
    long (*next_long)(FileReader*);
    float (*next_float)(FileReader*);
    double (*next_double)(FileReader*);
    bool (*has_next)(FileReader*);
};

string __filereader_next_line(FileReader* filereader) {
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

string __filereader_next_string(FileReader* filereader) {
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

char __filereader_next_char(FileReader* filereader) {
    if (filereader == NULL || filereader->file == NULL)
        return CHAR_MAX;
    int c;
    do {
        c = fgetc(filereader->file);
    } while (isspace(c));
    if (c == EOF) return CHAR_MAX;
    return (char)c;
}

int __filereader_next_int(FileReader* filereader) {
    if (filereader == NULL || filereader->file == NULL)
        return INT_MAX;
    string line = __filereader_next_string(filereader);
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

long __filereader_next_long(FileReader* filereader) {
    if (filereader == NULL || filereader->file == NULL)
        return LONG_MAX;
    string line = __filereader_next_string(filereader);
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

float __filereader_next_float(FileReader* filereader) {
    if (filereader == NULL || filereader->file == NULL)
        return FLT_MAX;
    string line = __filereader_next_string(filereader);
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

double __filereader_next_double(FileReader* filereader) {
    if (filereader == NULL || filereader->file == NULL)
        return DBL_MAX;
    string line = __filereader_next_string(filereader);
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

bool __filereader_has_next(FileReader* filereader) {
    if (filereader == NULL || filereader->file == NULL)
        return false;
    int c = fgetc(filereader->file);
    if (c != EOF && ungetc(c, filereader->file) == EOF)
        return false;
    return c != EOF;
}

FileReader* new_filereader(const char* filename) {
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
    filereader->next_line = __filereader_next_line;
    filereader->next_string = __filereader_next_string;
    filereader->next_char = __filereader_next_char;
    filereader->next_int = __filereader_next_int;
    filereader->next_long = __filereader_next_long;
    filereader->next_float = __filereader_next_float;
    filereader->next_double = __filereader_next_double;
    filereader->has_next = __filereader_has_next;
    return filereader;
}

void close_filereader(FileReader* filereader) {
    if (filereader != NULL) {
        if (filereader->file != NULL)
            fclose(filereader->file);
        if (filereader->buffer != NULL)
            free(filereader->buffer);
        free(filereader);
    }
}


/* File Writer */
typedef struct FileWriter FileWriter;
struct FileWriter {
    FILE* file;
    void (*write_line)(FileWriter*, const char*);
    void (*write_string)(FileWriter*, const char*);
    void (*write_char)(FileWriter*, char);
    void (*write_int)(FileWriter*, int);
    void (*write_long)(FileWriter*, long);
    void (*write_float)(FileWriter*, float);
    void (*write_double)(FileWriter*, double);
};

void __filewriter_write_line(FileWriter* filewriter, const char* line) {
    if (filewriter == NULL || filewriter->file == NULL || line == NULL) return;
    fprintf(filewriter->file, "%s\n", line);
}

void __filewriter_write_string(FileWriter* filewriter, const char* s) {
    if (filewriter == NULL || filewriter->file == NULL || s == NULL) return;
    fprintf(filewriter->file, "%s", s);
}

void __filewriter_write_char(FileWriter* filewriter, char c) {
    if (filewriter == NULL || filewriter->file == NULL) return;
    fprintf(filewriter->file, "%c", c);
}

void __filewriter_write_int(FileWriter* filewriter, int n) {
    if (filewriter == NULL || filewriter->file == NULL) return;
    fprintf(filewriter->file, "%d", n);
}

void __filewriter_write_long(FileWriter* filewriter, long n) {
    if (filewriter == NULL || filewriter->file == NULL) return;
    fprintf(filewriter->file, "%ld", n);
}

void __filewriter_write_float(FileWriter* filewriter, float f) {
    if (filewriter == NULL || filewriter->file == NULL) return;
    fprintf(filewriter->file, "%f", f);
}

void __filewriter_write_double(FileWriter* filewriter, double d) {
    if (filewriter == NULL || filewriter->file == NULL) return;
    fprintf(filewriter->file, "%lf", d);
}

FileWriter* new_filewriter_WA(const char* filename, bool append) {
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
    filewriter->write_line = __filewriter_write_line;
    filewriter->write_string = __filewriter_write_string;
    filewriter->write_char = __filewriter_write_char;
    filewriter->write_int = __filewriter_write_int;
    filewriter->write_long = __filewriter_write_long;
    filewriter->write_float = __filewriter_write_float;
    filewriter->write_double = __filewriter_write_double;
    return filewriter;
}
FileWriter* new_filewriter_W(const char* filename) { return new_filewriter_WA(filename, false); }
FileWriter* new_filewriter_A(const char* filename) { return new_filewriter_WA(filename, true); }

// Macro for simulating function overloading
#define GET_MACRO(_1, _2, NAME, ...) NAME
#define new_filewriter(...) GET_MACRO(__VA_ARGS__, new_filewriter_WA, new_filewriter_W)(__VA_ARGS__)

void close_filewriter(FileWriter* filewriter) {
    if (filewriter != NULL) {
        if (filewriter->file != NULL)
            fclose(filewriter->file);
        free(filewriter);
    }
}

void close_filewriter_flush(FileWriter* filewriter) {
    __filewriter_write_char(filewriter, '\n');
    close_filewriter(filewriter);
}


#pragma GCC diagnostic pop

#endif  // _MYCLIB_H_