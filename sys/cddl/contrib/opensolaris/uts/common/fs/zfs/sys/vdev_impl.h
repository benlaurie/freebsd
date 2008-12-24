/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */
/*
 * Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#ifndef _SYS_VDEV_IMPL_H
#define	_SYS_VDEV_IMPL_H

#include <sys/avl.h>
#include <sys/dmu.h>
#include <sys/metaslab.h>
#include <sys/nvpair.h>
#include <sys/space_map.h>
#include <sys/vdev.h>
#include <sys/dkio.h>
#include <sys/uberblock_impl.h>

#ifdef	__cplusplus
extern "C" {
#endif

/*
 * Virtual device descriptors.
 *
 * All storage pool operations go through the virtual device framework,
 * which provides data replication and I/O scheduling.
 */

/*
 * Forward declarations that lots of things need.
 */
typedef struct vdev_queue vdev_queue_t;
typedef struct vdev_cache vdev_cache_t;
typedef struct vdev_cache_entry vdev_cache_entry_t;

/*
 * Virtual device operations
 */
typedef int	vdev_open_func_t(vdev_t *vd, uint64_t *size, uint64_t *ashift);
typedef void	vdev_close_func_t(vdev_t *vd);
typedef uint64_t vdev_asize_func_t(vdev_t *vd, uint64_t psize);
typedef int	vdev_io_start_func_t(zio_t *zio);
typedef void	vdev_io_done_func_t(zio_t *zio);
typedef void	vdev_state_change_func_t(vdev_t *vd, int, int);

typedef struct vdev_ops {
	vdev_open_func_t		*vdev_op_open;
	vdev_close_func_t		*vdev_op_close;
	vdev_asize_func_t		*vdev_op_asize;
	vdev_io_start_func_t		*vdev_op_io_start;
	vdev_io_done_func_t		*vdev_op_io_done;
	vdev_state_change_func_t	*vdev_op_state_change;
	char				vdev_op_type[16];
	boolean_t			vdev_op_leaf;
} vdev_ops_t;

/*
 * Virtual device properties
 */
struct vdev_cache_entry {
	char		*ve_data;
	uint64_t	ve_offset;
	uint64_t	ve_lastused;
	avl_node_t	ve_offset_node;
	avl_node_t	ve_lastused_node;
	uint32_t	ve_hits;
	uint16_t	ve_missed_update;
	zio_t		*ve_fill_io;
};

struct vdev_cache {
	avl_tree_t	vc_offset_tree;
	avl_tree_t	vc_lastused_tree;
	kmutex_t	vc_lock;
};

struct vdev_queue {
	avl_tree_t	vq_deadline_tree;
	avl_tree_t	vq_read_tree;
	avl_tree_t	vq_write_tree;
	avl_tree_t	vq_pending_tree;
	kmutex_t	vq_lock;
};

/*
 * Virtual device descriptor
 */
struct vdev {
	/*
	 * Common to all vdev types.
	 */
	uint64_t	vdev_id;	/* child number in vdev parent	*/
	uint64_t	vdev_guid;	/* unique ID for this vdev	*/
	uint64_t	vdev_guid_sum;	/* self guid + all child guids	*/
	uint64_t	vdev_asize;	/* allocatable device capacity	*/
	uint64_t	vdev_ashift;	/* block alignment shift	*/
	uint64_t	vdev_state;	/* see VDEV_STATE_* #defines	*/
	uint64_t	vdev_prevstate;	/* used when reopening a vdev	*/
	vdev_ops_t	*vdev_ops;	/* vdev operations		*/
	spa_t		*vdev_spa;	/* spa for this vdev		*/
	void		*vdev_tsd;	/* type-specific data		*/
	vdev_t		*vdev_top;	/* top-level vdev		*/
	vdev_t		*vdev_parent;	/* parent vdev			*/
	vdev_t		**vdev_child;	/* array of children		*/
	uint64_t	vdev_children;	/* number of children		*/
	space_map_t	vdev_dtl_map;	/* dirty time log in-core state	*/
	space_map_t	vdev_dtl_scrub;	/* DTL for scrub repair writes	*/
	vdev_stat_t	vdev_stat;	/* virtual device statistics	*/

	/*
	 * Top-level vdev state.
	 */
	uint64_t	vdev_ms_array;	/* metaslab array object	*/
	uint64_t	vdev_ms_shift;	/* metaslab size shift		*/
	uint64_t	vdev_ms_count;	/* number of metaslabs		*/
	metaslab_group_t *vdev_mg;	/* metaslab group		*/
	metaslab_t	**vdev_ms;	/* metaslab array		*/
	txg_list_t	vdev_ms_list;	/* per-txg dirty metaslab lists	*/
	txg_list_t	vdev_dtl_list;	/* per-txg dirty DTL lists	*/
	txg_node_t	vdev_txg_node;	/* per-txg dirty vdev linkage	*/
	boolean_t	vdev_remove_wanted; /* async remove wanted?	*/
	boolean_t	vdev_probe_wanted; /* async probe wanted?	*/
	list_node_t	vdev_config_dirty_node; /* config dirty list	*/
	list_node_t	vdev_state_dirty_node; /* state dirty list	*/
	uint64_t	vdev_deflate_ratio; /* deflation ratio (x512)	*/
	uint64_t	vdev_islog;	/* is an intent log device	*/

	/*
	 * Leaf vdev state.
	 */
	uint64_t	vdev_psize;	/* physical device capacity	*/
	space_map_obj_t	vdev_dtl;	/* dirty time log on-disk state	*/
	txg_node_t	vdev_dtl_node;	/* per-txg dirty DTL linkage	*/
	uint64_t	vdev_wholedisk;	/* true if this is a whole disk */
	uint64_t	vdev_offline;	/* persistent offline state	*/
	uint64_t	vdev_faulted;	/* persistent faulted state	*/
	uint64_t	vdev_degraded;	/* persistent degraded state	*/
	uint64_t	vdev_removed;	/* persistent removed state	*/
	uint64_t	vdev_nparity;	/* number of parity devices for raidz */
	char		*vdev_path;	/* vdev path (if any)		*/
	char		*vdev_devid;	/* vdev devid (if any)		*/
	char		*vdev_physpath;	/* vdev device path (if any)	*/
	uint64_t	vdev_not_present; /* not present during import	*/
	uint64_t	vdev_unspare;	/* unspare when resilvering done */
	hrtime_t	vdev_last_try;	/* last reopen time		*/
	boolean_t	vdev_nowritecache; /* true if flushwritecache failed */
	boolean_t	vdev_checkremove; /* temporary online test	*/
	boolean_t	vdev_forcefault; /* force online fault		*/
	uint8_t		vdev_tmpoffline; /* device taken offline temporarily? */
	uint8_t		vdev_detached;	/* device detached?		*/
	uint8_t		vdev_cant_read;	/* vdev is failing all reads	*/
	uint8_t		vdev_cant_write; /* vdev is failing all writes	*/
	uint64_t	vdev_isspare;	/* was a hot spare		*/
	uint64_t	vdev_isl2cache;	/* was a l2cache device		*/
	vdev_queue_t	vdev_queue;	/* I/O deadline schedule queue	*/
	vdev_cache_t	vdev_cache;	/* physical block cache		*/
	spa_aux_vdev_t	*vdev_aux;	/* for l2cache vdevs		*/
	zio_t		*vdev_probe_zio; /* root of current probe	*/

	/*
	 * For DTrace to work in userland (libzpool) context, these fields must
	 * remain at the end of the structure.  DTrace will use the kernel's
	 * CTF definition for 'struct vdev', and since the size of a kmutex_t is
	 * larger in userland, the offsets for the rest fields would be
	 * incorrect.
	 */
	kmutex_t	vdev_dtl_lock;	/* vdev_dtl_{map,resilver}	*/
	kmutex_t	vdev_stat_lock;	/* vdev_stat			*/
	kmutex_t	vdev_probe_lock; /* protects vdev_probe_zio	*/
};

#define	VDEV_SKIP_SIZE		(8 << 10)
#define	VDEV_BOOT_HEADER_SIZE	(8 << 10)
#define	VDEV_PHYS_SIZE		(112 << 10)
#define	VDEV_UBERBLOCK_RING	(128 << 10)

#define	VDEV_UBERBLOCK_SHIFT(vd)	\
	MAX((vd)->vdev_top->vdev_ashift, UBERBLOCK_SHIFT)
#define	VDEV_UBERBLOCK_COUNT(vd)	\
	(VDEV_UBERBLOCK_RING >> VDEV_UBERBLOCK_SHIFT(vd))
#define	VDEV_UBERBLOCK_OFFSET(vd, n)	\
	offsetof(vdev_label_t, vl_uberblock[(n) << VDEV_UBERBLOCK_SHIFT(vd)])
#define	VDEV_UBERBLOCK_SIZE(vd)		(1ULL << VDEV_UBERBLOCK_SHIFT(vd))

/* ZFS boot block */
#define	VDEV_BOOT_MAGIC		0x2f5b007b10cULL
#define	VDEV_BOOT_VERSION	1		/* version number	*/

typedef struct vdev_boot_header {
	uint64_t	vb_magic;		/* VDEV_BOOT_MAGIC	*/
	uint64_t	vb_version;		/* VDEV_BOOT_VERSION	*/
	uint64_t	vb_offset;		/* start offset	(bytes) */
	uint64_t	vb_size;		/* size (bytes)		*/
	char		vb_pad[VDEV_BOOT_HEADER_SIZE - 4 * sizeof (uint64_t)];
} vdev_boot_header_t;

typedef struct vdev_phys {
	char		vp_nvlist[VDEV_PHYS_SIZE - sizeof (zio_block_tail_t)];
	zio_block_tail_t vp_zbt;
} vdev_phys_t;

typedef struct vdev_label {
	char		vl_pad[VDEV_SKIP_SIZE];			/*   8K	*/
	vdev_boot_header_t vl_boot_header;			/*   8K	*/
	vdev_phys_t	vl_vdev_phys;				/* 112K	*/
	char		vl_uberblock[VDEV_UBERBLOCK_RING];	/* 128K	*/
} vdev_label_t;							/* 256K total */

/*
 * vdev_dirty() flags
 */
#define	VDD_METASLAB	0x01
#define	VDD_DTL		0x02

/*
 * Size and offset of embedded boot loader region on each label.
 * The total size of the first two labels plus the boot area is 4MB.
 */
#define	VDEV_BOOT_OFFSET	(2 * sizeof (vdev_label_t))
#define	VDEV_BOOT_SIZE		(7ULL << 19)			/* 3.5M	*/

/*
 * Size of label regions at the start and end of each leaf device.
 */
#define	VDEV_LABEL_START_SIZE	(2 * sizeof (vdev_label_t) + VDEV_BOOT_SIZE)
#define	VDEV_LABEL_END_SIZE	(2 * sizeof (vdev_label_t))
#define	VDEV_LABELS		4

#define	VDEV_ALLOC_LOAD		0
#define	VDEV_ALLOC_ADD		1
#define	VDEV_ALLOC_SPARE	2
#define	VDEV_ALLOC_L2CACHE	3

/*
 * Allocate or free a vdev
 */
extern int vdev_alloc(spa_t *spa, vdev_t **vdp, nvlist_t *config,
    vdev_t *parent, uint_t id, int alloctype);
extern void vdev_free(vdev_t *vd);

/*
 * Add or remove children and parents
 */
extern void vdev_add_child(vdev_t *pvd, vdev_t *cvd);
extern void vdev_remove_child(vdev_t *pvd, vdev_t *cvd);
extern void vdev_compact_children(vdev_t *pvd);
extern vdev_t *vdev_add_parent(vdev_t *cvd, vdev_ops_t *ops);
extern void vdev_remove_parent(vdev_t *cvd);

/*
 * vdev sync load and sync
 */
extern void vdev_load(vdev_t *vd);
extern void vdev_sync(vdev_t *vd, uint64_t txg);
extern void vdev_sync_done(vdev_t *vd, uint64_t txg);
extern void vdev_dirty(vdev_t *vd, int flags, void *arg, uint64_t txg);

/*
 * Available vdev types.
 */
extern vdev_ops_t vdev_root_ops;
extern vdev_ops_t vdev_mirror_ops;
extern vdev_ops_t vdev_replacing_ops;
extern vdev_ops_t vdev_raidz_ops;
#ifdef _KERNEL
extern vdev_ops_t vdev_geom_ops;
#else
extern vdev_ops_t vdev_disk_ops;
#endif
extern vdev_ops_t vdev_file_ops;
extern vdev_ops_t vdev_missing_ops;
extern vdev_ops_t vdev_spare_ops;

/*
 * Common size functions
 */
extern uint64_t vdev_default_asize(vdev_t *vd, uint64_t psize);
extern uint64_t vdev_get_rsize(vdev_t *vd);

/*
 * zdb uses this tunable, so it must be declared here to make lint happy.
 */
extern int zfs_vdev_cache_size;

#ifdef	__cplusplus
}
#endif

#endif	/* _SYS_VDEV_IMPL_H */
