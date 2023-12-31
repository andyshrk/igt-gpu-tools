/*
 * Copyright © 2017 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 */

#ifndef __IGT_DEVICE_H__
#define __IGT_DEVICE_H__

int __igt_device_set_master(int fd);
void igt_device_set_master(int fd);

int __igt_device_drop_master(int fd);
void igt_device_drop_master(int fd);

int igt_device_get_card_index(int fd);
struct pci_device *igt_device_get_pci_device(int fd);
struct pci_device *__igt_device_get_pci_device(int fd, unsigned int vf_id);
struct pci_device *igt_device_get_pci_root_port(int fd);

void igt_device_get_pci_slot_name(int fd, char *pci_slot_name);
#endif /* __IGT_DEVICE_H__ */
