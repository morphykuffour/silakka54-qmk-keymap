{
  description = "silakka54 QMK keymap build environment";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

  outputs = { self, nixpkgs }:
    let
      systems = [ "x86_64-linux" "aarch64-linux" "x86_64-darwin" "aarch64-darwin" ];
      forAllSystems = f: nixpkgs.lib.genAttrs systems (system: f system);
    in {
      devShells = forAllSystems (system:
        let pkgs = nixpkgs.legacyPackages.${system}; in {
          default = pkgs.mkShell {
            packages = with pkgs; [
              qmk
              gcc-arm-embedded
              dfu-util
              git
            ];
            shellHook = ''
              # Sync sm_td submodule
              git submodule update --init --recursive

              # This keymap repo lives at keyboards/<kb>/keymaps/ inside vial-qmk
              export QMK_HOME=$(cd "$PWD/../../.." && pwd)
              echo "QMK_HOME=$QMK_HOME"
              echo "Run 'make build' or: qmk compile -c -kb silakka54 -km keymap"
            '';
          };
        });
    };
}
