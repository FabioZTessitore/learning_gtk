#include <gtk/gtk.h>

static void app_activate(GApplication *app)
{
  g_printerr("You need a filename argument.\n");
}

static void app_open(GApplication *app, GFile **files, int n_files, char *hint)
{
  GtkWidget *win;
  GtkWidget *notebook;
  GtkWidget *label;
  GtkNotebookPage *notebookpage;
  GtkWidget *scrolledwindow;
  GtkWidget *textview;
  GtkTextBuffer *textbuffer;
  char *contents;
  gsize length;
  char *filename;
  GError *err = NULL;
  int i;

  win = gtk_application_window_new(GTK_APPLICATION(app));
  gtk_window_set_title(GTK_WINDOW(win), "GtkNotebook");
  gtk_window_set_default_size(GTK_WINDOW(win), 600, 400);

  notebook = gtk_notebook_new();
  gtk_window_set_child(GTK_WINDOW(win), notebook);

  for (i = 0; i < n_files; i++)
  {
    if (g_file_load_contents(files[i], NULL, &contents, &length, NULL, &err))
    {
      scrolledwindow = gtk_scrolled_window_new();

      textview = gtk_text_view_new();
      textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
      gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textview), GTK_WRAP_WORD_CHAR);
      gtk_text_view_set_editable(GTK_TEXT_VIEW(textview), FALSE);
      gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolledwindow), textview);

      gtk_text_buffer_set_text(textbuffer, contents, length);
      g_free(contents);
      if ((filename = g_file_get_basename(files[i])) != NULL)
      {
        label = gtk_label_new(filename);
        g_free(filename);
      }
      else
      {
        label = gtk_label_new("");
      }

      gtk_notebook_append_page(GTK_NOTEBOOK(notebook), scrolledwindow, label);
      notebookpage = gtk_notebook_get_page(GTK_NOTEBOOK(notebook), scrolledwindow);
      g_object_set(notebookpage, "tab-expand", TRUE, NULL);
    }
    else
    {
      g_printerr("%s.\n", err->message);
      g_clear_error(&err);
    }
  }

  if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) > 0)
  {
    gtk_window_present(GTK_WINDOW(win));
  }
  else
  {
    gtk_window_destroy(GTK_WINDOW(win));
  }
}

int main(int argc, char *argv[])
{
  GtkApplication *app;
  int status;

  app = gtk_application_new("learning_gtk.notebook", G_APPLICATION_HANDLES_OPEN);
  g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
  g_signal_connect(app, "open", G_CALLBACK(app_open), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}