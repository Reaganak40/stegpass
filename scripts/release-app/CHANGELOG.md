# Changelog

All notable changes to StegPass will be documented in this file.

## [Unreleased]

### Added

- `Get Password` to the gui to make it MUCH easier to retreive passwords from images
- This CHANGELOG to more easily document changes between versions
- StegPass icon is now present in the application

### Fixed

- A formatting bug in the `Add Password` form that moves the button off the screen.

## [0.0.1-alpha] - 6/12/2024

### Added

- stegpass.exe `gui`: the main executable to run the application. This will launch the main GUI used to create new accounts and passwords.
- stegpass.exe `cli`: when this is run in the command line with the arguments `--password <path_to_file>` it will try to extract a password from the image file.
- utilities: standalone executables that work under the hood with StegPass to handle steganography, encryption, and other security measures.