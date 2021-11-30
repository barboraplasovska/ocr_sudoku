#include <gtk/gtk.h>
#include "image_processing.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <err.h>



typedef struct UI
{
    GtkApplicationWindow* welcome,
    GtkApplicationWindow* display,
    GtkApplicationWindow* processed,
    GtkApplicationWindow* solved,
    char[]* filepath,
    SDL_Surface* image_surface,
    GtkButton* backtoProcessed_button,
    GtkButton* save_button,
    GtkButton* backtoDisplay_button,
    GtkButton* solve_button,
    GtkButton* backtoWelcome_button,
    GtkButton* process_button,
    GtkFileChooserButton* choose_file_button,
    GtkButton* next_button,
    GtkImage* processedImage,
    GtkImage* chosenImage,
    GtkImage* solvedImage, 
}UI;

GtkWidget* gtk_image_new_from_sdl_surface (SDL_Surface *surface)
{
    Uint32 src_format;
    Uint32 dst_format;

    GdkPixbuf *pixbuf;
    gboolean has_alpha;
    int rowstride;
    guchar *pixels;

    GtkWidget *image;

    // select format                                                            
    src_format = surface->format->format;
    has_alpha = SDL_ISPIXELFORMAT_ALPHA(src_format);
    if (has_alpha) {
        dst_format = SDL_PIXELFORMAT_RGBA32;
    }
    else {
        dst_format = SDL_PIXELFORMAT_RGB24;
    }

    // create pixbuf                                                            
    pixbuf = gdk_pixbuf_new (GDK_COLORSPACE_RGB, has_alpha, 8,
                             surface->w, surface->h);
    rowstride = gdk_pixbuf_get_rowstride (pixbuf);
    pixels = gdk_pixbuf_get_pixels (pixbuf);

    // copy pixels                                                              
    SDL_LockSurface(surface);
    SDL_ConvertPixels (surface->w, surface->h, src_format,
               surface->pixels, surface->pitch,
               dst_format, pixels, rowstride);
    SDL_UnlockSurface(surface);

    // create GtkImage from pixbuf                                              
    image = gtk_image_new_from_pixbuf (pixbuf);

    // release our reference to the pixbuf                                      
    g_object_unref (pixbuf);

    return image;
}

void on_display(GtkButton *button)
{

}

void on_process(GtkButton *button,gpointer* userdata)
{
    //close display
    //open process
    UI gui = userdata;
    image_surface = load_image("image.jpeg");
    ApplyBlackAndWhite(gui.image_surface, 180);
    image_surface = RotateSurface(gui.image_surface, 37);
    char filtered[] = "filtered.jpeg";
    SDL_SaveBMP(gui.image_surface, filtered);
    SDL_FreeSurface(gui.image_surface);
    gtk_image_set_from_file(gui.processedImage,filtered);
}

void on_solver(GtkButton *button,gpointer* userdata)
{   
    //close process
    //open solve
    UI gui = userdata;
    //solver code
    //saved image
    char[] path[] = "solved.jpeg";
    image_surface = load_image(path);
    //image_surface = ; fct that creates it
    gtk_image_set_from_image(gui.solvedImage,
        GTK_IMAGE(gtk_image_new_from_sdl_surface(gui.image_surface)));
    SDL_FreeSurface(gui.image_surface);
}

void on_save(GtkButton *button,gpointer* userdata)
{
    UI gui = userdata;
    SDL_SaveBMP(ui.solvedImage,"solved.jpeg");
}

void on_backDisplay(GtkButton *button,gpointer* userdata)
{

}

void on_backWelcome(GtkButton *button,gpointer* userdata)
{

}

void on_backProcessed(GtkButton *button,gpointer* userdata)
{

}

void update_image(GtkFileChooser *file_chooser, gpointer* data)
{
    UI gui = userdata;
    GtkWidget *preview;
    char *filename;
    GdkPixbuf *pixbuf;
    gboolean have_preview;

    preview = GTK_WIDGET(gui.chosenImage);
    filename = gtk_file_chooser_get_preview_filename(file_chooser);

    pixbuf = gdk_pixbuf_new_from_file_at_size (filename, 128, 128, NULL);
    have_preview = (pixbuf != NULL);
    g_free (filename);

    gtk_image_set_from_pixbuf (GTK_IMAGE (preview), pixbuf);
    if (pixbuf)
        g_object_unref (pixbuf);

    gtk_file_chooser_set_preview_widget_active (file_chooser, have_preview);
}


int main (int argc, char *argv[])
{
    // Initializes GTK.
    gtk_init(NULL, NULL);

    // Constructs a GtkBuilder instance.
    GtkBuilder* window = gtk_builder_new ();

    // Loads the UI description.
    // (Exits if an error occurs.)
    GError* error = NULL;
    if (gtk_builder_add_from_file(window, "welcome_window.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // Gets the widgets.
    GtkApplicationWindow* welcome = GTK_WINDOW(gtk_builder_get_object(window, "org.gtk.welcome_window"));
    GtkApplicationWindow* display = GTK_WINDOW(gtk_builder_get_object(window, "org.gtk.welcome_window"));
    GtkApplicationWindow* processed = GTK_WINDOW(gtk_builder_get_object(window, "org.gtk.welcome_window"));
    GtkApplicationWindow* solved = GTK_WINDOW(gtk_builder_get_object(window, "org.gtk.welcome_window"));


    GtkButton* next_button = GTK_BUTTON(gtk_builder_get_object(window, "next_button"));
    GtkButton* process_button = GTK_BUTTON(gtk_builder_get_object(window, "process_button"));
    GtkFileChooserButton* choose_file_button = GTK_BUTTON(gtk_builder_get_object(window, "choose_file_button"));
    GtkButton* solve_button = GTK_BUTTON(gtk_builder_get_object(window, "solve_button"));
    GtkButton* backtoDisplay_button = GTK_BUTTON(gtk_builder_get_object(window, "backtoDisplay_button"));
    GtkButton* backtoWelcome_button = GTK_BUTTON(gtk_builder_get_object(window, "backtoWelcome_button"));
    GtkButton* backtoProcessed_button = GTK_BUTTON(gtk_builder_get_object(window, "backtoProcessed_button"));

    GtkImage* processedImage = GTK_IMAGE(gtk_builder_get_object(window, "processed"));
    GtkImage* chosenImage = GTK_IMAGE(gtk_builder_get_object(window, "input"));
    GtkImage* solvedImage = GTK_IMAGE(gtk_builder_get_object(window, "solved"));

    UI gui =
    {
        .welcome = welcome,
        .display = display,
        .processed = processed,
        .solved = solved,
        .filepath = filepath,
        .image_surface = image_surface,
        .backtoProcessed_button = backtoProcessed_button,
        .save_button = save_button,
        .backtoDisplay_button = backtoDisplay_button,
        .solve_button = solve_button,
        .backtoWelcome_button = backtoWelcome_button,
        .process_button = process_button,
        .choose_file_button = choose_file_button,
        .next_button = next_button,
        .processedImage = processedImage,
        .chosenImage = chosenImage,
        .solvedImage = solvedImage,

    }   

    GtkWidget *preview;

    // Connects event handlers.
    g_signal_connect(welcome, "destroy", G_CALLBACK(gtk_main_quit), gui);
    g_signal_connect(process_button, "clicked", G_CALLBACK(on_process), gui);
    g_signal_connect(next_button, "clicked", G_CALLBACK(on_display), gui);
    g_signal_connect(solve_button, "clicked", G_CALLBACK(on_solver), gui);
    g_signal_connect(backtoDisplay_button,"clicked",G_CALLBACK(on_backDisplay), gui);
    g_signal_connect(backtoProcessed_button,"clicked",G_CALLBACK(on_backProcessed), gui);
    g_signal_connect(backtoWelcome_button,"clicked",G_CALLBACK(on_backWelcome), gui);
    gtk_file_chooser_set_preview_widget(choose_file_button, chosenImage);
    g_signal_connect(choose_file_button, "update-preview", G_CALLBACK (update_image), preview);

    // Runs the main loop.
    gtk_main();

    // Exits.
    return 0;
}