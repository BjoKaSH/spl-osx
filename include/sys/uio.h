/*****************************************************************************\
 *  Copyright (C) 2007-2010 Lawrence Livermore National Security, LLC.
 *  Copyright (C) 2007 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Brian Behlendorf <behlendorf1@llnl.gov>.
 *  UCRL-CODE-235197
 *
 *  This file is part of the SPL, Solaris Porting Layer.
 *  For details, see <http://github.com/behlendorf/spl/>.
 *
 *  The SPL is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  The SPL is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with the SPL.  If not, see <http://www.gnu.org/licenses/>.
\*****************************************************************************/

#ifndef _SPL_UIO_H
#define _SPL_UIO_H

//#include <linux/uio.h>


// OSX defines "uio_t" as "struct uio *"
// ZFS defines "uio_t" as "struct uio"
#undef uio_t
#include_next <sys/uio.h>
#define uio_t struct uio

//#include <asm/uaccess.h>
#include <sys/types.h>

typedef struct iovec iovec_t;

typedef enum uio_seg uio_seg_t;
typedef enum uio_rw uio_rw_t;



#if 0
typedef enum uio_rw {
	UIO_READ =	0,
	UIO_WRITE =	1,
} uio_rw_t;

typedef enum uio_seg {
	UIO_USERSPACE =	0,
	UIO_SYSSPACE =	1,
	UIO_USERISPACE=	2,
} uio_seg_t;



typedef struct uio {
    //struct uio {
	struct iovec	*uio_iov;
	int		uio_iovcnt;
	offset_t	uio_loffset;
	uio_seg_t	uio_segflg;
	uint16_t	uio_fmode;
	uint16_t	uio_extflg;
	offset_t	uio_limit;
	ssize_t		uio_resid;
    } uio_t;
//} ;

#endif

typedef struct aio_req {
	uio_t		*aio_uio;
	void		*aio_private;
} aio_req_t;

typedef enum xuio_type {
	UIOTYPE_ASYNCIO,
	UIOTYPE_ZEROCOPY,
} xuio_type_t;


#define UIOA_IOV_MAX    16

typedef struct uioa_page_s {
	int	uioa_pfncnt;
	void	**uioa_ppp;
	caddr_t	uioa_base;
	size_t	uioa_len;
} uioa_page_t;

typedef struct xuio {
	uio_t *xu_uio;
	enum xuio_type xu_type;
	union {
		struct {
			uint32_t xu_a_state;
			ssize_t xu_a_mbytes;
			uioa_page_t *xu_a_lcur;
			void **xu_a_lppp;
			void *xu_a_hwst[4];
			uioa_page_t xu_a_locked[UIOA_IOV_MAX];
		} xu_aio;

		struct {
			int xu_zc_rw;
			void *xu_zc_priv;
		} xu_zc;
	} xu_ext;
} xuio_t;

#define XUIO_XUZC_PRIV(xuio)	xuio->xu_ext.xu_zc.xu_zc_priv
#define XUIO_XUZC_RW(xuio)	xuio->xu_ext.xu_zc.xu_zc_rw


/*
 * same as uiomove() but doesn't modify uio structure.
 * return in cbytes how many bytes were copied.
 */
static inline int uiocopy(void *p, size_t n, enum uio_rw rw, struct uio *uio, size_t *cbytes) \
{                                                \
    int result;                                  \
    struct uio *nuio = uio_duplicate(uio);       \
    if (!nuio) return ENOMEM;                    \
    result = uiomove(p,n,nuio);                  \
    uio_free(nuio);                              \
    return result;                               \
}


// Apple's uiomove puts the uio_rw in uio_create
#define uiomove(A,B,C,D) uiomove((A),(B),(D))
#define uioskip(A,B)     uio_update((A), (B))

#endif /* SPL_UIO_H */
