
#pragma once

namespace SC
{

template<typename T, typename Hash>
class HashMap
{
public:
	struct KeyValuePair
	{
		KeyValuePair(T v, const Hash h)
			: value(v), key(h) {}
		KeyValuePair(const KeyValuePair &) = default;
		~KeyValuePair() = default;

		KeyValuePair &operator=(const KeyValuePair&) = delete;

		T value;
		const Hash key;
	};

	struct Node
	{
		Node(Node *p, T value, const Hash key)
			: pair(value, key), prev(p) {}
		~Node()
		{
			// no seg faults
			if(a != nullptr) delete a;
			if(b != nullptr) delete b;
		}

		KeyValuePair pair;

		Node *prev {nullptr}; // prev is only nullptr if node is head
		Node *a {nullptr};
		Node *b {nullptr};
	};

	HashMap() = default;
	~HashMap()
	{
		delete m_head;
	}

	// returns a valid node pointer
	Node *AllocateNode(Node *prev, T value, Hash hash)
	{
		// we need to write an allocator class at SOME point
		return (new Node(prev, value, hash));
	}

	// returns wether or not the operation was successful
	bool Emplace(T value, Hash hash)
	{
		if(m_head == nullptr)
		{
			m_head = AllocateNode(nullptr, value, hash);
			return true;
		}

		Node *current = m_head;
		bool found = false;
		while(found == false)
		{
			if(current->pair.key == hash)
			{
				return false;
			}

			// Check if we found the correct bucket
			if(current->pair.key > hash)
			{
				if(current->b == nullptr)
				{
					current->b = AllocateNode(current, value, hash);
					found = true;
				} else {
					current = current->b;
					continue;
				}
			}else{
				if(current->a == nullptr)
				{
					current->a = AllocateNode(current, value, hash);
					found = true;
				} else {
					current = current->a;
					continue;
				}
			}
		}

		return true;
	}

	T Get(Hash hash) const
	{
		Node *n = FindNode(hash);
		// if(n == nullptr) KN_WARN("could not find hash");
		// assert(n != nullptr);
		return n->pair.value;
	}

	bool CheckCollision(Hash hash) const
	{
		return (FindNode(hash) != nullptr);
	}

	// remove an entry from the table
	void Remove(Hash hash)
	{
		Node *n = FindNode(hash);
		if(n == nullptr) return;
			
		// make a copy of all the nodes UNDER n
		HashMap<T, Hash> copy;
		CopyHashMap(copy, n->a);
		CopyHashMap(copy, n->b);

		if(n->prev->a == n) n->prev->a = nullptr;
		if(n->prev->b == n) n->prev->b = nullptr;
		delete n;

		EmplaceHashMap(copy.m_head);
	}

	bool HasHash(Hash hash)
	{
		return (FindNode(hash) == nullptr);
	}

	// including head node
	// i dont personaly think that recursion is that big of a deal ibh
	void CopyHashMap(HashMap<T, Hash> &dst, Node *head)
	{
		// we have hit the bottom
		if(head == nullptr) return;
		dst.Emplace(head->pair.value, head->pair.key);
		CopyHashMap(dst, head->a);
		CopyHashMap(dst, head->b);
	}

	void EmplaceHashMap(Node *head)
	{
		if(head == nullptr) return;
		Emplace(head->pair.value, head->pair.key);
		EmplaceHashMap(head->a);
		EmplaceHashMap(head->b);
	}

private:
	inline Node *FindNode(Hash hash) const
	{
		Node *current = m_head;
		// bool found = false;
		while(current != nullptr)
		{
			if(current->pair.key == hash)
			{
				return current;
			}

			// Check if we found the correct bucket
			if(current->pair.key > hash)
			{
				// if(current->b == nullptr) return nullptr;
				current = current->b;
			}else{
				// if(current->a == nullptr) return nullptr;
				current = current->a;
			}
		}

		return nullptr;
	}

private:
	Node *m_head {nullptr};
	// Node *m_tail;
};

}

