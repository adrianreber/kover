/* config.h.  Generated automatically by configure.  */
/* config.h.in.  Generated automatically from configure.in by autoheader.  */

/* Define if the C++ compiler supports BOOL */
#define HAVE_BOOL 1

/* defines if having libgif (always 1) */
#define HAVE_LIBGIF 1

/* defines if having libjpeg (always 1) */
#define HAVE_LIBJPEG 1

/* defines if having libtiff */
#define HAVE_LIBTIFF 1

/* defines if you have libpng */
#define HAVE_LIBPNG 1

/* defines which to take for ksize_t */
#define ksize_t socklen_t

/* Define to 1 if NLS is requested.  */
#define ENABLE_NLS 1

/* Define if you have the <linux/cdrom.h> header file.  */
#define HAVE_LINUX_CDROM_H 1

/* Define if you have the <linux/ucdrom.h> header file.  */
/* #undef HAVE_LINUX_UCDROM_H */

/* Name of package */
#define PACKAGE "kover"

/* Version number of package */
#define VERSION "0.4"

#define ENABLE_DEBUG_OUTPUT

#ifndef HAVE_BOOL
#define HAVE_BOOL
typedef int bool;
const bool false = 0;
const bool true = 1;
#endif
