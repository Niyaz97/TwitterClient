#include <iostream>
#include <regex>
#include <boost/process.hpp>

namespace bp =::boost::process;

auto WordCounter(const std::string& text)-> bp::child {

    bp::context ctx;
    std::vector<std::string> args={text};
    ctx.stdout_behavior = bp::capture_stream();
    return bp::launch(text, args, ctx);
}

int main(int argc, char** argv) {

    if (argc != 2)
        return 0;
    bp::child child=WordCounter(argv[1]);

    bp::pistream& is = child.get_stdout();

    std::string text;
    std::regex e("\\w+\\W*");
    std::smatch sm;


    size_t counter = 0;
    while (std::getline(is, text)) {
        while (std::regex_match(text, sm, e)) {
            counter += sm.size();
            text = sm.suffix();
        }
    }

    std::cout << "Number of words: " << counter << std::endl;
    bp::status stat=child.wait();
    return stat.exited() ? stat.exit_status() : EXIT_FAILURE;
}
