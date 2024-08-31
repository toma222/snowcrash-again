
#include "Path.hpp"

#include <sys/stat.h>

namespace SC
{
    Path::Path(String str)
        : m_path(str)
    {
        struct stat buffer;
        int err = stat(str.c_str(), &buffer);
        if (err == -1)
        {
            return;
        }

        m_valid = true;

        if (buffer.st_mode == S_IFDIR)
        {
            m_isPath = false;
        }
        else
        {
            m_fileSize = buffer.st_size;
        }

        m_relative = (str.c_str()[0] == '/');
    }

    Path::~Path() = default;
}
