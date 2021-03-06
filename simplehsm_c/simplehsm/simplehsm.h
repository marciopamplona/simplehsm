/**
 * @file
 * @author  Daniel Newton <djpnewton@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Copyright (c) 2009 Daniel Newton
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @section DESCRIPTION
 *
 * The simplehsm C implementation header
 */

//---------------------------------------------------------------------------
#ifndef simplehsmH
#define simplehsmH
//---------------------------------------------------------------------------

//
// Boolean definitions
//

#ifndef BOOL
typedef int BOOL;
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

/** \enum simplehsm_signals_t
 * The base state machine signals
 */
enum simplehsm_signals_t
{
  SIG_NULL     = 0, /**< Null signal, all state functions should ignore this signal and return their parent state (or #stnone if the top level state) */
  SIG_INIT     = 1, /**< Initialisation signal, a state function should transition to a default substate (simplehsm_init_transition_state()) if it has substates */
  SIG_ENTRY    = 2, /**< Entry signal, a state function should perform its entry actions (if any) */
#ifdef SHSM_DEEPHIST
  SIG_DEEPHIST = 3, /**< Record deep history signal, a state function should return 'stdeephist' if it contains a deep history psuedostate */
#endif
  SIG_EXIT     = 4, /**< Exit signal, a state function should perform its exit actions (if any) */
  SIG_USER     = 5, /**< User signals should start from this index */
};

//
// State machine definitions
//

/** 
 * Used by a state function when it returns no parent state (either because it is the top state
 * or because the signal is handled).
 */
#define stnone NULL

#ifdef SHSM_DEEPHIST
/** 
 * Used by a state function when it handles a #SIG_DEEPHIST signal (which represents that the
 * state has a child deep history psuedostate).
 */
#define stdeephist NULL
#endif

/**
 * A generic pointer that points to a state function
 */
typedef void* stnext;

/**
 * A state function
 *
 * @todo make this definition better (simplehsm_state_t)?
 */
typedef stnext (*stfunc)(int, void*);

#ifdef SHSM_DEEPHIST
/**
 * The maximum number of deep history psuedostates supported in a single simplehsm instance
 */
#define MAX_HISTORY 10

/**
 * A stucture to hold an entry in the deep history table
 */
typedef struct
{
  stfunc parent;
  stfunc child;
} deep_history_t;
#endif

/**
 * The state machine object
 */
typedef struct
{
  stfunc current_state; /**< The current state of the state machine */
  stfunc top_state;     /**< The topmost state of the state machine hierarchy */
#ifdef SHSM_DEEPHIST
  deep_history_t deep_history_table[MAX_HISTORY]; /**< The table of current deep history psuedostates */
#endif
} simplehsm_t;

//
// State utility function definitions
//

void simplehsm_initialize(simplehsm_t* hsm, stfunc top_state);
#ifdef SHSM_DEEPHIST
void simplehsm_transition_state_ex(simplehsm_t* hsm, stfunc new_state, BOOL to_deep_hist);
#endif
void simplehsm_transition_state(simplehsm_t* hsm, stfunc new_state);
void simplehsm_init_transition_state(simplehsm_t* hsm, stfunc new_state);
void simplehsm_signal_current_state(simplehsm_t* hsm, int signal, void* param);
BOOL simplehsm_is_in_state(simplehsm_t* hsm, stfunc state);
#ifdef SHSM_DEEPHIST
void simplehsm_record_deephist(simplehsm_t* hsm, stfunc history_parent, stfunc history_state);
stfunc simplehsm_retrieve_deephist(simplehsm_t* hsm, stfunc history_parent);
#endif

//---------------------------------------------------------------------------
#endif  //simplehsmH
