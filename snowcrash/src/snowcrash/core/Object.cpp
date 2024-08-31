
#include "Object.hpp"

#include <snowcrash/core/Context.hpp>

namespace SC
{

	Object::Object(Context *context)
		: m_context(context) {}

	Object::~Object() = default;

	const Resource *Object::GetResourcePointer(String p)
	{
		return m_context->resourceArray.GetResource<Resource>(p);
	}

	void Object::ContextQueueResourceLoad(
		ResourceLoader *resourceLoader, String path)
	{
		m_context->resourceArray.QueueResourceForLoad(resourceLoader, path);
	}
}
