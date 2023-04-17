#include "../inc/client.h"

void log_menu(GtkWidget *__attribute__((unused)) widget)
{
    if (main_window)
        clear_window(main_window);

    create_new_window("Login", 400, 1, false);

    build_login(info);

    gtk_widget_show_all(main_window);
}

void sign_up_menu(GtkWidget *__attribute__((unused)) widget)
{
    if (main_window)
        clear_window(main_window);

    create_new_window("Sign up", 400, 1, false);

    build_signup(info);

    gtk_widget_show_all(main_window);
}
