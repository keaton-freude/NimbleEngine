#include <memory>

#include "nimble/BasicShader.h"

using namespace Nimble;

// Creation method:
std::shared_ptr<IShaderProgram> CreateBasicShader() {
	return std::make_shared<BasicShader>(); 
}
