/****************************************************************************
** CDDB_Fill meta object code from reading C++ file 'cddb_fill.h'
**
** Created: Fri Dec 29 22:22:06 2000
**      by: The Qt MOC ($Id: cddb_fill.moc.cpp,v 1.1 2000/12/30 22:10:08 adrian Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_CDDB_Fill
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 9
#elif Q_MOC_OUTPUT_REVISION != 9
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "cddb_fill.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *CDDB_Fill::className() const
{
    return "CDDB_Fill";
}

QMetaObject *CDDB_Fill::metaObj = 0;

void CDDB_Fill::initMetaObject()
{
    if ( metaObj )
	return;
    if ( qstrcmp(QObject::className(), "QObject") != 0 )
	badSuperclassWarning("CDDB_Fill","QObject");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION

QString CDDB_Fill::tr(const char* s)
{
    return qApp->translate( "CDDB_Fill", s, 0 );
}

QString CDDB_Fill::tr(const char* s, const char * c)
{
    return qApp->translate( "CDDB_Fill", s, c );
}

#endif // QT_NO_TRANSLATION

QMetaObject* CDDB_Fill::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QObject::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    QMetaData::Access *slot_tbl_access = 0;
    typedef void(CDDB_Fill::*m2_t0)(const char*);
    typedef void(CDDB_Fill::*m2_t1)(bool);
    m2_t0 v2_0 = Q_AMPERSAND CDDB_Fill::statusText;
    m2_t1 v2_1 = Q_AMPERSAND CDDB_Fill::updateDisplay;
    QMetaData *signal_tbl = QMetaObject::new_metadata(2);
    signal_tbl[0].name = "statusText(const char*)";
    signal_tbl[0].ptr = (QMember)v2_0;
    signal_tbl[1].name = "updateDisplay(bool)";
    signal_tbl[1].ptr = (QMember)v2_1;
    metaObj = QMetaObject::new_metaobject(
	"CDDB_Fill", "QObject",
	0, 0,
	signal_tbl, 2,
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

// SIGNAL statusText
void CDDB_Fill::statusText( const char* t0 )
{
    activate_signal( "statusText(const char*)", t0 );
}

// SIGNAL updateDisplay
void CDDB_Fill::updateDisplay( bool t0 )
{
    activate_signal_bool( "updateDisplay(bool)", t0 );
}
