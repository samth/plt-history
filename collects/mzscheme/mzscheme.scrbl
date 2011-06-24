#lang scribble/doc
@(require scribble/manual
          (for-label mzscheme
                     (only-in r5rs set-car! set-cdr!)
                     (only-in scheme/base
                              for-syntax #%plain-module-begin
                              exact-nonnegative-integer?
                              exact-positive-integer?
                              syntax?
                              #%plain-lambda #%plain-app
                              syntax->datum datum->syntax
                              make-base-empty-namespace
                              make-hash make-hasheq make-weak-hash make-weak-hasheq
                              make-immutable-hash
                              make-immutable-hasheq
			      exn:fail exn:fail:contract)))

@(define-syntax-rule (def-base base-define base-define-syntax base-define-for-syntax
                               base-define-struct
                               base-if base-cond base-case base-top-interaction
                               base-open-input-file base-apply base-prop:procedure
                               base-free-identifier=? base-free-template-identifier=?
                               base-free-transformer-identifier=? base-free-label-identifier=?)
   (begin
    (require (for-label scheme/base))
    (define base-define (scheme define))
    (define base-define-syntax (scheme define-syntax))
    (define base-define-for-syntax (scheme define-for-syntax))
    (define base-define-struct (scheme define-struct))
    (define base-if (scheme if))
    (define base-cond (scheme cond))
    (define base-case (scheme case))
    (define base-top-interaction (scheme #%top-interaction))
    (define base-open-input-file (scheme open-input-file))
    (define base-apply (scheme apply))
    (define base-prop:procedure (scheme prop:procedure))
    (define base-free-identifier=? (scheme free-identifier=?))
    (define base-free-template-identifier=? (scheme free-template-identifier=?))
    (define base-free-transformer-identifier=? (scheme free-transformer-identifier=?))
    (define base-free-label-identifier=? (scheme free-label-identifier=?))))
@(def-base base-define base-define-syntax base-define-for-syntax base-define-struct
           base-if base-cond base-case base-top-interaction
           base-open-input-file base-apply base-prop:procedure
           base-free-identifier=? base-free-template-identifier=?
           base-free-transformer-identifier=? base-free-label-identifier=?)

@(define-syntax-rule (additionals racket/base id ...)
   (begin
     (require (for-label (only-in racket/base id ...)))
     (racketblock id ...)))

@(define old-vers @elem{version 372})

@title{@bold{MzScheme}: Legacy Module Language}

@defmodule[mzscheme]{

The @racketmodname[mzscheme] language provides nearly the same
bindings as the @racketmodname[mzscheme] module of PLT Scheme
@|old-vers| and earlier.}

Unlike @|old-vers|, the @racketmodname[mzscheme] language does not
include @racket[set-car!]  or @racket[set-cdr!], and @racket[cons]
makes immutable pairs, as in @racket[scheme/base]; those changes make
modules built on @racketmodname[mzscheme] reasonably compatible with
modules built on @racketmodname[scheme/base].

Otherwise, the @racketmodname[mzscheme] language shares many bindings
with @racketmodname[scheme/base]. It renames a few bindings, such as
@racket[syntax-object->datum] instead of @racket[syntax->datum], and
it provides old versions of some syntactic forms, such as
@racket[lambda] without support for keyword and optional arguments.

@table-of-contents[]

@; ----------------------------------------

@section{Old Syntactic Forms}

@defform[(#%module-begin form ...)]{

Like @racket[#%plain-module-begin] from @racketmodname[scheme/base],
but @racket[(require-for-syntax mzscheme)] is added to the beginning
of the @racket[form] sequence, thus importing @racketmodname[mzscheme]
into the transformer environment for the module body. (In contrast,
@racketmodname[scheme/base] exports @racket[for-syntax] minimal
transformer support, while @racketmodname[scheme] exports all of
@racketmodname[scheme/base] @racket[for-syntax].}


@defform[(#%plain-module-begin form ...)]{

The same binding as @racket[#%plain-module-begin] from
@racketmodname[scheme/base].}


@defform[(#%plain-lambda formals body ...+)]{

The same binding as @racket[#%plain-lambda] in
@racketmodname[scheme/base].  (This binding was not present in
@|old-vers| and earlier.)}


@deftogether[(
@defform[(lambda formals body ...+)]
@defform[(λ formals body ...+)]
)]{

The same bindings as @racket[#%plain-lambda].}


@defform*[[(#%app proc-expr arg-expr ...)
           (#%app)]]{

The same binding as @racket[#%plain-app] from
@racketmodname[scheme/base].}

@defform*[[(#%plain-app proc-expr arg-expr ...)
           (#%plain-app)]]{

The same binding as @racket[#%app].  (This binding was not present in
@|old-vers| and earlier.)}

@defform*/subs[[(define id expr)
                (define (head args) body ...+)]
                ([head id
                       (head args)]
                 [args (code:line arg-id ...)
                       (code:line arg-id ... @#,racketparenfont{.} rest-id)])]{

Like @|base-define| in @racketmodname[scheme/base], but without
support for keyword arguments or optional arguments.}

@deftogether[(
@defform*[[(define-syntax id expr)
           (define-syntax (head args) body ...+)]]
@defform*[[(define-for-syntax id expr)
           (define-for-syntax (head args) body ...+)]]
)]{

Like @|base-define-syntax| and @|base-define-for-syntax| in
@racketmodname[scheme/base], but without support for keyword arguments
or optional arguments (i.e., @racket[head] is as for @racket[define]).}

@defform*[[(if test-expr then-expr else-expr)
           (if test-expr then-expr)]]{

Like @|base-if| in @racketmodname[scheme/base], but @racket[else-expr]
defaults to @racket[(void)].}

@deftogether[(
@defform[(cond cond-clause ...)]
@defform[(case val-expr case-clause ...)]
)]{

Like @|base-cond| and @|base-case| in @racketmodname[scheme/base], but
@racket[else] and @racket[=>] are recognized as unbound identifiers,
instead of as the @racketmodname[scheme/base] bindings. }

@defform[(fluid-let ([id expr] ...) body ...+)]{

Provides a kind of dynamic binding via mutation of the @racket[id]s.

The @racket[fluid-let] form first evaluates each @racket[expr] to
obtain an @defterm{entry value} for each @racket[id]. As evaluation
moves into @racket[body], either though normal evaluation or a
continuation jump, the current value of each @racket[id] is swapped
with the entry value. On exit from @racket[body], then the current
value and entry value are swapped again.}

@defform/subs[(define-struct id-maybe-super (field-id ...) maybe-inspector-expr)
              ([maybe-inspector-expr code:blank
                                     expr])]{

Like @base-define-struct from @racket[scheme/base], but with fewer
options. Each field is implicitly mutable, and the optional
@racket[expr] is analogous to supplying an @racket[#:inspector]
expression.}

@defform[(let-struct id-maybe-super (field-id ...) body ...+)]{

Expands to 

@racketblock[
(let ()
  (define-struct id-maybe-super (field-id ...))
  body ...+)
]}

@deftogether[(
@defform[(require raw-require-spec)]
@defform[(require-for-syntax raw-require-spec)]
@defform[(require-for-template raw-require-spec)]
@defform[(require-for-label raw-require-spec)]
@defform[(provide raw-provide-spec)]
@defform[(provide-for-syntax raw-provide-spec)]
@defform[(provide-for-label raw-provide-spec)]
)]{

Like @racket[#%require] and @racket[#%provide]. The
@racketidfont{-for-syntax}, @racketidfont{-for-template}, and
@racketidfont{-for-label} forms are translated to @racket[#%require]
and @racket[#%provide] using @racketidfont{for-syntax},
@racketidfont{for-template}, and @racketidfont{for-label} sub-forms,
respectively.}

@defform[(#%datum . datum)]{

Expands to @racket[(quote datum)], even if @racket[datum] is a
keyword.}

@defform[(#%top-interaction . form)]{

The same as @|base-top-interaction| in @racketmodname[scheme/base].}

@; ----------------------------------------

@section[#:tag "Old_Functions"]{Old Functions}

@defproc[(apply [proc procedure?] [v any/c] ... [lst list?]) any]{

Like @base-apply from @racketmodname[scheme/base], but without support
for keyword arguments.}

@defthing[prop:procedure struct-type-property?]{

Like @base-prop:procedure from @racketmodname[scheme/base], but even
if the property's value for a structure type is a procedure that
accepts keyword arguments, then instances of the structure type still
do not accept keyword arguments. (In contrast, if the property's value
is an integer for a field index, then a keyword-accepting procedure in
the field for an instance causes the instance to accept keyword
arguments.)}

@deftogether[(
@defproc[(open-input-file [file path-string?] [mode (one-of/c 'text 'binary) 'binary])
         input-port?]
@defproc[(open-output-file [file path-string?]
                           [mode (one-of/c 'text 'binary) 'binary]
                           [exists (one-of/c 'error 'append 'update
                                             'replace 'truncate 'truncate/replace) 'error])
         input-port?]
@defproc[(open-input-output-file [file path-string?]
                           [mode (one-of/c 'text 'binary) 'binary]
                           [exists (one-of/c 'error 'append 'update
                                             'replace 'truncate 'truncate/replace) 'error])
         (values input-port? output-port?)]
@defproc[(with-input-from-file [file path-string?] 
                               [thunk (-> any)]
                               [mode (one-of/c 'text 'binary) 'binary])
         any]
@defproc[(with-output-to-file [file path-string?] 
                              [thunk (-> any)]
                              [mode (one-of/c 'text 'binary) 'binary]
                              [exists (one-of/c 'error 'append 'update
                                                'replace 'truncate 'truncate/replace) 'error])
         any]
@defproc[(call-with-input-file [file path-string?] 
                               [proc (input-port? -> any)]
                               [mode (one-of/c 'text 'binary) 'binary])
         any]
@defproc[(call-with-output-file [file path-string?] 
                                [proc (output-port? -> any)]
                                [mode (one-of/c 'text 'binary) 'binary]
                                [exists (one-of/c 'error 'append 'update
                                                  'replace 'truncate 'truncate/replace) 'error])
         any]
)]{

Like @base-open-input-file, etc. from @racketmodname[scheme/base], but
@racket[mode] and @racket[exists] arguments are not keyword
arguments. When both @racket[mode] and @racket[exists] are accepted,
they are accepted in either order.}


@deftogether[(
@defproc[(syntax-object->datum [stx syntax?]) any]
@defproc[(datum->syntax-object [ctxt (or/c syntax? false/c)]
                        [v any/c]
                        [srcloc (or/c syntax? false/c
                                      (list/c any/c
                                              (or/c exact-positive-integer? false/c)
                                              (or/c exact-nonnegative-integer? false/c)
                                              (or/c exact-nonnegative-integer? false/c)
                                              (or/c exact-positive-integer? false/c))
                                      (vector/c any/c
                                                (or/c exact-positive-integer? false/c)
                                                (or/c exact-nonnegative-integer? false/c)
                                                (or/c exact-nonnegative-integer? false/c)
                                                (or/c exact-positive-integer? false/c)))]
                        [prop (or/c syntax? false/c) #f]
                        [cert (or/c syntax? false/c) #f])
          syntax?]
)]{

The same as @racket[syntax->datum] and @racket[datum->syntax].}

@deftogether[(
@defproc[(module-identifier=? [a-id syntax?][b-id syntax?]) boolean?]
@defproc[(module-transformer-identifier=? [a-id syntax?][b-id syntax?]) boolean?]
@defproc[(module-template-identifier=? [a-id syntax?][b-id syntax?]) boolean?]
@defproc[(module-label-identifier=? [a-id syntax?][b-id syntax?]) boolean?]
@defproc[(free-identifier=? [a-id syntax?][b-id syntax?]) boolean?]
)]{

The @racket[module-identifier=?], @|etc| functions are the same as
@base-free-identifier=?, @|etc| in @racketmodname[scheme/base].

The @racket[free-identifier=?] procedure returns

@racketblock[
(and (eq? (syntax-e a) (syntax-e b)) 
     (module-identifier=? a b))
]}


@defproc[(make-namespace [mode (one-of/c 'initial 'empty) 'initial]) namespace?]{

Creates a namespace with @racketmodname[mzscheme] attached. If the
@racket[mode] is empty, the namespace's top-level environment is left
empty. If @racket[mode] is @racket['initial], then the namespace's
top-level environment is initialized with
@racket[(namespace-require/copy 'mzscheme)]. See also
@racket[make-base-empty-namespace].}


@defproc[(namespace-transformer-require [req any/c]) void?]{

Equivalent to @racket[(namespace-require `(for-syntax ,req))].}

@deftogether[(
@defproc[(transcript-on [filename any/c]) any]
@defproc[(transcript-off) any]
)]{

Raises @racket[exn:fail], because the operations are not supported.}


@defproc*[([(hash-table? [v any/c]) 
            hash-table?]
           [(hash-table? [v any/c] [flag (one-of/c 'weak 'equal 'eqv)]) 
            hash-table?]
           [(hash-table? [v any/c] 
                         [flag (one-of/c 'weak 'equal 'eqv)]
                         [flag (one-of/c 'weak 'equal 'eqv)]) 
            hash-table?])]{

Returns @racket[#t] if @racket[v] like a hash table created by
@racket[make-hash-table] or @racket[make-immutable-hash-table] with
the given @racket[flag]s (or more), @racket[#f] otherwise. Each
provided @racket[flag] must be distinct and @racket['equal] cannot be
used with @racket['eqv], otherwise the @racket[exn:fail:contract]
exception is raised.}


@defproc*[([(make-hash-table) 
            hash-table?]
           [(make-hash-table [flag (one-of/c 'weak 'equal 'eqv)]) 
            hash-table?]
           [(make-hash-table [flag (one-of/c 'weak 'equal 'eqv)]
                             [flag (one-of/c 'weak 'equal 'eqv)]) 
            hash-table?])]{

Creates and returns a new hash table. If provided, each @racket[flag]
must one of the following:

 @itemize[

  @item{@indexed-racket['weak] --- creates a hash table with
   weakly-held keys via @racket[make-weak-hash],
   @racket[make-weak-hasheq], or @racket[make-weak-hasheqv].}

  @item{@indexed-racket['equal] --- creates a hash table that compares
   keys using @racket[equal?] instead of @racket[eq?] using
   @racket[make-hash] or @racket[make-weak-hash].}

  @item{@indexed-racket['eqv] --- creates a hash table that compares
   keys using @racket[eqv?] instead of @racket[eq?] using
   @racket[make-hasheqv] or @racket[make-weak-hasheqv].}

 ]

By default, key comparisons use @racket[eq?] (i.e., the hash table is
created with @racket[make-hasheq]). If the second @racket[flag] is
redundant or @racket['equal] is provided with @racket['eqv], the
@racket[exn:fail:contract] exception is raised.}


@defproc*[([(make-immutable-hash-table [assocs (listof pair?)])
            (and/c hash-table? immutable?)]
           [(make-immutable-hash-table [assocs (listof pair?)]
                                       [flag (one-of/c 'equal 'eqv)])
            (and/c hash-table? immutable?)])]{

Like @racket[make-immutable-hash], @racket[make-immutable-hasheq], or
@racket[make-immutable-hasheqv], depending on whether an
@racket['equal] or @racket['eqv] @racket[flag] is provided.}

@; ----------------------------------------

@section{Extra Libraries}

The @racketmodname[mzscheme] library re-exports
@racketmodname[racket/promise], @racketmodname[racket/tcp], and
@racketmodname[racket/udp].

@; ----------------------------------------

@section{Omitted Functions}

In addition to functions that have replacements listed in
@secref["Old_Functions"], the following functions are exported by
@racketmodname[racket/base] but not @racketmodname[mzscheme]:

@additionals[ racket/base
compose filter sort foldl foldr
remv remq remove remv* remq* remove* memf assf findf 
build-vector build-string build-list
hash-keys hash-values hash->list hash-set* hash-set*!
]
