
#pragma once

#include <snowcrash/core/Core.hpp>
#include <snowcrash/types/Time.hpp>

namespace SC
{

// Describes the game world
// does not hold any systems
// just holds the data
class World
{

};

// Holds functions that update the data in the world
class System
{
public:
    virtual ~System();

    virtual void Update(World &world, TimeStamp deltatime);
};

}
