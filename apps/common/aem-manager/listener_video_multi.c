/*
 * Copyright 2014-2016 Freescale Semiconductor, Inc.
 * Copyright 2016-2018, 2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 @brief Listener entity
 @details Listener AVDECC entity definition with four input streams
*/

#include "genavb/adp.h"
#include "genavb/aem_helpers.h"

#include "listener_video_multi.h"

AEM_ENTITY_STORAGE();

void listener_video_multi_init(struct aem_desc_hdr *aem_desc)
{
	AEM_ENTITY_INIT(aem_desc);
}