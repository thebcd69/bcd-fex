# Licensing of Madeira's modifications

This repository is a fork of [FEX-Emu](https://github.com/FEX-Emu/FEX). **The upstream licence
is unchanged and continues to apply to all upstream code.** See `LICENSE`.

## What is licensed how

| Code | Licence |
|---|---|
| All upstream FEX-Emu code | as in `LICENSE` (MIT) — unchanged |
| Modifications and new files authored for **Madeira** by Will Faust | **GPL-3.0-or-later** |

Where a file contains both, the file as a whole may only be distributed under
terms compatible with GPL-3.0-or-later, because the GPL-covered contributions
cannot be separated from it. The underlying upstream code remains available
under MIT **from upstream**, and nothing here withdraws that.

## Why

The intent is that derivatives of this work which are *distributed* remain open
source. MIT permits a proprietary derivative; the GPL does not. MIT is
GPL-compatible, so combining them this way is permitted.

Three limits, stated plainly rather than left implied:

- The GPL constrains **distribution**. It does not restrict private
  modification, internal use, or a separate program that merely invokes this
  one.
- **Modifications published earlier, while this repository presented itself as
  MIT, were granted under MIT. That grant cannot be revoked.** Only
  contributions made from 2026-08-28 onward are GPL-only. Anyone who already has a
  copy keeps their MIT rights to it.
- It cannot stop anyone independently reimplementing the same ideas.

## Contributing

Contributions are accepted under **GPL-3.0-or-later**. See `CONTRIBUTING.md`.

## Additional permission for the Apple Metal Shader Converter (adopted 2026-09-24)

The Madeira-authored modifications and new files in this repository (the
commits by Will Faust) are offered under GPL-3.0-or-later **with** the
following additional permission, reproduced here in full so this grant is
self-contained. Upstream code keeps its own licence and notices and needs
no exception. Prepared 2026-09-16; the copyright holder adopted it on
2026-09-24, as recorded in the adoption line of the top-level Madeira
repository's LICENSE-EXCEPTION.md, before any public push.

### Madeira Converter Exception, version 1 (of 2026-09-16; in effect from the adoption recorded in the top-level LICENSE-EXCEPTION.md)

Additional permission under GNU GPL version 3 section 7.

If you modify this Program, or any covered work, by linking or combining it
with the Apple Metal Shader Converter dynamic library
(libmetalirconverter.dylib, in any version) or with Apple's Metal,
Foundation, CoreGraphics, QuartzCore, UIKit, AppKit and related system
frameworks, or with modified versions of those libraries, the licensors of
this Program grant you additional permission to convey the resulting work.
Corresponding Source for a non-source form of such a combination shall
include the source code for the parts of the Program used in the
combination, but need not include the source code of those Apple libraries.
This permission does not extend to those libraries, which remain subject to
Apple's own licence terms. You may remove this additional permission from
copies you convey, as GPL-3.0 section 7 allows.

## Third-party contributions

Contributed under `CONTRIBUTING.md` (GPL-3.0-or-later with the Madeira
Converter Exception) and signed off under the DCO; copyright stays with the
author.

- **125hz**, merged 2026-09-29 from pull requests #2, #3 and #4 (32-bit WOW64
  guests on the iOS host): `479da619c`, `d8f9d483c`, `e936d2c6d`,
  `c97be2836`, `b8cd63b23`, `4585944ba`, `da7addf5a`, and their merge
  commits. The two files they add, `Source/Windows/WOW64/IosMonoBridge.cpp`
  and `Source/Windows/WOW64/IosTeb.h`, carry the author's own MIT notice
  (SPDX), which is GPL-compatible.

- Taken from Madeira's FEX fork (`willfaust/FEX`, branch `ios-port-2607`) after the two forks diverged on 2026-10-15, or just before
  it where this fork did not yet have them, with the authors kept in the commit history; Madeira accepts contributions
  under `CONTRIBUTING.md` (GPL-3.0-or-later with the Madeira Converter Exception), and the copyright stays with the author. The first hash is the commit in this
  fork, the second the same commit in Madeira's fork. **125hz**: `7b3d7edb465` (`68bb21cf49e`), 2026-10-11, hot SMC pages for
  data that shares a page with code (this fork's version is adapted, and its behaviour is opt-in); `623756c9aec` (`e6055c96e5b`),
  2026-10-25, the same for the WOW64 module. **Ixion**: `e6ac5f36a5e` (`2934fa4527f`), 2026-10-19, keep x86 loads whose value is
  unused; `fff046661fc` (`dfcc614901d`), 2026-10-19, LookupCache: invalidate L1 entries under every dynamic L1 size. The pin moved to
  them on 2026-11-05.
