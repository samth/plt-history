#lang scribble/doc
@(require scribble/manual
          (for-label mysterx
                     scheme/class
                     scheme/base
                     scheme/contract))

@title{@bold{MysterX}: Using Windows COM Objects in Scheme}

@bold{MysterX} is a toolkit for building Windows applications from
ActiveX and COM components, using Scheme as glue code.  Dynamic HTML
is used for component presentation and event-handling.  MysterX
requires Internet Explorer (IE) 4 or later to be installed.
Distributed COM (DCOM) for your version of Windows is also required.
Recent versions of Windows come with DCOM; DCOM packages for Windows
95 and 98 are made available separately.

@table-of-contents[]

@section{Installation}

Two Windows DLLs support low-level operations in MysterX:
@filepath{myspage.dll} and @filepath{myssink.dll}.  Both are installed
in the registry (using @exec{regsvr32.exe}) when Setup PLT runs the
the MysterX post-installer. If you move the location of your PLT
installation, you may need to re-run Setup PLT to make MysterX
work. Neither of these DLLs is specific to a PLT Scheme version, so
it's ok for one version of PLT Scheme to use the DLLs registered by
another.

@margin-note{Prior to version 369.4, @filepath{myssink.dll} was
version-specific. Its GUID was changed when it was made
version-independent.}

If you build a stand-alone executable that uses MysterX, you need to
specifically include @filepath{myspage.dll} and @filepath{myssink.dll}
with your distribution, and the DLLs will need to be registered on the
end user's machine. One way to do that is to include the following
little setup program (as an executable) in your distribution:

@schemeblock[
  (module setup scheme/base
    (require mzlib/runtime-path
             mzlib/process)

    (code:comment #, @t{Ensure that DLLs are included with the distribution:})
    (define-runtime-path myspage-dll '(so "myspage"))
    (define-runtime-path myssink-dll '(so "myssink"))

    (code:comment #, @t{Register the DLLs:})
    (define regsvr32 
      (path->string (find-executable-path "regsvr32.exe" #f)))
    (system* regsvr32 (path->string myspage-dll))
    (system* regsvr32 (path->string myssink-dll)))
]


@section{Running a Demo}

Try 

@schemeblock[
  (require mysterx/mxdemo)
]

The demo requires the MSCal Calendar control.  The calendar control is
normally installed with Microsoft Office, but it can also be
downloaded from elsewhere; look for @filepath{mscal.ocx}.


@section{Loading}

Load the MysterX module with 

@schemeblock[
  (require mysterx)
]

Because some MysterX code relies on the @schememodname[scheme/class]
class system, you may also need

@schemeblock[
  (require mzlib/class)
]

Several MysterX procedures take HTML strings as input.  The
@schememodname[xml] library provides procedures that convert Scheme
syntax into XML strings.  You may find using these procedures useful
in creating HTML strings for use by MysterX.

@; ----------------------------------------------------------------------

@section[#:style 'toc]{Functions and Classes}

@defmodule[mysterx]

@local-table-of-contents[]

@; ----------------------------------------

@subsection{Version}

@defproc[(mx-version) string?]{

 Returns a string indicating the version of MysterX.}

@; ----------------------------------------
@subsection{Browsers}

 A MysterX application consists of one or more browsers, which are
 instances of the class @scheme[mx-browser%].

@defproc[(block-while-browsers)  void?]{
 
 Blocks until all browser windows have been closed or hidden, using
 the show method of @scheme[mx-browser%].  This is useful when a
 MysterX program file is run as a script, to prevent @exec{mzscheme}
 or @exec{mred} from closing prematurely.}

@defclass[mx-browser% object% ()]{

@defconstructor[([label string? "MysterX"] 
                 [width (or/c exact-nonnegative-integer? (one-of/c 'default)) 'default]
                 [height (or/c exact-nonnegative-integer? (one-of/c 'default)) 'default]
                 [x (or/c exact-integer? (one-of/c 'default)) 'default]
                 [y (or/c exact-integer? (one-of/c 'default)) 'default]
                 [style (listof (any-of/c 'iconize 'maximize 'no-system-menu
                                          'no-thick-border 'scrollbars))])]{

  Creates an instance of a MysterX browser. The @scheme[label]
  argument is a string for the document caption, with default .  The
  @scheme[width], @scheme[height], @scheme[x], and @scheme[y] give the
  size and placement of the browser window on the desktop, with
  defaults provided by Windows.  When @scheme[style-list] includes
  @scheme['scrollbars], the vertical scrollbar is disabled if
  scrolling is unnecessary, and the horizontal scrollbar disappears if
  scrolling is unnecessary.

  Although the browser window cannot be hidden initially, it can be
  iconized.  The @method[mx-browser% restore] method can be used to
  restore an iconized browser to an ordinary window.}

@defmethod[(current-document) (is-a?/c mx-document%)]{

 Returns the current document in the browser.}

@defmethod[(print-document) void?]{

 Prints the document displayed by the browser to the default
 printer.  As an unintentional side-effect, the browser 
 window is minimized.}

@defmethod[(show [show? any/c]) void?]{

  If @scheme[show?] is @scheme[#f], the browser window is hidden.
  Otherwise, the window is shown.}

@defmethod[(navigate [url string?]) string?]{

  Navigates the browser to the URL given by @scheme[url].
  Any DHTML changes to the page associated with the URL 
  are not shown.  Returns a string that is the actual URL 
  navigated to.}

@defmethod[(navigate/status [url string?]) 
           (list/c string? (or/c false/c integer? (one-of/c 'no-status)))]{

  Navigates the browser to the URL given by @scheme[url].
  Any DHTML changes to the page associated with the URL 
  are not shown.  Returns a list, whose first element string that 
  is the actual URL navigated to, and whose second element is
  a status code, one of: @scheme[#f], indicating no status could be 
  obtained; a number, such as @scheme[200] or @scheme[404], indicating the 
  http status; or @scheme['no-status], indicating that @scheme[url] does not 
  denote a URL with the ``http'' scheme.}

@defmethod[(go-back) string?]{

  Navigates the browser back to a URL within its history list.
  Any DHTML changes to the page associated with the URL 
  are not shown.  Returns a string that is the actual URL 
  navigated to.}

@defmethod[(go-forward) string?]{

  Navigates the browser forward to a URL within its history list.  
  Any DHTML changes to the page associated with the URL are 
  not shown.  Returns a string that is the actual URL 
  navigated to.}

@defmethod[(refresh) boolean?]{

  Refreshes the document in the browser.  Returns @scheme[#t]
  if the refresh is successful, @scheme[#f] otherwise.}

@defmethod[(iconize) void?]{

  Iconizes the browser window.}

@defmethod[(restore) void?]{

  Restores the browser window, if it has been iconized.}

@defmethod[(current-url) string?]{

  Returns a string indicating the currently displayed URL. }

@defmethod[(register-event-handler [elem (is-a?/c mx-element%)] 
                                   [f ((is-a?/c mx-event%) . -> . any)])
           void?]{

  Registers an event handler for the HTML element @scheme[elem].
  The result of @scheme[f] is discarded.}

@defmethod[(unregister-event-handler [elem (is-a?/c mx-element%)]) void?]{

  Unregisters an event handler for an HTML element
  in the browser.}

@defmethod[(handle-events) void?]{

  Creates a thread to handle events using the registered event
  handlers.}

@defmethod[(stop-handling-events) void?]{

  Kills the thread currently handling events for
  the browser.}

}

@; ----------------------------------------

@subsection{Documents}

 A browser contains one document at a time.  If 
 hyperlinks are clicked, or the navigation methods
 (navigate, go-forward, go-back) are used, the 
 document changes.

@defclass[mx-document% object% ()]{

@defmethod[(insert-html [html string?]) void?]{

  Inserts the specified HTML string at the
  beginning of the document.}

@defmethod[(append-html [html string?]) void?]{

  Appends the specified HTML string at the end of
  the document.}

@defmethod[(replace-html [html string?]) void?]{

  Replace the current HTML in the document with
  the specified HTML string.}

@defmethod[(objects) (listof (is-a?/c com-object%))]{

  Returns a list of COM objects, including ActiveX
  controls, that occur in the document.  The order
  of the objects is the same as in the document.}

@defmethod[(insert-object-from-coclass [coclass string?]
                                       [width exact-integer?]
                                       [height exact-integer?]
                                       [size (one-of/c 'pixels 'percent) 'pixels])
           com-object%]{

  Inserts a COM object with class @scheme[coclass] at the beginning of
  the document.  The optional @scheme[size] argument gives an
  interpretation for the width and height, where @scheme['percent]
  indicates that the width and height are a fixed percentage of the
  document window size.  An instance of <com-object> is returned.}

@defmethod[(insert-object-from-progid [progid string?]
                                       [width exact-integer?]
                                       [height exact-integer?]
                                       [size (one-of/c 'pixels 'percent) 'pixels])
           com-object%]{

  Like @method[mx-document% insert-object-from-coclass], but with a
  ProgID instead of a COM class.}

@defmethod[(append-object-from-coclass [coclass string?]
                                       [width exact-integer?]
                                       [height exact-integer?]
                                       [size (one-of/c 'pixels 'percent) 'pixels])
           com-object%]{

 Like @method[mx-document% insert-object-from-coclass], but adds to the
 end of the document.}


@defmethod[(append-object-from-progid [progid string?]
                                       [width exact-integer?]
                                       [height exact-integer?]
                                       [size (one-of/c 'pixels 'percent) 'pixels])
           com-object%]{

 Like @method[mx-document% insert-object-from-progid], but adds to the
 end of the document.}

@defmethod[(title) string?]{

  Returns a string indicating the document's title, that is,
  the text that appears within HTML TITLE tags.  If the document 
  has no title, the empty string is returned.}

@defmethod[(find-element [tag string?]
                         [id string?]
                         [index exact-nonnegative-integer? 0])
           (is-a?/c mx-element%)]{

  Returns an object that encapsulates an HTML element, where
  @scheme[tag] names an HTML tag, and @scheme[id] names the @scheme["id"]
  attribute of the HTML element.  The @scheme[index] is a nonnegative
  integer indicating the zero-based index of the element among all
  elements with the same @scheme[tag] and @scheme[id].  The ordering
  of elements is defined by Internet Explorer.  The requested element
  must be within the document's @scheme["body"] tags or the
  @scheme["body"] element itself.}

@defmethod[(find-element-by-id-or-name
                         [id string?]
                         [index exact-nonnegative-integer? 0])
           (is-a?/c mx-element%)]{

  Returns an object that encapsulates an HTML element, where
  @scheme[id] names either the @scheme["id"] or @scheme["name"]
  attribute of the HTML element.  The @scheme[index] is a nonnegative
  integer indicating the zero-based index of the element among all
  elements with the same @scheme["id"] or @scheme["name"].  The ordering
  of elements is defined by Internet Explorer.  The requested element
  must be within the document's @scheme["body"] tags or the
  @scheme["body"] element itself.}

@defmethod[(elements-with-tag [tag string?])
           (listof (is-a?/c mx-element%))]{

  Returns a list of elements with the HTML tag given by @scheme[tag].
  The requested elements must be within the document's @scheme["body"]
  tags or the @scheme["body"] element itself.}

}

@;{

  HTML events
  -----------

  MysterX HTML events are generated by mouse and
  keyboard interaction with HTML elements in a
  document.  HTML events are instances of the
  class mx-event%.  The action that generated an
  event can be determined by the following
  predicates, only one of which may hold:

> keypress? :: (send an-mx-event keypress?)
> keydown? :: (send an-mx-event event-keydown?) 
> keyup? :: (send an-mx-event keyup?)
> mousedown? :: (send an-mx-event mousedown?)
> mousemove? :: (send an-mx-event mousemove?)
> mouseover? :: (send an-mx-event mouseover?)
> mouseout? :: (send an-mx-event mouseout?)
> mouseup? :: (send an-mx-event mouseup?)
> click? :: (send an-mx-event click?)
> dblclick? :: (send an-mx-event dblclick?)
> error? :: (send an-mx-event error?)
  
  Events have attributes that give detail about
  the event.

> alt-key :: (send an-mx-event alt-key)

  Returns #t if the Alt key was pressed when the 
  event was generated, #f otherwise.

> ctrl-key :: (send an-mx-event ctrl-key)

  Returns #t if the Ctrl key was pressed when the 
  event was generated, #f otherwise. 

> from-tag :: (send an-mx-event from-tag)

  Returns a string indicating the tag of the HTML
  element where the mouse is being moved from.
  Return value is valid only for events for which
  `mouseover?' or `mouseout?' hold.

> from-id :: (send an-mx-event from-id)

  Returns a string indicating the identifier of
  the HTML element where the mouse is being moved
  from.  Return value is valid only for events for
  which `mouseover?' or `mouseout?' hold.

> id :: (send an-mx-event id)

  Returns a string indicating the identifier of
  the HTML element where the event occurred.

> keycode :: (send an-mx-event keycode)

  Returns a number indicating the keycode for the
  key that generated the event.  Return value is 
  valid only for events for which `keypress?',
  `keydown', or `keyup?' hold.

> shift-key :: (send an-mx-event shift-key)

  Returns #t if the Shift key was pressed when the 
  event was generated, #f otherwise.

> tag :: (send an-mx-event tag)

  Returns a string indicating the HTML tag of the
  element where the event occurred.

> to-tag :: (send an-mx-event to-tag)

  Returns a string indicating the tag of the
  target HTML element where the mouse is being
  moved to.  Return value is valid only for events
  for which `mouseover?' or `mouseout?' hold.

> to-id :: (send an-mx-event to-id)

  Returns a string indicating the identifier of
  the target HTML element where the mouse is being
  moved from.  Return value is valid only for
  events for which `mouseover?' or `mouseout?' hold.

> x :: (send an-mx-event x)

  Returns an integer indicating the x-coordinate 
  within the document where the event occurred.

> y :: (send an-mx-event y)

  Returns an integer indicating the y-coordinate 
  within the document where the event occurred.

  COM types
  ---------

> (com-object-type a-com-object)

  Returns a <com-type> value for `a-com-object', a <com-object>.

> (com-is-a? a-com-object a-com-type)

  Return #t if `a-com-object', a <com-object>, is of the
  type `a-com-type', a <com-type>.

  COM methods and properties
  --------------------------

  MysterX allows scripting of most COM components
  from Scheme.  A COM component can be scripted in
  MysterX if 1) it supports OLE Automation, that
  is, the IDispatch interface, and 2) publishes
  type information using the ITypeInfo interface.

> (com-all-coclasses)

  Returns a list of strings for all COM classes
  registered on a system.

> (com-all-controls)

  Returns a list of strings for all COM classes
  registered on a system that have the "Control"
  subkey.

> (cocreate-instance-from-coclass a-coclass [where])
> (cci/coclass a-coclass [where])

  `a-coclass` is the name of a COM class.  Returns
  an instance of the class, of type <com-object>.
  Useful for COM classes without a visual
  representation, or when a visual representation
  is not needed.

  The optional argument `where' indicates a for
  running the instance, and may be 'local (the
  default), 'remote, or a string indicating a
  machine name.  See "Running remote COM servers",
  below, for more information.

> (cocreate-instance-from-progid a-progid [where])
> (cci/progid a-progid [where])

  `a-progid` is the ProgID for a COM class.
  Returns an instance of the class, of type
  <com-object>.  Useful for COM classes without a
  visual representation, or when a visual
  representation is not needed.

  The optional argument `where' indicates a for
  running the instance, and may be 'local (the
  default), 'remote, or a string indicating a
  machine name.  See "Running remote COM servers",
  below, for more information.

> (coclass a-com-object)

  Given a <com-object>, returns a string that is
  the name of the COM class instantiated by the
  object.  Raises an error if the COM class is not
  known.

> (progid a-com-object)

  Given a <com-object>, returns a string that is
  the ProgID for the COM class instantiated by the
  object.  Raises an error if the COM class is not
  known.

> (set-coclass! a-com-object coclass)

  Sets the COM class for a <com-object> to be
  `coclass', a string.  This is useful when
  MysterX COM event-handling procedures can only
  obtain ambiguous information about the object's
  COM class.

> (set-coclass-from-progid! a-com-object progid)

  Sets the COM class for a <com-object> to be the
  class denoted by `progid', a string.  This is
  useful when MysterX COM event-handling
  procedures can only obtain ambiguous information
  about the object's COM class.

> (com-methods a-com-object)
> (com-methods a-com-type)

  `a-com-object' is a <com-object>.  `a-com-type'
   is a <com-type>.  Returns a list of strings
   indicating the names of methods.

> (com-method-type a-com-object method-name)
> (com-method-type a-com-type method-name)

  `a-com-object' is a <com-object>.  `a-com-type'
  is a <com-type>.  `method-name' is a string.
  Returns a list of symbols indicating the type of
  the specified method.

> (com-invoke a-com-object method-name ...)

  `a-com-object' is a <com-object>, and
  `method-name' is a string.  Invokes the
  requested method using the arguments given.  The
  special value `com-omit' may be used for
  optional arguments, useful when values are
  supplied for arguments to the right of the
  omitted argument(s).

> (com-get-properties a-com-object)
> (com-get-properties a-com-type)

  `a-com-object' has type <com-object>.
  `a-com-type' is a <com-type>.  Returns a list of
  strings indicating the names of readable
  properties in the object.

> (com-get-property-type a-com-object property-name)
> (com-get-property-type a-com-type property-name)

  `a-com-object' is a <com-object>, `a-com-type'
  is a <com-type>, and `property-name' is a
  string.  Returns a list of symbols indicating
  the type of the specified property.

> (com-get-property a-com-object property property ...)

  `a-com-object' is a <com-object>, and each 
  `property' is a string, or a list whose first element is a string,
  and whose remaining elements are Scheme values (an "indexed" 
  property).  Returns the value of the final property 
  by following the indicated path of properties, where each 
  intermediate property is a <com-object>. 

> (com-set-properties a-com-object)
> (com-set-properties a-com-type)

  `a-com-object' has type <com-object>.
  `a-com-type' is a <com-type>.  Returns a list of
  strings indicating the names of writable
  properties in the object.

> (com-set-property-type a-com-object pptyroperty-name)
> (com-set-property-type a-com-type property-name)

  `a-com-object' is a <com-object>, `a-com-type'
  is a <com-type>, and `property-name' is a
  string.  Returns a list of symbols indicating
  the type of the specified property.

> (com-set-property! a-com-object property property ... val)

  `a-com-object' is a <com-object>, each `property' is a 
   string, or a list whose first element is a string and whose
   remaining elements are Scheme values (indicating an "indexed"
   property); `val' is a Scheme value.  Sets the value of the final 
   property by following the indicated path of properties, where 
   each intermediate property is a <com-object>. 

> (com-help a-com-object [topic])
> (com-help a-com-type [topic])

  Starts the Window Help system with help about
  the COM object or COM type.  `a-com-object' is a
  <com-object>, `a-com-type' is a <com-type>, and
  `topic' is an optional string, typically a
  method or property name.

> (com-object-eq? a-com-object a-com-object)

  Returns #t if the two COM objects are the same,
  #f otherwise.

> (com-object? a-com-object)

  Returns #t if the argument is a COM object, #f
  otherwise.

> (com-add-ref a-com-object)
 
  Increments the reference count for a COM object.
  This procedure should only be called when system-level 
  errors occur due to a mismanaged COM object.  Ordinarily,
  MysterX handles all COM reference-counting automatically.
  See also `com-ref-count'.

> (com-ref-count a-com-object)

  Returns a number indicating the current reference count
  for a COM object.  See also `com-add-ref'.

_DCOM_
======
_Remote COM servers_
====================

  Running remote COM servers
  --------------------------

  For the MysterX procedures
  cocreate-instance-from-coclass and
  cocreate-instance-from-progid, the optional
  `where' argument may be 'remote.  In that case,
  the server instance is run at the location given
  by the Registry key

    HKEY_CLASSES_ROOT\AppID\<CLSID>\RemoteServerName

  where <CLSID> is the CLSID of the application.
  This key may be set using the DCOMCNFG utility.
  From DCOMCNFG, pick the application to be run on
  the Applications tab, then click on the
  Properties button.  On the Location tab, choose
  "Run application on the following computer", and
  enter the machine name.

  In order to run a COM remote server, the Registry on the 
  client machine must contain an entry at 

    HKEY_CLASSES_ROOT\CLSID\<CLSID>

  where <CLSID> is the CLSID for the server.  The server 
  application itself need not be installed on the client
  machine.

  There are a number of configuration issues relating to 
  DCOM, which MysterX uses to invoke remote COM servers.
  The Web page 

    http://www.distribucon.com/dcom95.html 

  discusses how to setup client and server machines for 
  DCOM.

  COM events
  -----------

  COM events are generated by COM objects.  Unlike
  HTML events, there is no fixed set of COM
  events, though there are "stock" events that
  many COM objects support.  MysterX allows the
  programmer to write handlers for both stock and
  custom events.

> (com-events a-com-object)
> (com-events a-com-type)

  Returns a list of strings naming the events
  supported by the COM object `a-com-object' or
  the COM type `a-com-type'.  If calling this
  procedure results in an error indicating that
  the COM object's coclass is ambiguous, try using
  either `set-coclass!' or
  `set-coclass-from-progid!', then retry the
  procedure.

> (com-event-type a-com-object ev)
> (com-event-type a-com-type ev)

  Returns the type of an event handler for the
  event ev, a string, generated by the particular
  COM object `a-com-object', or by COM objects
  that have type `a-com-type'.  The return type of
  all COM event handlers is void.  If calling this
  procedure results in an error indicating that
  the COM object's coclass is ambiguous, try using
  either `set-coclass!' or
  `set-coclass-from-progid!', then retry the
  procedure.

> (com-register-event-handler a-com-object ev fn)

  Registers the event handler fn, a procedure, to
  handle the event ev, a string, when generated by
  the COM object `a-com-object'.  The input type
  of `fn' depends on the event.  `fn' may have any
  output type, though its output is always
  discarded.  If calling this procedure results in
  an error indicating that the COM object's
  coclass is ambiguous, try using either
  `set-coclass!' or `set-coclass-from-progid',
  then retry the procedure.

> (com-unregister-event-handler a-com-object ev)

  Unregisters any event handler for the event ev,
  a string, that is generated by the COM object
  `a-com-object'.

  MysterX types
  -------------

  There are a few special types that appear in the
  types of COM component methods and properties.
  We describe those types and, in some cases, operations
  involving those types.

> boolean

  A boolean.  When a boolean is required in
  argument position, MysterX will interpret #f as
  false, and any other value as true.

> short-int

  A 16-bit integer, with value -32768 <= n <= 32767.

> com-currency

  A currency value.

> (com-currency? a-val)

  Returns #t if `a-val' has type com-currency, #f otherwise.

> (com-currency->number a-com-curr)

  Returns a number given `a-com-curr', a value of type
  com-currency.

> (number->com-currency a-num)

  Returns a value of type com-currency given `a-num', a number.
  The com-currency value stores a 64-bit two's-complement integer,
  though `a-num' may contain decimal digits.  If the number is
  too large, an error occurs.
  
> com-date

  A date value.
  
> (com-date? a-val)

  Returns #t if `a-val' has type com-date, #f otherwise.

> (com-date->date a-com-date)

  Returns an instance of the date structure, given `a-com-date',
  a com-date.  In the returned structure, the dst? field is
  always #f and the time-zone-offset field is 0.

> (date->com-date a-date)

  Returns a com-date value, given `a-date', an instance of the date
  structure.

> com-scode

  An error value.
  
> (com-scode? a-val)

  Returns #t if `a-val' has type com-scode, #f otherwise.
  
> (com-scode->number an-scode)

  Returns a number, given `an-scode', a value of type com-scode.

> (number->com-scode a-num)

  Returns a com-scode value, given the number `a-num'.
  The number must be representable as a 32-bit two's-complement
  number, else an error occurs.

> com-iunknown

  An interface pointer for a COM object.
  Currently, there is no way to construct such
  values, though they may be returned by COM
  objects.
  
> (com-iunknown? a-val)

  Returns #t if `a-val' has type com-iunknown, #f otherwise.
  
> mx-any

  A method or property requiring a value of mx-any
  will accept values of any of these Scheme types:
  char, int, float, real, and string, as well as
  the values #t and #f, as well as values of the
  COM-specific types <mx-currency>, <mx-date>,
  <mx-scode>, and <mx-unknown-com-object>.

  HTML and Dynamic HTML support
  -----------------------------

> (coclass->html name width height [size])

  Returns a string containing HTML which when
  inserted into a document loads the COM object
  with the COM class given by `name'.  This
  procedure is suitable for placing ActiveX
  controls within complex HTML.  `width' and
  `height' are exact integers.  The optional
  `size' argument gives an interpretation for the
  width and height.  By default, size is 'pixels,
  but may also be 'percent, indicating that the
  width and height are a fixed percentage of the
  document window size.  See also `progid->html'.

> (progid->html name width height [size])

  Returns a string containing HTML which when
  inserted into a document loads the COM object
  with the ProgID given by `name'.  This procedure
  is suitable for placing ActiveX controls within
  complex HTML.  `width' and `height' are exact
  integers.  The optional `size' argument gives an
  interpretation for the width and height.  By
  default, size is 'pixels, but may also be
  'percent, indicating that the width and height
  are a fixed percentage of the document window
  size.  See also `coclass->html'.

  HTML elements 
  -------------

  The mx-element% class encapsulates HTML
  elements.  By calling the methods of this class,
  you can change the appearance of elements, and
  place new HTML before or after the element.
  While the methods are described here, a good
  DHTML reference, such as D. Goodman, "Dynamic
  HTML" will have more complete information.

  Many of the mx-element% methods have two
  variants, a version that takes or returns Scheme
  data, and another "native" version that takes or
  returns a string.  In versions of MysterX up to
  v101, only the native methods were available,
  though they were not labeled as such.  Older
  MysterX code can usually be ported by simply
  adding "-native" to the names of those
  mx-element% methods that have a native version.
  For methods that return values of element
  properties, we assume two characteristics, which
  we do not mention in the methods' documentation:
  1) Native methods return the empty string for
  properties that have not been set, and 2)
  non-native methods raise an error for properties
  that have not been set.

  In the following method descriptions, "CSS"
  refers to the Cascading Style Sheets
  specification.  A CSS length is string
  consisting of a decimal integer number followed
  by one of the units "px" (pixels), "em" (font
  height), "ex" (height of an "x"), "in" (inches),
  "cm" (centimeters), "mm" (millimeters), "pc"
  (picas), or "pt" (points).  A CSS percentage is
  a string consisting of a decimal real number
  followed by "%".  When using "-native" methods,
  CSS lengths and percentages are given as
  strings.  For use by non-native methods, the
  following structures have been defined:

    (define-struct css-percentage (num))
    (define-struct css-length (num units))

 where num is a number, and units is a symbol.
 The valid symbols for the units field are those
 in '(em ex cm mm in pt pc px).
 
 Colors
 ------

 Many element properties represent colors.  In
 HTML, colors may be represented by an RGB string,
 which contains 7 characters.  The first character
 is #, the rest are hexadecimal digits (0-9 and
 A-F).  The first two digits are for the red
 component of the color, the middle two for the
 green component, and the last two for the blue
 component.  For example, "#FFFFFF" is white,
 "#000000" is black, and "#00FF00" is green.

 There are also predefined color names.  The
 "-native" methods use these names in strings,
 while their nonnative counterpart methods use the
 names as symbols.  The predefined color names are:

  aliceblue antiquewhite aqua aquamarine azure
  beige bisque black blanchedalmond blue
  blueviolet brown burlywood cadetblue chartreuse
  chocolate coral cornflower cornsilk crimson cyan
  darkblue darkcyan darkgoldenrod darkgray
  darkgreen darkkhaki darkmagenta darkolivegreen
  darkorange darkorchid darkred darksalmon
  darkseagreen darkslateblue darkslategray
  darkturquoise darkviolet deeppink deepskyblue
  dimgray dodgerblue firebrick floralwhite
  forestgreen fuchsia gainsboro ghostwhite gold
  goldenrod gray green greenyellow honeydew
  hotpink indianred indigo ivory khaki lavender
  lavenderblush lawngreen lemonchiffon lightblue
  lightcoral lightcyan lightgoldenrodyellow
  lightgreen lightgray lightpink lightsalmon
  lightseagreen lightskyblue lightslategray
  lightsteelblue lightyellow lime limegreen linen
  magenta maroon mediumaquamarine mediumblue
  mediumorchid mediumpurple mediumseagreen
  mediumslateblue mediumspringgreen
  mediumturquoise mediumvioletred midnightblue
  mintcream mistyrose moccasin navajowhite navy
  oldlace olive olivedrab orange orangered orchid
  palegoldenrod palegreen paleturquoise
  palevioletred papayawhip peachpuff peru pink
  plum powderblue purple red rosybrown royalblue
  saddlebrown salmon sandybrown seagreen seashell
  sienna silver skyblue slateblue slategray snow
  springgreen steelblue tan teal thistle tomato
  turquoise violet wheat white whitesmoke yellow
  yellowgreen

> get-html :: (send an-mx-element get-html)

  Returns a string containing all the HTML between the pair of 
  tags represented by the element.

> get-text :: (send an-mx-element get-text)

  Returns a string containing just the text between the pair of 
  tags represented by the element.  Any nested HTML tags 
  are not contained in the returned string.

> insert-html :: (send an-mx-element insert-html html)

  Places the HTML given by the string `html' before the element.

> append-html :: (send an-mx-element append-html html)

  Places the HTML given by the string `html' after the element.

> replace-html :: (send an-mx-element replace-html html)

  Replaces the HTML in the element with the string `html'.  You must 
  use the `find-element' or `find-element-by-id-or-name' methods of 
  mx-document% to retrieve the updated element.
  
> insert-text :: (send an-mx-element insert-text txt)

  Places the text given by the string `txt' before the HTML element.
  
> append-text :: (send an-mx-element append-text txt)

  Places the text given by the string `txt' after the HTML element.
  
> insert-object-from-coclass :: (send an-mx-element insert-object coclass width height [size])

  Inserts a COM object with class `coclass' before the HTML element.
  `width' and `height' are exact integers.  The optional `size' argument 
  gives an interpretation for the width and height.  By default, size 
  is 'pixels, but may also be 'percent, indicating that the width and 
  height are a fixed percentage of the document window size.
  An instance of <com-object> is returned.

> insert-object-from-progid :: (send an-mx-element insert-object progid width height [size])

  Inserts a COM object with ProgID `progid' before the HTML element.
  `width' and `height' are exact integers.  The optional `size' argument 
  gives an interpretation for the width and height.  By default, size 
  is 'pixels, but may also be 'percent, indicating that the width and 
  height are a fixed percentage of the document window size.
  An instance of <com-object> is returned.

> append-object-from-coclass :: (send an-mx-element append-object coclass width height [size])

  Places a COM object with class `coclass' after the HTML element.  
  `width' and `height' are exact integers.  The optional `size' argument 
  gives an interpretation for the width and height.  By default, size 
  is 'pixels, but may also be 'percent, indicating that the width and 
  height are a fixed percentage of the document window size.
  An instance of <com-object> is returned.

> append-object-from-progid :: (send an-mx-element append-object progid width height [size])

  Places a COM object with ProgID `progid' after the HTML element.  
  `width' and `height' are exact integers.  The optional `size' argument 
  gives an interpretation for the width and height.  By default, size 
  is 'pixels, but may also be 'percent, indicating that the width and 
  height are a fixed percentage of the document window size.
  An instance of <com-object> is returned.

> focus :: (send an-mx-element focus)

  Sets the focus to the element.  This method works only with
  Internet Explorer 5 and later.

> selection :: (send an-mx-element selection)

  If the element has the SELECT tag, returns a string indicating
  the value of the current selection.  Otherwise, an error
  occurs.  The value of the selection may be different from
  the string visible in the dropdown list.

> set-selection! :: (send an-mx-element set-selection! val)

  If the element has the SELECT tag, sets the selection to 
  the entry with the value `val', a string.  Otherwise, an error
  occurs.  The value of the selection may be different from
  the string visible in the dropdown list.

> attribute :: (send an-mx-element attribute attr)

  Retrieves the attribute named by the string `attr'.  The return
  value has a type that depends on the attribute.

> set-attribute! :: (send an-mx-element set-attribute! attr val)

  Sets the attribute named by the string `attr'.  The new
  value `val' has a type that depends on the attribute,
  but must be either a string, an integer, a float, #t, or #f.
  
> click :: (send an-mx-element click)

  Simulates a mouse click on the element. 

> tag :: (send an-mx-element tag)

  Retrieves the element's HTML tag, which is a string.

> font-family :: (send an-mx-element font-family)

  Returns a list of strings representing a priority list of 
  font families for the element.  

> font-family-native :: (send an-mx-element font-family-native)

  Returns a comma-delimited string representing a priority list of 
  font families for the element.

> set-font-family! :: (send an-mx-element set-font-family! ff)

  Sets the list of font families to the list of strings `ff',
  representing a priority list.  List items may include the generic 
  families "serif", "sans-serif", "cursive", "fantasy", and "monospace".

> set-font-family-native! :: (send an-mx-element set-font-family-native! ff)

  Sets the list of font families to the string `ff', which is 
  a comma-delimited priority list.  List items may include 
  the generic families `serif', `sans-serif', `cursive', 
  `fantasy', and `monospace'.

> font-style :: (send an-mx-element font-style)

  Retrieves a symbol indicating the element's font style,
  which is one of 'normal, 'italic, and 'oblique. 

> font-style-native :: (send an-mx-element font-style-native)

  Retrieves a string indicating the element's font style,
  which is one of "normal", "italic", and "oblique". 

> set-font-style! :: (send an-mx-element set-font-style! fs)

  Sets the element's font style to the string `fs', which 
  should be one of "normal", "italic", or "oblique". 

> font-variant :: (send an-mx-element font-variant)

  Retrieves a symbol indicating the element's font variation,
  which is either 'normal or 'small-caps.  

> font-variant-native :: (send an-mx-element font-variant-native)

  Retrieves a string indicating the element's font variation,
  which is either "normal", "small-caps", or the empty string.

> set-font-variant! :: (send an-mx-element set-font-variant! fv)

  Sets the element's font variation to the symbol `fv', which should 
  be either 'normal or 'small-caps.

> set-font-variant-native! :: (send an-mx-element set-font-variant-native! fv)

  Sets the element's font variation to the string `fv', which should 
  be either "normal", "small-caps", or the empty string.

> font-weight :: (send an-mx-element font-weight)

  Retrieves a value indicating the element's font weight,
  which is either one of the following symbols: 'normal, 'bold,  
  'bolder, 'lighter, or one of the numbers 100, 200, 300, 
  400, 500, 600, 700", 800, or 900.

> font-weight-native :: (send an-mx-element font-weight-native)

  Retrieves a string indicating the element's font weight,
  which is one of the following strings: "normal", "bold",  
  "bolder", "lighter", "100", "200", "300", "400", "500", 
  "600", "700", "800", or "900".

> set-font-weight! :: (send an-mx-element set-font-weight! fw)

  Sets the element's font weight using the value `fw', 
  which is either one of the symbols 'normal, 'bold,  
  'bolder, 'lighter, or one of the numbers 100, 200, 300, 400, 
  500, 600, 700, 800, or 900.

> set-font-weight-native! :: (send an-mx-element set-font-weight-native! fw)

  Sets the element's font weight using the string `fw', 
  which is one of the following strings: "normal", "bold",  
  "bolder", "lighter", "100", "200", "300", "400", "500", 
  "600", "700", "800", or "900".

> font-native :: (send an-mx-element font-native)

  Retrieves a string indicating the CSS font-style, font-variant, 
  font-weight, font-size, line-height, and font-family. 

> set-font-native! :: (send an-mx-element set-font-native! f)

  Sets the CSS font-style, font-variant, font-weight, font-size, 
  line-height, and font-family for the element from the string `f',
  which has the format:

  [font-style | font-variant | font-weight] font-size / 
  [line-height] font-family

  See the "-native" versions of the methods of these properties
  for syntax of the constituent substrings.

> font-size :: (send an-mx-element font-size)

  Retrieves the CSS font-size for the element.  See
  `set-font-size!' for possible values.  Raises an
  error if the property has not been set.

> font-size-native :: (send an-mx-element font-size-native)

  Retrieves the CSS font-size for the element.  See
  `set-font-size!' for possible values.  Raises an
  error if the property has not been set.

> set-font-size! :: (send an-mx-element set-font-size! fs)

  Sets the element's CSS font-size using `fs', which may 
  be any of 1) an symbol in the list '(xx-small x-small small medium 
  large x-large xx-large); 2) a symbol in the list '(larger smaller), 
  indicating a size relative to a parent element; 3) an instance
  of the css-length structure, or 4) an instance of the css-percentage
  structure, where the percentage is relative to the parent element's
  font-size.

> set-font-size-native! :: (send an-mx-element set-font-size-native! fs)

  Sets the element's CSS font-size using `fs', which may 
  be any of 1) a string indicating an absolute size, one
  of "xx-small", "x-small", "small", "medium", "large", "x-large", or 
  "xx-large"; 2) a string indicating a size relative to a parent
  element, either "larger" or "smaller"; 3) a CSS length; or
  4) a CSS percentage, considered relative to the parent element's
  font-size.

> background-native :: (send an-mx-element background-native)   

  Retrieves a string indicating the CSS background-color, 
  background-image, background-repeat, background-attachment, and 
  background-position for the element.

> set-background-native! :: (send an-mx-element set-background-native! b)   

  Sets the element's CSS background-color, background-image,
  background-repeat, background-attachment, and background-position
  using the string `b'.  See the "-native" versions of the methods
  for those properties for the syntax of the substrings.

> background-image :: (send an-mx-element background-image)   

  Retrieves a value indicating the CSS background-image for the
  element, either 'none or a string representing a URL.

> background-image-native :: (send an-mx-element background-image-native)   

  Retrieves a string indicating the CSS background-image for the
  element.  The string may be "none", the empty string, or 
  of the form "url(url-string)", indicating a URL.

> set-background-image! :: (send an-mx-element set-background-image! bi)   

  Sets the element's CSS background-image using the value `bi',
  which is either 'none, or of the form "url(url-string)".

> set-background-image-native! :: (send an-mx-element set-background-image-native! bi)   

  Sets the element's CSS background-image using the string `bi',
  which is either "none", or of the form "url(url-string)".

> background-repeat :: (send an-mx-element background-repeat)   

  Retrieves a symbol indicating the CSS background-repeat for the 
  element.  See set-background-repeat! for possible values of that
  symbol.

> background-repeat-native :: (send an-mx-element background-repeat-native)   

  Retrieves a string indicating the CSS background-repeat for the element.
  See set-background-repeat-native! for possible values of that
  string.

> set-background-repeat! :: (send an-mx-element set-background-repeat! br)

  Sets the element's CSS background-repeat using the symbol `br',
  which must be from the list '(no-repeat repeat repeat-x repeat-y).

> set-background-repeat-native! :: (send an-mx-element set-background-repeat-native! br)

  Sets the element's CSS background-repeat using the string `br',
  which must be from the list '("no-repeat" "repeat" "repeat-x" "repeat-y").

> background-position :: (send an-mx-element background-position)   

  Retrieves a string indicating the CSS background-position for the
  element.

> background-position-native :: (send an-mx-element background-position-native)   

  Retrieves a string indicating the CSS background-position for the
  element.

> set-background-position! :: (send an-mx-element set-background-position! bp)

  Sets the element's CSS background-position using `bp',
  which either 1) a single css-length, a css-percentage, or a 
  symbol from the list '(left center right), in which case the 
  property applies to the horizontal position; or 2) a two-element 
  list, where either element may be a css-length or css-percentage, 
  the first element may be a symbol from '(left center right), and 
  the second element may be from '(top center bottom), in which
  case the first element applies horizontally, and the second 
  vertically.

> set-background-position-native! :: (send an-mx-element set-background-position-native! bp)

  Sets the element's CSS background-position using the string `bp',
  which may be either 1) a CSS percentage, a CSS length
  or a string from '("left" "center" "right") or 2) two substrings
  separated by spaces, where either substring may be a CSS percentage
  or CSS length, the first substring may be one of 
  '("left" "center" "right"), and the second substring may be one of 
  '("top" "center" "bottom").  When one value is specified, it 
  applies to the horizontal position; when two are given, the
  first applies to the horizontal position, the second applies to 
  the vertical position.

> text-decoration :: (send an-mx-element text-decoration)

  Retrieves a list drawn from the symbols in 
  '(none underline overline line-through blink), indicating the 
  CSS text-decoration for the element.  

> text-decoration-native :: (send an-mx-element text-decoration-native)

  Retrieves a string indicating the CSS text-decoration for the
  element.  The string contains substrings drawn from 
  '("none" "underline" "overline" "line-through" "blink").

> set-text-decoration! :: (send an-mx-element set-text-decoration! td)

  Sets the element's CSS text-decoration using the list `td',
  which contains symbols drawn from 
  '(none underline overline line-through blink).

> set-text-decoration-native! :: (send an-mx-element set-text-decoration-native! td)

  Sets the element's CSS text-decoration using the string `td',
  which contains space-separated substrings drawn from 
  '("none" "underline" "overline" "line-through" "blink").

> text-transform :: (send an-mx-element text-transform)

  Retrieves a symbol indicating the CSS text-transform for the
  element, from the list '(none capitalize uppercase lowercase).

> text-transform-native :: (send an-mx-element text-transform-native)

  Retrieves a string indicating the CSS text-transform for the
  element, from the list '("none" "capitalize" "uppercase" "lowercase").

> set-text-transform! :: (send an-mx-element set-text-transform! tt)

  Sets the element's CSS text-transform using the symbol `tt',
  which is from the list '(none capitalize lowercase uppercase).

> set-text-transform-native! :: (send an-mx-element set-text-transform-native! tt)

  Sets the element's CSS text-transform using the string `tt',
  which is one of "none", "capitalize", "lowercase", or "uppercase".

> text-align :: (send an-mx-element text-align)

  Retrieves a symbol indicating the CSS text-align for the
  element, from the list '(left right center justify).

> text-align-native :: (send an-mx-element text-align-native)

  Retrieves a string indicating the CSS text-align for the
  element, from the list '("left" "right" "center" "justify").

> set-text-align! :: (send an-mx-element set-text-align! ta)

  Sets the element's CSS text-transform using the symbol `ta',
  which is from the list '(left right center justify).

> set-text-align-native! :: (send an-mx-element set-text-align-native! ta)

  Sets the element's CSS text-transform using the string `ta',
  which is from the list '("left" "right" "center" "justify").

> margin :: (send an-mx-element margin)

  Retrieves a list indicating the CSS margin for the
  element.  The list contains one to four elements,
  each either 'auto, a css-length, or a css-percentage.

> margin-native :: (send an-mx-element margin-native)

  Retrieves a string indicating the CSS margin for the
  element.  The string contains one to four substrings, each
  either "auto", a CSS length, or a CSS percentage.

> set-margin! :: (send an-mx-element set-margin! marg)

  Sets the element's CSS margin using the list `marg', which
  consists of a list of one to four entries, each of 
  which is a css-length, css-percentage, or 'auto.

> set-margin-native! :: (send an-mx-element set-margin-native! marg)

  Sets the element's CSS margin using the string `marg', which
  consists of one to four space-separated entries, each of 
  which is a CSS length, CSS percentage, or "auto".

> padding :: (send an-mx-element padding)

  Retrieves a list indicating the CSS padding for the
  element.  The list contains one to four elements, each
  a css-length or a css-percentage.

> padding-native :: (send an-mx-element padding-native)

  Retrieves a string indicating the CSS padding for the
  element.  The string consists of one to four CSS lengths
  or CSS percentages, separated by spaces.

> set-padding! :: (send an-mx-element set-padding! pad)

  Sets the element's CSS padding using the list `pad'.  Each
  element of the list is either a css-length or a css-percentage.

> set-padding-native! :: (send an-mx-element set-padding-native! pad)

  Sets the element's CSS padding using the string `pad', which
  consists of one to four space-separated entries, each of 
  which is a CSS length or percentage.

> border :: (send an-mx-element border)

  Retrieves a list describing the CSS border for the element.
  Each element of the list describes a width, style, or color.
  A width may be in '(medium thin thick) or a css-length.  
  A style is in '(none dotted dashed solid double groove 
  ridge inset outset).  A color is a symbol indicating a
  color or an RGB string.

> border-native :: (send an-mx-element border-native)

  Retrieves a string describing the CSS border for
  the element.  The string may contain
  space-separated values indicating a width,
  style, and color.  A width may be in '("medium"
  "thin" "thick") or a CSS length.  A style is in
  '("none" "dotted" "dashed" "solid" "double"
  "groove" "ridge" "inset" "outset").  A color is
  an HTML color or RGB string.

> set-border! :: (send an-mx-element set-border! border)

  Sets the element's CSS border using the list
  `border', which may contain a width, a style,
  and a color.  The width may be in '(medium thin
  thick) or a css-length.  The style is in '(none
  dotted dashed solid double groove ridge inset
  outset).  A color is a symbol indicating an HTML
  color or an RGB string.

> set-border-native! :: (send an-mx-element set-border-native! border)

  Sets the element's CSS border using the string
  `border', which may contain a width, style, and
  color.  The string may contain space-separated
  values indicating a width, style, and color.  A
  width may be in '("medium" "thin" "thick") or a
  CSS length.  A style is in '("none" "dotted"
  "dashed" "solid" "double" "groove" "ridge"
  "inset" "outset").  A color is an HTML color
  symbol or RGB string.

> border-top :: (send an-mx-element border-top)

  Retrieves a list describing the CSS border-top
  for the element.  See the method `border' for a
  description of the list.

> border-top-native :: (send an-mx-element border-top-native)

  Retrieves a string describing the CSS border-top
  for the element.  See the method `border-native'
  for a description of the string.

> set-border-top! :: (send an-mx-element set-border-top! bt)

  Sets the element's CSS border-top using the list
  `bt'.  See the method `set-border!' for a
  description of the list.

> set-border-top-native! :: (send an-mx-element set-border-top-native! bt)

  Sets the element's CSS border-top using the
  string `bt'.  See the method
  `set-border-native!' for a description of the
  list.

> border-bottom :: (send an-mx-element border-bottom)

  Retrieves a list describing the CSS
  border-bottom for the element.  See the method
  `border' for a description of the list.

> border-bottom-native :: (send an-mx-element border-bottom-native)

  Retrieves a string describing the CSS
  border-bottom for the element.  See the method
  `border-native' for a description of the string.

> set-border-bottom! :: (send an-mx-element set-border-bottom! bb)

  Sets the element's CSS border-bottom using the
  list `bb'.  See the method `set-border!' for a
  description of the list.

> set-border-bottom-native! :: (send an-mx-element set-border-bottom-native! bb)

  Sets the element's CSS border-bottom using the
  string `bb'.  See the method
  `set-border-native!' for a description of the
  list.

> border-left :: (send an-mx-element border-left)

  Retrieves a list describing the CSS border-left
  for the element.  See the method `border' for a
  description of the list.

> border-left-native :: (send an-mx-element border-left-native)

  Retrieves a string describing the CSS
  border-left for the element.  See the method
  `border-native' for a description of the string.

> set-border-left! :: (send an-mx-element set-border-left! bl)

  Sets the element's CSS border-left using the
  list `bl'.  See the method `set-border!' for a
  description of the list.

> set-border-left-native! :: (send an-mx-element set-border-left-native! bl)

  Sets the element's CSS border-left using the
  string `bl'.  See the method
  `set-border-native!' for a description of the
  list.

> border-right :: (send an-mx-element border-right)

  Retrieves a list describing the CSS border-right
  for the element.  See the method `border' for a
  description of the list.

> border-right-native :: (send an-mx-element border-right-native)

  Retrieves a string describing the CSS
  border-right for the element.  See the method
  `border-native' for a description of the string.

> set-border-right! :: (send an-mx-element set-border-right! br)

  Sets the element's CSS border-right using the
  list `br'.  See the method `set-border!' for a
  description of the list.

> set-border-right-native! :: (send an-mx-element set-border-right-native! br)

  Sets the element's CSS border-right using the
  string `br'.  See the method
  `set-border-native!' for a description of the
  list.

> border-color :: (send an-mx-element border-color)

  Retrieves a list of one to four elements in
  which each element is either a symbol indicating
  an HTML color or an RGB string.  If the list
  contains just one element, it applies to all
  four sides.  If there are two, the first is for
  top and bottom, the other for left and right.
  If there are three, they are for top, left and
  right, and bottom.

> border-color-native :: (send an-mx-element border-color-native)

  Retrieves a string in which describing the CSS
  border-color for the element.  The string
  contains one to four space-separated substrings
  indicating a color.  If there is just one color,
  it applies to all four sides.  If there are two,
  the first is for top and bottom, the other for
  left and right.  If there are three, they are
  for top, left and right, and bottom.

> set-border-color! :: (send an-mx-element set-border-color! bc)

  Sets the element's CSS border-color using the
  list `bc', which has one to four elements, each
  of which is either a symbol indicating an HTML
  color, or an RGB string.  If there is just one
  element, it applies to all four sides.  If there
  are two, the first is for top and bottom, the
  second for left and right.  If there are three,
  they are for top, left and right, and bottom.

> set-border-color-native! :: (send an-mx-element set-border-color-native! bc)

  Sets the element's CSS border-color using the
  string `bc', which consists of one to four HTML
  colors, separated by spaces.  If there is just
  one color, it applies to all four sides.  If
  there are two, the first is for top and bottom,
  the second for left and right.  If there are
  three, they are for top, left and right, and
  bottom.

> border-width :: (send an-mx-element border-width)

  Retrieves a list of one to four elements, each
  of which is either a css-length, or a symbol
  from '(medium thin thick).  If only one width is
  given, it applies to all four sides.  If two are
  given, the first is for top and bottom, the
  second for left and right.  If three are given,
  the first is for top, the second for left and
  right, the third for bottom.

> border-width-native :: (send an-mx-element border-width-native)

  Retrieves a string describing the CSS
  border-width for the element.  The string
  contains one to four substrings, each of which
  is either a CSS length, or a string from
  '("medium" "thin" "thick").  If only one width
  is given, it applies to all four sides.  If two
  are given, the first is for top and bottom, the
  second for left and right.  If three are given,
  the first is for top, the second for left and
  right, the third for bottom.

> set-border-width! :: (send an-mx-element set-border-width! bw)

  Sets the element's CSS border-color using the
  list `bw', which may contain one to four
  elements, each of which is either a css-length,
  or a symbol from '(thin medium thick).  If only
  one width is given, it applies to all four
  sides.  If two are given, the first is for top
  and bottom, the second for left and right.  If
  three are given, the first is for top, the
  second for left and right, the third for bottom.

> set-border-width-native! :: (send an-mx-element set-border-width-native! bw)

  Sets the element's CSS border-color using the
  string `bw', which consists of one to four
  space-separated substrings, each of which is
  either a CSS length or a string from '("thin"
  "medium" "thick").  If only one width is given,
  it applies to all four sides.  If two are given,
  the first is for top and bottom, the second for
  left and right.  If three are given, the first
  is for top, the second for left and right, the
  third for bottom.

> border-style :: (send an-mx-element border-style)

  Retrieves a symbol describing the CSS
  border-style for the element.  The symbol is
  from '(none dotted dashed solid double groove
  ridge inset outset).

> border-style-native :: (send an-mx-element border-style-native)

  Retrieves a string describing the CSS
  border-style for the element.  The string is
  from '("none" "dotted" "dashed" "solid" "double"
  "groove" "ridge" "inset" "outset").

> set-border-style! :: (send an-mx-element set-border-style! bs)

  Sets the element's CSS border-style using the
  symbol `bs', which is from '(none dotted dashed
  solid double groove ridge inset outset).

> set-border-style-native! :: (send an-mx-element set-border-style-native! bs)

  Sets the element's CSS border-style using the
  string `bs', which is from '("none" "dotted"
  "dashed" "solid" "double" "groove" "ridge"
  "inset" "outset").

> border-top-style :: (send an-mx-element border-top-style)

  Retrieves a symbol describing the CSS
  border-top-style for the element.  See
  border-style for possible values.

> border-top-style-native :: (send an-mx-element border-top-style-native)

  Retrieves a string describing the CSS
  border-top-style for the element.  See
  border-style-native for possible values.

> set-border-top-style! :: (send an-mx-element set-border-top-style! bts)

  Sets the element's CSS border-top-style using
  the symbol `bts'.  See set-border-style! for
  possible values.

> set-border-top-style-native! :: (send an-mx-element set-border-top-style-native! bts)

  Sets the element's CSS border-top-style using
  the string `bts'.  See set-border-style-native!
  for possible values.

> border-bottom-style :: (send an-mx-element border-bottom-style)

  Retrieves a symbol describing the CSS
  border-bottom-style for the element.  See
  border-style for possible values.

> border-bottom-style-native :: (send an-mx-element border-bottom-style-native)

  Retrieves a string describing the CSS
  border-bottom-style for the element.  See
  border-style-native for possible values.

> set-border-bottom-style! :: (send an-mx-element set-border-bottom-style! bbs)

  Sets the element's CSS border-bottom-style using
  the symbol `bbs'.  See set-border-style! for
  possible values.

> set-border-bottom-style-native! :: (send an-mx-element set-border-bottom-style-native! bbs)

  Sets the element's CSS border-bottom-style using
  the string `bbs'.  See set-border-style-native!
  for possible values.

> border-left-style :: (send an-mx-element border-left-style)

  Retrieves a symbol describing the CSS
  border-left-style for the element.  See
  border-style for possible values.

> border-left-style-native :: (send an-mx-element border-left-style-native)

  Retrieves a string describing the CSS
  border-left-style for the element.  See
  border-style-native for possible values.

> set-border-left-style! :: (send an-mx-element set-border-left-style! bls)

  Sets the element's CSS border-left-style using
  the string `bls'.  See set-border-style! for
  possible values.

> set-border-left-style-native! :: (send an-mx-element set-border-left-style-native! bls)

  Sets the element's CSS border-left-style using
  the string `bls'.  See set-border-style-native!
  for possible values.

> border-right-style :: (send an-mx-element border-right-style)

  Retrieves a symbol describing the CSS
  border-right-style for the element.  See
  border-style for possible values.

> border-right-style-native :: (send an-mx-element border-right-style-native)

  Retrieves a string describing the CSS
  border-right-style for the element.  See
  border-style-native for possible values.

> set-border-right-style! :: (send an-mx-element set-border-right-style! brs)

  Sets the element's CSS border-right-style using
  the symbol `brs'.  See set-border-style! for
  possible values.

> set-border-right-style! :: (send an-mx-element set-border-right-style! brs)

  Sets the element's CSS border-right-style using
  the string `brs'.  See set-border-style-native!
  for possible values.

> style-float :: (send an-mx-element style-float)

  Retrieves a symbol describing the CSS
  style-float for the element, one of '(none left
  right).

> style-float-native :: (send an-mx-element style-float-native)

  Retrieves a string describing the CSS
  style-float for the element, one of '("none"
  "left" "right").

> set-style-float! :: (send an-mx-element set-style-float! sf)

  Sets the element's CSS style-float using the
  symbol `sf', which is one of '(none left right).

> set-style-float-native! :: (send an-mx-element set-style-float-native! sf)

  Sets the element's CSS style-float using the
  string `sf', which is one of '("none" "left"
  "right").

> clear :: (send an-mx-element clear)

  Retrieves a symbol describing the CSS clear for
  the element, which is one of '(none left right
  both).

> clear-native :: (send an-mx-element clear-native)

  Retrieves a string describing the CSS clear for
  the element, which is one of '("none" "left"
  "right" "both").

> set-clear! :: (send an-mx-element set-clear! clr)

  Sets the element's CSS clear using the symbol
  `clr', which is one of '(none left right both).

> set-clear-native! :: (send an-mx-element set-clear-native! clr)

  Sets the element's CSS clear using the string
  `clr', which is one of '("none" "left" "right"
  "both").

> display :: (send an-mx-element display)

  Retrieves a symbol describing the CSS clear for
  the element, which is one of '(block none inline
  list-item table-header-group
  table-footer-group).

> display-native :: (send an-mx-element display-native)

  Retrieves a string describing the CSS clear for
  the element, which is one of '("block" "none"
  "inline" "list-item" "table-header-group"
  "table-footer-group").

> set-display! :: (send an-mx-element set-display! dpy)

  Sets the element's CSS display using the symbol
  `dpy', which is one of '(block none inline
  list-item table-header-group
  table-footer-group).

> set-display-native! :: (send an-mx-element set-display-native! dpy)

  Sets the element's CSS display using the string
  `dpy', which is one of '("block" "none" "inline"
  "list-item" "table-header-group"
  "table-footer-group").

> visibility :: (send an-mx-element visibility)

  Retrieves a symbol describing the CSS visibility
  for the element, which is one of '(inherit
  visible hidden).

> visibility-native :: (send an-mx-element visibility-native)

  Retrieves a string describing the CSS visibility
  for the element, which is one of '("inherit"
  "visible" "hidden").

> set-visibility! :: (send an-mx-element set-visibility! vis)

  Sets the element's CSS visibility using the
  symbol `vis', which is one of '(inherit visible
  hidden).

> set-visibility-native! :: (send an-mx-element set-visibility-native! vis)

  Sets the element's CSS visibility using the
  string `vis', which is one of '("inherit"
  "visible" "hidden").

> list-style-type :: (send an-mx-element list-style-type)

  Retrieves a symbol describing the CSS
  list-style-type for the element, one of '(disc
  circle square decimal lower-roman upper-roman
  lower-alpha upper-alpha none).

> list-style-type-native :: (send an-mx-element list-style-type-native)

  Retrieves a string describing the CSS
  list-style-type for the element, one of '("disc"
  "circle" "square" "decimal" "lower-roman"
  "upper-roman" "lower-alpha" "upper-alpha"
  "none").

> set-list-style-type! :: (send an-mx-element set-list-style-type! sty)

  Sets the element's CSS list-style-type using the
  symbol `sty', which is one of '(disc circle
  square decimal lower-roman upper-roman
  lower-alpha upper-alpha none).

> set-list-style-type-native! :: (send an-mx-element set-list-style-type-native! lst)

  Sets the element's CSS list-style-type using the
  string `sty', which is one of '("disc" "circle"
  "square" "decimal" "lower-roman" "upper-roman"
  "lower-alpha" "upper-alpha" "none").

> list-style-position :: (send an-mx-element list-style-position)

  Retrieves a symbol describing the CSS
  list-style-position for the element, one of
  '(outside inside).

> list-style-position-native :: (send an-mx-element list-style-position-native)

  Retrieves a string describing the CSS
  list-style-position for the element, one of
  '("outside" "inside").

> set-list-style-position! :: (send an-mx-element set-list-style-position! pos)

  Sets the element's CSS list-style-position using
  the symbol `pos', which is one of '(outside
  inside).

> set-list-style-position-native! :: (send an-mx-element set-list-style-position-native! pos)

  Sets the element's CSS list-style-position using
  the string `pos', which is one of '("outside"
  "inside").

> list-style-image :: (send an-mx-element list-style-image)

  Retrieves a string describing the CSS
  list-style-image for the element, which is
  either "none" or a URL.

> list-style-image-native :: (send an-mx-element list-style-image-native)

  Retrieves a string describing the CSS
  list-style-image for the element, which is
  either "none" or of the form "url(url-string)".

> set-list-style-image! :: (send an-mx-element set-list-style-image! image)

  Sets the element's CSS list-style-image using
  the string `image', which is either "none" or a
  URL.

> set-list-style-image-native! :: (send an-mx-element set-list-style-image-native! image)

  Sets the element's CSS list-style-image using
  the string `image', which is either "none" or of
  the form "url(url-string)".

> list-style :: (send an-mx-element list-style)

  Retrieves a list of one to three values
  describing the CSS list-style for the element.
  Each value in the list is as the values returned
  by the methods list-style-type,
  list-style-position, and list-style-image.  The
  values may appear in any order.

> list-style-native :: (send an-mx-element list-style-native)

  Retrieves a string with one to three substrings
  describing the CSS list-style for the element.
  Each substring is of the form of the strings
  returned by the methods list-style-type-native,
  list-style-position-native, and
  list-style-image-native.  The substrings may
  appear in any order.

> set-list-style! :: (send an-mx-element set-list-style! ls)

  Sets the element's CSS list-style using the list
  `ls', which contains one to three elements, each
  of which is of the form of the possible
  arguments to set-list-style-type!,
  set-list-style-image!, and
  set-list-style-position!.

> set-list-style-native! :: (send an-mx-element set-list-style-native! ls)

  Sets the element's CSS list-style using the
  string `ls', which contains one to three
  space-separated substrings.  Each substring is
  of the form of the possible arguments to
  set-list-style-type-native!,
  set-list-style-image-native!, and
  set-list-style-position-native!.

> position :: (send an-mx-element position)

  Retrieves a symbol describing the CSS position
  for the element, which is one of '(absolute
  relative static).

> position-native :: (send an-mx-element position-native)

  Retrieves a string describing the CSS position
  for the element, which is one of '("absolute"
  "relative" "static").

> overflow :: (send an-mx-element overflow)

  Retrieves a symbol describing the CSS overflow
  for the element, which is one of '(visible
  scroll hidden auto).

> overflow-native :: (send an-mx-element overflow-native)

  Retrieves a string describing the CSS overflow
  for the element, which is one of '("visible"
  "scroll" "hidden" "auto").

> set-overflow! :: (send an-mx-element set-overflow! of)

  Sets the element's CSS overflow using the symbol
  `of', which is one of '(visible scroll hidden
  auto).

> set-overflow-native! :: (send an-mx-element set-overflow-native! of)

  Sets the element's CSS overflow using the string
  `of', which is one of '("visible" "scroll"
  "hidden" "auto").

> pagebreak-before :: (send an-mx-element pagebreak-before)

  Retrieves a symbol describing the CSS
  pagebreak-before for the element, which is one
  of '(always auto none).

> pagebreak-before-native :: (send an-mx-element pagebreak-before-native)

  Retrieves a string describing the CSS
  pagebreak-before for the element, which is one
  of '("always" "auto" "").  Note that unlike most
  other "-native" properties, the empty string
  does not mean the property has not been set.

> set-pagebreak-before! :: (send an-mx-element set-pagebreak-before! pbb)

  Sets the element's CSS pagebreak-before using
  the symbol `pbb', which is one of '(always auto
  none).

> set-pagebreak-before-native! :: (send an-mx-element set-pagebreak-before-native! pbb)

  Sets the element's CSS pagebreak-before using
  the string `pbb', which is one of '("always"
  "auto" "").  Note that the empty string is a
  valid value.

> pagebreak-after :: (send an-mx-element pagebreak-after)

  Retrieves a symbol describing the CSS
  pagebreak-after for the element, which is one of
  '(always auto none).

> pagebreak-after-native :: (send an-mx-element pagebreak-after-native)

  Retrieves a string describing the CSS
  pagebreak-after for the element, which is one of
  '("always" "auto" "").  Note that unlike most
  other "-native" properties, the empty string
  does not mean the property has not been set.

> set-pagebreak-after! :: (send an-mx-element set-pagebreak-after! pba)

  Sets the element's CSS pagebreak-after using the
  symbol `pbb', which is one of '(always auto
  none).

> set-pagebreak-after-native! :: (send an-mx-element set-pagebreak-after-native! pba)

  Sets the element's CSS pagebreak-after using the
  string `pbb', which is one of '("always" "auto"
  "").  Note that the empty string is a valid
  value.

> css-text-native :: (send an-mx-element css-text-native)

  Retrieves a string describing the CSS text for
  the element.

> set-css-text-native! :: (send an-mx-element set-css-text-native! txt)

  Sets the element's CSS text using the string
  `txt'.

> cursor :: (send an-mx-element cursor)

  Retrieves a symbol describing the CSS cursor for
  the element, one of '(auto crosshair default
  hand move n-resize ne-resize nw-resize s-resize
  se-resize sw-resize e-resize w-resize text wait
  help).

> cursor-native :: (send an-mx-element cursor-native)

  Retrieves a string describing the CSS cursor for
  the element, one of '("auto" "crosshair"
  "default" "hand" "move" "n-resize" "ne-resize"
  "nw-resize" "s-resize" "se-resize" "sw-resize"
  "e-resize" "w-resize" "text" "wait" "help").

> set-cursor! :: (send an-mx-element set-cursor! csr)

  Sets the element's CSS cursor using the symbol
  `csr', which is one of '(auto crosshair default
  hand move n-resize ne-resize nw-resize s-resize
  se-resize sw-resize e-resize w-resize text wait
  help).

> set-cursor-native! :: (send an-mx-element set-cursor-native! csr)

  Sets the element's CSS cursor using the string
  `csr', which is one of '("auto" "crosshair"
  "default" "hand" "move" "n-resize" "ne-resize"
  "nw-resize" "s-resize" "se-resize" "sw-resize"
  "e-resize" "w-resize" "text" "wait" "help").

> clip :: (send an-mx-element clip)

  Retrieves a value describing the CSS clip for
  the element.  The value is either 'auto, or a
  list of four values for the top, right, bottom,
  and left edges.  In the list, each value is
  either 'auto or a css-length.

> clip-native :: (send an-mx-element clip-native)

  Retrieves a string describing the CSS clip for
  the element.  The string is either "auto" or of
  the form "rect(top right bottom left)", where
  the edges are either "auto" or CSS lengths.

> set-clip! :: (send an-mx-element set-clip! clp)

  Sets the element's CSS clip using the value
  `clp'.  The value is either 'auto, or a list of
  four values for the top, right, bottom, and left
  edges.  In the list, each value is either 'auto
  or a css-length.

> set-clip-native! :: (send an-mx-element set-clip-native! clp)

  Sets the element's CSS clip using the string
  `clp'.  The string is either "auto" or of the
  form "rect(top right bottom left)", where the
  edges are either "auto" or CSS lengths.

> filter :: (send an-mx-element filter)

  Retrieves a list describing the CSS filter for
  the element.  The list is of the form
  (filter-name (opt val) ...).  See the method
  set-filter! for the filters and their options.

> filter-native :: (send an-mx-element filter-native)

  Retrieves a string describing the CSS filter for
  the element.  The string is of the form
  "filter-name(opt=val,...)"  or simply
  "filter-name".  See the Internet Explorer SDK
  for more information about filters and their
  options.

> set-filter! :: (send an-mx-element set-filter! flt opt ...)

  Sets the element's CSS filter using the `flt', 
  which is a symbol.  Each option `opt' is a pair 
  consisting of a symbol and a value.  For all filters, 
  any of its options may be omitted.  The filters and 
  their options are

  filter           option              value      
  -----            ------              -----
  alpha            enabled             boolean
                   finish-opacity      n exact, 0 <= n <= 100
                   opacity             n exact, 0 <= n <= 100
                   start-x             exact number
                   start-y             exact number 
                   finish-x            exact number
                   finish-y            exact number
                   style               in '(uniform linear
                                            radial rectangular)
  blend-trans      enable              boolean
                   duration            number
                   status              in '(stopped applied playing)
  blur             add                 boolean
                   enabled             boolean
                   direction           in '(0 45 90 135 180 
                                            225 270 315)
                   strength            exact n, 1 <= n <= 100
  chroma           enabled             boolean
                   color               RGB string
  drop-shadow      enabled             boolean
                   off-x               exact number
                   off-y               exact number  
  flip-horizontal  enabled             boolean
  flip-vertical    enabled             boolean 
  glow             enabled             boolean
                   color               RGB string
                   strength            exact n, 1 <= n <= 100  
  gray             enabled             boolean
  invert           enabled             boolean 
  light            enabled             boolean 
  mask             enabled             boolean
                   color               RGB string
  redirect         enabled             boolean
  reveal-trans     enabled             boolean
                   duration            number
                   status              in '(stopped applied playing)
  shadow           enabled             boolean
                   color               RGB string
                   direction           in '(0 45 90 135 180 
                                            225 270 315)   
  wave             enabled             boolean
                   freq                number n, n >= 0
                   light-strength      exact n, 1 <= n <= 100
  x-ray            enabled             boolean

> set-filter-native! :: (send an-mx-element set-filter-native! flt)

  Sets the element's CSS filter using the string
  `flt'.  The string is of the form
  "filter-name(opt=val,...)"  or simply
  "filter-name".  See the Internet Explorer SDK
  for more information on filters and their
  options.

> style-string :: (send an-mx-element style-string)

  Retrieves a string describing the complete CSS
  description for the element.

> text-decoration-none :: (send an-mx-element text-decoration-none)

  Retrieves the CSS text-decoration-none, a
  boolean value, for the element.

> set-text-decoration-none! :: (send an-mx-element set-text-decoration-none! tdn)

  Sets the element's CSS text-decoration-none
  using `tdn'.  If `tdn' is #f, the value is
  false, otherwise it is considered true.

> text-decoration-underline :: (send an-mx-element text-decoration-underline)

  Retrieves the CSS text-decoration-underline, a
  boolean value, for the element.

> set-text-decoration-underline! :: (send an-mx-element set-text-decoration-underline! tdu)

  Sets the element's CSS text-decoration-underline
  using `tdu'.  If `tdn' is #f, the value is
  false, otherwise it is considered true.

> text-decoration-overline :: (send an-mx-element text-decoration-overline)

  Retrieves the CSS text-decoration-overline, a
  boolean value, for the element.

> set-text-decoration-overline! :: (send an-mx-element set-text-decoration-overline! tdo)

  Sets the element's CSS text-decoration-overline
  using `tdo'.  If `tdn' is #f, the value is
  false, otherwise it is considered true.

> text-decoration-linethrough :: (send an-mx-element text-decoration-linethrough)

  Retrieves the CSS text-decoration-linethrough, a
  boolean value, for the element.

> set-text-decoration-linethrough! :: (send an-mx-element set-text-decoration-linethrough! tdlt)

  Sets the element's CSS
  text-decoration-linethrough using `tdlt'.  If
  `tdn' is #f, the value is false, otherwise it is
  considered true.

> text-decoration-blink :: (send an-mx-element text-decoration-blink)

  Retrieves the CSS text-decoration-blink, a
  boolean value, for the element.

> set-text-decoration-blink! :: (send an-mx-element set-text-decoration-blink! bl)

  Sets the element's CSS text-decoration-blink
  using `bl'.  If `tdn' is #f, the value is false,
  otherwise it is considered true.

> pixel-top :: (send an-mx-element pixel-top)

  Retrieves the CSS pixel-top, an integer, for the
  element.

> set-pixel-top! :: (send an-mx-element set-pixel-top! pt)

  Sets the element's CSS pixel-top using `pt',
  which is an integer.

> pixel-left :: (send an-mx-element pixel-left)

  Retrieves the CSS pixel-left, an integer, for
  the element.

> set-pixel-left! :: (send an-mx-element set-pixel-left! pl)

  Sets the element's CSS pixel-left using `pl',
  which is an integer.

> pixel-width :: (send an-mx-element pixel-width)

  Retrieves the CSS pixel-width, an integer, for
  the element.

> set-pixel-width! :: (send an-mx-element set-pixel-width! pw)

  Sets the element's CSS pixel-width using `pw',
  which is an integer.

> pixel-height :: (send an-mx-element pixel-height)

  Retrieves the CSS pixel-height, an integer, for
  the element.

> set-pixel-height! :: (send an-mx-element set-pixel-height! ph)

  Sets the element's CSS pixel-height using `ph',
  which is an integer.

> pos-top :: (send an-mx-element pos-top)

  Retrieves the CSS pos-top, an inexact number,
  for the element.

> set-pos-top! :: (send an-mx-element set-pos-top! pt)

  Sets the element's CSS pos-top using `pt', which
  is an inexact number.

> pos-left :: (send an-mx-element pos-left)

  Retrieves the CSS pos-left, an inexact number,
  for the element.

> set-pos-left! :: (send an-mx-element set-pos-left! pl)

  Sets the element's CSS pos-left using `pl',
  which is an inexact number.

> pos-width :: (send an-mx-element pos-width)

  Retrieves the CSS pos-width, an inexact number,
  for the element.

> set-pos-width! :: (send an-mx-element set-pos-width! pw)

  Sets the element's CSS pos-width using `pw',
  which is an inexact number.

> pos-height :: (send an-mx-element pos-height)

  Retrieves the CSS pos-height, an inexact number,
  for the element.

> set-pos-height! :: (send an-mx-element set-pos-height! ph)

  Sets the element's CSS pos-height using `ph',
  which is an inexact number.

> color :: (send an-mx-element color)

  Retrieves the CSS color for the element, either
  a symbol indicating the name of a color, or an
  RGB string.

> color-native :: (send an-mx-element color-native)

  Retrieves a string indicating the CSS color for
  the element.  The string either names a color or
  is an RGB string.

> set-color! :: (send an-mx-element set-color! col)

  Sets the element's CSS color using `col', which
  may be a string indicating a color name, an
  integer describing an RGB value, or a string
  encoding an RGB value.

> set-color-native! :: (send an-mx-element set-color-native! col)

  Sets the element's CSS color using the string
  `col', which may be a color name or an RGB
  string.

> background-color :: (send an-mx-element background-color)

  Retrieves a string indicating the CSS
  background-color for the element.  The string
  either names a color or is an RGB string.

> background-color-native :: (send an-mx-element background-color-native)

  Retrieves a string indicating the CSS
  background-color for the element.  The string
  either names a color or is an RGB string.

> set-background-color! :: (send an-mx-element set-background-color! bc)

  Sets the element's CSS background-color using
  `bc', which may be a string indicating a color
  name, an integer describing an RGB value, or a
  string encoding an RGB value.

> set-background-color-native! :: (send an-mx-element set-background-color-native! bc)

  Sets the element's CSS background-color using
  the string `bc', which may be a color name or an
  RGB string.

> background-position-x :: (send an-mx-element background-position-x)

  Retrieves the CSS background-position-x for the
  element.  The value is either a css-length, a
  css-percentage, or a symbol in '(left center
  right).

> background-position-x-native :: (send an-mx-element background-position-x-native)

  Retrieves the CSS background-position-x for the
  element, a string which is either a CSS length,
  a CSS percentage, or in '("left" "center"
  "right").

> set-background-position-x! :: (send an-mx-element set-background-position-x! bpx)

  Sets the element's background-position-x using
  `bpx', which is either a css-length, a
  css-percentage, or a symbol in '(left center
  right).

> set-background-position-x-native! :: (send an-mx-element set-background-position-x-native! bpx)

  Sets the element's background-position-x using
  the string `bpx', which is either a CSS length,
  a CSS percentage, or one of '("left" "center"
  "right").

> background-position-y :: (send an-mx-element background-position-y)

  Retrieves the CSS background-position-y for the
  element.  The value is either a css-length, a
  css-percentage, or a symbol in '(top center
  bottom).

> background-position-y-native :: (send an-mx-element background-position-y-native)

  Retrieves the CSS background-position-y for the
  element, a string which is either a CSS length,
  a CSS percentage, or in '("top" "center"
  "bottom").

> set-background-position-y! :: (send an-mx-element set-background-position-y! bpy)

  Sets the element's background-position-y using
  `bpy', which is either a css-length, a
  css-percentage, or a symbol in '(top center
  bottom).

> set-background-position-y-native! :: (send an-mx-element set-background-position-y-native! bpy)

  Sets the element's background-position-y using
  the string `bpy', which is either a CSS length,
  a CSS percentage, or one of '("top" "center"
  "bottom").

> letter-spacing :: (send an-mx-element letter-spacing)

  Retrieves the CSS letter-spacing for the
  element, either a css-length or 'normal.

> letter-spacing-native :: (send an-mx-element letter-spacing-native)

  Retrieves the CSS letter-spacing for the
  element.  The value is a string that is either a
  CSS length or "normal".

> set-letter-spacing! :: (send an-mx-element set-letter-spacing! ls)

  Sets the element's letter-spacing using `ls',
  which is either a css-length or 'normal.

> set-letter-spacing-native! :: (send an-mx-element set-letter-spacing-native! ls)

  Sets the element's letter-spacing using the
  string `ls', which is either a CSS length or
  "normal".

> vertical-align :: (send an-mx-element vertical-align)

  Retrieves the CSS vertical-align for the
  element.  The value is a symbol in '(baseline
  sub super top middle bottom text-top text-bottom
  ).

> vertical-align-native :: (send an-mx-element vertical-align-native)

  Retrieves the CSS vertical-align for the
  element.  The value is a string in '("baseline"
  "sub" "super" "top" "middle" "bottom" "text-top"
  "text-bottom").

> set-vertical-align! :: (send an-mx-element set-vertical-align! va)

  Sets the element's CSS vertical-align using
  `va', which is a symbol in '(baseline sub super
  top middle bottom text-top text-bottom ).

> set-vertical-align-native! :: (send an-mx-element set-vertical-align-native! va)

  Sets the element's letter-spacing using `va',
  which is a string in '("baseline" "sub" "super"
  "top" "middle" "bottom" "text-top"
  "text-bottom").

> text-indent :: (send an-mx-element text-indent)

  Retrieves the CSS text-indent for the element.
  The value is either a css-length or a
  css-percentage.

> text-indent-native :: (send an-mx-element text-indent-native)

  Retrieves the CSS text-indent for the element.
  The value is a string that is either a CSS
  length or a CSS percentage.

> set-text-indent! :: (send an-mx-element set-text-indent! ti)

  Sets the element's text-indent using `ti', which
  is either a css-length a css-percentage.

> set-text-indent-native! :: (send an-mx-element set-text-indent-native! ti)

  Sets the element's text-indent using the string
  `ti', which is either a CSS length or a CSS
  percentage.

> line-height :: (send an-mx-element line-height)

  Retrieves the CSS line-height for the element.
  The value is either a css-length, a
  css-percentage, or 'normal.

> line-height-native :: (send an-mx-element line-height-native)

  Retrieves the CSS line-height for the element.
  The value is a string that is either a CSS
  length, a CSS percentage, or "normal".

> set-line-height! :: (send an-mx-element set-line-height! lh)

  Sets the element's line-height using `lh', which
  is either a css-length, a css-percentage, or
  'normal.

> set-line-height-native! :: (send an-mx-element set-line-height-native! lh)

  Sets the element's line-height using the string
  `lh', which is either a CSS length, a CSS
  percentage, or "normal".

> margin-top :: (send an-mx-element margin-top)

  Retrieves the CSS margin-top for the element.
  The value is a css-length, a css-percentage, or
  'auto.

> margin-top-native :: (send an-mx-element margin-top-native)

  Retrieves the CSS margin-top for the element.
  The value is a string that is a CSS length, a
  CSS percentage, or "auto".

> set-margin-top! :: (send an-mx-element set-margin-top! mt)

  Sets the element's margin-top using `mt', which
  is a css-length, a css-percentage, or 'auto.

> set-margin-top-native! :: (send an-mx-element set-margin-top-native! mt)

  Sets the element's margin-top using the string
  `mt', which is a CSS length, a CSS percentage,
  or "auto".

> margin-bottom :: (send an-mx-element margin-bottom)

  Retrieves the CSS margin-bottom for the element.
  The value is a css-length, a css-percentage, or
  'auto.

> margin-bottom-native :: (send an-mx-element margin-bottom-native)

  Retrieves the CSS margin-bottom for the element.
  The value is a string that is a CSS length, a
  CSS percentage, or "auto".

> set-margin-bottom! :: (send an-mx-element set-margin-bottom! mb)

  Sets the element's margin-bottom using `mb',
  which is a css-length, a css-percentage, or
  'auto.

> set-margin-bottom-native! :: (send an-mx-element set-margin-bottom-native! mb)

  Sets the element's margin-bottom using the
  string `mb', which is a CSS length, a CSS
  percentage, or "auto".

> margin-left :: (send an-mx-element margin-left)

  Retrieves the CSS margin-left for the element.
  The value is a css-length, a css-percentage, or
  'auto.

> margin-left-native :: (send an-mx-element margin-left-native)

  Retrieves the CSS margin-left for the element.
  The value is a string that is a CSS length, a
  CSS percentage, or "auto".

> set-margin-left! :: (send an-mx-element set-margin-left! ml)

  Sets the element's margin-left using `ml', which
  is a css-length, a css-percentage, or 'auto.

> set-margin-left-native! :: (send an-mx-element set-margin-left-native! ml)

  Sets the element's margin-left using the string
  `ml', which is a CSS length, a CSS percentage,
  or "auto".

> margin-right :: (send an-mx-element margin-right)

  Retrieves the CSS margin-right for the element.
  The value is a css-length, a css-percentage, or
  'auto.

> margin-right-native :: (send an-mx-element margin-right-native)

  Retrieves the CSS margin-right for the element.
  The value is a string that is a CSS length, a
  CSS percentage, or "auto".

> set-margin-right! :: (send an-mx-element set-margin-right! mr)

  Sets the element's margin-right using `mr',
  which is a css-length, a css-percentage, or
  'auto.

> set-margin-right-native! :: (send an-mx-element set-margin-right-native! mr)

  Sets the element's margin-right using the string
  `mr', which is a CSS length, a CSS percentage,
  or "auto".

> padding-top :: (send an-mx-element padding-top)

  Retrieves the CSS padding-top for the element.
  The value is a css-length or css-percentage.

> padding-top-native :: (send an-mx-element padding-top-native)

  Retrieves the CSS padding-top for the element.
  The value is a string that is a CSS length or
  CSS percentage.

> set-padding-top! :: (send an-mx-element set-padding-top! pt)

  Sets the element's padding-top using `pt', which
  is either a css-length or a css-percentage.

> set-padding-top-native! :: (send an-mx-element set-padding-top-native! pt)

  Sets the element's padding-top using `pt', a
  string that is either a CSS length or a CSS
  percentage.

> padding-bottom :: (send an-mx-element padding-bottom)

  Retrieves the CSS padding-bottom for the
  element.  The value is a css-length or a
  css-percentage.

> padding-bottom-native :: (send an-mx-element padding-bottom-native)

  Retrieves the CSS padding-bottom for the
  element.  The value is a string that is a CSS
  length or a CSS percentage.

> set-padding-bottom! :: (send an-mx-element set-padding-bottom! pb)

  Sets the element's padding-bottom using `pb',
  which is either a css-length or a
  css-percentage.

> set-padding-bottom-native! :: (send an-mx-element set-padding-bottom-native! pb)

  Sets the element's padding-bottom using `pb', a
  string that is either a CSS length or a CSS
  percentage.

> padding-left :: (send an-mx-element padding-left)

  Retrieves the CSS padding-left for the element.
  The value is a css-length or a css-percentage.

> padding-left-native :: (send an-mx-element padding-left-native)

  Retrieves the CSS padding-left for the element.
  The value is a string that is a CSS length or a
  CSS percentage.

> set-padding-left! :: (send an-mx-element set-padding-left! pl)

  Sets the element's padding-left using `pl',
  which is either a css-length or a
  css-percentage.

> set-padding-left-native! :: (send an-mx-element set-padding-left-native! pl)

  Sets the element's padding-left using `pl', a
  string that is either a CSS length or a CSS
  percentage.

> padding-right :: (send an-mx-element padding-right)

  Retrieves the CSS padding-right for the element.
  The value is a css-length or a css-percentage.

> padding-right-native :: (send an-mx-element padding-right-native)

  Retrieves the CSS padding-right for the element.
  The value is a string that is a CSS length or a
  CSS percentage.

> set-padding-right! :: (send an-mx-element set-padding-right! pr)

  Sets the element's padding-right using `pr',
  which is either a css-length or a
  css-percentage.

> set-padding-right-native! :: (send an-mx-element set-padding-right-native! pr)

  Sets the element's padding-right using `pr',
  which is either a CSS length or a CSS
  percentage.

> border-top-color :: (send an-mx-element border-top-color)

  Retrieves the CSS border-top-color for the
  element.  The value is either a symbol
  indicating a color name or an RGB string.

> border-top-color-native :: (send an-mx-element border-top-color-native)

  Retrieves a string indicating the CSS
  border-top-color for the element.  The string is
  either the name of a color or an RGB string.

> set-border-top-color! :: (send an-mx-element set-border-top-color! btc)

  Sets the element's border-top-color using the
  string `btc', which is either a symbol
  indicating the name of a color or an RGB string.

> set-border-top-color-native! :: (send an-mx-element set-border-top-color-native! btc)

  Sets the element's border-top-color using the
  string `btc', which is either the name of a
  color or an RGB string.

> border-bottom-color :: (send an-mx-element border-bottom-color)

  Retrieves the CSS border-bottom-color for the
  element.  The value is either a symbol
  indicating a color name or an RGB string.

> border-bottom-color-native :: (send an-mx-element border-bottom-color-native)

  Retrieves the CSS border-bottom-color for the
  element.  The value is a string that is either a
  color name or an RGB string.

> set-border-bottom-color! :: (send an-mx-element set-border-bottom-color! btc)

  Sets the element's border-bottom-color using
  `btc', which either a symbol indicating a color
  name, or an RGB string.

> set-border-bottom-color-native! :: (send an-mx-element set-border-bottom-color-native! btc)

  Sets the element's border-bottom-color using
  `btc', a string that is either a color name or
  an RGB string.

> border-left-color :: (send an-mx-element border-left-color)

  Retrieves the CSS border-left-color for the
  element.  The value is either a symbol
  indicating a color name or an RGB string.

> border-left-color-native :: (send an-mx-element border-left-color-native)

  Retrieves the CSS border-left-color for the
  element.  The value is a string that is either a
  color name or RGB string.

> set-border-left-color! :: (send an-mx-element set-border-left-color! btc)

  Sets the element's border-left-color using
  `btc', which is either a symbol indicating a
  color name or an RGB string.

> set-border-left-color-native! :: (send an-mx-element set-border-left-color-native! btc)

  Sets the element's border-left-color using the
  string `btc', which is either a color name or
  RGB string.

> border-right-color :: (send an-mx-element border-right-color)

  Retrieves the CSS border-right-color for the
  element.  The value is either a symbol
  indicating a color name or an RGB string.

> border-right-color-native :: (send an-mx-element border-right-color-native)

  Retrieves the CSS border-right-color for the
  element.  The value is a string indicating a
  color name or an RGB string.

> set-border-right-color! :: (send an-mx-element set-border-right-color! btc)

  Sets the element's border-right-color using
  `btc', which is either a symbol indicating a
  color name or an RGB string.

> set-border-right-color-native! :: (send an-mx-element set-border-right-color-native! btc)

  Sets the element's border-right-color using the
  string `btc', which is either a color name or
  RGB string.

> border-top-width :: (send an-mx-element border-top-width)

  Retrieves a value describing the CSS
  border-top-width for the element.  The value is
  either a css-length or a symbol, one of '(medium
  thin thick).

> border-top-width-native :: (send an-mx-element border-top-width-native)

  Retrieves a string describing the CSS
  border-top-width for the element.  The string is
  either a CSS length or one of '("medium" "thin"
  "thick").

> set-border-top-width! :: (send an-mx-element set-border-top-width! btw)

  Sets the element's CSS border-top-width using
  `btw', which is either a css-length, or a symbol
  in '(thin medium thick).

> set-border-top-width-native! :: (send an-mx-element set-border-top-width-native! btw)

  Sets the element's CSS border-top-width using
  the string `btw', which is either a CSS length,
  or in '("thin" "medium" "thick").

> border-bottom-width :: (send an-mx-element border-bottom-width)

  Retrieves a value describing the CSS
  border-bottom-width for the element.  The value
  is either a css-length or a symbol in '(medium
  thin thick).

> border-bottom-width-native :: (send an-mx-element border-bottom-width-native)

  Retrieves a string describing the CSS
  border-bottom-width for the element.  The string
  is either a CSS length or in '("thin" "medium"
  "thick").

> set-border-bottom-width! :: (send an-mx-element set-border-bottom-width! bbw)

  Sets the element's CSS border-bottom-width using
  the value `bbw', which is either a css-length,
  or a symbol in '(thin medium thick).

> set-border-bottom-width-native! :: (send an-mx-element set-border-bottom-width-native! bbw)

  Sets the element's CSS border-bottom-width using
  the string `bbw', which is either a CSS length
  or in '("thin" "medium" "thick").

> border-left-width :: (send an-mx-element border-left-width)

  Retrieves a value describing the CSS
  border-left-width for the element.  The value is
  either a css-length or a symbol in '(thin medium
  thick).

> border-left-width-native :: (send an-mx-element border-left-width-native)

  Retrieves a string describing the CSS
  border-left-width for the element.  The string
  is either a CSS length or in '("thin" "medium"
  "thick").

> set-border-left-width! :: (send an-mx-element set-border-left-width! blw)

  Sets the element's CSS border-left-width using
  `blw', which is either a css-length or a symbol
  in '(thin medium thick).

> set-border-left-width-native! :: (send an-mx-element set-border-left-width-native! blw)

  Sets the element's CSS border-left-width using
  the string `blw', which is either a CSS length
  or in '("thin" "medium" "thick").

> border-right-width :: (send an-mx-element border-right-width)

  Retrieves a value describing the CSS
  border-right-width for the element.  The value
  is either a css-length or a symbol in '(thin
  medium thick).

> border-right-width-native :: (send an-mx-element border-right-width-native)

  Retrieves a string describing the CSS
  border-right-width for the element.  The string
  is either a CSS length or in '("thin" "medium"
  "thick").

> set-border-right-width! :: (send an-mx-element set-border-right-width! brw)

  Sets the element's CSS border-right-width using
  `brw', which is either a css-length or a symbol
  in '(thin medium thick).

> set-border-right-width-native! :: (send an-mx-element set-border-right-width-native! brw)

  Sets the element's CSS border-right-width using
  the string `brw', which is either a CSS length
  or in '("thin" "medium" "thick").

> width :: (send an-mx-element width)

  Retrieves a value describing the CSS width for
  the element.  The value is a css-length, a
  css-percentage, or 'auto.

> width-native :: (send an-mx-element width-native)

  Retrieves a string describing the CSS width for
  the element.  The string is a CSS length, a CSS
  percentage, or "auto".

> set-width! :: (send an-mx-element set-width! w)

  Sets the element's CSS width using `w', which
  may be a css-length, a css-percentage, or 'auto.

> set-width-native! :: (send an-mx-element set-width-native! w)

  Sets the element's CSS width using `w', which
  may be a CSS length, a CSS percentage, or
  "auto".

> height :: (send an-mx-element height)

  Retrieves a value describing the CSS height for
  the element.  The value is a css-length, a
  css-percentage, or 'auto.

> height-native :: (send an-mx-element height-native)

  Retrieves a string describing the CSS height for
  the element.  The string is a CSS length, a CSS
  percentage, or "auto".

> set-height! :: (send an-mx-element set-height! h)

  Sets the element's CSS height using `h', which
  may be css-length, a css-percentage, or 'auto.

> set-height-native! :: (send an-mx-element set-height-native! h)

  Sets the element's CSS height using `h', which
  may be CSS length, a CSS percentage, or "auto".

> top :: (send an-mx-element top)

  Retrieves a value describing the CSS topt for
  the element.  The value is a css-length, a
  css-percentage, or 'auto.

> top-native :: (send an-mx-element top-native)

  Retrieves a string describing the CSS top for
  the element.  The string is a CSS length, a CSS
  percentage, or "auto".

> set-top! :: (send an-mx-element set-top! t)

  Sets the element's CSS top using `t', which may
  be css-length, a css-percentage, or 'auto.

> set-top-native! :: (send an-mx-element set-top-native! t)

  Sets the element's CSS top using `t', which may
  be CSS length, a CSS percentage, or "auto".

> left :: (send an-mx-element left)

  Retrieves a string describing the CSS left for
  the element.  The string is a CSS length, a CSS
  percentage, or "auto".

> left-native :: (send an-mx-element left-native)

  Retrieves a string describing the CSS left for
  the element.  The string is a CSS length, a CSS
  percentage, or "auto".

> set-left! :: (send an-mx-element set-left! l)

  Sets the element's CSS left using `l', which may
  be css-length, a css-percentage, or 'auto.

> set-left-native! :: (send an-mx-element set-left-native! l)

  Sets the element's CSS left using `l', which may
  be CSS length, a CSS percentage, or "auto".

> z-index :: (send an-mx-element z-index)

  Retrieves a value describing the CSS z-index for
  the element.  The value is either an integer or
  'auto.

> z-index-native :: (send an-mx-element z-index-native)

  Retrieves a value describing the CSS z-index for
  the element.  The value is either an integer or
  "auto".

> set-z-index! :: (send an-mx-element set-z-index! zi)

  Sets the element's CSS z-index using `zi', which
  may be either an integer or 'auto.

> set-z-index-native! :: (send an-mx-element set-z-index-native! zi)

  Sets the element's CSS z-index using `zi', which
  may be either an integer or "auto".

}