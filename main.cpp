#include "utils.h"
#include "packman.h" // install

#include <iostream>
#include <string_view>
#include <vector>
#include <map>
#include <functional>
#include <format>


int config(const std::vector<std::string_view>& args);
int init(const std::vector<std::string_view>& args);
int compose(const std::vector<std::string_view>& args);
int include(const std::vector<std::string_view>& args);
int version(const std::vector<std::string_view>& args);
int help(const std::vector<std::string_view>& args);

int main(int argc, char *argv[]) {
   if(argc < 2) {
      crash(std::format("missing command\nTry '{} --help' for more information.", argv[0]));
   }

   std::vector<std::string_view> args;
   args.reserve(argc);
   for(int i=0;i<argc;i++) {
      args.push_back(std::string_view(argv[i]));
   }

   const std::map<std::string_view, std::function<int(const std::vector<std::string_view>&)>> router = {
      {"config", config},
      {"init", init},
      {"compose", compose},
      {"install", install},
      {"include", include},
      {"--version", version},
      {"-v", version},
      {"--help", help},
      {"-h", help}
   };

   auto it = router.find(args[1]);
   if(it == router.end()) {
      crash(std::format("no command called '{}'", args[1]));
   }

   return it->second(args);
}


int version(const std::vector<std::string_view>& args) {
   std::cout << constants::version_info << std::endl;
   return EXIT_SUCCESS;
}

int help(const std::vector<std::string_view>& args) {
   return display_help("general", args[0]);
}

int config(const std::vector<std::string_view>& args) { return EXIT_SUCCESS; }
int init(const std::vector<std::string_view>& args) { return EXIT_SUCCESS; }
int compose(const std::vector<std::string_view>& args) { return EXIT_SUCCESS; }
int include(const std::vector<std::string_view>& args) { return EXIT_SUCCESS; }