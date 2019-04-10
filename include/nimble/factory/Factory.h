#pragma once

// Andrei Alexandrescu generic Factory
// AbstractProduct: The Base class this factory returns instances of
// IdentifierType: Type of the key in the mapping of "Type -> Instance"
// ProductCreator: Some Functor which returns a concrete instance of an AbstractProduct
template <class AbstractProduct, typename IdentifierType, typename ProductCreator>
class Factory {
public:
	bool Register(const IdentifierType &id, ProductCreator creator) {
	}

private:
};
