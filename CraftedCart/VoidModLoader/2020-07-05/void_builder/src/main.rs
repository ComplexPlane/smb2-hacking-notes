use env_logger::Env;
use std::fs::OpenOptions;
use std::io::prelude::*;
use std::io::{Seek, SeekFrom};
use std::path::Path;
use void_builder::error::*;

// #[macro_use]
// extern crate error_chain;

#[macro_use]
extern crate log;

fn main() {
    env_logger::from_env(Env::default().default_filter_or("debug")).init();

    // Error handling
    if let Err(ref e) = run() {
        error!("Error: {}", e);

        for e in e.iter().skip(1) {
            error!("Caused by: {}", e);
        }

        if let Some(backtrace) = e.backtrace() {
            error!("Backtrace:\n{:?}", backtrace);
        } else {
            error!("No backtrace available - run with `RUST_BACKTRACE=1` to enable backtraces on error");
        }

        error!("Goodbye");
        std::process::exit(1);
    }
}

fn run() -> Result<()> {
    let cwd = std::env::current_dir().chain_err(|| "Failed to determine current working directory")?;
    info!("Running in {}", cwd.display());

    maybe_make_workspace().chain_err(|| "Failed to make workspace")?;
    patch_main_dol().chain_err(|| "Failed to patch main.dol")?;

    Ok(())
}

fn maybe_make_workspace() -> Result<()> {
    let workspace_exists = Path::new("workspace_root").exists();

    if workspace_exists {
        info!("workspace_root exists, not copying clean_root to workspace_root");
    } else {
        info!("Copying clean_root to workspace_root");
        void_builder::fs::copy_recursive("clean_root", "workspace_root")?;
    }

    Ok(())
}

fn patch_main_dol() -> Result<()> {
    info!("Patching main.dol to use //vml/vml_init.rel");

    let mut file = OpenOptions::new()
        .read(true)
        .write(true)
        .create(false)
        .open("workspace_root/sys/main.dol")?;

    // Replace `//mkb2.main_loop.rel` with `//void/main_loop.rel`
    file.seek(SeekFrom::Start(0x0007E640))?;
    file.write_all(b"//vml/vml_init.rel\0")?;

    Ok(())
}
