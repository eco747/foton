#pragma once
#include "attributes.h"
#include <string>

namespace litehtml
{
	class	property_value
	{
	public:
		xstring		m_value;
		bool		m_important;

		property_value()
		{
			m_important = false;
		}

		property_value( const tchar_t* val, bool imp )
		{
			m_important = imp;
			m_value		= val;
		}

		property_value(const property_value& val)
		{
			m_important	= val.m_important;
			m_value		= val.m_value;
		}

		property_value& operator=(const property_value& val)
		{
			m_important	= val.m_important;
			m_value		= val.m_value;
			return *this;
		}
	};

	typedef std::map<atom, property_value>	props_map;

	class style
	{
	public:
		typedef std::shared_ptr<style>		ptr;
		typedef std::vector<style::ptr>		vector;

	private:
		props_map			m_properties;
		static attr_map		m_valid_values;

	public:
		style();
		style(const style& val);
		virtual ~style();

		void operator=(const style& val)
		{
			m_properties = val.m_properties;
		}

		void add(const tchar_t* txt, const tchar_t* baseurl)
		{
			parse(txt, baseurl);
		}

		void add_property(atom name, const tchar_t* val, const tchar_t* baseurl, bool important);

		const tchar_t* get_property(atom name) const
		{
			if(name)
			{
				props_map::const_iterator f = m_properties.find(name);
				if(f != m_properties.end())
				{
					return f->second.m_value.c_str();
				}
			}
			return 0;
		}

		void combine(const style& src);
		void clear()
		{
			m_properties.clear();
		}

	private:
		void parse_property(const xstring& txt, const tchar_t* baseurl);
		void parse(const tchar_t* txt, const tchar_t* baseurl);
		void parse_short_border(const tstring& prefix, const tstring& val, bool important);
		void parse_short_background(const tstring& val, const tchar_t* baseurl, bool important);
		void parse_short_font(const tstring& val, bool important);
		void add_parsed_property(atom name, const tstring& val, bool important);
		void remove_property(atom name, bool important);
	};
}
