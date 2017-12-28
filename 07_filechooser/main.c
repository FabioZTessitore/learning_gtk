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

gchar *get_filename()
{
  GtkFileChooser *fileChooser;
  fileChooser = GTK_FILE_CHOOSER( gtk_builder_get_object(builder, "fileChooser") );

  return gtk_file_chooser_get_filename(fileChooser);
}

void on_file_choosen()
{
  GtkLabel *labelPath;

  gchar *filename = get_filename();

  labelPath = GTK_LABEL( gtk_builder_get_object(builder, "labelPath") );
  gtk_label_set_text(labelPath, filename);
}

void on_button_clicked()
{
  GtkLabel *labelResult;

  gchar *filename = get_filename();

  labelResult = GTK_LABEL( gtk_builder_get_object(builder, "labelResult") );

  FILE *f;
  char message[128] = "";

  /* non viene selezionato un file */
  if (!g_file_test(filename, G_FILE_TEST_EXISTS)) {
    gtk_label_set_text(labelResult, "Il file non esiste");
    return;
  }

  /* non dovrebbe succedere, fileChooser non permette di selezionare directory */
  if (!g_file_test(filename, G_FILE_TEST_IS_REGULAR)) {
    gtk_label_set_text(labelResult, "Non hai selezionato un file");
    return;
  }

  f = fopen(filename, "r");
  if (!f) {
    gtk_label_set_text(labelResult, "Impossibile aprire il file");
    return;
  }

  int c;
  int n_vocali = 0; /* numero di vocali */

  while ( (c = fgetc(f)) != EOF) {
    switch(c) {
      case 'a':
      case 'e':
      case 'i':
      case 'o':
      case 'u':
        n_vocali++;
        break;
      default:
        /* nothing to do */
        break;
    }
  }

  fclose(f);

  g_snprintf(message, 128, "N. Vocali: %d", n_vocali);
  gtk_label_set_text(labelResult, message);
}
