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
type Io = (InOut,Name) 

data Import = Import Path Alias deriving Show
data Block = Filter Name [Args] [CommandLine] | Io Name [Args] [SubBlock] deriving Show
data SubBlock = IfBlock CommandLine [CommandLine] | SubBlock [CommandLine] deriving Show
data Script = Script [Import] [Block] | Err String deriving Show
data CommandLine = CommandLine [Io] [String] | Heredoc Io String deriving Show
