/****************************************************************************
** PreferencesDialog meta object code from reading C++ file 'PreferencesDialog.h'
**
** Created: Tue Dec 26 22:34:57 2000
**      by: The Qt MOC ($Id: PreferencesDialog.moc.cpp,v 1.1 2000/12/30 22:10:08 adrian Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_PreferencesDialog
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 9
#elif Q_MOC_OUTPUT_REVISION != 9
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "PreferencesDialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *PreferencesDialog::className() const
{
    return "PreferencesDialog";
}

QMetaObject *PreferencesDialog::metaObj = 0;

void PreferencesDialog::initMetaObject()
{
    if ( metaObj )
	return;
    if ( qstrcmp(KDialogBase::className(), "KDialogBase") != 0 )
	badSuperclassWarning("PreferencesDialog","KDialogBase");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION

QString PreferencesDialog::tr(const char* s)
{
    return qApp->translate( "PreferencesDialog", s, 0 );
}

QString PreferencesDialog::tr(const char* s, const char * c)
{
    return qApp->translate( "PreferencesDialog", s, c );
}

#endif // QT_NO_TRANSLATION

QMetaObject* PreferencesDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) KDialogBase::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    QMetaData::Access *slot_tbl_access = 0;
    metaObj = QMetaObject::new_metaobject(
	"PreferencesDialog", "KDialogBase",
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    metaObj->set_slot_access( slot_tbl_access );
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    return metaObj;
}
