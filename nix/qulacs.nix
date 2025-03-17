{ stdenv, fetchFromGitHub, cmake, boost, pkg-config, eigen }:

stdenv.mkDerivation {
  name = "qulacs";
  src = fetchFromGitHub {
    owner = "qulacs";
    repo = "qulacs";
    rev = "v0.6.11";
    hash = "sha256-w+3Vzo41plWBSratmKfLqZmJ1FsImbvNDj2ZUCnMrd8=";
  };

  nativeBuildInputs = [
    cmake
    boost
    pkg-config
    eigen
  ];
  cmakeFlags = [
    "-DUSE_PYTHON=no"
    "-DUSE_TEST=no"
  ];
  patches = [
    ./0001-patch-out-fetching.patch
    ./0002-install-headers.patch
    ./0003-namespace-the-paths.patch
  ];
}
