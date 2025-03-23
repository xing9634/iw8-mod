#pragma once
#include "common.hpp"
#include "engine/iw8/enums/XAssetType.hpp"

namespace Client::AssetReplacer {
	inline std::filesystem::path GetAssetReplaceDir(const std::string& type) {
		std::filesystem::path cd = ".";
		return cd / "iw8-mod" / "assets" / type;
	}
}
