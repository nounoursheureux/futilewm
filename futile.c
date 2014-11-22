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
    browseWindows(dpy);
    XGrabButton(dpy,1,Mod4Mask,DefaultRootWindow(dpy),True,ButtonPressMask|ButtonReleaseMask|PointerMotionMask,GrabModeAsync,GrabModeAsync,None,None);
    XGrabButton(dpy,3,Mod4Mask,DefaultRootWindow(dpy),False,ButtonPressMask|ButtonReleaseMask|PointerMotionMask,GrabModeAsync,GrabModeAsync,None,None);
    XGrabKey(dpy,XKeysymToKeycode(dpy,XK_Return),Mod4Mask,DefaultRootWindow(dpy),False,GrabModeAsync,GrabModeAsync);
    XGrabKey(dpy,XKeysymToKeycode(dpy,XK_d),Mod4Mask,DefaultRootWindow(dpy),False,GrabModeAsync,GrabModeAsync);
    XGrabKey(dpy,XKeysymToKeycode(dpy,XK_Tab),Mod4Mask,DefaultRootWindow(dpy),False,GrabModeAsync,GrabModeAsync);
    XGrabKey(dpy,XKeysymToKeycode(dpy,XK_x),Mod4Mask,DefaultRootWindow(dpy),False,GrabModeAsync,GrabModeAsync);
    XGrabKey(dpy,XKeysymToKeycode(dpy,XK_c),Mod4Mask,DefaultRootWindow(dpy),False,GrabModeAsync,GrabModeAsync);
    btn.subwindow = None;
    for(;;)
    {
        browseWindows(dpy);
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
            btn = ev.xbutton;
        }
        else if(ev.type == MotionNotify && btn.subwindow != None)
        {
            int xdiff = ev.xbutton.x_root - btn.x_root;
            int ydiff = ev.xbutton.y_root - btn.y_root;
            if(btn.button == 1 && btn.state == Mod4Mask)
            {
                XMoveWindow(dpy,btn.subwindow, attr.x + xdiff, attr.y + ydiff);
            }
            else if(btn.button == 3)
            {
                XResizeWindow(dpy,btn.subwindow,attr.width + xdiff,attr.height + ydiff);
            }
        }
        else if(ev.type == ButtonRelease) btn.subwindow = None;
        else if(ev.type == FocusIn) 
        {
            XRaiseWindow(dpy,ev.xfocus.window);
        }
        else if(ev.type == EnterNotify)
        {
            XSetInputFocus(dpy,ev.xfocus.window,RevertToParent,CurrentTime);
        }
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

void browseWindows(Display* dpy)
{
    Window root_window,parent_window;
    Window* children_list;
    unsigned int children_number;
    unsigned int i;
    XQueryTree(dpy,DefaultRootWindow(dpy),&root_window,&parent_window,&children_list,&children_number);
    fprintf(stderr,"Nombre de fenêtres: %i",children_number);
    if(children_number >= 1)
    {
        for(i = 0; i < children_number; ++i)
        { 
            if(i >= 1)
            {
                XSelectInput(dpy,children_list[i],FocusChangeMask|EnterWindowMask);
                /*XSetWindowBorderWidth(dpy,children_list[i],3);*/
            }
        }
    }
}
