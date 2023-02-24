#include "../inc/client.h"

void log_menu(GtkWidget *widget, t_info *info) {

    if (main_window)
        clear_window(main_window);
    
    create_new_window("Login", 400, 1, false);

    build_login(info);

    gtk_widget_show_all (main_window);

    if(info){}
        (void)widget;
}

void sign_up_menu(GtkWidget *widget, t_info *info) {
    
    if (main_window)
        clear_window(main_window);
    
    create_new_window("Sign up", 400, 1, false);

    build_signup(info);

    gtk_widget_show_all (main_window);
    if(info){}
        (void)widget;
}
