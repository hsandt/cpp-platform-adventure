# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]
### Added
- App: Added WindowConfig, which can be set with _config/window.yml_

## [0.0.4] - 2020-07-15
### Added
- Visual: Draw grass with camera moving vertically

### Changed
- Application: Do not allow window resize, size set to 1280x720
- Build: Premake inserts "_action_" (generator) name in build folder
- VS Code: Fixed launch executable path, added quick make (g++) to not check deps

## [0.0.3] - 2020-07-09
### Changed
- Travis Linux: removed chmod +x, counting on natural permissions before archiving Game_Linux

## [0.0.2] - 2020-07-09
### Changed
- Travis Linux: zip Game_Linux into Game_Linux.zip to preserve execution permissions

## [0.0.1] - 2020-07-09
### Added
- Initial project structure with README, LICENSE, CHANGELOG
- Engine third-party SFML as submodule and build_sfml script
- Basic main.cpp to demonstrate SFML window working
- Scripts to build game and cleanup
- .vscode folder for tasks, launch, general and C++ settings
- YAML for Travis (working) and AppVeyor (experimental)

[Unreleased]: https://github.com/hsandt/cpp-platform-adventure/compare/v0.0.4...HEAD
[0.0.4]: https://github.com/hsandt/cpp-platform-adventure/compare/v0.0.3...v0.0.4
[0.0.3]: https://github.com/hsandt/cpp-platform-adventure/compare/v0.0.2...v0.0.3
[0.0.2]: https://github.com/hsandt/cpp-platform-adventure/compare/v0.0.1...v0.0.2
[0.0.1]: https://github.com/hsandt/cpp-platform-adventure/releases/tag/v0.0.1
