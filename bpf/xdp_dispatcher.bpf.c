/* SPDX-License-Identifier: GPL-2.0 */
#include <linux/bpf.h>
#include <linux/in.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_endian.h>

#define XDP_METADATA_SECTION "xdp_metadata"
#define XDP_DISPATCHER_VERSION 1
#define XDP_DISPATCHER_RETVAL 31
#define MAX_DISPATCHER_ACTIONS 10

struct xdp_dispatcher_config {
	__u8 num_progs_enabled;
	__u32 chain_call_actions[MAX_DISPATCHER_ACTIONS];
	__u32 run_prios[MAX_DISPATCHER_ACTIONS];
};
volatile const struct xdp_dispatcher_config conf = {};

__attribute__ ((noinline))
int prog0(struct xdp_md *ctx) {
    volatile int ret = XDP_DISPATCHER_RETVAL;

    if (!ctx)
        return XDP_ABORTED;
    return ret;
}

__attribute__ ((noinline))
int prog1(struct xdp_md *ctx) {
    volatile int ret = XDP_DISPATCHER_RETVAL;

    if (!ctx)
        return XDP_ABORTED;
    return ret;
}

__attribute__ ((noinline))
int prog2(struct xdp_md *ctx) {
    volatile int ret = XDP_DISPATCHER_RETVAL;

    if (!ctx)
        return XDP_ABORTED;
    return ret;
}

__attribute__ ((noinline))
int prog3(struct xdp_md *ctx) {
    volatile int ret = XDP_DISPATCHER_RETVAL;

    if (!ctx)
        return XDP_ABORTED;
    return ret;
}

__attribute__ ((noinline))
int prog4(struct xdp_md *ctx) {
    volatile int ret = XDP_DISPATCHER_RETVAL;

    if (!ctx)
        return XDP_ABORTED;
    return ret;
}

__attribute__ ((noinline))
int prog5(struct xdp_md *ctx) {
    volatile int ret = XDP_DISPATCHER_RETVAL;

    if (!ctx)
        return XDP_ABORTED;
    return ret;
}

__attribute__ ((noinline))
int prog6(struct xdp_md *ctx) {
    volatile int ret = XDP_DISPATCHER_RETVAL;

    if (!ctx)
        return XDP_ABORTED;
    return ret;
}

__attribute__ ((noinline))
int prog7(struct xdp_md *ctx) {
    volatile int ret = XDP_DISPATCHER_RETVAL;

    if (!ctx)
        return XDP_ABORTED;
    return ret;
}

__attribute__ ((noinline))
int prog8(struct xdp_md *ctx) {
    volatile int ret = XDP_DISPATCHER_RETVAL;

    if (!ctx)
        return XDP_ABORTED;
    return ret;
}

__attribute__ ((noinline))
int prog9(struct xdp_md *ctx) {
    volatile int ret = XDP_DISPATCHER_RETVAL;

    if (!ctx)
        return XDP_ABORTED;
    return ret;
}

__attribute__ ((noinline))
int compat_test(struct xdp_md *ctx) {
        volatile int ret = XDP_DISPATCHER_RETVAL;

        if (!ctx)
          return XDP_ABORTED;
        return ret;
}


SEC("xdp/dispatcher")
int xdp_dispatcher(struct xdp_md *ctx)
{
        __u8 num_progs_enabled = conf.num_progs_enabled;
        int ret;

        if (num_progs_enabled < 1)
            goto out;
        ret = prog0(ctx);
        if (!((1U << ret) & conf.chain_call_actions[0]))
            return ret;

        if (num_progs_enabled < 2)
            goto out;
        ret = prog1(ctx);
        if (!((1U << ret) & conf.chain_call_actions[1]))
            return ret;
        
        if (num_progs_enabled < 3)
            goto out;
        ret = prog2(ctx);
        if (!((1U << ret) & conf.chain_call_actions[2]))
            return ret;

        if (num_progs_enabled < 4)
            goto out;
        ret = prog3(ctx);
        if (!((1U << ret) & conf.chain_call_actions[3]))
            return ret;

        if (num_progs_enabled < 5)
            goto out;
        ret = prog4(ctx);
        if (!((1U << ret) & conf.chain_call_actions[4]))
            return ret;
        
        if (num_progs_enabled < 6)
            goto out;
        ret = prog5(ctx);
        if (!((1U << ret) & conf.chain_call_actions[5]))
            return ret;
        
        if (num_progs_enabled < 7)
            goto out;
        ret = prog6(ctx);
        if (!((1U << ret) & conf.chain_call_actions[6]))
            return ret;

        if (num_progs_enabled < 8)
            goto out;
        ret = prog7(ctx);
        if (!((1U << ret) & conf.chain_call_actions[7]))
            return ret;

        if (num_progs_enabled < 9)
            goto out;
        ret = prog8(ctx);
        if (!((1U << ret) & conf.chain_call_actions[8]))
            return ret;

        if (num_progs_enabled < 10)
            goto out;
        ret = prog9(ctx);
        if (!((1U << ret) & conf.chain_call_actions[9]))
            return ret;

        /* keep a reference to the compat_test() function so we can use it
         * as an freplace target in xdp_multiprog__check_compat() in libxdp
         */
        if (num_progs_enabled < 11)
                goto out;
        ret = compat_test(ctx);
out:
        return XDP_PASS;
}

SEC("xdp/pass")
int xdp_pass(struct xdp_md *ctx)
{
        return XDP_PASS;
}

char _license[] SEC("license") = "GPL";
__uint(dispatcher_version, XDP_DISPATCHER_VERSION) SEC(XDP_METADATA_SECTION);
