/****************************************************************************
** ImageDlg meta object code from reading C++ file 'imagedlg.h'
**
** Created: Tue Dec 26 22:34:15 2000
**      by: The Qt MOC ($Id: imagedlg.moc.cpp,v 1.1 2000/12/30 22:10:08 adrian Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_ImageDlg
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 9
#elif Q_MOC_OUTPUT_REVISION != 9
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "imagedlg.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *ImageDlg::className() const
{
    return "ImageDlg";
}

QMetaObject *ImageDlg::metaObj = 0;

void ImageDlg::initMetaObject()
{
    if ( metaObj )
	return;
    if ( qstrcmp(QDialog::className(), "QDialog") != 0 )
	badSuperclassWarning("ImageDlg","QDialog");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION

QString ImageDlg::tr(const char* s)
{
    return qApp->translate( "ImageDlg", s, 0 );
}

QString ImageDlg::tr(const char* s, const char * c)
{
    return qApp->translate( "ImageDlg", s, c );
}

#endif // QT_NO_TRANSLATION

QMetaObject* ImageDlg::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QDialog::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(ImageDlg::*m1_t0)();
    typedef void(ImageDlg::*m1_t1)();
    typedef void(ImageDlg::*m1_t2)();
    typedef void(ImageDlg::*m1_t3)();
    typedef void(ImageDlg::*m1_t4)();
    m1_t0 v1_0 = Q_AMPERSAND ImageDlg::OK;
    m1_t1 v1_1 = Q_AMPERSAND ImageDlg::Cancel;
    m1_t2 v1_2 = Q_AMPERSAND ImageDlg::Browse1;
    m1_t3 v1_3 = Q_AMPERSAND ImageDlg::Browse2;
    m1_t4 v1_4 = Q_AMPERSAND ImageDlg::Browse3;
    QMetaData *slot_tbl = QMetaObject::new_metadata(5);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(5);
    slot_tbl[0].name = "OK()";
    slot_tbl[0].ptr = (QMember)v1_0;
    slot_tbl_access[0] = QMetaData::Protected;
    slot_tbl[1].name = "Cancel()";
    slot_tbl[1].ptr = (QMember)v1_1;
    slot_tbl_access[1] = QMetaData::Protected;
    slot_tbl[2].name = "Browse1()";
    slot_tbl[2].ptr = (QMember)v1_2;
    slot_tbl_access[2] = QMetaData::Protected;
    slot_tbl[3].name = "Browse2()";
    slot_tbl[3].ptr = (QMember)v1_3;
    slot_tbl_access[3] = QMetaData::Protected;
    slot_tbl[4].name = "Browse3()";
    slot_tbl[4].ptr = (QMember)v1_4;
    slot_tbl_access[4] = QMetaData::Protected;
    metaObj = QMetaObject::new_metaobject(
	"ImageDlg", "QDialog",
	slot_tbl, 5,
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
