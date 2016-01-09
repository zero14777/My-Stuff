(* $Id: maindc.ml,v 1.4 2012-02-16 17:47:43-08 - - $ *)
 (* Jim Brower         jrbrower@ucsc.edu *)
 (* Greer-Berezovsky   pbgreerb@ucsc.eu *)

 (* NAME *)
 (* ocamldc *)

 (* SYNOPSIS *)
 (*    ocamldc [file] *)

 (* DESCRIPTION *)
 (*    An partial implementation of dc in ocaml.  *)
 (*    Reads in argv[1] if it exists  *)
 (*    then continues accepting input *)


include Scanner
include Bigint

open Bigint
open Printf
open Scanner

type stack_t = Bigint.bigint Stack.t
let push = Stack.push
let pop = Stack.pop

let ord thechar = int_of_char thechar
type binop_t = bigint -> bigint -> bigint

(* Most of these functions are just what Mackey gave us, 
most changes are in bigint.ml *)

let print_number number = printf "%s\n%!" (string_of_bigint number)

(* empty register access complaint *)
let print_regempty reg = printf "dc: register '%c' (0%d) is empty\n%!"
                          (char_of_int reg) reg

let print_stackempty () = eprintf "dc: stack empty\n%!"

(* added functionality using symtable. registers can pop a value off *)
(* the stack or put their value back onto the stack *)
let executereg (thestack: stack_t) symtable (oper: char) (reg: int) =
    try match oper with
      | 'l' -> (try 
                  push (Hashtbl.find symtable reg) thestack
                with Not_found -> print_regempty reg)
      | 's' -> Hashtbl.add symtable reg (pop thestack)
      | _   -> printf "0%o 0%o is unimplemented\n%!" (ord oper) reg
    with Stack.Empty -> print_stackempty()

let executebinop (thestack: stack_t) (oper: binop_t) dividing =
    try let right = pop thestack
        in  try let left = pop thestack
                (* checks for zero denom before dividing *)
                in if (((Bigint.getabs right) = []) && (dividing = 1))
                     then (printf "dc: divide by zero\n%!";
                           push left thestack;
                           push right thestack)
                     else push (oper left right) thestack
            with Stack.Empty -> (print_stackempty ();
                                 push right thestack)
    with Stack.Empty -> print_stackempty ()

let execute (thestack: stack_t) (oper: char) =
    try match oper with
        | '+'  -> executebinop thestack Bigint.add 0
        | '-'  -> executebinop thestack Bigint.sub 0
        | '*'  -> executebinop thestack Bigint.mul 0
        | '/'  -> executebinop thestack Bigint.div 1
        | '%'  -> executebinop thestack Bigint.rem 1
        | '^'  -> executebinop thestack Bigint.pow 0
        | 'c'  -> Stack.clear thestack
        | 'd'  -> push (Stack.top thestack) thestack
        | 'f'  -> Stack.iter print_number thestack
        | 'l'  -> failwith "operator l scanned with no register"
        | 'p'  -> print_number (Stack.top thestack)
        | 's'  -> failwith "operator s scanned with no register"
        | '\n' -> ()
        | ' '  -> ()
        | _    -> printf "dc: '%c' (0%o) unimplemented\n%!" 
                    oper (ord oper)
    with Stack.Empty -> print_stackempty()

(* added symtable for registers *)
let toploop (thestack: stack_t) symtable inputchannel =
    let scanbuf = Lexing.from_channel inputchannel in
    let rec toploop () = 
        try  let nexttoken = Scanner.scanner scanbuf
             in  (match nexttoken with
                 | Number number       -> push number thestack
                 | Regoper (oper, reg) -> executereg thestack 
                                            symtable oper reg
                 | Operator oper       -> execute thestack oper
                 );
             toploop ()
        with End_of_file -> ()
    in  toploop ()

(* added symtable for registers *)
let readfiles () =
    let thestack : bigint Stack.t = Stack.create ()
    in  let symtable = Hashtbl.create 128
        in((if Array.length Sys.argv > 1 
          then try  let thefile = open_in Sys.argv.(1)
                    in  toploop thestack symtable thefile
            with Sys_error message -> (
              printf "%s: %s\n%!" Sys.argv.(0) message;
              exit 1));
           toploop thestack symtable stdin)


(* added symtable for registers *)
let interact () =
    let thestack : bigint Stack.t = Stack.create ()
    in  let symtable = Hashtbl.create 128
        in toploop thestack symtable stdin

let _ = if not !Sys.interactive then readfiles ()
