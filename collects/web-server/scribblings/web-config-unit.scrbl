#lang scribble/doc
@(require "web-server.ss")

@title[#:tag "web-config-unit.ss"
             #:style 'toc]{Web Config Unit}
@(require (for-label web-server/web-config-unit
                     web-server/configuration/namespace
                     web-server/configuration/configuration-table
                     web-server/configuration/configuration-table-structs
                     web-server/private/util
                     web-server/servlet/setup
                     scheme/tcp
                     (prefix-in servlets: web-server/dispatchers/dispatch-servlets)
                     web-server/web-config-sig))

The @web-server offers a unit-based approach to configuring the server.

@local-table-of-contents[]

@section{Configuration Signature}

@defmodule[web-server/web-config-sig]{

@defsignature[web-config^ ()]{

@signature-desc{
Provides contains the following identifiers.
}

@defthing[max-waiting integer?]{
 Passed to @scheme[tcp-accept].
}

@defthing[virtual-hosts (string? . -> . host?)]{
 Contains the configuration of individual virtual hosts.
}

@defthing[initial-connection-timeout integer?]{
 Specifies the initial timeout given to a connection.
}

@defthing[port port-number?]{
 Specifies the port to serve HTTP on.
}

@defthing[listen-ip (or/c false/c string?)]{
 Passed to @scheme[tcp-listen].
}

@defthing[make-servlet-namespace make-servlet-namespace/c]{
 Passed to @scheme[servlets:make] through @scheme[make-default-path->servlet].
}
}
             
}

@section{Configuration Units}

@defmodule[web-server/web-config-unit]{

@defproc[(configuration-table->web-config@ [path path-string?]
                                           [#:port port (or/c false/c port-number?) #f]
                                           [#:listen-ip listen-ip (or/c false/c string?) #f]
                                           [#:make-servlet-namespace make-servlet-namespace make-servlet-namespace/c (make-make-servlet-namespace)])
         (unit/c (import) (export web-config^))]{
 Reads the S-expression at @scheme[path] and calls
 @scheme[configuration-table-sexpr->web-config@] appropriately.
}

@defproc[(configuration-table-sexpr->web-config@ [sexpr list?]
                                                 [#:web-server-root web-server-root path-string?
                                                                    (directory-part default-configuration-table-path)]
                                                 [#:port port (or/c false/c port-number?) #f]
                                                 [#:listen-ip listen-ip (or/c false/c string?) #f]
                                                 [#:make-servlet-namespace make-servlet-namespace make-servlet-namespace/c
                                                                           (make-make-servlet-namespace)])
         (unit/c (import) (export web-config^))]{
 Parses @scheme[sexpr] as a configuration-table and constructs a @scheme[web-config^] unit.
}

}
