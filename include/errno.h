//
// Created by cheyao on 27.12.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//

#ifndef _ERRNO_H
#define _ERRNO_H

#if defined(__cplusplus)
extern "C" {
#endif

// POSIX errors from https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/errno.h.html
#define EDOM            1  /* Numerical argument out of domain */
#define EILSEQ          2  /* Illegal byte sequence */
#define ERANGE          3  /* Result too large */
#define E2BIG           4  /* Argument list too long */
#define EACCES          5  /* Permission denied */
#define EADDRINUSE      6  /* Address in use. */
#define EADDRNOTAVAIL   7  /* Address not available */
#define EAFNOSUPPORT    8  /* Address family not supported */
#define EAGAIN          9  /* Resource unavailable, try again (may be the same value as [EWOULDBLOCK]) */
#define EALREADY        10 /* Connection already in progress */
#define EBADF           11 /* Bad file descriptor */
#define EBADMSG         12 /* Bad message */ \
#define EBUSY           13 /* Device or resource busy */
#define ECANCELED       14 /* Operation canceled */
#define ECHILD          15 /* No child processes */
#define ECONNABORTED    16 /* Connection aborted */
#define ECONNREFUSED    17 /* Connection refused */
#define ECONNRESET      18 /* Connection reset */
#define EDEADLK         19 /* Resource deadlock would occur */
#define EDESTADDRREQ    20 /* Destination address required */
#define EDQUOT          21 /* Reserved */
#define EEXIST          22 /* File exists */
#define EFAULT          23 /* Bad address */
#define EFBIG           24 /* File too large */
#define EHOSTUNREACH    25 /* Host is unreachable */
#define EIDRM           26 /* Identifier removed */
#define EINPROGRESS     27 /* Operation in progress */
#define EINTR           28 /* Interrupted function */
#define EINVAL          29 /* Invalid argument */
#define EIO             30 /* I/O error */
#define EISCONN         31 /* Socket is connected */
#define EISDIR          32 /* Is a directory */
#define ELOOP           33 /* Too many levels of symbolic links */
#define EMFILE          34 /* File descriptor value too large */
#define EMLINK          35 /* Too many links */
#define EMSGSIZE        36 /* Message too large */
#define EMULTIHOP       37 /* Reserved */
#define ENAMETOOLONG    38 /* Filename too long */
#define ENETDOWN        39 /* Network is down */
#define ENETRESET       40 /* Connection aborted by network */
#define ENETUNREACH     41 /* Network unreachable */
#define ENFILE          42 /* Too many files open in system */
#define ENOBUFS         43 /* No buffer space available */
#define ENODATA         44 /* [OB XSR] [Option Start] No message is available on the STREAM head read queue */ [Option End]
#define ENODEV          45 /* No such device */
#define ENOENT          46 /* No such file or directory */
#define ENOEXEC         47 /* Executable file format error */
#define ENOLCK          48 /* No locks available */
#define ENOLINK         49 /* Reserved */
#define ENOMEM          50 /* Not enough space */
#define ENOMSG          51 /* No message of the desired type */
#define ENOPROTOOPT     52 /* Protocol not available */
#define ENOSPC          53 /* No space left on device */
#define ENOSR           54 /* [OB XSR] [Option Start] No STREAM resources */ [Option End]
#define ENOSTR          55 /* [OB XSR] [Option Start] Not a STREAM */ [Option End]
#define ENOSYS          56 /* Functionality not supported */
#define ENOTCONN        57 /* The socket is not connected */
#define ENOTDIR         58 /* Not a directory or a symbolic link to a directory */
#define ENOTEMPTY       59 /* Directory not empty */
#define ENOTRECOVERABLE 60 /* State not recoverable */
#define ENOTSOCK        61 /* Not a socket */
#define ENOTSUP         62 /* Not supported (may be the same value as [EOPNOTSUPP]) */
#define ENOTTY          63 /* Inappropriate I/O control operation */
#define ENXIO           64 /* No such device or address */
#define EOPNOTSUPP      65 /* Operation not supported on socket (may be the same value as [ENOTSUP]) */
#define EOVERFLOW       66 /* Value too large to be stored in data type */
#define EOWNERDEAD      67 /* Previous owner died */
#define EPERM           68 /* Operation not permitted */
#define EPIPE           69 /* Broken pipe */
#define EPROTO          70 /* Protocol error */
#define EPROTONOSUPPORT 71 /* Protocol not supported */
#define EPROTOTYPE      72 /* Protocol wrong type for socket */
#define EROFS           73 /* Read-only file system */
#define ESPIPE          74 /* Invalid seek */
#define ESRCH           75 /* No such process */
#define ESTALE          76 /* Reserved */
#define ETIME           77 /* [OB XSR] [Option Start] Stream ioctl() timeout */ [Option End]
#define ETIMEDOUT       78 /* Connection timed out */
#define ETXTBSY         79 /* Text file busy */
#define EWOULDBLOCK     80 /* Operation would block (may be the same value as [EAGAIN]) */
#define EXDEV           81 /* Cross-device link */

extern int __error;
#define errno __error

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif //_ERRNO_H
