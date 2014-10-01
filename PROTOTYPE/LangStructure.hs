module LangStructure
( Name
, Args
, CommandLine(..)
, SubCmd(..)
, Import(..)
, InOut(..)
, IoObj
, SubSubCmd(..)
, Script(..)
) where

type Name = String
type Args = (Int,String)
type Path = String
type Alias = String
type Type = String
type Entity = String

data InOut = Read | Write | Append | Str deriving Show
type IoObj = (InOut,Name) 

data Import = Import Path Alias deriving Show
data SubCmd = Func Name [Args] [CommandLine] | Proc Name [Args] [SubSubCmd] | Test Name [Args] [SubSubCmd] deriving Show
data SubSubCmd = IfSubCmd CommandLine [CommandLine] | SubSubCmd [CommandLine] deriving Show
data Script = Script [Import] [SubCmd] | Err String deriving Show
data CommandLine = CommandLine [IoObj] [String] | Heredoc IoObj String deriving Show
