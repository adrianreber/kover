/****************************************************************************
** CDView meta object code from reading C++ file 'CDView.h'
**
** Created: Tue Dec 26 22:34:07 2000
**      by: The Qt MOC ($Id: CDView.moc.cpp,v 1.1 2000/12/30 22:10:07 adrian Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_CDView
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 9
#elif Q_MOC_OUTPUT_REVISION != 9
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "CDView.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *CDView::className() const
{
    return "CDView";
}

QMetaObject *CDView::metaObj = 0;

void CDView::initMetaObject()
{
    if ( metaObj )
	return;
    if ( qstrcmp(QFrame::className(), "QFrame") != 0 )
	badSuperclassWarning("CDView","QFrame");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION

QString CDView::tr(const char* s)
{
    return qApp->translate( "CDView", s, 0 );
}

QString CDView::tr(const char* s, const char * c)
{
    return qApp->translate( "CDView", s, c );
}

#endif // QT_NO_TRANSLATION

QMetaObject* CDView::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QFrame::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(CDView::*m1_t0)(bool);
    m1_t0 v1_0 = Q_AMPERSAND CDView::dataChanged;
    QMetaData *slot_tbl = QMetaObject::new_metadata(1);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(1);
    slot_tbl[0].name = "dataChanged(bool)";
    slot_tbl[0].ptr = (QMember)v1_0;
    slot_tbl_access[0] = QMetaData::Public;
    typedef void(CDView::*m2_t0)();
    typedef void(CDView::*m2_t1)();
    m2_t0 v2_0 = Q_AMPERSAND CDView::stopPreview;
    m2_t1 v2_1 = Q_AMPERSAND CDView::actualSize;
    QMetaData *signal_tbl = QMetaObject::new_metadata(2);
    signal_tbl[0].name = "stopPreview()";
    signal_tbl[0].ptr = (QMember)v2_0;
    signal_tbl[1].name = "actualSize()";
    signal_tbl[1].ptr = (QMember)v2_1;
    metaObj = QMetaObject::new_metaobject(
	"CDView", "QFrame",
	slot_tbl, 1,
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

// SIGNAL stopPreview
void CDView::stopPreview()
{
    activate_signal( "stopPreview()" );
}

// SIGNAL actualSize
void CDView::actualSize()
{
    activate_signal( "actualSize()" );
}
