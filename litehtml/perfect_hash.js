// Perfect minimal hashing
//stevehanov.ca/blog/index.php?id=119

// Calculates a distinct hash function for a given string. Each value of the
// integer d results in a different hash value.
function hash( d, str) {
  if(d == 0) { d = 0x811c9dc5; }
  for(var i = 0; i < str.length; i++) {
//en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
//isthe.com/chongo/src/fnv/hash_32.c
    // multiply by the 32 bit FNV magic prime mod 2^32
    d += (d << 1) + (d << 4) + (d << 7) + (d << 8) + (d << 24);
    // xor the bottom with the current octet
    d ^= str.charCodeAt(i);
  }
  return d & 0x7fffffff;
}

// Computes a minimal perfect hash table using the given Javascript object hash. It
// returns a tuple (G, V). G and V are both arrays. G contains the intermediate
// table of values needed to compute the index of the value in V. V contains the
// values of the dictionary.

function create_hash( inp ) {

	var dict = {};
	var positions = [];
	var ctxt = "";
	var pos = 0;
	var index = -1;
	
	for( var j=0; j<inp.length; j++ ) {

		if( inp[j]==',' ) {
			positions.push( pos );
			dict[ctxt] = index--;
			
			pos = j+1;
			ctxt = "";
		}
		else {
			ctxt += inp[j];
		}
	}

	console.log( Object.keys(dict).length );

  var size = Object.keys(dict).length,
      buckets = [],
      G = new Array(size),
      values = new Array(size),
      i, b, bucket;

  // Place all of the keys into buckets
  Object.keys(dict).forEach(function(key) {
    var bkey = hash(0, key) % size;
    if(!buckets[bkey]) {
      buckets[bkey] = [];
    }
    buckets[bkey].push( key );
  });

  // Sort the buckets and process the ones with the most items first.
  buckets.sort(function(a, b) { return b.length - a.length; });

  for(b = 0; b < size; b++) {
    if(buckets[b].length <= 1) break;
    bucket = buckets[b];

    var d = 1, item = 0, slots = [], slot, used = {};

    // Repeatedly try different values of d until we find a hash function
    // that places all items in the bucket into free slots
    while(item < bucket.length) {
      slot = hash(d, bucket[item]) % size;
      if(values[slot] || used[slot]) {
        d++;
        item = 0;
        slots = [];
        used = {};
      } else {
        used[slot] = true;
        slots.push(slot);
        item++;
      }
    }

    G[hash(0, bucket[0]) % size] = d;
    for(i = 0; i < bucket.length; i++) {
      values[slots[i]] = dict[bucket[i]];
    }
  }

  // Only buckets with 1 item remain. Process them more quickly by directly
  // placing them into a free slot. Use a negative value of d to indicate
  // this.

  var freelist = [];
  for(i = 0; i < size; i++) {
    if(typeof values[i] == 'undefined' ) {
      freelist.push(i);
    }
  }

  for(; b < size; b++ ) {
    if (!buckets[b] || buckets[b].length == 0) break;
    bucket = buckets[b];
    slot = freelist.pop();

    // We subtract one to ensure it's negative even if the zeroeth slot was used.
    G[hash(0, bucket[0]) % size] = 0-slot-1;
    values[slot] = dict[bucket[0]];
  }

  return [ G, values ];
};


// Look up a value in the hash table, defined by G and V.
function lookup_hash(G, V, key) {
  var d = G[ hash(0,key) % G.length ];
  if (d < 0) return V[ 0-d-1 ];
  return V[hash(d, key) % V.length];
};









let input = "pseudo-el,before,after,pseudo,id,class,inherit,underline,line-through,overline,auto,br,p,table,td,th,img,link,title,a,tr,style,base,body,div,script,"+
 "font,::before,::after,tbody,thead,tfoot,*,href,src,align,text-align,display,color,face,font-face,size,font-size,height,width,rel,media,cellspacing,cellpadding,border-spacing,"+
 "border-width,border,bgcolor,background-color,background,valign,vertical-align,border-left-style,border-right-style,border-top-style,border-bottom-style,border-left-width,"+
 "border-right-width,border-top-width,border-bottom-width,border-left-color,border-right-color,border-top-color,border-bottom-color,colspan,rowspan,border-collapse,text-transform,"+
 "position,overflow,white-space,visibility,box-sizing,z-index,float,clear,text-indent,min-width,min-height,max-width,max-height,line-height,left,top,right,bottom,margin,"+
 "margin-left,margin-top,margin-right,margin-bottom,padding,padding-left,padding-top,padding-right,padding-bottom,border-left,border-top,border-right,border-bottom,"+
 "border-style,border-color,border-radius,border-radius-x,border-radius-y,border-top-left-radius,border-top-left-radius-x,border-top-left-radius-y,border-top-right-radius,"+
 "border-top-right-radius-x,border-top-right-radius-y,border-bottom-right-radius,border-bottom-right-radius-x,border-bottom-right-radius-y,border-bottom-left-radius,"+
 "border-bottom-left-radius-x,border-bottom-left-radius-y,list-style,list-style-type,list-style-position,list-style-image,list-style-baseurl,background-position,background-size,"+
 "background-attachment,background-repeat,background-clip,background-origin,background-image,background-baseurl,font-family,font-variant,font-weight,font-style,text-decoration,"+
 "cursor,content,-xx-border-spacing-x,-xx-border-spacing-y,head,center,meta,layer,article,aside,footer,header,hgroup,main,nav,section,marquee,address,blockquote,figcaption,"+
 "figure,q,hr,map,object-fit,video,h1,h2,h3,h4,h5,h6,col,colgroup,caption,ul,menu,dir,ol,li,dd,dl,dt,form,label,legend,fieldset,button,input,textarea,keygen,select,meter,progress,"+
 "text-rendering,letter-spacing,word-spacing,text-shadow,type,align-items,direction,opacity,pointer-events,align-self,flex-direction,resize,word-wrap,optgroup,option,"+
 "disabled,outline,outline-offset,datalist,area,param,list,readonly,overflow-x,overflow-y,output,u,ins,strong,b,i,cite,em,var,dfn,tt,code,kbd,samp,pre,xmp,plaintext,listing,"+
 "mark,big,small,s,strike,del,sub,sup,nobr,html,embed,iframe,object,ruby,rt,rp,noframes,frameset,frame,details,summary,template,unicode-bidi,bdi,bdo,dialog,lang,charset,"+
 "name,role,abbr,datetime,time,alt,controls,audio,canvas,value,min,max,svg,cx,cy,r,fill,circle,for,placeholder,checked,rows,cols,span,nowrap,isindex,flex-grow,flex-basis,background-position-x,"+
 "background-position-y,word-break,table-layout,zoom,text-overflow,box-shadow,justify-content,user_drag,tabindex,halign,column-gap,flex-wrap,column-width,column-count,align-content,"+
 "flex-flow,transparent,"+
 "none,block,inline,inline-block,inline-table,list-item,table-caption,table-cell,table-column,table-column-group,table-footer-group,table-header-group,table-row,table-row-group,flex,"+
 "static,relative,absolute,fixed,"+
 "capitalize,uppercase,lowercase,"+
 "border-box,padding-box,content-box"+
 "repeat,repeat-x,repeat-y,no-repeat";

// -webkit-margin-before,webkit-margin-after,-webkit-margin-start,-webkit-margin-end,-webkit-columns ,-webkit-text-emphasis ,-webkit-text-security ,-webkit-align-self ,-webkit-user-modify ,-webkit-user-select ,-webkit-rtl-ordering ,-webkit-writing-mode ,-webkit-appearance ,-webkit-padding-after ,-webkit-padding-before ,-webkit-padding-end ,-webkit-padding-start 
 

input = input.split( "," );
input.sort( );
input = input.join( "," );




 tables = create_hash(input);

let line = "";

console.log( "	int GT[] = {")
for( var x=0; x<tables[0].length; x++ ) {

	if( x>0 && (x%20==0) ) {
		console.log( "\t\t"+line );
		line = "";
	}

	let t = tables[0][x];
	line += (t===undefined ? 0 : t)  + ", ";
}
console.log( "\t\t" + line + "\n\t};")


////////////////////////

console.log( "	int VT[] = {" )
line = "";

for( var x=0; x<tables[1].length; x++ ) {
	
	if( x>0 && (x%20==0) ) {
		console.log( "\t\t"+line );
		line = "";
	}

	let vpos = tables[1][x];
	line += vpos+", ";
}
console.log( "\t\t" + line + "\n\t};")


//////////////////////

let 	all_in = input.split( ',' );

console.log( "\nstatic const char input[] = " );
line = "";

for( var x=0; x<all_in.length; x++ ) {
	
	if( x>0 && (x%10==0) ) {
		console.log( "\t\t\""+line+"\" \\" );
		line = "";
	}

	let v = all_in[x];
	if( v.length>0 ) {
		line += v+"\\0";
	}
}
console.log( "\t\t\""+line+"\";" );

///////////////////

console.log( "\n	uint32_t	RH[] = {" );
line = "";

for( var x=0; x<all_in.length-1; x++ ) {

	if( x>0 && (x%10==0) ) {
		console.log( "\t\t"+line+" \\" );
		line = "";
	}

	line += "0x" + hash(0,all_in[x]).toString(16) + "," ;
}

console.log( "\t\t"+line+" \\" );
console.log( "};");


//////////////////

console.log( "\nenum atom {" );
console.log( "\tatom_null = 0," );

for( var x=0, y=-1; x<all_in.length; x++, y-- ) {

	let v = all_in[x];

	if( v.length ) {
		let name = v.replace( /[-:]/gi, "_" );
			name = name.replace( /[*]/gi, 'star' );
		console.log( "\tatom_"+name+" = "+y+"," );
	}
}

console.log( "};" );


/*
console.log( lookup_hash( tables[0], tables[1], "head" ));

Object.keys(input).forEach(function(key) {
	console.log('key: ' + key + ' value: ' +  lookup_hash( tables[0], tables[1], key));
});


*/
