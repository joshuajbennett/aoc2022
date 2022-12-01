{ lib
, llvmPackages_11
, cmake
, protobuf
, grpc
, openssl
, c-ares
, pkg-config
}:

llvmPackages_11.stdenv.mkDerivation rec {
  pname = "aoc2022";
  version = "0.1.0";

  src = ./.;

  nativeBuildInputs = [ cmake openssl pkg-config ];
  buildInputs = [ protobuf grpc c-ares ];

  cmakeFlags = [
    "-DENABLE_TESTING=OFF"
    "-DENABLE_INSTALL=ON"
  ];

  meta = with lib; {
    description = ''
      A C++ library for optional time control, allowing applications to run faster than real-time.";
    '';
    licencse = licenses.mit;
    platforms = with platforms; linux ++ darwin;
  };
}
