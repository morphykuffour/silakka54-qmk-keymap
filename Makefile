KEYBOARD := silakka54
KEYMAP   := keymap
QMK_HOME := $(shell cd $(CURDIR)/../../.. && pwd)
SVG      := assets/silakka54_keymap.svg
YAML     := assets/silakka54_keymap.yaml

.PHONY: build svg clean

build:
	nix develop --command bash -c \
	  "QMK_HOME=$(QMK_HOME) qmk compile -c -kb $(KEYBOARD) -km $(KEYMAP)"

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
