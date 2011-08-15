#lang scribble/doc
@(require "mz.rkt")

@title[#:tag "bytestrings"]{Byte Strings}

@guideintro["bytestrings"]{byte strings}

A @deftech{byte string} is a fixed-length array of bytes. A
 @pidefterm{byte} is an exact integer between @racket[0] and
 @racket[255] inclusive.

@index['("byte strings" "immutable")]{A} byte string can be
@defterm{mutable} or @defterm{immutable}. When an immutable byte
string is provided to a procedure like @racket[bytes-set!], the
@exnraise[exn:fail:contract]. Byte-string constants generated by the
default reader (see @secref["parse-string"]) are immutable.

Two byte strings are @racket[equal?] when they have the same length
and contain the same sequence of bytes.

A byte string can be used as a single-valued sequence (see
@secref["sequences"]). The bytes of the string serve as elements
of the sequence. See also @racket[in-bytes].

See also: @racket[immutable?].

@; ----------------------------------------
@section{Byte String Constructors, Selectors, and Mutators}

@defproc[(bytes? [v any/c]) boolean?]{ Returns @racket[#t] if @racket[v]
 is a byte string, @racket[#f] otherwise.

@mz-examples[(bytes? #"Apple") (bytes? "Apple")]}


@defproc[(make-bytes [k exact-nonnegative-integer?] [b byte? 0])
bytes?]{ Returns a new mutable byte string of length @racket[k] where each
position in the byte string is initialized with the byte @racket[b].

@mz-examples[(make-bytes 5 65)]}


@defproc[(bytes [b byte?] ...) bytes?]{ Returns a new mutable byte
string whose length is the number of provided @racket[b]s, and whose
positions are initialized with the given @racket[b]s.

@mz-examples[(bytes 65 112 112 108 101)]}


@defproc[(bytes->immutable-bytes [bstr bytes?])
         (and/c bytes? immutable?)]{
 Returns an immutable byte string with the same content
 as @racket[bstr], returning @racket[bstr] itself if @racket[bstr] is
 immutable.

@examples[
(bytes->immutable-bytes (bytes 65 65 65))
(define b (bytes->immutable-bytes (make-bytes 5 65)))
(bytes->immutable-bytes b)
(eq? (bytes->immutable-bytes b) b)
]}

@defproc[(byte? [v any/c]) boolean?]{ Returns @racket[#t] if @racket[v] is
 a byte (i.e., an exact integer between @racket[0] and @racket[255]
 inclusive), @racket[#f] otherwise.

@mz-examples[(byte? 65) (byte? 0) (byte? 256) (byte? -1)]}


@defproc[(bytes-length [bstr bytes?]) exact-nonnegative-integer?]{
 Returns the length of @racket[bstr].

@mz-examples[(bytes-length #"Apple")]}


@defproc[(bytes-ref [bstr bytes?] [k exact-nonnegative-integer?])
 byte?]{  Returns the character at position @racket[k] in @racket[bstr].
 The first position in the bytes cooresponds to @racket[0], so the
 position @racket[k] must be less than the length of the bytes,
 otherwise the @exnraise[exn:fail:contract].

@mz-examples[(bytes-ref #"Apple" 0)]}


@defproc[(bytes-set! [bstr (and/c bytes? (not/c immutable?))] [k
 exact-nonnegative-integer?] [b byte?]) void?]{  Changes the
 character position @racket[k] in @racket[bstr] to @racket[b].  The first
 position in the byte string cooresponds to @racket[0], so the position
 @racket[k] must be less than the length of the bytes, otherwise the
 @exnraise[exn:fail:contract].

@mz-examples[(define s (bytes 65 112 112 108 101))
             (bytes-set! s 4 121)
             s]}


@defproc[(subbytes [bstr bytes?] [start exact-nonnegative-integer?]
 [end exact-nonnegative-integer? (bytes-length str)]) bytes?]{ Returns
 a new mutable byte string that is @racket[(- end start)] bytes long,
 and that contains the same bytes as @racket[bstr] from @racket[start]
 inclusive to @racket[end] exclusive.  The @racket[start] and
 @racket[end] arguments must be less than or equal to the length of
 @racket[bstr], and @racket[end] must be greater than or equal to
 @racket[start], otherwise the @exnraise[exn:fail:contract].

@mz-examples[(subbytes #"Apple" 1 3)
             (subbytes #"Apple" 1)]}


@defproc[(bytes-copy [bstr bytes?]) bytes?]{ Returns
 @racket[(subbytes str 0)].}


@defproc[(bytes-copy! [dest (and/c bytes? (not/c immutable?))]
                      [dest-start exact-nonnegative-integer?]
                      [src bytes?]
                      [src-start exact-nonnegative-integer? 0]
                      [src-end exact-nonnegative-integer? (bytes-length src)])
         void?]{

 Changes the bytes of @racket[dest] starting at position
 @racket[dest-start] to match the bytes in @racket[src] from
 @racket[src-start] (inclusive) to @racket[src-end] (exclusive). The
 bytes strings @racket[dest] and @racket[src] can be the same byte
 string, and in that case the destination region can overlap with the
 source region; the destination bytes after the copy match the source
 bytes from before the copy. If any of @racket[dest-start],
 @racket[src-start], or @racket[src-end] are out of range (taking into
 account the sizes of the bytes strings and the source and destination
 regions), the @exnraise[exn:fail:contract].

@mz-examples[(define s (bytes 65 112 112 108 101))
             (bytes-copy! s 4 #"y")
             (bytes-copy! s 0 s 3 4)
             s]}

@defproc[(bytes-fill! [dest (and/c bytes? (not/c immutable?))] [b
 byte?]) void?]{ Changes @racket[dest] so that every position in the
 bytes is filled with @racket[b].

@mz-examples[(define s (bytes 65 112 112 108 101))
             (bytes-fill! s 113)
             s]}


@defproc[(bytes-append [bstr bytes?] ...) bytes?]{ 

@index['("byte strings" "concatenate")]{Returns} a new mutable byte string
that is as long as the sum of the given @racket[bstr]s' lengths, and
that contains the concatenated bytes of the given @racket[bstr]s. If
no @racket[bstr]s are provided, the result is a zero-length byte
string.

@mz-examples[(bytes-append #"Apple" #"Banana")]}


@defproc[(bytes->list [bstr bytes?]) (listof byte?)]{ Returns a new
 list of bytes corresponding to the content of @racket[bstr]. That is,
 the length of the list is @racket[(bytes-length bstr)], and the
 sequence of bytes in @racket[bstr] is the same sequence in the
 result list.

@mz-examples[(bytes->list #"Apple")]}


@defproc[(list->bytes [lst (listof byte?)]) bytes?]{ Returns a new
 mutable byte string whose content is the list of bytes in @racket[lst].
 That is, the length of the byte string is @racket[(length lst)], and
 the sequence of bytes in @racket[lst] is the same sequence in
 the result byte string.

@mz-examples[(list->bytes (list 65 112 112 108 101))]}

@defproc[(make-shared-bytes [k exact-nonnegative-integer?] [b byte? 0])
bytes?]{ Returns a new mutable byte string of length @racket[k] where each
position in the byte string is initialized with the byte @racket[b].
For communication among @tech{places}, the new byte string is allocated in the
@tech{shared memory space}.

@mz-examples[(make-shared-bytes 5 65)]}


@defproc[(shared-bytes [b byte?] ...) bytes?]{ Returns a new mutable byte
string whose length is the number of provided @racket[b]s, and whose
positions are initialized with the given @racket[b]s.
For communication among @tech{places}, the new byte string is allocated in the
@tech{shared memory space}.

@mz-examples[(shared-bytes 65 112 112 108 101)]}


@; ----------------------------------------
@section{Byte String Comparisons}

@defproc[(bytes=? [bstr1 bytes?] [bstr2 bytes?] ...+) boolean?]{ Returns
 @racket[#t] if all of the arguments are @racket[eqv?].}

@mz-examples[(bytes=? #"Apple" #"apple")
             (bytes=? #"a" #"as" #"a")]

@(define (bytes-sort direction)
   @elem{Like @racket[bytes<?], but checks whether the arguments are @|direction|.})

@defproc[(bytes<? [bstr1 bytes?] [bstr2 bytes?] ...+) boolean?]{
 Returns @racket[#t] if the arguments are lexicographically sorted
 increasing, where individual bytes are ordered by @racket[<],
 @racket[#f] otherwise.

@mz-examples[(bytes<? #"Apple" #"apple")
             (bytes<? #"apple" #"Apple")
             (bytes<? #"a" #"b" #"c")]}

@defproc[(bytes>? [bstr1 bytes?] [bstr2 bytes?] ...+) boolean?]{
 @bytes-sort["decreasing"]

@mz-examples[(bytes>? #"Apple" #"apple")
             (bytes>? #"apple" #"Apple")
             (bytes>? #"c" #"b" #"a")]}

@; ----------------------------------------
@section{Bytes to/from Characters, Decoding and Encoding}

@defproc[(bytes->string/utf-8 [bstr bytes?]
                              [err-char (or/c #f char?) #f]
                              [start exact-nonnegative-integer? 0]
                              [end exact-nonnegative-integer? (bytes-length bstr)])
         string?]{
 Produces a string by decoding the @racket[start] to @racket[end]
 substring of @racket[bstr] as a UTF-8 encoding of Unicode code
 points.  If @racket[err-char] is not @racket[#f], then it is used for
 bytes that fall in the range @racket[#o200] to @racket[#o377] but are
 not part of a valid encoding sequence. (This rule is consistent with
 reading characters from a port; see @secref["encodings"] for more
 details.)  If @racket[err-char] is @racket[#f], and if the
 @racket[start] to @racket[end] substring of @racket[bstr] is not a
 valid UTF-8 encoding overall, then the @exnraise[exn:fail:contract].
 
@examples[
(bytes->string/utf-8 (bytes #xc3 #xa7 #xc3 #xb0 #xc3 #xb6 #xc2 #xa3))
]}

@defproc[(bytes->string/locale [bstr bytes?]
                               [err-char (or/c #f char?) #f]
                               [start exact-nonnegative-integer? 0]
                               [end exact-nonnegative-integer? (bytes-length bstr)])
         string?]{
 Produces a string by decoding the @racket[start] to @racket[end] substring
 of @racket[bstr] using the current locale's encoding (see also
 @secref["encodings"]). If @racket[err-char] is not
 @racket[#f], it is used for each byte in @racket[bstr] that is not part
 of a valid encoding; if @racket[err-char] is @racket[#f], and if the
 @racket[start] to @racket[end] substring of @racket[bstr] is not a valid
 encoding overall, then the @exnraise[exn:fail:contract].}

@defproc[(bytes->string/latin-1 [bstr bytes?]
                                [err-char (or/c #f char?) #f]
                                [start exact-nonnegative-integer? 0]
                                [end exact-nonnegative-integer? (bytes-length bstr)])
         string?]{
 Produces a string by decoding the @racket[start] to @racket[end] substring
 of @racket[bstr] as a Latin-1 encoding of Unicode code points; i.e.,
 each byte is translated directly to a character using
 @racket[integer->char], so the decoding always succeeds.
 The @racket[err-char]
 argument is ignored, but present for consistency with the other
 operations.
 
@examples[
(bytes->string/latin-1 (bytes #xfe #xd3 #xd1 #xa5))
]}

@defproc[(string->bytes/utf-8 [str string?]
                              [err-byte (or/c #f byte?) #f]
                              [start exact-nonnegative-integer? 0]
                              [end exact-nonnegative-integer? (string-length str)])
         bytes?]{
 Produces a byte string by encoding the @racket[start] to @racket[end]
 substring of @racket[str] via UTF-8 (always succeeding). The
 @racket[err-byte] argument is ignored, but included for consistency with
 the other operations.
@examples[
(define b
  (bytes->string/utf-8
   (bytes #xc3 #xa7 #xc3 #xb0 #xc3 #xb6 #xc2 #xa3)))

(string->bytes/utf-8 b)
(bytes->string/utf-8 (string->bytes/utf-8 b))
]}

@defproc[(string->bytes/locale [str string?]
                               [err-byte (or/c #f byte?) #f]
                               [start exact-nonnegative-integer? 0]
                               [end exact-nonnegative-integer? (string-length str)])
         bytes?]{
 Produces a string by encoding the @racket[start] to @racket[end] substring
 of @racket[str] using the current locale's encoding (see also
 @secref["encodings"]). If @racket[err-byte] is not @racket[#f], it is used
 for each character in @racket[str] that cannot be encoded for the
 current locale; if @racket[err-byte] is @racket[#f], and if the
 @racket[start] to @racket[end] substring of @racket[str] cannot be encoded,
 then the @exnraise[exn:fail:contract].}

@defproc[(string->bytes/latin-1 [str string?]
                                [err-byte (or/c #f byte?) #f]
                                [start exact-nonnegative-integer? 0]
                                [end exact-nonnegative-integer? (string-length str)])
         bytes?]{
 Produces a string by encoding the @racket[start] to @racket[end] substring
 of @racket[str] using Latin-1; i.e., each character is translated
 directly to a byte using @racket[char->integer]. If @racket[err-byte] is
 not @racket[#f], it is used for each character in @racket[str] whose
 value is greater than @racket[255].
 If @racket[err-byte] is @racket[#f], and if the
 @racket[start] to @racket[end] substring of @racket[str] has a character
 with a value greater than @racket[255], then the
 @exnraise[exn:fail:contract].
 
@examples[
(define b
  (bytes->string/latin-1 (bytes #xfe #xd3 #xd1 #xa5)))

(string->bytes/latin-1 b)
(bytes->string/latin-1 (string->bytes/latin-1 b))
]}

@defproc[(string-utf-8-length [str string?]
                              [start exact-nonnegative-integer? 0]
                              [end exact-nonnegative-integer? (string-lenght str)])
         exact-nonnegative-integer?]{
 Returns the length in bytes of the UTF-8 encoding of @racket[str]'s
 substring from @racket[start] to @racket[end], but without actually
 generating the encoded bytes.
 
@examples[
(string-utf-8-length 
  (bytes->string/utf-8 (bytes #xc3 #xa7 #xc3 #xb0 #xc3 #xb6 #xc2 #xa3)))
(string-utf-8-length "hello")
]}

@defproc[(bytes-utf-8-length [bstr bytes?]
                             [err-char (or/c #f char?) #f]
                             [start exact-nonnegative-integer? 0]
                             [end exact-nonnegative-integer? (bytes-length bstr)])
         exact-nonnegative-integer?]{
 Returns the length in characters of the UTF-8 decoding of
 @racket[bstr]'s substring from @racket[start] to @racket[end], but without
 actually generating the decoded characters. If @racket[err-char] is
 @racket[#f] and the substring is not a UTF-8 encoding overall, the
 result is @racket[#f]. Otherwise, @racket[err-char] is used to resolve
 decoding errors as in @racket[bytes->string/utf-8].
 
@examples[
(bytes-utf-8-length (bytes #xc3 #xa7 #xc3 #xb0 #xc3 #xb6 #xc2 #xa3))
(bytes-utf-8-length (make-bytes 5 65))
]}

@defproc[(bytes-utf-8-ref [bstr bytes?]
                          [skip exact-nonnegative-integer? 0]
                          [err-char (or/c #f char?) #f]
                          [start exact-nonnegative-integer? 0]
                          [end exact-nonnegative-integer? (bytes-length bstr)])
         char?]{
 Returns the @racket[skip]th character in the UTF-8 decoding of
 @racket[bstr]'s substring from @racket[start] to @racket[end], but without
 actually generating the other decoded characters. If the substring is
 not a UTF-8 encoding up to the @racket[skip]th character (when
 @racket[err-char] is @racket[#f]), or if the substring decoding produces
 fewer than @racket[skip] characters, the result is @racket[#f]. If
 @racket[err-char] is not @racket[#f], it is used to resolve decoding
 errors as in @racket[bytes->string/utf-8].
 
@examples[
(bytes-utf-8-ref (bytes #xc3 #xa7 #xc3 #xb0 #xc3 #xb6 #xc2 #xa3) 0)
(bytes-utf-8-ref (bytes #xc3 #xa7 #xc3 #xb0 #xc3 #xb6 #xc2 #xa3) 1)
(bytes-utf-8-ref (bytes #xc3 #xa7 #xc3 #xb0 #xc3 #xb6 #xc2 #xa3) 2)
(bytes-utf-8-ref (bytes 65 66 67 68) 0)
(bytes-utf-8-ref (bytes 65 66 67 68) 1)
(bytes-utf-8-ref (bytes 65 66 67 68) 2)
]}

@defproc[(bytes-utf-8-index [bstr bytes?]
                            [skip exact-nonnegative-integer? 0]
                            [err-char (or/c #f char?) #f]
                            [start exact-nonnegative-integer? 0]
                            [end exact-nonnegative-integer? (bytes-length bstr)])
         exact-nonnegative-integer?]{
 Returns the offset in bytes into @racket[bstr] at which the @racket[skip]th
 character's encoding starts in the UTF-8 decoding of @racket[bstr]'s
 substring from @racket[start] to @racket[end] (but without actually
 generating the other decoded characters). The result is relative to
 the start of @racket[bstr], not to @racket[start]. If the substring is not
 a UTF-8 encoding up to the @racket[skip]th character (when
 @racket[err-char] is @racket[#f]), or if the substring decoding produces
 fewer than @racket[skip] characters, the result is @racket[#f]. If
 @racket[err-char] is not @racket[#f], it is used to resolve decoding
 errors as in @racket[bytes->string/utf-8].

@examples[
(bytes-utf-8-index (bytes #xc3 #xa7 #xc3 #xb0 #xc3 #xb6 #xc2 #xa3) 0)
(bytes-utf-8-index (bytes #xc3 #xa7 #xc3 #xb0 #xc3 #xb6 #xc2 #xa3) 1)
(bytes-utf-8-index (bytes #xc3 #xa7 #xc3 #xb0 #xc3 #xb6 #xc2 #xa3) 2)
(bytes-utf-8-index (bytes 65 66 67 68) 0)
(bytes-utf-8-index (bytes 65 66 67 68) 1)
(bytes-utf-8-index (bytes 65 66 67 68) 2)
]}

@; ----------------------------------------
@section{Bytes to Bytes Encoding Conversion}

@defproc[(bytes-open-converter [from-name string?] [to-name string?])
         bytes-converter?]{

Produces a @deftech{byte converter} to go from the encoding named by
@racket[from-name] to the encoding named by @racket[to-name]. If the
requested conversion pair is not available, @racket[#f] is returned
instead of a converter.

Certain encoding combinations are always available:

 @itemize[

 @item{@racket[(bytes-open-converter "UTF-8" "UTF-8")] --- the
   identity conversion, except that encoding errors in the input lead
   to a decoding failure.}

 @item{@racket[(bytes-open-converter "UTF-8-permissive" "UTF-8")] ---
   @index['("UTF-8-permissive")]{the} identity conversion, except that
   any input byte that is not part of a valid encoding sequence is
   effectively replaced by the UTF-8 encoding sequence for
   @racketvalfont{#\uFFFD}.  (This handling of invalid sequences is
   consistent with the interpretation of port bytes streams into
   characters; see @secref["ports"].)}

 @item{@racket[(bytes-open-converter "" "UTF-8")] --- converts from
   the current locale's default encoding (see @secref["encodings"])
   to UTF-8.}

 @item{@racket[(bytes-open-converter "UTF-8" "")] --- converts from
   UTF-8 to the current locale's default encoding (see
   @secref["encodings"]).}

 @item{@racket[(bytes-open-converter "platform-UTF-8" "platform-UTF-16")]
   --- converts UTF-8 to UTF-16 on @|AllUnix|, where each UTF-16
   code unit is a sequence of two bytes ordered by the current
   platform's endianness. On Windows, the input can include
   encodings that are not valid UTF-8, but which naturally extend the
   UTF-8 encoding to support unpaired surrogate code units, and the
   output is a sequence of UTF-16 code units (as little-endian byte
   pairs), potentially including unpaired surrogates.}

 @item{@racket[(bytes-open-converter "platform-UTF-8-permissive" "platform-UTF-16")]
   --- like @racket[(bytes-open-converter "platform-UTF-8" "platform-UTF-16")],
   but an input byte that is not part of a valid UTF-8 encoding
   sequence (or valid for the unpaired-surrogate extension on
   Windows) is effectively replaced with @racket[(char->integer #\?)].}

 @item{@racket[(bytes-open-converter "platform-UTF-16" "platform-UTF-8")]
   --- converts UTF-16 (bytes orderd by the current platform's
   endianness) to UTF-8 on @|AllUnix|. On Windows, the input can
   include UTF-16 code units that are unpaired surrogates, and the
   corresponding output includes an encoding of each surrogate in a
   natural extension of UTF-8. On @|AllUnix|, surrogates are
   assumed to be paired: a pair of bytes with the bits @racket[#xD800]
   starts a surrogate pair, and the @racket[#x03FF] bits are used from
   the pair and following pair (independent of the value of the
   @racket[#xDC00] bits). On all platforms, performance may be poor
   when decoding from an odd offset within an input byte string.}

 ]

A newly opened byte converter is registered with the current custodian
(see @secref["custodians"]), so that the converter is closed when
the custodian is shut down. A converter is not registered with a
custodian (and does not need to be closed) if it is one of the
guaranteed combinations not involving @racket[""] on Unix, or if it
is any of the guaranteed combinations (including @racket[""]) on
Windows and Mac OS X.

@margin-note{In the Racket software distributions for Windows, a suitable
@filepath{iconv.dll} is included with @filepath{libmzsch@italic{VERS}.dll}.}

The set of available encodings and combinations varies by platform,
depending on the @exec{iconv} library that is installed; the
@racket[from-name] and @racket[to-name] arguments are passed on to
@tt{iconv_open}. On Windows, @filepath{iconv.dll} or
@filepath{libiconv.dll} must be in the same directory as
@filepath{libmzsch@italic{VERS}.dll} (where @italic{VERS} is a version
number), in the user's path, in the system directory, or in the
current executable's directory at run time, and the DLL must either
supply @tt{_errno} or link to @filepath{msvcrt.dll} for @tt{_errno};
otherwise, only the guaranteed combinations are available.

Use @racket[bytes-convert] with the result to convert byte strings.}


@defproc[(bytes-close-converter [converter bytes-converter?]) void]{

Closes the given converter, so that it can no longer be used with
@racket[bytes-convert] or @racket[bytes-convert-end].}


@defproc[(bytes-convert [converter bytes-converter?]
                        [src-bstr bytes?]
                        [src-start-pos exact-nonnegative-integer? 0]
                        [src-end-pos exact-nonnegative-integer? (bytes-length src-bstr)]
                        [dest-bstr (or/c bytes? #f) #f]
                        [dest-start-pos exact-nonnegative-integer? 0]
                        [dest-end-pos (or/c exact-nonnegative-integer? #f)
                                      (and dest-bstr
                                           (bytes-length dest-bstr))])
          (values (or/c bytes? exact-nonnegative-integer?)
                  exact-nonnegative-integer?
                  (or/c 'complete 'continues 'aborts 'error))]{

Converts the bytes from @racket[src-start-pos] to @racket[src-end-pos]
in @racket[src-bstr].

If @racket[dest-bstr] is not @racket[#f], the converted bytes are
written into @racket[dest-bstr] from @racket[dest-start-pos] to
@racket[dest-end-pos]. If @racket[dest-bstr] is @racket[#f], then a
newly allocated byte string holds the conversion results, and if
@racket[dest-end-pos] is not @racket[#f], the size of the result byte
string is no more than @racket[(- dest-end-pos dest-start-pos)].

The result of @racket[bytes-convert] is three values:

 @itemize[

 @item{@racket[_result-bstr] or @racket[_dest-wrote-amt] --- a byte
 string if @racket[dest-bstr] is @racket[#f] or not provided, or the
 number of bytes written into @racket[dest-bstr] otherwise.}

 @item{@racket[_src-read-amt] --- the number of bytes successfully converted
 from @racket[src-bstr].}

 @item{@indexed-racket['complete], @indexed-racket['continues],
 @indexed-racket['aborts], or @indexed-racket['error] --- indicates
 how conversion terminated:

  @itemize[

   @item{@racket['complete]: The entire input was processed, and
    @racket[_src-read-amt] will be equal to @racket[(- src-end-pos
    src-start-pos)].}

   @item{@racket['continues]: Conversion stopped due to the limit on
   the result size or the space in @racket[dest-bstr]; in this case,
   fewer than @racket[(- dest-end-pos dest-start-pos)] bytes may be
   returned if more space is needed to process the next complete
   encoding sequence in @racket[src-bstr].}

   @item{@racket['aborts]: The input stopped part-way through an
   encoding sequence, and more input bytes are necessary to continue.
   For example, if the last byte of input is @racket[#o303] for a
   @racket["UTF-8-permissive"] decoding, the result is
   @racket['aborts], because another byte is needed to determine how to
   use the @racket[#o303] byte.}

   @item{@racket['error]: The bytes starting at @racket[(+
   src-start-pos _src-read-amt)] bytes in @racket[src-bstr] do not form
   a legal encoding sequence. This result is never produced for some
   encodings, where all byte sequences are valid encodings. For
   example, since @racket["UTF-8-permissive"] handles an invalid UTF-8
   sequence by dropping characters or generating ``?,'' every byte
   sequence is effectively valid.}

  ]}
 ]

Applying a converter accumulates state in the converter (even when the
third result of @racket[bytes-convert] is @racket['complete]). This
state can affect both further processing of input and further
generation of output, but only for conversions that involve ``shift
sequences'' to change modes within a stream. To terminate an input
sequence and reset the converter, use @racket[bytes-convert-end].

@examples[
(define convert (bytes-open-converter "UTF-8" "UTF-16"))
(bytes-convert convert (bytes 65 66 67 68))
(bytes 195 167 195 176 195 182 194 163)
(bytes-convert convert (bytes 195 167 195 176 195 182 194 163))
(bytes-close-converter convert)
]}


@defproc[(bytes-convert-end [converter bytes-converter?]
                            [dest-bstr (or/c bytes? #f) #f]
                            [dest-start-pos exact-nonnegative-integer? 0]
                            [dest-end-pos (or/c exact-nonnegative-integer? #f)
                                          (and dest-bstr
                                               (bytes-length dest-bstr))])
          (values (or/c bytes? exact-nonnegative-integer?)
                  (or/c 'complete 'continues))]{

Like @racket[bytes-convert], but instead of converting bytes, this
procedure generates an ending sequence for the conversion (sometimes
called a ``shift sequence''), if any. Few encodings use shift
sequences, so this function will succeed with no output for most
encodings. In any case, successful output of a (possibly empty) shift
sequence resets the converter to its initial state.

The result of @racket[bytes-convert-end] is two values:

  @itemize[

  @item{@racket[_result-bstr] or @racket[_dest-wrote-amt] --- a byte string if
  @racket[dest-bstr] is @racket[#f] or not provided, or the number of
  bytes written into @racket[dest-bstr] otherwise.}

  @item{@indexed-racket['complete] or @indexed-racket['continues] ---
  indicates whether conversion completed. If @racket['complete], then
  an entire ending sequence was produced. If @racket['continues], then
  the conversion could not complete due to the limit on the result
  size or the space in @racket[dest-bstr], and the first result is
  either an empty byte string or @racket[0].}

  ]
}


@defproc[(bytes-converter? [v any/c]) boolean?]{

Returns @racket[#t] if @racket[v] is a @tech{byte converter} produced
by @racket[bytes-open-converter], @racket[#f] otherwise.

@examples[
(bytes-converter? (bytes-open-converter "UTF-8" "UTF-16"))
(bytes-converter? (bytes-open-converter "whacky" "not likely"))
(define b (bytes-open-converter "UTF-8" "UTF-16"))
(bytes-close-converter b)
(bytes-converter? b)
]}

@defproc[(locale-string-encoding) any]{

Returns a string for the current locale's encoding (i.e., the encoding
normally identified by @racket[""]). See also
@racket[system-language+country].}

@section{Additional Byte String Functions}
@note-lib[racket/bytes]
@(define string-eval (make-base-eval))
@(interaction-eval #:eval string-eval (require racket/bytes racket/list))

@defproc[(bytes-append* [str bytes?] ... [strs (listof bytes?)]) bytes?]{
@; Note: this is exactly the same description as the one for append*

Like @racket[bytes-append], but the last argument is used as a list
of arguments for @racket[bytes-append], so @racket[(bytes-append*
str ... strs)] is the same as @racket[(apply bytes-append str
... strs)].  In other words, the relationship between
@racket[bytes-append] and @racket[bytes-append*] is similar to the
one between @racket[list] and @racket[list*].

@mz-examples[#:eval string-eval
  (bytes-append* #"a" #"b" '(#"c" #"d"))
  (bytes-append* (cdr (append* (map (lambda (x) (list #", " x))
                                     '(#"Alpha" #"Beta" #"Gamma")))))
]}

@defproc[(bytes-join [strs (listof bytes?)] [sep bytes?]) bytes?]{

Appends the byte strings in @racket[strs], inserting @racket[sep] between
each pair of bytes in @racket[strs].

@mz-examples[#:eval string-eval
 (bytes-join '(#"one" #"two" #"three" #"four") #" potato ")
]}

@close-eval[string-eval]
