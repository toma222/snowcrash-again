
#pragma once

#include <snowcrash/core/Core.hpp>

namespace SC
{

	template <typename T, typename U>
	struct Pair
	{
		Pair() = default;
		Pair(const T &f, const U &u)
			: first(f), second(u) {}

		~Pair() = default;

		T first;
		U second;
	};

}
