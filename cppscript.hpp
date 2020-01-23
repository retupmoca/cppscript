#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <fmt/format.h>
#include <cstdarg>

namespace __glob {
#include <glob.h>
}

namespace term {
    template<typename... Targs>
    void print(Targs... Fargs) {
        std::cout << fmt::format(Fargs...);
    }

    template<typename... Targs>
    void say(std::string format, Targs... Fargs) {
        print(format + "\n", Fargs...);
    }

    void say(std::string stuff = "") {
        print(stuff + "\n");
    }

    std::string getline() {
        std::string input;
        std::getline(std::cin, input);
        return input;
    }

    template<typename... Targs>
    std::string input(Targs... Fargs) {
        print(Fargs...);
        return getline();
    }
}

namespace ps {
    template<typename... Targs>
    int shell(Targs... Fargs) {
        int ret = system(fmt::format(Fargs...).c_str());
        return ret;
    }
}

namespace file {
    template<typename... Targs>
    std::vector<std::string> glob(Targs... Fargs) {
        std::vector<std::string> results;

        __glob::glob_t globbuf;
        __glob::glob(fmt::format(Fargs...).c_str(), 0, nullptr, &globbuf);

        for(size_t i=0; i < globbuf.gl_pathc; i++) {
            results.push_back(globbuf.gl_pathv[i]);
        }

        __glob::globfree(&globbuf);

        return results;
    }
}
