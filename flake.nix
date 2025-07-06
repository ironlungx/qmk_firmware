{
  description = "Flake for building QMK firmware using Nix";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
        keyboard = "moonlander";
        keymap = "ryan";
        target = "${keyboard}:${keymap}";
      in
      {
        packages.default = pkgs.stdenv.mkDerivation {
          name = "qmk-${keyboard}-${keymap}";
          src = ./.;
          phases = [ "buildPhase" ];
          buildInputs = [ pkgs.qmk ];
          buildPhase = ''
            make -C $src BUILD_DIR=`pwd`/.build COPY=echo -j8 ${target}
            mkdir -p $out
            cp -r .build/* $out/
          '';
        };

        # Dev shell: nix develop
        devShell = pkgs.mkShell {
          buildInputs = [ pkgs.qmk ];
          shellHook = ''
            export KEYBOARD=${keyboard}
            export KEYMAP=${keymap}
            build() {
              local dir=''${1:-.build}
              make -C . BUILD_DIR=$dir COPY=echo -j8 ${target}
            }
            flash() {
              local dir=''${1:-.build}
              make -C . BUILD_DIR=$dir COPY=echo -j8 ${target}:flash
            }
            clean() {
              rm -rf .build
            }
            echo "Welcome to the QMK dev shell for ${target}"
            echo "Use: build [dir], flash [dir], clean"
          '';
        };
      }
    );
}

