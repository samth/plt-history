#lang scribble/doc
@require[(except-in "mz.ss" import export)
         (for-syntax scheme/base)
         (for-label scheme/require-transform
                    scheme/provide-transform)]

@(define stx-eval (make-base-eval))
@interaction-eval[#:eval stx-eval (require (for-syntax scheme/base))]

@define[(transform-time) @t{This procedure must be called during the
dynamic extent of a @tech{syntax transformer} application by the
expander, otherwise the @exnraise[exn:fail:contract].}]


@title[#:tag "stxtrans"]{Syntax Transformers}

@defproc[(make-set!-transformer [proc (syntax? . -> . syntax?)])
         set!-transformer?]{

Creates a @tech{syntax transformer} that cooperates with
@scheme[set!]. If the result of @scheme[make-set!-transformer] is
bound to @scheme[identifier] as a @tech{transformer binding}, then
@scheme[proc] is applied as a transformer when @scheme[identifier] is
used in an expression position, or when it is used as the target of a
@scheme[set!] assignment as @scheme[(set! identifier _expr)]. When the
identifier appears as a @scheme[set!] target, the entire @scheme[set!]
expression is provided to the transformer.

@examples[
#:eval stx-eval
(let ([x 1]
      [y 2])
  (let-syntax ([x (make-set!-transformer
                    (lambda (stx)
                      (syntax-case stx (set!)
                        (code:comment #, @t{Redirect mutation of x to y})
                        [(set! id v) (syntax (set! y v))]
                        (code:comment #, @t{Normal use of @scheme[x] really gets @scheme[x]})
                        [id (identifier? (syntax id)) (syntax x)])))])
    (begin
      (set! x 3)
      (list x y))))
]}


@defproc[(set!-transformer? [v any/c]) boolean?]{

Returns @scheme[#t] if @scheme[v] is a value created by
@scheme[make-set!-transformer], @scheme[#f] otherwise.}


@defproc[(set!-transformer-procedure [transformer set!-transformer?])
         (syntax? . -> . syntax?)]{

Returns the procedure that was passed to
@scheme[make-set!-transformer] to create @scheme[transformer].}


@defproc[(make-rename-transformer [id-stx syntax?])
         rename-transformer?]{

Creates a value that, when used as a @tech{transformer binding},
inserts the identifier @scheme[id-stx] in place of whatever identifier
binds the transformer, including in non-application positions, and in
@scheme[set!] expressions. Such a transformer could be written
manually, but the one created by @scheme[make-rename-transformer]
cooperates specially with @scheme[syntax-local-value] (see below).}


@defproc[(rename-transformer? [v any/c]) boolean?]{

Returns @scheme[#t] if @scheme[v] is a value created by
@scheme[make-rename-transformer], @scheme[#f] otherwise.}


@defproc[(rename-transformer-target [transformer rename-transformer?])
         syntax?]{

Returns the identifier passed to @scheme[make-rename-transformer] to
create @scheme[transformer].}


@defproc[(local-expand [stx syntax?]
                       [context-v (or/c (one-of 'expression 'top-level 'module
                                                'module-begin)
                                        list?)]
                       [stop-ids (or/c (listof identifier?) false/c)]
                       [intdef-ctx (or/c internal-definition-context?
                                         false/c)
                                          #f])
         syntax?]{

Expands @scheme[stx] in the lexical context of the expression
currently being expanded. The @scheme[context-v] argument is used as
the result of @scheme[syntax-local-context] for immediate expansions;
for a particular @tech{internal-definition context}, generate a unique
value and @scheme[cons] it onto the current result of
@scheme[syntax-local-context] if it is a list.

When an identifier in @scheme[stop-ids] is encountered by the expander
in a subexpression, expansions stops for the subexpression.  If
@scheme[#%app], @scheme[#%top], or @scheme[#%datum] appears in
@scheme[stop-ids], then application, top-level variable reference, and
literal data expressions without the respective explicit form are not
wrapped with the explicit form. If @scheme[stop-ids] is @scheme[#f]
instead of a list, then @scheme[stx] is expanded only as long as the
outermost form of @scheme[stx] is a macro (i.e., expansion does not
proceed to sub-expressions).

The optional @scheme[intdef-ctx] argument must be either @scheme[#f]
or the result of @scheme[syntax-local-make-definition-context]. In the
latter case, lexical information for internal definitions is added to
@scheme[stx] before it is expanded. The lexical information is also
added to the expansion result (because the expansion might introduce
bindings or references to internal-definition bindings).

Expansion of @scheme[stx] can use certificates for the expression
already being expanded (see @secref["stxcerts"]) , and @tech{inactive
certificates} associated with @scheme[stx] are activated for
@scheme[stx] (see @secref["stxcerts"]). Furthermore, if the
transformer is defined within a module (i.e., the current expansion
was triggered by a use of a module-defined identifier with a
@tech{transformer binding}) or if the current expression is being
expanded for the body of a module, then the expansion of @scheme[stx]
can use any identifier defined by the module.

@transform-time[]}


@defproc[(syntax-local-expand-expression [stx syntax?])
         (values syntax? syntax?)]{

Like @scheme[local-expand] given @scheme['expression] and an empty
stop list, but with two results: a syntax object for the fully
expanded expression, and a syntax object whose content is opaque. The
latter can be used in place of the former (perhaps in a larger
expression produced by a macro transformer), and when the macro
expander encouters the opaque object, it substitutes the fully
expanded expression without re-expanding it; the
@exnraise[exn:fail:syntax] if the expansion context includes bindings
or marks that were not present for the original expansion, in which
case re-expansion might produce different results. Consistent use of
@scheme[syntax-local-expand-expression] and the opaque object thus
avoids quadratic expansion times when local expansions are nested.

@transform-time[]}


@defproc[(local-transformer-expand [stx syntax?]
                       [context-v (or/c (one-of 'expression 'top-level 'module 
                                                'module-begin)
                                        list?)]
                       [stop-ids (or/c (listof identifier?) false/c)]
                       [intdef-ctx (or/c internal-definition-context?
                                         false/c)
                                          #f])
         syntax?]{

Like @scheme[local-expand], but @scheme[stx] is expanded as a
transformer expression instead of a run-time expression.}


@defproc[(local-expand/capture-lifts [stx syntax?]
                       [context-v (or/c (one-of 'expression 'top-level 'module
                                                'module-begin)
                                        list?)]
                       [stop-ids (or/c (listof identifier?) false/c)]
                       [intdef-ctx (or/c internal-definition-context?
                                         false/c)
                                          #f]
                       [lift-ctx any/c (gensym 'lifts)])
         syntax?]{

Like @scheme[local-expand], the result is a syntax object that
represents a @scheme[begin] expression. Lifted expressions---from
calls to @scheme[syntax-local-lift-expression] during the expansion of
@scheme[stx]---appear with their identifiers in @scheme[define-values]
forms, and the expansion of @scheme[stx] is the last expression in the
@scheme[begin]. The @scheme[lift-ctx] value is reported by
@scheme[syntax-local-lift-context] during local expansion. The lifted
expressions are not expanded, but instead left as provided in the
@scheme[begin] form.}


@defproc[(local-transformer-expand/capture-lifts [stx syntax?]
                       [context-v (or/c (one-of 'expression 'top-level 'module
                                                'module-begin)
                                        list?)]
                       [stop-ids (or/c (listof identifier?) false/c)]
                       [intdef-ctx (or/c internal-definition-context?
                                         false/c)
                                          #f])
         syntax?]{

Like @scheme[local-expand/capture-lifts], but @scheme[stx] is expanded
as a transformer expression instead of a run-time expression. Lifted
expressions are reported as @scheme[define-values] forms (in the
transformer environment).}


@defproc[(syntax-local-make-definition-context) internal-definition-context?]{

Creates an opaque internal-definition context value to be used with
@scheme[local-expand] and other functions. A transformer should create
one context for each set of internal definitions to be expanded, and
use it when expanding any form whose lexical context should include
the definitions. After discovering an internal @scheme[define-values]
or @scheme[define-syntaxes] form, use
@scheme[syntax-local-bind-syntaxes] to add bindings to the context.

@transform-time[]}


@defproc[(syntax-local-bind-syntaxes [id-list (listof identifier?)]
                                     [expr (or/c syntax? false/c)]
                                     [intdef-ctx internal-definition-context?])
         void?]{

Binds each identifier in @scheme[id-list] within the
internal-definition context represented by @scheme[intdef-ctx], where
@scheme[intdef-ctx] is the result of
@scheme[syntax-local-make-definition-context]. Supply @scheme[#f] for
@scheme[expr] when the identifiers correspond to
@scheme[define-values] bindings, and supply a compile-time expression
when the identifiers correspond to @scheme[define-syntaxes] bindings;
the later case, the number of values produces by the expression should
match the number of identifiers, otherwise the
@exnraise[exn:fail:contract:arity].

@transform-time[]}


@defproc[(syntax-local-value [id-stx syntax?]
                             [failure-thunk (or/c (-> any) false/c)
                                            #f]
                             [intdef-ctx (or/c internal-definition-context?
                                               false/c)
                                         #f])
         any]{

Returns the @tech{transformer binding} value of @scheme[id-stx] in
either the context asscoiated with @scheme[intdef-ctx] (if not
@scheme[#f]) or the context of the expression being expanded (if
@scheme[indef-ctx] is @scheme[#f]).  If @scheme[intdef-ctx] is
provided, it must be an extension of the context of the expression
being expanded.

If @scheme[id-stx] is bound to a rename transformer created with
@scheme[make-rename-transformer], @scheme[syntax-local-value]
effectively calls itself with the target of the rename and returns
that result, instead of the rename transformer.

If @scheme[id-stx] has no @tech{transformer binding} (via
@scheme[define-syntax], @scheme[let-syntax], etc.) in that
environment, the result is obtained by applying @scheme[failure-thunk]
if not @scheme[#f]. If @scheme[failure-thunk] is @scheme[false], the
@exnraise[exn:fail:contract].

Resolving @scheme[id-stx] can use certificates for the expression
being transformed (see @secref["stxcerts"]) as well as @tech{inactive
certificates} associated with @scheme[id-stx] (see
@secref["stxcerts"]). Furthermore, if the transformer is defined
within a module (i.e., the current transformation was triggered by a
use of a module-defined identifier) or if the current expression is
being expanded for the body of a module, then resolving
@scheme[id-stx] can access any identifier defined by the module.

@transform-time[]}


@defproc[(syntax-local-lift-expression [stx syntax?])
         identifier?]{

Returns a fresh identifier, and cooperates with the @scheme[module],
@scheme[letrec-syntaxes+values], @scheme[define-syntaxes],
@scheme[begin-for-syntax], and top-level expanders to bind the
generated identifier to the expression @scheme[stx].

A run-time expression within a module is lifted to the module's top
level, just before the expression whose expansion requests the
lift. Similarly, a run-time expression outside of a module is lifted
to a top-level definition. A compile-time expression in a
@scheme[letrec-syntaxes+values] or @scheme[define-syntaxes] binding is
lifted to a @scheme[let] wrapper around the corresponding right-hand
side of the binding. A compile-time expression within
@scheme[begin-for-syntax] is lifted to a @scheme[define-for-syntax]
declaration just before the requesting expression.

Other syntactic forms can capture lifts by using
@scheme[local-expand/capture-lifts] or
@scheme[local-transformer-expand/capture-lifts].

@transform-time[]}


@defproc[(syntax-local-lift-context)
         any/c]{

Returns a value that represents the target for expressions lifted via
@scheme[syntax-local-lift-expression]. That is, for different
transformer calls for which this procedure returns the same value (as
determined by @scheme[eq?]), lifted expressions for the two
transformer are moved to the same place. Thus, the result is useful
for caching lift information to avoid redundant lifts.

@transform-time[]}




@defproc[(syntax-local-lift-module-end-declaration [stx syntax?])
         void?]{

Cooperates with the @scheme[module] form to insert @scheme[stx] as
a top-level declaration at the end of the module currently being
expanded. If the current expression being transformed is not within a
@scheme[module] form, or if it is not a run-time expression, then the
@exnraise[exn:fail:contract]. If the current expression being
transformed is not in the module top-level, then @scheme[stx] is
eventually expanded in an expression context.

@transform-time[]}


@defproc[(syntax-local-name) (or/c symbol? false/c)]{

Returns an inferred name for the expression position being
transformed, or @scheme[#f] if no such name is available. See also
@secref["infernames"].

@transform-time[]}


@defproc[(syntax-local-context)
         (or/c (one-of 'expression 'top-level 'module 'module-begin)
               list?)]{

Returns an indication of the context for expansion that triggered a
@tech{syntax transformer} call. See @secref["expand-context-model"]
for more information on contexts.

The symbol results indicate that the expression is being expanded for
an @tech{expression context}, a @tech{top-level context}, a
@tech{module context}, or a @tech{module-begin context}.

A list result indicates expansion in an @tech{internal-definition
context}. The identity of the lists's first element (i.e., its
@scheme[eq?]ness) reflects the identity of the internal-definition
context; in particular two transformer expansions receive the same
first value if and only if they are invoked for the same
internal-definition context. Later values in the list similarly
identify internal-definition contexts that are still being expanded,
and that required the expansion of nested internal-definition
contexts.

@transform-time[]}


@defproc[(syntax-local-module-exports [mod-path module-path?]) 
         (values (listof symbol?) (listof symbol?) (listof symbol?))]{

Returns three lists of symbols that represent the @scheme[provide]d
bindings of the module named by @scheme[mod-path]. The first list
corresponds to the @tech{phase level} 0 exports of the module, the
second list corresponds to the @tech{phase level} -1 exports of the
module, and the last list corresponds to the @tech{label phase level}
exports of the module.

@transform-time[]}


@defproc[(syntax-local-get-shadower [id-stx identifier?]) identifier?]{

Returns @scheme[id-stx] if no binding in the current expansion context
shadows @scheme[id-stx], if @scheme[id-stx] has no module bindings in
its lexical information, and if the current expansion context is not a
@tech{module context}.

If a binding of @scheme[inner-identifier] shadows @scheme[id-stx], the
result is the same as @scheme[(syntax-local-get-shadower
@scheme[inner-identifier])], except that it has the location and
properties of @scheme[id-stx].

Otherwise, the result is the same as @scheme[id-stx] with its module
bindings (if any) removed from its lexical information, and the
lexical information of the current @tech{module context} (if any)
added.

Thus, the result is an identifier corresponding to the innermost
shadowing of @scheme[id-stx] in the current context if its shadowed,
and a module-contextless version of @scheme[id-stx] otherwise.

@transform-time[]}


@defproc[(syntax-local-certifier [active? boolean? #f])
         (syntax? (any/c (or/c procedure? false/c)) 
                  . opt-> . syntax?)]{

Returns a procedure that captures any certificates currently available
for @scheme[syntax-local-value] or @scheme[local-expand]. The
procedure accepts one to three arguments: @scheme[_stx] (required),
@scheme[_key] (optional), and @scheme[_intro] (optional). The
procedure's result is a syntax object like @scheme[stx], except that
it includes the captured certificates as inactive (see
@secref["stxcerts"]) if @scheme[active?] is @scheme[#f] (the default)
or active otherwise. If @scheme[key] is supplied and not @scheme[#f],
it is associated with each captured certificate for later use through
@scheme[syntax-recertify]. If @scheme[_intro] is supplied, and if it
is not @scheme[#f] (the default), then it must be a procedure created
by @scheme[make-syntax-introducer], in which case the certificate
applies only to parts of @scheme[stx] that are marked as introduced by
@scheme[_intro].

Supply @scheme[#t] for @scheme[active?] when the syntax to be
certified can be safely used in any context by any party, and where
access to the syntax object should not confer any additional
access. Supply @scheme[#f] for @scheme[active?] when the syntax to be
certified is not accessible to parties that might abuse the access
that the certificate provides, and when the certified syntax
eventually appears (via macro expansion) within a larger expression
from which it cannot be safely extracted by other parties.

@transform-time[]}

@defproc[(syntax-transforming?) boolean?]{

Returns @scheme[#t] during the dynamic extent of a @tech{syntax
transformer} application by the expander, @scheme[#f] otherwise.}


@defproc[(syntax-local-introduce [stx syntax?]) syntax?]{

Produces a syntax object that is like @scheme[stx], except that a
@tech{syntax mark} for the current expansion is added (possibly
canceling an existing mark in parts of @scheme[stx]). See
@secref["transformer-model"] for information on @tech{syntax
marks}.

@transform-time[]}


@defproc[(make-syntax-introducer) (syntax? . -> . syntax?)]{

Produces a procedure that behaves like
@scheme[syntax-local-introduce], but using a fresh @tech{syntax
mark}. Multiple applications of the same
@scheme[make-syntax-introducer] result procedure use the same mark,
and different result procedures use distinct marks.}


@defproc[(syntax-local-transforming-module-provides?) boolean?]{

Returns @scheme[#t] while a @tech{provide transformer} is running (see
@scheme[make-provide-transformer]) or while a @schemeidfont{expand} sub-form of
@scheme[#%provide] is expanded, @scheme[#f] otherwise.}


@defproc[(syntax-local-module-defined-identifiers) 
         (values (listof identifier?) (listof identifier?))]{

Can be called only while
@scheme[syntax-local-transforming-module-provides?] returns
@scheme[#t].

It returns two lists of identifiers corresponding to all definitions
within the module being expanded. This information is used for
implementing @scheme[provide] sub-forms like @scheme[all-defined-out].

The first result list corresponds to @tech{phase} 0 (i.e., normal)
definitions, and the second corresponds to @tech{phase} -1 (i.e.,
for-syntax) definitions.}


@defproc[(syntax-local-module-required-identifiers
          [mod-path (or/c module-path? false/c)]
          [normal-imports? any/c]
          [syntax-imports? any/c]
          [label-imports? any/c])
         (values (listof identifier?)
                 (listof identifier?) 
                 (listof identifier?))]{

Can be called only while
@scheme[syntax-local-transforming-module-provides?] returns
@scheme[#t].

It returns three lists of identifiers corresponding to all bindings
imported into the module being expanded using the module path
@scheme[mod-path], or all modules if @scheme[mod-path] is
@scheme[#f]. This information is used for implementing
@scheme[provide] sub-forms like @scheme[all-from-out].

The first result list corresponds to @tech{phase level} 0 (i.e.,
normal) bindings, and the second list corresponds to @tech{phase
level} -1 (i.e., for-syntax) bindings, and the last list corresponds
corresponds to @tech{label phase level} (i.e., for-label) bindings.

The @scheme[normal-imports?], @scheme[syntax-imports?], and
@scheme[label-imports?] arguments determine whether each of normal,
@scheme[for-syntax], and @scheme[for-label] @scheme[require]s are
considered in building the result lists. Note that normal
@scheme[require]s can add to all three lists, while
@scheme[for-syntax] and @scheme[for-label] @scheme[require]s
contribute only to one of the latter two lists, respectively.}

@; ----------------------------------------------------------------------

@section[#:tag "require-trans"]{@scheme[require] Transformers}

@note-lib-only[scheme/require-transform]

A @tech{transformer binding} whose value is a structure with the
@scheme[prop:require-transformer] property implements a derived
@scheme[_require-spec] for @scheme[require].

The transformer is called with the syntax object representing its use
as a @scheme[_require-spec] within a @scheme[require] form, and the
result must be two lists: a list of @scheme[import]s and a list of
@scheme[import-source]s.

If the derived form contains a sub-form that is a
@scheme[_require-spec], then it can call @scheme[expand-import] to
transform the sub-@scheme[_require-spec] to lists of imports and
import sources.


@defproc[(expand-import [stx syntax?])
         (values (listof import?) 
                 (listof import-source?))]{

Expands the given @scheme[_require-spec] to lists of imports and
import sources.}


@defproc[(make-require-transformer [proc ((syntax?) . ->* . ((listof import?)
                                                             (listof import-source?)))])
         require-transformer?]{

Creates a @deftech{require transformer} (i.e., a structure with the
@scheme[prop:require-transformer] property) using the given procedure
as the transformer.}


@defthing[prop:require-transformer struct-type-property?]{

A property to identify @scheme[require] transformers. The property
value must be a procedure that takes a syntax object and returns
import and import-source lists.}


@defproc[(require-transformer? [v any/c]) boolean?]{

Returns @scheme[#t] if @scheme[v] has the
@scheme[prop:require-transformer] property, @scheme[#f] otherwise.}


@defstruct[import ([local-id identifier?]
                   [src-sym symbol?]
                   [src-mod-path module-path?]
                   [orig-stx syntax?]
                   [mode (one-of/c 'run 'syntax 'template 'label)]
                   [req-mode (one-of/c 'run 'syntax 'template 'label)])]{

A structure representing a single imported identifier:

@itemize{

 @item{@scheme[local-id] --- the identifier to be bound within the
       importing module.}

 @item{@scheme[src-sym] --- the external name of the binding as
       exported from its source module.}

 @item{@scheme[src-mod-path] --- a @tech{module path} (relative to the
       importing module) for the source of the imported binding.}

 @item{@scheme[orig-stx] --- a @tech{syntax object} for the source of
       the import, used for error reporting.}

 @item{@scheme[mode] --- the @tech{phase level} of the binding in the
       importing module.}

 @item{@scheme[req-mode] --- the @tech{phase level} shift of the
       import relative to the exporting module. Since the underlying
       module system currently allows only for-run @scheme[require]s
       to import into other @tech{phase levels}, if this value is not
       @scheme['run], then it must match the @scheme[mode] field's
       value.}

}}


@defstruct[import-source ([mod-path-stx (and/c syntax?
                                               (lambda (x)
                                                 (module-path? (syntax->datum x))))]
                          [mode (one-of/c 'run 'syntax 'template 'label)])]{

A structure representing an imported module, which must be
instantiated or visited even if no binding is imported into a module.

@itemize{

 @item{@scheme[mod-path-stx] --- a @tech{module path} (relative
       to the importing module) for the source of the imported binding.}

 @item{@scheme[mode] --- the @tech{phase level} shift the import.}

}}


@; ----------------------------------------------------------------------

@section[#:tag "provide-trans"]{@scheme[provide] Transformers}

@note-lib-only[scheme/provide-transform]

A @tech{transformer binding} whose value is a structure with the
@scheme[prop:provide-transformer] property implements a derived
@scheme[_provide-spec] for @scheme[provide].

The transformer is called with the syntax object representing its use
as a @scheme[_provide-spec] within a @scheme[provide] form and a list
of symbols representing the export modes specified by enclosing
@scheme[_provide-spec]s. The result must be a list of
@scheme[export]s.

If the derived form contains a sub-form that is a
@scheme[_provide-spec], then it can call @scheme[expand-export] to
transform the sub-@scheme[_provide-spec] to a lists of exports.


@defproc[(expand-export [stx syntax?] [modes (listof (one-of/c 'run 'syntax 'label))])
         (listof export?)]{

Expands the given @scheme[_provide-spec] to a list of exports. The
@scheme[modes] list controls the expansion of
sub-@scheme[_provide-specs]; for example, an identifier refers to a
@tech{phase level} 0 binding unless the @scheme[modes] list specifies
otherwise. Normally, @scheme[modes] is either empty or contains a
single symbol.}


@defproc[(make-provide-transformer [proc (syntax? (listof (one-of/c 'run 'syntax 'label)) 
                                          . -> . (listof export?))])
         provide-transformer?]{

Creates a @deftech{provide transformer} (i.e., a structure with the
@scheme[prop:provide-transformer] property) using the given procedure
as the transformer.}


@defthing[prop:provide-transformer struct-type-property?]{

A property to identify @scheme[provide] transformers. The property
value must be a procedure that takes a syntax object and mode list and
returns an export list.}


@defproc[(provide-transformer? [v any/c]) boolean?]{

Returns @scheme[#t] if @scheme[v] has the
@scheme[prop:provide-transformer] property, @scheme[#f] otherwise.}


@defstruct[export ([local-id identifier?]
                   [out-sym symbol?]
                   [orig-stx syntax?]
                   [protect? any/c]
                   [mode (one-of/c 'run 'syntax 'label)])]{

A structure representing a single imported identifier:

@itemize{

 @item{@scheme[local-id] --- the identifier that is bound within the
       exporting module.}

 @item{@scheme[out-sym] --- the external name of the binding.}

 @item{@scheme[orig-stx] --- a @tech{syntax object} for the source of
       the export, used for error reporting.}

 @item{@scheme[protect?] --- indicates whether the identifier should
       be protected (see @secref["modprotect"]).}

 @item{@scheme[mode] --- the @tech{phase level} of the binding in the
       exporting module.}

}}
