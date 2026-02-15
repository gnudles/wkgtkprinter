# wkgtkprinter Examples

This directory contains simple examples demonstrating how to use the `wkgtkprinter` library to generate PDF files from HTML.

## Prerequisites

- `libwkgtkprinter.so` must be built in the parent directory.
- `libgtk-3-dev` and `libwebkit2gtk-4.1-dev` (or 4.0) must be installed.

## Building the Examples

Run `make` in this directory to build all examples:

```bash
make
```

## Running the Examples

Make sure to set `LD_LIBRARY_PATH` to the directory containing `libwkgtkprinter.so` (the parent directory).

### Invoice Generator

Generates a PDF invoice from an HTML template (`invoice_template.html`).

```bash
export LD_LIBRARY_PATH=..
./invoice_generator
```
Output: `invoice.pdf`

### Wedding Invitation Generator

Generates a wedding invitation PDF using embedded HTML/CSS. You can optionally provide names and date as arguments.

```bash
export LD_LIBRARY_PATH=..
./invitation_generator "Romeo" "Juliet" "February 14, 2024"
```
Output: `invitation.pdf`

### Card Game Generator

Generates a sheet of playing cards.

```bash
export LD_LIBRARY_PATH=..
./card_game_generator
```
Output: `cards.pdf`

## Note on Headless Environments

If you are running in a headless environment (e.g., a server without a display), you may need to use `xvfb-run`:

```bash
xvfb-run ./invoice_generator
```
