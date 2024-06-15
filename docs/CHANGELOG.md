# Changelog

All notable changes to StegPass will be documented in this file.

## [0.0.2] - 6/15/2024

### Added

- `Get Password` to the gui to make it MUCH easier to retreive passwords from images.
- Active user cache to simplify the get/add password process, removing the need for repeated logins.
- This CHANGELOG to more easily document changes between versions.
- StegPass icon is now present in the application.
- Backwards compatibility format for byte streams.

### Changed

- Complete change in the GUIs handling of the title bar and menu bar (created from scratch)
- Users cannot access parts of the app until an account is created.
- The UI layout for creating passwords.
- The UI layout for the login screen

### Fixed

- A formatting bug in the `Add Password` form that moves the button off the screen.
- A bug where saving passwords fails when not saving to password folder.
- A bug where toast notifier crashes when getting multiple passwords in quick succession.
- An input bug where using specific control characters in password breaks the command line parsing code.
- An OS error when saving passwords to the same file as the image source.

## [0.0.1-alpha] - 6/12/2024

### Added

- stegpass.exe `gui`: the main executable to run the application. This will launch the main GUI used to create new accounts and passwords.
- stegpass.exe `cli`: when this is run in the command line with the arguments `--password <path_to_file>` it will try to extract a password from the image file.
- utilities: standalone executables that work under the hood with StegPass to handle steganography, encryption, and other security measures.