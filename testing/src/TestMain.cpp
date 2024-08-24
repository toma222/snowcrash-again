
#include <snowcrash/application/Application.hpp>
#include <snowcrash/types/ArrayList.hpp>
#include <snowcrash/types/Stack.hpp>
#include <snowcrash/types/String.hpp>
#include <snowcrash/types/HashMap.hpp>
#include <snowcrash/core/Logging.hpp>
#include <snowcrash/types/UnorderedMap.hpp>

int main(void)
{
    using namespace snowcrash;

    // Testing strings
    String s("Hello World");
    SC_TRACE("String Print: %s", s.c_str());

    return 0;
}
