#lang scribble/doc
@(require "utils.ss"
          (for-label ffi/unsafe/atomic))

@title{Atomic Execution}

@defmodule[ffi/unsafe/atomic]

@deftogether[(
@defproc[(start-atomic) void?]
@defproc[(end-atomic) void?]
)]{

Disables and enables context switches at the level of Scheme
threads. Calls to @scheme[start-atomic] and @scheme[end-atomic] can be
nested.

Using @scheme[call-as-atomic] is somewhat safer, in that
@scheme[call-as-atomic] correctly catches exceptions and re-raises
them after exiting atomic mode. For simple uses, however,
@scheme[start-atomic] and @scheme[end-atomic] are faster.}


@defproc[(call-as-atomic [thunk (-> any)]) any]{

Calls @scheme[thunk] in atomic mode. If @scheme[thunk] raises and
exception, the exception is caught and re-raised after exiting atomic
mode.

When @scheme[call-as-atomic] is used in the dynamic extent of
@scheme[call-as-atomic], then @scheme[thunk] is simply called directly
(as a tail call).}


@defproc[(call-as-nonatomic [thunk (-> any)]) any]{

Within the dynamic extent of a @scheme[call-as-atomic], calls
@scheme[thunk] in non-atomic mode. Beware that the current thread
maybe suspended or terminated by other threads during @scheme[thunk],
in which case the call never returns.

When used not in the dynamic extent of @scheme[call-as-atomic],
@scheme[call-as-nonatomic] raises @scheme[exn:fail:contract].}

