// This default policy simply throws an exception with the missing ID

#include <exception>
#include <fmt/format.h>
#include <memory>

template <class IdentifierType, class ProductType>
class DefaultFactoryError {
public:
	class Exception : public std::exception {
	public:
		Exception(const IdentifierType &unknownId) : _unknownId(unknownId) {
		}

		virtual const char *what() {
			return fmt::format("Unknown object type passed to factory with id: {}", _unknownId).c_str();
		}

		const IdentifierType GetId() {
			return _unknownId;
		}

	private:
		IdentifierType _unknownId;
	};

protected:
	std::shared_ptr<ProductType> OnUnknownType(const IdentifierType &id) {
		// In this case, no intervention is performed.
		throw Exception(id);
	}
};
