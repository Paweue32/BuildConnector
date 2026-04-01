#include "parsing.h"

int main(int argc, char **argv) {
    std::vector<std::string> args;
    args.reserve(argc-1);
    for(int i=1;i<argc;i++) {
        args.push_back(std::string(argv[i]));
    }

     if(args.size() < 2 || args[0] != "alias") {
        parse_default(args);
     }
     else if(args[1] == "set") {
        parse_varset(args);
     }
     else if(args[1] == "unset") {
        parse_vardelete(args);
     }
     else if(args[1] == "lookup") {
        parse_varread(args);
     }
     else if(args[1] == "table") {
        parse_varview(args);
     }
     else {
        parse_default(args);
     }
    
    return 0;
}
