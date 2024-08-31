
#pragma once

#include <snowcrash/types/String.hpp>
#include <snowcrash/core/Core.hpp>

#include <snowcrash/resource/ResourceArray.hpp>
#include <snowcrash/resource/Resource.hpp>

namespace SC
{

	class Context;

	struct TypeInfo
	{
		String name;
		u32 hash;
	};

	// Any object that has access to the context
	class Object
	{
	public:
		explicit Object(Context *context);
		virtual ~Object();

		static TypeInfo *GetStaticTypeInfo() { return nullptr; }

		virtual const TypeInfo *GetParentTypeInfo() const = 0;
		virtual const TypeInfo *GetTypeInfo() const = 0;
		virtual String GetName() const = 0;
		virtual u32 GetHash() const = 0;

	public:
		template <typename T>
		T *Cast() { return const_cast<T *>(this); }

		template <typename T>
		const T *Cast() const { return const_cast<const T *>(this); }

		template <class R>
		const R *GetResource(String p)
		{
			Resource *r = const_cast<Resource *>(GetResourcePointer(p));
			R *res = dynamic_cast<R *>(r);
			if (res == nullptr)
				SC_ERROR("Resource does not exist [%s]", p.c_str());
			return res;
		}

		template <class RL>
		void QueueResourceForLoad(RL *resourceLoader, String path)
		{
			ContextQueueResourceLoad(static_cast<ResourceLoader *>(resourceLoader), path);
		}

	private:
		const Resource *GetResourcePointer(String p);
		void ContextQueueResourceLoad(ResourceLoader *resourceLoader, String path);

	protected:
		Context *m_context;
	};

#define SC_OBJECT(className, parent)                                                                 \
public:                                                                                              \
	static const ::SC::TypeInfo *GetStaticTypeInfo()                                                 \
	{                                                                                                \
		static const ::SC::TypeInfo info{#className, ::SC::String::StaticHash(#className)};          \
		return &info;                                                                                \
	}                                                                                                \
	const ::SC::TypeInfo *GetParentTypeInfo() const override { return parent::GetStaticTypeInfo(); } \
	const ::SC::TypeInfo *GetTypeInfo() const override { return GetStaticTypeInfo(); }               \
	::SC::String GetName() const override { return GetTypeInfo()->name; }                            \
	::SC::u32 GetHash() const override { return GetTypeInfo()->hash; }

}
