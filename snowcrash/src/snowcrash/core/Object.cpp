
#include "Object.hpp"

#include <snowcrash/core/Context.hpp>

namespace SC
{

	Object::Object(Context *context)
		: m_context(context) {}

	Object::~Object() = default;

	const Resource *Object::GetResourcePointer(String name)
	{
		return m_context->resourceArray.GetResource<Resource>(name);
	}

	void Object::ContextQueueResourceLoad(
		ResourceLoader *resourceLoader, String path)
	{
		Path resourcePath = m_context->currentProject.projectPath += path;
		m_context->resourceArray.QueueResourceForLoad(resourceLoader, resourcePath.GetString());
	}
}
