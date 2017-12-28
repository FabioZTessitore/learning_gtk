#include <gtk/gtk.h>
#include <stdlib.h>
#include <math.h>

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

void on_radioGeom_toggled()
{
  GtkRadioButton *radioCube, *radioSphere;
  GtkLabel *labelInput;

  radioCube = GTK_RADIO_BUTTON( gtk_builder_get_object(builder, "radioCube") );
  radioSphere = GTK_RADIO_BUTTON( gtk_builder_get_object(builder, "radioSphere") );
  labelInput = GTK_LABEL( gtk_builder_get_object(builder, "labelInput") );

  if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(radioCube) ) == TRUE) {
    gtk_label_set_text(labelInput, "Lato del Cubo:");
  } else if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(radioSphere) ) == TRUE) {
    gtk_label_set_text(labelInput, "Raggio della Sfera:");
  } else {
    gtk_label_set_text(labelInput, "Selezione non valida");
    return;
  }
}

void on_button_clicked()
{
  GtkEntry *entry;
  GtkRadioButton *radioCube, *radioSphere;
  GtkLabel *labelResult;

  char message[128];
  char *ptrNext;

  radioCube = GTK_RADIO_BUTTON( gtk_builder_get_object(builder, "radioCube") );
  radioSphere = GTK_RADIO_BUTTON( gtk_builder_get_object(builder, "radioSphere") );

  entry = GTK_ENTRY( gtk_builder_get_object(builder, "entry") );
  const char *entryText = gtk_entry_get_text(entry);
  gdouble n = strtod(entryText, &ptrNext);

  labelResult = GTK_LABEL( gtk_builder_get_object(builder, "labelResult") );

  if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(radioCube) ) == TRUE) {
    g_snprintf(message, 128, "Volume: %g", n*n*n);
  } else if (gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(radioSphere) ) == TRUE) {
    g_snprintf(message, 128, "Volume: %g", 4./3. * M_PI * n*n*n);
  } else {
    g_strlcpy(message, "Non hai scelto nessuna geometria", 128);
  }

  gtk_label_set_text(labelResult, message);
}
