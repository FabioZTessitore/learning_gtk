#include <gtk/gtk.h>

/* Define TfeTextView Widget */

#define TFE_TYPE_TEXT_VIEW tfe_text_view_get_type()
G_DECLARE_FINAL_TYPE(TfeTextView, tfe_text_view, TFE, TEXT_VIEW, GtkTextView)

struct _TfeTextView
{
  GtkTextView parent;
  GFile *file;
};

G_DEFINE_FINAL_TYPE(TfeTextView, tfe_text_view, GTK_TYPE_TEXT_VIEW);

static void tfe_text_view_init(TfeTextView *textview)
{
  textview->file = NULL;
}

static void tfe_text_view_class_init(TfeTextViewClass *class)
{
}

void tfe_text_view_set_file(TfeTextView *textview, GFile *f)
{
  textview->file = f;
}

GFile *tfe_text_view_get_file(TfeTextView *textview)
{
  return textview->file;
}

GtkWidget *tfe_text_view_new(void)
{
  return GTK_WIDGET(g_object_new(TFE_TYPE_TEXT_VIEW, NULL));
}

/* ---------------- end Define TfeTextView Widget */

static gboolean before_close(GtkWindow *win, GtkWidget *notebook)
{
  unsigned int n;
  unsigned int i;
  GtkWidget *scrolledwindow;
  GtkWidget *textview;
  GFile *file;
  GtkTextBuffer *textbuffer;
  GtkTextIter start_iter;
  GtkTextIter end_iter;
  char *contents;
  GError *err;

  n = gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook));
  for (i = 0; i < n; ++i)
  {
    scrolledwindow = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
    textview = gtk_scrolled_window_get_child(GTK_SCROLLED_WINDOW(scrolledwindow));
    file = tfe_text_view_get_file(TFE_TEXT_VIEW(textview));
    textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
    gtk_text_buffer_get_bounds(textbuffer, &start_iter, &end_iter);
    contents = gtk_text_buffer_get_text(textbuffer, &start_iter, &end_iter, FALSE);
    if (!g_file_replace_contents(file, contents, strlen(contents), NULL, TRUE, G_FILE_CREATE_NONE, NULL, NULL, &err))
    {
      g_printerr("%s.\n", err->message);
      g_clear_error(&err);
    }
    g_free(contents);
    g_object_unref(file);
  }

  return FALSE;
}

static void app_activate(GApplication *app)
{
  g_print("You need to give filenames as arguments.\n");
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
  gtk_window_set_title(GTK_WINDOW(win), "file editor");
  gtk_window_set_default_size(GTK_WINDOW(win), 600, 400);

  notebook = gtk_notebook_new();
  gtk_window_set_child(GTK_WINDOW(win), notebook);

  for (i = 0; i < n_files; i++)
  {
    if (g_file_load_contents(files[i], NULL, &contents, &length, NULL, &err))
    {
      scrolledwindow = gtk_scrolled_window_new();

      textview = tfe_text_view_new();
      textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
      gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textview), GTK_WRAP_WORD_CHAR);
      gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolledwindow), textview);

      tfe_text_view_set_file(TFE_TEXT_VIEW(textview), g_file_dup(files[i]));
      gtk_text_buffer_set_text(textbuffer, contents, length);
      g_free(contents);
      filename = g_file_get_basename(files[i]);
      label = gtk_label_new(filename);
      g_free(filename);
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
    g_signal_connect(win, "close-request", G_CALLBACK(before_close), notebook);
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

  app = gtk_application_new("learning_gtk.textfileeditor1", G_APPLICATION_HANDLES_OPEN);
  g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
  g_signal_connect(app, "open", G_CALLBACK(app_open), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}