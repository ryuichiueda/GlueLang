module LangStructure
( Name
, Args
, Statement(..)
, Block(..)
, Import(..)
--, Filter(..)
, Script(..)
) where

type Name = String
type Args = (Int,String)
type Path = String
type Alias = String

data Import = Import Path Alias deriving Show
data Block = Filter Name [Args] [Statement] | Procedure Name [Args] [Statement] deriving Show
data Script = Script [Import] [Block] | Err String deriving Show
data Statement = Statement [String] deriving Show
