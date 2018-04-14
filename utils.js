/**
 /**
 * test if a variable is a String
 * @param  {any} variable to test
 * @return {Boolean}
 */

export
function isString( a ) {
    return 	!!a && a.constructor===String;
}

/**
 * test if a variable is an Array
 * @param  {any} variable to test
 * @return {Boolean}
 */

export
function isArray( a ) {
    return 	!!a && a.constructor===Array;
}

/**
 * test if a variable is an object
 * @param  {any} variable to test
 * @return {Boolean}
 */

export
function isObject( a ) {
    return 	!!a && a.constructor===Object;
}

/**
 * test if a variable is a function
 * @param  {any} variable to test
 * @return {Boolean}
 */

export
function isFunction( a ) {
    return 	!!a && a.constructor===Function;
}

/**
 * test is a value is numeric
 * @param {*} value - value to test
 * @return {boolean} false for strings, false for undefined
 */

export
function isNumeric( value ) {
    return typeof value === "number"
}
