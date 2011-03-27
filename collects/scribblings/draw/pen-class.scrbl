#lang scribble/doc
@(require "common.ss")

@defclass/title[pen% object% ()]{

A pen is a drawing tool with a color, width, and style. A pen draws
 lines and outlines, such as the outline of a rectangle. In a
 monochrome destination, all non-white pens are drawn as black.

In addition to its color, width, and style, a pen can have a stipple
 bitmap. Painting with a stipple pen is similar to
 calling @method[dc<%> draw-bitmap] with the stipple bitmap in region
 painted by the pen.

A pen's style is one of the following:

@itemize[

 @item{@indexed-scheme['transparent] --- Draws with no effect (on the
       outline of the drawn shape).}

 @item{@indexed-scheme['solid] --- Draws using the pen's color. If a
        (monochrome) stipple is installed into the pen, black pixels
        from the stipple are transferred to the destination using the
        brush's color, and white pixels from the stipple are not
        transferred.}

 @item{@indexed-scheme['xor] --- The same as @racket['solid], accepted 
        only for partial backward compatibility.}

 @item{@indexed-scheme['hilite] --- Draws with black and a @racket[0.3] alpha.}

 @item{The following special pen modes use the pen's color, and they only
       apply when a stipple is not used:
    @itemize[
  @item{@indexed-scheme['dot]}
  @item{@indexed-scheme['long-dash]}
  @item{@indexed-scheme['short-dash]}
  @item{@indexed-scheme['dot-dash]}
  @item{@indexed-scheme['xor-dot]}
  @item{@indexed-scheme['xor-long-dash]}
  @item{@indexed-scheme['xor-short-dash]}
  @item{@indexed-scheme['xor-dot-dash]}
  ]}

]

To avoid creating multiple pens with the same characteristics, use the
 global @scheme[pen-list%] object @indexed-scheme[the-pen-list], or
 provide a color, width, and style to @xmethod[dc<%> set-pen].

When drawing in @racket['smoothed] or @racket['aligned] mode, a pen's
 size is truncated after scaling to an integral size. A pen of size
 @scheme[0] (after truncation, if applicable) uses a non-zero,
 scale-insensitive line size for the destination drawing context:
 @racket[1/4] unit (after scaling) for @racket[post-script-dc%] or
 @racket[pdf-dc%] contexts in @racket['smoothed] mode, or @racket[1]
 unit (after scaling) for any other context.  For example, in unscaled 
 canvas and bitmap contexts, a zero-width pen behaves the same as a
 pen of size @scheme[1].


@defconstructor[([color (or/c string? (is-a?/c color%)) "black"]
                 [width (real-in 0 255) 0]
                 [style (one-of/c 'transparent 'solid 'xor 'hilite 
                                  'dot 'long-dash 'short-dash 'dot-dash 
                                  'xor-dot 'xor-long-dash 'xor-short-dash 
                                  'xor-dot-dash)
                        'solid]
                 [cap (one-of/c 'round 'projecting 'butt)
                      'round]
                 [join (one-of/c 'round 'bevel 'miter)
                       'round]
                 [stipple (or/c #f (is-a?/c bitmap%)) 
                          #f])]{

Creates a pen with the given color, width, style, cap style (see
 @method[pen% get-cap]), join style (see @method[pen% get-join]), and
 stipple.  For the case that the color is specified using a name, see
 @scheme[color-database<%>] for information about color names; if the
 name is not known, the pen's color is black.

}

@defmethod[(get-cap)
           (one-of/c 'round 'projecting 'butt)]{

Returns the pen cap style, which determines the shape of a line at
each of its ending points when drawn by @method[dc<%> draw-line] or at the
non-connecting ends of lines when drawn by @method[dc<%> draw-lines] or
@method[dc<%> draw-path]. The default is @scheme['round], which draws the
end of a line as a semi-circle. The @racket['projecting] style draws a
square in place of the semi-circle (i.e., past the point at which the
line stops). The @racket['butt] style ends the line with a straight
edge, instead of projecting past the ending point of the line.

}

@defmethod[(get-color)
           (is-a?/c color%)]{

Returns the pen's color object.

}

@defmethod[(get-join)
           (one-of/c 'round 'bevel 'miter)]{

Returns the pen join style that is used between multiple lines
connected through @method[dc<%> draw-lines], @method[dc<%>
draw-rectangle], @method[dc<%> draw-polygon], or @method[dc<%>
draw-path].  The join style fills the space that would be left at the
outside corner of two lines if they were draw separately with
@racket['butt] line endings. The default join style is
@scheme['round], which fills under an arc that lines up with the
outside of each of the two lines. The @racket['bevel] style fills in
the gap without adding extra pixels (i.e., it makes a blunt
corner). The @racket['miter] style fills the gap by adding pixels that
would be covered by both lines if they were extended past the corner
(i.e., it makes a sharp corner).}


@defmethod[(get-stipple)
           (or/c (is-a?/c bitmap%) #f)]{

Gets the current stipple bitmap, or returns @scheme[#f] if no stipple
 bitmap is installed.

}

@defmethod[(get-style)
           (one-of/c 'transparent 'solid 'xor 'hilite 
                     'dot 'long-dash 'short-dash 'dot-dash 
                     'xor-dot 'xor-long-dash 'xor-short-dash 
                     'xor-dot-dash)]{

Returns the pen style. See @scheme[pen%] for information about
possible styles.

}

@defmethod[(get-width)
           (real-in 0 255)]{

Returns the pen width.

}

@defmethod[(set-cap [cap-style (one-of/c 'round 'projecting 'butt)])
           void?]{

Sets the pen cap style. See @method[pen% get-cap] for information about cap
 styles.

A pen cannot be modified if it was obtained from a @scheme[pen-list%]
 or while it is selected into a drawing context.

}

@defmethod*[([(set-color [color (is-a?/c color%)])
              void?]
             [(set-color [color-name string?])
              void?]
             [(set-color [red (integer-in 0 255)]
                         [green (integer-in 0 255)]
                         [blue (integer-in 0 255)])
              void?])]{

Sets the pen color.

A pen cannot be modified if it was obtained from a
 @scheme[pen-list%] or while it is selected into a drawing context.

}

@defmethod[(set-join [join-style (one-of/c 'round 'bevel 'miter)])
           void?]{

Sets the pen join style. See @method[pen% get-join] for information about join
 styles.

A pen cannot be modified if it was obtained from a
 @scheme[pen-list%] or while it is selected into a drawing context.

}

@defmethod[(set-stipple [bitmap (or/c (is-a?/c bitmap%) #f)])
           void?]{

Sets the pen stipple bitmap, where @scheme[#f] turns off the stipple bitmap.

If @racket[bitmap] is modified while is associated with a pen, the
 effect on the pen is unspecified. A pen cannot be modified if it was
 obtained from a @scheme[pen-list%] or while it is selected into a
 drawing context.

}

@defmethod[(set-style [style (one-of/c 'transparent 'solid 'xor 'hilite 
                                       'dot 'long-dash 'short-dash 'dot-dash 
                                       'xor-dot 'xor-long-dash 'xor-short-dash 
                                       'xor-dot-dash)])
           void?]{

Sets the pen style. See @scheme[pen%] for information about the
 possible styles.

A pen cannot be modified if it was obtained from a
 @scheme[pen-list%] or while it is selected into a drawing context.

}

@defmethod[(set-width [width (real-in 0 255)])
           void?]{

Sets the pen width.

A pen cannot be modified if it was obtained from a
 @scheme[pen-list%] or while it is selected into a drawing context.

}}

