# clipboard

[![CI](https://github.com/alya-lang/clipboard/actions/workflows/ci.yml/badge.svg)](https://github.com/alya-lang/clipboard/actions/workflows/ci.yml)
[![License](https://img.shields.io/github/license/alya-lang/clipboard?color=blue&label=License)](LICENSE)
[![Alya](https://img.shields.io/badge/dynamic/toml?url=https%3A%2F%2Fraw.githubusercontent.com%2Falya-lang%2Fclipboard%2Fmain%2Falya.toml&query=%24.package.alya-version&label=Alya&color=orange&prefix=%3E%3D)](https://github.com/alya-lang/alya)
[![Package Version](https://img.shields.io/badge/dynamic/toml?url=https%3A%2F%2Fraw.githubusercontent.com%2Falya-lang%2Fclipboard%2Fmain%2Falya.toml&query=%24.package.version&label=Version&color=brightgreen)](alya.toml)

Cross-platform clipboard toolkit: text, HTML, image, file list, history, watcher and sync for Alya

---

## 🌟 Features

- 📋 **Multi-Format Payloads**: Unicode text, HTML with plain-text fallback, raw RGB/RGBA images, PCM audio, file path lists, and application-defined custom formats
- 🖼️ **Image Pipeline**: Solid-fill synthesis, RGB grayscale conversion, nearest-neighbor thumbnails, and dimension/byte-length validation
- 🔊 **Audio Pipeline**: Silence synthesis plus square/triangle-wave tone generators in pure integer arithmetic, with duration and frame introspection
- 🕘 **History Log**: Retention-capped `ClipboardEntry` journal with indexed access, per-format search, and snapshot dump/restore round-trips
- 👀 **Change Watcher**: Cooperative sequence polling with `fn(entry)` hooks — no threads, no blocking, headless-safe
- 🖥️ **Native OS Bridge**: Best-effort Win32 clipboard backend (text) behind the `native` feature; macOS/Linux fall back to deterministic stubs so CI never needs a display server
- 🧩 **Modular Architecture**: Clean public facade (`src/lib.alya`), rich data models (`src/types.alya`), and encapsulated engines (`src/core/store.alya`, `codec.alya`, `history.alya`, `watcher.alya`, `system.alya`, `formatter.alya`, `extras.alya`)
- 🔒 **Public/Private Visibility (`pub`)**: Fine-grained export control keeping internal helpers encapsulated
- 🚩 **Feature-Gated API Slices**: `native` (OS bridge) and `extras` (synthesis, batch copy, sync, snapshots) slices via `[features]` with descriptive fallback stubs
- 🧪 **Enterprise Test & Benchmark Suite**: Deterministic in-memory tests plus micro-benchmarks for text, image, files, history, and stats paths

---

## 📁 Project Architecture

```
clipboard/
├── .alyalint               # Linter configuration (rules, exclusions, severity overrides)
├── .editorconfig           # Uniform formatting rules across IDEs and editors
├── .gitignore              # Ecosystem standard ignore filters
├── .vscode/                # VS Code workspace settings, DAP launch configurations & tasks
├── alya.toml               # Package manifest with dependencies, [features] and optional [build]
├── c/                      # Native C sources for zero-dependency FFI packages
│   ├── clipboard.c         # Portable fallback core (version, limits, arithmetic probe)
│   ├── clipboard.h         # Shared native API declarations
│   ├── win32_clipboard.c   # Windows backend (OpenClipboard, CF_UNICODETEXT, sequence)
│   ├── cocoa_clipboard.c   # macOS stub backend (headless-safe no-op)
│   └── linux_clipboard.c   # Linux stub backend (headless-safe no-op)
├── src/
│   ├── lib.alya            # Public API facade (pub exports, re-exports & pipeline runners)
│   ├── types.alya          # Data models, pub enums, pub structs, and struct methods
│   ├── ffi.alya            # Native extern "C" declarations
│   └── core/               # Subdirectory module hierarchy
│       ├── store.alya      # In-memory state engine (copy/paste/has/clear per format)
│       ├── codec.alya      # Text/HTML/image/audio/file helpers and validators
│       ├── history.alya    # History log queries and retention tuning
│       ├── watcher.alya    # Sequence polling and change hooks
│       ├── system.alya     # Native OS bridge (feature-gated `native`)
│       ├── formatter.alya  # Human-readable renderers for entries, stats, payloads
│       └── extras.alya     # Synthesis, batch copy, sync, snapshots (feature-gated `extras`)
├── examples/
│   └── demo.alya           # Comprehensive runnable walkthrough of all package capabilities
├── tests/
│   └── test_basic.alya     # Automated test suite with 100% feature coverage
└── benches/
    └── bench_basic.alya    # Micro-benchmarks measuring performance and throughput
```

> [!NOTE]
> **Visibility & Modularity:** Symbols annotated with `pub` (`pub function`, `pub struct`, `pub enum`, `pub interface`) are exported to external consumers and re-exporting modules. Symbols without `pub` remain strictly internal to their declaring module, preventing symbol collisions and implementation leakage.

---

## 📦 Installation

Add `clipboard` to the `[dependencies]` section in your `alya.toml`:

```toml
[dependencies]
clipboard = { git = "https://github.com/alya-lang/clipboard", branch = "main" }
```

Or install it directly using the Alya package CLI:

```bash
alya add clipboard --git https://github.com/alya-lang/clipboard --branch main
alya install
```

---

## 🚀 Quick Start

```alya
import "clipboard" as pkg

function main()
    # 1. Text round-trip on an in-memory clipboard
    let cb = pkg::new_clipboard()
    cb.copy_text("hello clipboard")
    say f"Text: {cb.paste_text()}"

    # 2. Image payload with validation and methods
    let img = pkg::blank_image(8, 8, 3, 200)
    cb.copy_image(img)
    say f"Image: {pkg::format_image(cb.paste_image())}"

    # 3. Audio payload, history, and stats
    cb.copy_audio(pkg::beep_audio(440, 100, 8000, 60))
    say f"History: {cb.history_len()} entries, last: {cb.history_last().preview}"
    say f"Stats:   {cb.stats().stats_summary()}"
end

main()
```

---

## 📖 API Reference

| Symbol | Visibility | Description |
|---|---|---|
| `new_clipboard(backend, max_history)` | `pub function` | Factory creating an empty `Clipboard` (`"memory"` backend, 32 history entries by default). |
| `clipboard_from_config(cfg)` | `pub function` | Factory creating a `Clipboard` from a `ClipboardConfig` instance. |
| `clipboard_config(backend, max_history, native_fallback)` | `pub function` | Factory constructing clipboard construction options. |
| `native_add(a, b)` | `pub function` | Bundled C engine smoke test (`a + b` executed natively). |
| `Clipboard.copy_text(text)` | `pub method` | Stores normalized text, replacing any previous text payload. |
| `Clipboard.paste_text()` | `pub method` | Returns stored text, or null when absent. |
| `Clipboard.has_text()` | `pub method` | Returns 1 for non-blank stored text, 0 otherwise. |
| `Clipboard.copy_html(html, plain)` | `pub method` | Stores HTML markup with an optional plain-text alternative. |
| `Clipboard.paste_html()` | `pub method` | Returns the stored `{ "html", "plain" }` payload map, or null. |
| `Clipboard.paste_html_text()` | `pub method` | Returns the plain-text alternative of the stored HTML, or null. |
| `Clipboard.has_html()` | `pub method` | Returns 1 when an HTML payload is present, 0 otherwise. |
| `Clipboard.copy_image(img)` | `pub method` | Stores a `ClipboardImage`; returns 1 on success, 0 when invalid. |
| `Clipboard.paste_image()` | `pub method` | Returns the stored `ClipboardImage`, or null when absent. |
| `Clipboard.has_image()` | `pub method` | Returns 1 for a valid stored image, 0 otherwise. |
| `Clipboard.copy_audio(aud)` | `pub method` | Stores a `ClipboardAudio`; returns 1 on success, 0 when invalid. |
| `Clipboard.paste_audio()` | `pub method` | Returns the stored `ClipboardAudio`, or null when absent. |
| `Clipboard.has_audio()` | `pub method` | Returns 1 for a valid stored audio payload, 0 otherwise. |
| `Clipboard.copy_files(paths)` | `pub method` | Stores a normalized file path list; 0 when the list is empty. |
| `Clipboard.paste_files()` | `pub method` | Returns the stored file path array, or null when absent. |
| `Clipboard.has_files()` | `pub method` | Returns 1 for a non-empty stored file list, 0 otherwise. |
| `Clipboard.set_custom(name, value, mime)` | `pub method` | Stores an application-defined payload under `custom:<name>`. |
| `Clipboard.get_custom(name)` | `pub method` | Returns a custom payload by name, or null when absent. |
| `Clipboard.has_custom(name)` | `pub method` | Returns 1 when the named custom payload exists, 0 otherwise. |
| `Clipboard.clear_format(name)` | `pub method` | Clears one format slot; returns 1 when a payload was removed. |
| `Clipboard.clear()` | `pub method` | Clears every format slot; returns the removed format count. |
| `Clipboard.formats()` | `pub method` | Returns an independent copy of the present format-name list. |
| `Clipboard.has(name)` | `pub method` | Returns 1 when the named format slot is present, 0 otherwise. |
| `Clipboard.is_empty()` | `pub method` | Returns 1 when no payload is stored, 0 otherwise. |
| `Clipboard.meta(name)` | `pub method` | Returns the `ClipboardEntry` metadata for a slot, or null. |
| `Clipboard.stats()` | `pub method` | Builds a `ClipboardStats` snapshot of this container. |
| `Clipboard.clipboard_summary()` | `pub method` | One-line container overview with backend, formats, and counters. |
| `Clipboard.clipboard_describe()` | `pub method` | Detailed container description with the last mutation timestamp. |
| `Clipboard.clipboard_valid()` | `pub method` | Returns 1 (a constructed clipboard is always usable). |
| `Clipboard.history_len()` | `pub method` | Returns the retained history entry count. |
| `Clipboard.history_last()` | `pub method` | Returns the most recent history entry, or null when empty. |
| `Clipboard.history_get(index)` | `pub method` | Indexed history access (negative counts from the end). |
| `Clipboard.history_by_format(format)` | `pub method` | Returns history entries filtered by format. |
| `Clipboard.on_change(hook)` | `pub method` | Registers a `fn(entry)` change hook for watcher polls. |
| `Clipboard.watch_poll()` | `pub method` | Polls once, firing the hook on change; 1 on change, 0 otherwise. |
| `native_available()` | `pub function` (`native` feature, default-on) | Returns 1 when an OS clipboard backend answered, 0 for stubs. |
| `system_copy_text(text)` | `pub function` (`native` feature, default-on) | Writes text to the OS clipboard (best-effort). |
| `system_paste_text()` | `pub function` (`native` feature, default-on) | Reads text from the OS clipboard (`""` when unavailable). |
| `auto_copy_text(cb, text)` | `pub function` (`native` feature, default-on) | Stores locally and mirrors to the OS clipboard when possible. |
| `auto_paste_text(cb)` | `pub function` (`native` feature, default-on) | Prefers the OS clipboard, falls back to the memory copy. |
| `gradient_image(w, h)` | `pub function` (`extras` feature, default-on) | Synthesizes a deterministic diagonal RGB gradient image. |
| `tone_audio(freq, ms, rate)` | `pub function` (`extras` feature, default-on) | Synthesizes a triangle-wave tone in integer arithmetic. |
| `batch_copy(cb, text, html, img, aud, paths)` | `pub function` (`extras` feature, default-on) | Atomically writes several formats at once; returns written count. |
| `sync_from(dst, src)` | `pub function` (`extras` feature, default-on) | Mirrors every present format from `src` into `dst`. |
| `snapshot_dump(cb)` | `pub function` (`extras` feature, default-on) | Serializes present payloads into a plain map snapshot. |
| `snapshot_restore(cb, dump)` | `pub function` (`extras` feature, default-on) | Restores payloads from a snapshot map; returns restored count. |
| `make_image(w, h, channels, data)` | `pub function` | Factory constructing a `ClipboardImage` payload struct. |
| `make_audio(rate, channels, bits, data)` | `pub function` | Factory constructing a `ClipboardAudio` payload struct. |
| `ClipboardFormat` | `pub enum` | Payload formats (`Text = 0`, `Html = 1`, `Image = 2`, `Audio = 3`, `Files = 4`, `Custom = 5`). |
| `ClipboardBackend` | `pub enum` | Backend selectors (`Memory = 0`, `Native = 1`, `Auto = 2`). |
| `ClipboardImage` | `pub struct` | Image payload (`width`, `height`, `channels`, `data`) with `image_valid`, `pixels`, `byte_len`, `image_summary`, `image_describe` methods. |
| `ClipboardAudio` | `pub struct` | Audio payload (`sample_rate`, `channels`, `bits`, `data`) with `audio_valid`, `frames`, `duration_ms`, `audio_summary`, `audio_describe` methods. |
| `ClipboardEntry` | `pub struct` | History record (`format`, `mime`, `seq`, `created_at`, `size`, `preview`) with `entry_valid`, `entry_summary`, `entry_describe` methods. |
| `Clipboard` | `pub struct` | Stateful container (store/meta/formats/seq/counters/history/watcher) with format, history, and watcher methods. |
| `ClipboardStats` | `pub struct` | Utilization snapshot (`writes`, `reads`, `clears`, `seq`, `formats`, `history_len`, `backend`) with `total_ops` and `stats_summary` methods. |

> [!TIP]
> **Internal Helpers & Documentation:** Public symbols are documented with `##` Markdown docstrings, enabling automatic API documentation generation via `alya doc`. Private functions in `src/core/*.alya` are not annotated with `pub` and remain encapsulated within their respective modules.

---

## 🧪 Running Tests & Benchmarks

Run the automated test suite using `alya test`:

```bash
alya test
```

Exercise feature selection (both `extras` and `native` slices are default-on):

```bash
alya test --features extras,native
alya test --no-default-features
```

Generate static API documentation:

```bash
alya doc . -o docs --markdown
```

Run the benchmark suite:

```bash
alya run benches/bench_basic.alya
```

Run the example demo:

```bash
alya run examples/demo.alya
```

> [!NOTE]
> **Upstream compiler issues:** struct values flowing through `map` slots can miscompile depending on overall program shape — untyped-receiver method calls emitting undefined `fn_<method>` symbols ([alya-lang/alya#133](https://github.com/alya-lang/alya/issues/133), fixed) and struct-field reads returning garbage/crashing under `alya run` ([alya-lang/alya#138](https://github.com/alya-lang/alya/issues/138), open). `alya test` exercises the same operations deterministically and is the reliable verification gate.

Check code formatting:

```bash
alya fmt . --check
```

Run static code linter:

```bash
alya lint . --check
```

---

### 💻 Developer Tooling & VS Code Integration

This package comes preconfigured with recommended workspace settings and tasks for **Visual Studio Code**:
- **LSP & Formatting**: Auto-formatting on save and real-time Language Server diagnostics via `alya-lang.vscode-alya`.
- **DAP Debugging**: Launch configurations in `.vscode/launch.json` ready for interactive step-debugging via `F5`.
- **Predefined Tasks**: Press `Ctrl+Shift+B` or run tasks (`Test`, `Lint`, `Format`, `Build Docs`) directly from the Command Palette.

---

## 🤝 Contributing

Contributions are welcome! Please follow these steps:

1. Fork the repository and clone it locally
2. Install dependencies:
   ```bash
   alya install
   ```
3. Create your feature branch (`git checkout -b feature/my-feature`)
4. Verify tests and formatting before opening a PR:
   ```bash
   alya test
   ```
5. Commit your changes (`git commit -m "feat: add feature"`) and open a Pull Request

---

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
