/*
 * Rational number handling functions
 * Copyright 2014-2015 Freescale Semiconductor, Inc.
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include <linux/kernel.h>
#include "rational.h"
#include <asm/div64.h>

/**
 * DOC: */

/* rational_reduce() - makes sure p/q < 1
 *
 */
static inline void rational_reduce(struct rational *r)
{
	if (r->p >= r->q) {
		/* Optimize the common case */
		r->i++;
		r->p -= r->q;

		if (r->p >= r->q) {
			unsigned int n = r->p / r->q;

			r->i += n;
			r->p -= n * r->q;
		}
	}
}

/**
 * rational_init() -
 */
void rational_init(struct rational *r, unsigned long long p, unsigned int q)
{
	if (!q) {
		pr_err("%s: 0 denominator (%llu/%u)\n", __func__, p, q);
		q = 1;
	}

	r->p = do_div(p, q);

	if (p > 0xffffffff)
		pr_err("%s: 32bit integer overflow (%llu)\n", __func__, p);

	r->i = p;
	r->q = q;

	rational_reduce(r);
}

/**
 * rational_add() - adds two rational numbers r = r1 + r2
 */
void rational_add(struct rational *r, struct rational *r1, struct rational *r2)
{
	r->i = r1->i + r2->i;

	if (r1->q == r2->q) {
		r->p = r1->p + r2->p;
		r->q = r1->q;
	} else {
		/* Slow path, may overflow */
		r->p = r1->p * r2->q + r2->p * r1->q;
		r->q = r1->q * r2->q;
	}

	rational_reduce(r);
}


/**
 * rational_div() - divides two rational numbers r = r1 / r2
 * Fast but overflow easily.
 */
void rational_div(struct rational *r, struct rational *r1, struct rational *r2)
{
	unsigned int p, q;

	p = (r1->i * r1->q + r1->p) * r2->q;
	q = (r2->i * r2->q + r2->p) * r1->q;

	rational_init(r, p, q);
}

/**
 * rational_cmp() - compares two rational numbers in reduced form
 */
int rational_cmp(struct rational *r1, struct rational *r2)
{
	if (((int)r1->i - (int)r2->i) > 0)
		return 1;
	else if (((int)r1->i - (int)r2->i) < 0)
		return -1;
	else {
		if (r1->q == r2->q) {
			if (r1->p > r2->p)
				return 1;
			else if (r1->p == r2->p)
				return 0;
			else
				return -1;
		} else {
			/* Slow path, may overflow */
			unsigned int r1p = r1->p * r2->q;
			unsigned int r2p = r2->p * r1->q;

			if (r1p > r2p)
				return 1;
			else if (r1p == r2p)
				return 0;
			else
				return -1;
		}
	}
}
