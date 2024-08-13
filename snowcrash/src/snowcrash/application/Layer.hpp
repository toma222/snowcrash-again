
#pragma once

#include <snowcrash/core/Core.hpp>

namespace SC
{

// Contains layers of the application
//      The game layer
//      The editor layer
//      The audio layer
//
// Layer holds logic for recieving events
// Layer has an update function 

class Layer
{
public:
    virtual ~Layer();
    Layer();

	virtual void Init() = 0;
	virtual void Update() = 0;

    bool GetActive() const { return m_active; }
    void SetActive(bool v) { m_active = v; }

protected:
    bool m_active;
};

}

