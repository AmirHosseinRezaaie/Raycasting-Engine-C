# Raycasting Engine in C with Raylib

**A retro-style pseudo-3D engine inspired by Wolfenstein 3D – built from scratch in pure C!**

<div align="center">

**3D First-Person Views**

<img src="https://lodev.org/cgtutor/images/wolf3d.jpg" width="300" alt="Classic Wolfenstein 3D raycasting view">

<img src="https://external-preview.redd.it/a-simple-raycaster-game-in-c-v0-d203NTY2dmtkeXJkMc4sujC9o6lKEqUwo_oVKZgMqQg7Ng9cY-8DCqcAT07p.png?format=pjpg&auto=webp&s=43b349a9f35cb9117c9f28826edb6ff7079d1248" width="300" alt="Simple raycaster in C">

<img src="https://preview.redd.it/oze1fzu02oe61.gif?width=637&auto=webp&s=99e31bd1b7f8f2cc3a41fa7877e77d1878a46a96" width="300" alt="Raycasting with Raylib animated example">

<br><br>

**2D Map Editor & Demos**

<img src="https://lodev.org/cgtutor/images/wolfmapedit.jpg" width="300" alt="Top-down map editor mode">

<img src="https://austinhenley.com/blog/images/raycastingdemo.png" width="300" alt="Modern raycasting demo with split view">

<img src="https://lodev.org/cgtutor/images/raycasteruntextured.gif" width="300" alt="Untextured raycasting animation">

</div>

This project implements a classic **raycasting** renderer, recreating the iconic "3D" illusion of early 90s FPS games like *Wolfenstein 3D* – all using just a 2D grid map!

Navigate a maze-like world in first-person view, with smooth movement, collision detection, and perspective-correct wall rendering. Plus, toggle to a **runtime map editor** mode to build or destroy walls on the fly!

Built with pure **C99** and the lightweight **raylib** library – no modern 3D APIs, just raw math and pixels.

### Features
- **Raycasting with DDA algorithm** → Fast, accurate ray-grid intersection
- **Perspective projection** → Fisheye correction, wall height scaling, simple shading
- **Player movement** → WASD for translation/strafe, arrow keys for rotation (frame-rate independent)
- **Collision detection** → With wall sliding for natural feel
- **Dual modes** → Press `M` to switch between 3D play and 2D top-down editor
- **Interactive editing** → Left/right mouse click to place or remove walls in editor mode
- **Bonus (optional)** → Save/load maps to/from text files (F5/F9)

### Tech Stack
- **Language:** Pure C (C99 standard)
- **Library:** [raylib](https://www.raylib.com/) – Simple and powerful for windowing, input, and 2D drawing
- No external 3D engines – everything rendered manually with `DrawLine`!

### How to Build & Run
1. Install raylib (e.g., `sudo apt install libraylib-dev` on Ubuntu)
2. Clone the repo:
   ```bash
   git clone https://github.com/yourusername/raycasting-engine-c.git
   cd raycasting-engine-c
