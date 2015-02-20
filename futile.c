#include "futile.h"

int main()
{
    Display* dpy = XOpenDisplay(NIL);
    XEvent ev;
    XGrabButton(dpy,1,Mod4Mask,DefaultRootWindow(dpy),False,ButtonPressMask|ButtonReleaseMask|PointerMotionMask,GrabModeAsync,GrabModeAsync,None,None);
    XGrabButton(dpy,3,Mod4Mask,DefaultRootWindow(dpy),False,ButtonPressMask|ButtonReleaseMask|PointerMotionMask,GrabModeAsync,GrabModeAsync,None,None);
    for(;;)
    {
        XNextEvent(dpy,&ev);
        XButtonEvent btn;
        XWindowAttributes attr;
        btn.subwindow = None;
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
    }
    XFlush(dpy);
    sleep(10);
}

