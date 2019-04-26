\mainpage

#  Motivation

GROG  was  written  as  an  personal  challenge  to  implement  a  complete  3D  rendering  library  for  the  ARM  Cortex-M0+  chip,  i.e.  limited  to  32kB  of  RAM,  and  leave  enough  resources  (memory,  CPU)  for  integration  in  a  Gamebuino  Meta  game.

#  Design choices

* The  frame  buffer  must  fit  in  the  32kB  of  RAM,  while  being  able  to  display  something  interesting  on  the  Gamebuino  Meta  screen.
  * The  full  resolution  is  160*128,  meaning 20k pixels. The rasterization algorithms process the whole image at once for efficiency, so rendering slices synchronized with DMA would be inefficient: all triangles would have to be processed for each slice, which is a waste of cycles. And cycles are precious resources!
  * So we'll work in quarter resolution: 80*64. Which is [naturally provided by the Meta SDK](https://github.com/Gamebuino/Gamebuino-META/blob/master/src/utility/Graphics/Image.h#L127), how lucky of us! 80*64 is still 5k pixels, which can be a big deal. Let's make it indexed on a 4-bit palette (16 colors possible per pixel) and we can cram the whole frame buffer in 2.5kB.
  * **Summary:** Target frame buffer: 80*64, 4-bit palettized
* ARM Cortex M0+ architectures are simple architectures designed for 32-bit processing.
  * No `double`, no `uint64_t`. By the way, I have measured that integer computation was way faster than floating point computation. And gcc on ARM is nice with us, as right-shifting signed integers keeps the sign information, so we can have [cheap integer dividing by powers of two](https://www.geeksforgeeks.org/left-shift-right-shift-operators-c-cpp/).
  * **Design choice:** Work with fixed-point arithmetics. The base unit in GROG is [signed Q21.10](https://en.wikipedia.org/wiki/Q_%28number_format%29) (sign bit + 21 integer bits + 10 fractional bits).
  * If the unit is a meter, we can design objects with precision ranging from ~1mm (lowest fractional bit set to 1) to ~32m (lower 5 integer bits + 10 lowest fractional bits). But *why can't we go up to 21 integer bits?*. Because we'll have multiplies coming in, and multiplying numbers with more than 16 bits can overflow. Adapt your scales with this in mind!
* **API design**: loosely  based  on  pre-existing  3D  API  like  OpenGL  (the  pre-shaders  versions!).
* Coding the library was done with the following **rules of thumb** in mind:
  * Adding, subtracting, shifting is cheap. Multiplying is more expensive. Dividing is evil. Advanced math functions should be avoided where possible.
  * Function calls can be expensive in tight loops (i.e. in the rasterizer). Inline. And don't give the compiler a choice. In this specific case, the code size inflating is acceptable.
  * Call me paranoid, but I'm not sure I should trust branch prediction on a simple processor like this one, and I am not sure of the cost of a branch miss. Where relevant, hint the processor with branch probability. I know this is bad, as I have not measured it, and this could as well be placebo.

# Graphics Pipeline

# API usage
