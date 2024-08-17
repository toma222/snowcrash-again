
#pragma once

#include "snowcrash/application/Event.hpp"
#include "snowcrash/types/String.hpp"
#include <snowcrash/core/Core.hpp>

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
	virtual void OnEvent(Event &event);

public:
	template<typename T>
	T *Cast() { return const_cast<T*>(this); }

	template<typename T>
	const T *Cast() const { return const_cast<const T*>(this); }

protected:
	Context *m_context;
};

#define SC_OBJECT(className, parent) public: \
	static const ::SC::TypeInfo *GetStaticTypeInfo() { static const ::SC::TypeInfo info{#className, ::SC::String::StaticHash(#className)}; return &info; } \
	const ::SC::TypeInfo *GetParentTypeInfo() const override { return parent::GetStaticTypeInfo(); } \
	const ::SC::TypeInfo *GetTypeInfo() const override { return GetStaticTypeInfo(); } \
	::SC::String GetName() const override { return GetTypeInfo()->name; } \
	::SC::u32 GetHash() const override { return GetTypeInfo()->hash; }

}

