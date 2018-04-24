#pragma once
#include "style.h"
#include "media_query.h"

namespace litehtml
{
	//////////////////////////////////////////////////////////////////////////

	struct selector_specificity
	{
		int		a;
		int		b;
		int		c;
		int		d;

		selector_specificity(int va = 0, int vb = 0, int vc = 0, int vd = 0)
		{
			a	= va;
			b	= vb;
			c	= vc;
			d	= vd;
		}

		void operator += (const selector_specificity& val)
		{
			a	+= val.a;
			b	+= val.b;
			c	+= val.c;
			d	+= val.d;
		}

		bool operator==(const selector_specificity& val) const
		{
			return (a == val.a && b == val.b && c == val.c && d == val.d);
		}

		bool operator!=(const selector_specificity& val) const
		{
			return (a != val.a || b != val.b || c != val.c || d != val.d);
		}

		bool operator > (const selector_specificity& val) const
		{
			if(a > val.a)	{return true;}
			if(a < val.a)	{return false;}
			if(b > val.b)	{return true;}
			if(b < val.b)	{return false;}
			if(c > val.c)	{return true;}
			if(c < val.c)	{return false;}
			if(d > val.d)	{return true;}
			if(d < val.d)	{return false;}

			return false;
		}

		bool operator >= (const selector_specificity& val) const
		{
			if( (*this) == val) return true;
			if( (*this) > val) return true;
			return false;
		}

		bool operator <= (const selector_specificity& val) const
		{
			if( (*this) > val ) {return false;}
			return true;
		}

		bool operator < (const selector_specificity& val) const
		{
			if( (*this)<=val && (*this) != val ) {
				return true;
			}

			return false;
		}
	};

	//////////////////////////////////////////////////////////////////////////

	enum attr_select_condition
	{
		select_exists,
		select_equal,
		select_contain_str,
		select_start_str,
		select_end_str,
		select_pseudo_class,
		select_pseudo_element,
	};

	//////////////////////////////////////////////////////////////////////////

	struct css_attribute_selector
	{
		typedef std::vector<css_attribute_selector>	vector;

		atom					attribute;
		tstring					val;
		string_vector			class_val;
		attr_select_condition	condition;

		css_attribute_selector()
		{
			condition = select_exists;
		}
	};

	//////////////////////////////////////////////////////////////////////////

	class css_element_selector
	{
	public:
		atom	m_tag;
		css_attribute_selector::vector	m_attrs;

	public:
		void parse(const tstring& txt);
	};

	//////////////////////////////////////////////////////////////////////////

	enum css_combinator
	{
		combinator_descendant,
		combinator_child,
		combinator_adjacent_sibling,
		combinator_general_sibling
	};

	//////////////////////////////////////////////////////////////////////////

	class css_selector
	{
	//public:
		//typedef std::shared_ptr<css_selector>	ptr;
		//typedef std::vector<css_selector::ptr>	vector;

	public:
		selector_specificity	m_specificity;
		css_element_selector	m_right;
		css_selector*			m_left;
		css_combinator			m_combinator;
		style*					m_style;
		int						m_order;
		media_query_list*		m_media_query;

	public:
		css_selector(media_query_list* media, style* st = NULL )
		{
			m_media_query	= media;
			m_combinator	= combinator_descendant;
			m_order			= 0;
			m_left			= NULL;
			m_style			= st;
		}

		~css_selector()
		{
		}

		css_selector(const css_selector& val)
		{
			m_right			= val.m_right;
			if( val.m_left ) {
				m_left			= new css_selector( *val.m_left );
			}
			else {
				m_left = NULL;
			}

			m_combinator	= val.m_combinator;
			m_specificity	= val.m_specificity;
			m_order			= val.m_order;
			m_media_query	= val.m_media_query;
		}

		bool parse(const tstring& text);
		void calc_specificity();
		bool is_media_valid() const;
		void add_media_to_doc(document* doc) const;
	};

	inline bool css_selector::is_media_valid() const
	{
		if(!m_media_query) {
			return true;
		}

		return m_media_query->is_used();
	}


	//////////////////////////////////////////////////////////////////////////

	inline bool operator > (const css_selector& v1, const css_selector& v2)
	{
		if(v1.m_specificity == v2.m_specificity) {
			return (v1.m_order > v2.m_order);
		}

		return (v1.m_specificity > v2.m_specificity);
	}

	inline bool operator < (const css_selector& v1, const css_selector& v2)
	{
		if(v1.m_specificity == v2.m_specificity) {
			return (v1.m_order < v2.m_order);
		}

		return (v1.m_specificity < v2.m_specificity);
	}

	//inline bool operator >(const css_selector*& v1, const css_selector*& v2)
	//{
	//	return (*v1 > *v2);
	//}
	//
	//inline bool operator < (const css_selector* v1, const css_selector* v2)
	//{
	//	return (*v1 < *v2);
	//}

	//////////////////////////////////////////////////////////////////////////

	class used_selector
	{
	//public:
	//	typedef std::unique_ptr<used_selector>	ptr;
	//	typedef std::vector<used_selector::ptr>	vector;

		const css_selector*	m_selector;
		bool		m_used;

		used_selector(const css_selector* selector, bool used)
		{
			m_used		= used;
			m_selector	= selector;
		}
	};
}
