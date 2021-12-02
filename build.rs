use std::process::Command;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    Command::new("make").current_dir("bpf").spawn()?;
    Ok(())
}
