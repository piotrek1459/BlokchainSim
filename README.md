Here’s a drop-in **`README.md`** you can copy to the root of the repo.
It explains—from zero to running—the Docker workflow on Windows (GUI and
headless) and on any Linux/macOS host.

---

````markdown
# Blockchain Simulator (Qt + C++20)

A desktop demo that shows core blockchain concepts (blocks, proof-of-work,
transaction parsing) with a Qt6 GUI.  
This README focuses on the **Docker** distribution—no local tool-chain
needed.

---

## ✨ Features

* C++20, Qt 6 Widgets, OpenSSL SHA-256
* Optional proof-of-work (difficulty selector)
* Transaction pattern validation (`Alice->Bob:50`)
* Save / load ledger to *chain.json*
* Unit-tested with Google Test & built in CI on Windows + Ubuntu
* Multi-stage **Dockerfile** – tiny runtime image (~230 MB)

---

## 1 · Prerequisites

| Host OS | Requirement |
|---------|-------------|
| **Windows 10 / 11** | **Docker Desktop** (WSL 2 backend preferred) <br/>**VcXsrv** (or X410 / Xming) to see the Qt window |
| **Linux** | Docker Engine (20.10+) and any X-server already running |
| **macOS** | Docker Desktop & XQuartz (optional, only if you want the GUI) |

---

## 2 · Clone the repo

```powershell
git clone https://github.com/piotrek1459/BlokchainSim.git
cd <BlokchainSim>
````

---

## 3 · Build the image

```powershell
docker build -t blokchainsim .
```

*First run downloads Qt & compiles the code (≈ 3 min).
Subsequent builds use Docker cache and finish in seconds.*

---

## 4 · Run modes

### 4.1 Headless / CI (no GUI)

Perfect for GitHub Actions or servers without an X-server.

```bash
docker run --rm -e QT_QPA_PLATFORM=offscreen blokchainsim
```

### 4.2 Visible GUI on **Windows**

1. **Start VcXsrv**

   ```
   Start ►  XLaunch
     • Multiple windows   (display 0)
     • Start no client
     • ☐  Disable access control   ✔  ← tick this
   ```

2. **Run the container**

   ```powershell
   docker run --rm ^
     -e DISPLAY=host.docker.internal:0 ^
     blokchainsim
   ```

   The Qt MainWindow pops up on your Windows desktop.

### 4.3 Visible GUI on **Linux / WSL 2 / macOS**

```bash
# Linux host (assumes DISPLAY=:0 already exported)
docker run --rm -e DISPLAY=$DISPLAY blokchainsim
```

macOS users: start **XQuartz**, enable *“Allow connections from network
clients”*, then `-e DISPLAY=host.docker.internal:0`.

---

## 5 · Runtime options

| Env var           | Default                   | Description                                                               |
| ----------------- | ------------------------- | ------------------------------------------------------------------------- |
| `QT_QPA_PLATFORM` | *(unset)*                 | `offscreen` = disable window; any other value lets Qt choose XCB/Wayland. |
| `LEDGER_PATH`     | `./blockchain/chain.json` | Override where the app reads/writes the ledger.                           |
| `POW_DIFFICULTY`  | `2`                       | Mining difficulty for proof-of-work nodes.                                |

Example:

```bash
docker run --rm \
  -e POW_DIFFICULTY=5 \
  -e LEDGER_PATH=/data/mychain.json \
  -v $PWD:/data \
  blokchainsim
```

---

## 6 · Cleaning up

```bash
docker image rm blokchainsim          # remove the image
docker builder prune -f               # free build cache space
```

---

## 7 · Troubleshooting

| Symptom                        | Fix                                                                                                                                                                               |
| ------------------------------ | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `could not connect to display` | X-server not running or access control enabled; restart VcXsrv with **Disable access control**.                                                                                   |
| Qt complains about `xcb` libs  | The provided runtime image already installs the needed libs. If you customised the Dockerfile, ensure it still installs: <br>`libx11-xcb1 libxcb-keysyms1 libxcb-render-util0 …`. |
| GUI flickers / blank           | On Windows, some Intel GPUs need the **WGL** backend. Add `-e QT_OPENGL=software`.                                                                                                |

---

### Happy hacking & demoing! 🎉


