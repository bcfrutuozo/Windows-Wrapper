#pragma once

#include "IComponent.h"
#include "Rectangle.h"
#include "BoundsSpecified.h"
#include "Size.h"

#include <vector>

class IArrangedElement : public IComponent
{
public:

	virtual Drawing::Rectangle GetRectangle() const noexcept = 0;
	virtual void SetRectangle(Drawing::Rectangle rect) = 0;
	virtual void SetBounds(Drawing::Rectangle bounds, BoundsSpecified specified) = 0;
	virtual Size GetPreferredSize(Size proposedSize) noexcept = 0;
	virtual Drawing::Rectangle GetDisplayRectangle() const noexcept = 0;
	virtual bool GetParticipatesInLayout() noexcept = 0;
	virtual void PerformLayout(IArrangedElement* affectedElement, const std::string& property) = 0;
	virtual IArrangedElement* GetContainer() const noexcept = 0;
	virtual std::vector<IArrangedElement*> GetChildren() const noexcept = 0;
};