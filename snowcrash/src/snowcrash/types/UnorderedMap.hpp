
#pragma once

#include <snowcrash/core/Core.hpp>
#include <snowcrash/types/Pair.hpp>
#include <snowcrash/types/ArrayList.hpp>

namespace SC
{

	template <typename T, typename Hash>
	class UnorderedMap
	{
	public:
		UnorderedMap()
			: m_array() {}
		~UnorderedMap() = default;
		UnorderedMap(u32 size)
			: m_array(size) {}

		T &Get(Hash hash) const
		{
			for (int i = 0; i < m_array.GetIndex(); i++)
			{
				if (m_array.Get(i).second == hash)
				{
					return m_array[i].first;
				}
			}

			return m_array[0].first;
		}

		void Enter(T value, Hash hash)
		{
			m_array.Add(Pair<T, Hash>{value, hash});
		}

		bool HasDuplicate(Hash hash) const
		{
			for (int i = 0; i < m_array.GetIndex(); i++)
				if (m_array.Get(i).second == hash)
					return true;

			return false;
		}

		int GetSize() const { return m_array.GetIndex(); }

		T &operator[](T hash) { return Get(hash); }

		ArrayList<Pair<T, Hash>> &GetArray() { return m_array; }

	private:
		ArrayList<Pair<T, Hash>> m_array;
	};

}
