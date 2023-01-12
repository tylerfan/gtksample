#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <gdk/gdkx.h>


typedef void* GtkWindowPtr;
//typedef void* GtkWidget*;


void SetGtkWindowTransientFor(GtkWindowPtr parent, GtkWindowPtr child)
{
    gtk_window_set_transient_for(GTK_WINDOW(child), GTK_WINDOW(parent));
}

void SetGtkWindowShouldGetFocus(GtkWindowPtr window, bool shouldGetFocus)
{
    gtk_window_set_focus_on_map(GTK_WINDOW(window), shouldGetFocus ? 1 : 0);
}

void SetGtkWidgetChild(GtkWidget* container, GtkWidget* child, bool addChildToScrolledWindow /* = false */)
{
    if (addChildToScrolledWindow)
    {
        // add scrollbar workaround to get proper resize behavior
        GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL , 0); //uses box with vertical orientation since vbox is deprecated since 3.2
        gtk_container_add(GTK_CONTAINER(container), GTK_WIDGET(vbox));

        GtkWidget* scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
        GtkPolicyType scrollbarPolicy = static_cast<GtkPolicyType>(GTK_POLICY_EXTERNAL);

        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindow), scrollbarPolicy, scrollbarPolicy);
        gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(scrolledWindow), 200);
        gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(scrolledWindow), 240);

        gtk_container_add(GTK_CONTAINER(scrolledWindow), GTK_WIDGET(child));
        gtk_box_pack_end(GTK_BOX(vbox), GTK_WIDGET(scrolledWindow), TRUE, TRUE, 0);
    }
    else
    {
        gtk_container_add(GTK_CONTAINER(container), GTK_WIDGET(child));
    }
}

void SetGtkWindowSize(GtkWindowPtr window, int width, int height)
{
    gtk_window_resize(GTK_WINDOW(window), width, height);
}

void SetGtkWindowPosition(GtkWindowPtr window, int x, int y)
{
    gtk_window_move(GTK_WINDOW(window), x, y);
}

void SetGtkWindowGravity(GtkWindowPtr window)
{
    gtk_window_set_gravity(GTK_WINDOW(window), GDK_GRAVITY_STATIC);
}

static void FixupGtkWidgetVisual(GtkWidget* widgetPtr)
{

    GtkWidget* gtk_widget = GTK_WIDGET(widgetPtr);
    GdkScreen* screen = gtk_widget_get_screen(gtk_widget);
    GdkVisual* visual = gdk_screen_get_rgba_visual(screen);

    if (!visual)
    {
        visual = gdk_screen_get_system_visual(screen);
    }

    gtk_widget_set_visual(gtk_widget, visual);
}

GtkWindow* CreateGtkWindow(bool toplevel, bool ignore=true, bool isWSL=false)
{
    // GTK_WINDOW_POPUP is only used for tooltips
    GtkWindow *window = GTK_WINDOW(gtk_window_new(toplevel ? GTK_WINDOW_TOPLEVEL : GTK_WINDOW_POPUP));

    // When running via nvidia prime on demand, gtk may choose the wrong visual for new windows
    // We can detect/correct this, but it has to happen before the window is realized
    FixupGtkWidgetVisual((GtkWidget*)window);

    // Need to set geometry hints if we want to be able to shrink the window.
    // Otherwise Gtk will refuse to even request resize
    // to a size smaller than the aggregate requested sizes of existing widgets.
    GdkGeometry geometry;
    geometry.min_width = geometry.min_height = geometry.base_width = geometry.base_height = 1;
    GdkWindowHints mask = (GdkWindowHints)(GDK_HINT_MIN_SIZE | GDK_HINT_BASE_SIZE);
    gtk_window_set_geometry_hints(window, NULL, &geometry, mask);
    gtk_widget_add_events(GTK_WIDGET(window), GDK_FOCUS_CHANGE_MASK);
    auto gAccelGroup = gtk_accel_group_new();

    if (toplevel)
        // Need to add global accel group for keybindings
        gtk_window_add_accel_group(window, gAccelGroup);
    return window;
}

GtkWidget* CreateGtkFixedContainer()
{
    return gtk_fixed_new();
}

GtkWidget* CreateGtkDrawingArea()
{
    GtkWidget* box = gtk_drawing_area_new();
    gtk_widget_set_double_buffered(box, false);
    // Add all the events (trim away if this is too much)
    gtk_widget_add_events(box,
        GDK_STRUCTURE_MASK |
        GDK_FOCUS_CHANGE_MASK |
        GDK_EXPOSURE_MASK |
        GDK_POINTER_MOTION_MASK |
        GDK_POINTER_MOTION_HINT_MASK |
        GDK_BUTTON_MOTION_MASK |
        GDK_BUTTON1_MOTION_MASK |
        GDK_BUTTON2_MOTION_MASK |
        GDK_BUTTON3_MOTION_MASK |
        GDK_BUTTON_PRESS_MASK |
        GDK_BUTTON_RELEASE_MASK |
        GDK_SCROLL_MASK |
        GDK_SMOOTH_SCROLL_MASK |
        GDK_KEY_PRESS_MASK |
        GDK_KEY_RELEASE_MASK |
        GDK_ENTER_NOTIFY_MASK |
        GDK_LEAVE_NOTIFY_MASK |
        GDK_FOCUS_CHANGE_MASK |
        GDK_STRUCTURE_MASK |
        GDK_PROPERTY_CHANGE_MASK |
        GDK_PROXIMITY_IN_MASK |
        GDK_PROXIMITY_OUT_MASK);
    return box;
}

void SetGtkWidgetSize(GtkWidget* widget, int width, int height)
{
    gtk_widget_set_size_request(GTK_WIDGET(widget), width, height);
}

void SetGtkWidgetCanFocus(GtkWidget* widget, bool canFocus)
{
    gtk_widget_set_can_focus(GTK_WIDGET(widget), canFocus ? 1 : 0);
}

void RegisterGtkWidgetForMotionEvents(GtkWidget* widget, bool highFrequency)
{
    // GDK_POINTER_MOTION_HINT_MASK tries to send pointer motion events in an efficient manner.
    // If we don't get enough events this way, we should consider changing to GDK_POINTER_MOTION_MASK everywhere.
    GdkEventMask mask = (GdkEventMask)(GDK_BUTTON_MOTION_MASK |
        (highFrequency ? GDK_POINTER_MOTION_MASK : GDK_POINTER_MOTION_HINT_MASK));
    gtk_widget_add_events(GTK_WIDGET(widget), mask);
}

void RegisterGtkWidgetForKeyEvents(GtkWidget* widget)
{
    gtk_widget_add_events(GTK_WIDGET(widget), GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK);
}
static const char* kUnityDragTarget = "unity/internal";
static const char* kExternalDragTarget = "text/uri-list";


void RegisterGtkWidgetForDragEvents(GtkWidget* widget)
{
    GtkTargetEntry entry[] =
    {
        // Intra-app drags
        { const_cast<gchar*>(kUnityDragTarget), GTK_TARGET_SAME_APP, 0 },

        // Drags from outside Unity
        { const_cast<gchar*>(kExternalDragTarget), 0, 0 },
    };
    gtk_drag_dest_set(GTK_WIDGET(widget),
        static_cast<GtkDestDefaults>(GTK_DEST_DEFAULT_MOTION | GTK_DEST_DEFAULT_DROP),
        entry,
        sizeof(entry)/sizeof(GtkTargetEntry),
        static_cast<GdkDragAction>(GDK_ACTION_MOVE | GDK_ACTION_COPY | GDK_ACTION_LINK | GDK_ACTION_PRIVATE));
}

void ShowGtkWidget(GtkWidget* widget)
{
    gtk_widget_show_all(GTK_WIDGET(widget));
}

void AddToGtkFixedContainer(GtkWidget* fixedContainer, GtkWidget* child, int x, int y)
{
    gtk_fixed_put(GTK_FIXED(fixedContainer), GTK_WIDGET(child), x, y);
}

void SetGtkWindowIsPopup(GtkWindow* window)
{
    gtk_window_set_type_hint(GTK_WINDOW(window), GDK_WINDOW_TYPE_HINT_POPUP_MENU);
}

void SetGtkWindowHasDecorations(GtkWindow* window, bool hasDecorations)
{
    gtk_window_set_decorated(GTK_WINDOW(window), hasDecorations ? 1 : 0);
}

void SetGtkWindowShouldGetFocus(GtkWindow* window, bool shouldGetFocus)
{
    gtk_window_set_focus_on_map(GTK_WINDOW(window), shouldGetFocus ? 1 : 0);
}



int main(void) {
        gtk_init(0, NULL);
        auto m_Window = CreateGtkWindow(true);
        GtkWidget* m_Fixed = CreateGtkFixedContainer();

        //SetGtkWindowTransientFor(s_FocusGuardian.GetCurrentParentWindow(), m_Window);
        SetGtkWindowIsPopup(m_Window);
        SetGtkWindowHasDecorations(m_Window, false);        
        SetGtkWindowShouldGetFocus(m_Window, true);
        SetGtkWidgetChild((GtkWidget*)m_Window, m_Fixed, false);
        SetGtkWindowSize(m_Window, 111, 111);
        SetGtkWindowGravity(m_Window);
        SetGtkWindowPosition(m_Window, 0, 0);

        // ConnectGtkSignal(m_Window, "map-event", (GtkCallbackPtr)OnMap, this);
        // ConnectGtkSignal(m_Window, "configure-event", (GtkCallbackPtr)OnConfigure, this);
        // ConnectGtkSignal(m_Window, "delete-event", (GtkCallbackPtr)OnClose, this);
        // ConnectGtkSignal(m_Window, "focus-in-event", (GtkCallbackPtr)OnFocusIn, this);
        // ConnectGtkSignal(m_Window, "focus-out-event", (GtkCallbackPtr)OnFocusOut, this);
        // ConnectGtkSignal(m_Window, "window-state-event", (GtkCallbackPtr)OnStateChange, this);

        auto m_View = CreateGtkDrawingArea();
        SetGtkWidgetSize(m_View, 111, 111);
        SetGtkWidgetCanFocus(m_View, true);
        RegisterGtkWidgetForMotionEvents(m_View, true /* ContainerWindow::kShowPopupMenu == m_Window->GetShowMode () */);
        RegisterGtkWidgetForKeyEvents(m_View);
        RegisterGtkWidgetForDragEvents(m_View);
        AddToGtkFixedContainer(m_Fixed, m_View, 0, 0);
        ShowGtkWidget((GtkWidget*)m_Window);
        // ConnectGtkSignal(m_View, "focus-in-event", (GtkCallbackPtr)OnGUIViewFocusIn, this);
        // ConnectGtkSignal(m_View, "focus-out-event", (GtkCallbackPtr)OnGUIViewFocusOut, this);
        // ConnectGtkSignal(m_View, "motion-notify-event", (GtkCallbackPtr)OnGUIViewMouseMove, this);
        // ConnectGtkSignal(m_View, "key-press-event", (GtkCallbackPtr)OnGUIViewKeyPress, this);
        // ConnectGtkSignal(m_View, "key-release-event", (GtkCallbackPtr)OnGUIViewKeyRelease, this);
        // ConnectGtkSignal(m_View, "button-press-event", (GtkCallbackPtr)OnGUIViewButtonPress, this);
        // ConnectGtkSignal(m_View, "button-release-event", (GtkCallbackPtr)OnGUIViewButtonRelease, this);
        // ConnectGtkSignal(m_View, "scroll-event", (GtkCallbackPtr)OnGUIViewScroll, this);
        // ConnectGtkSignal(m_View, "drag-begin", (GtkCallbackPtr)OnGUIViewDragBegin, this);
        // ConnectGtkSignal(m_View, "drag-motion", (GtkCallbackPtr)OnGUIViewDragMotion, this);
        // ConnectGtkSignal(m_View, "drag-leave", (GtkCallbackPtr)OnGUIViewDragLeave, this);
        // ConnectGtkSignal(m_View, "drag-drop", (GtkCallbackPtr)OnGUIViewDragDrop, this);
        // ConnectGtkSignal(m_View, "drag-failed", (GtkCallbackPtr)OnGUIViewDragFailed, this);
        // ConnectGtkSignal(m_View, "drag-data-received", (GtkCallbackPtr)OnGUIViewDragDataReceived, this);
        // ConnectGtkSignal(m_View, "enter-notify-event", (GtkCallbackPtr)OnGUIViewWindowCrossingEnter, this);
        // ConnectGtkSignal(m_View, "leave-notify-event", (GtkCallbackPtr)OnGUIViewWindowCrossingLeave, this);
        // ConnectGtkSignal(m_View, "map-event", (GtkCallbackPtr)OnGUIViewMap, this);
        gtk_main();

}
