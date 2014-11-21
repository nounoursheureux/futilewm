#include "futile.h"
#include "config.h"

int main()
{
    Display* dpy = XOpenDisplay(NIL);
    XButtonEvent btn;
    Window win;
    XWindowAttributes attr;
    unsigned int screenWidth = WidthOfScreen(DefaultScreenOfDisplay(dpy));
    unsigned int screenHeight = HeightOfScreen(DefaultScreenOfDisplay(dpy));
    unsigned int winWidth;
    unsigned int winHeight;
    int winX,winY;
    int focus;
    XEvent ev;
    XGrabButton(dpy,1,Mod4Mask,DefaultRootWindow(dpy),False,ButtonPressMask|ButtonReleaseMask|PointerMotionMask,GrabModeAsync,GrabModeAsync,None,None);
    XGrabButton(dpy,3,Mod4Mask,DefaultRootWindow(dpy),False,ButtonPressMask|ButtonReleaseMask|PointerMotionMask,GrabModeAsync,GrabModeAsync,None,None);
    XGrabKey(dpy,XKeysymToKeycode(dpy,XK_Return),Mod4Mask,DefaultRootWindow(dpy),False,GrabModeAsync,GrabModeAsync);
    XGrabKey(dpy,XKeysymToKeycode(dpy,XK_d),Mod4Mask,DefaultRootWindow(dpy),False,GrabModeAsync,GrabModeAsync);
    XGrabKey(dpy,XKeysymToKeycode(dpy,XK_Tab),Mod4Mask,DefaultRootWindow(dpy),False,GrabModeAsync,GrabModeAsync);
    XGrabKey(dpy,XKeysymToKeycode(dpy,XK_x),Mod4Mask,DefaultRootWindow(dpy),False,GrabModeAsync,GrabModeAsync);
    XGrabKey(dpy,XKeysymToKeycode(dpy,XK_c),Mod4Mask,DefaultRootWindow(dpy),False,GrabModeAsync,GrabModeAsync);
    btn.subwindow = None;
    for(;;)
    {
        XNextEvent(dpy,&ev);
        if(ev.type == KeyPress)
        {
            if(ev.xkey.keycode == XKeysymToKeycode(dpy,XK_Return)) create_process(term);
            if(ev.xkey.keycode == XKeysymToKeycode(dpy,XK_d)) 
            {
                create_process(browser);
            }
            if(ev.xkey.keycode == XKeysymToKeycode(dpy,XK_Tab)) {}
            if(ev.xkey.keycode == XKeysymToKeycode(dpy,XK_x))
            {
                int x_return, y_return;
                unsigned int width_return,height_return,border_width_return,depth_return;
                Window root_return;
                XGetInputFocus(dpy,&win,&focus);
                XGetGeometry(dpy,win,&root_return,&x_return,&y_return,&width_return,&height_return,&border_width_return,&depth_return);
                if(width_return == screenWidth && height_return == screenHeight) 
                {
                    XMoveResizeWindow(dpy,win,winX,winY,winWidth,winHeight);
                }
                else 
                {
                    winX = x_return;
                    winY = y_return;
                    winWidth = width_return;
                    winHeight = height_return;
                    XMoveResizeWindow(dpy,win,0,0,screenWidth,screenHeight);
                }
            }
            if(ev.xkey.keycode == XKeysymToKeycode(dpy,XK_c))
            {
                XGetInputFocus(dpy,&win,&focus);
                XDestroyWindow(dpy,win);
            }
        }
        if(ev.type == ButtonPress && ev.xbutton.subwindow != None)
        {
            XGetWindowAttributes(dpy, ev.xbutton.subwindow, &attr);
            XSetInputFocus(dpy,ev.xbutton.subwindow,RevertToParent,CurrentTime);
            XRaiseWindow(dpy,ev.xbutton.subwindow);
            btn = ev.xbutton;
        }
        else if(ev.type == MotionNotify && btn.subwindow != None)
        {
            int xdiff = ev.xbutton.x_root - btn.x_root;
            int ydiff = ev.xbutton.y_root - btn.y_root;
            if(btn.button == 1)
            {
                XMoveWindow(dpy,btn.subwindow, attr.x + xdiff, attr.y + ydiff);
            }
            else if(btn.button == 3)
            {
                XResizeWindow(dpy,btn.subwindow,attr.width + xdiff,attr.height + ydiff);
            }
        }
        else if(ev.type == ButtonRelease) btn.subwindow = None;
    }
    XFlush(dpy);
    sleep(10);
}

int create_process(char* command)
{
    pid_t pid;
    do 
    {
        pid = fork();
    } while ((pid == -1) && (errno == EAGAIN));
    switch(pid) 
    {
        case -1:
            perror("fork");
            return EXIT_FAILURE;
            break;
        case 0:
            {
                char* arg[] = {command,NULL,NULL};
                if(execvp(command,arg) == -1)
                {
                    perror("execvp");
                    exit(EXIT_FAILURE);
                }

            }
    }
    return 1;

}
