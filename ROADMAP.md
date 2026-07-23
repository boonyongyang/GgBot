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
- [x] **Consistent animation speed across displays.** Animation state advances once per
      rendered frame, so on a 120 Hz display the robot moved ~2× too fast. The render loop
      now caps to ~60 FPS. (A full delta-time rewrite would decouple update from render —
      a larger refactor, since draw functions currently mutate animation state; the frame
      cap solves the reported symptom with near-zero risk.)
- [x] De-duplicated the `metal2.bmp` load (`texMetal` now reuses `textureArrOuter[0]`).

## P2 — Fidelity polish (optional)

- [x] Wireframe foot sphere rendered as actual lines (`glPolygonMode(GL_LINE)`), matching
      the original's `GL_LINE_STRIP`.
- [ ] Optionally texture the flat detail pieces (spine-joint sub-spheres, `drawScale`,
      `drawLegInnerNerve`, `drawCoreDetail1`). **Intentionally left as-is** — documented in
      CLAUDE.md as a deliberate "internal detail = flat color" simplification.
- [ ] Sphere UV/pole axis matches `gluSphere`. **Deferred** — cosmetic only (shifts the
      sky-sphere texture seam); negligible visual impact, not worth the churn.

## P3 — Docs / UX / cleanup

- [x] README: macOS/CMake build section added (plus the full modern controls incl. `O`).
- [x] Controls printed to stdout on launch.
- [x] Annotated the intentionally-unused `drawInnerBodyStructure` and `renderCuboidGLU`
      (faithful to the original's dead code, now flagged for readers).

## Apple Silicon verification checklist (for the maintainer)

1. `cmake -B build && cmake --build build` on macOS (Apple Silicon).
2. `./build/GgBot` — window opens, robot renders.
3. Toggle `O` (legacy ↔ modern) and `P` (ortho ↔ perspective); confirm both look right.
4. Exercise animations (Q walk, E spin, F1–F4 weapon modes, mouse orbit, scroll zoom).
5. Report any visual difference vs the Windows build so it can be tuned.
