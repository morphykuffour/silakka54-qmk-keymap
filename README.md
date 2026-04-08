## silakka54 QMK keymap

![](./silakka54_keyboard.jpeg)

## Setup

Clone [vial-qmk](https://github.com/vial-kb/vial-qmk):

```bash
git clone https://github.com/vial-kb/vial-qmk.git ~/git/vial-qmk
```

Clone this keymap (with submodules):

```bash
git clone --recurse-submodules https://github.com/morphykuffour/keymap.git \
  ~/git/vial-qmk/keyboards/silakka54/keymaps/keymap
```

## Build

```bash
make build
```

Pulls the toolchain via `nix develop` and runs:

```bash
qmk compile -c -kb silakka54 -km keymap
```

Output: `.build/silakka54_keymap.uf2`

### Interactive shell

```bash
nix develop   # sets QMK_HOME, syncs sm_td submodule
qmk compile -c -kb silakka54 -km keymap
```

### Clean

```bash
make clean
```

## Layers

| Layer | Name | Activated by |
|-------|------|--------------|
| 0 | Colemak-DH | Default / Neovim insert mode |
| 1 | Nav/Fn | Hold `MO(1)` |
| 2 | Symbols | Hold `MO(2)` via sm_td |
| 3 | QWERTY | Neovim normal/visual/command mode |

Layer 0 ↔ 3 switching is automatic via [rawtalk](#vim-mode-layer-switching-rawtalk).
Manual overrides: `FORCE_QWERTY` (layer 1, right-outer thumb) and `FORCE_COLEMAK` (layer 3, left-outer thumb).

## Vim-mode layer switching (rawtalk)

Neovim automatically switches the keyboard between Colemak (insert mode) and QWERTY (normal/visual/command mode) via [rawtalk](https://github.com/morphykuffour/rawtalk), a Rust daemon that bridges Neovim and the keyboard over Raw HID.

```
Neovim ModeChanged → Unix socket (/tmp/rawtalk.sock) → rawtalk → Raw HID → keyboard
```

The keyboard's VID/PID (`0xFEED`/`0x1212`) match rawtalk's hardcoded constants — no configuration needed.

### Raw HID protocol

| `data[0]` | `data[1]` | Action |
|-----------|-----------|--------|
| `0x00` | layer (0–3) | Switch default layer, ack `data[2]=0xAA` |
| `0x40` | — | Return current layer in `data[0]` |
| other | — | `0xFF` unhandled, passed to VIA/VIAL |

### Running rawtalk

```bash
cd ~/git/rawtalk
cargo build --release
./target/release/rawtalk
```

### Neovim integration

Add to `init.lua`:

```lua
local socket_path       = "/tmp/rawtalk.sock"
local uv                = vim.uv or vim.loop
local rawtalk_client    = nil
local rawtalk_connected = false

local function rawtalk_connect()
    if rawtalk_connected then return true end
    rawtalk_client = uv.new_pipe(false)
    rawtalk_client:connect(socket_path, function(err)
        rawtalk_connected = not err
    end)
    vim.wait(50, function() return rawtalk_connected end, 10)
    return rawtalk_connected
end

local function rawtalk_send_mode(mode)
    if not rawtalk_connected then rawtalk_connect() end
    if rawtalk_connected and rawtalk_client then
        pcall(function() rawtalk_client:write(mode .. "\n") end)
    end
end

local rawtalk_group = vim.api.nvim_create_augroup("Rawtalk", { clear = true })

vim.api.nvim_create_autocmd("ModeChanged", {
    group    = rawtalk_group,
    pattern  = "*",
    callback = function() rawtalk_send_mode(vim.fn.mode()) end,
})

vim.api.nvim_create_autocmd("VimEnter", {
    group    = rawtalk_group,
    callback = function()
        vim.defer_fn(function()
            rawtalk_connect()
            rawtalk_send_mode(vim.fn.mode())
        end, 100)
    end,
})

vim.api.nvim_create_autocmd("VimLeave", {
    group    = rawtalk_group,
    callback = function()
        if rawtalk_client then pcall(function() rawtalk_client:close() end) end
    end,
})
```

## Keymap

![keymap](./silakka54_keymap.svg)
