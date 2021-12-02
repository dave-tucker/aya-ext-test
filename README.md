# xdp-test

## Prerequisites

1. Install a rust stable toolchain: `rustup install stable`
1. Install a rust nightly toolchain: `rustup install nightly`
1. Install bpf-linker: `cargo install bpf-linker`

## Build eBPF

```bash
cargo xtask build-ebpf
```

To perform a release build you can use the `--release` flag.
You may also change the target architecture with the `--target` flag

## Build Userspace

```bash
cargo build
```

## Run

```bash
# replace a stub from a big program
sudo RUST_LOG=debug ./target/debug/xdp-test -iface wlp2s0
# replace the whole program...
sudo RUST_LOG=debug ./target/debug/xdp-test2 -iface wlp2s0
```
