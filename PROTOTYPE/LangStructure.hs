module LangStructure
( Name
, Args
, CommandLine(..)
, Block(..)
, Import(..)
, InOut(..)
--, Filter(..)
, Script(..)
) where

type Name = String
type Args = (Int,String)
type Path = String
type Alias = String
type Type = String
type Entity = String

data InOut = Read | Write | Append deriving Show
type File = (InOut,Name) 

data Import = Import Path Alias deriving Show
data Block = Filter Name [Args] [CommandLine] | Io Name [Args] [CommandLine] deriving Show
data Script = Script [Import] [Block] | Err String deriving Show
data CommandLine = CommandLine [File] [String] deriving Show
