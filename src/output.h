#ifndef OUTPUT_H
#define OUTPUT_H
#include "sicpstd.h"

#include "obj.h"
#include "outport.h"

obj newline(void);
obj newlinep(struct outport *);
obj write(obj);
obj writep(struct outport *, obj);
char *debugstr(obj dat);
obj writestr(obj);

#endif
