/****************************************************************************
** KoverFile meta object code from reading C++ file 'koverfile.h'
**
** Created: Tue Dec 26 22:34:17 2000
**      by: The Qt MOC ($Id: koverfile.moc.cpp,v 1.1 2000/12/30 22:10:08 adrian Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_KoverFile
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 9
#elif Q_MOC_OUTPUT_REVISION != 9
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "koverfile.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *KoverFile::className() const
{
    return "KoverFile";
}

QMetaObject *KoverFile::metaObj = 0;

void KoverFile::initMetaObject()
{
    if ( metaObj )
	return;
    if ( qstrcmp(QObject::className(), "QObject") != 0 )
	badSuperclassWarning("KoverFile","QObject");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION

QString KoverFile::tr(const char* s)
{
    return qApp->translate( "KoverFile", s, 0 );
}

QString KoverFile::tr(const char* s, const char * c)
{
    return qApp->translate( "KoverFile", s, c );
}

#endif // QT_NO_TRANSLATION

QMetaObject* KoverFile::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QObject::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    QMetaData::Access *slot_tbl_access = 0;
    typedef void(KoverFile::*m2_t0)(bool);
    m2_t0 v2_0 = Q_AMPERSAND KoverFile::dataChanged;
    QMetaData *signal_tbl = QMetaObject::new_metadata(1);
    signal_tbl[0].name = "dataChanged(bool)";
    signal_tbl[0].ptr = (QMember)v2_0;
    metaObj = QMetaObject::new_metaobject(
	"KoverFile", "QObject",
	0, 0,
	signal_tbl, 1,
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

// SIGNAL dataChanged
void KoverFile::dataChanged( bool t0 )
{
    activate_signal_bool( "dataChanged(bool)", t0 );
}
