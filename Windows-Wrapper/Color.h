#pragma once

#include "Object.h"
#include "KnownColor.h"
#include "Map.h"

#include <array>
#include <string>
#include <sstream>
#include <cassert>

#define REGISTERKNOWNCOLORMAP(entry) { #entry, (int)entry }
#define REGISTERKNOWNCOLORREVERSEMAP(entry) { (int)entry, #entry }

using namespace std::literals::string_view_literals;

class Color : public Object
{
private:

	static constexpr std::array<std::pair<std::string_view, int>, 142> m_KnownColorMap
	{
		{
			REGISTERKNOWNCOLORMAP(KnownColor::Transparent),
			REGISTERKNOWNCOLORMAP(KnownColor::AliceBlue),
			REGISTERKNOWNCOLORMAP(KnownColor::AntiqueWhite),
			REGISTERKNOWNCOLORMAP(KnownColor::Aqua),
			REGISTERKNOWNCOLORMAP(KnownColor::Aquamarine),
			REGISTERKNOWNCOLORMAP(KnownColor::Azure),
			REGISTERKNOWNCOLORMAP(KnownColor::Beige),
			REGISTERKNOWNCOLORMAP(KnownColor::Bisque),
			REGISTERKNOWNCOLORMAP(KnownColor::Black),
			REGISTERKNOWNCOLORMAP(KnownColor::BlanchedAlmond),
			REGISTERKNOWNCOLORMAP(KnownColor::Blue),
			REGISTERKNOWNCOLORMAP(KnownColor::BlueViolet),
			REGISTERKNOWNCOLORMAP(KnownColor::Brown),
			REGISTERKNOWNCOLORMAP(KnownColor::BurlyWood),
			REGISTERKNOWNCOLORMAP(KnownColor::CadetBlue),
			REGISTERKNOWNCOLORMAP(KnownColor::Chartreuse),
			REGISTERKNOWNCOLORMAP(KnownColor::Chocolate),
			REGISTERKNOWNCOLORMAP(KnownColor::Coral),
			REGISTERKNOWNCOLORMAP(KnownColor::CornflowerBlue),
			REGISTERKNOWNCOLORMAP(KnownColor::Cornsilk),
			REGISTERKNOWNCOLORMAP(KnownColor::Crimson),
			REGISTERKNOWNCOLORMAP(KnownColor::Cyan),
			REGISTERKNOWNCOLORMAP(KnownColor::DarkBlue),
			REGISTERKNOWNCOLORMAP(KnownColor::DarkCyan),
			REGISTERKNOWNCOLORMAP(KnownColor::DarkGoldenrod),
			REGISTERKNOWNCOLORMAP(KnownColor::DarkGray),
			REGISTERKNOWNCOLORMAP(KnownColor::DarkGreen),
			REGISTERKNOWNCOLORMAP(KnownColor::DarkKhaki),
			REGISTERKNOWNCOLORMAP(KnownColor::DarkMagenta),
			REGISTERKNOWNCOLORMAP(KnownColor::DarkOliveGreen),
			REGISTERKNOWNCOLORMAP(KnownColor::DarkOrange),
			REGISTERKNOWNCOLORMAP(KnownColor::DarkOrchid),
			REGISTERKNOWNCOLORMAP(KnownColor::DarkRed),
			REGISTERKNOWNCOLORMAP(KnownColor::DarkSalmon),
			REGISTERKNOWNCOLORMAP(KnownColor::DarkSeaGreen),
			REGISTERKNOWNCOLORMAP(KnownColor::DarkSlateBlue),
			REGISTERKNOWNCOLORMAP(KnownColor::DarkSlateGray),
			REGISTERKNOWNCOLORMAP(KnownColor::DarkTurquoise),
			REGISTERKNOWNCOLORMAP(KnownColor::DarkViolet),
			REGISTERKNOWNCOLORMAP(KnownColor::DeepPink),
			REGISTERKNOWNCOLORMAP(KnownColor::DeepSkyBlue),
			REGISTERKNOWNCOLORMAP(KnownColor::DimGray),
			REGISTERKNOWNCOLORMAP(KnownColor::DodgerBlue),
			REGISTERKNOWNCOLORMAP(KnownColor::Firebrick),
			REGISTERKNOWNCOLORMAP(KnownColor::FloralWhite),
			REGISTERKNOWNCOLORMAP(KnownColor::ForestGreen),
			REGISTERKNOWNCOLORMAP(KnownColor::Fuchsia),
			REGISTERKNOWNCOLORMAP(KnownColor::Gainsboro),
			REGISTERKNOWNCOLORMAP(KnownColor::GhostWhite),
			REGISTERKNOWNCOLORMAP(KnownColor::Gold),
			REGISTERKNOWNCOLORMAP(KnownColor::Goldenrod),
			REGISTERKNOWNCOLORMAP(KnownColor::Gray),
			REGISTERKNOWNCOLORMAP(KnownColor::Green),
			REGISTERKNOWNCOLORMAP(KnownColor::GreenYellow),
			REGISTERKNOWNCOLORMAP(KnownColor::Honeydew),
			REGISTERKNOWNCOLORMAP(KnownColor::HotPink),
			REGISTERKNOWNCOLORMAP(KnownColor::IndianRed),
			REGISTERKNOWNCOLORMAP(KnownColor::Indigo),
			REGISTERKNOWNCOLORMAP(KnownColor::Ivory),
			REGISTERKNOWNCOLORMAP(KnownColor::Khaki),
			REGISTERKNOWNCOLORMAP(KnownColor::Lavender),
			REGISTERKNOWNCOLORMAP(KnownColor::LavenderBlush),
			REGISTERKNOWNCOLORMAP(KnownColor::LawnGreen),
			REGISTERKNOWNCOLORMAP(KnownColor::LemonChiffon),
			REGISTERKNOWNCOLORMAP(KnownColor::LightBlue),
			REGISTERKNOWNCOLORMAP(KnownColor::LightCoral),
			REGISTERKNOWNCOLORMAP(KnownColor::LightCyan),
			REGISTERKNOWNCOLORMAP(KnownColor::LightGoldenrodYellow),
			REGISTERKNOWNCOLORMAP(KnownColor::LightGray),
			REGISTERKNOWNCOLORMAP(KnownColor::LightGreen),
			REGISTERKNOWNCOLORMAP(KnownColor::LightPink),
			REGISTERKNOWNCOLORMAP(KnownColor::LightSalmon),
			REGISTERKNOWNCOLORMAP(KnownColor::LightSeaGreen),
			REGISTERKNOWNCOLORMAP(KnownColor::LightSkyBlue),
			REGISTERKNOWNCOLORMAP(KnownColor::LightSlateGray),
			REGISTERKNOWNCOLORMAP(KnownColor::LightSteelBlue),
			REGISTERKNOWNCOLORMAP(KnownColor::LightYellow),
			REGISTERKNOWNCOLORMAP(KnownColor::Lime),
			REGISTERKNOWNCOLORMAP(KnownColor::LimeGreen),
			REGISTERKNOWNCOLORMAP(KnownColor::Linen),
			REGISTERKNOWNCOLORMAP(KnownColor::Magenta),
			REGISTERKNOWNCOLORMAP(KnownColor::Maroon),
			REGISTERKNOWNCOLORMAP(KnownColor::MediumAquamarine),
			REGISTERKNOWNCOLORMAP(KnownColor::MediumBlue),
			REGISTERKNOWNCOLORMAP(KnownColor::MediumOrchid),
			REGISTERKNOWNCOLORMAP(KnownColor::MediumPurple),
			REGISTERKNOWNCOLORMAP(KnownColor::MediumSeaGreen),
			REGISTERKNOWNCOLORMAP(KnownColor::MediumSlateBlue),
			REGISTERKNOWNCOLORMAP(KnownColor::MediumSpringGreen),
			REGISTERKNOWNCOLORMAP(KnownColor::MediumTurquoise),
			REGISTERKNOWNCOLORMAP(KnownColor::MediumVioletRed),
			REGISTERKNOWNCOLORMAP(KnownColor::MidnightBlue),
			REGISTERKNOWNCOLORMAP(KnownColor::MintCream),
			REGISTERKNOWNCOLORMAP(KnownColor::MistyRose),
			REGISTERKNOWNCOLORMAP(KnownColor::Moccasin),
			REGISTERKNOWNCOLORMAP(KnownColor::NavajoWhite),
			REGISTERKNOWNCOLORMAP(KnownColor::Navy),
			REGISTERKNOWNCOLORMAP(KnownColor::OldLace),
			REGISTERKNOWNCOLORMAP(KnownColor::Olive),
			REGISTERKNOWNCOLORMAP(KnownColor::OliveDrab),
			REGISTERKNOWNCOLORMAP(KnownColor::Orange),
			REGISTERKNOWNCOLORMAP(KnownColor::OrangeRed),
			REGISTERKNOWNCOLORMAP(KnownColor::Orchid),
			REGISTERKNOWNCOLORMAP(KnownColor::PaleGoldenrod),
			REGISTERKNOWNCOLORMAP(KnownColor::PaleGreen),
			REGISTERKNOWNCOLORMAP(KnownColor::PaleTurquoise),
			REGISTERKNOWNCOLORMAP(KnownColor::PaleVioletRed),
			REGISTERKNOWNCOLORMAP(KnownColor::PapayaWhip),
			REGISTERKNOWNCOLORMAP(KnownColor::PeachPuff),
			REGISTERKNOWNCOLORMAP(KnownColor::Peru),
			REGISTERKNOWNCOLORMAP(KnownColor::Pink),
			REGISTERKNOWNCOLORMAP(KnownColor::Plum),
			REGISTERKNOWNCOLORMAP(KnownColor::PowderBlue),
			REGISTERKNOWNCOLORMAP(KnownColor::Purple),
			REGISTERKNOWNCOLORMAP(KnownColor::Red),
			REGISTERKNOWNCOLORMAP(KnownColor::RosyBrown),
			REGISTERKNOWNCOLORMAP(KnownColor::RoyalBlue),
			REGISTERKNOWNCOLORMAP(KnownColor::SaddleBrown),
			REGISTERKNOWNCOLORMAP(KnownColor::Salmon),
			REGISTERKNOWNCOLORMAP(KnownColor::SandyBrown),
			REGISTERKNOWNCOLORMAP(KnownColor::SeaGreen),
			REGISTERKNOWNCOLORMAP(KnownColor::SeaShell),
			REGISTERKNOWNCOLORMAP(KnownColor::Sienna),
			REGISTERKNOWNCOLORMAP(KnownColor::Silver),
			REGISTERKNOWNCOLORMAP(KnownColor::SkyBlue),
			REGISTERKNOWNCOLORMAP(KnownColor::SlateBlue),
			REGISTERKNOWNCOLORMAP(KnownColor::SlateGray),
			REGISTERKNOWNCOLORMAP(KnownColor::Snow),
			REGISTERKNOWNCOLORMAP(KnownColor::SpringGreen),
			REGISTERKNOWNCOLORMAP(KnownColor::SteelBlue),
			REGISTERKNOWNCOLORMAP(KnownColor::Tan),
			REGISTERKNOWNCOLORMAP(KnownColor::Teal),
			REGISTERKNOWNCOLORMAP(KnownColor::Thistle),
			REGISTERKNOWNCOLORMAP(KnownColor::Tomato),
			REGISTERKNOWNCOLORMAP(KnownColor::Turquoise),
			REGISTERKNOWNCOLORMAP(KnownColor::Violet),
			REGISTERKNOWNCOLORMAP(KnownColor::Wheat),
			REGISTERKNOWNCOLORMAP(KnownColor::White),
			REGISTERKNOWNCOLORMAP(KnownColor::WhiteSmoke),
			REGISTERKNOWNCOLORMAP(KnownColor::Yellow),
			REGISTERKNOWNCOLORMAP(KnownColor::YellowGreen),
			REGISTERKNOWNCOLORMAP(KnownColor::RebeccaPurple),
		}
	};

	static constexpr std::array<std::pair<int, std::string_view>, 142> m_ReverseKnownColorMap
	{
		{
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Transparent),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::AliceBlue),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::AntiqueWhite),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Aqua),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Aquamarine),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Azure),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Beige),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Bisque),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Black),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::BlanchedAlmond),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Blue),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::BlueViolet),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Brown),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::BurlyWood),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::CadetBlue),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Chartreuse),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Chocolate),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Coral),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::CornflowerBlue),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Cornsilk),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Crimson),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Cyan),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::DarkBlue),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::DarkCyan),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::DarkGoldenrod),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::DarkGray),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::DarkGreen),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::DarkKhaki),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::DarkMagenta),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::DarkOliveGreen),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::DarkOrange),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::DarkOrchid),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::DarkRed),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::DarkSalmon),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::DarkSeaGreen),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::DarkSlateBlue),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::DarkSlateGray),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::DarkTurquoise),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::DarkViolet),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::DeepPink),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::DeepSkyBlue),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::DimGray),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::DodgerBlue),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Firebrick),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::FloralWhite),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::ForestGreen),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Fuchsia),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Gainsboro),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::GhostWhite),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Gold),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Goldenrod),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Gray),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Green),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::GreenYellow),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Honeydew),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::HotPink),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::IndianRed),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Indigo),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Ivory),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Khaki),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Lavender),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::LavenderBlush),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::LawnGreen),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::LemonChiffon),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::LightBlue),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::LightCoral),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::LightCyan),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::LightGoldenrodYellow),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::LightGray),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::LightGreen),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::LightPink),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::LightSalmon),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::LightSeaGreen),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::LightSkyBlue),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::LightSlateGray),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::LightSteelBlue),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::LightYellow),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Lime),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::LimeGreen),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Linen),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Magenta),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Maroon),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::MediumAquamarine),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::MediumBlue),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::MediumOrchid),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::MediumPurple),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::MediumSeaGreen),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::MediumSlateBlue),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::MediumSpringGreen),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::MediumTurquoise),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::MediumVioletRed),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::MidnightBlue),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::MintCream),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::MistyRose),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Moccasin),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::NavajoWhite),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Navy),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::OldLace),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Olive),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::OliveDrab),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Orange),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::OrangeRed),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Orchid),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::PaleGoldenrod),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::PaleGreen),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::PaleTurquoise),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::PaleVioletRed),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::PapayaWhip),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::PeachPuff),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Peru),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Pink),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Plum),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::PowderBlue),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Purple),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Red),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::RosyBrown),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::RoyalBlue),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::SaddleBrown),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Salmon),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::SandyBrown),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::SeaGreen),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::SeaShell),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Sienna),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Silver),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::SkyBlue),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::SlateBlue),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::SlateGray),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Snow),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::SpringGreen),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::SteelBlue),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Tan),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Teal),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Thistle),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Tomato),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Turquoise),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Violet),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Wheat),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::White),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::WhiteSmoke),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Yellow),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::YellowGreen),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::RebeccaPurple),
		}
	};

	static constexpr std::array<std::pair<std::string_view, int>, 33> m_SystemColorMap
	{
		{
			REGISTERKNOWNCOLORMAP(KnownColor::ActiveBorder),
			REGISTERKNOWNCOLORMAP(KnownColor::ActiveCaption),
			REGISTERKNOWNCOLORMAP(KnownColor::ActiveCaptionText),
			REGISTERKNOWNCOLORMAP(KnownColor::AppWorkspace),
			REGISTERKNOWNCOLORMAP(KnownColor::Control),
			REGISTERKNOWNCOLORMAP(KnownColor::ControlDark),
			REGISTERKNOWNCOLORMAP(KnownColor::ControlDarkDark),
			REGISTERKNOWNCOLORMAP(KnownColor::ControlLight),
			REGISTERKNOWNCOLORMAP(KnownColor::ControlLightLight),
			REGISTERKNOWNCOLORMAP(KnownColor::ControlText),
			REGISTERKNOWNCOLORMAP(KnownColor::Desktop),
			REGISTERKNOWNCOLORMAP(KnownColor::GrayText),
			REGISTERKNOWNCOLORMAP(KnownColor::Highlight),
			REGISTERKNOWNCOLORMAP(KnownColor::HighlightText),
			REGISTERKNOWNCOLORMAP(KnownColor::HotTrack),
			REGISTERKNOWNCOLORMAP(KnownColor::InactiveBorder),
			REGISTERKNOWNCOLORMAP(KnownColor::InactiveCaption),
			REGISTERKNOWNCOLORMAP(KnownColor::InactiveCaptionText),
			REGISTERKNOWNCOLORMAP(KnownColor::Info),
			REGISTERKNOWNCOLORMAP(KnownColor::InfoText),
			REGISTERKNOWNCOLORMAP(KnownColor::Menu),
			REGISTERKNOWNCOLORMAP(KnownColor::MenuText),
			REGISTERKNOWNCOLORMAP(KnownColor::ScrollBar),
			REGISTERKNOWNCOLORMAP(KnownColor::Window),
			REGISTERKNOWNCOLORMAP(KnownColor::WindowFrame),
			REGISTERKNOWNCOLORMAP(KnownColor::WindowText),
			REGISTERKNOWNCOLORMAP(KnownColor::ButtonFace),
			REGISTERKNOWNCOLORMAP(KnownColor::ButtonHighlight),
			REGISTERKNOWNCOLORMAP(KnownColor::ButtonShadow),
			REGISTERKNOWNCOLORMAP(KnownColor::GradientActiveCaption),
			REGISTERKNOWNCOLORMAP(KnownColor::GradientInactiveCaption),
			REGISTERKNOWNCOLORMAP(KnownColor::MenuBar),
			REGISTERKNOWNCOLORMAP(KnownColor::MenuHighlight),
}
	};

	static constexpr std::array<std::pair<int, std::string_view>, 33> m_ReverseSystemColorMap
	{
		{
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::ActiveBorder),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::ActiveCaption),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::ActiveCaptionText),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::AppWorkspace),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Control),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::ControlDark),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::ControlDarkDark),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::ControlLight),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::ControlLightLight),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::ControlText),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Desktop),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::GrayText),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Highlight),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::HighlightText),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::HotTrack),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::InactiveBorder),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::InactiveCaption),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::InactiveCaptionText),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Info),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::InfoText),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Menu),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::MenuText),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::ScrollBar),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::Window),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::WindowFrame),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::WindowText),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::ButtonFace),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::ButtonHighlight),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::ButtonShadow),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::GradientActiveCaption),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::GradientInactiveCaption),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::MenuBar),
			REGISTERKNOWNCOLORREVERSEMAP(KnownColor::MenuHighlight),
}
	};

	static KnownColor LookupNamedColor(const std::string& colorName)
	{
		static constexpr auto map = InvertedMap<std::string_view, int, m_KnownColorMap.size()>{ {m_KnownColorMap} };
		return (KnownColor)map.at(colorName);
	}

	static KnownColor LookupSystemColor(const std::string& colorName)
	{
		static constexpr auto map = InvertedMap<std::string_view, int, m_SystemColorMap.size()>{ {m_SystemColorMap} };
		return (KnownColor)map.at(colorName);
	}

	static std::string_view LookupReverseNamedColor(int argb)
	{
		static constexpr auto map = Map<int, std::string_view, m_ReverseKnownColorMap.size()>{ {m_ReverseKnownColorMap} };
		return map.at(argb);
	}

	static std::string_view LookupReverseSystemColor(int argb)
	{
		static constexpr auto map = Map<int, std::string_view, m_ReverseSystemColorMap.size()>{ {m_ReverseSystemColorMap} };
		return map.at(argb);
	}

	static constexpr short StateKnownColorValid = 0x0001;
	static constexpr short StateARGBValueValid = 0x0002;
	static constexpr short StateValueMask = (short)(StateARGBValueValid);
	static constexpr short StateNameValid = 0x0008;
	static constexpr long NotDefinedValue = 0;

	static constexpr int ARGBAlphaShift = 24;
	static constexpr int ARGBRedShift = 16;
	static constexpr int ARGBGreenShift = 8;
	static constexpr int ARGBBlueShift = 0;

	std::string m_Name;
	long m_Value;
	short m_KnownColor;
	short m_State;

	constexpr Color(KnownColor knownColor)
		:
		m_Value(0),
		m_State(StateKnownColorValid),
		m_KnownColor((short)knownColor)
	{

	}

	constexpr Color(long value, short state, const std::string& name, KnownColor knownColor)
		:
		m_Value(value),
		m_State(state),
		m_Name(name),
		m_KnownColor((short)knownColor)
	{

	}

	static constexpr void CheckByte(int value, const std::string& name)
	{
		if(value < 0 || value > 255) throw ArgumentException("InvalidEx2BoundArgument_" + name);
	}

	static long MakeArgb(unsigned char alpha, unsigned char red, unsigned char green, unsigned char blue)
	{
		return (long)(((unsigned int)(red << ARGBRedShift) |
					   green << ARGBGreenShift |
					   blue << ARGBBlueShift |
					   alpha << ARGBAlphaShift) & 0XFFFFFFFF);
	}

	constexpr long GetValue() const
	{
		if((m_State & StateValueMask) != 0) return m_Value;
		if(IsKnownColor()) return (long)LookupNamedColor(m_Name);

		return NotDefinedValue;
	}

	constexpr std::string GetNameAndRGBValue()
	{
		return "{{Name={" + m_Name + "}, ARGB=({" + std::to_string(GetA()) + "}, {" + std::to_string(GetR()) + "}, {" + std::to_string(GetG()) + "}, {" + std::to_string(GetB()) + "})}}";
	}

	// Bringing some utilities to handle std::string with hexadecimal without using stringstream, which is not constexpr compliant
	// With this, the Color class, with the exception of the inherited functions, is fully constexpr
	template <typename I>
	std::string IntToHex(I w, size_t hex_len = sizeof(I) << 1)
	{
		static const char* digits = "0123456789ABCDEF";
		std::string rc(hex_len, '0');
		for(size_t i = 0, j = (hex_len - 1) * 4; i < hex_len; ++i, j -= 4)
			rc[i] = digits[(w >> j) & 0x0f];
		return rc;
	}

public:

	constexpr bool operator==(const Color& color)
	{
		if(m_Value == color.m_Value
		   && m_State == color.m_State
		   && m_KnownColor == color.m_KnownColor)
		{
			if(m_Name.compare(color.m_Name) == 0) return true;
		}

		return false;
	}

	constexpr bool operator!=(const Color& color)
	{
		return !(*this == color);
	}

	static constexpr Color FromArgb(int argb)
	{
		return Color((long)argb & 0XFFFFFFFF, StateARGBValueValid, "", KnownColor::Unknown);
	}

	static constexpr Color FromArgb(int alpha, int red, int green, int blue)
	{
		CheckByte(alpha, "alpha");
		CheckByte(red, "red");
		CheckByte(green, "green");
		CheckByte(blue, "blue");
		return Color(MakeArgb((unsigned char)alpha, (unsigned char)red, (unsigned char)green, (unsigned char)blue), StateARGBValueValid, "", KnownColor::Unknown);
	}

	static constexpr Color FromArgb(int alpha, Color baseColor)
	{
		CheckByte(alpha, "alpha");
		return Color(MakeArgb((unsigned char)alpha, baseColor.GetR(), baseColor.GetG(), baseColor.GetB()), StateARGBValueValid, "", KnownColor::Unknown);
	}

	static constexpr Color FromArgb(int red, int green, int blue)
	{
		return FromArgb(255, red, green, blue);
	}

	static constexpr Color FromKnownColor(KnownColor color)
	{
		return Color(color);
	}

	static Color FromName(const std::string& name);

	bool Equals(const Object* const obj) const override;
	int GetHashCode() const noexcept override;

	constexpr unsigned char GetR() const noexcept { return (GetValue() >> ARGBRedShift) & 0xFF; }
	constexpr unsigned char GetG() const noexcept { return (GetValue() >> ARGBGreenShift) & 0xFF; }
	constexpr unsigned char GetB() const noexcept { return (GetValue() >> ARGBBlueShift) & 0xFF; }
	constexpr unsigned char GetA() const noexcept { return (GetValue() >> ARGBAlphaShift) & 0xFF; }
	
	constexpr std::string GetName()
	{
		if((m_State & StateNameValid) != 0) return m_Name;

		if(IsKnownColor())
		{
			auto colorName = std::string(LookupReverseNamedColor(m_KnownColor));
			if(colorName.empty()) assert(false, "Could not find known color for value " + m_KnownColor);
		}

		return IntToHex(m_Value);
	}

	constexpr float GetBrightness()  noexcept
	{
		float r = static_cast<float>(GetR()) / 255.0f;
		float g = static_cast<float>(GetG()) / 255.0f;
		float b = static_cast<float>(GetB()) / 255.0f;

		float max = r, min = r;

		if(g > max) max = g;
		if(b > max) max = b;

		if(g < min) min = g;
		if(b < min) min = b;

		return (max + min) / 2;
	}

	constexpr float GetHue() noexcept
	{
		if(GetR() == GetG() && GetG() == GetB()) return 0;

		float r = static_cast<float>(GetR()) / 255.0f;
		float g = static_cast<float>(GetG()) / 255.0f;
		float b = static_cast<float>(GetB()) / 255.0f;

		float max = r, min = r;
		float hue = 0.0f;

		if(g > max) max = g;
		if(b > max) max = b;

		if(g < min) min = g;
		if(b < min) min = b;

		float delta = max - min;

		if(r == max) hue = (g - b) / delta;
		else if(g == max) hue = 2 + (b - r) / delta;
		else if(b == max) hue = 4 + (r - g) / delta;

		hue *= 60.0f;

		if(hue < 0.0f)hue += 360.0f;

		return hue;
	}

	constexpr float GetSaturation() noexcept
	{
		float r = static_cast<float>(GetR()) / 255.0f;
		float g = static_cast<float>(GetG()) / 255.0f;
		float b = static_cast<float>(GetB()) / 255.0f;

		float max = r, min = r;
		float l = 0, s = 0;

		if(g > max) max = g;
		if(b > max) max = b;

		if(g < min) min = g;
		if(b < min) min = b;

		// If max == min means there's no color and the saturation is 0
		if(max != min)
		{
			l = (max + min) / 2;

			if(l <= 0.5f)
			{
				s = (max - min) / (max + min);
			}
			else
			{
				s = (max - min) / (2 - max - min);
			}
		}

		return s;
	}

	constexpr bool IsKnownColor() const { return ((m_State & StateKnownColorValid) != 0); }
	constexpr bool IsEmpty() const { return m_State == 0; }
	constexpr bool IsNamedColor() const { return ((m_State & StateNameValid) != 0) || IsKnownColor(); }
	constexpr bool IsSystemColor() const { return IsKnownColor() && ((((KnownColor)m_KnownColor) <= KnownColor::WindowText || (((KnownColor)m_KnownColor) > KnownColor::YellowGreen))); }

	constexpr int ToArgb() const noexcept { return (int)m_Value; }
	constexpr KnownColor ToKnownColor() const noexcept { return (KnownColor)m_KnownColor; }
	std::string ToString() const noexcept override;

	static constexpr Color Transparent() { return Color(KnownColor::Transparent); }
	static constexpr Color AliceBlue() { return Color(KnownColor::AliceBlue); }
	static constexpr Color AntiqueWhite() { return Color(KnownColor::AntiqueWhite); }
	static constexpr Color Aqua() { return Color(KnownColor::Aqua); }
	static constexpr Color Aquamarine() { return Color(KnownColor::Aquamarine); }
	static constexpr Color Azure() { return Color(KnownColor::Azure); }
	static constexpr Color Beige() { return Color(KnownColor::Beige); }
	static constexpr Color Bisque() { return Color(KnownColor::Bisque); }
	static constexpr Color Black() { return Color(KnownColor::Black); }
	static constexpr Color BlanchedAlmond() { return Color(KnownColor::BlanchedAlmond); }
	static constexpr Color Blue() { return Color(KnownColor::Blue); }
	static constexpr Color BlueViolet() { return Color(KnownColor::BlueViolet); }
	static constexpr Color Brown() { return Color(KnownColor::Brown); }
	static constexpr Color BurlyWood() { return Color(KnownColor::BurlyWood); }
	static constexpr Color CadetBlue() { return Color(KnownColor::CadetBlue); }
	static constexpr Color Chartreuse() { return Color(KnownColor::Chartreuse); }
	static constexpr Color Chocolate() { return Color(KnownColor::Chocolate); }
	static constexpr Color Coral() { return Color(KnownColor::Coral); }
	static constexpr Color CornflowerBlue() { return Color(KnownColor::CornflowerBlue); }
	static constexpr Color Cornsilk() { return Color(KnownColor::Cornsilk); }
	static constexpr Color Crimson() { return Color(KnownColor::Crimson); }
	static constexpr Color Cyan() { return Color(KnownColor::Cyan); }
	static constexpr Color DarkBlue() { return Color(KnownColor::DarkBlue); }
	static constexpr Color DarkCyan() { return Color(KnownColor::DarkCyan); }
	static constexpr Color DarkGoldenrod() { return Color(KnownColor::DarkGoldenrod); }
	static constexpr Color DarkGray() { return Color(KnownColor::DarkGray); }
	static constexpr Color DarkGreen() { return Color(KnownColor::DarkGreen); }
	static constexpr Color DarkKhaki() { return Color(KnownColor::DarkKhaki); }
	static constexpr Color DarkMagenta() { return Color(KnownColor::DarkMagenta); }
	static constexpr Color DarkOliveGreen() { return Color(KnownColor::DarkOliveGreen); }
	static constexpr Color DarkOrange() { return Color(KnownColor::DarkOrange); }
	static constexpr Color DarkOrchid() { return Color(KnownColor::DarkOrchid); }
	static constexpr Color DarkRed() { return Color(KnownColor::DarkRed); }
	static constexpr Color DarkSalmon() { return Color(KnownColor::DarkSalmon); }
	static constexpr Color DarkSeaGreen() { return Color(KnownColor::DarkSeaGreen); }
	static constexpr Color DarkSlateBlue() { return Color(KnownColor::DarkSlateBlue); }
	static constexpr Color DarkSlateGray() { return Color(KnownColor::DarkSlateGray); }
	static constexpr Color DarkTurquoise() { return Color(KnownColor::DarkTurquoise); }
	static constexpr Color DarkViolet() { return Color(KnownColor::DarkViolet); }
	static constexpr Color DeepPink() { return Color(KnownColor::DeepPink); }
	static constexpr Color DeepSkyBlue() { return Color(KnownColor::DeepSkyBlue); }
	static constexpr Color DimGray() { return Color(KnownColor::DimGray); }
	static constexpr Color DodgerBlue() { return Color(KnownColor::DodgerBlue); }
	static constexpr Color Firebrick() { return Color(KnownColor::Firebrick); }
	static constexpr Color FloralWhite() { return Color(KnownColor::FloralWhite); }
	static constexpr Color ForestGreen() { return Color(KnownColor::ForestGreen); }
	static constexpr Color Fuchsia() { return Color(KnownColor::Fuchsia); }
	static constexpr Color Gainsboro() { return Color(KnownColor::Gainsboro); }
	static constexpr Color GhostWhite() { return Color(KnownColor::GhostWhite); }
	static constexpr Color Gold() { return Color(KnownColor::Gold); }
	static constexpr Color Goldenrod() { return Color(KnownColor::Goldenrod); }
	static constexpr Color Gray() { return Color(KnownColor::Gray); }
	static constexpr Color Green() { return Color(KnownColor::Green); }
	static constexpr Color GreenYellow() { return Color(KnownColor::GreenYellow); }
	static constexpr Color Honeydew() { return Color(KnownColor::Honeydew); }
	static constexpr Color HotPink() { return Color(KnownColor::HotPink); }
	static constexpr Color IndianRed() { return Color(KnownColor::IndianRed); }
	static constexpr Color Indigo() { return Color(KnownColor::Indigo); }
	static constexpr Color Ivory() { return Color(KnownColor::Ivory); }
	static constexpr Color Khaki() { return Color(KnownColor::Khaki); }
	static constexpr Color Lavender() { return Color(KnownColor::Lavender); }
	static constexpr Color LavenderBlush() { return Color(KnownColor::LavenderBlush); }
	static constexpr Color LawnGreen() { return Color(KnownColor::LawnGreen); }
	static constexpr Color LemonChiffon() { return Color(KnownColor::LemonChiffon); }
	static constexpr Color LightBlue() { return Color(KnownColor::LightBlue); }
	static constexpr Color LightCoral() { return Color(KnownColor::LightCoral); }
	static constexpr Color LightCyan() { return Color(KnownColor::LightCyan); }
	static constexpr Color LightGoldenrodYellow() { return Color(KnownColor::LightGoldenrodYellow); }
	static constexpr Color LightGreen() { return Color(KnownColor::LightGreen); }
	static constexpr Color LightGray() { return Color(KnownColor::LightGray); }
	static constexpr Color LightPink() { return Color(KnownColor::LightPink); }
	static constexpr Color LightSalmon() { return Color(KnownColor::LightSalmon); }
	static constexpr Color LightSeaGreen() { return Color(KnownColor::LightSeaGreen); }
	static constexpr Color LightSkyBlue() { return Color(KnownColor::LightSkyBlue); }
	static constexpr Color LightSlateGray() { return Color(KnownColor::LightSlateGray); }
	static constexpr Color LightSteelBlue() { return Color(KnownColor::LightSteelBlue); }
	static constexpr Color LightYellow() { return Color(KnownColor::LightYellow); }
	static constexpr Color Lime() { return Color(KnownColor::Lime); }
	static constexpr Color LimeGreen() { return Color(KnownColor::LimeGreen); }
	static constexpr Color Linen() { return Color(KnownColor::Linen); }
	static constexpr Color Magenta() { return Color(KnownColor::Magenta); }
	static constexpr Color Maroon() { return Color(KnownColor::Maroon); }
	static constexpr Color MediumAquamarine() { return Color(KnownColor::MediumAquamarine); }
	static constexpr Color MediumBlue() { return Color(KnownColor::MediumBlue); }
	static constexpr Color MediumOrchid() { return Color(KnownColor::MediumOrchid); }
	static constexpr Color MediumPurple() { return Color(KnownColor::MediumPurple); }
	static constexpr Color MediumSeaGreen() { return Color(KnownColor::MediumSeaGreen); }
	static constexpr Color MediumSlateBlue() { return Color(KnownColor::MediumSlateBlue); }
	static constexpr Color MediumSpringGreen() { return Color(KnownColor::MediumSpringGreen); }
	static constexpr Color MediumTurquoise() { return Color(KnownColor::MediumTurquoise); }
	static constexpr Color MediumVioletRed() { return Color(KnownColor::MediumVioletRed); }
	static constexpr Color MidnightBlue() { return Color(KnownColor::MidnightBlue); }
	static constexpr Color MintCream() { return Color(KnownColor::MintCream); }
	static constexpr Color MistyRose() { return Color(KnownColor::MistyRose); }
	static constexpr Color Moccasin() { return Color(KnownColor::Moccasin); }
	static constexpr Color NavajoWhite() { return Color(KnownColor::NavajoWhite); }
	static constexpr Color Navy() { return Color(KnownColor::Navy); }
	static constexpr Color OldLace() { return Color(KnownColor::OldLace); }
	static constexpr Color Olive() { return Color(KnownColor::Olive); }
	static constexpr Color OliveDrab() { return Color(KnownColor::OliveDrab); }
	static constexpr Color Orange() { return Color(KnownColor::Orange); }
	static constexpr Color OrangeRed() { return Color(KnownColor::OrangeRed); }
	static constexpr Color Orchid() { return Color(KnownColor::Orchid); }
	static constexpr Color PaleGoldenrod() { return Color(KnownColor::PaleGoldenrod); }
	static constexpr Color PaleGreen() { return Color(KnownColor::PaleGreen); }
	static constexpr Color PaleTurquoise() { return Color(KnownColor::PaleTurquoise); }
	static constexpr Color PaleVioletRed() { return Color(KnownColor::PaleVioletRed); }
	static constexpr Color PapayaWhip() { return Color(KnownColor::PapayaWhip); }
	static constexpr Color PeachPuff() { return Color(KnownColor::PeachPuff); }
	static constexpr Color Peru() { return Color(KnownColor::Peru); }
	static constexpr Color Pink() { return Color(KnownColor::Pink); }
	static constexpr Color Plum() { return Color(KnownColor::Plum); }
	static constexpr Color PowderBlue() { return Color(KnownColor::PowderBlue); }
	static constexpr Color Purple() { return Color(KnownColor::Purple); }
	static constexpr Color Red() { return Color(KnownColor::Red); }
	static constexpr Color RosyBrown() { return Color(KnownColor::RosyBrown); }
	static constexpr Color RoyalBlue() { return Color(KnownColor::RoyalBlue); }
	static constexpr Color SaddleBrown() { return Color(KnownColor::SaddleBrown); }
	static constexpr Color Salmon() { return Color(KnownColor::Salmon); }
	static constexpr Color SandyBrown() { return Color(KnownColor::SandyBrown); }
	static constexpr Color SeaGreen() { return Color(KnownColor::SeaGreen); }
	static constexpr Color SeaShell() { return Color(KnownColor::SeaShell); }
	static constexpr Color Sienna() { return Color(KnownColor::Sienna); }
	static constexpr Color Silver() { return Color(KnownColor::Silver); }
	static constexpr Color SkyBlue() { return Color(KnownColor::SkyBlue); }
	static constexpr Color SlateBlue() { return Color(KnownColor::SlateBlue); }
	static constexpr Color SlateGray() { return Color(KnownColor::SlateGray); }
	static constexpr Color Snow() { return Color(KnownColor::Snow); }
	static constexpr Color SpringGreen() { return Color(KnownColor::SpringGreen); }
	static constexpr Color SteelBlue() { return Color(KnownColor::SteelBlue); }
	static constexpr Color Tan() { return Color(KnownColor::Tan); }
	static constexpr Color Teal() { return Color(KnownColor::Teal); }
	static constexpr Color Thistle() { return Color(KnownColor::Thistle); }
	static constexpr Color Tomato() { return Color(KnownColor::Tomato); }
	static constexpr Color Turquoise() { return Color(KnownColor::Turquoise); }
	static constexpr Color Violet() { return Color(KnownColor::Violet); }
	static constexpr Color Wheat() { return Color(KnownColor::Wheat); }
	static constexpr Color White() { return Color(KnownColor::White); }
	static constexpr Color WhiteSmoke() { return Color(KnownColor::WhiteSmoke); }
	static constexpr Color Yellow() { return Color(KnownColor::Yellow); }
	static constexpr Color YellowGreen() { return Color(KnownColor::YellowGreen); }
};