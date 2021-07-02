# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.1.6] - 2021-06-27

### Added

- Add definition of Listenable

### Changed

- Use Listenable for message transport instead of the custom mechanism to manage listeners

## [0.1.5]

### Changed

- Use Boost.Asio instead of the standalone version

## [0.1.4]

### Added

- Add support for custom serializer ([A000ADMS-860](https://jira.integrasources.com/browse/A000ADMS-860)).

## [0.1.3] - 2021-06-02

### Changed

- Pass compile options (CMAKE_CXX_FLAGS) to the Conan
- Use header-only version of the fmt library

## [0.1.2] - 2021-06-02

### Removed

- Remove predefined compiler/linker flags from the makefile

## [0.1.1] - 2021-05-01

### Added

- New binary serializer (provides serialization support for most standard library containers).
- Support for serialization on platforms with different endianness.

### Changed

- Use executor work guard for the IO context running in the messaging service

[Unreleased]: https://gitlab.integrasources.com/public-projects/a000-isml/compare/v0.1.6...dev
[0.1.6]: https://gitlab.integrasources.com/public-projects/a000-isml/compare/v0.1.5...v0.1.6
[0.1.5]: https://gitlab.integrasources.com/public-projects/a000-isml/compare/v0.1.4...v0.1.5
[0.1.4]: https://gitlab.integrasources.com/public-projects/a000-isml/compare/v0.1.3...v0.1.4
[0.1.3]: https://gitlab.integrasources.com/public-projects/a000-isml/compare/v0.1.2...v0.1.3
[0.1.2]: https://gitlab.integrasources.com/public-projects/a000-isml/compare/v0.1.1...v0.1.2
[0.1.1]: https://gitlab.integrasources.com/public-projects/a000-isml/tree/v0.1.1
