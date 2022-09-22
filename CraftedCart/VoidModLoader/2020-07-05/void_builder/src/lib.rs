#[macro_use]
extern crate error_chain;

#[macro_use]
extern crate log;

pub mod fs;

pub mod error {
    error_chain! {
        foreign_links {
            Io(std::io::Error);
        }
    }
}
