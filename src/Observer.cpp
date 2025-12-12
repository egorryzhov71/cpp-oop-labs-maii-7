#include "/workspaces/C++/LABA7/include/Observer.h"
#include <iostream>
#include <fstream>

void ConsoleObserver::notify(const std::string& msg) { std::cout << msg << std::endl; }
FileObserver::FileObserver(const std::string& fn) : filename(fn) {}
void FileObserver::notify(const std::string& msg) {
    std::ofstream ofs(filename, std::ios::app);
    if (ofs) ofs << msg << '\n';
}
