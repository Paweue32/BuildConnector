#include "utils.h" // init
#include "packman.h" // install

#include <iostream>
#include <string_view>
#include <vector>
#include <map>
#include <functional>


int config(const std::vector<std::string_view>& args); // todo
int init(const std::vector<std::string_view>& args); // done
int compose(const std::vector<std::string_view>& args); // todo
int install(const std::vector<std::string_view>& args); // todo
int version(const std::vector<std::string_view>& args); // done
int help(const std::vector<std::string_view>& args); // done

int main(int argc, char *argv[]) {
   if(argc < 2) {
      crash(std::string("missing command\nTry '") + argv[0] + " --help' for more information.");
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
      {"--version", version},
      {"-v", version},
      {"--help", help},
      {"-h", help}
   };

   auto it = router.find(args[1]);
   if(it == router.end()) {
      crash(std::string("no command called '") + std::string(args[1]) + "'");
   }

   return it->second(args);
}


int version(const std::vector<std::string_view>& args) {
   std::cout << constants::version_info << std::endl;
   return EXIT_SUCCESS;
}

int help(const std::vector<std::string_view>& args) {
   if(args.size() > 2 && args[2] == "--help") {
      return display_help("help", args[0]);
   }

   return display_help("general", args[0]);
}