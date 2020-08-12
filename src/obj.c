#include "obj.h"

#include <stdlib.h>
#include <string.h>
#include "error.h"

#define AREA "OBJ"
#define VALUE val /* allow renaming of value member to check api leak */

static obj new_simp(int type, int subtype)
{
	obj dat = { false, .simp = { .type = type, .subtype = subtype } };
	return dat;
}

enum type type(obj dat)
{
	if (dat.ispair) {
		eprintf(AREA, "Cannot get type of a pair");
		error_argument_type();
	}
	return dat.simp.type;
}

enum subtype subtype(obj dat)
{
	if (dat.ispair) {
		eprintf(AREA, "Cannot get type of a pair");
		error_argument_type();
	}
	return dat.simp.subtype;
}

// SYMBOL

bool issymbol(obj dat)
{
	return !ispair(dat) && dat.simp.type == TYPE_SYMBOL;
}

static obj ofidentifier(char *id)
{
	obj dat = new_simp(TYPE_SYMBOL, SUBTYPE_NOT_SET);
	dat.simp.VALUE.string = id;
	return dat;
}

// NUMBER

bool isnumber(obj dat)
{
	return !ispair(dat) && dat.simp.type == TYPE_NUMBER;
}

static obj ofint64(int64_t n)
{
	obj dat = new_simp(TYPE_NUMBER, NUMBER_INT64);
	dat.simp.VALUE.int64 = n;
	return dat;
}

static int64_t toint64(obj dat)
{
	return dat.simp.VALUE.int64;
}

// STRING

bool isstring(obj dat)
{
	return !ispair(dat) && dat.simp.type == TYPE_STRING;
}

const obj nl_struct = {
	false, .simp = { TYPE_STRING, SUBTYPE_NOT_SET, { .string = "\n" } }
};

static obj nl(void)
{
	return nl_struct;
}

static obj ofstring(char *str)
{
	obj dat = new_simp(TYPE_STRING, SUBTYPE_NOT_SET);
	dat.simp.VALUE.string = str;
	return dat;
}

static char *tostring(obj dat)
{
	return dat.simp.VALUE.string;
}

// PAIR

bool ispair(obj dat)
{
	return dat.ispair;
}

bool isnull(obj dat)
{
	return !ispair(dat) && dat.simp.type == TYPE_EMPTY_LIST;
}

const obj emptylst = { false,
		       .simp = { TYPE_EMPTY_LIST, SUBTYPE_NOT_SET, { 0 } } };

obj cons(obj car, obj cdr)
{
	obj p = { true,
		  .pair = { ispair(car) ? Obj.reference(car).simp : car.simp,
			    ispair(cdr) ? Obj.reference(cdr).simp :
					  cdr.simp } };
	return p;
}

obj car(obj pair)
{
	obj dat = { false, .simp = pair.pair.car };
	if (!ispair(pair)) {
		eprintf(AREA, "car expects a pair");
		return error_argument_type();
	}
	return isreference(dat) ? Obj.dereference(dat) : dat;
}

obj cdr(obj pair)
{
	obj dat = { false, .simp = pair.pair.cdr };
	if (!ispair(pair)) {
		eprintf(AREA, "cdr expects a pair");
		return error_argument_type();
	}
	return isreference(dat) ? Obj.dereference(dat) : dat;
}

// REFERENCE

bool isreference(obj dat)
{
	return !ispair(dat) && dat.simp.type == TYPE_REFERENCE;
}

static obj reference(obj dat)
{
	obj *ptr = (obj *)calloc(sizeof(obj), 1);
	if (ptr == NULL) {
		eprintf(AREA, "No memory for reference");
		return error_memory();
	}
	*ptr = dat;
	obj ref = new_simp(TYPE_REFERENCE, SUBTYPE_NOT_SET);
	ref.simp.VALUE.reference = ptr;
	return ref;
}

static obj dereference(obj dat)
{
	return *dat.simp.VALUE.reference;
}

// PRIMITIVE PROCEDURES (FUNCTIONS)

bool isprimproc(obj dat)
{
	return !ispair(dat) && dat.simp.type == TYPE_PRIMITIVE_PROCEDURE;
}

static obj offunction(obj (*funptr)(obj))
{
	obj pp = new_simp(TYPE_PRIMITIVE_PROCEDURE, SUBTYPE_NOT_SET);
	pp.simp.VALUE.primproc = funptr;
	return pp;
}

static obj (*tofunction(obj dat))(obj)
{
	return dat.simp.VALUE.primproc;
}

// UNSPECIFIED

const obj unspecified_struct = {
	false, .simp = { TYPE_UNSPECIFIED, SUBTYPE_NOT_SET, { 0 } }
};

static obj unspecified(void)
{
	return unspecified_struct;
}

// EOF

const obj eof_struct = { false, .simp = { TYPE_EOF, SUBTYPE_NOT_SET, { 0 } } };

bool iseof(obj dat)
{
	return !ispair(dat) && dat.simp.type == TYPE_EOF;
}

static obj eof(void)
{
	return eof_struct;
}

// ERROR

obj make_err(int err_subtype)
{
	obj obj = new_simp(TYPE_ERROR, err_subtype);
	return obj;
}

// ACCESSOR

const struct obj_accessor Obj = {
	.ofidentifier = ofidentifier,

	.ofint64 = ofint64,
	.toint64 = toint64,

	.nl = nl,
	.ofstring = ofstring,
	.tostring = tostring,

	.reference = reference,
	.dereference = dereference,

	.offunction = offunction,
	.tofunction = tofunction,

	.unspecified = unspecified,

	.eof = eof,
};
