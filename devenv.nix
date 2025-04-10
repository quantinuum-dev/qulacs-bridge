{ pkgs, lib, config, inputs, ... }:
let
  qulacs = pkgs.callPackage ./nix/qulacs.nix {};
in {
  env = {
    LIBQULACS_PATH = "${qulacs}/lib";
    LIBCLANG_PATH = "${pkgs.libclang.lib}/lib";
  };
  
  packages = [ pkgs.pkg-config pkgs.boost pkgs.eigen pkgs.mpi qulacs pkgs.act ];
  languages.rust.enable = true;
  languages.nix.enable = true;
}
