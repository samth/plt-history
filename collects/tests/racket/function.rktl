
(load-relative "loadtest.rktl")

(Section 'function)

(require scheme/function mzlib/etc)

;; stuff from scheme/base

(test 0 (compose add1 sub1) 0)
(test 2 (compose add1 (lambda () 1)))
(test 5 (compose (lambda (a b) a) (lambda (x) (values (add1 x) x))) 4)
(test -1 (compose (lambda (a b) (+ a b)) (lambda (x y) (values (- y) x))) 2 3)
(test 'hi (compose (case-lambda [(x) 'bye][(y z) 'hi]) (lambda () (values 1 2))))
(test 'ok (compose (lambda () 'ok) (lambda () (values))))
(test 'ok (compose (lambda () 'ok) (lambda (w) (values))) 5)
(test 0 (compose) 0)
(test-values '(1 2 3) (lambda () ((compose (lambda (x) (values x (add1 x) (+ x 2))) (lambda (y) y)) 1)))

(err/rt-test (compose 5))
(err/rt-test (compose add1 sub1 5))
(err/rt-test (compose add1 5 sub1))
(err/rt-test (compose 5 add1 sub1))
(err/rt-test ((compose add1 (lambda () (values 1 2)))) exn:application:arity?)
(err/rt-test ((compose add1 sub1)) exn:application:arity?)
(err/rt-test ((compose (lambda () 1) add1) 8) exn:application:arity?)

(arity-test compose 0 -1)

;; ---------- rec (from mzlib/etc) ----------
(let ()
  (test 3 (rec f (λ (x) 3)) 3)
  (test 3 (rec f (λ (x) x)) 3)
  (test 2 (rec f (λ (x) (if (= x 3) (f 2) x))) 3)
  (test 3 (rec (f x) 3) 3)
  (test 3 (rec (f x) x) 3)
  (test 2 (rec (f x) (if (= x 3) (f 2) x)) 3)
  (test 2 (rec (f x . y) (car y)) 1 2 3)
  (test 2 'no-duplications
        (let ([x 1]) (rec ignored (begin (set! x (+ x 1)) void)) x))
  (test 'f object-name (rec (f x) x))
  (test 'f object-name (rec (f x . y) x))
  (test 'f object-name (rec  f (lambda (x) x)))
  (test (list 2) (rec (f . x) (if (= (car x) 3) (f 2) x)) 3))

;; ---------- identity ----------
(let ()
  (test 'foo identity 'foo)
  (test 1 identity 1)
  (err/rt-test (identity 1 2))
  (err/rt-test (identity)))

;; ---------- const ----------
(let ()
  (test 'foo (const 'foo))
  (test 'foo (const 'foo) 1)
  (test 'foo (const 'foo) 1 2 3 4 5))

;; ---------- negate ----------
(let ()
  (define *not  (negate not))
  (define *void (negate void))
  (define *<    (negate <))
  (test #t *not #t)
  (test #f *not #f)
  (test #t *not 12)
  (test #f *void)
  (test #t *< 12 3)
  (test #t *< 12 12)
  (test #f *< 11 12)
  (test #t *< 14 13 12 11)
  (test #f *< 11 12 13 14))

;; ---------- curry/r ----------
(let ()
  (define foo0 (lambda () 0))
  (define foo1 (lambda (x) x))
  (define foo3 (lambda (x y z) (list x y z)))
  (define foo2< (lambda (x y . r) (list* x y r)))
  (define foo35 (case-lambda [(a b c) 3] [(a b c d e) 5]))
  (define foo:x (lambda (#:x [x 1] n . ns) (* x (apply + n ns))))
  (define *foo0  (curry foo0))
  (define *foo1  (curry foo1))
  (define *foo3  (curry foo3))
  (define *foo2< (curry foo2<))
  (define *foo35 (curry foo35))
  (define *foo:x2 (curry foo:x #:x 2))
  (define ++ (curry +))
  (define-syntax-rule ((f x ...) . => . e2) (test e2 f x ...))
  ;; see the docs for why these are expected
  (((curry foo0)) . => . 0)
  ((*foo0) . => . 0)
  ((curry foo1 123) . => . 123)
  ((*foo1 123) . => . 123)
  (((*foo1) 123) . => . 123)
  ((((*foo1)) 123) . => . 123)
  ((curry foo3 1 2 3) . => . '(1 2 3))
  ((*foo3 1 2 3) . => . '(1 2 3))
  (((*foo3 1 2) 3) . => . '(1 2 3))
  (((((((*foo3) 1)) 2)) 3) . => . '(1 2 3))
  (((curry foo2< 1 2)) . => . '(1 2))
  (((curry foo2< 1 2 3)) . => . '(1 2 3))
  (((curry foo2< 1 2) 3) . => . '(1 2 3))
  (((*foo2< 1 2)) . => . '(1 2))
  (((*foo2< 1 2 3)) . => . '(1 2 3))
  (((*foo2< 1 2) 3) . => . '(1 2 3))
  (((curry + 1 2) 3) . => . 6)
  (((++ 1 2) 3) . => . 6)
  (((++) 1 2) . => . 3)
  (((++)) . => . 0)
  (((curry foo35 1 2) 3) . => . 3)
  (((curry foo35 1 2 3)) . => . 3)
  (((*foo35 1 2) 3) . => . 3)
  (((*foo35 1 2 3)) . => . 3)
  (((((*foo35 1 2 3 4))) 5) . => . 5)
  (((((((((((*foo35)) 1)) 2)) 3 4))) 5) . => . 5)
  ((*foo:x2 1 2 3) . => . 12)
  ((map *foo:x2 '(1 2 3)) . => . '(2 4 6))
  ((((curryr foo3 1) 2) 3) . => . '(3 2 1))
  (((curryr list 1) 2 3) . => . '(2 3 1))
  )

(report-errs)
