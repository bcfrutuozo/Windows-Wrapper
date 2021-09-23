#pragma once

// This include is the most important one because Object and Type are circular dependency.
// So everytime a new class is created, should include this file instead of the Object.h
#include "Object.h"
#include "Type.h"