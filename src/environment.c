/* Line numbers refer to:
 https://github.com/codybartfast/sicp/blob/master/chapter5/mc-evaluator-50.scm */

#include "environment.h"

#include <string.h>
#include "custom.h"
#include "error.h"
#include "list.h"
#include "output.h"
#include "primproc.h"

#define AREA "ENVIRONMENT"

static obj the_empty_environment(void);
static obj lvv_env_loop(obj var, obj env);
static obj setup_environment(void);

// until we have a proper eq? - also in mceval.c
bool eq_symbol(obj a, obj b)
{
	return is_symbol(a) && is_symbol(b) &&
	       strcmp(to_string(a), to_string(b)) == 0;
}

//ln 231
static obj enclosing_environment(obj env)
{
	return cdr(env);
}

// ln 232
static obj first_frame(obj env)
{
	return car(env);
}

// ln 233
static obj the_empty_environment(void)
{
	return emptylst;
}

// ln 235
static obj make_frame(obj variables, obj values)
{
	return cons(variables, values);
}

// ln 237
static obj frame_variables(obj frame)
{
	return car(frame);
}

// ln 238
static obj frame_values(obj frame)
{
	return cdr(frame);
}

// ln 239
static obj add_binding_to_frame(obj var, obj val, obj frame)
{
	obj r = set_car(frame, cons(var, car(frame)));
	if (is_err(r))
		return r;
	r = set_cdr(frame, cons(val, cdr(frame)));
	return r;
}

// ln 243
obj extend_environment(obj vars, obj vals, obj base_env)
{
	int nvars = length_i(vars, 0, false);
	int nvals = length_i(vals, 0, false);

	if (nvars >= 0 && nvals >= 0) {
		return (nvars == nvals) ?
			       cons(make_frame(vars, vals), base_env) :
			       error_arity(
				       AREA,
				       "Too %s arguments supplied, var: %s, vals: %s",
				       nvars < nvals ? "many" : "few",
				       errstr(vars), errstr(vals));
	} else if (nvals < 0) {
		return error_syntax(
			AREA,
			"The arguments are not a proper list, var: %s, vals: %s",
			errstr(vars), errstr(vals));
	} else {
		// "dotted tail"
		obj ovars = vars, ovals = vals;
		obj rvars, rvals;

		for (rvars = emptylst, rvals = emptylst;
		     is_pairptr(vars) && is_pairptr(vals);
		     vars = cdr(vars), vals = cdr(vals)) {
			rvars = cons(car(vars), rvars);
			rvals = cons(car(vals), rvals);
		}
		if (is_pairptr(vars)) {
			return error_arity(
				AREA,
				"Too few arguments supplied, var: %s, vals: %s",
				errstr(ovars), errstr(ovals));
		}
		rvars = cons(vars, rvars); // vars should be a var
		rvals = cons(vals, rvals); // vals should be a list
		return cons(make_frame(reverse(rvars), reverse(rvals)),
			    base_env);
	}
}

// ln 250
static obj lvv_scan(obj var, obj env, obj vars, obj vals)
{
	if (is_null(vars)) {
		return lvv_env_loop(var, enclosing_environment(env));
	} else if (eq_symbol(var, car(vars))) {
		return car(vals);
	} else {
		return lvv_scan(var, env, cdr(vars), cdr(vals));
	}
}

// ln 250
static obj lvv_env_loop(obj var, obj env)
{
	if (is_null(env)) { // should be eq - but don't have it yet.
		return error_unbound_variable(AREA, "%s", to_string(var));
	}
	obj frame = first_frame(env);
	return lvv_scan(var, env, frame_variables(frame), frame_values(frame));
}

// ln 250
obj lookup_variable_value(obj var, obj env)
{
	obj val = lvv_env_loop(var, env);
	return val;
}

// ln 280
static obj dv_scan(obj vars, obj vals, obj var, obj val, obj frame)
{
	if (is_null(vars))
		return add_binding_to_frame(var, val, frame);
	if (eq_symbol(var, car(vars)))
		return set_car(vals, val);
	return dv_scan(cdr(vars), cdr(vals), var, val, frame);
}
// ln 280
obj define_variable(obj var, obj val, obj env)
{
	obj frame = first_frame(env);
	return dv_scan(frame_variables(frame), frame_values(frame), var, val,
		       frame);
}

// ln 295
static obj _initial_procedures;
static obj initial_procedures(void)
{
	obj initial_primprocs =
		listn(13, // must match number of items below
		      list2(of_identifier("true"), tru_o), // 1
		      list2(of_identifier("false"), fls_o), // 2
		      list2(of_identifier("+"), of_function(add)), // 3
		      list2(of_identifier("-"), of_function(sub)), // 4
		      list2(of_identifier("*"), of_function(mul)), // 5
		      list2(of_identifier("/"), of_function(divd)), // 6
		      list2(of_identifier("<"), of_function(lt)), // 7
		      list2(of_identifier("="), of_function(eqn)), // 8
		      list2(of_identifier(">"), of_function(gt)), // 9
		      list2(of_identifier("and"), of_function(and)), // 10
		      list2(of_identifier("or"), of_function(or)), // 11
		      list2(of_identifier("not"), of_function(not )), // 12
		      // Implementation specific, (not in book):
		      list2(of_identifier("%defined"),
			    of_function(display_defined))); // 13

	return is_pairptr(_initial_procedures) ?
		       _initial_procedures :
		       (_initial_procedures = reverse(initial_primprocs));
}

// ln 301
static obj initial_procedure_names(void)
{
	return map_u(car, initial_procedures());
}

// ln 305
static obj initial_procedure_objects(void)
{
	return map_u(cadr, initial_procedures());
}

// ln 309
static obj setup_environment(void)
{
	obj initial_env = extend_environment(initial_procedure_names(),
					     initial_procedure_objects(),
					     the_empty_environment());
	return initial_env;
}

// ln 317
static obj _the_global_environment;
obj the_global_environment(void)
{
	if (!is_pairptr(_the_global_environment))
		_the_global_environment = setup_environment();
	return _the_global_environment;
}

obj tge(void)
{
	return the_global_environment();
}

// new
void set_global_environment(obj tge)
{
	_the_global_environment = tge;
}
