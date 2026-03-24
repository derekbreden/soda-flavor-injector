# AI-generated CAD models with Python

Source: https://claude.ai/share/f902fd24-d355-4a0a-92cc-e1e913022717
Date: 2026-03-23

---

## Derek

> (Attached files hidden in shared chat)

So, can I use AI to generate CAD things via python similar to I had AI generate my logo and the animated version of my logo that I use for a loading screen? (It used python and SVG, and converted to a butter smooth iOS animation and 16 frames for the ESP32-S3 screen)

## Claude

Yes, absolutely — and the analogy is a good one. The same workflow pattern applies: you describe what you want, AI generates Python code that produces a precise geometric output, and you get a file you can use downstream.

For CAD, the main library worth looking at is **CadQuery**. It's a Python library built on the OpenCASCADE kernel (real solid modeling, not mesh approximation), and it uses a fluent/chainable API that's very natural to describe to an LLM. You write things like:

```python
result = (
    cq.Workplane("XY")
    .box(60, 40, 10)
    .faces(">Z")
    .hole(5)
)
```

That exports to STEP, STL, or other standard formats. The key insight that maps to your logo experience: just as SVG gave you a precise, resolution-independent representation that could be converted to both iOS animations and ESP32 frames, STEP/STL gives you a precise geometric representation that can go to 3D printing, CNC, or visualization.

The practical workflow would be: describe the part to Claude (or Claude Code), get CadQuery Python back, run it, export to STEP/STL. For your soda machine project specifically, this could be interesting for things like custom pump mounts, hopper brackets, the knob housing, and enclosures — anything where you're currently either hand-designing in a CAD tool or improvising with off-the-shelf parts.

There's also **OpenSCAD** (which uses its own scripting language rather than Python) and **Cadscript** (a simpler wrapper over CadQuery), but CadQuery is the sweet spot for AI-generated work because its API is expressive enough to describe complex parts and well-represented enough in training data that LLMs produce decent code for it.

One caveat compared to your SVG experience: 3D solid modeling has more ways to fail silently (invalid geometry, non-manifold edges) than 2D vector graphics. So the "review the output, don't just vibes-accept it" principle you already apply to AI code applies doubly here — you'd want to visually inspect the generated models before sending them anywhere.

---

## Derek

I bought a Bambu H2C. I have a 180 day trial of OnShape.

One point of friction is that I can edit Python fine, but it is also, even as a total beginner, easier to work with onshape, but taking any change I make back to Python is tedious and manual, or at least requires me describing to the AI what to do.

So it seems "working together" may have some tension points.

## Claude

Yeah, this is a real tension and it's worth thinking about clearly before you build habits around it.

The core problem is that you have two representations of the same part with no reliable round-trip between them. OnShape is the richer editing environment — parametric, visual, immediate feedback — but its internal representation is proprietary. CadQuery Python is the one AI can read and write fluently, but editing it blind (without visual feedback) is clunky for spatial reasoning. And STEP/STL exports are effectively one-way: you can go from either tool *to* those formats, but you can't meaningfully go back from an exported file to an editable parametric model in the other tool.

So the practical question is: which one is your source of truth?

A few ways people handle this:

**OnShape as source of truth, AI as advisor.** You design in OnShape, and use AI to help you *think* — calculate dimensions, suggest approaches, troubleshoot fit issues, generate G-code post-processing scripts. AI never touches the geometry directly. This avoids the sync problem entirely but leaves the AI's generative capability on the table.

**CadQuery as source of truth, OnShape as viewer.** You generate parts in CadQuery via AI, export STEP, import into OnShape purely to inspect and measure. If you need changes, you describe them back to the AI and regenerate. This keeps the AI in the loop but means OnShape is read-only — you never edit there. The friction is that "describe what to change" step you already identified.

**Split by part complexity.** Simple brackets, mounts, spacers — CadQuery via AI. Complex organic shapes or things that need a lot of visual iteration — OnShape. Accept that these are separate workflows and don't try to bridge them.

The thing I'd watch out for is the middle ground where you're half-editing in both and losing changes in translation. That's the same class of problem as maintaining two codebases that are supposed to stay in sync — it always costs more than you expect.

Given you have 180 days on OnShape and you're a total beginner at CAD, it might be worth spending the first few weeks just learning OnShape's paradigm directly. Once you have the mental model of sketches, extrudes, fillets, and constraints, you'll be *much* better at describing changes to AI in CadQuery terms — because you'll know what operations you're actually asking for.

---

## Derek

Oh, so the "operations" I do in onshape, with the right language, can also be done in CADQuery?

## Claude

Yes, exactly. They share the same conceptual foundation — it's all constructive solid geometry and parametric modeling. The operations map pretty directly:

OnShape's sketch → extrude → fillet → chamfer → shell → boolean workflow is essentially the same sequence of operations you'd chain in CadQuery. When you learn in OnShape that you "sketch a rectangle on the XY plane, extrude it 10mm, then fillet the top edges with a 2mm radius," that translates almost literally:

```python
result = (
    cq.Workplane("XY")
    .rect(60, 40)
    .extrude(10)
    .edges("|Z")
    .fillet(2)
)
```

So learning OnShape isn't just learning OnShape — it's learning the *vocabulary* of parametric CAD, which is the same vocabulary you'd use to direct AI in CadQuery. Sketch, extrude, cut, revolve, loft, sweep, fillet, chamfer, shell, boolean union/subtract/intersect — these are the verbs in both systems.

This actually resolves some of the tension you identified. The workflow becomes: learn and explore in OnShape where you get visual feedback, then when you want AI to generate or modify a part, you can say "take this base plate and add a 5mm extrusion with a centered hole, then fillet the outer edges" — and that's precise enough for AI to produce correct CadQuery code, because you're speaking in the shared operation language.

It's similar to how knowing what a peristaltic pump *does* mechanically means you can describe what you need to an AI in code terms. The domain knowledge is the bridge. In this case, OnShape is where you build that domain knowledge, and it transfers directly to the AI-assisted workflow.

---

## Derek

Can you give me a list of the most common "operations"?

## Claude

Sure. These are the core operations you'll encounter in both OnShape and CadQuery, roughly in the order you'd typically use them:

**Creating base shapes:** Sketch a 2D profile (rectangle, circle, polygon, spline) on a workplane, then extrude it into a 3D solid. Revolve spins a 2D profile around an axis instead of pushing it straight. Loft connects two or more 2D profiles at different heights into a smooth transitional shape. Sweep pushes a 2D profile along a path/curve.

**Modifying solids:** Fillet rounds an edge to a given radius. Chamfer cuts an edge at an angle instead of rounding it. Shell hollows out a solid, leaving walls of a specified thickness. Draft adds a slight angle to faces (important for molded parts).

**Boolean operations:** Union combines two solids into one. Cut/subtract removes one solid's volume from another (this is how you make holes, pockets, slots). Intersect keeps only the volume where two solids overlap.

**Hole-making:** Through hole, blind hole (goes partway), counterbore (stepped hole for a bolt head to sit flush), countersink (angled entry for a flat-head screw). These are technically just specialized cuts, but they're so common they get their own operations.

**Workplane manipulation:** Selecting a face of an existing solid and sketching on it, offsetting a workplane by some distance, creating angled workplanes. This is how you build features on top of features rather than just working from the ground plane.

**Patterning:** Linear pattern repeats a feature along a line (row of holes). Circular pattern repeats around an axis (bolt circle). Mirror duplicates features across a plane of symmetry.

That covers probably 90% of what you'd do for mechanical parts like pump mounts, brackets, housings, and knob enclosures. The remaining 10% is things like threads, text embossing, and imported geometry — useful but not where you'd start.

---

## Derek

Interesting, chamfer is a word I've heard in woodworking. Some of the language inherits from the pre computer days?

## Claude

Yeah, most of it does. CAD software didn't invent a new vocabulary — it formalized and digitized operations that machinists, woodworkers, and patternmakers had been doing by hand for centuries.

Chamfer, fillet, bore, counterbore, countersink, draft, kerf — these all predate computers by a long time. A machinist in 1950 knew exactly what a fillet was (a rounded interior corner) and what a chamfer was (an angled cut on an edge) because they were making them with files, lathes, and mills. When CAD software came along, it just parametrized those operations — instead of "eyeball it and file until it looks right," you specify a radius or an angle numerically.

"Extrude" has a physical analog — it comes from manufacturing, where you literally push material through a shaped die to create a continuous cross-section (think aluminum extrusions, pasta makers). The CAD operation is the same concept: take a 2D shape, push it through space to create a 3D solid.

"Boolean" is the exception — that's from mathematics (George Boole). But even boolean operations have physical intuition: union is gluing two things together, subtract is cutting one thing out of another, intersect is asking "where do these two things overlap?"

This is actually good news for you. You already have woodworking intuition about what these operations *mean physically*, which is a head start over someone who's only ever encountered them as abstract software features. When OnShape asks you to define a fillet radius, you already know what that's going to look like on the part — you've seen rounded edges on real objects. That physical grounding is exactly the kind of knowledge that makes your descriptions to AI more precise.

It's another instance of that pattern you've noticed across your projects — the domain knowledge is the thing that matters, and the tools are just different interfaces to the same underlying concepts.

---

## Derek

If fillet is an inner round edge, what is an outer round edge? Also, what does bevel mean in these contexts?

## Claude

Fillet actually covers both — I was imprecise. In CAD, fillet just means rounding any edge to a specified radius, inner or outer. The distinction I was gesturing at is more from traditional woodworking/machining terminology where fillet sometimes specifically meant a concave (interior) rounded transition, but in CadQuery and OnShape, you just select an edge and apply a fillet and it handles either case.

So if you fillet the top edges of a box, you get rounded outer edges. If you fillet the interior corner where a rib meets a wall, you get a rounded inner transition. Same operation, same command.

As for bevel — it's essentially synonymous with chamfer in this context. Both mean cutting an edge at an angle rather than rounding it. In woodworking, there's a traditional distinction: a chamfer is an angled cut on an edge (usually 45°), while a bevel is an angled cut across the full face of a piece. But in CAD, you'll mostly see "chamfer" as the operation name, and "bevel" shows up more in mesh-based 3D modeling tools like Blender where you're working with polygons rather than solids.

So practically: if you're in OnShape or CadQuery and you want an angled edge, you're looking for chamfer. If you're in Blender, the same visual result is called a bevel. Different tool lineages, different naming conventions for the same geometric idea.

---

## Derek

Okay. I appreciate the categorization you did in paragraphs there, but I do want like a list with the keywords in bold and short definitions that follow. It can be a long list.

## Claude

**Sketch** — a 2D drawing (lines, arcs, circles, splines) on a flat plane that defines the profile for a 3D operation.

**Workplane** — the flat reference plane you sketch on. Can be the base XY/XZ/YZ planes, or any face of an existing solid.

**Extrude** — push a 2D sketch straight out into 3D by a specified distance.

**Cut extrude** — same as extrude but removes material instead of adding it.

**Revolve** — spin a 2D sketch around an axis to create a 3D solid (how you'd make a knob, bottle, or anything rotationally symmetric).

**Sweep** — move a 2D profile along a path/curve to create a 3D shape (tubes, rails, channels).

**Loft** — connect two or more 2D profiles at different positions into a smooth transitional solid (think a shape that morphs from a circle at the bottom to a rectangle at the top).

**Fillet** — round an edge to a specified radius, inner or outer.

**Chamfer** — cut an edge at an angle instead of rounding it. Essentially synonymous with bevel.

**Shell** — hollow out a solid, leaving walls of a specified thickness. You typically select one face to remove (the opening).

**Draft** — tilt faces by a small angle relative to a direction. Critical for injection-molded parts so they release from the mold.

**Union** — combine two solids into one merged body.

**Subtract/Cut** — remove one solid's volume from another.

**Intersect** — keep only the volume where two solids overlap.

**Through hole** — a hole that goes completely through a body.

**Blind hole** — a hole that goes only partway into a body to a specified depth.

**Counterbore** — a stepped hole: a wider shallow cylinder on top, narrower hole below. Lets a bolt head sit flush or recessed.

**Countersink** — a conical taper at the top of a hole. Lets a flat-head screw sit flush with the surface.

**Pocket** — a recessed area cut into a face, like a shallow rectangular or shaped cavity.

**Rib** — a thin wall added to a part for structural reinforcement, usually connecting two faces.

**Boss** — a raised cylindrical feature, typically where a screw or pin will go.

**Linear pattern** — repeat a feature at equal spacing along a straight line.

**Circular pattern** — repeat a feature at equal spacing around an axis.

**Mirror** — duplicate features reflected across a symmetry plane.

**Offset** — create a new sketch or plane at a specified distance from an existing one.

**Constraint** — a rule applied to sketch geometry: parallel, perpendicular, tangent, coincident, concentric, equal length, fixed distance, etc. This is what makes sketches parametric rather than just freehand.

**Dimension** — a specific numeric value assigned to a length, angle, or radius in a sketch. Changing the dimension updates everything that depends on it.

**Spline** — a smooth freeform curve defined by control points. Used when arcs and lines aren't enough to describe the shape you want.

**Thread** — helical grooves cut into or around a cylindrical surface. Can be cosmetic (visual only) or modeled (actual geometry for 3D printing).

**Emboss/Engrave** — raise or cut text or a logo into a face.

**Plane** — an infinite flat reference surface used for sketching, mirroring, or sectioning.

**Axis** — an infinite reference line used for revolves, circular patterns, or measurements.

**Assembly** — combining multiple individual parts together with defined relationships (this bolt goes in this hole, this face sits against that face).

**Mate/Constraint (assembly)** — a rule defining how two parts relate in an assembly: fastened, revolute (can spin), slider (can translate), etc.

**Section view** — slice through a part with a plane to see the interior geometry.

**BOM (Bill of Materials)** — a list of all parts in an assembly with quantities. Generated from the model, not maintained separately.

---

## Derek

Great. This is helpful for one direction of my problem. The other, bigger problem, is I have no idea how to go from the description I gave of what the cartridge does (please look up the pump replacement cartridge we discussed) and rough analogs of "printer inkjet replacement" and "bicycle wheel lock" to an actually good well thought out idea of the shape of things.

I think there is AI knowledge I can type into here, but I don't have the right language to even begin to ask my questions, it's just "help me solve this problem" and that sees to fail to enlist the mechanical engineering friend I need, that understands what I need, and has a breadth of understanding of the field of solutions and what all my options are.

## Claude

Let me find our previous discussions about the cartridge design.

*Searched memory*

Okay, I've pulled up our conversation history. Here's what I have from our discussions:

The cartridge concept involves a replaceable module — a pump + bag assembly — that a consumer can swap out, with analogies to an inkjet printer cartridge (slides in, makes its connections, done) and a bicycle wheel quick-release (a cam lever that, when you close it, pushes on the quick-connect tubing collars to seat them). The dock has rails the cartridge slides into, and the cam mechanism handles the fluid connections mechanically so the user never touches tubing fittings.

Now to your actual question — which is really: "I know *what* the thing needs to do, but I don't know *what shape* it should be, and I don't even know what questions to ask to figure that out."

This is an industrial design and mechanism design problem, and you're right that it's a different discipline than the software and electronics work you've been doing. The challenge isn't that you lack intelligence or creativity — it's that mechanism design has its own vocabulary of *solution patterns* that you haven't been exposed to, and without knowing those patterns exist, you can't ask for them by name.

Here's what I mean. When a mechanical engineer hears "replaceable module that slides in and makes fluid connections," they're immediately thinking in terms of specific known mechanisms: dovetail slides vs. rail-and-groove vs. drawer slides for the insertion path; cam-actuated vs. over-center toggle vs. bayonet twist for the locking/engagement; push-to-connect vs. dry-break couplings vs. face-seal ports for the fluid connections; and detent balls or spring clips for the "you're fully seated" feedback.

Each of those is a well-understood mechanism with known tradeoffs in force, tolerance, reliability, and manufacturability. The reason "help me solve this problem" falls flat with AI is that without specifying which of these mechanism families you're exploring, the design space is enormous and the AI has no way to narrow it.

So the productive way to use AI here might be more structured than open-ended. Something like:

**Phase 1: Catalog the functional requirements.** Not shape, not mechanism — just what has to happen. "The cartridge must align to the dock within ±1mm. Two fluid connections must seal leak-free. The user must feel a positive 'locked' indication. Insertion and removal must be one-handed. The cam must push two quick-connect collars simultaneously." You already know most of this.

**Phase 2: For each requirement, ask AI to show you the mechanism families.** "What are the standard mechanical approaches to aligning a removable module into a dock?" — and you'll get answers about tapered pins, V-grooves, kinematic couplings, rail systems, and so on. Each of those is a searchable term that opens up a whole world of prior art.

**Phase 3: Pick a mechanism family for each requirement, then sketch.** This is where OnShape comes in. You don't need to design the final part — you need to prove to yourself that the chosen mechanisms can physically coexist in the space you have.

**Phase 4: AI generates the parametric model** once you can describe it in terms of those mechanism choices.

Want to start with Phase 1 — nailing down the functional requirements list — or Phase 2, where I walk you through the mechanism families for the cartridge-and-dock problem specifically?

---

## Derek

Yes.

On the cartridge mechanism. It is just 2 pumps. The bag is a separate maintenance cycle, if any. There is a clean cycle already for its main problem, and I anticipate its wear lifetime to be similar to the tubing, which means basically the whole device needs replaced.

The 2 pumps though means 4 quick connects. And we will be pushing the collars on the quick connects to allow the release.

What else can you remember about it? What questions do you have?

## Claude

Let me check what else we've discussed about the specifics.

*Searched memory*

Okay. Here's what I've got from our history, and then my questions.

**What I know about the cartridge:**

The cartridge contains 2 Kamoer peristaltic pumps (one per flavor line). It slides into a dock on rails. A cam lever mechanism — the bicycle wheel quick-release analogy — actuates to push 4 quick-connect collars simultaneously (2 pumps × inlet + outlet each), making/breaking the fluid connections. The user interaction is: slide it in, close the lever, done. Open the lever, slide it out to replace. The pumps are driven by L298N motor drivers at 12V, controlled by the ESP32 via PWM. The tubing is 1/8" ID silicone on the current prototype.

The cartridge doesn't include the bag — the bag is part of the permanent installation with its own maintenance cycle (hopper fill, clean cycle via tap water). So the cartridge is purely the pump module.

**What I don't know / my questions:**

How does power get to the pumps when the cartridge is docked? The current prototype has everything hardwired. A replaceable cartridge needs some kind of electrical connection that mates when you insert it — either spring-loaded pogo pins, a blade connector, an edge connector, or something else. That's 4 wires minimum (2 motor power lines × 2 pumps), possibly more if you want any feedback signals. Have you thought about that?

What are the quick-connect fittings specifically? Are these the standard push-to-connect pneumatic/water fittings where you push tubing in and it grips, and pushing the collar releases? Or something else? The cam needs to push those collars in a specific direction with a specific amount of travel — knowing the exact fitting matters for the mechanism geometry.

How are the two pumps oriented relative to each other inside the cartridge? Side by side? Stacked? That determines the cartridge's footprint and where the 4 fluid ports end up on the mating face.

What's the insertion direction relative to the under-sink space? Front-to-back, side-to-side, top-down? This affects what the user is physically doing while crouching in a cabinet.

And maybe most importantly — do you have the quick-connect fittings in hand yet, or are those still to be selected? Because the entire cam mechanism geometry derives from the specific fitting's collar travel distance and release force.

---

## Derek

I have no idea on the electrical connects, but from the cartridge to the thing it docks into it need only be 3 contacts, one for ground and the other for 12V supply to each.

The L298Ns will be in the main body and not replaced with the cartridges.

Yes, the quick connects are the standard 1/4 type as you describe used to connect everyone's ice cube maker and reverse osmosis machines. They traditionally have a little blue lock that prevents pushing the collar accidentally, but my plan is to simply not have those present.

The pumps can be oriented any way we like, and we can run the 1/4 tubing however we like inside the carry ridge, so I thought the 4 tubes would simply all be next to each other with not much space between them - but the door is wide open to possibilities here.

My thought on user orientation was insert from the front, but it could be set it into the top corner or insert into the middle or something else - I am open to suggestions there too.

I have quick connects in hand, plenty of them, and they seemed like an ingenious way to solve this problem simply because they do water tight lock so well and so easily with a human hand, but if this is a dead end and there's prior art on a better solution I can actually use, I am game for that too.

## Claude

Good, this is getting concrete. Let me make sure I understand the quick-connect mechanics precisely, since the whole cam design flows from that.

*Searched the web*

Good. Now I have a clear enough picture. Let me lay out what I understand and then the real questions.

**The mechanism in concrete terms:** The John Guest-style 1/4" push-to-connect fittings work by pushing the collet (the ring/collar at the face of the fitting) inward, flush against the fitting body. That releases the stainless steel teeth gripping the tubing, allowing it to slide out. The collet travel is small — roughly 2-3mm of inward push. The force is light, maybe a pound or two per fitting. So your cam needs to produce about 2-3mm of linear travel on 4 collars simultaneously, with modest force. That's very achievable.

Now, here's where I want to reframe the problem for you, because I think you're closer to having the right language than you realize.

The thing you're actually designing isn't one mechanism — it's a sequence of three sub-problems that happen during insertion and the reverse during removal:

**1. Guide and align** — the cartridge needs to find its position in the dock reliably, even when you're crouching in a dark cabinet. This is the "rails" part. The tubing stubs on the cartridge need to line up with the fittings in the dock within a millimeter or so.

**2. Seat the fluid connections** — the 4 tubes on the cartridge push into the 4 John Guest fittings in the dock. This can happen as part of the slide-in motion itself if the fittings are at the back wall of the dock facing the insertion direction. You push the cartridge in, the tubes bottom out in the fittings, done. No cam needed for *making* the connection — John Guest fittings latch automatically on insertion.

**3. Lock and provide release capability** — the cam's job isn't actually to *make* the connections. It's to *unmake* them. The cam lever, when opened, pushes the 4 collets to release the tubes so you can slide the cartridge out. When closed, it just holds the cartridge seated and stays out of the way.

This is an important reframe. Your cam is a *release mechanism*, not a *connection mechanism*. The fittings do the connecting for free on push-in. The cam only works on removal.

Does that match your mental model, or were you imagining the cam doing something on both insertion and removal?

And the electrical question: 3 contacts (ground, motor A 12V, motor B 12V) is clean. Pogo pins or blade connectors that mate during the last few millimeters of insertion are the standard approach — that's exactly how laptop docking stations and battery packs work. The contacts on the cartridge side could just be exposed copper pads, and the dock side has spring-loaded pins that press against them. We can design those into the mating face alongside the tube stubs.

---

## Derek

That matches my mental model.

## Claude

Good. So let me state the functional requirements cleanly, and then we can move to mechanism families for each.

**Functional Requirements:**

1. The cartridge slides into the dock along rails, guided so that 4 tube stubs align with 4 John Guest 1/4" fittings within ~1mm tolerance.
2. The final push of insertion seats the tubes into the fittings. The fittings latch automatically. No mechanism needed.
3. Three electrical contacts (ground, motor A, motor B) mate during the last portion of insertion via spring-loaded pins or similar.
4. A lever or latch locks the cartridge in the fully seated position so it can't drift out.
5. To remove: actuating that same lever pushes 4 collets inward ~2-3mm simultaneously, releasing the tube grip, and the user slides the cartridge out.
6. The user should feel a clear "locked" and "released" state — no ambiguity about whether it's seated.
7. One-handed operation is ideal given the under-sink context.
8. Front insertion into the dock, user facing the front of the unit.

Now here are the mechanism questions worth thinking about, in the order that constrains everything else:

**The mating face layout.** Where do the 4 tubes and 3 electrical contacts sit relative to each other on the face where the cartridge meets the dock? This is the single most constraining decision. If the 4 tubes are in a line, the cam can be a simple bar that sweeps across all 4 collets. If they're in a square pattern, you need a different actuator geometry. The electrical contacts need to be far enough from the fluid connections that moisture never reaches them.

**The cam geometry.** A bicycle QR skewer works by rotating an eccentric cam lobe that converts rotation into ~3-4mm of linear displacement. Your collets need ~2-3mm. A single lever that rotates a bar or plate which presses all 4 collets simultaneously is the simplest version. The lever could be on the top of the cartridge (flip up to release, push down to lock), on the side, or on the front face. Top is probably most accessible under a sink.

**The rail profile.** This determines how forgiving the insertion is. A tapered lead-in (wider at the entrance, narrowing to final position) means you can be sloppy with initial alignment and the cartridge self-centers as you push it in. Think about how a desk drawer or a server blade chassis works — you roughly aim it and the last inch does the precision work.

The thing I'd suggest you do with the quick-connect fittings you have in hand right now, before any CAD work: take 4 of them and arrange them on a flat surface. Try different spacings and patterns — a row of 4, a 2×2 grid, two pairs. Push some tubing stubs in and practice pressing the collets with a flat object like a ruler edge. Get a feel for how much force and travel you actually need, and how precisely aligned the flat bar needs to be to hit all 4 collets evenly.

That physical experimentation will tell you more about the cam geometry in 20 minutes than any amount of modeling. Once you know the pattern and spacing that feels right, *that's* when CAD and AI become useful — because you'll be able to say "4 fittings in a row, 25mm center-to-center, cam bar needs 2.5mm travel" and that's specific enough to generate a real design.

Want to talk through the mating face layout options, or do you want to go play with fittings first?

---

## Derek

I think I need at least a precise plate to play with. And maybe that is what I "design" and print first. Because a ruler will only press on one side and the collars must be pushed from at least two sides. Do you know what I mean?

Maybe I drill some holes in a ruler ... the holes need to be very precisely sized though.

## Claude

Yes, I know exactly what you mean. The collet is a ring around the tube. Pressing it from one side with a flat edge would cock it sideways instead of pushing it straight in. You need to press it evenly all the way around — or at least from opposing sides — which means the release mechanism needs to be a hole that's just slightly larger than the tube but smaller than the collet's outer diameter, so when the plate slides forward it pushes the collet inward concentrically.

This is actually how the John Guest release tools work — they're basically a shaped ring or clip that fits around the tube and presses the collet evenly.

So your release plate is a flat plate with 4 precisely sized holes. Each hole clears the 1/4" OD tube (so maybe 7-8mm diameter) but catches the collet rim and pushes it inward as the plate moves toward the fitting face. The cam moves this plate ~2-3mm toward the fittings.

And yes — this is a perfect first print. It's small, it tests your dimensional accuracy (the hole diameters matter to within maybe half a millimeter), and it directly informs the rest of the cartridge design. If the holes are too big, they won't catch the collet. Too small, the tube won't pass through. You'll probably print 3 or 4 iterations dialing in the fit, and that's exactly how you should be learning the printer.

What you'd want to measure with calipers before designing it: the OD of the tube (nominally 1/4" / 6.35mm), the outer diameter of the collet ring on your specific fittings, and the inner diameter where the collet meets the fitting body. The hole in your plate needs to be between the tube OD and the collet OD — that's your working window.

This is a good first CAD exercise too. It's literally a rectangle with 4 holes in it — sketch, extrude, linear pattern of holes. Uses maybe 3 operations from that list we built.
