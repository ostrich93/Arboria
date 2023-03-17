#include <cassert>
#include <type_traits>


namespace Arboria {
	template<typename Type, typename SourceType>
	Type check_static_cast(SourceType* item) {
		static_assert(dynamic_cast<Type>(item));
		return static_cast<Type>(item);
	}
}