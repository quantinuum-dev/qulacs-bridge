use std::env;
use std::path::PathBuf;

fn main() {
    let qulacs_path = PathBuf::from(env::var("QULACS_PATH").unwrap());
    let mut qulacs_lib = qulacs_path.clone();
    qulacs_lib.push("lib");

    let mut qulacs_include = qulacs_path.clone();
    qulacs_include.push("include");
    println!(
        "cargo:rustc-link-search={}",
        qulacs_lib
            .canonicalize()
            .expect("cannot cannonicalize path")
            .to_string_lossy()
    );

    println!("cargo:rustc-link-lib=cppsim_static");
    println!("cargo:rustc-link-lib=csim_static");
    println!("cargo:rustc-link-arg=-fopenmp");

    let eigen = pkg_config::probe_library("eigen3").unwrap();
    cxx_build::bridge("src/lib.rs")
        .file("src/qulacs-bridge.cc")
        .include(qulacs_include)
        .includes(eigen.include_paths)
        .flag("-fext-numeric-literals")
        .flag("-fopenmp") // todo: better way to do this?
        .std("c++14")
        .compile("qulacs-bridge");

    println!("cargo:rerun-if-changed=src/lib.rs");
    println!("cargo:rerun-if-changed=src/qulacs-bridge.cc");
    println!("cargo:rerun-if-changed=include/qulacs-bridge.h");
}
