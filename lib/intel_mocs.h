/* SPDX-License-Identifier: MIT */
/*
 * Copyright © 2022 Intel Corporation
 */

#ifndef _INTEL_MOCS_H
#define _INTEL_MOCS_H

uint8_t intel_get_wb_mocs_index(int fd);
uint8_t intel_get_uc_mocs_index(int fd);

#endif /* _INTEL_MOCS_H */
