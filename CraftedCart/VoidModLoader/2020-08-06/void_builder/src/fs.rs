use crate::error::*;
use std::fs;
use std::path::{Path, PathBuf};

pub fn copy_recursive<P, Q>(from: P, to: Q) -> Result<()>
where
    P: AsRef<Path>,
    Q: AsRef<Path>,
{
    copy_recursive_intern(from, to, true)
}

fn copy_recursive_intern<P, Q>(from: P, to: Q, is_root: bool) -> Result<()>
where
    P: AsRef<Path>,
    Q: AsRef<Path>,
{
    let from = from.as_ref();
    let to = to.as_ref();

    let name = from.file_name().unwrap();

    if from.is_dir() {
        let to_subdir = if is_root {
            PathBuf::from(to)
        } else {
            to.join(name)
        };

        debug!("Creating dir {}", &to_subdir.display());
        fs::create_dir(&to_subdir)?;

        for entry in fs::read_dir(from)? {
            let entry = entry?;
            let path = entry.path();
            copy_recursive_intern(&path, &to_subdir, false)?;
        }
    } else if from.is_file() {
        debug!("Copying {} -> {}", from.display(), to.display());
        fs::copy(from, to.join(name))?;
    }

    Ok(())
}
