#include <stdlib.h>
#include <string.h>
#include "sserror.h"
#include "lexer.h"
#include "parser.h"

#define AREA "PARSER"

static obj *number(struct token *);

struct in_port *default_in = NULL;
static struct in_port *dfltin(void)
{
	return default_in == NULL ? (default_in = openin_ptr(stdin)) :
				    default_in;
}

obj *read(void)
{
	return  readp(dfltin());
}

obj *readp(struct in_port *in)
{
	struct token *tkn = read_token(in);
	switch (tkn->type) {
	case TKN_EOF:
		return lexer_errored ? error_lexor() : eof_object;
	case TKN_NUMBER:
		return number(tkn);
	default:
		error(AREA, "BUG: more token types than cases.");
		return error_internal();
	}
}

static obj *number(struct token *tkn)
{
	if (strlen(tkn->value) > 18)
		return error_parser();
	return int64(atoll(tkn->value));
}
