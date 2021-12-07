#pragma once

enum class ChartDashStyle
{
	// The line style is not set
	NotSet = 0,

	// A dashed line
	Dash = 1,

	// A line with a repeating dash - dot pattern
	DashDot = 2,

	// A line a repeating dash - dot - dot pattern
	DashDotDot = 3,

	// A line with a repeating dot pattern.
	Dot = 4,

	// A solid line.
	Solid = 5
};