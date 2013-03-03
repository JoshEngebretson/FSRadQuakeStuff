These files represent the process that the lightmap packer goes through, when packing a single
complex polygon. The complex polygon is shown in white. The colored regions show the "occupied
regions" (i.e. those regions that have been allocated in the lightmap.) You'll notice that the
black regions are one pixel larger than the white regions -- this is for a single-pixel border
on all sides of the individual pieces of the complex polygon.

THe colored regions show the available regions -- regions of the lightmap that are available
for future polygons to be packed into them.

I recently sent out an email to Marco Koegler that explains the lightmap packing algorithm as
well as how to properly handle polygon edges to avoid bilinear filtering problems. The
following is the text of that email. Marco's comments are blocked...

---------------------------------------------------------------------------------
> Another thing which caught my eye was the recursive lightmap packer.
> That's something I would love to hear more about. You are inserting
> complex polygons ... our lightmap packer only did convex polys,
> splitting them if they wouldn't fit into the lightmap. We had some
> optimizations which filled in gaps with smaller polygons and other
> optimizations which grouped equal materials on the same lightmap. So I
> always started by placing 'big' polys first and then fill it up with
> small polys. It's tough to find a compromise between the optimizations
> and end up using lightmap space wisely. If I would have had more time,
> I would have gone with a structure which would have encoded filled and
> unfilled 'regions', but this would have only decreased search times, I
> think.
---------------------------------------------------------------------------------

It's actually much simpler than it sounds. I came up with the idea and
coded it in about 2 hours. Suprised at how well it worked. I think I got
lucky. :)

preface: "Complex polygon" is a bad term. As I was coding it, I realized
that I didn't actually need to use complex polygons, so I renamed the
class to "CombinedPoly". So don't think "complex polygon" while reading
the following description, or you might get confused.

The basic idea is this...

You start by sorting all polygons by their plane equation's D value. You
then go through that sorted list and group them by D and by normals. This
allows you to quickly locate all polygons that lie on the same plane. The
next step is to visit each of these "same-plane" groups of polygons and
locate those polygons that are connected.

This is where the term "Complex polygon" is the wrong term. When searching
for connections between polygons, do _not_ connect by edge, but rather by
vertex. Therefore, any polygon sharing even a single vertex with another
polygon is considered "connected". This is important, because these
polygons will all be connected within the lightmap, and since the
lightmaps are applied with bilinear filtering, a single shared vertex will
allow bilinear filtering from one polygon to the next. If you don't do
this, you'll end up with a seam where the polygons meet, even if they only
meet at a single vertex.

Note that three polygons (A, B, C) might be connected, even if A and C do
not share any vertices. Rather, the connection is made through B (both A
and C share at least one vertex with B). So the process of locating
connected polygons must be iterative.

You may also want to limit these connections in other ways. For myself, I
disallow connections between polygons that do not share the same light
emission values -- otherwise, a continuous surface (made of multiple
polygons, some of which emit light) will not be blurred together. If you
look at the IOTD, the ceiling of the cornell room is one example. That is
a series of polygons, with a hole cut out of the ceiling where the light
is. Without this limitation, the light source's light would "blend" into
the ceiling, rather than have a harsh edge defining the light itself.

At this point, you should have a series of groups of polygons, all of
which are in some way connected, and share the same plane. At this point,
you generate the light-mapping coordinates for these polygons. I use a
world-space, major axis-aligned planar mapping scheme (very common.)

Also, it's a good idea to store a bounding box of your mapping coordinates
for an entire combined polygon in UV space (i.e. take the min/max UV of
all polygons in the group.) You'll need this later.

I'll refer to a single group of polygons as a single "combined polygon"
(really, it's just an array of polygon pointers.)

Once you've completed all of this, you've completed the first of three
steps. The second step is to clip these "combined polygon" groups.
Clipping is only necessary when a combined polygon extends beyond the
range of a single lightmap's dimensions. I won't go into detail about
this, assuming you've got experience in this already.

The final step, is to populate the lightmaps. This is the recursive
procedure that performs the region fitting. And it's a little tricky, but
it's not as bad as it seems...

The simplest way to explain it, is to start by explaining how you populate
the lightmap without filling in the holes (I'll explain the rest shortly.)

You enter the recusive procedure with a set of coordinates (minU, minV,
maxU, maxV) for the available region of the lightmap. This should start
off as (0, 0, 128, 128) for a 128x128 lightmap. You then determine if the
current region is primarily horizontal or primarily vertical, like this:

        bool primHoriz = (maxU-minU) >= (maxV-minV);

You then visit each of the combined polygons, and determine if it is
primarily horizontal or primarily vertical. If the orientations don't
match, rotate your combined polygon's UVs by 90-degrees so they do match.
If the combined polygon will fit within the lightmap, calculate its area
and retain it. You want to find the combined polygon with the most area
that will fit. Much like you were doing. This is called a "Greedy
algorithm".

Once you've found the largest combined polygon that fits, mark it as
"done" (so you can skip it later) and re-map it so that its UVs are offset
properly into the region of the lightmap that it belongs in.

Say, you've just insterted a compbined polygon (100x50) into a 128x128
lightmap. This goes in the upper-left corner. There is now an L-shaped
portion of the lightmap that is still unused (the bottom 78 rows, and the
right-hand 28 columns). Figure out which is larger. In this case, the
region at the bottom (128x78) is larger than the region to the right
(28x128). So we'll split it up that way. This gives us a region at the
bottom that is 128x78, and a region to the right bottom that is 28x50.

We'll recursively call this routine with these values (largest region
first):

   call 1: 0, 50, 128, 128
   call 2: 100, 0, 128, 50

This gets us maxumum use of the area for rectangular regions only...
here's how you modify that to fill in the gaps. By the way.. I lied in the
IOTD... I forgot that I removed the recursion when I modified the code to
handle filling in the gaps. :)

Rather than calling the routine recursively, I create a list of "available
regions" -- just a list of rectangles. I initialize this list with the
entire lightmap's rect. I locate the best fit (largest area, properly
oriented complex polygon) just like I explained earlier. This doesn't
change.

Once you've got the best fit complex polygon, you visit each of the
individual polygons (in that complex polygon) and start marking regions of
the lightmap occupied with them, rather than with the entire bounding rect
of the entire complex polygon. This isn't as simple as it sounds... I did
this in a very specific way in order to maximize usage. For the sake of
simplicity, I created another list of rects; this list contains the
bounding rects of the individual polygons within the complex poly. We'll
call this "Occupied rects".

I'm not done creating lists yet. :) There's another list, called "Working
rects"  -- this list is primed with the single rect that we're currently
working with (i.e. the rect of the best fit complex polygon.)

The idea is to go through the working rect and subtract the occupied
rects. This will work much like a boolean operation (but for rects, this
is simple.) Each time we subtract a single rect from the working rects
list, we'll find we may have more pieces then we started with, so add them
to the working rects list. After each rect in the "occupied rects" list
has been subtracted from the "working rects" list, we delete it from the
list. It is done.

After this process, you may find that you still have some "working rects"
remaining. These are the pieces that are still available. Add them to the
"Available rects" list (our original list.)

Three lists of rects.. confused? :) Anyway, there are still a couple
details I should mention.

When doing that boolean subtraction (where we subtract an "occupied rect"
from the "working rects"), it's important that this be done in a specific
order. You don't want to subtract the smallest first, or the largest
first. You want to perform the boolean operation of one occupied rect and
one working rect, and look at the result. Find the largest piece left over
and calculate it's area. You'll need to go through all the occupied rects
(for each working rect) and find the one that, when booleaned, leaves you
with the largest piece. Once you've found that "best subtraction", go with
that. To extend this concept even further, when you calculate a boolean
result, you'll be slicing up one rect by another. Much like before (when
we calculated the largest portion of an available region) -- you want to
do the same when doing the boolean. This is a bit confusing, but I think
you got the idea. Again, this is to maximize usage.

The last detail is another way to maximize usage...

Once you've done all the subtraction, and you're left with (potentially) a
few working rects, you'll be adding them to the available list. Doing so,
you might find a few available rects that can be merged. You'll want to do
this. :) Again, be sure to merge those rects which result in the largest
area first. Continue doing this iteratively until no more rects can be
merged.

At this point, you've reached the bottom of your loop. You've allocated
enough lightmap space for a single complex polygon, marked it as "done",
subtracted just the used pieces out of the available lightmap space, and
returned some of that used space back to the lightmap for further
allocation. Time to step back up to the top of the loop and repeat the
process for the next "best fit" complex polygon.

I didn't mention pixel gaps in here.. just make sure that the rects you
calculate from your your individual polygons (and the complex polygons)
include a single pixel gap on all sides. With this, you'll end up with a
two-pixel gap between two neighboring complex polygons within the
lightmap -- just enough room.

---------------------------------------------------------------------------------
> Last but not least, there is the issue of the pixel border. Psykotic
> and I already talked a lot about this in the Flipcode forum.
> Basically, I would solve this semi-pragmatically. For shared coplanar
> neighboring edges, you sample the neighboring world space and for the
> non-coplanar case you can probably get away by just duplicating the
> intensities, rather than sampling the neighbor.
---------------------------------------------------------------------------------

Yikes! I got a better one for ya. :)

First, since all connected polygons are, well, "connected" :) -- in the
lightmap -- edges between neighboring polygons don't matter. And remember,
we've merged polygons by vertices, not edges, so we've even got rid of a
few more potential bilinear filtering issues.

But what about those nasty dark edges you get when the edge of a polygon
tries to bilinear filter into the next lumel over (i.e. a black, unused
pixel in the lightmap.) I solved this quite simply.

Initialize your lightmaps with a specific color (like chroma-key). After
you've done your lighting and placed the light into the lightmaps, you'll
be left with lightmaps that have chroma-key colored pixels wherever a
polygon is absent (this must be pixel-accurate.)

Loop through your lightmaps, and if you find a chroma-key color, locate
those neighboring pixels that are _not_ keyed colors. Average them and
replace the current pixel. Make sure you do this like a low-pass filter,
where you don't work on the current image, but read from one copy and
write to another copy. This will expand all of your edges out by exactly
one pixel in all directions, guaranteeing you no edges.
