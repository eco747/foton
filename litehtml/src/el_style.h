#pragma once
#include "html_tag.h"

namespace litehtml {
	class el_style : public element {
		elements_vector		m_children;
	public:
		el_style( document* doc );
		virtual ~el_style();

		virtual void			parse_attributes() override;
		virtual bool			appendChild( const ptr& el ) override;
		virtual atom			get_tagName() const override;
	};
}
