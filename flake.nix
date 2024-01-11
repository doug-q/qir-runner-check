{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    devenv = {
      url = "github:cachix/devenv";
      inputs = { nixpkgs.follows = "nixpkgs"; };
    };
    flake-parts.url = "github:hercules-ci/flake-parts";
    fenix = {
      url = "github:nix-community/fenix";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };
  outputs = inputs@{ flake-parts, self, ... }:
    flake-parts.lib.mkFlake { inherit inputs; } {
      imports = [ inputs.devenv.flakeModule ];
      systems = [ "x86_64-linux" "aarch64-darwin" ];
      perSystem = { config, self', inputs', pkgs, system, ... }: {
        devenv.shells.default = {pkgs,...}: {
          packages = [ pkgs.openssl.dev pkgs.lldb  ];
          languages.rust = {
            enable = true;
            channel = "stable";
            components =
              [ "rustc" "cargo" "clippy" "rustfmt" "rust-analyzer" ];
          };
          env = {
            CC = "${pkgs.stdenv.cc}/bin/cc";
            CXX = "${pkgs.stdenv.cc}/bin/c++";
          };
        };
      };
    };
}

