#pragma once

#include "nimble/core/Assert.h"
#include "nimble/utility/StringUtility.h"

namespace Nimble {

enum class MaterialType { PHONG, UNLIT };

const static std::string PhongStr = "phong";
const static std::string UnlitStr = "unlit";

struct MaterialTypeUtil {
	static MaterialType MaterialTypeFromString(const std::string &value) {
		if(iequals(value, PhongStr)) {
			return MaterialType::PHONG;
		} else if(iequals(value, UnlitStr)) {
			return MaterialType::UNLIT;
		} else {
			ASSERT_TRUE(false, fmt::format("Material Type String ({}) does not map to a known enum value", value).c_str());
		}
	}
};

} // namespace Nimble