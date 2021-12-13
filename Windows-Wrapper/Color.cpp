#include "Color.h"
#include "Type.h"

#include <sstream>

Color Color::FromName(const std::string& name)
{
	auto color = LookupNamedColor(name);
	if(color == KnownColor::Unknown) color = LookupSystemColor(name);

	if(color != KnownColor::Unknown) return color;

	return Color(NotDefinedValue, StateNameValid, name, KnownColor::Unknown);
}

bool Color::Equals(const Object* const obj) const
{
	if(obj != nullptr)
	{
		if(const auto right = dynamic_cast<const Color*>(obj))
		{
			if(m_Value == right->m_Value
			   && m_State == right->m_State
			   && m_KnownColor == right->m_KnownColor)
			{
				if(m_Name.compare(right->m_Name)) return true;
			}
		}
	}

	return false;
}

int Color::GetHashCode() const noexcept
{
	return ((int)((long)m_Value)) ^ (int)(m_Value >> 32)						// m_Value (int GetHashCode())
		^ ((int)((unsigned short)m_State) | (((int)m_State) << 16))				// m_State (short GetHashCode())
		^ ((int)((unsigned short)m_KnownColor) | (((int)m_KnownColor) << 16));	// m_KnownColor (short GetHashCode())
}

std::string Color::ToString() const noexcept
{
	std::ostringstream oss;

	oss << GetType().ToString();
	oss << "[";

	if(m_State & StateNameValid != 0) oss << m_Name;
	else if(m_State & StateKnownColorValid != 0) oss << m_Name;
	else if(m_State & StateValueMask != 0)
	{
		oss << "A=" << GetA() << ", R=" << GetR() << ", G=" << GetG() << ", B=" << GetB();
	}
	else
	{
		oss << "Empty";
	}

	oss << "]";

	return oss.str();
}
