#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <functional>

#include <fmt/format.h>
#include <cstdarg>

namespace __glob {
#include <glob.h>
}

namespace term {
    namespace color {
        const std::string reset("\033[0m");

        const std::string black("\033[0;30m");
        const std::string red("\033[0;31m");
        const std::string green("\033[0;32m");
        const std::string yellow("\033[0;33m");
        const std::string blue("\033[0;34m");
        const std::string magenta("\033[0;35m");
        const std::string cyan("\033[0;36m");
        const std::string white("\033[0;37m");

        const std::string bg_black("\033[0;40m");
        const std::string bg_red("\033[0;41m");
        const std::string bg_green("\033[0;42m");
        const std::string bg_yellow("\033[0;43m");
        const std::string bg_blue("\033[0;44m");
        const std::string bg_magenta("\033[0;45m");
        const std::string bg_cyan("\033[0;46m");
        const std::string bg_white("\033[0;47m");

        const std::string bold_black("\033[1;30m");
        const std::string bold_red("\033[1;31m");
        const std::string bold_green("\033[1;32m");
        const std::string bold_yellow("\033[1;33m");
        const std::string bold_blue("\033[1;34m");
        const std::string bold_magenta("\033[1;35m");
        const std::string bold_cyan("\033[1;36m");
        const std::string bold_white("\033[1;37m");
    }

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

    void menu(bool repeated, std::string title, std::vector<std::pair<std::string, std::function<void()>>> options) {
        do {
            say();
            say("{}{}{}", color::bold_white, title, color::reset);
            say();
            int i = 0;
            for (auto &o : options) {
                say("{}{}{}) {}", color::bold_cyan, ++i, color::reset, o.first);
            }
            say();
            say("{}q{}) Quit", color::bold_red, color::reset);
            say();

            std::string menu = term::input("{}Selection?{} ", color::bold_white, color::reset);

            if (menu == "q")
                return;

            try {
                int selected = std::stoi(menu);

                options[selected - 1].second();
            }
            catch(std::invalid_argument const &) {
                say("Invalid option.");
            }
            catch(std::bad_function_call const &) {
                say("Invalid option.");
            }
        } while(repeated);
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
