/****************************************************************************
** KoverTop meta object code from reading C++ file 'KoverTop.h'
**
** Created: Fri Dec 29 21:17:31 2000
**      by: The Qt MOC ($Id: KoverTop.moc.cpp,v 1.1 2000/12/30 22:10:07 adrian Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_KoverTop
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 9
#elif Q_MOC_OUTPUT_REVISION != 9
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "KoverTop.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *KoverTop::className() const
{
    return "KoverTop";
}

QMetaObject *KoverTop::metaObj = 0;

void KoverTop::initMetaObject()
{
    if ( metaObj )
	return;
    if ( qstrcmp(KMainWindow::className(), "KMainWindow") != 0 )
	badSuperclassWarning("KoverTop","KMainWindow");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION

QString KoverTop::tr(const char* s)
{
    return qApp->translate( "KoverTop", s, 0 );
}

QString KoverTop::tr(const char* s, const char * c)
{
    return qApp->translate( "KoverTop", s, c );
}

#endif // QT_NO_TRANSLATION

QMetaObject* KoverTop::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) KMainWindow::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(KoverTop::*m1_t0)();
    typedef void(KoverTop::*m1_t1)();
    typedef void(KoverTop::*m1_t2)(int);
    typedef void(KoverTop::*m1_t3)(int);
    typedef void(KoverTop::*m1_t4)();
    typedef void(KoverTop::*m1_t5)(bool);
    typedef void(KoverTop::*m1_t6)(int);
    typedef void(KoverTop::*m1_t7)(bool);
    typedef void(KoverTop::*m1_t8)(const char*);
    typedef void(KoverTop::*m1_t9)();
    typedef void(KoverTop::*m1_t10)(bool);
    typedef void(KoverTop::*m1_t11)();
    typedef void(KoverTop::*m1_t12)();
    typedef void(KoverTop::*m1_t13)();
    typedef void(KoverTop::*m1_t14)();
    typedef void(KoverTop::*m1_t15)();
    typedef void(KoverTop::*m1_t16)();
    typedef void(KoverTop::*m1_t17)();
    typedef void(KoverTop::*m1_t18)();
    typedef void(KoverTop::*m1_t19)();
    typedef void(KoverTop::*m1_t20)();
    typedef void(KoverTop::*m1_t21)();
    typedef void(KoverTop::*m1_t22)();
    m1_t0 v1_0 = Q_AMPERSAND KoverTop::contentsBoxChanged;
    m1_t1 v1_1 = Q_AMPERSAND KoverTop::titleBoxChanged;
    m1_t2 v1_2 = Q_AMPERSAND KoverTop::handleMainToolBar;
    m1_t3 v1_3 = Q_AMPERSAND KoverTop::handleCdToolBar;
    m1_t4 v1_4 = Q_AMPERSAND KoverTop::stopPreview;
    m1_t5 v1_5 = Q_AMPERSAND KoverTop::numberChecked;
    m1_t6 v1_6 = Q_AMPERSAND KoverTop::numberChanged;
    m1_t7 v1_7 = Q_AMPERSAND KoverTop::dataChanged;
    m1_t8 v1_8 = Q_AMPERSAND KoverTop::setStatusText;
    m1_t9 v1_9 = Q_AMPERSAND KoverTop::actualSize;
    m1_t10 v1_10 = Q_AMPERSAND KoverTop::updateDisplay;
    m1_t11 v1_11 = Q_AMPERSAND KoverTop::fileNew;
    m1_t12 v1_12 = Q_AMPERSAND KoverTop::fileOpen;
    m1_t13 v1_13 = Q_AMPERSAND KoverTop::fileSave;
    m1_t14 v1_14 = Q_AMPERSAND KoverTop::fileSaveAs;
    m1_t15 v1_15 = Q_AMPERSAND KoverTop::filePrint;
    m1_t16 v1_16 = Q_AMPERSAND KoverTop::cut;
    m1_t17 v1_17 = Q_AMPERSAND KoverTop::copy;
    m1_t18 v1_18 = Q_AMPERSAND KoverTop::paste;
    m1_t19 v1_19 = Q_AMPERSAND KoverTop::editToolbars;
    m1_t20 v1_20 = Q_AMPERSAND KoverTop::cddbFill;
    m1_t21 v1_21 = Q_AMPERSAND KoverTop::preferences;
    m1_t22 v1_22 = Q_AMPERSAND KoverTop::imageEmbedding;
    QMetaData *slot_tbl = QMetaObject::new_metadata(23);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(23);
    slot_tbl[0].name = "contentsBoxChanged()";
    slot_tbl[0].ptr = (QMember)v1_0;
    slot_tbl_access[0] = QMetaData::Public;
    slot_tbl[1].name = "titleBoxChanged()";
    slot_tbl[1].ptr = (QMember)v1_1;
    slot_tbl_access[1] = QMetaData::Public;
    slot_tbl[2].name = "handleMainToolBar(int)";
    slot_tbl[2].ptr = (QMember)v1_2;
    slot_tbl_access[2] = QMetaData::Public;
    slot_tbl[3].name = "handleCdToolBar(int)";
    slot_tbl[3].ptr = (QMember)v1_3;
    slot_tbl_access[3] = QMetaData::Public;
    slot_tbl[4].name = "stopPreview()";
    slot_tbl[4].ptr = (QMember)v1_4;
    slot_tbl_access[4] = QMetaData::Public;
    slot_tbl[5].name = "numberChecked(bool)";
    slot_tbl[5].ptr = (QMember)v1_5;
    slot_tbl_access[5] = QMetaData::Public;
    slot_tbl[6].name = "numberChanged(int)";
    slot_tbl[6].ptr = (QMember)v1_6;
    slot_tbl_access[6] = QMetaData::Public;
    slot_tbl[7].name = "dataChanged(bool)";
    slot_tbl[7].ptr = (QMember)v1_7;
    slot_tbl_access[7] = QMetaData::Public;
    slot_tbl[8].name = "setStatusText(const char*)";
    slot_tbl[8].ptr = (QMember)v1_8;
    slot_tbl_access[8] = QMetaData::Public;
    slot_tbl[9].name = "actualSize()";
    slot_tbl[9].ptr = (QMember)v1_9;
    slot_tbl_access[9] = QMetaData::Public;
    slot_tbl[10].name = "updateDisplay(bool)";
    slot_tbl[10].ptr = (QMember)v1_10;
    slot_tbl_access[10] = QMetaData::Public;
    slot_tbl[11].name = "fileNew()";
    slot_tbl[11].ptr = (QMember)v1_11;
    slot_tbl_access[11] = QMetaData::Private;
    slot_tbl[12].name = "fileOpen()";
    slot_tbl[12].ptr = (QMember)v1_12;
    slot_tbl_access[12] = QMetaData::Private;
    slot_tbl[13].name = "fileSave()";
    slot_tbl[13].ptr = (QMember)v1_13;
    slot_tbl_access[13] = QMetaData::Private;
    slot_tbl[14].name = "fileSaveAs()";
    slot_tbl[14].ptr = (QMember)v1_14;
    slot_tbl_access[14] = QMetaData::Private;
    slot_tbl[15].name = "filePrint()";
    slot_tbl[15].ptr = (QMember)v1_15;
    slot_tbl_access[15] = QMetaData::Private;
    slot_tbl[16].name = "cut()";
    slot_tbl[16].ptr = (QMember)v1_16;
    slot_tbl_access[16] = QMetaData::Private;
    slot_tbl[17].name = "copy()";
    slot_tbl[17].ptr = (QMember)v1_17;
    slot_tbl_access[17] = QMetaData::Private;
    slot_tbl[18].name = "paste()";
    slot_tbl[18].ptr = (QMember)v1_18;
    slot_tbl_access[18] = QMetaData::Private;
    slot_tbl[19].name = "editToolbars()";
    slot_tbl[19].ptr = (QMember)v1_19;
    slot_tbl_access[19] = QMetaData::Private;
    slot_tbl[20].name = "cddbFill()";
    slot_tbl[20].ptr = (QMember)v1_20;
    slot_tbl_access[20] = QMetaData::Private;
    slot_tbl[21].name = "preferences()";
    slot_tbl[21].ptr = (QMember)v1_21;
    slot_tbl_access[21] = QMetaData::Private;
    slot_tbl[22].name = "imageEmbedding()";
    slot_tbl[22].ptr = (QMember)v1_22;
    slot_tbl_access[22] = QMetaData::Private;
    metaObj = QMetaObject::new_metaobject(
	"KoverTop", "KMainWindow",
	slot_tbl, 23,
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
