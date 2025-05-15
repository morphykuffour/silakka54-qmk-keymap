#!/usr/bin/env bash

qmk c2json ./keymap.c | keymap parse -c 10 -q - > silakka54_keymap.yaml

keymap draw silakka54_keymap.yaml > silakka54_keymap.svg
