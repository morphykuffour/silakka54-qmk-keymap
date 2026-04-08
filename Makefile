KEYBOARD := silakka54
KEYMAP   := keymap
QMK_HOME := $(shell cd $(CURDIR)/../../.. && pwd)
SVG      := assets/silakka54_keymap.svg
YAML     := assets/silakka54_keymap.yaml

.PHONY: build svg flash clean

build:
	nix develop --command bash -c \
	  "QMK_HOME=$(QMK_HOME) qmk compile -c -kb $(KEYBOARD) -km $(KEYMAP)"

UF2   := $(QMK_HOME)/.build/$(KEYBOARD)_$(KEYMAP).uf2
MOUNT := /Volumes/RPI-RP2

flash: build
	@test -f "$(UF2)" || (echo "ERROR: $(UF2) not found — build failed?"; exit 1)
	@echo "Double-tap reset on the silakka54..."
	@until [ -d "$(MOUNT)" ]; do sleep 0.5; done
	@echo "$(MOUNT) detected — copying firmware..."
	cp "$(UF2)" "$(MOUNT)/"
	@echo "Done. Keyboard will reboot automatically."

svg:
	nix develop --command bash -c \
	  "QMK_HOME=$(QMK_HOME) qmk c2json --no-cpp -kb $(KEYBOARD) -km $(KEYMAP) $(KEYMAP)/keymap.c \
	   | keymap parse -c 10 -q - > $(YAML) && keymap draw $(YAML) > $(SVG)"

clean:
	nix develop --command bash -c \
	  "QMK_HOME=$(QMK_HOME) qmk compile -kb $(KEYBOARD) -km $(KEYMAP) -- clean" 2>/dev/null; \
	rm -rf $(QMK_HOME)/.build/obj_$(KEYBOARD)_$(KEYMAP) \
	       $(QMK_HOME)/.build/$(KEYBOARD)_$(KEYMAP).elf \
	       $(QMK_HOME)/.build/$(KEYBOARD)_$(KEYMAP).uf2 \
	       $(QMK_HOME)/.build/$(KEYBOARD)_$(KEYMAP).map
