#include <gtk/gtk.h>

static void app_activate(GApplication *app)
{
  g_printerr("You need a filename argument.\n");
}

static void app_open(GApplication *app, GFile **files, int n_files, char *hint)
{
  GtkWidget *win;
  GtkWidget *scrolledwindow;
  GtkWidget *textview;
  GtkTextBuffer *textbuffer;
  char *contents;
  gsize length;
  char *filename;
  GError *err = NULL;

  win = gtk_application_window_new(GTK_APPLICATION(app));
  gtk_window_set_default_size(GTK_WINDOW(win), 400, 300);

  scrolledwindow = gtk_scrolled_window_new();
  gtk_window_set_child(GTK_WINDOW(win), scrolledwindow);

  textview = gtk_text_view_new();
  textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textview), GTK_WRAP_WORD_CHAR);
  gtk_text_view_set_editable(GTK_TEXT_VIEW(textview), FALSE);
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolledwindow), textview);

  if (g_file_load_contents(files[0], NULL, &contents, &length, NULL, &err))
  {
    gtk_text_buffer_set_text(textbuffer, contents, length);
    g_free(contents);
    if ((filename = g_file_get_basename(files[0])) != NULL)
    {
      gtk_window_set_title(GTK_WINDOW(win), filename);
      g_free(filename);
    }
    gtk_window_present(GTK_WINDOW(win));
  }
  else
  {
    g_printerr("%s.\n", err->message);
    g_error_free(err);
    gtk_window_destroy(GTK_WINDOW(win));
  }
}

int main(int argc, char *argv[])
{
  GtkApplication *app;
  int status;

  app = gtk_application_new("learning_gtk.app_open", G_APPLICATION_HANDLES_OPEN);
  g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
  g_signal_connect(app, "open", G_CALLBACK(app_open), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}