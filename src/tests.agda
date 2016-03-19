module tests where

 id : (A : Set) -> A -> A
 id A x = x

 apply : (A : Set) (B : A -> Set) -> ((x : A) -> B x) -> (a : A) -> B a 
 apply A B f a = f a

 data Bool : Set where 
  true : Bool
  false : Bool

 not : Bool -> Bool
 not true = false
 not false = true

 data Nat : Set where
  zero : Nat
  suc : Nat -> Nat

 _+_ : Nat -> Nat -> Nat
 zero + m = m
 suc n + m = suc (n + m)

