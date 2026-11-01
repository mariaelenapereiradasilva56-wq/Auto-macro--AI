# GD AI MOD v0.1.0 — Android64 source

Target reported: Geometry Dash 2.2081, Geode 5.11.2, Android Launcher 1.9.0, ARM64-v8a.

## Implemented
- Geode project metadata pinned to Geode 5.11.2 and GD Android 2.2081.
- LevelInfoLayer hook adding IA and PLAY controls.
- Frame-based macro event model (press/release).
- PLAY creates a sample timeline to test the data model.

## Not implemented yet
- Full level-object parsing.
- Physics/path simulation and route search.
- Sending inputs to live gameplay.
- Automatic retries/learning or verified support for all modes.

The sample frame events are only a test fixture, not a route for a real level.

## Build
The GitHub Actions workflow builds Android64 using the SDK declared in mod.json:
1. Upload this folder's contents to a GitHub repository.
2. Open Actions and run "Build GD AI MOD for Android".
3. Download artifact "GD-AI-MOD-Android64".
4. Extract the .geode file and install it through Geode.

Official build instructions: https://github.com/geode-sdk/docs/blob/main/getting-started/create-mod.md

This source has not been compiled in this environment. The first Android build must be tested before claiming runtime compatibility.
