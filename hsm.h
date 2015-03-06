/*
The MIT License (MIT)

Copyright (c) 2015 Howard Chan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef __HSM_H__
#define __HSM_H__

//#TODO:
// 1) Add Tran INIT check

// Enable for HSM debugging
#define HSM_DEBUG

// The following can be moved to another file
#define NULL 		((void *)0)
#define TRUE		(1)
#define FALSE		(0)
typedef unsigned int UINT32;
typedef unsigned char UINT8;

#define HSM_MAX_DEPTH 5

// State definitions
#define HSME_NULL	0
#define HSME_ENTRY	1
#define HSME_EXIT	2
#define HSME_INIT	3
#define HSME_START	4

// Structure declaration
typedef UINT32 HSM_EVENT;
typedef struct HSM_STATE_T HSM_STATE;
typedef struct HSM_T HSM;

typedef HSM_EVENT (* HSM_FN)(HSM *This, HSM_EVENT event, UINT32 param);

struct HSM_STATE_T
{
	HSM_STATE *parent;			// parent state
	HSM_FN handler;				// associated event handler for state
	char *name;             	// name of state
	UINT8 level;                // depth level of the state
};

struct HSM_T
{
	HSM_STATE *curState;		// Current HSM State
	char *name;					// Name of HSM Machine
	UINT8 hsmDebug;				// HSM debug flag
};

// Func: void HSM_STATE_Create(HSM_STATE *This, char *name, HSM_FN handler, HSM_STATE *parent)
// Desc: Create an HSM State for the HSM.
// This: Pointer to HSM_STATE object
// name: Name of state (for debugging)
// handler: State Event Handler
// parent: Parent state.  If NULL, then internal ROOT handler is used as catchall
void HSM_STATE_Create(HSM_STATE *This, char *name, HSM_FN handler, HSM_STATE *parent);

// Func: void HSM_Create(HSM *This, char *name, HSM_STATE *initState)
// Desc: Create the HSM instance.  Required for each instance
// name: Name of state machine (for debugging)
// initState: Initial state of statemachine
void HSM_Create(HSM *This, char *name, HSM_STATE *initState);

// Func: HSM_STATE *HSM_GetState(HSM *This)
// Desc: Get the current HSM STATE
// This: Pointer to HSM instance
// return|HSM_STATE *: Pointer to HSM STATE
HSM_STATE *HSM_GetState(HSM *This);

// Func: void HSM_Run(HSM *This, HSM_EVENT event, UINT32 param)
// Desc: Run the HSM with event
// This: Pointer to HSM instance
// event: HSM_EVENT processed by HSM
// param: Parameter associated with HSM_EVENT
void HSM_Run(HSM *This, HSM_EVENT event, UINT32 param);

// Func: void HSM_Tran(HSM *This, HSM_STATE *nextState, UINT32 param, void (*method)(void))
// Desc: Transition to another HSM STATE
// This: Pointer to HSM instance
// nextState: Pointer to next HSM STATE
// param: Optional Parameter associated with HSME_ENTRY and HSME_EXIT event
// moethod: Optional function hook between the HSME_ENTRY and HSME_EXIT event handling
void HSM_Tran(HSM *This, HSM_STATE *nextState, UINT32 param, void (*method)(void));

#endif // __HSM_H__