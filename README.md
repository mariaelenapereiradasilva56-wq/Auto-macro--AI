# GD AI MOD v0.4.0 — Macro Recorder / Player prototype

Android64 Geode mod for Geometry Dash 2.2081 / Geode 5.11.2.

## Current features
- Draggable GD bubble and Windows XP-inspired floating panel.
- REC starts/stops recording player 1 button presses and releases.
- Recording events are tagged with the PlayLayer update frame.
- Stopping REC saves the latest macro to the mod's Geode save directory as `last_macro.csv`.
- PLAY loads the saved macro if needed and replays its input events through PlayerObject.
- IA is reserved for the future level-analysis/solver module.

## How to test
1. Enter a level and open the GD AI panel.
2. Tap REC, play a short and simple section, then tap REC again to stop and save.
3. Restart the same level or return to the beginning of the attempt.
4. Tap PLAY to replay the captured inputs.
5. Test first in a safe practice session and report any timing differences or crashes.

## Important limitations
- This is the first recorder/player prototype and has not yet been validated on the target Android device.
- It currently records player 1 only.
- Playback timing is based on PlayLayer update frames; frame pacing, death/reset behavior, dual mode and game speed changes need further testing.
- It does not solve a level by itself. Neural network inference/training, level parsing, physics simulation, route planning, autonomous macro generation, and learning from deaths are not implemented.
- Do not submit macro-assisted runs as legitimate records.

## Build
GitHub Actions builds Android64. Download the `GD-AI-MOD-Android64` artifact from the latest successful run, extract the ZIP, and install the `.geode` package through Geode.
