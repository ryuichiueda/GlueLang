module LangStructure
( Name
, Args
, Code
, Block(..)
, Import(..)
--, Filter(..)
, Script(..)
) where

type Name = String
type Args = (Int,String)
type Code = String
type Path = String
type Alias = String

data Import = Import Path Alias deriving Show
data Block = Filter Name [Args] [Code] | Procedure Name [Args] [Code] deriving Show
data Script = Script [Import] [Block] | Err String deriving Show
