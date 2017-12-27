#include <gtk/gtk.h>

GtkBuilder *builder;

int main(int argc, char *argv[])
{
  GtkWidget *window;

  gtk_init(&argc, &argv);

  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "../window_main.glade", NULL);

  window = GTK_WIDGET( gtk_builder_get_object(builder, "window") );
  gtk_builder_connect_signals(builder, NULL);

  gtk_widget_show(window);
  gtk_main();

  return 0;
}

void on_window_destroy()
{
  g_object_unref(builder);
  gtk_main_quit();
}

void on_button_clicked()
{
  GtkLabel *label;
  GtkEntry *entry;

  char message[128];

  entry = GTK_ENTRY( gtk_builder_get_object(builder, "entry") );
  const char *entryText = gtk_entry_get_text(entry);

  g_strlcpy(message, "Hello, ", 128);
  g_strlcat(message, entryText, 128);

  label = GTK_LABEL( gtk_builder_get_object(builder, "label") );
  gtk_label_set_text(label, message);
}
