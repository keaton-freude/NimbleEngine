#pragma once

#include <map>
#include <memory>
#include <fmt/format.h>
#include <functional>
#include <utility>

#include "nimble/factory/DefaultFactoryErrorPolicy.h"

namespace Nimble {
// Andrei Alexandrescu generic Factory
// AbstractProduct: The Base class this factory returns instances of
// IdentifierType: Type of the key in the mapping of "Type -> Instance"
// ProductCreator: Some Functor which returns a concrete instance of an AbstractProduct
template <class AbstractProduct, 
		 typename IdentifierType, 
		 typename ProductCreator = std::function<std::shared_ptr<AbstractProduct>()>, 
		 template<typename, class> 
		 class FactoryErrorPolicy = DefaultFactoryError>
class Factory : public FactoryErrorPolicy<IdentifierType, AbstractProduct> {
	typedef std::map<IdentifierType, ProductCreator> StorageTy;
public:
	bool Register(const IdentifierType &id, ProductCreator creator) {
		return _associations.insert(std::pair<int, ProductCreator>(id, creator)).second;
	}

	bool Unregister(const IdentifierType &id) {
		return _associations.erase(id) == 1;
	}

	std::shared_ptr<AbstractProduct> CreateObject(const IdentifierType &id) {
		// Returns an instance, based on the creator we were given
		auto i = _associations.find(id);
		if (i != _associations.end()) {
			// Found it
			return std::shared_ptr<AbstractProduct>((i->second)());
		}

		return FactoryErrorPolicy<IdentifierType, AbstractProduct>::OnUnknownType(id);
	}

private:
	StorageTy _associations;
};
}
