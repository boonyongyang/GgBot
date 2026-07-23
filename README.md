# GgBot

An interactive 3D robot renderer with animations and weapon modes.

The project exists in two forms:

- **`src/`** — the current cross-platform port targeting **macOS Apple Silicon**
  (and any platform with OpenGL 3.3 Core Profile), built with CMake + GLFW + GLAD + GLM.
- **`Main.cpp`** — the original Windows-only implementation (Win32 + legacy OpenGL),
  kept for reference. Open `GgBot.sln` in Visual Studio to build it.

See `CLAUDE.md` for the architecture and `ROADMAP.md` for status and planned work.

## Building (macOS / cross-platform)

```bash
cmake -B build
cmake --build build
./build/GgBot
```

Dependencies (GLFW 3.4, GLAD, GLM) are fetched automatically by CMake via
`FetchContent`; `stb_image` is vendored. The `shaders/` and `textures/` folders are
copied next to the binary by a `POST_BUILD` step, and the executable resolves them
relative to its own location, so it can be launched from any directory.

On Linux you may need the usual GLFW build prerequisites:

```bash
sudo apt-get install -y libxrandr-dev libxinerama-dev libxcursor-dev \
  libxi-dev libgl1-mesa-dev libglu1-mesa-dev
# then configure with: cmake -B build -DGLFW_BUILD_WAYLAND=OFF
```

## Controls

### Mouse
- **Drag (left button)** — orbit the robot (x/y axes)
- **Scroll wheel** — zoom (perspective)

### View & projection
- **O** — toggle legacy Windows look ↔ modern Phong shading
- **P** — toggle perspective ↔ orthographic (ortho is the default)
- **SPACE** — reset
- **5 / 6** — move camera left / right
- **7 / 8** — move camera up / down (ortho) or front / back (perspective)
- **W / A / S / D** — change facing direction (front / left / back / right)

### Lighting
- **L** — toggle lighting
- **G / H / J** — toggle ambient / diffuse / specular
- **3 / 4** — switch light color scheme (white / blue)

### Textures
- **K** — cycle outer texture (5)
- **M** — cycle inner texture (3)

### Animation
- **UP / DOWN** — move robot front / back (perspective)
- **LEFT / RIGHT** — move robot left / right
- **Q** — walk
- **F** — toggle upper-body rotation
- **B / N** — turn head left / right
- **T / Y** — right arm up / down
- **U / I** — left arm up / down
- **Z / X** — fingers fist / open

### Weapon modes & attacks
- **F1** — normal
- **F2** — dual cannon (**C** to shoot)
- **F3** — sword + cannon (**V** sword attack, **C** shoot)
- **F4** — HI pose
- **E** — 360° spin attack (**C** to shoot)
