/* $FreeBSD$ */
/*	$OpenBSD: ieee80211_amrr.h,v 1.3 2006/06/17 19:34:31 damien Exp $	*/

/*-
 * Copyright (c) 2006
 *	Damien Bergamini <damien.bergamini@free.fr>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#ifndef _NET80211_IEEE80211_AMRR_H_
#define _NET80211_IEEE80211_AMRR_H_

/*-
 * Naive implementation of the Adaptive Multi Rate Retry algorithm:
 *
 * "IEEE 802.11 Rate Adaptation: A Practical Approach"
 *  Mathieu Lacage, Hossein Manshaei, Thierry Turletti
 *  INRIA Sophia - Projet Planete
 *  http://www-sop.inria.fr/rapports/sophia/RR-5208.html
 */

/*
 * Rate control settings.
 */
struct ieee80211vap;

struct ieee80211_amrr {
	u_int	amrr_min_success_threshold;
	u_int	amrr_max_success_threshold;
	int	amrr_interval;		/* update interval (ticks) */
};

#define IEEE80211_AMRR_MIN_SUCCESS_THRESHOLD	 1
#define IEEE80211_AMRR_MAX_SUCCESS_THRESHOLD	15

/*
 * Rate control state for a given node.
 */
struct ieee80211_amrr_node {
	struct ieee80211_amrr *amn_amrr;/* backpointer */
	int	amn_rix;		/* current rate index */
	int	amn_ticks;		/* time of last update */
	/* statistics */
	u_int	amn_txcnt;
	u_int	amn_success;
	u_int	amn_success_threshold;
	u_int	amn_recovery;
	u_int	amn_retrycnt;
};

void	ieee80211_amrr_init(struct ieee80211_amrr *, struct ieee80211vap *,
	    int, int, int);
void	ieee80211_amrr_cleanup(struct ieee80211_amrr *);
void	ieee80211_amrr_setinterval(struct ieee80211_amrr *, int);
void	ieee80211_amrr_node_init(struct ieee80211_amrr *,
	    struct ieee80211_amrr_node *, struct ieee80211_node *);
int	ieee80211_amrr_choose(struct ieee80211_node *,
	    struct ieee80211_amrr_node *);

#define	IEEE80211_AMRR_SUCCESS	1
#define	IEEE80211_AMRR_FAILURE	0

/*
 * Update statistics with tx complete status.  Ok is non-zero
 * if the packet is known to be ACK'd.  Retries has the number
 * retransmissions (i.e. xmit attempts - 1).
 */
static __inline void
ieee80211_amrr_tx_complete(struct ieee80211_amrr_node *amn,
    int ok, int retries)
{
	amn->amn_txcnt++;
	if (ok)
		amn->amn_success++;
	amn->amn_retrycnt += retries;
}

/*
 * Set tx count/retry statistics explicitly.  Intended for
 * drivers that poll the device for statistics maintained
 * in the device.
 */
static __inline void
ieee80211_amrr_tx_update(struct ieee80211_amrr_node *amn,
    int txcnt, int success, int retrycnt)
{
	amn->amn_txcnt = txcnt;
	amn->amn_success = success;
	amn->amn_retrycnt = retrycnt;
}
#endif /* _NET80211_IEEE80211_AMRR_H_ */
