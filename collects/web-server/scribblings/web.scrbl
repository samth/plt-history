#lang scribble/doc
@(require "web-server.rkt")

@title[#:tag "web"]{Web Interaction}
@(require (for-label web-server/servlet/web
                     web-server/servlet/servlet-structs
                     web-server/http
                     racket/list
                     net/url))

@defmodule[web-server/servlet/web]{The
@racketmodname[web-server/servlet/web] library provides the primary
functions of interest for the servlet developer.

@defproc[(send/back [response can-be-response?])
         void?]{
 Sends @racket[response] to the client. No continuation is captured, so the servlet is done.
       
 Example:
 @racketblock[
  (send/back
   (response/xexpr
    `(html
      (body
       (h1 "The sum is: "
           ,(+ first-number
               second-number))))))
 ]
}

@defproc[(send/suspend [make-response (string? . -> . can-be-response?)])
         request?]{
 Captures the current continuation, stores it with @racket[exp] as the expiration
 handler, and binds it to a URL. @racket[make-response] is called with this URL and
 is expected to generate a @racket[can-be-response?], which is sent to the client. If the
 continuation URL is invoked, the captured continuation is invoked and the request is
 returned from this call to @racket[send/suspend].
 
 Example:
 @racketblock[
  (send/suspend
   (lambda (k-url)
     (response/xexpr
      `(html (head (title "Enter a number"))
             (body 
              (form ([action ,k-url])
                    "Enter a number: "
                    (input ([name "number"]))
                    (input ([type "submit"]))))))))
  ]
 
 When this form is submitted by the browser, the request will be sent to the URL generated by @racket[send/suspend].
 Thus, the request will be ``returned'' from @racket[send/suspend] to the continuation of this call.
}
                  
@defproc[(send/suspend/url [make-response (url? . -> . can-be-response?)])
         request?]{
 Like @racket[send/suspend] but with a URL struct.
}
                  
@defproc[(send/suspend/dispatch [make-response (((request? . -> . any) . -> . string?) . -> . can-be-response?)])
         any]{
 Calls @racket[make-response] with a function (@racket[embed/url]) that, when called with a procedure from
 @racket[request?] to @racket[any/c] will generate a URL, that when invoked will call
 the function with the @racket[request?] object and return the result to the caller of
 @racket[send/suspend/dispatch]. Therefore, if you pass @racket[embed/url] the identity function,
 @racket[send/suspend/dispatch] devolves into @racket[send/suspend]:
 
 @racketblock[
  (define (send/suspend response-generator)
    (send/suspend/dispatch
     (lambda (embed/url)
       (response-generator (embed/url (lambda (x) x))))))
  ]
 
 Use @racket[send/suspend/dispatch] when there are multiple `logical' continuations of a page.
 For example, we could either add to a number or subtract from it:
 @racketblock[
  (define (count-dot-com i)
    (count-dot-com 
     (send/suspend/dispatch
      (lambda (embed/url)
        (response/xexpr
         `(html 
           (head (title "Count!"))
           (body
            (h2 (a ([href
                     ,(embed/url
                       (lambda (req)
                         (sub1 i)))])
                   "-"))
            (h1 ,(number->string i))
            (h2 (a ([href
                     ,(embed/url
                       (lambda (req)
                         (add1 i)))])
                   "+")))))))))
  ]
 Notice that in this example the result of the handlers are returned to the continuation of @racket[send/suspend/dispatch].
 However, it is very common that the return value of @racket[send/suspend/dispatch] is irrelevant in
 your application and you may think of it as ``embedding'' value-less callbacks. Here is the same example in this style:
 @racketblock[
  (define (count-dot-com i)
    (send/suspend/dispatch
     (lambda (embed/url)
       (response/xexpr
        `(html 
          (head (title "Count!"))
          (body
           (h2 (a ([href
                    ,(embed/url
                      (lambda (req)
                        (count-dot-com (sub1 i))))])
                  "-"))
           (h1 ,(number->string i))
           (h2 (a ([href
                    ,(embed/url
                      (lambda (req)
                        (count-dot-com (add1 i))))])
                  "+"))))))))
  ]
}
             
@defproc[(send/suspend/url/dispatch [make-response (((request? . -> . any) . -> . url?) . -> . can-be-response?)])
         any]{
 Like @racket[send/suspend/dispatch], but with a URL struct.
}

@defproc[(send/forward [make-response (string? . -> . can-be-response?)])
         request?]{
 Calls @racket[clear-continuation-table!], then @racket[send/suspend].
       
 Use this if the user can logically go `forward' in your application, but cannot go backward.
}

@defproc[(send/finish [response can-be-response?])
         void?]{
 Calls @racket[clear-continuation-table!], then @racket[send/back].
       
 Use this if the user is truly `done' with your application. For example, it may be used to display the post-logout page:
 @racketblock[
  (send/finish
   (response/xexpr
    `(html (head (title "Logged out"))
           (body (p "Thank you for using the services "
                    "of the Add Two Numbers, Inc.")))))
  ]
}

@defproc[(redirect/get [#:headers hs (listof header?) empty])
         request?]{
 Calls @racket[send/suspend] with @racket[redirect-to], passing @racket[hs] as the headers.
       
 This implements the Post-Redirect-Get pattern. 
 Use this to prevent the @onscreen["Refresh"] button from duplicating effects, such as adding items to a database. 
}

@defproc[(redirect/get/forget [#:headers hs (listof header?) empty])
         request?]{
 Calls @racket[send/forward] with @racket[redirect-to], passing @racket[hs] as the headers.
}
                  
@defthing[current-servlet-continuation-expiration-handler 
          (parameter/c (or/c false/c
                             (request? . -> . can-be-response?)))]{
 Holds the expiration handler to be used when a continuation
 captured in this context is expired, then looked up.
 
 Example:
 @racketblock[
  (parameterize 
      ([current-servlet-continuation-expiration-handler
        (lambda (req)
          (response/xexpr
           `(html (head (title "Custom Expiration!")))))])
    (send/suspend
     ....))
  ]               
}

@defproc[(clear-continuation-table!)
         void?]{
 Calls the servlet's manager's @racket[clear-continuation-table!] function. Normally, this deletes all the previously
 captured continuations.
}
               
@defproc[(with-errors-to-browser [send/finish-or-back (can-be-response? . -> . request?)]
                                 [thunk (-> any)])
         any]{
 Calls @racket[thunk] with an exception handler that generates an HTML error page
 and calls @racket[send/finish-or-back].
 
 Example:
 @racketblock[
  (with-errors-to-browser
   send/back
   (lambda ()
     (/ 1 (get-number (request-number)))))
 ]
}

@defproc[(adjust-timeout! [t number?])
         void?]{
 Calls the servlet's manager's @racket[adjust-timeout!] function.
       
 @warning{This is deprecated and will be removed in a future release.}
}
                  
@defproc[(continuation-url? [u url?])
         (or/c false/c (list/c number? number? number?))]{
 Checks if @racket[u] is a URL that refers to a continuation, if so
 returns the instance id, continuation id, and nonce.
}

@defthing[servlet-prompt continuation-prompt-tag?]{
  The tag used for Web interaction continuation capture.}

}
