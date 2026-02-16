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

Generates a PDF invoice from an HTML template (`invoice_template.html`). It supports customization via command-line arguments.

```bash
export LD_LIBRARY_PATH=..
./invoice_generator --customer "Jane Smith" --date "2024-02-14" --number "INV-999" --total "$500.00" --items "<tr><td>Consulting</td><td>2</td><td>$250.00</td><td>$500.00</td></tr>"
```

Options:
- `-c, --customer`: Customer Name
- `-d, --date`: Invoice Date
- `-n, --number`: Invoice Number
- `-t, --total`: Total Amount
- `-i, --items`: Items Rows (HTML `<tr>...</tr>`)
- `-o, --output`: Output PDF file (default: invoice.pdf)
- `-T, --template`: Template HTML file (default: invoice_template.html)

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
