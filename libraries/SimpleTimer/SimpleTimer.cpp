/*
 * SimpleTimer.cpp
 *
 * SimpleTimer - A timer library for Arduino.
 * Author: mromani@ottotecnica.com
 * Copyright (c) 2010 OTTOTECNICA Italy
 *
 * This library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * This library is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser
 * General Public License along with this library; if not,
 * write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "SimpleTimer.h"

static inline unsigned long elapsed() { return millis(); }

SimpleTimer::SimpleTimer() {
    unsigned long current_millis = elapsed();

    for (int i = 0; i < MAX_TIMERS; i++) {
        enabled[i] = false;
        callbacks[i] = 0; 
        vcallbacks[i] = 0;
        // if the callback pointer is zero, the slot is free, i.e. doesn't "contain" any timer
        prev_millis[i] = current_millis;
        numRuns[i] = 0;
    }

    numTimers = 0;
}


void SimpleTimer::run() {
    unsigned long current_millis = elapsed();

    for (int i = 0; i < MAX_TIMERS; i++) {

        toBeCalled[i] = DEFCALL_DONTRUN;

        // no callback == no timer, i.e. jump over empty slots
        if (callbacks[i] || vcallbacks[i] ) {

            if (current_millis - prev_millis[i] >= delays[i]) {

                // update time
                prev_millis[i] += delays[i];

                // check if the timer callback has to be executed
                if (enabled[i]) {

                    // "run forever" timers must always be executed
                    if (maxNumRuns[i] == RUN_FOREVER) {
                        toBeCalled[i] = DEFCALL_RUNONLY;
                    }
                    // other timers get executed the specified number of times
                    else if (numRuns[i] < maxNumRuns[i]) {
                        toBeCalled[i] = DEFCALL_RUNONLY;
                        numRuns[i]++;

                        // after the last run, delete the timer
                        if (numRuns[i] >= maxNumRuns[i]) {
                            toBeCalled[i] = DEFCALL_RUNANDDEL;
                        }
                    }
                    // NEW is this a variable timer
					else if (maxNumRuns[i] == RUN_VARIABLE) {
                        toBeCalled[i] = DEFCALL_RUNVAR;
                        numRuns[i]++;
					}
                }
            }
        }
    }

    for (int i = 0; i < MAX_TIMERS; i++) {
        switch(toBeCalled[i]) {

            case DEFCALL_DONTRUN:
                break;

            case DEFCALL_RUNONLY:
                (*callbacks[i])();
                break;

            case DEFCALL_RUNANDDEL:
                (*callbacks[i])();
                deleteTimer(i);
                break;
                
            // NEW
            case DEFCALL_RUNVAR:
            	// Call and set new dealy
            	delays[i] = (*vcallbacks[i])( numRuns[i] );
            	if (delays[i] == 0) deleteTimer(i);
            	break;
        }
    }
}


// find the first available slot
// return -1 if none found
int SimpleTimer::findFirstFreeSlot() {
    int i;

    // all slots are used
    if (numTimers >= MAX_TIMERS) {
        return -1;
    }

    // return the first slot with no callback (i.e. free)
    for (i = 0; i < MAX_TIMERS; i++) {
        if (callbacks[i] == 0 && vcallbacks[i] == 0) {
            return i;
        }
    }

    // no free slots found
    return -1;
}

// NEW
int SimpleTimer::setVariableTimer(variable_callback f) {

    int freeTimer = findFirstFreeSlot();
    if (freeTimer < 0 || f == NULL) {
        return -1;
    }

	// actually calls the function passing 0, the return value
	// is used to setup the first delay.
    delays[freeTimer] = (*f)(0); 
    vcallbacks[freeTimer] = f;
    maxNumRuns[freeTimer] = RUN_VARIABLE;
    enabled[freeTimer] = true;
    prev_millis[freeTimer] = elapsed();

    numTimers++;

	// If for some reason the first call returns a zero immediately delete
	if (delays[freeTimer] == 0) deleteTimer(freeTimer);

    return freeTimer;
}

int SimpleTimer::setTimer(long d, timer_callback f, int n) {

    int freeTimer = findFirstFreeSlot();
    if (freeTimer < 0 || f == NULL) {
        return -1;
    }

    delays[freeTimer] = d;
    callbacks[freeTimer] = f;
    maxNumRuns[freeTimer] = n;
    enabled[freeTimer] = true;
    prev_millis[freeTimer] = elapsed();

    numTimers++;

    return freeTimer;
}


int SimpleTimer::setInterval(long d, timer_callback f) {
    return setTimer(d, f, RUN_FOREVER);
}


int SimpleTimer::setTimeout(long d, timer_callback f) {
    return setTimer(d, f, RUN_ONCE);
}


void SimpleTimer::deleteTimer(int timerId) {

    // nothing to delete if no timers are in use
    if (timerId >= MAX_TIMERS || numTimers == 0) {
        return;
    }

    // don't decrease the number of timers if the
    // specified slot is already empty
    if (callbacks[timerId] != NULL || vcallbacks[timerId] != NULL) {
        callbacks[timerId] = 0;
        vcallbacks[timerId] = 0;
        enabled[timerId] = false;
        delays[timerId] = 0;
        numRuns[timerId] = 0;

        // update number of timers
        numTimers--;
    }
}


// function contributed by code@rowansimms.com
void SimpleTimer::restartTimer(int numTimer) {
    if (numTimer >= MAX_TIMERS) {
        return;
    }

    prev_millis[numTimer] = elapsed();
}


boolean SimpleTimer::isEnabled(int numTimer) {
    if (numTimer >= MAX_TIMERS) {
        return false;
    }

    return enabled[numTimer];
}


void SimpleTimer::enable(int numTimer) {
    if (numTimer >= MAX_TIMERS) {
        return;
    }

    enabled[numTimer] = true;
}


void SimpleTimer::disable(int numTimer) {
    if (numTimer >= MAX_TIMERS) {
        return;
    }

    enabled[numTimer] = false;
}


void SimpleTimer::toggle(int numTimer) {
    if (numTimer >= MAX_TIMERS) {
        return;
    }

    enabled[numTimer] = !enabled[numTimer];
}