#pragma once

#include "ContentAlignment.h"
#include "AnchorStyles.h"
#include "Padding.h"
#include "Size.h"
#include "DockStyle.h"
#include "IArrangedElement.h"

class CommonProperties
{
public:

	static Drawing::Rectangle GetSpecifiedBounds(IArrangedElement* const element);
	static void UpdateSpecifiedBounds(IArrangedElement* const element, int x, int y, int width, int height, BoundsSpecified specified);

	static constexpr ContentAlignment DefaultAlignment = ContentAlignment::TopLeft;
	static constexpr AnchorStyles DefaultAnchor = AnchorStyles::Top | AnchorStyles::Left;
	static constexpr bool DefaultAutoSize = false;
	static constexpr DockStyle DefaultDock = DockStyle::None;
	static constexpr Padding DefaultMargin = Padding(3);
	static constexpr Size DefaultMinimumSize = Size(0, 0);
	static constexpr Size DefaultMaximumSize = Size(0, 0);
};