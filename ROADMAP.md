# GgBot Roadmap

Status and planned work for the macOS / OpenGL 3.3 Core port (`src/`). The original
Windows `Main.cpp` is reference-only and not tracked here.

## Done — v1 port

- [x] Multi-file port: Win32 + fixed-function GL → GLFW 3.4 + GLAD (GL 3.3 Core) + GLM, built with CMake.
- [x] Procedural cached mesh generators replacing GLU quadrics; stb_image texture loading.
- [x] Builds clean (zero warnings) and runs; verified by headless render under Mesa software GL.
- [x] Correctness review of all 34 `draw*()` functions vs `Main.cpp`, with fixes:
      cylinder axis (+Y → +Z), `drawTopBack` color asymmetry, three leg texture slips.
- [x] Reproduces the original fixed-function look (verbatim projection, flat/fullbright
      shading, `GL_COLOR_MATERIAL`-off color behaviour).
- [x] `O` key toggles legacy Windows look ↔ modern per-fragment Phong.
- [x] `CLAUDE.md` architecture + fidelity docs. PR: #2.

## P0 — Validation & CI (confidence blockers)

- [ ] **Run on real Apple Silicon hardware.** Everything so far was verified on a Linux
      toolchain with Mesa *software* GL. Needs a physical Mac (Apple GPU, real GL 3.3 Core
      context). *Owner: maintainer — see the checklist at the bottom.*
- [x] **GitHub Actions CI.** Builds on `macos-latest` (validates the Apple path) and
      `ubuntu-latest`; runs a headless smoke render on Linux (Xvfb + Mesa via
      `GGBOT_MAX_FRAMES`) so regressions are caught automatically. `.github/workflows/ci.yml`.
- [x] **Startup asset check.** Fails loudly with a clear message if `shaders/` or `textures/`
      are missing, instead of a silent black/invisible model.

## P1 — Robustness

- [x] **Resolve asset paths relative to the executable** (chdir to the binary's directory at
      startup), so `./GgBot` works from anywhere — no longer tied to `build/`.
- [ ] **Frame-rate-independent animation.** Angles advance a fixed amount per frame, so
      animation speed scales with FPS (a 120 Hz Mac animates ~2× faster than 60 Hz). Drive
      updates by delta-time. *Executable now.*
- [ ] De-duplicate the `metal2.bmp` load (loaded for both `textureArrOuter[0]` and `texMetal`).

## P2 — Fidelity polish (optional)

- [ ] Wireframe foot sphere rendered as actual lines (original used `GL_LINE_STRIP`; the port
      draws a solid sphere).
- [ ] Optionally texture the flat detail pieces (spine-joint sub-spheres, `drawScale`,
      `drawLegInnerNerve`, `drawCoreDetail1`) to match the original's inherited texture.
- [ ] Sphere UV/pole axis matches `gluSphere` (affects sky/eye texture seam placement).

## P3 — Docs / UX / cleanup

- [x] README: macOS/CMake build section added (plus the full modern controls incl. `O`).
- [ ] Print the controls to stdout on launch, or add a small on-screen help overlay.
- [ ] Remove or clearly annotate the intentionally-unused `drawInnerBodyStructure` and
      `renderCuboidGLU` (faithful to the original's dead code, but flagged for readers).

## Apple Silicon verification checklist (for the maintainer)

1. `cmake -B build && cmake --build build` on macOS (Apple Silicon).
2. `./build/GgBot` — window opens, robot renders.
3. Toggle `O` (legacy ↔ modern) and `P` (ortho ↔ perspective); confirm both look right.
4. Exercise animations (Q walk, E spin, F1–F4 weapon modes, mouse orbit, scroll zoom).
5. Report any visual difference vs the Windows build so it can be tuned.
