#include "custom.h"

#include <ctype.h>
#include <stdlib.h>
#include "environment.h"
#include "error.h"
#include "eval.h"
#include "list.h"
#include "parser.h"
#include "primproc.h"
#include "storage.h"

#define AREA "CUSTOM"

static obj evalstr(char *e, obj env)
{
	return eval(readp(openin_string(e)), env);
}

static obj add_extras(int ex, obj env)
{
	if (ex > 101) {
		//define_variable(of_identifier("abs"), of_function(absl), env);
		evalstr("(define (abs x) (if (< x 0) (- x) x))", env);

		define_variable(of_identifier("<="), of_function(lte), env);
		//evalstr("(define (<= x y) (not (> x y)))", env);

		define_variable(of_identifier(">="), of_function(gte), env);
		//evalstr("(define (>= x y) (not (< x y)))", env);

		evalstr("(define (square x) (* x x))", env);
		evalstr("(define (cube x) (* x x x))", env);
	}
	if (ex >= 109) {
		define_variable(of_identifier("exp"), of_function(expn), env);
		define_variable(of_identifier("log"), of_function(logn), env);
		// inc and dec could be 'defined' but making them primitive
		// avoids the risk of loops in questions where + is defined in
		// terms of inc
		define_variable(of_identifier("inc"), of_function(inc), env);
		define_variable(of_identifier("dec"), of_function(dec), env);
	}
	if (ex >= 106) {
		evalstr("(define (average x y) (/ (+ x y) 2))", env);
	}
	if (ex >= 115) {
		define_variable(of_identifier("remainder"), of_function(rem),
				env);
	}
	if (ex >= 116) {
		evalstr("(define (even? n) (= (remainder n 2) 0))", env);
		evalstr("(define (odd? n) (= (remainder n 2) 1))", env);
	}
	if (ex >= 120) {
		evalstr("(define (gcd a b) (if (= b 0) a (gcd b (remainder a b))))",
			env);
		define_variable(of_identifier("random"), of_function(rnd), env);
	}
	if (ex >= 121) {
		evalstr("(define (prime? n)"
			"  (define (smallest-divisor n)"
			"    (define (find-divisor n test-divisor)"
			"      (define (divides? a b)"
			"        (= (remainder b a) 0))"
			"      (cond ((> (square test-divisor) n) n)"
			"            ((divides? test-divisor n) test-divisor)"
			"            (else (find-divisor n (+ test-divisor 1)))))"
			"    (find-divisor n 2))"
			"  (= n (smallest-divisor n)))",
			env);
	}
	if (ex >= 122) {
		define_variable(of_identifier("display"), of_function(display),
				env);
		define_variable(of_identifier("ignore"), of_function(ignore),
				env);
		define_variable(of_identifier("newline"), of_function(newline),
				env);
		define_variable(of_identifier("runtime"), of_function(runtime),
				env);
		define_variable(of_identifier("seconds"), of_function(seconds),
				env);
		define_variable(of_identifier("ticks"), of_function(ticks),
				env);
	}
	if (ex >= 129) {
		evalstr("(define (identity x) x)", env);
	}
	if (ex >= 135) {
		define_variable(of_identifier("sin"), of_function(sine), env);
		define_variable(of_identifier("cos"), of_function(cosine), env);
		define_variable(of_identifier("error"), of_function(user_error),
				env);
		evalstr("(define (positive? x) (< 0 x))", env);
		evalstr("(define (negative? x) (< x 0))", env);
	}
	if (ex >= 145) {
		// not in the book but most answers use it.
		define_variable(of_identifier("floor"), of_function(flr), env);
	}
	if (ex >= 201) {
		define_variable(of_identifier("cons"), of_function(consp), env);
		define_variable(of_identifier("car"), of_function(carp), env);
		define_variable(of_identifier("cdr"), of_function(cdrp), env);
	}
	if (ex >= 203) {
		define_variable(of_identifier("sqrt"), of_function(sqroot),
				env);
	}
	if (ex >= 205) {
		evalstr("(define (expt b n)"
			"  (define (expt-iter b c p)"
			"    (if (= c 0) p (expt-iter b (- c 1) (* b p))))"
			"  (expt-iter b n 1))",
			env);
	}
	if (ex >= 207) {
		define_variable(of_identifier("min"), of_function(minimum),
				env);
		define_variable(of_identifier("max"), of_function(maximum),
				env);
	}
	if (ex >= 217) {
		define_variable(of_identifier("nil"), emptylst, env);
		define_variable(of_identifier("list"), of_function(list), env);
		evalstr("(define (list-ref items n)"
			"  (if (= n 0)"
			"      (car items)"
			"      (list-ref (cdr items) (- n 1))))",
			env);
		define_variable(of_identifier("null?"), of_function(is_null_p),
				env);
		define_variable(of_identifier("length"), of_function(length_p),
				env);
		define_variable(of_identifier("reverse"),
				of_function(reverse_p), env);
		define_variable(of_identifier("append"), of_function(append_p),
				env);
		define_variable(of_identifier("caar"), of_function(caar_p),
				env);
		define_variable(of_identifier("cadr"), of_function(cadr_p),
				env);
		define_variable(of_identifier("cdar"), of_function(cdar_p),
				env);
		define_variable(of_identifier("cddr"), of_function(cddr_p),
				env);
		define_variable(of_identifier("caaar"), of_function(caaar_p),
				env);
		define_variable(of_identifier("caadr"), of_function(caadr_p),
				env);
		define_variable(of_identifier("cadar"), of_function(cadar_p),
				env);
		define_variable(of_identifier("caddr"), of_function(caddr_p),
				env);
		define_variable(of_identifier("cdaar"), of_function(cdaar_p),
				env);
		define_variable(of_identifier("cdadr"), of_function(cdadr_p),
				env);
		define_variable(of_identifier("cddar"), of_function(cddar_p),
				env);
		define_variable(of_identifier("cdddr"), of_function(cdddr_p),
				env);
		define_variable(of_identifier("caaaar"), of_function(caaaar_p),
				env);
		define_variable(of_identifier("caaadr"), of_function(caaadr_p),
				env);
		define_variable(of_identifier("caadar"), of_function(caadar_p),
				env);
		define_variable(of_identifier("caaddr"), of_function(caaddr_p),
				env);
		define_variable(of_identifier("cadaar"), of_function(cadaar_p),
				env);
		define_variable(of_identifier("cadadr"), of_function(cadadr_p),
				env);
		define_variable(of_identifier("caddar"), of_function(caddar_p),
				env);
		define_variable(of_identifier("cadddr"), of_function(cadddr_p),
				env);
		define_variable(of_identifier("cdaaar"), of_function(cdaaar_p),
				env);
		define_variable(of_identifier("cdaadr"), of_function(cdaadr_p),
				env);
		define_variable(of_identifier("cdadar"), of_function(cdadar_p),
				env);
		define_variable(of_identifier("cdaddr"), of_function(cdaddr_p),
				env);
		define_variable(of_identifier("cddaar"), of_function(cddaar_p),
				env);
		define_variable(of_identifier("cddadr"), of_function(cddadr_p),
				env);
		define_variable(of_identifier("cdddar"), of_function(cdddar_p),
				env);
		define_variable(of_identifier("cddddr"), of_function(cddddr_p),
				env);
	}
	if (ex >= 221) {
		evalstr("(define (map proc . arglists)"
			"  (define (smap proc items)"
			"    (define (iter items mapped)"
			"      (if (null? items)"
			"          mapped"
			"          (iter (cdr items)"
			"                (cons (proc (car items))"
			"                      mapped))))"
			"    (reverse (iter items nil)))"
			"  (define (iter arglists mapped)"
			"    (if (null? (car arglists))"
			"        mapped"
			"        (iter (smap cdr arglists)"
			"              (cons (apply proc (smap car arglists))"
			"                    mapped))))"
			"  (reverse (iter arglists nil)))",
			env);
	}
	return unspecified;
}

static int ex_num(obj arg)
{
	const char *str;
	int ok = false;
	if (is_string(arg)) {
		str = to_string(arg);
		if (isdigit(str[0]) && (str[1] == '.') && isdigit(str[2]) &&
		    (str[3] == '\0' || (isdigit(str[3]) && str[4] == '\0'))) {
			ok = true;
		}
	}
	if (!ok) {
		return -1;
	}
	return (100 * (str[0] - 48)) + atoi(str + 2);
}

static obj conf_ex(obj env, obj args)
{
	obj arg;
	int ex;
	if (is_err(arg = chkarity("%ex", 1, args)))
		return arg;
	if ((ex = ex_num(car(args))) == -1)
		return error_argument_type(
			AREA, "%%ex expects a string of from \"1.23\"");
	return add_extras(ex, env);
}

obj do_head(obj env, struct inport *in)
{
	disable_gc = true;
	obj exp = readp(in);
	if (is_pairptr(exp) && eq_symbol(car(exp), _ex)) {
		obj r = conf_ex(env, cdr(exp));
		if (is_err(r))
			return r;
		exp = readp(in);
	} else {
		add_extras(552, env);
	}
	disable_gc = false;
	return exp;
}

static obj defined(void)
{
	return caar(the_global_environment());
}

static obj display_definedp(struct outport *out)
{
	obj names;
	obj lpad = of_string("  ");
	displayp(out, of_string("Defined Variables:"));
	for (names = reverse(defined()); is_pairptr(names);
	     names = cdr(names)) {
		newline(emptylst);
		displayp(out, lpad);
		displayp(out, car(names));
	}
	newline(emptylst);
	displayp(out, of_string("Custom Special Forms:"));
	newline(emptylst);
	displayp(out, lpad);
	displayp(out, of_string("apply"));
	newline(emptylst);
	displayp(out, lpad);
	displayp(out, of_string("time"));
	return _void;
}

obj display_defined(obj _)
{
	(void)_;
	return display_definedp(default_out());
}
