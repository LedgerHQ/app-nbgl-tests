# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.1.8] - 2026-09-25

### Added

- Tests for `nbgl_useCaseAdvancedReviewStreamingStart`, with and without a Web3 Checks threat

### Changed

- Python linting and formatting migrated to `ruff`, and `clang-format` applied
- Linter, formatter, VSCode and workflow configurations aligned

### Fixed

- Flaky spinner test in the CI

## [1.1.7] - 2025-11-20

### Added

- Tests for `nbgl_useCaseStaticReviewLight`, `nbgl_useCaseAction` and `nbgl_useCaseChoiceWithDetails`
- Test for a review with multiple warnings

### Changed

- Keypad and its tests enabled on Nano devices
- Tests previously skipped on Nano re-enabled
- App aligned with the linters and the reusable workflows

## [1.1.6] - 2025-09-18

### Added

- Apex P support

### Changed

- Icons replaced with the SDK ones
- Usage of `nbgl_useCaseKeypadXXX` updated to the current SDK API
- Useless `HAVE_NBGL` flag removed
- CI moved to the new reusable workflows

## [1.1.5] - 2025-08-07

### Changed

- Demo flows use tag/value pairs instead of the tipbox

## [1.1.4] - 2025-06-03

### Added

- Demo flow `ui_display_review_with_warning` and its test

### Changed

- Home screen icon converted dynamically at compilation time
- Strings adapted to Nano devices
- Polygon icon updated for Nano

### Removed

- Old Nano S 16px icons

## [1.1.3] - 2025-04-24

### Added

- APDU to play and test sounds

### Changed

- Linters and workflows updated

## [1.1.2] - 2025-03-31

### Changed

- `nbgl_test` variant selected by default
- 1inch demo test re-enabled

## [1.1.1] - 2025-02-24

### Added

- Tests for other NBGL APIs: light review, confirm, generic config/review/settings, keypad and navigation

### Changed

- NBGL used on Nano devices
- Keypad disabled on Nano devices until API_LEVEL 23

### Fixed

- `nbgl_useCaseAdvancedReview` usage

## [1.1.0] - 2025-01-06

### Added

- Demo variant with its demo flows
- New tests: review, blind-signing review, streaming review, address review, spinner and static review
- Dynamic check of AppName and AppVersion in the tests
- Repository dispatch in the CI workflow

### Changed

- App cleaned up
- Linter configuration and VSCode configuration updated

### Removed

- Unused `pysha3` dependency

## [1.0.0] - 2024-09-02

### Added

- Initial commit with the brand new NBGL Tests application
