
#pragma once

#include <snowcrash/core/Core.hpp>

#include <snowcrash/types/ArrayList.hpp>
#include <snowcrash/types/Pair.hpp>
#include <snowcrash/types/String.hpp>
#include <snowcrash/types/UnorderedMap.hpp>

namespace SC
{
    class CommandLineArgs
    {
    public:
        CommandLineArgs(int argc, char **argv);
        ~CommandLineArgs();

        String GetArgument(String arg)
        {
            for (int i = 0; i < m_arguments.GetIndex(); i++)
            {
                if (m_arguments[i].first == arg)
                    return m_arguments[i].second;
            }

            SC_WARN("Can not find command line argument %s", arg.c_str());
            return "";
        }

    private:
        // arg def
        ArrayList<Pair<String, String>> m_arguments;
    };

} // namespace SC
