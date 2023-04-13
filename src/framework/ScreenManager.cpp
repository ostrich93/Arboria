#include "ScreenManager.h"
#include "Screen.h"

namespace Arboria {
    void ScreenManager::clear() {
        while (isEmpty()) {
            pop();
        }
    }

    Screen* ScreenManager::getCurrent() {
        if (isEmpty()) {
            return NULL;
        }

        return stack[stack.getLength() - 1];
    }

    Screen* ScreenManager::getPrevious(Screen& from) {
        int idx = stack.findIndex(&from);
        if (idx <= 0) {
            return NULL;
        }

        return stack[idx-1];
    }
    
    bool ScreenManager::isEmpty()
    {
        return stack.getLength() == 0;
    }

    Screen* ScreenManager::pop() {
        Screen* retVal = getCurrent();
        if (retVal) {
            retVal->leave();
            stack.removeLast();
        }

        if (getCurrent()) {
            getCurrent()->resume();
        }

        return retVal;
    }

    void ScreenManager::push(Screen& scene) {
        if (getCurrent()) {
            getCurrent()->pause();
        }

        stack.append(&scene);
        scene.enter();
    }
    List<ScreenCommand>& ScreenManager::getScreenCommands()
    {
        return screenCommands;
    }
    const List<ScreenCommand>& ScreenManager::getScreenCommands() const
    {
        return screenCommands;
    }
}