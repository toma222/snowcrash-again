
#include "CommandLineArgs.hpp"

namespace SC
{
    CommandLineArgs::CommandLineArgs(int argc, char **argv)
    {
        // holds an argument that begins with a - character
        String arg("");

        for (int i = 0; i < argc; i++)
        {
            String in(argv[i]);

            if (in.GetCharAtIndex(0) == '-')
            {
                if (arg == "")
                {
                    arg = String(in);
                }
                else
                {
                    SC_WARN("arg followed by arg. Wrong command line formating");
                }
            }
            else
            {
                if (arg == "")
                    SC_WARN("definiton does not have a arg pair. Maybe you are forgetting a dash");

                m_arguments.Add(Pair<String, String>(arg, in));
                arg = "";
            }
        }
    }

    CommandLineArgs::~CommandLineArgs()
    {
    }
} // namespace SC
