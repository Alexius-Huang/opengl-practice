#include "examples/01-hello-world.h"
#include "examples/02-element-buffer-object.h"
using namespace std;

int main() {
    GUI gui;

    ReturnType result;
    result.selectedIndex = 0;

    while (true) {
        if (result.selectedIndex == 0) {
            result = _01_helloWorld(&gui);
        } else if (result.selectedIndex == 1) {
            result = _02_elementBufferObject(&gui);
        } else {
            cout << "Index  " << result.selectedIndex << " not existed" << endl;
            return -1;
        }
    }

    return 0;
}
