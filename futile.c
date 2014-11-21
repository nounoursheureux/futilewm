#include "futile.h"
#include "config.h"

int main()
{
    Display* dpy = XOpenDisplay(NIL);
    XButtonEvent btn;
    Window win;
    XWindowAttributes attr;
    int screenWidth = WidthOfScreen(DefaultScreenOfDisplay(dpy));
    int screenHeight = HeightOfScreen(DefaultScreenOfDisplay(dpy));
    int focus;
    XEvent ev;
    XGrabButton(dpy,1,Mod4Mask,DefaultRootWindow(dpy),False,ButtonPressMask|ButtonReleaseMask|PointerMotionMask,GrabModeAsync,GrabModeAsync,None,None);
    XGrabButton(dpy,3,Mod4Mask,DefaultRootWindow(dpy),False,ButtonPressMask|ButtonReleaseMask|PointerMotionMask,GrabModeAsync,GrabModeAsync,None,None);
    XGrabKey(dpy,XKeysymToKeycode(dpy,XK_Return),Mod4Mask,DefaultRootWindow(dpy),False,GrabModeAsync,GrabModeAsync);
    XGrabKey(dpy,XKeysymToKeycode(dpy,XK_d),Mod4Mask,DefaultRootWindow(dpy),False,GrabModeAsync,GrabModeAsync);
    XGrabKey(dpy,XKeysymToKeycode(dpy,XK_Tab),Mod4Mask,DefaultRootWindow(dpy),False,GrabModeAsync,GrabModeAsync);
    XGrabKey(dpy,XKeysymToKeycode(dpy,XK_x),Mod4Mask,DefaultRootWindow(dpy),False,GrabModeAsync,GrabModeAsync);
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
                XGetInputFocus(dpy,&win,&focus);
                XResizeWindow(dpy,win,screenWidth,screenHeight);
                XMoveWindow(dpy,win,0,0);
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
