#ifndef _EXAMPLE_H
#define _EXAMPLE_H

#include <vector>
#include <string>
#include "context.h"

using namespace std;

class Example {
private:
    const string& title;
    bool shouldExit = false;    
protected:
    Context* ctx;

    void setShouldExit(bool value) {
        this->shouldExit = value;
    }

    virtual void setup() = 0;
    virtual void render() = 0;
    virtual void cleanup() = 0;
public:
    Example(const string& title, Context* ctx): title(title), ctx(ctx) {}

    const string getTitle() {
        return this->title;
    }

    void run() {
        this->setShouldExit(false);

        this->setup();
        while (!this->shouldExit) {
            this->render();
        }
        this->cleanup();
    }
};

#endif