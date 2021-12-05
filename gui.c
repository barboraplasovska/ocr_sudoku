#include <gtk/gtk.h>
#include <stdlib.h>
#include "digitRecog.h"
#include "network.h"
//#include "solver.h"
#include "image_processing.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL.h"
#include <err.h>
#include <stdio.h>

typedef struct UI
{
    GtkWindow* window;
    char *filepath; // initial grid
    gchar* filename;
    GtkButton* solve_button;
    GtkButton* process_button;
    GtkButton* check_button;
    GtkButton* restart_button;
    GtkButton* exit_button;
    GtkFileChooserButton* file_chooser;
    GtkImage* processedImage;
    GtkImage* chosenImage;
    GtkImage* solvedImage; 
    GtkImage* oldImage; 
    GtkImage* generatedImage;
    GtkStack *stack;
    SDL_Surface *image_surface;
    int** recGrid;
}UI;

int grid[9][9];
int* p = &grid[0][0];

void on_choose(GtkFileChooser *chooser, gpointer userdata)
{
    UI* gui = userdata;
    //printf("set file name here and it is %s\n", gui->filename);
    gui->filename = gtk_file_chooser_get_preview_filename(chooser);
    //printf("set file name here and it is %s\n", gui->filename);
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_size(gui->filename, 650, 500, NULL);
    gtk_image_set_from_pixbuf(gui->chosenImage, pixbuf);
    gtk_widget_set_sensitive(GTK_WIDGET(gui->process_button),TRUE);
    //printf("set file name here and it is %s\n", gui->filename);
}

void processing(SDL_Surface *image_surface)
{
    //CreateFolder("image_processed_folder");
    char boxes[] = "Box00.bmp";
    //printf("before applying filter and %i\n", image_surface->w);
    ApplyAllFilters(image_surface, boxes);
    SDL_SaveBMP(image_surface,"processed.bmp");
}

void on_process(GtkButton *button,gpointer userdata)
{
    button = button;
    UI* gui = userdata;
    gui->image_surface = load_image(gui->filename);
    processing(gui->image_surface);
    gui->filepath = "processed.bmp";
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_size(gui->filepath, 650, 500, NULL);
    gtk_image_set_from_pixbuf(gui->processedImage,pixbuf);
    gtk_widget_set_sensitive(GTK_WIDGET(gui->check_button),TRUE);
    gtk_stack_set_visible_child_name(gui->stack,"process_page");
}

int** recognizeDigits(int** res)
{
    char boxPath[] = "Boxxy.bmp";
    //printf("justbeforeneural\n");
    Network *nn = setupNetwork();
    //nn = nn;
    //Network *nn = createNetwork(784,20,10);
    //printf("created network\n");
    //loadWeights("weights.txt", nn);
    FILE *f = fopen("grid", "r");
    FILE *ff = fopen("unsolvedGrid", "w");

    for (int y = 0; y < 9; y++)
    {
	
	    if (y==3 || y==6)
	    {
		fprintf(ff,"\n");
	    }
        for (int x = 0; x < 9; x++)
        {
	    //printf("the X is %c, The Y is: %c\n" ,x + '0', y + '0');
            boxPath[3] = x + '0';
            boxPath[4] = y + '0';
	    //printf("box path is: %s\n", boxPath);
            char c = fgetc(f);
            if (x==3||x==6)
	    {
		fprintf(ff," ");
	    }
	    //if (x==8)
	  //  {
	//	fprintf(ff,"\n");
	 //   }
	    //printf("the char is: %c\n", c);
            if (c == '1') // there is an image to work with
            {
                SDL_Surface *img = load_image(boxPath);
                res[y][x] = findDigit(nn, img);
                printf("the digit is: ............%i\n", findDigit(nn, img));
                //res[y][x] = 1;
                SDL_FreeSurface(img);
		fprintf(ff,"%i",res[y][x]);

            }
            else
            {
                //printf("stuff\n");
                res[y][x] = 0;
		fprintf(ff,".");

            }
 
        }
	fprintf(ff,"\n");
    }
    
    free(f);
    //free(ff);
    fclose(ff);
    shutDownNetwork(nn);
    
    printf("before res\n");

    return res;
}

void on_check(GtkButton* button,gpointer userdata)
{
    UI* gui = userdata;
    button = button;

    //int** oldgrid = FileToMatrix("grid_00");
    //int** grid = FileToMatrix("grid_00.result");

    //printf("value is: %i", oldgrid[0][0]);
    // ma
    //int** grid = (int **) malloc(9 * sizeof(int *));
    
    //for (int i = 0; i < 9; i++)
    //{
    //    grid[i] = (int *) malloc(9 * sizeof(int));
    //}
    
    gui->recGrid = recognizeDigits(gui->recGrid);
    //printf("before recognize\n");
    //recognizeDigits();
    //printf("did neural\n");
    SaveSolvedGrid(gui->recGrid, gui->recGrid,"recognized.bmp");
    //printf("after neural\n");
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_size(gui->filepath, 400, 400, NULL);
    gtk_image_set_from_pixbuf(gui->oldImage,pixbuf);
    pixbuf = gdk_pixbuf_new_from_file_at_size("recognized.bmp", 400, 400, NULL);
    gtk_image_set_from_pixbuf(gui->generatedImage,pixbuf);
    gtk_widget_set_sensitive(GTK_WIDGET(gui->solve_button),TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(gui->exit_button),TRUE);
    gtk_stack_set_visible_child_name(gui->stack,"check_digits_page");
    
    //for (int i=0; i< 9; ++i)
    //{
    //    free(grid[i]);
    //}

    //free(grid);
}

void load(char path[])
{
    char c;
    int x = 0;
    int y = 0;
    FILE* f = fopen(path, "r");
    while ((c = fgetc(f))!= EOF)
    {
        if ((c >= '1' && c <= '9' )|| c == '.')
        {
            if (c == '.')
                c = '0';
            *(p + y*9 + x) = c - '0';
            x+=1;
        }
        if (x == 9)
        {
            y += 1;
            x = 0;
        }
    }


    fclose(f);
}

int** FileToDoubleMatrix(char path[])
{
    int** grid = (int **) malloc(9 * sizeof(int *));

    for (int i = 0; i < 9; i++)
    {
        grid[i] = (int *) malloc(9 * sizeof(int));
    }

    //int *p = &grid[0][0];

    char c;
    int x = 0;
    int y = 0;
    FILE* f = fopen(path, "r");

    while ((c = fgetc(f))!= EOF)
    {
	if (c>'0' && c<='9')
	{
		grid[y][x] = c - '0';
		x=x+1;
		if (x==9)
		{
		   x=0;
		   y=y+1;
		}
	}
	    /*
        if ((c >= '1' && c <= '9' )|| c == '.')
        {
            if (c == '.')
                c = '0';
            *(p + y*9 + x) = c - '0';
            x+=1;
        }
        if (x == 9)
        {
            y += 1;
            x = 0;
        }*/
    }


    fclose(f);

    return grid;
}

int** FileToDoubleMatrix2(char path[])
{
    int** grid = (int **) malloc(9 * sizeof(int *));

    for (int i = 0; i < 9; i++)
    {
        grid[i] = (int *) malloc(9 * sizeof(int));
    }

    //int *p = &grid[0][0];

    char c;
    int x = 0;
    int y = 0;
    FILE* f = fopen(path, "r");

    while ((c = fgetc(f))!= EOF)
    {
	if (c>'0' && c<='9')
	{
		grid[y][x] = c - '0';
		x=x+1;
		if (x==9)
		{
		   x=0;
		   y=y+1;
		}
	}
	if ( c=='.')
	{
		grid[y][x]=0;
		x=x+1;
		if (x==9)
		{
		   x=0;
		   y=y+1;
		}
	}
	
	    /*
        if ((c >= '1' && c <= '9' )|| c == '.')
        {
            if (c == '.')
                c = '0';
            *(p + y*9 + x) = c - '0';
            x+=1;
        }
        if (x == 9)
        {
            y += 1;
            x = 0;
        }*/
    }


    fclose(f);

    return grid;
}


int is_column_solved(int x)
{
    for (int i = 1; i <= 9; i++)
    {
        int found = 0;
        for (int y = 0; !found && y < 9; y++)
        {
            if (*(p + y*9 + x) == i)
                found = 1;
        }

        if (!found)
            return 0;
    }

    return 1;
}

/// Returns true if the given line is solved
/// <param name="y"> index of the line</param>
int is_line_solved(int y)
{
    for (int i = 1; i <= 9; i++)
    {
        int found = 0;
        for (int x = 0; !found && x < 9; x++)
        {
            if (*(p + y*9 + x) == i)
                found = 1;
        }

        if (!found)
            return 0;
    }

    return 1;
}

/// Returns true if the 3x3 square containing the given coords is solved
/// <param name="x"> index of the column</param>
/// <param name="y"> index of the line</param>
int is_square_solved(int x, int y)
{
    x /= 3;
    y /= 3;

    for (int i = 1; i <= 9; i++)
    {
        int found = 0;
        for (int Y = 0; !found && Y < 3; Y++)
        {
            for (int X = 0; !found && X < 3; X++)
            {
                int v1 = Y + y * 3;
                int v2 = X + x * 3;
                if (*(p + v1*9 + v2) == i)
                    found = 1;
            }
        }

        if (!found)
            return 0;
    }

    return 1;
}

/// Returns true the grid is solved
int is_solved()
{
    for (int i = 0; i < 9; i++)
    {
        if (!is_column_solved(i) || !is_line_solved(i) ||
        !is_square_solved(i / 3, i % 3))
            return 0;
    }
    return 1;
}

/// <summary>
/// Returns true if the given column already contains the given value
/// </summary>
/// <param name="x"> index of the column</param>
/// <param name="val"> value that must be checked</param>
int already_in_column(int x, int val)
{
    for (int y = 0; y < 9; y++)
    {
        if (*(p + y*9 + x) == val)
            return 1;
    }

    return 0;
}

/// Returns true if the given line already contains the given value
/// <param name="y"> index of the line</param>
/// <param name="val"> value that must be checked</param>
int already_in_line(int y, int val)
{
    for (int x = 0; x < 9; x++)
    {
        if (*(p + y*9 + x) == val)
            return 1;
    }

    return 0;
}

/// Returns true if the 3x3 square containing the given
/// already contains the given value
/// <param name="x"> index of the column</param>
/// <param name="y"> index of the line</param>
/// <param name="val"> value that must be checked</param>
int already_in_square(int x, int y, int val)
{
    x /= 3;
    y /= 3;

    for (int Y = 0; Y < 3; Y++)
    {
        for (int X = 0; X < 3; X++)
        {
            int v1 = Y + y * 3;
            int v2 = X + x * 3;
            if (*(p + v1*9 + v2) == val)
                return 1;
        }
    }

    return 0;
}

void SetNextCoords(int *nextX,int *nextY)
{
    if (*nextX >= 9)
    {
        *nextX = 0;
        *nextY += 1;
    }
}

int _solve(int x, int y)
{
    if (y >= 9)
        return 1;

    int nextX = x + 1;
    int nextY = y;
    SetNextCoords(&nextX, &nextY);

    if (*(p + y*9 + x) != 0)
        return _solve(nextX,nextY);

    for (int i = 1; i <= 9; i++)
    {
        if (!already_in_column(x, i) &&
         !already_in_line(y, i) &&
         !already_in_square(x, y, i))
        {
            *(p + y*9 + x) = i;
            if (_solve(nextX, nextY))
                return 1;
            *(p + y*9 + x) = 0;
        }
    }

    return 0;
}

/// Solves the grid
void solve()
{
    _solve(0, 0);
}


/// Writes the grid into a new file "filename.result"
void finalProduct(char path[])
{
    char newpath[8] = ".result";
    char str3[100];

    int i = 0, j = 0;

    // Insert the first string in the new string
    while (path[i] != '\0') {
        str3[j] = path[i];
        i++;
        j++;
    }

    // Insert the second string in the new string
    i = 0;
    while (newpath[i] != '\0') {
        str3[j] = newpath[i];
        i++;
        j++;
    }

    str3[j] = '\0';
    FILE* f = fopen(str3, "w");
    for (int y = 0; y < 9; y++)
    {
        if ((y == 3) || (y==6))
        {
           fprintf(f,"\n");
        }

        for (int x = 0; x < 9 ; x++)
        {
            int val = *(p + y*9 + x);
            if (x == 8)
                fprintf(f,"%i\n",val);
            else if ((x == 2)||(x==5))
                fprintf(f,"%i ",val);
            else
                fprintf(f,"%i",val);
        }

    }
    fclose(f);

}

void on_solver(GtkButton * button,gpointer userdata)
{
    UI* gui = userdata;
    button = button;
/*     int** matrix = FileToMatrix(gui->filepath);
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

    int** solvedGrid = FileToMatrix(newpath); */
    //int **oldGrid = gui->recGrid;
    //solve(*(gui->recGrid));
    //SaveSolvedGrid(oldGrid, gui->recGrid,"solved.bmp");
   
    printf("before load in solver\n");
    load("unsolvedGrid");
    solve();
    finalProduct("unsolvedGrid");
    printf("after final product\n");

    int** dm = FileToDoubleMatrix("unsolvedGrid.result");
    
    int** dm2 = FileToDoubleMatrix2("unsolvedGrid");

    //printf("after double matrix\n");
//	FILE* f = fopen("test","w");
  //  for (int x = 0; x < 9 ; x++)
    //{
      //   for(int y = 0; y < 9; y++)
	// {
          //    printf("we're here\n");
            //  fprintf(f,"%i" ,dm[x][y]);
	// }
    
   // }
    SaveSolvedGrid(dm2, dm, "solved.bmp");

    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_size("solved.bmp", 650,
		    500, NULL);
    gtk_image_set_from_pixbuf(gui->solvedImage,pixbuf);
    gtk_stack_set_visible_child_name(gui->stack,"solve_page");
    /*
    for (int i=0; i< 9; ++i)
    {
        free(gui->recGrid[i]);
    }

    free();*/
}

void on_restart(GtkButton *button, gpointer userdata)
{  
    //printf("on restart\n");
    UI* gui = userdata;
    button = button;
    // reset the grid?
    gtk_image_set_from_pixbuf(gui->processedImage,NULL);
    gtk_image_set_from_pixbuf(gui->oldImage,NULL);
    gtk_image_set_from_pixbuf(gui->generatedImage,NULL);
    gtk_image_set_from_pixbuf(gui->chosenImage,NULL);
    gtk_image_set_from_pixbuf(gui->solvedImage,NULL);
    gtk_image_set_from_pixbuf(gui->chosenImage,NULL);
    gtk_widget_show(GTK_WIDGET(gui->file_chooser));
    gtk_stack_set_visible_child_name(gui->stack,"welcome_page");
}

void on_quit(GtkButton *button, gpointer userdata)
{
    UI* gui = userdata;
    button = button;
    gtk_widget_destroy(GTK_WIDGET(gui->window));
    printf("\e[1;1H\e[2J");
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
    GtkButton* restart_button = GTK_BUTTON(gtk_builder_get_object(builder, "restart_button"));
    GtkButton* exit_button = GTK_BUTTON(gtk_builder_get_object(builder, "exit_button"));
    GtkImage* processedImage = GTK_IMAGE(gtk_builder_get_object(builder, "processedImage"));
    GtkImage* chosenImage = GTK_IMAGE(gtk_builder_get_object(builder, "chosenImage"));
    GtkImage* solvedImage = GTK_IMAGE(gtk_builder_get_object(builder, "solvedImage"));
    GtkImage* oldImage = GTK_IMAGE(gtk_builder_get_object(builder, "oldImage"));
    GtkImage* generatedImage = GTK_IMAGE(gtk_builder_get_object(builder, "generatedImage"));

    GtkStack *stack = GTK_STACK(gtk_builder_get_object(builder, "stack"));

    SDL_Surface *image_surface = load_image("images/image_01.jpeg");
    char* filepath = "";
    char* filename = "";


    int** recGrid = (int **) malloc(9 * sizeof(int *));
    
    for (int i = 0; i < 9; i++)
    {
	recGrid[i] = (int *) malloc(9 * sizeof(int));
    }

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
        .generatedImage = generatedImage,
        .stack = stack,
        .image_surface = image_surface,
        .restart_button = restart_button,
        .check_button = check_button,
        .exit_button = exit_button,
	.recGrid = recGrid,
    };

    // Connects event handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(process_button, "clicked", G_CALLBACK(on_process), &gui);
    g_signal_connect(solve_button, "clicked", G_CALLBACK(on_solver), &gui);
    g_signal_connect(check_button, "clicked", G_CALLBACK(on_check), &gui);
    g_signal_connect(restart_button, "clicked", G_CALLBACK(on_restart), &gui);
    g_signal_connect(exit_button, "clicked", G_CALLBACK(on_quit), &gui);
    g_signal_connect(file_chooser, "selection-changed", G_CALLBACK(on_choose), &gui);


    gtk_widget_show(GTK_WIDGET(window));

    // Runs the main loop.
    gtk_main();

    // Exits.
    return 0;
}
