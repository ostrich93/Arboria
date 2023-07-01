#include "ScreenManager.h"
#include "UIContext.h"

namespace Arboria {
    void ScreenManager::clear() {
        while (isEmpty()) {
            pop();
        }
    }

    UIContext* ScreenManager::getCurrent() {
        if (isEmpty()) {
            return NULL;
        }

        return stack[stack.getLength() - 1];
    }

    UIContext* ScreenManager::getPrevious(UIContext& from) {
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

    UIContext* ScreenManager::pop() {
        UIContext* retVal = getCurrent();
        if (retVal) {
            retVal->leave();
            stack.removeLast();
        }

        if (getCurrent()) {
            getCurrent()->resume();
        }

        return retVal;
    }

    void ScreenManager::push(UIContext& scene) {
        if (getCurrent()) {
            getCurrent()->pause();
        }

        stack.append(&scene);
        scene.enter();
    }
    void ScreenManager::push(UIContext* screen)
    {
        if (!screen)
            return; //throw error

        if (getCurrent())
            getCurrent()->pause();
        
        stack.append(screen);
        screen->enter();
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