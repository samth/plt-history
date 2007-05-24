#reader(lib "docreader.ss" "scribble")
@require["mz.ss"]

@title[#:tag "bytestrings"]{Byte Strings}

A @pidefterm{byte string} is a fixed-length arary of bytes. A
 @pidefterm{byte} is an exact integer between @scheme[0] and
 @scheme[255] inclusive.

@index['("byte strings" "immutable")]{A} byte string can be
@defterm{mutable} or @defterm{immutable}. When an immutable byte
string is provided to a procedure like @scheme[bytes-set!], the
@exnraise[exn:fail:contract]. Byte-string constants generated by the
default reader (see @secref["parse-string"]) are immutable.

Two byte strings are @scheme[eq?] when mutating one would mutate the
other.  Two byte strings are @scheme[eqv?] and @scheme[equal?] when
they have the same length and contain the same sequence of bytes.

See also: @scheme[immutable].

@; ----------------------------------------
@section{Byte String Constructors, Selectors, and Mutators}

@defproc[(make-bytes [k exact-nonnegative-integer?] [b byte? 0])
bytes?]{ Returns a new mutable byte string of length @scheme[k] where each
position in the byte string is initialized with the byte @scheme[b].

@examples[(make-bytes 5 65)]}


@defproc[(bytes [b byte?] ...0) bytes?]{ Returns a new mutable byte
string whose length is the number of provided @scheme[b]s, and whose
positions are initialized with the given @scheme[b]s.

@examples[(bytes 65 112 112 108 101)]}


@defproc[(bytes->immutable-bytes [bstr bytes?]) (and/c bytes?
 immutable?)]{  Returns an immutable byte string with the same content
 as @scheme[bstr], returning @scheme[bstr] itself if @scheme[bstr] is
 immutable.}


@defproc[(bytes? [v any/c]) boolean?]{ Returns @scheme[#t] if @scheme[v]
 is a byte string, @scheme[#f] otherwise.

@examples[(bytes? #"Apple") (bytes? "Apple")]}


@defproc[(byte? [v any/c]) boolean?]{ Returns @scheme[#t] if @scheme[v] is
 a byte (i.e., an exact integer between @scheme[0] and @scheme[255]
 inclusive), @scheme[#f] otherwise.

@examples[(byte? 65) (byte? 0) (byte? 256) (byte? -1)]}


@defproc[(bytes-length [bstr bytes?]) exact-nonnegative-integer?]{
 Returns the length of @scheme[bstr].

@examples[(bytes-length #"Apple")]}


@defproc[(bytes-ref [bstr bytes?] [k exact-nonnegative-integer?])
 byte?]{  Returns the character at position @scheme[k] in @scheme[bstr].
 The first position in the bytes cooresponds to @scheme[0], so the
 position @scheme[k] must be less than the length of the bytes,
 otherwise the @exnraise[exn:fail:contract].

@examples[(bytes-ref #"Apple" 0)]}


@defproc[(bytes-set! [bstr (and/c bytes? (not/c immutable?))] [k
 exact-nonnegative-integer?] [b byte?]) void?]{  Changes the
 character position @scheme[k] in @scheme[bstr] to @scheme[b].  The first
 position in the byte string cooresponds to @scheme[0], so the position
 @scheme[k] must be less than the length of the bytes, otherwise the
 @exnraise[exn:fail:contract].

@examples[(define s (bytes 65 112 112 108 101))
          (bytes-set! s 4 121)
          s]}


@defproc[(subbytes [bstr bytes?] [start exact-nonnegative-integer?]
 [end exact-nonnegative-integer? (bytes-length str)]) bytes?]{
 Returns a new mutable byte string that is @scheme[(- @scheme[end]
 @scheme[start])] bytes long, and that contains the same bytes
 as @scheme[bstr] from @scheme[start] inclusive to @scheme[end] exclusive.  The
 @scheme[start] and @scheme[end] arguments must be less than the length of
 @scheme[bstr], and @scheme[end] must be greater than or equal to @scheme[bstr],
 otherwise the @exnraise[exn:fail:contract].

@examples[(subbytes #"Apple" 1 3)
          (subbytes #"Apple" 1)]}


@defproc[(bytes-copy [bstr bytes?]) bytes?]{ Returns
 @scheme[(subbytes str 0)].}


@defproc[(bytes-copy! [dest (and/c bytes? (not/c immutable?))]
                      [dest-start exact-nonnegative-integer?]
                      [src bytes?]
                      [src-start exact-nonnegative-integer? 0] 
                      [src-end exact-nonnegative-integer? (bytes-length src)])
         void?]{ 
 Changes the bytes of @scheme[dest] from positions
 @scheme[dest-start] (inclusive) to @scheme[dest-end] (exclusive) to
 match the bytes in @scheme[src] from @scheme[src-start]
 (inclusive). The bytes strings @scheme[dest] and @scheme[src] can be the
 same byte string, and in that case the destination region can overlap with
 the source region; the destination bytes after the copy match
 the source bytes from before the copy. If any of
 @scheme[dest-start], @scheme[src-start], or @scheme[src-end]
 are out of range (taking into account the sizes of the bytes strings and
 the source and destination regions), the
 @exnraise[exn:fail:contract].

@examples[(define s (bytes 65 112 112 108 101))
          (bytes-copy! s 4 #"y")
          (bytes-copy! s 0 s 3 4)
          s]}

@defproc[(bytes-fill! [dest (and/c bytes? (not/c immutable?))] [char
 char?]) void?]{ Changes @scheme[dest] so that every position in the
 bytes is filled with @scheme[char].

@examples[(define s (bytes 65 112 112 108 101))
          (bytes-fill! s 113)
          s]}


@defproc[(bytes-append [bstr bytes?] ...0) bytes?]{  Returns a new
 mutable byte string that is as long as the sum of the given @scheme[bstr]s'
 lengths, and that contains the concatenated bytes of the given
 @scheme[bstr]s. If no @scheme[bstr]s are provided, the result is a zero-length
 byte string.

@examples[(bytes-append #"Apple" #"Banana")]}


@defproc[(bytes->list [bstr bytes?]) (listof byte?)]{ Returns a new
 list of bytes coresponding to the content of @scheme[bstr]. That is,
 the length of the list is @scheme[(bytes-length @scheme[bstr])], and the
 sequence of bytes of @scheme[bstr] are in the same sequence in the
 result list.

@examples[(bytes->list #"Apple")]}


@defproc[(list->bytes [lst (listof byte?)]) bytes?]{ Returns a new
 mutable bytes whose content is the list of bytes in @scheme[lst].
 That is, the length of the bytes is @scheme[(length @scheme[lst])], and
 the sequence of bytes in @scheme[lst] is in the same sequence in
 the result bytes.

@examples[(list->bytes (list 65 112 112 108 101))]}


@; ----------------------------------------
@section{Byte String Comparisons}


@defproc[(bytes=? [bstr1 bytes?] [bstr2 bytes?] ...1) boolean?]{ Returns
 @scheme[#t] if all of the arguments are @scheme[eqv?].}

@examples[(bytes=? #"Apple" #"apple")
          (bytes=? #"a" #"as" #"a")]

@define[(bytes-sort direction)
         @elem{Like @scheme[bytes<?], but checks whether the arguments are @|direction|.}]

@defproc[(bytes<? [bstr1 bytes?] [bstr2 bytes?] ...1) boolean?]{
 Returns @scheme[#t] if the arguments are lexicographically sorted
 increasing, where individual bytes are ordered by @scheme[<],
 @scheme[#f] otherwise.

@examples[(bytes<? #"Apple" #"apple")
          (bytes<? #"apple" #"Apple")
          (bytes<? #"a" #"b" #"c")]}

@defproc[(bytes>? [bstr1 bytes?] [bstr2 bytes?] ...1) boolean?]{
 @bytes-sort["decreasing"]

@examples[(bytes>? #"Apple" #"apple")
          (bytes>? #"apple" #"Apple")
          (bytes>? #"c" #"b" #"a")]}

@; ----------------------------------------
@section{Bytes to/from Characters, Decoding and Encoding}


@defproc[(bytes->string/utf-8 [bstr bytes?] 
                              [err-char (or/c false/c char?) #f] 
                              [start exact-nonnegative-integer? 0]
                              [end exact-nonnegative-integer? (bytes-length bstr)])
         string?]{
 Produces a string by decoding the @scheme[start] to @scheme[end]
 substring of @scheme[bstr] as a UTF-8 encoding of Unicode code points.
 If @scheme[err-char] is not @scheme[#f], then it is used
 for bytes that fall in the range @scheme[#o200] to @scheme[#o377] but
 are not part of a valid encoding sequence. (This is consistent with
 reading characters from a port; see @secref["ports"] for more details.)
 If @scheme[err-char] is @scheme[#f], and if the
 @scheme[start] to @scheme[end] substring of @scheme[bstr] is not a valid
 UTF-8 encoding overall, then the @exnraise[exn:fail:contract].}

@defproc[(bytes->string/locale [bstr bytes?] 
                               [err-char (or/c false/c char?) #f]
                               [start exact-nonnegative-integer? 0]
                               [end exact-nonnegative-integer? (bytes-length bstr)])
         string?]{
 Produces a string by decoding the @scheme[start] to @scheme[end] substring
 of @scheme[bstr] using the current locale's encoding (see also
 @secref["locales"]). If @scheme[err-char] is not
 @scheme[#f], it is used for each byte in @scheme[bstr] that is not part
 of a valid encoding; if @scheme[err-char] is @scheme[#f], and if the
 @scheme[start] to @scheme[end] substring of @scheme[bstr] is not a valid
 encoding overall, then the @exnraise[exn:fail:contract].}

@defproc[(bytes->string/latin-1 [bstr bytes?]
                                [err-char (or/c false/c char?) #f]
                                [start exact-nonnegative-integer? 0]
                                [end exact-nonnegative-integer? (bytes-length bstr)])
         string?]{
 Produces a string by decoding the @scheme[start] to @scheme[end] substring
 of @scheme[bstr] as a Latin-1 encoding of Unicode code points; i.e.,
 each byte is translated directly to a character using
 @scheme[integer->char], so the decoding always succeeds. (See also the
 Latin-1 footnote of @secref["encodings"].)  The @scheme[err-char]
 argument is ignored, but present for consistency with the other
 operations.}
 
@defproc[(string->bytes/utf-8 [str string?]
                              [err-byte (or/c false/c byte?) #f] 
                              [start exact-nonnegative-integer? 0]
                              [end exact-nonnegative-integer? (string-length str)])
         bytes?]{
 Produces a byte string by encoding the @scheme[start] to @scheme[end]
 substring of @scheme[str] via UTF-8 (always succeeding). The
 @scheme[err-byte] argument is ignored, but included for consistency with
 the other operations.}

@defproc[(string->bytes/locale [str string?] 
                               [err-byte (or/c false/c byte?) #f] 
                               [start exact-nonnegative-integer? 0]
                               [end exact-nonnegative-integer? (string-length str)])
         bytes?]{
 Produces a string by encoding the @scheme[start] to @scheme[end] substring
 of @scheme[str] using the current locale's encoding (see also
 @secref["locales"]). If @scheme[err-byte] is not @scheme[#f], it is used
 for each character in @scheme[str] that cannot be encoded for the
 current locale; if @scheme[err-byte] is @scheme[#f], and if the
 @scheme[start] to @scheme[end] substring of @scheme[str] cannot be encoded,
 then the @exnraise[exn:fail:contract].}

@defproc[(string->bytes/latin-1 [str string?]
                                [err-byte (or/c false/c byte?) #f] 
                                [start exact-nonnegative-integer? 0]
                                [end exact-nonnegative-integer? (string-length str)])
         bytes?]{
 Produces a string by encoding the @scheme[start] to @scheme[end] substring
 of @scheme[str] using Latin-1; i.e., each character is translated
 directly to a byte using @scheme[char->integer]. If @scheme[err-byte] is
 not @scheme[#f], it is used for each character in @scheme[str] whose
 value is greater than @scheme[255]. (See also the Latin-1 footnote of
 @secref["encodings"]. If @scheme[err-byte] is @scheme[#f], and if the
 @scheme[start] to @scheme[end] substring of @scheme[str] has a character
 with a value greater than @scheme[255], then the
 @exnraise[exn:fail:contract].}

@defproc[(string-utf-8-length [str string?]
                              [start exact-nonnegative-integer? 0]
                              [end exact-nonnegative-integer? (string-lenght str)])
         exact-nonnegative-integer?]{
 Returns the length in bytes of the UTF-8 encoding of @scheme[str]'s
 substring from @scheme[start] to @scheme[end], but without actually
 generating the encoded bytes.}

@defproc[(bytes-utf-8-length [bstr bytes?]
                             [err-char (or/c false/c char?) #f]
                             [start exact-nonnegative-integer? 0]
                             [end exact-nonnegative-integer? (bytes-length bstr)])
         exact-nonnegative-integer?]{
 Returns the length in characters of the UTF-8 decoding of
 @scheme[bstr]'s substring from @scheme[start] to @scheme[end], but without
 actually generating the decoded characters. If @scheme[err-char] is
 @scheme[#f] and the substring is not a UTF-8 encoding overall, the
 result is @scheme[#f]. Otherwise, @scheme[err-char] is used to resolve
 decoding errors as in @scheme[bytes->string/utf-8].}

@defproc[(bytes-utf-8-ref [bstr bytes?] 
                          [skip exact-nonnegative-integer? 0]
                          [err-char (or/c false/c char?) #f]
                          [start exact-nonnegative-integer? 0]
                          [end exact-nonnegative-integer? (bytes-length bstr)])
         char?]{
 Returns the @scheme[skip]th character in the UTF-8 decoding of
 @scheme[bstr]'s substring from @scheme[start] to @scheme[end], but without
 actually generating the other decoded characters. If the substring is
 not a UTF-8 encoding up to the @scheme[skip]th character (when
 @scheme[err-char] is @scheme[#f]), or if the substring decoding produces
 fewer than @scheme[skip] characters, the result is @scheme[#f]. If
 @scheme[err-char] is not @scheme[#f], it is used to resolve decoding
 errors as in @scheme[bytes->string/utf-8].}

@defproc[(bytes-utf-8-index [bstr bytes?]
                            [skip exact-nonnegative-integer? 0]
                            [err-char (or/c false/c char?) #f]
                            [start exact-nonnegative-integer? 0]
                            [end exact-nonnegative-integer? (bytes-length bstr)])
         exact-nonnegative-integer?]{
 Returns the offset in bytes into @scheme[bstr] at which the @scheme[skip]th
 character's encoding starts in the UTF-8 decoding of @scheme[bstr]'s
 substring from @scheme[start] to @scheme[end] (but without actually
 generating the other decoded characters). The result is relative to
 the start of @scheme[bstr], not to @scheme[start]. If the substring is not
 a UTF-8 encoding up to the @scheme[skip]th character (when
 @scheme[err-char] is @scheme[#f]), or if the substring decoding produces
 fewer than @scheme[skip] characters, the result is @scheme[#f]. If
 @scheme[err-char] is not @scheme[#f], it is used to resolve decoding
 errors as in @scheme[bytes->string/utf-8].}

@; ----------------------------------------
@section{Bytes to Bytes Encoding Conversion}

