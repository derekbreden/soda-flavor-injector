# Fluid Fitting Alternatives for Cartridge Dock

Survey of every viable option for the 4 fluid connections between the removable pump cartridge and the dock. Current design assumes John Guest push-to-connect with a cam lever release plate — this was never compared against alternatives.

**Application:** Food-grade flavor concentrate, <5 PSI, ~30-60 ml/min, 1/4" OD tubing, connect/disconnect once per 18-36 months.

---

## Options Evaluated

### A. John Guest Push-to-Connect (Current Assumption) — $8 for 4

**How it works:** Tube pushes in past collet teeth + O-ring. To disconnect, push collet ring inward while pulling tube out.

| Spec | Value |
|---|---|
| Body OD | ~12-14mm |
| Body length (union) | ~38-42mm |
| Insertion depth | ~15-18mm per side |
| Cost (PI0408S) | ~$2 each, $8 for 4 |
| Food safety | NSF 61 (potable water) |
| Auto-shutoff | ❌ No — open bore when disconnected |
| Disconnect method | Two-handed (push collet + pull tube) |
| Intuitiveness | **Poor** — user must know to push the collet, non-obvious after 2 years |
| Release mechanism needed? | **Yes** — 4 simultaneous collet releases require cam lever / release plate |

**Pros:** Cheap, proven in beverage applications, ubiquitous availability.
**Cons:** Non-intuitive disconnect, no auto-shutoff, requires complex release mechanism for simultaneous 4-fitting disconnect.

### B. CPC Quick-Disconnects (PLC NSF Series, Valved) — $70 for 4 ★

**How it works:** Purpose-built two-piece quick-disconnect. Thumb latch on body clicks over insert. Valved versions have spring-loaded poppet valves on BOTH halves that close automatically when separated.

| Spec | Value |
|---|---|
| Body OD | ~22.4mm |
| Body length | ~29-50mm (threaded vs panel-mount) |
| Mated pair length | ~65-75mm |
| Cost (PLC NSF valved) | ~$17-18 per pair, **$70 for 4 connections** |
| Food safety | **NSF 169** (specifically for food equipment — strongest cert available) |
| Auto-shutoff | **✅ Yes — both sides valve closed on disconnect** |
| Disconnect method | One-handed squeeze-and-pull |
| Intuitiveness | **Good** — thumb latch is visually obvious, audible click confirms connection |
| Release mechanism needed? | **No** — each coupling is independently thumb-released |

**Pros:** Only option with auto-shutoff (zero dripping during swap), eliminates cam lever/release plate entirely, NSF 169 food cert, one-hand operation, audible click feedback.
**Cons:** Expensive ($70 total), larger body OD (22mm vs 12mm for JG), requires more space on mating face.

**This is the only option that prevents dripping during cartridge swap.** Under a sink where dripping means water damage/staining, this has real product value.

### C. Barb Fittings with Hose Clamps — $5 for 4

Cheapest option. Push silicone tubing over a barbed nipple.

| Spec | Value |
|---|---|
| Cost | $0.75-$2.50 per connection, $3-10 total |
| Food safety | FDA compliant materials |
| Auto-shutoff | ❌ No |
| Disconnect method | Twist and pull hard — requires significant force |
| Intuitiveness | **Poor** — pulling tubing off a barb is non-obvious and physically difficult |
| Release mechanism needed? | No, but disconnection is unacceptable for a consumer product |

**Not recommended.** The disconnect experience is unacceptable for a consumer product.

### D. Luer Lock — $12 for 4

Medical/lab fittings. Tapered male cone into female socket with quarter-turn threaded lock collar.

| Spec | Value |
|---|---|
| Bore | ~2.5-2.9mm (significant flow restriction from 6.35mm tube) |
| Cost | $1-6 per pair, $4-24 total |
| Food safety | FDA compliant materials (medical-grade) |
| Auto-shutoff | ❌ No (specialty valved versions rare and expensive) |
| Disconnect method | Twist collar and pull — intuitive (widely understood paradigm) |
| Intuitiveness | **Good** — twist-to-unlock is universally understood |
| Release mechanism needed? | No — individual twist-off per connection |

**Concern:** The ~2.5-2.9mm bore is an 80% flow area reduction from 1/4" tubing. May not matter at 30-60 ml/min but adds unnecessary restriction.

### E. SMC Pneumatic Push-to-Connect — $17 for 4

Functionally identical to John Guest but from the industrial pneumatics market.

- Standard KQ2 series: **NOT food-safe certified**
- FDA-compliant KQG2-F series: $15-30 per fitting (much more expensive)
- Same two-handed disconnect problem as John Guest
- **Not recommended over John Guest** — same UX weakness, higher cost, no food cert on affordable series.

### F. Custom 3D-Printed Bayonet — ~$1 for 4 (O-rings only)

Quarter-turn bayonet integrated into the 3D-printed cartridge body with O-ring seals.

| Spec | Value |
|---|---|
| Cost | ~$0.20-1.00 (O-rings only; cartridge body is already being printed) |
| Food safety | **❌ FDM layer lines harbor bacteria** — not certifiable for food contact |
| Disconnect method | Quarter-turn and pull |
| Intuitiveness | Fair — user must know direction and amount of turn |
| Release mechanism needed? | No — bayonet IS the mechanism |

**Risks:** FDM tolerance (±0.2mm) is marginal for O-ring sealing. Bayonet tabs may crack under torsional load at layer lines. Good for prototyping, not for production without injection molding.

### G. Magnetic Coupling with O-Ring — ~$15 for 4

Neodymium magnets self-align and pull cartridge into dock. O-rings seal the fluid path.

| Spec | Value |
|---|---|
| Cost | $10-25 total (magnets + O-rings + custom design) |
| Food safety | Achievable if magnets are isolated from fluid path |
| Auto-shutoff | ❌ No (could add valves but complex) |
| Disconnect method | Pull straight out — magnets release |
| Intuitiveness | **Excellent** — push in (self-aligns), pull out. No instruction needed. Apple MagSafe UX. |
| Release mechanism needed? | No — pull overcomes magnetic force |

**Risks:** Magnetic force must be carefully balanced (strong enough to seal, weak enough to pull apart). May attract metallic debris. May interfere with pogo pins or sensors. No precedent in food/beverage equipment. Fully custom design — no off-the-shelf product exists.

### H. Press-Fit / Friction Fit with O-Ring — ~$2 for 4

Cylindrical stubs push into close-tolerance bores. O-rings provide sealing and frictional retention. No locking mechanism.

| Spec | Value |
|---|---|
| Cost | $0.20-1.00 (O-rings only) + dock bore machining/molding |
| Food safety | FDA compliant with standard food-grade materials |
| Auto-shutoff | ❌ No |
| Disconnect method | Pull straight out — O-ring friction releases |
| Intuitiveness | **Very good** — push in, pull out. Nearly as intuitive as magnetic. |
| Release mechanism needed? | No — cartridge simply slides in and out |

**Risk:** No positive locking. Vibration, bumps, or accidental contact could partially unseat connections and cause leaks. Mitigation: guide rails with a detent/latch on the cartridge housing (not on the fittings themselves).

---

## Cross-Cutting Comparison

### Which eliminate the cam lever / release plate?

| Eliminates release mechanism | Options |
|---|---|
| ✅ Yes | B (CPC), D (Luer), F (Bayonet), G (Magnetic), H (Press-Fit) |
| ❌ No — requires release mechanism | A (John Guest), E (SMC) |

Only John Guest and SMC push-to-connect fittings require the complex release plate because their collets must be individually depressed for disconnection. **Every other option eliminates this mechanism entirely.**

### Which provide auto-shutoff (no drip on disconnect)?

Only **B. CPC valved couplings.** Every other option requires draining lines before disconnection or accepting drips.

### Intuitiveness Ranking (for a 2-year-gap user)

| Rank | Option | Why |
|---|---|---|
| 1 | G. Magnetic | Push in (self-aligns), pull out. Zero learning curve. |
| 2 | H. Press-Fit | Push in, pull out. Nearly as intuitive but no self-alignment. |
| 3 | B. CPC | Squeeze-and-pull is natural; audible click confirms connection. |
| 4 | D. Luer Lock | Twist-to-unlock is a widely understood paradigm. |
| 5 | F. Bayonet | Twist-lock but user must know direction and amount. |
| 6 | C. Barb | Push on is obvious; pulling off is difficult and confusing. |
| 7 | A/E. JG/SMC | Push-in is easy; disconnect requires non-obvious collet push. |

---

## Summary Matrix

| Criterion | A. JG | B. CPC ★ | C. Barb | D. Luer | F. Bayonet | G. Magnetic | H. Press-Fit |
|---|---|---|---|---|---|---|---|
| **Cost (4x)** | $8 | **$70** | $5 | $12 | $1 | $15 | $2 |
| **Auto-shutoff** | ❌ | **✅** | ❌ | ❌ | ❌ | ❌ | ❌ |
| **Intuitiveness** | Poor | Good | Poor | Good | Fair | Excellent | Very Good |
| **One-hand disconnect** | ❌ | ✅ | ❌ | ✅ | ✅ | ✅ | ✅ |
| **Food cert** | NSF 61 | **NSF 169** | FDA mat'l | FDA mat'l | ❌ | Custom | FDA mat'l |
| **Eliminates lever** | ❌ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| **Off-the-shelf** | ✅ | ✅ | ✅ | ✅ | ❌ | ❌ | Partial |
| **Connection reliability** | High | High | Moderate | High | Low-Med | Med-High | Medium |
| **Production-ready** | ✅ | ✅ | ✅ | ✅ | ❌ | ❌ | ✅ |

---

## Implications for Other Design Documents

### If CPC is chosen:
- `cam-lever.md`, `collet-release.md`, `release-plate.md` become **obsolete** — no release mechanism needed
- `mating-face.md` needs complete rewrite — 4 CPC panel-mount bodies replace John Guest fittings, different port spacing (22mm body OD vs 12mm)
- `cartridge-envelope.md` — CPC inserts replace tube stubs, slightly different cartridge rear face
- `cartridge-change-workflow.md` — simplified to: squeeze-pull each CPC, slide cartridge out (or design so slide-out actuates all 4)
- `bill-of-materials.md` — $70 instead of $8 for fittings, but subtract cam lever + release plate hardware

### If Press-Fit is chosen:
- Same obsolescence of cam lever / release plate docs
- Mating face is simpler (smooth stubs + O-rings)
- Need to add a detent/latch mechanism to the cartridge housing for positive retention
- Cheapest option but reliability concern needs physical testing

### If John Guest is kept:
- All existing release mechanism research remains relevant
- The cam lever / release plate adds ~$5-10 in hardware plus significant design complexity
- The UX weakness (non-intuitive disconnect) is accepted as a tradeoff for lower cost

---

## What This Research Changes

The existing design invested heavily in solving the John Guest collet release problem (cam lever, release plate, stepped bores, push rods). **Five of seven alternatives eliminate this problem entirely.** The question shifts from "how do we release 4 collets simultaneously?" to "which fitting type best serves a consumer who does this once every 2 years?"

The three most viable options for production:

1. **CPC valved ($70)** — best UX, only auto-shutoff, eliminates release mechanism, NSF 169 certified
2. **Press-Fit ($2)** — cheapest, good UX, eliminates release mechanism, needs positive latch for reliability
3. **John Guest + release plate ($15-20 total)** — proven, food-safe, but worst disconnect UX and most complex mechanism
