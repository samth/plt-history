#lang scribble/manual

@begin[(require "../utils.rkt" scribble/eval scriblib/footnote
                racket/sandbox)
       (require (for-label (only-meta-in 0 [except-in typed/racket for])
                           (only-in racket/base for)
                           racket/list srfi/14
                           version/check))]

@(define the-eval (make-base-eval))
@(the-eval '(require (except-in typed/racket #%top-interaction #%module-begin)))
@(define the-top-eval (make-base-eval))
@(the-top-eval '(require (except-in typed/racket #%module-begin)))

@(define-syntax-rule (ex . args)
   (examples #:eval the-top-eval . args))


@title{Typed Regions}

The @racket[with-type] for allows for localized Typed Racket regions in otherwise untyped code.

@defform*/subs[[(with-type result-spec fv-clause body ...+)
                (with-type export-spec fv-clause body ...+)]
              ([fv-clause code:blank
                          (code:line #:freevars ([id fv-type] ...))]
               [result-spec (code:line #:result type)]
               [export-spec ([export-id export-type] ...)])]{
The first form, an expression, checks that @racket[body ...+] has the type @racket[type].
If the last expression in @racket[body ...+] returns multiple values, @racket[type] must
be a type of the form @racket[(values t ...)].
Uses of the result values are appropriately checked by contracts generated from 
@racket[type].

The second form, which can be used as a definition, checks that each of the @racket[export-id]s 
has the specified type.  These types are also enforced in the surrounding code with contracts.

The @racket[id]s are assumed to 
have the types ascribed to them; these types are converted to contracts and checked dynamically.

@examples[#:eval the-eval
(with-type #:result Number 3)

((with-type #:result (Number -> Number)
   (lambda: ([x : Number]) (add1 x)))
 #f)

(let ([x "hello"])
  (with-type #:result String
    #:freevars ([x String])
    (string-append x ", world")))

(let ([x 'hello])
  (with-type #:result String
    #:freevars ([x String])
    (string-append x ", world")))

(with-type ([fun (Number -> Number)]
            [val Number])
  (define (fun x) x)
  (define val 17))

(fun val)]}

