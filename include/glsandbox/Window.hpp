#ifndef WINDOW_HPP_
#define WINDOW_HPP_

class Window
{
public:
    virtual const void *GetNativeWindow() = 0;
};

#endif // WINDOW_HPP_
