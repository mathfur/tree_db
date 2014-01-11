module Foo where

mySum :: [Int] -> Int
mySum [x] = x
mySum (x : xs) = x + mySum xs
mySum _ = 0
