(* $Id: bigint.ml,v 1.5 2014-11-11 15:06:24-08 - - $ *)
 (* Jim Brower         jrbrower@ucsc.edu *)
 (* Greer-Berezovsky   pbgreerb@ucsc.eu *)

 (* NAME *)
 (* ocamldc *)

 (* SYNOPSIS *)
 (*    ocamldc [file] *)

 (* DESCRIPTION *)
 (*    An Ocaml version of the bigint module.  *)
 (*    This module stores numbers and a tuple of  *)
 (*    a sign and an int list *)

open Printf

module Bigint = struct

  type relation = Greater | Equal | Less
  type sign     = Pos | Neg
  type bigint   = Bigint of sign * int list
  let  radix    = 10
  let  radixlen =  1

  let car       = List.hd
  let cdr       = List.tl
  let map       = List.map
  let reverse   = List.rev
  let length = List.length
  let strcat    = String.concat
  let strlen    = String.length
  let strsub    = String.sub
  let zero      = Bigint (Pos, [])

  (* trims leading zeroes for both user input and output *)
  let trim list =
    let rec trim' list' = match list' with
      | 0::cdr -> trim' cdr
      | trimmed -> reverse trimmed
    in trim' (reverse list)


  (* Compares the absolute values of the two numbers.
     Returns a relation of value1 to value2*)

  let cmp_numbers value1 value2 =
    let length1 = length value1 and length2 = length value2
    in if length1 > length2 then Greater
      else if length1 < length2 then Less
      else   
        (* assumes lists are the same length *)
        let rec cmp_digit list1 list2 =
          if list1 = [] then Equal
          else if car list1 > car list2 then Greater
          else if car list1 < car list2 then Less
          else cmp_digit (cdr list1) (cdr list2)
        in cmp_digit (reverse value1) (reverse value2)

  (* Takes in a string and returns a list of that strings 
     chars *)

  let charlist_of_string str = 
    let last = strlen str - 1
    in  let rec charlist pos result =
          if pos < 0
          then result
          else charlist (pos - 1) (str.[pos] :: result)
        in  charlist last []

  (* Takes in a string and returns a bigint of that string *)

  let bigint_of_string str =
    let len = strlen str
    in let to_intlist first =
         let substr = strsub str first (len - first) in
         let digit char = int_of_char char - int_of_char '0' in
         map digit (reverse (charlist_of_string substr))
       in  if   len = 0
         then zero
         else if   str.[0] = '_'
         then Bigint (Neg, (trim (to_intlist 1)))
         else Bigint (Pos, (trim (to_intlist 0)))

  (* Takes in a bigint and returns the string form of the
     number associated with it *)

  let string_of_bigint (Bigint (sign, value)) =
    match value with
    | []    -> "0"
    | value -> let reversed = reverse value
               in  strcat ""
               ((if sign = Pos then "" else "-") ::
                   (map string_of_int reversed))

  (* Takes in two int lists and returns the sum *)

  let rec add' list1 list2 carry = match (list1, list2, carry) with
    | list1, [], 0       -> list1
    | [], list2, 0       -> list2
    | list1, [], carry   -> add' list1 [carry] 0
    | [], list2, carry   -> add' [carry] list2 0
    | car1::cdr1, car2::cdr2, carry ->
       let sum = car1 + car2 + carry
       in  sum mod radix :: add' cdr1 cdr2 (sum / radix)

  (* Takes in two int lists and returns the difference *)

  let rec sub' list1 list2 carry = match (list1, list2, carry) with
    | list1, [], 0     -> list1
    | [], list2, 0     -> list2
    | list1, [], carry -> sub' list1 [carry] 0
    | [], list2, carry -> sub' [carry] list2 0
    | car1::cdr1, car2::cdr2, carry ->
       let difference = car1 - car2 - carry
       in if difference < 0
         then (difference + radix) :: sub' cdr1 cdr2 1
         else difference  :: sub' cdr1 cdr2 0

  (* Takes in two Bigints and returns the sum *)

  let add (Bigint (sign1, value1))  (Bigint (sign2, value2)) =
    let num_relation = cmp_numbers value1 value2
    in match (sign1, sign2) with
    | Pos, Neg -> if num_relation = Greater then 
        Bigint (Pos, (trim (sub' value1 value2 0)))
      else if num_relation = Less then 
        Bigint (Neg, (trim (sub' value2 value1 0)))
      else zero

    | Neg, Pos -> if num_relation = Greater then 
        Bigint (Neg, (trim (sub' value1 value2 0)))
      else if num_relation = Less then 
        Bigint (Pos, (trim (sub' value2 value1 0)))
      else zero

    | _, _     -> Bigint (sign1, (trim (add' value1 value2 0)))


  (* Takes in two Bigints and returns the difference *)

  let sub (Bigint (sign1, value1))  (Bigint (sign2, value2)) =
    let num_relation = cmp_numbers value1 value2
    in match (sign1, sign2) with
    | Pos, Pos -> if num_relation = Greater then 
        Bigint (Pos, (trim (sub' value1 value2 0)))
      else if num_relation = Less then 
        Bigint (Neg, (trim (sub' value2 value1 0)))
      else zero

    | Neg, Neg -> if num_relation = Greater then 
        Bigint (Neg, (trim (sub' value1 value2 0)))
      else if num_relation = Less then 
        Bigint (Pos, (trim (sub' value2 value1 0)))
      else zero

    | _, _     -> Bigint (sign1, (trim (add' value1 value2 0)))


  (* Takes in two int lists and returns the product *)

  let rec mul' list1 list2 shift =   
    let rec mul'' list1 digit carry = match (list1, digit, carry) with
      | _, 0, _  -> []
      | [], _, 0 -> []
      | [], _, carry -> [carry]
      | car::cdr, digit, carry -> 
         let prod = (car * digit) + carry
         in (prod mod radix) :: mul'' cdr digit (prod / radix)

    in let rec left_shift list places = 
         if places <= 0 then list
         else left_shift (0::list) (places - 1)

       in match list2 with
       | [] -> []
       | car::_ -> add' (left_shift (mul'' list1 car 0) shift)
          (mul' list1 (cdr list2) (shift + 1)) 0

  (* Takes in two Bigints and returns the product *)

  let mul (Bigint (sign1, value1))  (Bigint (sign2, value2)) =
    if value1 = [] || value2 = [] then zero
    else let final_sign = if sign1 = sign2 then Pos else Neg
         in (Bigint (final_sign, trim (mul' value1 value2 0))) 

  (* Takes in two int lists and returns a tuple that is the 
     quotient and remainder of the two int lists *)

  let rec div' numer deno quo = 
   if ((cmp_numbers (trim numer) (trim deno)) = Less)
     then (quo, numer)
     else (div' (sub' numer deno 0) deno (add' quo [] 1))

  (* Takes in two Bigints and returns the quotient *)

  let div (Bigint (sign1, value1)) (Bigint (sign2, value2)) =
    if (sign1 = sign2)
      then Bigint (Pos, trim (fst (div' value1 value2 [])))
      else Bigint (Neg, trim (fst (div' value1 value2 [])))

  (* Takes in two Bigints and returns the remainder *)

  let rem (Bigint (sign1, value1)) (Bigint (sign2, value2)) =
    Bigint (sign1, trim (snd (div' value1 value2 [])))

  (* Takes in a base int list and an exponent int list and
     returns the result *)

  let rec pow' base expp =
    if (expp = [1])
      then base
      else if ((snd (div' expp [2] [])) <> [])
             then (mul' base (pow' base (sub' expp [1] 0)) 0)
             else (mul' (pow' base (fst (div' expp [2] [])))
                        (pow' base (fst (div' expp [2] []))) 0)
  
  (* Takes in a base Bigint and an exponent Bigint and returns 
     the result *)

  let pow (Bigint (sign1, value1)) (Bigint (sign2, value2)) = 
                                match sign1, value1, sign2, value2 with
    | _, _, _, []    ->   (Bigint (Pos, [1]))
    | _, _, Neg, _   ->   
                (Bigint (Pos, (pow' (fst (div' [1] value1 [])) value2)))
    | Pos, _, _, _   ->   (Bigint (Pos, (pow' value1 value2)))
    | Neg, _, _, _   ->   (Bigint 
                            ((if ((snd (div' value2 [2] [])) <> [])
                                then Neg
                                else Pos),
                            (pow' value1 value2)))

  let getabs (Bigint (sign, value)) = value

end

