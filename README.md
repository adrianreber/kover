# Kover

Kover is an easy to use WYSIWYG CD cover printer with CDDB support.

## Features

- WYSIWYG (What You See Is What You Get) CD cover design
- CDDB/freedb support for automatic track listing retrieval
- CD-TEXT reading support
- Customizable fonts and colors for title and contents
- Image embedding support (center, tile, or stretch modes)
- Support for standard jewel cases and slim cases
- Booklet and inlet printing
- Recent files history

## Requirements

### Build Dependencies

- CMake >= 3.16
- C++20 compatible compiler
- Qt 6.4.0 or later
- KDE Frameworks 6.0.0 or later
- Extra CMake Modules (ECM)
- libcdio >= 0.90
- libcddb

### Runtime Dependencies

- Qt 6 (Core, Gui, Widgets, PrintSupport)
- KDE Frameworks 6 (CoreAddons, I18n, XmlGui, KIO, ConfigWidgets, IconThemes)

## Building

```bash
mkdir build
cd build
cmake ..
make
```

## Installation

```bash
sudo make install
```

## Usage

```bash
kover [options] [file.kover]
```

### Command Line Options

- `--help` - Display help message
- `--debug` - Enable debug output
- `--advise` - Get some helpful advice

## File Format

Kover saves cover designs in `.kover` files (XML format). It can also import K3b data project files.

## License

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

## Authors

- Denis Oliver Kropp (original author, 1998-2000)
- Adrian Reber (maintainer, 2000-2025)

## Links

- Repository: https://github.com/adrianreber/kover
