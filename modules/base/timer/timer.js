/*
 * Copyright (c) 2016-2017  Moddable Tech, Inc.
 *
 *   This file is part of the Moddable SDK Runtime.
 * 
 *   The Moddable SDK Runtime is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 * 
 *   The Moddable SDK Runtime is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Lesser General Public License for more details.
 * 
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with the Moddable SDK Runtime.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


/*
	timer
*/

export class Timer {
	static set(callback, delay, repeat) @ "xs_timer_set";
	static repeat(callback, delay) @ "xs_timer_repeat";
	static schedule(id, delay, repeat) @ "xs_timer_schedule";
	static clear(id) @ "xs_timer_clear";
	static delay(ms) @ "xs_timer_delay";
};

export function setTimeout( callback, delay ) {
    return Timer.set( callback, delay, false );
}

export function setInterval( callback, delay ) {
    return Timer.set( callback, delay, true );
}

export function clearTimeout( id ) {
    Timer.clear( id );
}

export function clearInterval( id ) {
    Timer.clear( id );
}



Object.freeze(Timer.prototype);
