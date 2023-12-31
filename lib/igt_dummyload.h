/*
 * Copyright © 2016 Intel Corporation
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

#ifndef __IGT_DUMMYLOAD_H__
#define __IGT_DUMMYLOAD_H__

#include <stdint.h>
#include <time.h>

#include "drmtest.h"
#include "igt_core.h"
#include "igt_list.h"
#include "i915_drm.h"
#include "intel_ctx.h"


/**
 * igt_spin_factory_t:
 * @ctx_id: GEM context handle
 * @ctx: intel_ctx_t context wrapper
 * @dependency: GEM object to depend on
 * @engine: Flags describing the engine to execute on
 * @flags: Set of IGT_SPIN_* flags
 * @fence: In-fence to wait on
 *
 * A factory struct which contains creation parameters for an igt_spin_t.
 */
typedef struct igt_spin_factory {
	uint32_t ctx_id;
	const intel_ctx_t *ctx;
	uint32_t dependency;
	uint64_t dependency_size;
	unsigned int engine;
	unsigned int flags;
	int fence;
	uint64_t ahnd;
	struct drm_xe_engine_class_instance *hwe;
	uint32_t vm;
} igt_spin_factory_t;

typedef struct igt_spin {
	struct igt_list_head link;

	uint32_t handle;
	uint32_t poll_handle;

	uint32_t *batch;

	uint32_t *condition;
	uint32_t cmd_precondition;

	uint32_t *poll;
#define SPIN_POLL_START_IDX 0

	struct timespec last_signal;
	pthread_t timer_thread;
	int timerfd;

	int out_fence;
	struct drm_i915_gem_exec_object2 obj[2];
#define IGT_SPIN_BATCH   1
	struct drm_i915_gem_execbuffer2 execbuf;

	unsigned int flags;
#define SPIN_CLFLUSH (1 << 0)

	struct igt_spin_factory opts;

	struct xe_spin *xe_spin;
	enum intel_driver driver;
	size_t bo_size;
	uint64_t address;
	unsigned int engine;
	uint32_t vm;
	uint32_t syncobj;

} igt_spin_t;


#define IGT_SPIN_FENCE_IN      (1 << 0)
#define IGT_SPIN_FENCE_SUBMIT  (1 << 1)
#define IGT_SPIN_FENCE_OUT     (1 << 2)
#define IGT_SPIN_POLL_RUN      (1 << 3)
#define IGT_SPIN_FAST          (1 << 4)
#define IGT_SPIN_NO_PREEMPTION (1 << 5)
#define IGT_SPIN_INVALID_CS    (1 << 6)
#define IGT_SPIN_USERPTR       (1 << 7)
#define IGT_SPIN_SOFTDEP       (1 << 8)

igt_spin_t *
__igt_spin_factory(int fd, const igt_spin_factory_t *opts);
igt_spin_t *
igt_spin_factory(int fd, const igt_spin_factory_t *opts);

#define __igt_spin_new(fd, ...) \
	__igt_spin_factory(fd, &((igt_spin_factory_t){__VA_ARGS__}))
#define igt_spin_new(fd, ...) \
	igt_spin_factory(fd, &((igt_spin_factory_t){__VA_ARGS__}))

void igt_spin_set_timeout(igt_spin_t *spin, int64_t ns);
void igt_spin_reset(igt_spin_t *spin);
void igt_spin_end(igt_spin_t *spin);
void igt_spin_free(int fd, igt_spin_t *spin);

static inline bool igt_spin_has_poll(const igt_spin_t *spin)
{
	return spin->poll;
}

static inline bool igt_spin_has_started(igt_spin_t *spin)
{
	return READ_ONCE(spin->poll[SPIN_POLL_START_IDX]);
}

static inline void igt_spin_busywait_until_started(igt_spin_t *spin)
{
	while (!igt_spin_has_started(spin))
		;
}

void igt_terminate_spins(void);
void igt_unshare_spins(void);
void igt_free_spins(int i915);

struct intel_execution_engine2;

igt_spin_t *__igt_sync_spin_poll(int i915, uint64_t ahnd,
				 const intel_ctx_t *ctx,
				 const struct intel_execution_engine2 *e);
unsigned long __igt_sync_spin_wait(int i915, igt_spin_t *spin);
igt_spin_t *__igt_sync_spin(int i915, uint64_t ahnd, const intel_ctx_t *ctx,
			    const struct intel_execution_engine2 *e);
igt_spin_t *igt_sync_spin(int i915, uint64_t ahnd, const intel_ctx_t *ctx,
			  const struct intel_execution_engine2 *e);

enum igt_cork_type {
	CORK_SYNC_FD = 1,
	CORK_VGEM_HANDLE
};

struct igt_cork_vgem {
	int device;
	uint32_t fence;
};

struct igt_cork_sw_sync {
	int timeline;
};

struct igt_cork {
	enum igt_cork_type type;

	union {
		int fd;

		struct igt_cork_vgem vgem;
		struct igt_cork_sw_sync sw_sync;
	};
};

#define IGT_CORK(name, cork_type) struct igt_cork name = { .type = cork_type, .fd = -1}
#define IGT_CORK_HANDLE(name) IGT_CORK(name, CORK_VGEM_HANDLE)
#define IGT_CORK_FENCE(name) IGT_CORK(name, CORK_SYNC_FD)

uint32_t igt_cork_plug(struct igt_cork *cork, int fd);
void igt_cork_unplug(struct igt_cork *cork);

#endif /* __IGT_DUMMYLOAD_H__ */
