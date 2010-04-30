#lang scribble/doc
@(require scribble/manual
          scribble/eval
          "guide-utils.ss"
          "contracts-utils.ss"
          (for-label racket/contract))

@title[#:tag "contracts-exists"]{Abstract Contracts using @racket[#:exists] and @racket[#:∃]}

The contract system provides existential contracts that can
protect abstractions, ensuring that clients of your module
cannot depend on the precise representation choices you make
for your data structures.

#; @ctc-section{Getting Started, with a Stack Example}

@margin-note{
  You can type @racket[#:exists] instead of @racket[#:∃] if you 
cannot easily type unicode characters; in DrRacket, typing
@tt{\exists} followed by either alt-\ or control-\ (depending
on your platform) will produce @racket[∃].}
The @racket[provide/contract] form allows you to write
@racketblock[#:∃ _name-of-a-new-contract] as one of its clauses. This declaration
introduces the variable @racket[_name-of-a-new-contract], binding it to a new
contract that hides information about the values it protects.

As an example, consider this (simple) implementation of a stack datastructure:
@racketmod[racket
           (define empty '())
           (define (enq top queue) (append queue (list top)))
           (define (next queue) (car queue))
           (define (deq queue) (cdr queue))
           (define (empty? queue) (null? queue))
           
           (provide/contract
            [empty (listof integer?)]
            [enq (-> integer? (listof integer?) (listof integer?))]
            [next (-> (listof integer?) integer?)]
            [deq (-> (listof integer?) (listof integer?))]
            [empty? (-> (listof integer?) boolean?)])]
This code implements a queue purely in terms of lists, meaning that clients
of this data structure might use @racket[car] and @racket[cdr] directly on the
data structure (perhaps accidentally) and thus any change in the representation
(say to a more efficient representation that supports amortized constant time
enqueue and dequeue operations) might break client code.

To ensure that the stack representation is abstact, we can use @racket[#:∃] in the
@racket[provide/contract] expression, like this:
@racketblock[(provide/contract
              #:∃ stack
              [empty stack]
              [enq (-> integer? stack stack)]
              [next (-> stack integer?)]
              [deq (-> stack (listof integer?))]
              [empty? (-> stack boolean?)])]

Now, if clients of the data structure try to use @racket[car] and @racket[cdr], they
receive an error, rather than mucking about with the internals of the queues.

See also @ctc-link["exists-gotcha"].
