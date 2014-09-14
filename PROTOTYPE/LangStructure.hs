module LangStructure
( Name
, Args
, CommandLine(..)
, Block(..)
, Import(..)
, InOut(..)
, SubBlock(..)
, Script(..)
) where

type Name = String
type Args = (Int,String)
type Path = String
type Alias = String
type Type = String
type Entity = String

data InOut = Read | Write | Append | Str deriving Show
type Proc = (InOut,Name) 

data Import = Import Path Alias deriving Show
data Block = Func Name [Args] [CommandLine] | Proc Name [Args] [SubBlock] deriving Show
data SubBlock = IfBlock CommandLine [CommandLine] | SubBlock [CommandLine] deriving Show
data Script = Script [Import] [Block] | Err String deriving Show
data CommandLine = CommandLine [Proc] [String] | Heredoc Proc String deriving Show
