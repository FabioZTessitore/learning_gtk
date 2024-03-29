#include <gtk/gtk.h>

static void app_activate(GApplication *app)
{
  GtkWidget *win;
  GtkWidget *scrolledwindow;
  GtkWidget *textview;
  GtkTextBuffer *textbuffer;
  gchar *text = "Once upon a time, there was an old man who was called Taketori-no-Okina. "
                "It is a japanese word that means a man whose work is making bamboo baskets.\n"
                "One day, he went into a hill and found a shining bamboo. "
                "\"What a mysterious bamboo it is!,\" he said. "
                "He cut it, then there was a small cute baby girl in it. "
                "The girl was shining faintly. "
                "He thought this baby girl is a gift from Heaven and took her home.\n"
                "His wife was surprized at his story. "
                "They were very happy because they had no children.";

  win = gtk_application_window_new(GTK_APPLICATION(app));
  gtk_window_set_title(GTK_WINDOW(win), "GtkScrolledWindow");
  gtk_window_set_default_size(GTK_WINDOW(win), 400, 300);

  scrolledwindow = gtk_scrolled_window_new();
  gtk_window_set_child(GTK_WINDOW(win), scrolledwindow);

  textview = gtk_text_view_new();
  textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
  gtk_text_buffer_set_text(textbuffer, text, -1);
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textview), GTK_WRAP_WORD_CHAR);

  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolledwindow), textview);

  gtk_window_present(GTK_WINDOW(win));
}

int main(int argc, char *argv[])
{
  GtkApplication *app;
  int status;

  app = gtk_application_new("learning_gtk.scrolledwindow", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}