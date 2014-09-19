module LangStructure
( Name
, Args
, CommandLine(..)
, InlineCmd(..)
, Import(..)
, InOut(..)
, IoObj
, SubInlineCmd(..)
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
data InlineCmd = Func Name [Args] [CommandLine] | Proc Name [Args] [SubInlineCmd] deriving Show
data SubInlineCmd = IfInlineCmd CommandLine [CommandLine] | SubInlineCmd [CommandLine] deriving Show
data Script = Script [Import] [InlineCmd] | Err String deriving Show
data CommandLine = CommandLine [IoObj] [String] | Heredoc IoObj String deriving Show
