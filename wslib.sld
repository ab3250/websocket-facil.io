(define-library (wslib)
 (export 
  for-accumulator
  knuth-shuffle
  list-set
  padn
  func-list
  func-string
  string->list3 
  deck2string 
  deck2
  deck
  jsonify)
 (import
  (scheme base)
  (only (srfi 130) string-pad)
  (srfi 27)
  (schemepunk json))

(begin

  (define for-accumulator (lambda (start end acc-initial func )
    (let loop ((index start)
              (acc acc-initial))
        (if (> index end)
                        acc   
                        (loop (+ index 1) (func index end acc))))))

  (define (knuth-shuffle lst-org)  
    (let loop ((count (length lst-org)) (lst lst-org))      
      (if (zero? count)
        lst
    (let*   ((j (random-integer count))
      (new-count (- count 1))
            (tmp (list-ref lst new-count))
            (lst2 (list-set lst new-count (list-ref lst j)))
            (lst3 (list-set lst2 j tmp)))	         
            (loop new-count lst3)))))

  (define (list-set lst idx val)
    (if (null? lst)
      lst
      (cons
        (if (zero? idx)
          val
          (car lst))
        (list-set (cdr lst) (- idx 1) val))))

  (define func-list (lambda (index end acc)
    (cons (if (< (- index 1)(/ end 2)) "0" "1") acc)))

  (define func-string (lambda (index end acc)
    (if (< (- index 1)(/ end 2)) 
    (string-append "0" acc)
    (string-append "1" acc))))

  (define deck2string (for-accumulator 1 5000 "" func-string))

  (define (string->list3 str)
    (map (lambda(x)(string x)) (string->list deck2string)))

  (define padn (lambda (x y)
    (string-pad (number->string x) y #\0)))
  
  (define jsonify (lambda (type data)
    (string-append "{\"type\": \"" type "\",\"data\":" (json->string data) "}")))

  (define deck2 (for-accumulator 1 5000 '() func-list))

  (define deck    '("0000" "0001" "0002" "0003" "0004" "0005" "0006" "0007" "0008" "0009" "0010" "0011" "0012"
                    "0100" "0101" "0102" "0103" "0104" "0105" "0106" "0107" "0108" "0109" "0110" "0111" "0112"
                    "0200" "0201" "0202" "0203" "0204" "0205" "0206" "0207" "0208" "0209" "0210" "0211" "0212"
                    "0300" "0301" "0302" "0303" "0304" "0305" "0306" "0307" "0308" "0309" "0310" "0311" "0312"))
))