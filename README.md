# GD AI MOD v0.3.0 — Neural Macro Generator UI

Android64 Geode mod for Geometry Dash 2.2081 / Geode 5.11.2.

## v0.3.0 interface
- Keeps the draggable GD bubble and Windows XP-inspired floating panel.
- Adds a macro creation progress bar from 0% to 100%.
- Displays the current macro-generation status and planned stages.
- Progress is initialized at 0% and is not artificially animated. It will advance only when real generation work is connected.

## Not implemented yet
- Neural network inference or training.
- Reading/parsing level objects.
- Physics simulation, pathfinding or AI route generation.
- Actual macro generation, validation or optimization.
- Automated gameplay input.
- Learning from deaths/retries.
- IA and PLAY remain prototype controls, not a working solver or macro player.

## Build
GitHub Actions builds Android64. Open Actions and download the GD-AI-MOD-Android64 artifact from the latest successful run. Extract the ZIP and install gdai.mobile.geode through Geode.

This release is an experimental interface prototype and must be tested on the target device.
