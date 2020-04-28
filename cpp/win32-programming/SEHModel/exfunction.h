
#include < windows.h >

typedef void (*MyFunctionType)(int);  // Function Type

// Add function to adds function pointers to the array of func pointers
void AddFunction(int, int priority=1);

// Executes the functions stored in the array
int ExeFunction(void);

// Get the text from the output list
wchar_t* getOutputText(int);

// flush all the buffers and remove all the set exception handlers/filters
void flushBuffer(void);
