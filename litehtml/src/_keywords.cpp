#include "html.h"

namespace litehtml {

int 	string_find( const tchar_t* text, int len, const tchar_t* inputs, int def );
int 	hash_find( const tchar_t* text, int len, int* GT, int GT_len, int* VT, int VT_len, uint32_t* RH, int RH_len, int def );

int media_type_GT[] = {
		   2,    0,    0,   16,    0,   58,    0,   -4,    0,    1,    0, 
	};

int media_type_VT[] = {
		   9,    1,    4,    8,   11,    2,    7,    5,    6,   10,    3, 
	};

uint32_t	media_type_RH[] = {
		0x2ca33bdc, 0x419c2c6f, 0x2ff97422, 0x4bb743c3, 0x2f0f0cdd, 0x50bd5824, 0x5eff9d38, 0x223f6aef, 0x7d0c8ff4, 0x276ca4a7, 
};

/*

static const char media_type_input[] = {
	"\x4""none" \
	"\x3""all" \
	"\x6""screen" \
	"\x5""print" \
	"\x7""braille" \
	"\x8""embossed" \
	"\x8""handheld" \
	"\xa""projection" \
	"\x6""speech" \
	"\x3""tty" \
	"\x2""tv" \
};
*/

int get_media_type( const tchar_t* text, int len, int def )
{
	return hash_find( text, len, media_type_GT, countof(media_type_GT),
						media_type_VT, countof(media_type_VT),
						media_type_RH, countof(media_type_RH), def );
}


int atom_GT[] = {
		   0,    0,    2,  -62,    0,    0, -122,    0,    0, -133,    2,    0,    0,    0,    9, -161,    5, -165, -167,    0, 
		   0,    2,    5,    5,    4,  -51,    0, -193, -235,    2,    2,    0,    0, -191,    0,    2, -273, -150,    4, -114, 
		 -48,    2,  -15, -304, -288,    0,   10,    0,    0,    0,    1,    0,    0,    0, -185,    0,    1,    0,    2, -145, 
		-128, -127,    0,    1,    0,    0,    0,  -61,    0,    0,    1,    0,    1,    1, -308,    0,    0, -296, -295, -294, 
		   1, -287, -286,    1,    0,    0, -271, -267,    2,    2,    0, -252, -251, -250,    0,    0,    0,    0,    3, -231, 
		-229,    0,    0,    4, -210,    2, -204,    0, -197, -196,    2,    4,   -1,    0, -187,    0,    0,    0, -177,    0, 
		-175, -173,    3,    2,    0,    2, -160, -159,    2, -155,    0, -152,    3,    0,    1, -143, -140,    0,    1,    0, 
		   0,    5, -126, -124,    0, -119, -116,    4,    4, -113, -112,    0, -107, -106,    0, -101,    0,  -96,    0,    0, 
		 -83,    5,   18,    0,  -65,    0,    0,    1,    0,    0,    0,  -58,    0,    0,  -52,    0,    2,  -49,    0,    0, 
		   0,    0,    0,    0,  -19,  -18,    0,  -14,    0,  -11,   -7,    0,    1,  -13,    2,    0,  -26,    0,  -38,    0, 
		  28,    3,  -46,    0,    0,   11,    0,    0,    0,    0,    1,  -55,    0,  -56,  -59,  -60,    0,   12,  -63,    6, 
		   0,  -68,    3,  -95, -100, -105, -108,    2,    1, -132, -139,    1, -153,    3,    2, -182, -184,    7,    0,    1, 
		-200,    0,    0,    0, -218, -219,    0,    0, -242,    1, -248,    0,    0, -249,    2, -272,    0,    0,   10,    2, 
		-307,  -41,    0,    0,  -50,    1,  -64,    0,    0,    0,    0,    0,  -69,    0,  -73,    0,    2, -158, -176,    0, 
		   2, -195,    0, -215, -255, -257, -266,    0, -282,    1,    2,    0,  -39,    0, -164,    0, -208, -190,    1,    3, 
		-147,    5,    2,   11,  -89,    0, -244, -115, 
	};

int atom_VT[] = {
		 122,  111,  176,  256,    8,  266,  127,  147,   12,   88,  203,  202,  215,  233,   37,  238,   25,  230,  276,  149, 
		  21,  270,  304,  193,  129,  192,  180,  269,  209,  287,    9,  128,  104,   85,  247,  126,  279,  301,  137,   13, 
		 178,  211,  142,   39,   33,   55,  169,   56,  234,  198,  130,  218,  283,  261,  217,  138,  284,  188,  288,  220, 
		 140,  139,   90,   49,   72,   68,  189,  295,  166,  210,   52,   51,   26,   48,  265,  246,  302,   19,  134,  102, 
		 133,   95,  200,  165,  101,  179,  121,  151,  113,   54,  159,  194,   28,  259,  119,  156,  146,  235,  143,  154, 
		 252,  231,  109,   91,  207,   10,  260,  117,  219,  225,  162,   82,    5,  124,  195,  258,  308,   93,  273,   92, 
		 187,  120,  206,    6,   70,   20,   69,  221,  214,   24,  163,  106,   73,    2,  182,  201,   57,  244,  107,   80, 
		 170,  297,  161,  253,   63,   78,    3,  184,  293,  116,  272,  305,  241,  112,  132,  223,  229,  296,  239,  240, 
		 227,  172,   34,  271,   60,  164,   59,  242,   35,   74,  243,   83,  103,  267,   65,  289,   84,   18,  278,  185, 
		  31,   15,  285,  152,   75,  197,  263,  232,   62,  123,  208,  118,  175,  158,  183,  306,  280,  236,  100,  148, 
		 226,   99,  250,   42,   67,   29,  245,  105,   58,  141,  262,  145,   61,  216,  299,  257,   77,  168,   89,  255, 
		  87,   66,  294,  204,  291,   32,  251,  224,   43,   38,  292,  277,  205,   50,  248,   97,  307,    7,  181,   76, 
		 213,  190,  174,  110,   47,  153,  177,   53,  136,  212,  274,  155,  115,  131,  268,   79,   11,   16,   96,  167, 
		 286,  222,   64,  150,  191,  228,  199,   14,  108,   45,  290,  135,  303,  281,  114,  186,  157,   44,   22,   46, 
		  23,  264,    1,   36,  237,   40,   98,  144,   41,   30,   71,   94,   17,  254,  125,   86,   81,  173,  282,  249, 
		 196,  298,   27,  160,  171,    4,  275,  300, 
	};

uint32_t	atom_RH[] = {
		0x50c5d36, 0x3fa6a53f, 0x3fa6a540, 0x1a021dce, 0x3ce37cd9, 0x50c5d7f, 0x38de710d, 0x621e09d0, 0x1f545b8e, 0x360ea6cd,  \
		0x386398a5, 0x2e365e60, 0x515d67c8, 0x419c2c77, 0x5bb60647, 0x4b5df356, 0xf2b4764, 0x3fc4085a, 0x4cc737c5, 0x50c5d7e,  \
		0x5999a082, 0x6892511e, 0x70982cb1, 0x72c39625, 0x1cc58c80, 0x3eaf3bd6, 0x31bfdc35, 0x161a0f88, 0x4824eb01, 0x4824eb02,  \
		0x91ba838, 0x15eb1452, 0x4cf9b597, 0x1b99b1f1, 0x1b99b1f7, 0x440dfc99, 0xc7f658e, 0x2699c28e, 0x123ab90d, 0x6dfe676e,  \
		0x79acc564, 0x42565c2e, 0x7c9c27ec, 0x30336511, 0x5a5df5e0, 0x5a5df5df, 0x33b890d4, 0x7a5dba35, 0x7a5dba36, 0x77aaeed2,  \
		0x5244908f, 0x64ec4702, 0x1f0a1232, 0x1f07c160, 0x352dc8e, 0x1dce8ec8, 0x5541b385, 0x2ee58ab7, 0x66ace706, 0x66ace705,  \
		0x64d46c63, 0x1e9e759d, 0x93b7f67, 0x33f9283a, 0x4a4e5448, 0x1b4e332c, 0x21e906, 0x178ae964, 0x4bdbd329, 0x531a7438,  \
		0x531a7437, 0x393b378c, 0x6dd849cd, 0x6dd849ce, 0x7db7513a, 0x15fc8f7, 0x202fb279, 0x6864ba61, 0x49721766, 0x460ea44,  \
		0x6f772bb6, 0x3a42c456, 0x3c6bb66a, 0x67afb8b6, 0x4adcab69, 0x8c95529, 0x76822487, 0x6bb76970, 0x2a6aa4d9, 0xe545ade,  \
		0x67ca2d8d, 0x5e8533dc, 0x688fd0b1, 0x53c5915b, 0x4097ad2e, 0x1ecc0dab, 0x301748db, 0x5bc59da5, 0x6aed9d82, 0x76411876,  \
		0x436a7ddd, 0x2719f4c5, 0x1d9253d9, 0x1c7eaa24, 0x4f507a50, 0x6e772a4d, 0x6e772a4e, 0x42064896, 0x5bb409c7, 0x6d7728c6,  \
		0x16952ce1, 0x2734ea4e, 0x1995312c, 0x49a80b0a, 0x1a9532ab, 0xffcd09, 0x41a03856, 0x35fe47de, 0x1a9532af, 0x6d7728ce,  \
		0x6d7728d6, 0x6c772764, 0x2319d829, 0x20c00293, 0x57a05c5e, 0x5a1edd62, 0x1d46dc56, 0x35ac9c23, 0x57f88aae, 0x115c6b59,  \
		0x27418fea, 0x5ae09255, 0x165df50e, 0x4edcd7aa, 0x33b11623, 0x61e81029, 0x7bb22310, 0x5c152a41, 0x23ecbfa7, 0x12f85837,  \
		0x3216548, 0x1a1dab95, 0x408f5e0f, 0x2fb11068, 0x6e2f4e8d, 0x1d13f85, 0x69772265, 0x69772268, 0x69772267, 0x69772262,  \
		0x69772261, 0x69772264, 0x5ba0e07b, 0x4d887e08, 0x3a199653, 0x4c47d5c1, 0x160b435, 0x69772228, 0x3170f967, 0x5d6233d7,  \
		0x50c5d77, 0x687720a7, 0x3645c762, 0x2288731b, 0x513749fb, 0x561ee000, 0x1f886eb8, 0x7b3bf3e4, 0x280137dc, 0x4b8436bf,  \
		0x3f4d41ed, 0x61867a7e, 0x28346586, 0x71482af1, 0x3eb433, 0x5ecb940b, 0x4db06ca6, 0x65771c61, 0x1ffbfe0a, 0x820aba2,  \
		0xd20b3de, 0x27c730a2, 0x19423a51, 0x62469476, 0x4cbfdea8, 0x344dc69f, 0x780a378c, 0x3c76e6bb, 0x3e7e1246, 0x4a2ae8d4,  \
		0x6ea69dbe, 0x69f93050, 0x6904fe13, 0x7fe019b6, 0x2776c5af, 0x244300d0, 0x3e7e124e, 0x9273d88, 0xd882a2f, 0x3b4d62dc,  \
		0x1b6c25f1, 0x316c4847, 0xf75bf95, 0x467e1ea4, 0x274603a6, 0x55a45c39, 0x2f8b3bf5, 0x167b94d1, 0x28a33501, 0x63c2a2b3,  \
		0x4543a451, 0x361de6b5, 0x65b62077, 0x6277173d, 0x212b5825, 0x26fb0056, 0x2b9d6e4f, 0x6e6ce7aa, 0x4d7f6d38, 0x498d6ac7,  \
		0x56628158, 0x2fb85571, 0x2fb85572, 0x50c5d70, 0x440b8035, 0x57989a5, 0x780c617f, 0x1d0beb1e, 0x2770e7df, 0x4a620ba5,  \
		0x5c4fce4d, 0x3451334f, 0x12a70bd3, 0x5e19d275, 0x3d7754b9, 0x126568e1, 0x4937e75c, 0x2f528ae0, 0x50c5d6f, 0x50c5d6e,  \
		0x12d23a50, 0x3a72d29b, 0x21efca8e, 0x6816f04a, 0x78d05a2a, 0x13d084bf, 0x2361cdc4, 0x5f7712e8, 0x5f7712e4, 0x769a3d56,  \
		0x50c5d6d, 0x30e998a7, 0x23177499, 0x291a199f, 0x556391b6, 0x23fe9f73, 0x6e13411b, 0x3910092a, 0x2b707ca2, 0x7502dd08,  \
		0x51f39bd9, 0x4a5356c1, 0x26707482, 0x8ab57a4, 0x26707494, 0x27707612, 0x34d5d75f, 0x7a1e796c, 0xae95b3f, 0x3787cc5c,  \
		0x5d770f16, 0x4529f970, 0x4de7210d, 0xe7f89e4, 0x45adcd98, 0x20e2e98, 0x16ae8388, 0x58bde79e, 0x19ddf2fc, 0x6a111f7c,  \
		0x53744fa8, 0x5d770f1a, 0x529ea246, 0x1e3669db, 0x5ce0e62e, 0x406ccc4b, 0x5d770f04, 0x188403fa, 0x5d770f06, 0x310f785e,  \
		0x50c5d6b, 0x5c770db3, 0x153b1672, 0xca8a5be, 0x2bf928f1, 0x28ed63dd, 0x2668266d, 0x7e45286c, 0x31cc1af7, 0x4e96dd3a,  \
		0xffca4ea, 0x182e64ec, 0x4924905c, 0x7f911e1e, 0x41ce8b2b, 0x1e639c29, 0x243ce5df, 
};

/*

static const char atom_input[] = {
	"\x1""*" \
	"\x14""-xx-border-spacing-x" \
	"\x14""-xx-border-spacing-y" \
	"\x7""::after" \
	"\x8""::before" \
	"\x1""a" \
	"\x4""abbr" \
	"\x7""address" \
	"\x5""after" \
	"\x5""align" \
	"\xd""align-content" \
	"\xb""align-items" \
	"\xa""align-self" \
	"\x3""alt" \
	"\x4""area" \
	"\x7""article" \
	"\x5""aside" \
	"\x5""audio" \
	"\x4""auto" \
	"\x1""b" \
	"\xa""background" \
	"\x15""background-attachment" \
	"\x12""background-baseurl" \
	"\xf""background-clip" \
	"\x10""background-color" \
	"\x10""background-image" \
	"\x11""background-origin" \
	"\x13""background-position" \
	"\x15""background-position-x" \
	"\x15""background-position-y" \
	"\x11""background-repeat" \
	"\xf""background-size" \
	"\x4""base" \
	"\x3""bdi" \
	"\x3""bdo" \
	"\x6""before" \
	"\x7""bgcolor" \
	"\x3""big" \
	"\xa""blockquote" \
	"\x4""body" \
	"\x6""border" \
	"\xd""border-bottom" \
	"\x13""border-bottom-color" \
	"\x19""border-bottom-left-radius" \
	"\x1b""border-bottom-left-radius-x" \
	"\x1b""border-bottom-left-radius-y" \
	"\x1a""border-bottom-right-radius" \
	"\x1c""border-bottom-right-radius-x" \
	"\x1c""border-bottom-right-radius-y" \
	"\x13""border-bottom-style" \
	"\x13""border-bottom-width" \
	"\xf""border-collapse" \
	"\xc""border-color" \
	"\xb""border-left" \
	"\x11""border-left-color" \
	"\x11""border-left-style" \
	"\x11""border-left-width" \
	"\xd""border-radius" \
	"\xf""border-radius-x" \
	"\xf""border-radius-y" \
	"\xc""border-right" \
	"\x12""border-right-color" \
	"\x12""border-right-style" \
	"\x12""border-right-width" \
	"\xe""border-spacing" \
	"\xc""border-style" \
	"\xa""border-top" \
	"\x10""border-top-color" \
	"\x16""border-top-left-radius" \
	"\x18""border-top-left-radius-x" \
	"\x18""border-top-left-radius-y" \
	"\x17""border-top-right-radius" \
	"\x19""border-top-right-radius-x" \
	"\x19""border-top-right-radius-y" \
	"\x10""border-top-style" \
	"\x10""border-top-width" \
	"\xc""border-width" \
	"\x6""bottom" \
	"\xa""box-shadow" \
	"\xa""box-sizing" \
	"\x2""br" \
	"\x6""button" \
	"\x6""canvas" \
	"\x7""caption" \
	"\xb""cellpadding" \
	"\xb""cellspacing" \
	"\x6""center" \
	"\x7""charset" \
	"\x7""checked" \
	"\x6""circle" \
	"\x4""cite" \
	"\x5""class" \
	"\x5""clear" \
	"\x4""code" \
	"\x3""col" \
	"\x8""colgroup" \
	"\x5""color" \
	"\x4""cols" \
	"\x7""colspan" \
	"\xc""column-count" \
	"\xa""column-gap" \
	"\xc""column-width" \
	"\x7""content" \
	"\x8""controls" \
	"\x6""cursor" \
	"\x2""cx" \
	"\x2""cy" \
	"\x8""datalist" \
	"\x8""datetime" \
	"\x2""dd" \
	"\x3""del" \
	"\x7""details" \
	"\x3""dfn" \
	"\x6""dialog" \
	"\x3""dir" \
	"\x9""direction" \
	"\x8""disabled" \
	"\x7""display" \
	"\x3""div" \
	"\x2""dl" \
	"\x2""dt" \
	"\x2""em" \
	"\x5""embed" \
	"\x4""face" \
	"\x8""fieldset" \
	"\xa""figcaption" \
	"\x6""figure" \
	"\x4""fill" \
	"\xa""flex-basis" \
	"\xe""flex-direction" \
	"\x9""flex-flow" \
	"\x9""flex-grow" \
	"\x9""flex-wrap" \
	"\x5""float" \
	"\x4""font" \
	"\x9""font-face" \
	"\xb""font-family" \
	"\x9""font-size" \
	"\xa""font-style" \
	"\xc""font-variant" \
	"\xb""font-weight" \
	"\x6""footer" \
	"\x3""for" \
	"\x4""form" \
	"\x5""frame" \
	"\x8""frameset" \
	"\x2""h1" \
	"\x2""h2" \
	"\x2""h3" \
	"\x2""h4" \
	"\x2""h5" \
	"\x2""h6" \
	"\x6""halign" \
	"\x4""head" \
	"\x6""header" \
	"\x6""height" \
	"\x6""hgroup" \
	"\x2""hr" \
	"\x4""href" \
	"\x4""html" \
	"\x1""i" \
	"\x2""id" \
	"\x6""iframe" \
	"\x3""img" \
	"\x7""inherit" \
	"\x5""input" \
	"\x3""ins" \
	"\x7""isindex" \
	"\xf""justify-content" \
	"\x3""kbd" \
	"\x6""keygen" \
	"\x5""label" \
	"\x4""lang" \
	"\x5""layer" \
	"\x4""left" \
	"\x6""legend" \
	"\xe""letter-spacing" \
	"\x2""li" \
	"\xb""line-height" \
	"\x4""link" \
	"\x4""list" \
	"\xa""list-style" \
	"\x12""list-style-baseurl" \
	"\x10""list-style-image" \
	"\x13""list-style-position" \
	"\xf""list-style-type" \
	"\x7""listing" \
	"\x4""main" \
	"\x3""map" \
	"\x6""margin" \
	"\xd""margin-bottom" \
	"\xb""margin-left" \
	"\xc""margin-right" \
	"\xa""margin-top" \
	"\x4""mark" \
	"\x7""marquee" \
	"\x3""max" \
	"\xa""max-height" \
	"\x9""max-width" \
	"\x5""media" \
	"\x4""menu" \
	"\x4""meta" \
	"\x5""meter" \
	"\x3""min" \
	"\xa""min-height" \
	"\x9""min-width" \
	"\x4""name" \
	"\x3""nav" \
	"\x4""nobr" \
	"\x8""noframes" \
	"\x6""nowrap" \
	"\x6""object" \
	"\xa""object-fit" \
	"\x2""ol" \
	"\x7""opacity" \
	"\x8""optgroup" \
	"\x6""option" \
	"\x7""outline" \
	"\xe""outline-offset" \
	"\x6""output" \
	"\x8""overflow" \
	"\xa""overflow-x" \
	"\xa""overflow-y" \
	"\x1""p" \
	"\x7""padding" \
	"\xe""padding-bottom" \
	"\xc""padding-left" \
	"\xd""padding-right" \
	"\xb""padding-top" \
	"\x5""param" \
	"\xb""placeholder" \
	"\x9""plaintext" \
	"\xe""pointer-events" \
	"\x8""position" \
	"\x3""pre" \
	"\x8""progress" \
	"\x6""pseudo" \
	"\x9""pseudo-el" \
	"\x1""q" \
	"\x1""r" \
	"\x8""readonly" \
	"\x3""rel" \
	"\x6""resize" \
	"\x5""right" \
	"\x4""role" \
	"\x4""rows" \
	"\x7""rowspan" \
	"\x2""rp" \
	"\x2""rt" \
	"\x4""ruby" \
	"\x1""s" \
	"\x4""samp" \
	"\x6""script" \
	"\x7""section" \
	"\x6""select" \
	"\x4""size" \
	"\x5""small" \
	"\x4""span" \
	"\x3""src" \
	"\x6""strike" \
	"\x6""strong" \
	"\x5""style" \
	"\x3""sub" \
	"\x7""summary" \
	"\x3""sup" \
	"\x3""svg" \
	"\x8""tabindex" \
	"\x5""table" \
	"\xc""table-layout" \
	"\x5""tbody" \
	"\x2""td" \
	"\x8""template" \
	"\xa""text-align" \
	"\xf""text-decoration" \
	"\xb""text-indent" \
	"\xd""text-overflow" \
	"\xe""text-rendering" \
	"\xb""text-shadow" \
	"\xe""text-transform" \
	"\x8""textarea" \
	"\x5""tfoot" \
	"\x2""th" \
	"\x5""thead" \
	"\x4""time" \
	"\x5""title" \
	"\x3""top" \
	"\x2""tr" \
	"\xb""transparent" \
	"\x2""tt" \
	"\x4""type" \
	"\x1""u" \
	"\x2""ul" \
	"\xc""unicode-bidi" \
	"\x9""user_drag" \
	"\x6""valign" \
	"\x5""value" \
	"\x3""var" \
	"\xe""vertical-align" \
	"\x5""video" \
	"\xa""visibility" \
	"\xb""white-space" \
	"\x5""width" \
	"\xa""word-break" \
	"\xc""word-spacing" \
	"\x9""word-wrap" \
	"\x3""xmp" \
	"\x7""z-index" \
	"\x4""zoom" \
};
*/

int get_atom( const tchar_t* text, int len, int def )
{
	return hash_find( text, len, atom_GT, countof(atom_GT),
						atom_VT, countof(atom_VT),
						atom_RH, countof(atom_RH), def );
}


int style_display_GT[] = {
		   0,    0,   -9,   -8,    0,    0,    5,    1,    0,    8,    0,    1,    3,   -5,    0,  -12,  -14,    0, 
	};

int style_display_VT[] = {
		  15,    3,    6,   10,    4,   11,   18,    2,    5,   14,   17,   16,    7,   13,    1,    8,   12,    9, 
	};

uint32_t	style_display_RH[] = {
		0x2ca33bdc, 0x6e8347b1, 0x5c488033, 0x4b96d333, 0xcd1c71a, 0x1c3e93f0, 0x7a1e796c, 0xacbc653, 0x2ace1119, 0x56026dc3,  \
		0x51573239, 0x521025e4, 0x2505eb1e, 0x57ec9071, 0x48bc2dbb, 0x49864707, 0x2ab95689, 
};

/*

static const char style_display_input[] = {
	"\x4""none" \
	"\x5""block" \
	"\x6""inline" \
	"\xc""inline-block" \
	"\xc""inline-table" \
	"\x9""list-item" \
	"\x5""table" \
	"\xd""table-caption" \
	"\xa""table-cell" \
	"\xc""table-column" \
	"\x12""table-column-group" \
	"\x12""table-footer-group" \
	"\x12""table-header-group" \
	"\x9""table-row" \
	"\xf""table-row-group" \
	"\xb""inline-text" \
	"\x4""flex" \
};
*/

int get_style_display( const tchar_t* text, int len, int def )
{
	return hash_find( text, len, style_display_GT, countof(style_display_GT),
						style_display_VT, countof(style_display_VT),
						style_display_RH, countof(style_display_RH), def );
}


int font_size_GT[] = {
		 -10,   -9,    0,   -7,   -5,    0,    4,   -1,   -3,   -8, 
	};

int font_size_VT[] = {
		   4,    8,    6,    7,   10,    2,    5,    9,    3,    1, 
	};

uint32_t	font_size_RH[] = {
		0x17cd390, 0x54148496, 0x6e13411b, 0x29d28bb1, 0x7f5e06bf, 0x4f88757e, 0x5eefc704, 0x4e1f7ba4, 0x3f049d69, 
};

/*

static const char font_size_input[] = {
	"\x8""xx-small" \
	"\x7""x-small" \
	"\x5""small" \
	"\x6""medium" \
	"\x5""large" \
	"\x7""x-large" \
	"\x8""xx-large" \
	"\x7""smaller" \
	"\x6""larger" \
};
*/

int get_font_size( const tchar_t* text, int len, int def )
{
	return hash_find( text, len, font_size_GT, countof(font_size_GT),
						font_size_VT, countof(font_size_VT),
						font_size_RH, countof(font_size_RH), def );
}


int list_style_type_GT[] = {
		   0,  -19,    1,    0,    0,    1,  -22,    0,  -15,    1,  -13,  -12,    0,  -10,   -4,    5,    1,    0,    0,  -14, 
		   3,  -11, 
	};

int list_style_type_VT[] = {
		   2,   18,   22,    9,    6,   19,   17,   20,   14,   12,   11,    4,   15,    3,   16,   10,    1,    5,    8,    7, 
		  13,   21, 
	};

uint32_t	list_style_type_RH[] = {
		0x2ca33bdc, 0xe545ade, 0x3dec353, 0x6c619019, 0x545d9301, 0x58dabb94, 0x42f988ab, 0x19529487, 0x3500098c, 0x36c8fe7d,  \
		0x1176182d, 0x6cdc6ad3, 0x6411ff24, 0x329dfef2, 0x323031a0, 0x2c921ebc, 0x5ae2998e, 0x1bc2b639, 0x2aa34bc1, 0x7308ce3b,  \
		0x408e6d1c, 
};

/*

static const char list_style_type_input[] = {
	"\x4""none" \
	"\x6""circle" \
	"\x4""disc" \
	"\x6""square" \
	"\x8""armenian" \
	"\xf""cjk-ideographic" \
	"\x7""decimal" \
	"\x14""decimal-leading-zero" \
	"\x8""georgian" \
	"\x6""hebrew" \
	"\x8""hiragana" \
	"\xe""hiragana-iroha" \
	"\x8""katakana" \
	"\xe""katakana-iroha" \
	"\xb""lower-alpha" \
	"\xb""lower-greek" \
	"\xb""lower-latin" \
	"\xb""lower-roman" \
	"\xb""upper-alpha" \
	"\xb""upper-latin" \
	"\xb""upper-roman" \
};
*/

int get_list_style_type( const tchar_t* text, int len, int def )
{
	return hash_find( text, len, list_style_type_GT, countof(list_style_type_GT),
						list_style_type_VT, countof(list_style_type_VT),
						list_style_type_RH, countof(list_style_type_RH), def );
}


int vertical_align_GT[] = {
		   0,    3,    0,   -1,    0,    2,    7,   -3,    0, 
	};

int vertical_align_VT[] = {
		   2,    8,    7,    6,    5,    1,    4,    3,    9, 
	};

uint32_t	vertical_align_RH[] = {
		0x953b7b1, 0x26707482, 0x39d34b97, 0x406ccc4b, 0x73144b8b, 0x3d311767, 0x6864ba61, 0x18e077a1, 
};

/*

static const char vertical_align_input[] = {
	"\x8""baseline" \
	"\x3""sub" \
	"\x5""super" \
	"\x3""top" \
	"\x8""text-top" \
	"\x6""middle" \
	"\x6""bottom" \
	"\xb""text-bottom" \
};
*/

int get_vertical_align( const tchar_t* text, int len, int def )
{
	return hash_find( text, len, vertical_align_GT, countof(vertical_align_GT),
						vertical_align_VT, countof(vertical_align_VT),
						vertical_align_RH, countof(vertical_align_RH), def );
}


int border_style_GT[] = {
		   0,  -11,    2,    0,   -9,   -7,   -2,    0,  -10,    0,    1, 
	};

int border_style_VT[] = {
		  10,    4,    6,   11,    5,    9,    1,    2,    7,    3,    8, 
	};

uint32_t	border_style_RH[] = {
		0x2ca33bdc, 0x57e16576, 0x265c64ba, 0x19d62747, 0x2c61fe2f, 0x27c6b047, 0x7eee3704, 0x91962a3, 0x56272e25, 0x398f906e, 
};

/*

static const char border_style_input[] = {
	"\x4""none" \
	"\x6""hidden" \
	"\x6""dotted" \
	"\x6""dashed" \
	"\x5""solid" \
	"\x6""double" \
	"\x6""groove" \
	"\x5""ridge" \
	"\x5""inset" \
	"\x6""outset" \
};
*/

int get_border_style( const tchar_t* text, int len, int def )
{
	return hash_find( text, len, border_style_GT, countof(border_style_GT),
						border_style_VT, countof(border_style_VT),
						border_style_RH, countof(border_style_RH), def );
}




// -------------------------------------------------


static const char font_style_input[] = {
	"\x6""normal" \
	"\x6""italic" \
};

int get_font_style( const tchar_t* text, int len, int def )
{
	return string_find( text, len, font_style_input, def );
}




// -------------------------------------------------


static const char font_variant_input[] = {
	"\x6""normal" \
	"\xa""small-caps" \
};

int get_font_variant( const tchar_t* text, int len, int def )
{
	return string_find( text, len, font_variant_input, def );
}




// -------------------------------------------------


static const char border_width_input[] = {
	"\x4""thin" \
	"\x6""medium" \
	"\x5""thick" \
};

int get_border_width( const tchar_t* text, int len, int def )
{
	return string_find( text, len, border_width_input, def );
}




// -------------------------------------------------


static const char element_float_input[] = {
	"\x4""none" \
	"\x4""left" \
	"\x5""right" \
};

int get_element_float( const tchar_t* text, int len, int def )
{
	return string_find( text, len, element_float_input, def );
}




// -------------------------------------------------


static const char element_clear_input[] = {
	"\x4""none" \
	"\x4""left" \
	"\x5""right" \
	"\x4""both" \
};

int get_element_clear( const tchar_t* text, int len, int def )
{
	return string_find( text, len, element_clear_input, def );
}


int css_units_GT[] = {
		   0,    4,   -9,    1,   -8,    0,    0,    0,    0,   -6,    2,   22,    0,    0,   16,    0,    1, 
	};

int css_units_VT[] = {
		   5,   11,   12,   17,    7,    6,   14,    4,   16,   13,    1,    9,   15,    8,    3,    2,   10, 
	};

uint32_t	css_units_RH[] = {
		0x2ca33bdc, 0x50c5d3b, 0x687720ad, 0x6e772a5a, 0x64771a1c, 0x6c772764, 0x6c772777, 0x617715ca, 0x617715df, 0x617715c6,  \
		0x2395416b, 0x63f5ea4e, 0x5b770c3d, 0x5b770c24, 0x64e9f310, 0x5ce9e6b2, 
};

/*

static const char css_units_input[] = {
	"\x4""none" \
	"\x1""%" \
	"\x2""in" \
	"\x2""cm" \
	"\x2""mm" \
	"\x2""em" \
	"\x2""ex" \
	"\x2""pt" \
	"\x2""pc" \
	"\x2""px" \
	"\x3""dpi" \
	"\x4""dpcm" \
	"\x2""vw" \
	"\x2""vh" \
	"\x4""vmin" \
	"\x4""vmax" \
};
*/

int get_css_units( const tchar_t* text, int len, int def )
{
	return hash_find( text, len, css_units_GT, countof(css_units_GT),
						css_units_VT, countof(css_units_VT),
						css_units_RH, countof(css_units_RH), def );
}




// -------------------------------------------------


static const char background_attachment_input[] = {
	"\x6""scroll" \
	"\x5""fixed" \
};

int get_background_attachment( const tchar_t* text, int len, int def )
{
	return string_find( text, len, background_attachment_input, def );
}




// -------------------------------------------------


static const char background_repeat_input[] = {
	"\x6""repeat" \
	"\x8""repeat-x" \
	"\x8""repeat-y" \
	"\x9""no-repeat" \
};

int get_background_repeat( const tchar_t* text, int len, int def )
{
	return string_find( text, len, background_repeat_input, def );
}




// -------------------------------------------------


static const char background_box_input[] = {
	"\xa""border-box" \
	"\xb""padding-box" \
	"\xb""content-box" \
};

int get_background_box( const tchar_t* text, int len, int def )
{
	return string_find( text, len, background_box_input, def );
}




// -------------------------------------------------


static const char element_position_input[] = {
	"\x6""static" \
	"\x8""relative" \
	"\x8""absolute" \
	"\x5""fixed" \
};

int get_element_position( const tchar_t* text, int len, int def )
{
	return string_find( text, len, element_position_input, def );
}




// -------------------------------------------------


static const char text_align_input[] = {
	"\x4""left" \
	"\x5""right" \
	"\x6""center" \
	"\x7""justify" \
};

int get_text_align( const tchar_t* text, int len, int def )
{
	return string_find( text, len, text_align_input, def );
}




// -------------------------------------------------


static const char text_transform_input[] = {
	"\x4""none" \
	"\xa""capitalize" \
	"\x9""uppercase" \
	"\x9""lowercase" \
};

int get_text_transform( const tchar_t* text, int len, int def )
{
	return string_find( text, len, text_transform_input, def );
}




// -------------------------------------------------


static const char white_space_input[] = {
	"\x6""normal" \
	"\x6""nowrap" \
	"\x3""pre" \
	"\x8""pre-line" \
	"\x8""pre-wrap" \
};

int get_white_space( const tchar_t* text, int len, int def )
{
	return string_find( text, len, white_space_input, def );
}




// -------------------------------------------------


static const char overflow_input[] = {
	"\x7""visible" \
	"\x6""hidden" \
	"\x6""scroll" \
	"\x4""auto" \
	"\xa""no-display" \
	"\xa""no-content" \
};

int get_overflow( const tchar_t* text, int len, int def )
{
	return string_find( text, len, overflow_input, def );
}




// -------------------------------------------------


static const char background_size_input[] = {
	"\x4""auto" \
	"\x5""cover" \
	"\x7""contain" \
};

int get_background_size( const tchar_t* text, int len, int def )
{
	return string_find( text, len, background_size_input, def );
}




// -------------------------------------------------


static const char visibility_input[] = {
	"\x7""visible" \
	"\x6""hidden" \
	"\x8""collapse" \
};

int get_visibility( const tchar_t* text, int len, int def )
{
	return string_find( text, len, visibility_input, def );
}




// -------------------------------------------------


static const char border_collapse_input[] = {
	"\x8""collapse" \
	"\x8""separate" \
};

int get_border_collapse( const tchar_t* text, int len, int def )
{
	return string_find( text, len, border_collapse_input, def );
}


int pseudo_class_GT[] = {
		   0,    1,    0,   -8,   -6,    4,   -5,   -3,   -2,  -11,    0,    0,    3, 
	};

int pseudo_class_VT[] = {
		   1,    3,    5,    6,   13,   12,    9,    8,    2,    4,   11,   10,    7, 
	};

uint32_t	pseudo_class_RH[] = {
		0xa412d27, 0x2c546141, 0x5e5f3db9, 0x7a86e80f, 0x6a3efe1d, 0x1e7d2c4b, 0x461c3c75, 0x57d3cac3, 0x2a36df68, 0x75b2ee96,  \
		0x207ba411, 0x28346586, 
};

/*

static const char pseudo_class_input[] = {
	"\xa""only-child" \
	"\xc""only-of-type" \
	"\xb""first-child" \
	"\xd""first-of-type" \
	"\xa""last-child" \
	"\xc""last-of-type" \
	"\x9""nth-child" \
	"\xb""nth-of-type" \
	"\xe""nth-last-child" \
	"\x10""nth-last-of-type" \
	"\x3""not" \
	"\x4""lang" \
};
*/

int get_pseudo_class( const tchar_t* text, int len, int def )
{
	return hash_find( text, len, pseudo_class_GT, countof(pseudo_class_GT),
						pseudo_class_VT, countof(pseudo_class_VT),
						pseudo_class_RH, countof(pseudo_class_RH), def );
}




// -------------------------------------------------


static const char content_property_input[] = {
	"\x4""none" \
	"\x6""normal" \
	"\xa""open-quote" \
	"\xb""close-quote" \
	"\xd""no-open-quote" \
	"\xe""no-close-quote" \
};

int get_content_property( const tchar_t* text, int len, int def )
{
	return string_find( text, len, content_property_input, def );
}




// -------------------------------------------------


static const char media_orientation_input[] = {
	"\x8""portrait" \
	"\x9""landscape" \
};

int get_media_orientation( const tchar_t* text, int len, int def )
{
	return string_find( text, len, media_orientation_input, def );
}


int media_feature_GT[] = {
		   3,    0,   -6,  -13,    0,    0,    0,    0,    0,    1,    2,  -25,    4,    1,  -15,    0,   -9,    0,   -5,    0, 
		   0,   -1,    0,    3,    1,    2,  -27,  -23,  -22,    0,    0,    1,   10, 
	};

int media_feature_VT[] = {
		  11,   20,   33,   15,   28,   21,    6,    4,   23,   17,   32,   25,   30,   22,   12,    5,   14,   19,    3,   31, 
		  13,   29,    1,   16,    7,    2,    9,   18,   27,    8,   10,   26,   24, 
	};

uint32_t	media_feature_RH[] = {
		0x2ca33bdc, 0x182e64ec, 0x55a45c39, 0xd882a2f, 0x4c47d5c1, 0x274603a6, 0x9273d88, 0x1291dc0d, 0x556b328a, 0x63e98a74,  \
		0x15834d02, 0x48823ca3, 0xaa74a9, 0x1a6871b2, 0x5d5cf56c, 0x47fa0cf3, 0x313361a9, 0x2bb0a08f, 0x2dd24e32, 0x3bd4fd54,  \
		0x301748db, 0x520453f2, 0x512c758c, 0x33a60836, 0x6f1e83cb, 0x4a0bfd1, 0x5d0432f, 0x4549bdb0, 0x78e6624e, 0x29ea3c40,  \
		0x3d36ed03, 0x57c4fc05, 
};

/*

static const char media_feature_input[] = {
	"\x4""none" \
	"\x5""width" \
	"\x9""min-width" \
	"\x9""max-width" \
	"\x6""height" \
	"\xa""min-height" \
	"\xa""max-height" \
	"\xc""device-width" \
	"\x10""min-device-width" \
	"\x10""max-device-width" \
	"\xd""device-height" \
	"\x11""min-device-height" \
	"\x11""max-device-height" \
	"\xb""orientation" \
	"\xc""aspect-ratio" \
	"\x10""min-aspect-ratio" \
	"\x10""max-aspect-ratio" \
	"\x13""device-aspect-ratio" \
	"\x17""min-device-aspect-ratio" \
	"\x17""max-device-aspect-ratio" \
	"\x5""color" \
	"\x9""min-color" \
	"\x9""max-color" \
	"\xb""color-index" \
	"\xf""min-color-index" \
	"\xf""max-color-index" \
	"\xa""monochrome" \
	"\xe""min-monochrome" \
	"\xe""max-monochrome" \
	"\xa""resolution" \
	"\xe""min-resolution" \
	"\xe""max-resolution" \
};
*/

int get_media_feature( const tchar_t* text, int len, int def )
{
	return hash_find( text, len, media_feature_GT, countof(media_feature_GT),
						media_feature_VT, countof(media_feature_VT),
						media_feature_RH, countof(media_feature_RH), def );
}




// -------------------------------------------------


static const char box_sizing_input[] = {
	"\xb""content-box" \
	"\xa""border-box" \
};

int get_box_sizing( const tchar_t* text, int len, int def )
{
	return string_find( text, len, box_sizing_input, def );
}


int web_colors_GT[] = {
		   4,    3,    0,    0,    0,  -36,    1,    0,    0, -129,    1, -142,  -84,  -78,    1,  -88,    0,    2,    0,  -43, 
		 -45,    0,  -60,  -67,    2,    0,    2,  -81,    1,  -91,    0, -106,    0, -125, -134,    0,    1,  -65,    0,    3, 
		   0,    0,    0, -119,    0,    0,    1, -104, -102,  -97,  -93,    0,    5,    0,  -85,    1,    1,  -80,    1,    0, 
		   0,    0,    0, -147,    8,  -57,  -52,    0,  -48,    0,    0,  -44,    4,  -42,    0,    0,    0,    2,  -32,    0, 
		   0,  -21,  -20,   -9,   -8,   -6,   -5,    1,    0,    1,    1,    0,    0,  -22,    3,    0,    0,    1,  -38,  -39, 
		   2,    3,  -51,    5,    0,    0,  -68,  -71,  -77,    0,    0,    0,    0,  -87,    6,   -1, -117,    0,    1,    3, 
		   0, -132,    0,    0,    1,    0, -140,    0,    0, -108,    2,    0,    0,    4,    0,  -47,   -4,    1,    1,    2, 
		 -34,    0,  -27,    0,    0,   -2,    0, 
	};

int web_colors_VT[] = {
		 146,   91,   66,  113,   57,   97,   98,  132,   79,  109,   36,   60,  131,   53,   39,   29,   47,  121,    1,  117, 
		 137,  118,  122,  103,   99,  119,   20,   11,  123,   45,   90,  148,    6,   32,   64,  134,   78,   92,  139,  144, 
		  88,    9,   76,  100,   37,   80,   77,   86,    2,   52,   81,    3,   95,   49,  133,   31,   96,  110,   21,   58, 
		  89,  129,  135,   84,   73,   72,   69,   22,  120,   71,  128,   55,   24,   93,    5,   63,   75,   14,  108,  147, 
		  44,   38,  127,  143,    7,   41,    4,   26,   35,   42,   56,  104,   13,   12,   19,    8,  101,   67,  106,  140, 
		 125,   17,  136,   34,   40,  112,  124,   15,   54,   43,   70,   62,  114,   27,  145,   10,  105,   59,  107,  111, 
		 141,   30,   94,   87,   18,   50,   68,  115,   48,   82,  102,   65,  130,   23,   51,  138,   46,   83,   74,   28, 
		  25,   85,   61,   16,  126,  116,  142, 
	};

uint32_t	web_colors_RH[] = {
		0x188403fa, 0x398cb1b4, 0x70b40c5c, 0x3bbe22ee, 0x539b07d4, 0x5d7e68a3, 0x1efc714c, 0x3c1a0895, 0x5e609c03, 0x7fa0640e,  \
		0x4f06856a, 0x32ba34f1, 0x120504bc, 0x7c146e5f, 0x68ab91ed, 0x6dc33ede, 0x30fc3e2e, 0x12262395, 0x87e212b, 0x262ba32d,  \
		0xe3269e1, 0x7aa33c0d, 0x28d1d012, 0x341ea3e5, 0x342a0582, 0x11d75bcd, 0xdd75511, 0x55fae18b, 0x400e95b6, 0x706c772b,  \
		0x2e7b2770, 0x5b0803e0, 0x11c9dc6, 0x7c068659, 0x6440813b, 0x537c2430, 0x877a3e4, 0x3cf160e5, 0x3de4c9b6, 0x39e4c3fa,  \
		0x5887a8c5, 0x5021739d, 0x5fb68bc, 0x632e791f, 0x177cad5, 0x7d77c419, 0xaa42781, 0x4a19765, 0x2f690c8f, 0xa25c5b0,  \
		0x3351fd75, 0x4f0b9470, 0x66f6734e, 0x26da85b0, 0x91b43fa, 0x360868e5, 0x32086229, 0x773297b3, 0x2fa93f2b, 0x5bc4053,  \
		0x731028cf, 0x3c16bf99, 0x1bff99ae, 0x7015a325, 0x413a5e4b, 0x2afa8359, 0x7dfe2244, 0x66064d04, 0x17ec245b, 0x9b67fb3,  \
		0x7c50aef4, 0x43d0cda3, 0x47d0d45f, 0x4fbe2389, 0x31fb9654, 0x5d1aef42, 0xb1c9822, 0x54e965c7, 0x66b2b89c, 0x62b2b2f0,  \
		0xe00e575, 0x1d167f48, 0x720aa53, 0x75296f7c, 0x7a6e4414, 0x6db1bfe3, 0x643e990e, 0x38f53df3, 0x3207b965, 0x431c643a,  \
		0x3a4cf05f, 0x92c8b83, 0x77755a88, 0x6c84753d, 0x5326903c, 0x5634767, 0x453800fe, 0x3291719a, 0x29444dbf, 0x35aede3f,  \
		0x3e0a9214, 0x348b430a, 0x195d1602, 0x45336b67, 0x5c524202, 0x513aa128, 0xc245995,  0xcf2b1, 0x148deef2, 0x28e828fb,  \
		0x3282d115, 0x76086f22, 0x2e1c9b6, 0x445de34e, 0x67f1997c, 0x73e71efe, 0x5ee041b0, 0x7c7cd76b, 0x34f6dfd8, 0x3a72d293,  \
		0x228f50fd, 0x76e1f763, 0x4ed6e511, 0x6f3dd378, 0x56144901, 0x39823fc, 0x713c33f9, 0x67068eb4, 0x539137c3, 0x36bcf121,  \
		0x140e479d, 0x5529250e, 0x51291f52, 0x2ef661d5, 0x32da46fc, 0x201c8b5b, 0x326cb65f, 0x4a2c21cc, 0x3359a9c3, 0x3ff99a78,  \
		0x4fbd0ffd, 0x2a99acf5, 0x306ead5f, 0x3d77bb99, 0x34ad9e2c, 0x255a881a, 0x9a5939f, 
};

/*

static const char web_colors_input[] = {
	"\xb""transparent" \
	"\x9""aliceblue" \
	"\xc""antiquewhite" \
	"\x4""aqua" \
	"\xa""aquamarine" \
	"\x5""azure" \
	"\x5""beige" \
	"\x6""bisque" \
	"\x5""black" \
	"\xe""blanchedalmond" \
	"\x4""blue" \
	"\xa""blueviolet" \
	"\x5""brown" \
	"\x9""burlywood" \
	"\x9""cadetblue" \
	"\xa""chartreuse" \
	"\x9""chocolate" \
	"\x5""coral" \
	"\xe""cornflowerblue" \
	"\x8""cornsilk" \
	"\x7""crimson" \
	"\x4""cyan" \
	"\x8""darkblue" \
	"\x8""darkcyan" \
	"\xd""darkgoldenrod" \
	"\x8""darkgray" \
	"\x8""darkgrey" \
	"\x9""darkgreen" \
	"\x9""darkkhaki" \
	"\xb""darkmagenta" \
	"\xe""darkolivegreen" \
	"\xa""darkorange" \
	"\xa""darkorchid" \
	"\x7""darkred" \
	"\xa""darksalmon" \
	"\xc""darkseagreen" \
	"\xd""darkslateblue" \
	"\xd""darkslategray" \
	"\xd""darkslategrey" \
	"\xd""darkturquoise" \
	"\xa""darkviolet" \
	"\x8""deeppink" \
	"\xb""deepskyblue" \
	"\x7""dimgray" \
	"\x7""dimgrey" \
	"\xa""dodgerblue" \
	"\x9""firebrick" \
	"\xb""floralwhite" \
	"\xb""forestgreen" \
	"\x7""fuchsia" \
	"\x9""gainsboro" \
	"\xa""ghostwhite" \
	"\x4""gold" \
	"\x9""goldenrod" \
	"\x4""gray" \
	"\x4""grey" \
	"\x5""green" \
	"\xb""greenyellow" \
	"\x8""honeydew" \
	"\x7""hotpink" \
	"\x5""ivory" \
	"\x5""khaki" \
	"\x8""lavender" \
	"\xd""lavenderblush" \
	"\x9""lawngreen" \
	"\xc""lemonchiffon" \
	"\x9""lightblue" \
	"\xa""lightcoral" \
	"\x9""lightcyan" \
	"\x14""lightgoldenrodyellow" \
	"\x9""lightgray" \
	"\x9""lightgrey" \
	"\xa""lightgreen" \
	"\x9""lightpink" \
	"\xb""lightsalmon" \
	"\xd""lightseagreen" \
	"\xc""lightskyblue" \
	"\xe""lightslategray" \
	"\xe""lightslategrey" \
	"\xe""lightsteelblue" \
	"\xb""lightyellow" \
	"\x4""lime" \
	"\x9""limegreen" \
	"\x5""linen" \
	"\x7""magenta" \
	"\x6""maroon" \
	"\x10""mediumaquamarine" \
	"\xa""mediumblue" \
	"\xc""mediumorchid" \
	"\xc""mediumpurple" \
	"\xe""mediumseagreen" \
	"\xf""mediumslateblue" \
	"\x11""mediumspringgreen" \
	"\xf""mediumturquoise" \
	"\xf""mediumvioletred" \
	"\xc""midnightblue" \
	"\x9""mintcream" \
	"\x9""mistyrose" \
	"\x8""moccasin" \
	"\xb""navajowhite" \
	"\x4""navy" \
	"\x7""oldlace" \
	"\x5""olive" \
	"\x9""olivedrab" \
	"\x6""orange" \
	"\x9""orangered" \
	"\x6""orchid" \
	"\xd""palegoldenrod" \
	"\x9""palegreen" \
	"\xd""paleturquoise" \
	"\xd""palevioletred" \
	"\xa""papayawhip" \
	"\x9""peachpuff" \
	"\x4""peru" \
	"\x4""pink" \
	"\x4""plum" \
	"\xa""powderblue" \
	"\x6""purple" \
	"\x3""red" \
	"\x9""rosybrown" \
	"\x9""royalblue" \
	"\xb""saddlebrown" \
	"\x6""salmon" \
	"\xa""sandybrown" \
	"\x8""seagreen" \
	"\x8""seashell" \
	"\x6""sienna" \
	"\x6""silver" \
	"\x7""skyblue" \
	"\x9""slateblue" \
	"\x9""slategray" \
	"\x9""slategrey" \
	"\x4""snow" \
	"\xb""springgreen" \
	"\x9""steelblue" \
	"\x3""tan" \
	"\x4""teal" \
	"\x7""thistle" \
	"\x6""tomato" \
	"\x9""turquoise" \
	"\x6""violet" \
	"\x5""wheat" \
	"\x5""white" \
	"\xa""whitesmoke" \
	"\x6""yellow" \
	"\xb""yellowgreen" \
};
*/

int get_web_colors( const tchar_t* text, int len, int def )
{
	return hash_find( text, len, web_colors_GT, countof(web_colors_GT),
						web_colors_VT, countof(web_colors_VT),
						web_colors_RH, countof(web_colors_RH), def );
}




// -------------------------------------------------


static const char list_style_position_input[] = {
	"\x6""inside" \
	"\x7""outside" \
};

int get_list_style_position( const tchar_t* text, int len, int def )
{
	return string_find( text, len, list_style_position_input, def );
}


int font_weight_GT[] = {
		   0,  -10,    1,    0,   -7,    5,   -5,    6,    0,   -4,   -8,  -12, 
	};

int font_weight_VT[] = {
		   5,    2,    6,   11,   12,    7,    8,    3,    9,    1,   10,    4, 
	};

uint32_t	font_weight_RH[] = {
		0x4527c499, 0x65fe5a19, 0x35723926, 0x10d5c847, 0x74d60a9f, 0x6ed3c296, 0x68d17aad, 0x62ce6924, 0x7ccc535b, 0x76ca0b52,  \
		0x70c6f9e9, 
};

/*

static const char font_weight_input[] = {
	"\x6""normal" \
	"\x4""bold" \
	"\x6""bolder" \
	"\x7""lighter" \
	"\x3""100" \
	"\x3""200" \
	"\x3""300" \
	"\x3""400" \
	"\x3""500" \
	"\x3""600" \
	"\x3""700" \
};
*/

int get_font_weight( const tchar_t* text, int len, int def )
{
	return hash_find( text, len, font_weight_GT, countof(font_weight_GT),
						font_weight_VT, countof(font_weight_VT),
						font_weight_RH, countof(font_weight_RH), def );
}




// -------------------------------------------------


static const char style_border_input[] = {
	"\x4""none" \
	"\x6""hidden" \
	"\x6""dotted" \
	"\x6""dashed" \
	"\x5""solid" \
	"\x6""double" \
};

int get_style_border( const tchar_t* text, int len, int def )
{
	return string_find( text, len, style_border_input, def );
}




// -------------------------------------------------


static const char text_decoration_input[] = {
	"\x9""underline" \
	"\xc""line-through" \
	"\x8""overline" \
};

int get_text_decoration( const tchar_t* text, int len, int def )
{
	return string_find( text, len, text_decoration_input, def );
}




// -------------------------------------------------


static const char background_position_input[] = {
	"\x4""left" \
	"\x5""right" \
	"\xa""top,bottom" \
	"\x6""center" \
};

int get_background_position( const tchar_t* text, int len, int def )
{
	return string_find( text, len, background_position_input, def );
}




}
