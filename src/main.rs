use aya::programs::{Extension, ProgramFd, Xdp, XdpFlags};
use aya::Pod;
use aya::{include_bytes_aligned, BpfLoader};

use log::info;

use std::{
    convert::TryInto,
    sync::atomic::{AtomicBool, Ordering},
    sync::Arc,
    thread,
    time::Duration,
};
use structopt::StructOpt;

fn main() {
    env_logger::init();
    if let Err(e) = try_main() {
        eprintln!("error: {:#}", e);
    }
}

#[derive(Debug, StructOpt)]
struct Opt {
    #[structopt(short, long, default_value = "eth0")]
    iface: String,
}

#[repr(C)]
#[derive(Copy, Clone)]
pub struct XdpDispatcherConfig {
    pub num_progs_enabled: u8,
    pub chain_call_actions: [u32; 10],
    pub run_prios: [u32; 10],
}

unsafe impl Pod for XdpDispatcherConfig {}

fn try_main() -> Result<(), anyhow::Error> {
    let opt = Opt::from_args();
    info!("Loading xdp_dispatcher");

    let dispatcher_conf = XdpDispatcherConfig {
        num_progs_enabled: 1,
        chain_call_actions: [1, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        run_prios: [1, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    };

    let mut bpf =
        BpfLoader::new()
            .set_global("conf", &dispatcher_conf)
            .load(include_bytes_aligned!(
                "../bpf/.output/xdp_dispatcher.bpf.o"
            ))?;
    let dispatcher: &mut Xdp = bpf.program_mut("dispatcher").unwrap().try_into()?;
    dispatcher.load()?;
    dispatcher.attach(&opt.iface, XdpFlags::default())?;

    info!("Loading xdp_pass as extension");
    let mut bpf = BpfLoader::new().extension("pass").load(include_bytes_aligned!("../bpf/.output/xdp_pass.bpf.o"))?;
    let pass: &mut Extension = bpf.program_mut("pass").unwrap().try_into()?;
    pass.load(dispatcher.fd().unwrap(), "prog0")?;
    pass.attach()?;

    let running = Arc::new(AtomicBool::new(true));
    let r = running.clone();

    ctrlc::set_handler(move || {
        r.store(false, Ordering::SeqCst);
    })
    .expect("Error setting Ctrl-C handler");

    println!("Waiting for Ctrl-C...");
    while running.load(Ordering::SeqCst) {
        thread::sleep(Duration::from_millis(500))
    }
    println!("Exiting...");

    Ok(())
}
