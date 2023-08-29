#include <gtk/gtk.h>
#include "tfetextview.h"

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