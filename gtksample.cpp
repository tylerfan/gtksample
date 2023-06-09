#include <wayland-egl.h>

#include <wayland-client.h>
#include <wayland-client-protocol.h>
#include <wayland-egl-core.h>


#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include "gtk-3.0/gdk/gdkwayland.h"

#include <EGL/egl.h>
#include <EGL/eglplatform.h>
#include <GL/gl.h>




static EGLDisplay egl_display;
static EGLSurface egl_surface;
static EGLContext egl_context;

static void realize_cb (GtkWidget *widget)
{
    EGLConfig egl_config;
    EGLint n_config;
    EGLint attributes[] = { EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
                            EGL_NONE };

    egl_display = eglGetDisplay (gdk_wayland_display_get_wl_display (gtk_widget_get_display (widget)));
    eglInitialize (egl_display, NULL, NULL);
    eglChooseConfig (egl_display, attributes, &egl_config, 1, &n_config);
    eglBindAPI (EGL_OPENGL_API);
    struct wl_egl_window* egl_window = wl_egl_window_create(gdk_wayland_window_get_wl_surface(gtk_widget_get_window (widget)),
                              480, 360);
    if (egl_window == EGL_NO_SURFACE) {
        fprintf(stderr, "Can't create egl window\n");
        exit(1);
    } else {
        fprintf(stderr, "Created egl window\n");
    }
    egl_surface = eglCreateWindowSurface (egl_display, egl_config, egl_window, NULL);
    egl_context = eglCreateContext (egl_display, egl_config, NULL, NULL);
}
static gboolean draw_cb (GtkWidget *widget)
{
    eglMakeCurrent (egl_display, egl_surface, egl_surface, egl_context);

    glViewport (0, 0, gtk_widget_get_allocated_width (widget), gtk_widget_get_allocated_height (widget));

    glClearColor (0, 0, 0, 1);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho (0, 100, 0, 100, 0, 1);

    glBegin (GL_TRIANGLES);
    glColor3f (1, 0, 0);
    glVertex2f (50, 10);
    glColor3f (0, 1, 0);
    glVertex2f (90, 90);
    glColor3f (0, 0, 1);
    glVertex2f (10, 90);
    glEnd ();

    eglSwapBuffers (egl_display, egl_surface);

    return TRUE;
}

int main (int argc, char **argv)
{
    GtkWidget *w;

    gtk_init (&argc, &argv);

    w = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_double_buffered (GTK_WIDGET (w), FALSE);
    g_signal_connect (G_OBJECT (w), "realize", G_CALLBACK (realize_cb), NULL);
    g_signal_connect (G_OBJECT (w), "draw", G_CALLBACK (draw_cb), NULL);

    gtk_widget_show (w);

    gtk_main ();

    return 0;
}