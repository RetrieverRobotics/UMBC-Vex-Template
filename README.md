# UMBC VEX Template

A VEX V5 robot project template with reusable robot support in the `umbc` C++ namespace. The template provides a `Robot` framework and supporting classes for controller input, LCD menus, configuration, file navigation, and recorded autonomous routines.

## Getting started

1. Open the project in a PROS-enabled VS Code environment with the V5 toolchain installed.
2. Implement your robot-specific initialization, driver control, and autonomous routines in the files under `src/`.
3. Build and upload from the project root:

   ```sh
   pros make
   pros upload
   ```

The template targets a VEX V5 Brain. Robot hardware configuration and behavior are intentionally left to the team.

## Project layout

```text
include/
  umbc.h                 Umbrella header for the UMBC framework
  umbc/                  UMBC class declarations
src/
  main.cpp               Robot lifecycle and mode selection entry points
  initialize.cpp         User initialization
  opcontrol.cpp          User driver-control loop
  autonomous.cpp         User competition and skills routines
  umbc/                  UMBC class implementations
```

Include `umbc.h` to use the framework. Add team-specific code to the user implementation files rather than editing the bundled framework unless you intend to change its behavior.

## Implement your robot

The main customization points are methods on `umbc::Robot`:

- `initialize()` in `src/initialize.cpp` — initialize motors, sensors, and other robot hardware.
- `opcontrol()` in `src/opcontrol.cpp` — implement the driver-control loop. The template exposes `controller_master` and `controller_partner` as `umbc::Controller*` pointers, so the same control code can read physical or virtual controller input.
- `autonomous_competition()` and `autonomous_skills()` in `src/autonomous.cpp` — implement the programmed routines for each match type.

`src/main.cpp` contains the V5 lifecycle entry points and invokes the `Robot` framework. Set `PARTNER_CONTROLLER` there to `1` when training or replaying routines that use a partner controller; it defaults to `0`.

## UMBC framework

| Class | Purpose |
| --- | --- |
| `umbc::Robot` | Tracks match type, alliance, and robot mode; coordinates driver control, programmed or recorded autonomous routines, and persistent selections. |
| `umbc::Controller` | Common interface used by physical (`PController`) and virtual (`VController`) controllers. |
| `umbc::ControllerRecorder` and `umbc::ControllerInput` | Capture controller states at a configured polling interval and save them for later playback. |
| `umbc::VController` | Replays controller input from a recording through the same interface used for a physical controller. |
| `umbc::Menu` and `umbc::LCD` | Provide the on-brain selection menus and helpers for the V5 Brain LCD. |
| `umbc::Json` | Parses and writes JSON while preserving unrelated values in the document. |
| `umbc::FileExplorer` | Lists files and navigates directories on the robot's SD card. |

Individual declarations are in `include/umbc/`; `include/umbc.h` includes the framework headers.

## Using the LCD menu

The menu appears on the Brain's LCD during competition initialization. Use the three buttons below the screen to choose the displayed options: the left button selects option 1, the center button selects option 2, and the right button selects option 3. A `Back` option returns to the previous menu.

The initial flow is:

1. Select **Tournament**, **Skills**, or **Utilities**.
2. For a match, select **Competition** or **Train Autonomous** mode.
3. For tournament matches, select **Red** or **Blue** alliance. Skills matches skip alliance selection.

When the robot is in Competition mode, the selected autonomous routine is used when the autonomous period runs. The menu selection flow ends after the required match, mode, and (for tournament) alliance choices are made.

### Utilities

Choose **Utilities** from the initial menu to open **Select Autonomous** and **Set Match Time**.

- **Select Autonomous:** Choose the Competition or Skills category, then choose **Recorded** or **Programmed**. Programmed selections call the corresponding `autonomous_competition()` or `autonomous_skills()` implementation. For a recorded selection, move through the list with **Next** (left button), choose the highlighted recording with **Select** (center), or return with **Back** (right). The menu only lists `.auto` recordings and hides partner-controller files.
- **Set Match Time:** Choose Competition or Skills, then use the left button to add five seconds and the center button to subtract five seconds. The right button saves. Values are bounded from 0 to 3600 seconds.

Autonomous selections and both match times are stored in `/usd/config.json` on the Brain's SD card and loaded at startup. Saving a setting updates the relevant values while preserving unrelated JSON settings already in the file.

### Training an autonomous routine

To record a routine, choose the desired match type and **Train Autonomous** mode in the initial menu. When operator control starts, the framework records the physical controller input for the configured match duration and saves it to that match type's autonomous directory. Training requires an SD card and the category directory described below. The default configuration records the master controller; set `PARTNER_CONTROLLER` to `1` in `src/main.cpp` if the routine should also record partner-controller input. The program exits after training finishes.

## Recorded autonomous routines

Recorded routines are stored on the Brain's SD card under:

```text
/usd/autonomous/competition/
/usd/autonomous/skills/
```

Create these directories on the SD card before using recording or playback; the robot does not create them. The menu lists available recordings for the selected category. The `Robot` framework also provides a training flow that records controller input and plays it back through virtual controllers.

Recording files use the `.auto` extension. When partner-controller recording is enabled, the partner input is saved as a separate recording with a `-partner` suffix.

## Where to look next

- Start with `include/umbc/robot.hpp` for the robot's modes, selections, and public operations.
- See `src/main.cpp` for how the framework is connected to the V5 program lifecycle.
- See `src/umbc/menu.cpp` for the LCD menu flow and `src/umbc/robot.cpp` for autonomous selection, training, and match-time behavior.
- See the corresponding files in `src/umbc/` for the implementation of each framework class.
