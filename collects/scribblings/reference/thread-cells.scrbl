#lang scribble/doc
@(require "mz.rkt")

@title[#:tag "threadcells"]{Thread Cells}

A @defterm{@tech{thread cell}} contains a thread-specific value; that
is, it contains a specific value for each thread, but it may contain
different values for different threads. A thread cell is created with
a default value that is used for all existing threads. When the cell's
content is changed with @scheme[thread-cell-set!], the cell's value
changes only for the current thread. Similarly,
@scheme[thread-cell-ref] obtains the value of the cell that is
specific to the current thread.

A thread cell's value can be @defterm{@tech{preserved}}, which means
that when a new thread is created, the cell's initial value for the
new thread is the same as the creating thread's current value. If a
thread cell is non-preserved, then the cell's initial value for a
newly created thread is the default value (which was supplied when the
cell was created).

Within the current thread, the current values of all preserved threads
cells can be captured through
@scheme[current-preserved-thread-cell-values]. The captured set of
values can be imperatively installed into the current thread through
another call to @scheme[current-preserved-thread-cell-values]. The
capturing and restoring threads can be different.


@defproc[(thread-cell? [v any/c]) boolean?]{
Returns @scheme[#t] if @scheme[v] is a @tech{thread cell}, 
@scheme[#f] otherwise.}


@defproc[(make-thread-cell [v any/c] [preserved? any/c #f]) thread-cell?]{

Creates and returns a new thread cell. Initially, @scheme[v] is the
cell's value for all threads. If @scheme[preserved?] is true, then the
cell's initial value for a newly created threads is the creating
thread's value for the cell, otherwise the cell's value is initially
@scheme[v] in all future threads.}


@defproc[(thread-cell-ref [cell thread-cell?]) any]{Returns the
current value of @scheme[cell] for the current thread.}

@defproc[(thread-cell-set! [cell thread-cell?] [v any/c]) any]{Sets the
value in @scheme[cell] to @scheme[v] for the current thread.}

 @examples[
(define cnp (make-thread-cell '(nerve) #f))
(define cp (make-thread-cell '(cancer) #t))

(thread-cell-ref cnp)
(thread-cell-ref cp)

(thread-cell-set! cnp '(nerve nerve))
(thread-cell-set! cp '(cancer cancer))

(thread-cell-ref cnp)
(thread-cell-ref cp)

(define ch (make-channel))
(thread (lambda ()
          (channel-put ch (thread-cell-ref cnp))
          (channel-put ch (thread-cell-ref cp))
          (channel-get ch) ; to wait
          (channel-put ch (thread-cell-ref cp))))

(channel-get ch)
(channel-get ch)

(thread-cell-set! cp '(cancer cancer cancer))

(thread-cell-ref cp)
(channel-put ch 'ok)
(channel-get ch)
]

@defproc*[([(current-preserved-thread-cell-values) any]
           [(current-preserved-thread-cell-values [thread-cell-vals any/c]) void?])]{

When called with no arguments, this procedure produces a
@scheme[thread-cell-vals] that represents the current values (in the
current thread) for all preserved thread cells.

When called with a @scheme[thread-cell-vals] generated by a previous
call to @scheme[current-preserved-thread-cell-values], the values of
all preserved thread cells (in the current thread) are set to the
values captured in @scheme[thread-cell-vals]; if a preserved thread
cell was created after @scheme[thread-cell-vals] was generated, then
the thread cell's value for the current thread reverts to its initial
value.}

