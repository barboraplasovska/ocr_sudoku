#include <gtk/gtk.h>
#include "image_processing.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL.h"
#include <err.h>

typedef struct UI
{
    GtkWindow* window;
    char *filepath; // initial grid
    gchar *filename;
    GtkButton* solve_button;
    GtkButton* process_button;
    GtkButton* restart_button;
    GtkFileChooserButton* file_chooser;
    GtkImage* processedImage;
    GtkImage* chosenImage;
    GtkImage* solvedImage; 
    GtkStack *stack;
    SDL_Surface *image_surface;
}UI;

void on_choose(GtkFileChooser *chooser, gpointer userdata)
{
    UI* gui = userdata;
    gui->filename = gtk_file_chooser_get_preview_filename(chooser);
    GdkPixbuf *pixbuf= gdk_pixbuf_new_from_file_at_size(gui->filename, 650, 500, NULL);
    gtk_image_set_from_pixbuf(gui->chosenImage, pixbuf);
    gtk_widget_set_sensitive(GTK_WIDGET(gui->process_button),TRUE);
}

void processing(gpointer userdata)
{
    UI* gui = userdata;
    gui->image_surface = load_image(gui->filename);
    ApplyBlackAndWhite(gui->image_surface, 180); //change value
    //gui->image_surface = RotateSurface(gui->image_surface, 37); //change value
    SDL_SaveBMP(gui->image_surface,"processed.jpeg");
    gtk_image_set_from_file(gui->processedImage,"processed.jpeg");
    gui->filepath = "processed.jpeg";
}

void on_process(GtkButton *button, gpointer userdata)
{
    UI* gui = userdata;
    processing(gui);
    gtk_widget_set_sensitive(GTK_WIDGET(gui->solve_button),TRUE);
    gtk_stack_set_visible_child_name(gui->stack,"process_page");
}


void on_solver(GtkButton *button, gpointer userdata)
{   
    //get initial grid
    //get solved grid (neural network)
    UI* gui = userdata;
    //open solve
    //solver code
    int** matrix = FileToMatrix(gui->filepath);

    char newpath[8] = ".result";
    char str3[100];
  
    int i = 0, j = 0;
  
    // Insert the first string in the new string
    while (gui->filepath[i] != '\0') 
    {
        str3[j] = gui->filepath[i];
        i++;
        j++;
    }
  
    // Insert the second string in the new string
    i = 0;
    while (newpath[i] != '\0') 
    {
        str3[j] = newpath[i];
        i++;
        j++;
    }

    int** solvedGrid = FileToMatrix(newpath);
    gui->image_surface = SaveSolvedGrid(matrix, solvedGrid);
    SDL_SaveBMP(gui->image_surface,"solved.jpeg");
    gtk_image_set_from_file(gui->solvedImage,"solved.jpeg");

    gtk_stack_set_visible_child_name(gui->stack,"solve_page");
}

void on_restart(GtkButton *button, gpointer userdata)
{   
    UI* gui = userdata;
    gtk_image_set_from_pixbuf(gui->chosenImage,NULL);
    gtk_widget_show(GTK_WIDGET(gui->file_chooser));
    gtk_widget_hide(GTK_WIDGET(button));
}


int main (int argc, char **argv)
{
    // Initializes GTK.
    gtk_init(&argc, &argv);

    // Constructs a GtkBuilder instance.
    GtkBuilder* builder = gtk_builder_new();

    // Loads the UI description.
    // (Exits if an error occurs.)
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "gui.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    g_object_set(gtk_settings_get_default(),"gtk-application-prefer-dark-theme",TRUE,NULL);
 
    // Gets the widgets.
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));

    GtkButton* process_button = GTK_BUTTON(gtk_builder_get_object(builder, "process_button"));
    GtkFileChooserButton* file_chooser = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder, "file_chooser"));
    GtkButton* solve_button = GTK_BUTTON(gtk_builder_get_object(builder, "solve_button"));
    GtkButton* restart_button = GTK_BUTTON(gtk_builder_get_object(builder, "restart_button"));
    GtkImage* processedImage = GTK_IMAGE(gtk_builder_get_object(builder, "processedImage"));
    GtkImage* chosenImage = GTK_IMAGE(gtk_builder_get_object(builder, "chosenImage"));
    GtkImage* solvedImage = GTK_IMAGE(gtk_builder_get_object(builder, "solvedImage"));

    GtkStack *stack = GTK_STACK(gtk_builder_get_object(builder, "stack"));

    SDL_Surface *image_surface = load_image("image_01.jpeg");
    char* filepath = "";
    gchar* filename = "";

    UI gui =
    {
        .window = window,
        .filepath = filepath,
        .filename = filename,
        .solve_button = solve_button,
        .process_button = process_button,
        .file_chooser = file_chooser,
        .processedImage = processedImage,
        .chosenImage = chosenImage,
        .solvedImage = solvedImage,
        .stack = stack,
        .image_surface = image_surface,
        .restart_button = restart_button,

    };   

    // Connects event handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(process_button, "clicked", G_CALLBACK(on_process), &gui);
    g_signal_connect(solve_button, "clicked", G_CALLBACK(on_solver), &gui);
    g_signal_connect(restart_button, "clicked", G_CALLBACK(on_restart), &gui);
    g_signal_connect(file_chooser, "selection-changed", G_CALLBACK(on_choose), &gui);


    gtk_widget_show(GTK_WIDGET(window));

    // Runs the main loop.
    gtk_main();

    // Exits.
    return 0;
}