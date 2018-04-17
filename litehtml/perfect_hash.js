let fs = require('fs');


// Calculates a distinct hash function for a given string. Each value of the
// integer d results in a different hash value.
function hash(d, str) {
	if (d == 0) {
		d = 0x811c9dc5;
	}

	for (var i = 0; i < str.length; i++) {
		//en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
		//isthe.com/chongo/src/fnv/hash_32.c
		// multiply by the 32 bit FNV magic prime mod 2^32
		d += (d << 1) + (d << 4) + (d << 7) + (d << 8) + (d << 24);
		// xor the bottom with the current octet
		d ^= str.charCodeAt(i);
	}

	d++;

	return d & 0x7fffffff;
}

// Computes a minimal perfect hash table using the given Javascript object hash. It
// returns a tuple (G, V). G and V are both arrays. G contains the intermediate
// table of values needed to compute the index of the value in V. V contains the
// values of the dictionary.

function create_hash(inp) {

	var dict = {};
	
	for (var j = 0; j < inp.length; j++) {
		dict[ inp[j] ] = j+1;
	}

	var size = Object.keys(dict).length;
	if( size<7 ) {
		size = 7;
	}


	var	buckets = [],
		G = new Array(size),
		values = new Array(size),
		i, b, bucket;

	// Place all of the keys into buckets
	Object.keys(dict).forEach(function (key) {
		var bkey = hash(0, key) % size;
		if (!buckets[bkey]) {
			buckets[bkey] = [];
		}
		buckets[bkey].push(key);
	});

	// Sort the buckets and process the ones with the most items first.
	buckets.sort(function (a, b) { return b.length - a.length; });

	for (b = 0; b < size; b++) {
		if (buckets[b].length <= 1) break;
		bucket = buckets[b];

		var d = 1, item = 0, slots = [], slot, used = {};

		// Repeatedly try different values of d until we find a hash function
		// that places all items in the bucket into free slots
		while (item < bucket.length) {
			slot = hash(d, bucket[item]) % size;
			if (values[slot] || used[slot]) {
				d++;
				item = 0;
				slots = [];
				used = {};
			}
			else {
				used[slot] = true;
				slots.push(slot);
				item++;
			}
		}

		G[hash(0, bucket[0]) % size] = d;
		for (i = 0; i < bucket.length; i++) {
			values[slots[i]] = dict[bucket[i]];
		}
	}

	// Only buckets with 1 item remain. Process them more quickly by directly
	// placing them into a free slot. Use a negative value of d to indicate
	// this.

	var freelist = [];
	for (i = 0; i < size; i++) {
		if (typeof values[i] == 'undefined') {
			freelist.push(i);
		}
	}

	for (; b < size; b++) {
		if (!buckets[b] || buckets[b].length == 0) break;
		bucket = buckets[b];
		slot = freelist.pop();

		// We subtract one to ensure it's negative even if the zeroeth slot was used.
		G[hash(0, bucket[0]) % size] = 0 - slot - 1;
		values[slot] = dict[bucket[0]];
	}

	return [G, values];
}


// Look up a value in the hash table, defined by G and V.
/*
function lookup_hash(G, V, key) {
	var d = G[hash(0, key) % G.length];
	if (d < 0) return V[0 - d - 1];
	return V[hash(d, key) % V.length];
}
*/


// -webkit-margin-before,webkit-margin-after,-webkit-margin-start,-webkit-margin-end,-webkit-columns ,-webkit-text-emphasis ,-webkit-text-security ,-webkit-align-self ,-webkit-user-modify ,-webkit-user-select ,-webkit-rtl-ordering ,-webkit-writing-mode ,-webkit-appearance ,-webkit-padding-after ,-webkit-padding-before ,-webkit-padding-end ,-webkit-padding-start 


function pad( x, n, ch=' ' ) {
	
	x = ''+x;
	while( x.length<n ) {
		x = ch+x;
	}

	return x;
}


let __cpp_output = '#include "html.h"\n\nnamespace litehtml {\n\n'+
					'int 	string_find( const tchar_t* text, int len, const tchar_t* inputs, int def );\n'+
					'int 	hash_find( const tchar_t* text, int len, int* GT, int GT_len, int* VT, int VT_len, int* RH, RH_len, int def );\n\n';

let __h_output = '#ifndef __KEYWORDS_H\n#define __KEYWORDS_H\n\n\nnamespace litehtml {\n\n';
					

function generate( input, output_name, sort_it, gen_zero ) {

	input	= input.split(';');
	if( sort_it ) {
		input.sort();
	}
		
	{
		let line = '';
		let text = '\n\n// -------------------------------------------------\n\n';
		let x;
		
		if( input.length>7 )  {
		
			let tables = create_hash(input);
			console.log( output_name+': '+tables[0].length );
		
			line = '';
			text = 'int '+output_name+'_GT[] = {\n';
			
			for ( x = 0; x < tables[0].length; x++) {

				if (x > 0 && (x % 20 == 0)) {
					text += '\t\t' + line + '\n';
					line = '';
				}

				let t = tables[0][x];
				line += pad( (t === undefined ? 0 : t), 4) + ', ';
			}

			text += '\t\t' + line + '\n\t};\n\n';
			
			////////////////////////

			text += 'int '+output_name+'_VT[] = {\n';
			line = '';

			for ( x = 0; x < tables[1].length; x++) {

				if (x > 0 && (x % 20 == 0)) {
					text += '\t\t' + line + '\n';
					line = '';
				}

				let vpos = tables[1][x];
				if( vpos===undefined )  {
					vpos = 0;
				}
				line += pad(vpos,4) + ', ';
			}
			
			text += '\t\t' + line + '\n\t};\n\n';

			///////////////////

			text += 'uint32_t	'+output_name+'_RH[] = {\n';
			line = '';

			for ( x = 0; x < input.length - 1; x++) {

				if (x > 0 && (x % 10 == 0)) {
					text += '\t\t' + line + ' \\\n';
					line = '';
				}

				line += pad( '0x' + hash(0, input[x]).toString(16), 8 ) + ', ';
			}

			text += '\t\t' + line + '\n';
			text += '};\n\n';
		}
		
		//////////////////////

		if( input.length>7 )  {
			text += '/*\n';
		}

		text += '\nstatic const char '+output_name+'_input[] = {\n';
		line = '';
		
		for ( x = 0; x < input.length; x++) {
		
			//if (x > 0 && (x % 10 == 0)) {
			//	text += '\t\t\'' + line + '\' \\\n';
			//	line = '';
			//}
		
			let v = input[x];
			if (v.length > 0) {
				text += '\t"\\x' + v.length.toString(16) + '""' + v + '" \\\n';
			}
		}
		text += '};\n';
		
		if( input.length>7 )  {
			text += '*/\n';
		}
			
		text += '\nint get_'+output_name+'( const tchar_t* text, int len, int def )\n{\n';

		if( input.length>7 )  {
			text += '	return hash_find( text, len, '+output_name+'_GT, countof('+output_name+'_GT),\n\t\t\t\t\t\t'+output_name+'_VT, countof('+output_name+'_VT),\n\t\t\t\t\t\t'+output_name+'_RH, countof('+output_name+'_RH), def );\n';
		}
		else {
			text += '	return string_find( text, len, '+output_name+'_input, def );\n';
		}

		text += '}\n\n\n';

		__cpp_output += text;
	}
	
	//////////////////

	{
		text = '\n\n// -------------------------------------------------\n\n';

		//text += '#ifndef __' + output_name.toUpperCase( ) + '\n';
		//text += '#define __' + output_name.toUpperCase( ) + '\n\n';
		
		text += 'int get_'+output_name+'( const tchar_t* text, int len = -1, int def = 0 );\n\n';
		
		text += 'enum '+output_name+' {\n';

		if( gen_zero ) {
			text += '	'+output_name+'_null = 0,\n';
		}

		for (var x = 0; x < input.length; x++ ) {

			let v = input[x];

			if (v.length) {
				let name = v.replace(/[-:]/gi, '_');
				name = name.replace(/[*]/gi, 'star');
				name = name.replace(/[%]/gi, 'perc');
				text += '	'+output_name+'_' + name + ' = ' + x + ',\n';
			}
		}

		text += '};\n\n';
		//text += '#endif //__' + output_name.toUpperCase( ) + '\n';

		__h_output += text;
	}

	console.log( "done" );
}





generate( 'none;all;screen;print;braille;embossed;handheld;projection;speech;tty;tv',
	'media_type', false, false  );


generate( 'pseudo-el;before;after;pseudo;id;class;inherit;auto;br;p;table;td;th;img;link;title;a;tr;style;base;body;div;script;' +
	'font;::before;::after;tbody;thead;tfoot;*;href;src;text-align;display;color;face;font-face;size;font-size;height;width;rel;media;cellspacing;cellpadding;border-spacing;' +
	'border-width;border;bgcolor;background-color;background;valign;vertical-align;border-left-style;border-right-style;border-top-style;border-bottom-style;border-left-width;' +
	'border-right-width;border-top-width;border-bottom-width;border-left-color;border-right-color;border-top-color;border-bottom-color;colspan;rowspan;border-collapse;text-transform;' +
	'position;overflow;white-space;visibility;box-sizing;z-index;float;clear;text-indent;min-width;min-height;max-width;max-height;line-height;left;top;right;bottom;margin;' +
	'margin-left;margin-top;margin-right;margin-bottom;padding;padding-left;padding-top;padding-right;padding-bottom;border-left;border-top;border-right;border-bottom;' +
	'border-style;border-color;border-radius;border-radius-x;border-radius-y;border-top-left-radius;border-top-left-radius-x;border-top-left-radius-y;border-top-right-radius;' +
	'border-top-right-radius-x;border-top-right-radius-y;border-bottom-right-radius;border-bottom-right-radius-x;border-bottom-right-radius-y;border-bottom-left-radius;' +
	'border-bottom-left-radius-x;border-bottom-left-radius-y;list-style;list-style-type;list-style-position;list-style-image;list-style-baseurl;background-position;background-size;' +
	'background-attachment;background-repeat;background-clip;background-origin;background-image;background-baseurl;font-family;font-variant;font-weight;font-style;text-decoration;' +
	'cursor;content;-xx-border-spacing-x;-xx-border-spacing-y;head;center;meta;layer;article;aside;footer;header;hgroup;main;nav;section;marquee;address;blockquote;figcaption;' +
	'figure;q;hr;map;object-fit;video;h1;h2;h3;h4;h5;h6;col;colgroup;caption;ul;menu;dir;ol;li;dd;dl;dt;form;label;legend;fieldset;button;input;textarea;keygen;select;meter;progress;' +
	'text-rendering;letter-spacing;word-spacing;text-shadow;type;align-items;direction;opacity;pointer-events;align-self;flex-direction;resize;word-wrap;optgroup;option;' +
	'disabled;outline;outline-offset;datalist;area;param;list;readonly;overflow-x;overflow-y;output;u;ins;strong;b;i;cite;em;var;dfn;tt;code;kbd;samp;pre;xmp;plaintext;listing;' +
	'mark;big;small;s;strike;del;sub;sup;nobr;html;embed;iframe;object;ruby;rt;rp;noframes;frameset;frame;details;summary;template;unicode-bidi;bdi;bdo;dialog;lang;charset;' +
	'name;role;abbr;datetime;time;alt;controls;audio;canvas;value;min;max;svg;cx;cy;r;fill;circle;for;placeholder;checked;rows;cols;span;nowrap;isindex;flex-grow;flex-basis;background-position-x;' +
	'background-position-y;word-break;table-layout;zoom;text-overflow;box-shadow;justify-content;user_drag;tabindex;halign;column-gap;flex-wrap;column-width;column-count;align-content;' +
	'flex-flow;transparent',
	//'none;block;inline;inline-block;inline-table;list-item;table-caption;table-cell;table-column;table-column-group;table-footer-group;table-header-group;table-row;table-row-group;flex;' +
	//'static;relative;absolute;fixed;capitalize;uppercase;lowercase;border-box;padding-box;content-box;repeat;repeat-x;repeat-y;no-repeat;' +
	//'justify;normal;pre-line;pre-wrap;visible;hidden;scroll;no-display;no-content;cover;contain;collapse;separate;only-child;only-of-type;first-child;first-of-type;last-child;last-of-type;' +
	//'nth-child;nth-of-type;nth-last-child;nth-last-of-type;not;open-quote;close-quote;no-open-quote;no-close-quote;portrait;landscape;' +
	//'device-width;min-device-width;max-device-width;device-height;min-device-height;max-device-height;orientation;aspect-ratio;min-aspect-ratio;max-aspect-ratio;device-aspect-ratio;' +
	//'min-device-aspect-ratio;max-device-aspect-ratio;min-color;max-color;color-index;min-color-index;max-color-index;monochrome;min-monochrome;max-monochrome;resolution;min-resolution;max-resolution;' +
	//'xx-small;x-small;medium;large;x-large;xx-large;smaller;larger;italic;small-caps;' +
	//'disc;square;armenian;cjk-ideographic;decimal;decimal-leading-zero;georgian;hebrew;hiragana;hiragana-iroha;katakana;katakana-iroha;lower-alpha;lower-greek;lower-latin;lower-roman;upper-alpha;upper-latin;upper-roman;' +
	//'inside;outside;baseline;super;text-top;middle;text-bottom;thin;thick;dotted;dashed;solid;double;groove;ridge;inset;outset;both;%;in;cm;mm;ex;pt;pc;px;dpi;dpcm;vw;vh;vmin;vmax;' +
	//'screen;print;braille;embossed;handheld;projection;speech;tty;tv;',
	'atom', true, true 
);


generate( 'none;block;inline;inline-block;inline-table;list-item;table;table-caption;table-cell;table-column;table-column-group;table-footer-group;table-header-group;table-row;table-row-group;inline-text;flex;',
	'style_display', false, false 
);

generate( 'xx-small;x-small;small;medium;large;x-large;xx-large;smaller;larger;',
	'font_size', false, false 
);

generate( 'none;circle;disc;square;armenian;cjk-ideographic;decimal;decimal-leading-zero;georgian;hebrew;hiragana;hiragana-iroha;katakana;katakana-iroha;lower-alpha;lower-greek;lower-latin;lower-roman;upper-alpha;upper-latin;upper-roman;',
	'list_style_type', false, false
);

generate( 'baseline;sub;super;top;text-top;middle;bottom;text-bottom;',
	'vertical_align', false, false 
);

generate( 'none;hidden;dotted;dashed;solid;double;groove;ridge;inset;outset;',
	'border_style', false, false 
);

generate( 'normal;italic;',
	'font_style', false, false  );

generate( 'normal;small-caps;',
	'font_variant', false, false  );

generate( 'thin;medium;thick;',
	'border_width', false, false  );

generate( 'none;left;right;',
	'element_float', false, false  );

generate( 'none;left;right;both;',
	'element_clear', false, false  );

generate( 'none;%;in;cm;mm;em;ex;pt;pc;px;dpi;dpcm;vw;vh;vmin;vmax;',
	'css_units', false, false  );

generate( 'scroll;fixed;',
	'background_attachment', false, false  );

generate( 'repeat;repeat-x;repeat-y;no-repeat;',
	'background_repeat', false, false  );

generate( 'border-box;padding-box;content-box;',
	'background_box', false, false  );

generate( 'static;relative;absolute;fixed;',
	'element_position', false, false  );

generate( 'left;right;center;justify;',
	'text_align', false, false  );

generate( 'none;capitalize;uppercase;lowercase;',
	'text_transform', false, false  );

generate( 'normal;nowrap;pre;pre-line;pre-wrap;', 
	'white_space', false, false  );

generate( 'visible;hidden;scroll;auto;no-display;no-content;', 
	'overflow', false, false  );

generate( 'auto;cover;contain;',
	'background_size', false, false  );

generate( 'visible;hidden;collapse;',
	'visibility', false, false  );

generate( 'collapse;separate;', 
	'border_collapse', false, false  );

generate( 'only-child;only-of-type;first-child;first-of-type;last-child;last-of-type;nth-child;nth-of-type;nth-last-child;nth-last-of-type;not;lang;',
	'pseudo_class', false, false  );

generate( 'none;normal;open-quote;close-quote;no-open-quote;no-close-quote;',
	'content_property', false, false  );

generate( 'portrait;landscape;',
	'media_orientation', false, false  );

generate( 'none;width;min-width;max-width;height;min-height;max-height;device-width;min-device-width;max-device-width;device-height;min-device-height;max-device-height;orientation;aspect-ratio;min-aspect-ratio;max-aspect-ratio;device-aspect-ratio;min-device-aspect-ratio;max-device-aspect-ratio;color;min-color;max-color;color-index;min-color-index;max-color-index;monochrome;min-monochrome;max-monochrome;resolution;min-resolution;max-resolution;',
	'media_feature', false, false  );

generate( 'content-box;border-box;',
	'box_sizing', false, false  );

generate( 'transparent;aliceblue;antiquewhite;aqua;aquamarine;azure;beige;bisque;black;blanchedalmond;blue;blueviolet;brown;burlywood;cadetblue;chartreuse;chocolate;coral;cornflowerblue;cornsilk;crimson;'+
			'cyan;darkblue;darkcyan;darkgoldenrod;darkgray;darkgrey;darkgreen;darkkhaki;darkmagenta;darkolivegreen;darkorange;;darkorchid;darkred;darksalmon;darkseagreen;darkslateblue;darkslategray;darkslategrey;'+
			'darkturquoise;darkviolet;deeppink;deepskyblue;dimgray;dimgrey;dodgerblue;firebrick;floralwhite;forestgreen;fuchsia;gainsboro;ghostwhite;gold;goldenrod;gray;grey;green;greenyellow;honeydew;hotpink;'+
			'ivory;khaki;lavender;lavenderblush;lawngreen;lemonchiffon;lightblue;lightcoral;lightcyan;lightgoldenrodyellow;lightgray;lightgrey;lightgreen;lightpink;lightsalmon;lightseagreen;lightskyblue;lightslategray;'+
			'lightslategrey;lightsteelblue;lightyellow;lime;limegreen;linen;magenta;maroon;mediumaquamarine;mediumblue;mediumorchid;mediumpurple;mediumseagreen;mediumslateblue;mediumspringgreen;mediumturquoise;mediumvioletred;'+
			'midnightblue;mintcream;mistyrose;moccasin;navajowhite;navy;oldlace;olive;olivedrab;orange;orangered;orchid;palegoldenrod;palegreen;paleturquoise;palevioletred;papayawhip;peachpuff;peru;pink;plum;powderblue;'+
			'purple;red;rosybrown;royalblue;saddlebrown;salmon;sandybrown;seagreen;seashell;sienna;silver;skyblue;slateblue;slategray;slategrey;snow;springgreen;steelblue;tan;teal;thistle;tomato;turquoise;violet;wheat;'+
			'white;whitesmoke;yellow;yellowgreen;',
	'web_colors', false, false );

generate( 'inside;outside;',
	'list_style_position', false, false  );

generate( 'normal;bold;bolder;lighter;100;200;300;400;500;600;700;',
	'font_weight', false, false  );

generate( 'none;hidden;dotted;dashed;solid;double;',
	'style_border', false, false );	//<why ?

generate( 'underline;line-through;overline;',
	'text_decoration', false, false );

generate( 'left;right;top,bottom;center;',
	'background_position', false, false );
	
__cpp_output += '\n\n}\n';
__h_output += '\n\n}\n\n#endif';


var file;

file = fs.openSync('src/_keywords.cpp', 'w' );
fs.writeSync( file, __cpp_output );
fs.closeSync( file );
		
file = fs.openSync('src/_keywords.h', 'w' );
fs.writeSync( file, __h_output );
fs.closeSync( file );
		