export
class DOMElement @ "dom_element_dtor" {

    constructor( tagName ) @ "dom_element_ctor";

    set tagName( value ) @ "dom_element_set_tag_name";
    get tagName( ) @ "dom_element_get_tag_name";

    set left( value ) @ "dom_element_set_left";
    get left( ) @ "dom_element_get_left";

    set top( ) @ "dom_element_set_top";
    get top( ) @ "dom_element_get_top";

    set right( ) @ "dom_element_set_right";
    get right( ) @ "dom_element_get_right";

    set bottom( ) @ "dom_element_set_bottom";
    get bottom( ) @ "dom_element_get_bottom";

    set width( ) @ "dom_element_set_width";
    get width( ) @ "dom_element_get_width";

    set height( ) @ "dom_element_set_height";
    get height( ) @ "dom_element_get_height";

    set margins( value ) @ "dom_element_set_margins";
    get margins() @ "dom_element_get_margins";

    get paddings( value ) @ "dom_element_set_paddings";
    get paddings() @ "dom_element_get_paddings";

    set borders( value ) @ "dom_element_set_borders";
    get borders() @ "dom_element_get_borders";

    set_attr( name, value ) @ "dom_element_set_attr";
    get_attr( name ) @ "dom_element_get_attr";

    set_style( name, value ) @ "dom_element_set_style";
    get_style( name ) @ "dom_element_get_style";

    appendChild( el ) @ "dom_element_append_child";
    removeChild( el ) @ "dom_element_remove_child";
    get parent( ) @ "dom_element_get_parent";
}

Object.freeze(DOMElement.prototype);
