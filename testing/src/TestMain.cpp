
#include <snowcrash/application/Application.hpp>
#include <snowcrash/types/ArrayList.hpp>
#include <snowcrash/types/Stack.hpp>
#include <snowcrash/types/String.hpp>
#include <snowcrash/types/HashMap.hpp>
#include <snowcrash/core/Logging.hpp>
#include <snowcrash/types/UnorderedMap.hpp>
#include <snowcrash/events/EventManager.hpp>
#include <snowcrash/events/ApplicationEvents.hpp>

#include <snowcrash/resource/ResourceArray.hpp>
#include <snowcrash/resource/ResourceImage.hpp>

using namespace snowcrash;
class TestLayer : SC::Layer
{
    SC_OBJECT(TestLayer, Layer)
public:
    TestLayer(Context *c)
        : Layer(c)
    {
    }
    ~TestLayer() = default;

    void Init() override {}
    void Update() override {}
};

int main(void)
{

    // Testing strings
    String s("Hello World");

    Context *cont = new Context({"Test", Path("/")});

    TestLayer *l = new TestLayer(cont);
    cont->AddLayer(l);

    cont->SubscribeEvent<EventApplicationExit>(
        [](const EventApplicationExit &)
        {
            SC_TRACE("asdas");
        });

    cont->QueueEvent(new EventApplicationExit());
    cont->DispatchEvents();

    ResourceArray resourceArray;
    resourceArray.QueueResourceForLoad(
        new ResourceImageLoader(), "snowcrash/templates/Sandbox3D/systems/Graphics/textures/viking_room.png");
    resourceArray.LoadResources();

    const ResourceImage *image = resourceArray.GetResource<ResourceImage>("viking_room.png");
    SC_TRACE("%i", image->GetHeight());

    delete cont;

    return 0;
}
