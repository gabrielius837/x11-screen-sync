#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <stdio.h>

#ifndef RR_PREFERRED
#define RR_PREFERRED (1 << 11)
#endif

int main() {
    Display *display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "Unable to open display\n");
        return 1;
    }

    Window root = RootWindow(display, DefaultScreen(display));

    XRRScreenResources *res = XRRGetScreenResources(display, root);
    if (!res) {
        fprintf(stderr, "Unable to get screen resources\n");
        XCloseDisplay(display);
        return 1;
    }

    printf("Number of screens: %d\n", res->noutput);


   for (int i = 0; i < res->noutput; ++i) {
        XRROutputInfo *output_info = XRRGetOutputInfo(display, res, res->outputs[i]);
        if (!output_info || !output_info->crtc)
            continue;

        XRRCrtcInfo *crtc_info = XRRGetCrtcInfo(display, res, output_info->crtc);
        if (!crtc_info)
            continue;

        printf("Screen %d:\n", i + 1);
        printf("  Name: %s\n", output_info->name);

        // Use XRRGetScreenResourcesCurrent to get the current configuration
        XRRScreenResources *current_res = XRRGetScreenResourcesCurrent(display, root);
        for (int j = 0; j < current_res->nmode; ++j) {
            if (current_res->modes[j].id == crtc_info->mode) {
                printf("  Resolution: %dx%d\n", current_res->modes[j].width, current_res->modes[j].height);
                printf("  Refresh Rate: %d Hz\n", current_res->modes[j].vTotal);
                printf("  Refresh Rate: %ld Hz\n", current_res->modes[j].dotClock);
                printf("  Refresh Rate: %ld Hz\n", current_res->modes[j].dotClock / (current_res->modes[j].hTotal * current_res->modes[j].vTotal));
                break;
            }
        }

        XRRFreeCrtcInfo(crtc_info);
        XRRFreeOutputInfo(output_info);
    }

    XRRFreeScreenResources(res);
    XCloseDisplay(display);

    return 0;
}

