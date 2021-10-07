#include "File.h"

bool File::Exists(const std::string& path) noexcept
{
	if (path.empty()) return false;

	return std::filesystem::exists(path);
}
