#include <gtk/gtk.h>
#include <stdlib.h>
#include "digitRecog.h"
#include "network.h"
#include "solver.h"
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
    GtkButton* check_button;
    GtkButton* submit_button;
    GtkButton* restart_button;
    GtkFileChooserButton* file_chooser;
    GtkImage* processedImage;
    GtkImage* chosenImage;
    GtkImage* solvedImage; 
    GtkImage* oldImage; 
    GtkImage* generatedImage;
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
    char boxes[] = "boxes";
    ApplyAllFilters(image_surface, boxes);
    SDL_SaveBMP(gui->image_surface,"processed.bmp");
    gtk_image_set_from_file(gui->processedImage,"processed.bmp");
    gui->filepath = "processed.bmp";
}

void on_process(GtkButton *button, gpointer userdata)
{
    UI* gui = userdata;
    processing(gui);
    gtk_widget_set_sensitive(GTK_WIDGET(gui->check_button),TRUE);
    gtk_stack_set_visible_child_name(gui->stack,"process_page");
}

void on_check(GtkButton *button, gpointer userdata)
{   
    UI* gui = userdata;
    gtk_image_set_from_file(gui->oldImage,"processed.bmp");
    gtk_widget_set_sensitive(GTK_WIDGET(gui->solve_button),TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(gui->submit_button),TRUE);
    gtk_stack_set_visible_child_name(gui->stack,"check_digits_page");
}

void on_remake(GtkButton *button, gpointer userdata)
{   
    UI* gui = userdata;
    gtk_image_set_from_file(gui->generatedImage,"processed.bmp");
    gtk_widget_set_sensitive(GTK_WIDGET(gui->submit_button),TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(gui->solve_button),TRUE);
    gtk_stack_set_visible_child_name(gui->stack,"check_digits_page");
}

int recognizeDigits()
{
    Char boxPath[] = "boxxy.bmp";
    Network *nn = setupNetwork();
    FILE *f = fopen("grid", "r");
    int res[9][9];
    for (size_t x = 0; x < 9; x++)
    {
        for (size_t y = 0; y < 9; y++)
        {
            boxPath[3] = x;
            boxPath[4] = y;
            char c = fgetc(f);
            if (c == '1') // there is an image to work with
            {
                SDL_Surface *img = load_image(boxPath);
                res[x][y] = findDigit(nn, img);
                SDL_FreeSurace(img);
            }
            else
                res[x][y] = 0;
        }
    }
    return res;
}


void on_solver(GtkButton *button, gpointer userdata)
{   
    //get initial grid
    //get solved grid (neural network)
    UI* gui = userdata;
    //open solve
    //solver code
    /* int** matrix = FileToMatrix(gui->filepath);

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
    SDL_SaveBMP(gui->image_surface,"solved.bmp"); 
    gtk_image_set_from_file(gui->solvedImage,"solved.bmp"); */

    recognizeDigits();

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
    GtkButton* check_button = GTK_BUTTON(gtk_builder_get_object(builder, "check_button"));
    GtkButton* submit_button = GTK_BUTTON(gtk_builder_get_object(builder, "submit_button"));;
    GtkButton* restart_button = GTK_BUTTON(gtk_builder_get_object(builder, "restart_button"));
    GtkImage* processedImage = GTK_IMAGE(gtk_builder_get_object(builder, "processedImage"));
    GtkImage* chosenImage = GTK_IMAGE(gtk_builder_get_object(builder, "chosenImage"));
    GtkImage* solvedImage = GTK_IMAGE(gtk_builder_get_object(builder, "solvedImage"));
    GtkImage* oldImage = GTK_IMAGE(gtk_builder_get_object(builder, "oldImage"));
    GtkImage* generatedImage = GTK_IMAGE(gtk_builder_get_object(builder, "generatedImage"));

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
        .oldImage = oldImage,
        .stack = stack,
        .image_surface = image_surface,
        .restart_button = restart_button,
        .check_button = check_button,
        .submit_button = submit_button,
        .generatedImage = generatedImage,

    };   

    // Connects event handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(process_button, "clicked", G_CALLBACK(on_process), &gui);
    g_signal_connect(solve_button, "clicked", G_CALLBACK(on_solver), &gui);
    g_signal_connect(check_button, "clicked", G_CALLBACK(on_check), &gui);
    g_signal_connect(submit_button, "clicked", G_CALLBACK(on_remake), &gui);
    g_signal_connect(restart_button, "clicked", G_CALLBACK(on_restart), &gui);
    g_signal_connect(file_chooser, "selection-changed", G_CALLBACK(on_choose), &gui);


    gtk_widget_show(GTK_WIDGET(window));

    // Runs the main loop.
    gtk_main();

    // Exits.
    return 0;
}