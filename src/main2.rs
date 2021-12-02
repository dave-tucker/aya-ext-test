use aya::programs::{Extension, Link, ProgramFd, Xdp, XdpFlags};
use aya::{include_bytes_aligned, BpfLoader};

use log::info;

use std::thread::sleep;
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

fn try_main() -> Result<(), anyhow::Error> {
    let opt = Opt::from_args();
    info!("Loading xdp_pass");

    let mut bpf = BpfLoader::new().load(include_bytes_aligned!("../bpf/.output/xdp_pass.bpf.o"))?;
    let pass: &mut Xdp = bpf.program_mut("pass").unwrap().try_into()?;
    pass.load()?;
    pass.attach(&opt.iface, XdpFlags::default())?;

    info!("Loading xdp_drop as extension");
    let mut bpf = BpfLoader::new().load(include_bytes_aligned!("../bpf/.output/xdp_drop.bpf.o"))?;
    let drop_: &mut Extension = bpf.program_mut("drop").unwrap().try_into()?;
    drop_.load(pass.fd().unwrap(), "xdp_pass")?;
    let mut drop_link = drop_.attach()?;

    sleep(Duration::from_secs(10));
    info!("detaching xdp_drop");
    drop_link.detach()?;

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
