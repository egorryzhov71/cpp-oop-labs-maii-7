#ifndef OBSERVER_H
#define OBSERVER_H

#include <string>
#include <memory>

struct Observer {
    virtual ~Observer() = default;
    virtual void notify(const std::string& msg) = 0;
};

using ObserverPtr = std::shared_ptr<Observer>;

struct ConsoleObserver : public Observer {
    void notify(const std::string& msg) override;
};

struct FileObserver : public Observer {
    std::string filename;
    FileObserver(const std::string& fn = "log.txt");
    void notify(const std::string& msg) override;
};

#endif 
