#include "../../Include/core/_property.hpp"

int _property_symbol::m_basic_symbol = 0;

_property_symbol::_property_symbol()noexcept
	: symbol(m_basic_symbol++)
{}

