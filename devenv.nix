{ pkgs, lib, config, inputs, ... }:
let
  qulacs = pkgs.callPackage ./nix/qulacs.nix { };
in
{
  env = {
    QULACS_PATH = qulacs;
    LIBCLANG_PATH = "${pkgs.libclang.lib}/lib";
  };

  packages = [
    pkgs.boost
    pkgs.eigen
    pkgs.mpi
    qulacs

    pkgs.release-plz
    pkgs.cargo-semver-checks
  ];

  languages.rust.enable = true;
  languages.nix.enable = true;
}
