## my silakka54 QMK keymap

![](./silakka54_keyboard.jpeg)

## Setup

Clone vial-qmk

```bash
git clone https://github.com/vial-kb/vial-qmk.git ~/git/vial-qmk
```

Clone the silakka54 keyboard firmware into vial-qmk

```bash
git clone https://github.com/Squalius-cephalus/silakka54.git ~/tmp/silakka54
mv ~/tmp/silakka54/firmware ~/git/vial-qmk/keyboards/silakka54
```

Clone this keymap into the silakka54 keymaps directory

```bash
git clone --recurse-submodules https://github.com/morphykuffour/silakka54-qmk-keymap.git \
  ~/git/vial-qmk/keyboards/silakka54/keymaps/silakka54-qmk-keymap
```

## Compile

### With Nix (recommended)

```bash
cd ~/git/vial-qmk/keyboards/silakka54/keymaps/silakka54-qmk-keymap
nix develop
cd ~/git/vial-qmk
qmk compile -c -kb silakka54 -km silakka54-qmk-keymap
```

### Without Nix

Install the QMK toolchain and arm-none-eabi-gcc, then from the vial-qmk root:

```bash
cd ~/git/vial-qmk
qmk compile -c -kb silakka54 -km silakka54-qmk-keymap
```

## My keymap
![keymap](./silakka54_keymap.svg)
