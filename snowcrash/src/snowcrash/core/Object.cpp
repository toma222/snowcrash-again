
#include "Object.hpp"

namespace SC
{

Object::Object(Context *context)
	: m_context(context) {}

Object::~Object() = default;

// we dont do anything
void Object::OnEvent(Event&) {}

}

