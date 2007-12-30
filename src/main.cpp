#include "kover.h"
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <KDE/KLocale>

static const char description[] =
    I18N_NOOP("A KDE 4 Application");

static const char version[] = "4";

int main(int argc, char **argv)
{
    KAboutData about("kover", 0, ki18n("kover"), version, ki18n(description),
                     KAboutData::License_GPL, ki18n("(C) 2007 Adrian Reber"), KLocalizedString(), 0, "adrian@lisas.de");
    about.addAuthor( ki18n("Adrian Reber"), KLocalizedString(), "adrian@lisas.de" );
    KCmdLineArgs::init(argc, argv, &about);

    KCmdLineOptions options;
    options.add("+[URL]", ki18n( "Document to open" ));
    KCmdLineArgs::addCmdLineOptions(options);
    KApplication app;

    kover *widget = new kover;

    // see if we are starting with session management
    if (app.isSessionRestored())
    {
        RESTORE(kover);
    }
    else
    {
        // no session.. just start up normally
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
        if (args->count() == 0)
        {
            //kover *widget = new kover;
            widget->show();
        }
        else
        {
            int i = 0;
            for (; i < args->count(); i++)
            {
                //kover *widget = new kover;
                widget->show();
            }
        }
        args->clear();
    }

    return app.exec();
}
