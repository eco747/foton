#pragma once
#include "html_tag.h"

namespace litehtml {
	class el_base
		: public html_tag {
	public:
		el_base( document* doc );
		virtual ~el_base();

		virtual void	parse_attributes() override;
	};
}
